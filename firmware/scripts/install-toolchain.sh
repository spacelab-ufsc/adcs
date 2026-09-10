#!/usr/bin/env bash
# Downloads and builds the RTEMS 6.2 arm-rtems6 toolchain + stm32f4 BSP via
# the RTEMS Source Builder, following:
# https://ftp.rtems.org/pub/rtems/releases/6/6.2/docs/html/user/start/index.html

set -euo pipefail

RTEMS_VERSION="6.2"
FIRMWARE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PREFIX="${RTEMS_PREFIX:-$FIRMWARE_DIR/toolchain}"

for cmd in curl tar python3; do
    command -v "$cmd" >/dev/null || { echo >&2 "$cmd not found. Aborting."; exit 1; }
done

python3 -c "import sysconfig,os,sys; sys.exit(0 if os.path.exists(os.path.join(sysconfig.get_path('include'),'Python.h')) else 1)" \
    || { echo >&2 "Python.h not found. Install your distro's python3 development package. Aborting."; exit 1; }

WORK_DIR="$(mktemp -d)"
trap 'status=$?; [ $status -eq 0 ] && rm -rf "$WORK_DIR" || echo >&2 "Build failed, logs kept at: $WORK_DIR"' EXIT

mkdir -p "$PREFIX"

cd "$WORK_DIR"
curl -L "https://ftp.rtems.org/pub/rtems/releases/6/${RTEMS_VERSION}/sources/rtems-source-builder-${RTEMS_VERSION}.tar.xz" | tar xJf -
mv "rtems-source-builder-${RTEMS_VERSION}" rsb

cd rsb/rtems
../source-builder/sb-set-builder --prefix="$PREFIX" 6/rtems-arm
../source-builder/sb-set-builder --prefix="$PREFIX" --target=arm-rtems6 \
    --with-rtems-bsp=arm/stm32f4 --with-rtems-tests=yes 6/rtems-kernel

echo "RTEMS ${RTEMS_VERSION} arm-rtems6/stm32f4 toolchain installed at: $PREFIX"
