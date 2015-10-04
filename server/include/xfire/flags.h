/*
 *  Binary operations
 *  Copyright (C) 2015   Michel Megens <dev@michelmegens.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @addtogroup bitops
 * @{
 */

#ifndef __BITOPS_H__
#define __BITOPS_H__

#include <xfire/xfire.h>
#include <xfire/os.h>

/**
 * @brief Atomic flags data structure.
 */
typedef struct {
	unsigned long flags; //!< Flags register.
	xfire_spinlock_t lock; //!< Flags lock.
} atomic_flags_t;

CDECL
extern int test_bit(int nr, atomic_flags_t *atom);
extern void set_bit(int nr, atomic_flags_t *atom);
extern void clear_bit(int nr, atomic_flags_t *atom);
extern int test_and_clear_bit(int nr, atomic_flags_t *atom);
extern int test_and_set_bit(int nr, atomic_flags_t *atom);
extern void swap_bit(int nr, atomic_flags_t *atom1, atomic_flags_t *atom2);
extern int test_and_swap_bit(int, atomic_flags_t *, atomic_flags_t *);

extern void atomic_flags_copy(atomic_flags_t *dst, atomic_flags_t *src);
extern void atomic_flags_destroy(atomic_flags_t *atom);
extern void atomic_flags_init(atomic_flags_t *atom);
CDECL_END

#endif

/** @} */

