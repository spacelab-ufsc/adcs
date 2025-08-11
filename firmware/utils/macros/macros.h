/*
 * macros.h
 *
 * Copyright The SLCam Contributors.
 *
 * This file is part of SLCam.
 *
 * SLCam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SLCam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SLCam. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief Utility macros.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.0.0
 *
 * \date 2025/01/12
 *
 * \defgroup macros Macros
 * \ingroup utils
 * \{
 */

#ifndef MACROS_H_
#define MACROS_H_

/* Utility Macros (the `_` is used to avoid collisions with already defined common macros */
#define _BIT(x) (1ULL << (x))
#define _ARRAY_SIZE(arr) ((sizeof((arr))) / (sizeof((arr)[0])))
#define READ_BIT(reg, bit) (((reg) & (bit)) != 0)
#define SET_BIT(reg, bit) ((reg) |= (bit))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(bit))

/* Stringfy macros */
#define STRINGZ(x) #x
#define ASTRINGZ(x) STRINGZ(x)

/* GCC extension macros */
#ifdef __GNUC__
#define __unused __attribute__((unused))
#define __always_inline __attribute__((always_inline))
#define __no_inline __attribute__((noinline))
#define container_of(ptr, type, member) ((type *)(void *)((char *)(ptr) - offsetof(type, member)))
#else
#define __unused
#define __always_inline
#define __no_inline
#endif

#endif

/** \} End of macros group */
