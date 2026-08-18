import matplotlib.pyplot as plt
import numpy as np

# ---------------------------
# Parâmetros físicos e satélite
# ---------------------------
mu0 = 4 * np.pi * 1e-7  # [H/m]
Re = 6371e3  # raio da Terra [m]
muE = 3.986004418e14  # GM [m^3/s^2]

# **CORREÇÃO IMPORTANTE**: momento dipolar da Terra (A·m^2)
M_earth = np.array([0.0, 0.0, 7.96e22])  # [A·m^2]  (ordem correta)

# Inércia do satélite (exemplo)
I = np.diag([0.12, 0.10, 0.08])  # [kg·m^2]
Iinv = np.linalg.inv(I)

# Magnetorquer / controle
m_max = 0.25  # saturação [A·m^2] (valor típico)
# Ganho: vamos auto-ajustar e limitar a um valor conservador
k_bdot_auto_factor = 0.5  # fator de segurança para evitar saturação inicial
k_bdot_conservative_max = 5e4  # limite superior conservador para o ganho

# --------------------------------
# Órbita (circular equatorial) e tempo
# --------------------------------
altitude = 500e3
r_orbit = Re + altitude
v_orbit = np.sqrt(muE / r_orbit)
T_orbita = 2 * np.pi * r_orbit / v_orbit

dt = 0.5  # passo [s] (se quiser acelerar, aumente)
t_final = T_orbita  # 1 órbita completa
N = int(np.ceil(t_final / dt))

# Estado inicial
q = np.array([1.0, 0.0, 0.0, 0.0])  # quaternion (scalar-first) corpo->inercial
omega = np.array([0.08, -0.06, 0.05])  # rad/s (condição pós-liberação)


# ---------------------------
# Funções utilitárias
# ---------------------------
def skew(v):
    return np.array([[0, -v[2], v[1]], [v[2], 0, -v[0]], [-v[1], v[0], 0]])


def quat_norm(q):
    return q / np.linalg.norm(q)


def quat_mul(q1, q2):
    w1, x1, y1, z1 = q1
    w2, x2, y2, z2 = q2
    return np.array(
        [
            w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2,
            w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
            w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2,
            w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2,
        ]
    )


def q_dot_from_omega(q, omega_b):
    wq = np.array([0.0, omega_b[0], omega_b[1], omega_b[2]])
    return 0.5 * quat_mul(wq, q)


def quat_to_dcm_body_to_inertial(q):
    q0, q1, q2, q3 = q
    R = np.array(
        [
            [
                1 - 2 * (q2 * q2 + q3 * q3),
                2 * (q1 * q2 - q0 * q3),
                2 * (q1 * q3 + q0 * q2),
            ],
            [
                2 * (q1 * q2 + q0 * q3),
                1 - 2 * (q1 * q1 + q3 * q3),
                2 * (q2 * q3 - q0 * q1),
            ],
            [
                2 * (q1 * q3 - q0 * q2),
                2 * (q2 * q3 + q0 * q1),
                1 - 2 * (q1 * q1 + q2 * q2),
            ],
        ]
    )
    return R


def magnetic_field_dipole(r_eci):
    """Campo dipolar em ECI (Tesla)."""
    r = np.linalg.norm(r_eci)
    if r < 1e-9:
        return np.zeros(3)
    rhat = r_eci / r
    m = M_earth
    term = (3 * rhat * np.dot(m, rhat) - m) / (r**3)
    B = (mu0 / (4 * np.pi)) * term
    return B


def orbit_position_eci(t):
    """Órbita circular no plano equatorial (ECI)."""
    theta = 2 * np.pi * t / T_orbita
    return np.array([r_orbit * np.cos(theta), r_orbit * np.sin(theta), 0.0])


def saturate(vec, max_norm):
    n = np.linalg.norm(vec)
    return vec if n <= max_norm else vec * (max_norm / n)


# ---------------------------
# Auto-ajuste do ganho (para evitar saturação inicial)
# ---------------------------
# Estimativa do módulo do campo na órbita (posição x=r_orbit, y=0)
B_ref = np.linalg.norm(magnetic_field_dipole(np.array([r_orbit, 0.0, 0.0])))
omega0_norm = np.linalg.norm(omega)
if omega0_norm < 1e-12 or B_ref < 1e-20:
    k_bdot = 5e4
else:
    k_candidate = k_bdot_auto_factor * m_max / (omega0_norm * B_ref)
    k_bdot = min(k_candidate, k_bdot_conservative_max)

print(
    f"Config: altitude={altitude / 1e3:.0f} km, T_orbita={T_orbita / 60:.1f} min, B_ref={B_ref:.3e} T"
)
print(
    f"Auto k_bdot => {k_bdot:.3g} (candidato {('%.3g' % k_candidate) if 'k_candidate' in locals() else 'n/a'})"
)

# ---------------------------
# Filtragem para dB/dt (suavizar ruído)
# ---------------------------
tau_filter = 1.0  # constante de tempo do filtro exponencial [s]
alpha = dt / (tau_filter + dt)

# ---------------------------
# Histórico
# ---------------------------
t_hist = np.zeros(N)
w_hist = np.zeros((N, 3))
wn_hist = np.zeros(N)
m_hist = np.zeros((N, 3))
tau_hist = np.zeros((N, 3))
B_hist = np.zeros((N, 3))
dB_hist = np.zeros((N, 3))

B_body_prev = None
dB_dt_filt = np.zeros(3)

# ---------------------------
# Loop de simulação (RK4 para dinâmica + cinemática)
# ---------------------------
for k in range(N):
    t = k * dt

    r_eci = orbit_position_eci(t)
    B_eci = magnetic_field_dipole(r_eci)

    # Rotaciona B para o corpo: v_body = R_bi.T * v_eci
    R_bi = quat_to_dcm_body_to_inertial(q).T
    B_body = R_bi @ B_eci

    # Derivada discreta e filtrada
    if B_body_prev is None:
        dB_dt = np.zeros(3)
    else:
        dB_dt = (B_body - B_body_prev) / dt
    dB_dt_filt = dB_dt_filt + alpha * (dB_dt - dB_dt_filt)
    B_body_prev = B_body.copy()

    # B-dot control (com saturação do dipolo)
    m_cmd = -k_bdot * dB_dt_filt
    m_cmd = saturate(m_cmd, m_max)

    # Torque magnético
    tau = np.cross(m_cmd, B_body)

    # Dinâmica: I * domega = tau - omega x (I*omega)
    def f_omega(omega_local, tau_local):
        return Iinv @ (tau_local - np.cross(omega_local, I @ omega_local))

    # RK4 (dinâmica + cinemática quaternion)
    k1_w = f_omega(omega, tau)
    k1_q = q_dot_from_omega(q, omega)

    w2 = omega + 0.5 * dt * k1_w
    q2 = quat_norm(q + 0.5 * dt * k1_q)
    k2_w = f_omega(w2, tau)
    k2_q = q_dot_from_omega(q2, w2)

    w3 = omega + 0.5 * dt * k2_w
    q3 = quat_norm(q + 0.5 * dt * k2_q)
    k3_w = f_omega(w3, tau)
    k3_q = q_dot_from_omega(q3, w3)

    w4 = omega + dt * k3_w
    q4 = quat_norm(q + dt * k3_q)
    k4_w = f_omega(w4, tau)
    k4_q = q_dot_from_omega(q4, w4)

    omega = omega + (dt / 6.0) * (k1_w + 2 * k2_w + 2 * k3_w + k4_w)
    q = quat_norm(q + (dt / 6.0) * (k1_q + 2 * k2_q + 2 * k3_q + k4_q))

    # Salva histórico
    t_hist[k] = t
    w_hist[k] = omega
    wn_hist[k] = np.linalg.norm(omega)
    m_hist[k] = m_cmd
    tau_hist[k] = tau
    B_hist[k] = B_body
    dB_hist[k] = dB_dt_filt

# ---------------------------
# Plots
# ---------------------------
plt.figure(figsize=(8, 4))
plt.plot(t_hist / 60.0, wn_hist)
plt.xlabel("Tempo [min]")
plt.ylabel("||ω|| [rad/s]")
plt.title("Detumbling B-dot — norma da velocidade angular (1 órbita)")
plt.grid(True)

plt.figure(figsize=(8, 4))
plt.plot(t_hist / 60.0, w_hist[:, 0], label="ωx")
plt.plot(t_hist / 60.0, w_hist[:, 1], label="ωy")
plt.plot(t_hist / 60.0, w_hist[:, 2], label="ωz")
plt.xlabel("Tempo [min]")
plt.ylabel("ω [rad/s]")
plt.title("Componentes de ω (corpo)")
plt.legend()
plt.grid(True)

plt.figure(figsize=(8, 4))
plt.plot(t_hist / 60.0, np.linalg.norm(B_hist, axis=1))
plt.xlabel("Tempo [min]")
plt.ylabel("||B_body|| [T]")
plt.title("Módulo do campo magnético (corpo)")
plt.grid(True)

plt.figure(figsize=(8, 4))
plt.plot(t_hist / 60.0, m_hist[:, 0], label="mx")
plt.plot(t_hist / 60.0, m_hist[:, 1], label="my")
plt.plot(t_hist / 60.0, m_hist[:, 2], label="mz")
plt.xlabel("Tempo [min]")
plt.ylabel("m_cmd [A·m²]")
plt.title("Comando de dipolo (saturado)")
plt.legend()
plt.grid(True)

plt.show()

# Relatório rápido
print("ω inicial [rad/s]:", np.linalg.norm(np.array([0.08, -0.06, 0.05])))
print("ω final   [rad/s]:", wn_hist[-1])
