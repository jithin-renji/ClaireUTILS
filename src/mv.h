/* mv.h: Definitions for the `mv` program. */

/*
 *      Copyright (C) 2020 Jithin Renji
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * */

#ifndef MV_H
#define MV_H

/* Flags */
#define MV_VERBOSE              1
#define MV_FORCE                1 << 1

/* Check flag macros */
#define CHKF_MV_VERBOSE(flags) ((flags & MV_VERBOSE) == MV_VERBOSE)
#define CHKF_MV_FORCE(flags) ((flags & MV_FORCE) == MV_FORCE)

#endif	/* PROG_H */
