/* cp.h: Definitions for the `cp` program. */

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

#ifndef CP_H
#define CP_H

/* Flags */
#define CP_RECURSIVE            1
#define CP_VERBOSE              1 << 1

/* Check flag macros */
#define CHKF_CP_RECURSIVE(flags) ((flags & CP_RECURSIVE) == CP_RECURSIVE)
#define CHKF_CP_VERBOSE(flags) ((flags & CP_VERBOSE) == CP_VERBOSE)

#endif	/* PROG_H */
