/* mkdir.h: Defintions for the `mkdir` program */

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

#ifndef MKDIR_H
#define MKDIR_H

/* Flags */
#define MK_PARENTS              1
#define MK_VERBOSE              1 << 1

/* Check flag macros */
#define CHKF_MK_PARENTS(flags) ((flags & MK_PARENTS) == MK_PARENTS)
#define CHKF_MK_VERBOSE(flags) ((flags & MK_VERBOSE) == MK_VERBOSE)

#endif	/* MKDIR_H */
