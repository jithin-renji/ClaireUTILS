/* rm.h: Defintions for the `rm` program */

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

#ifndef RM_H
#define RM_H

/* Flags */
#define RM_EMPTY_DIRS           1
#define RM_RECURSIVE            1 << 1
#define RM_FORCE                1 << 2
#define RM_INTERACTIVE          1 << 3

/* Check flag macros */
#define CHKF_EMPTY_DIRS(flags) ((flags & RM_EMPTY_DIRS) == RM_EMPTY_DIRS)
#define CHKF_RECURSIVE(flags) ((flags & RM_RECURSIVE) == RM_RECURSIVE)
#define CHKF_FORCE(flags) ((flags & RM_FORCE) == RM_FORCE)
#define CHKF_INTERACTIVE(flags) ((flags & RM_INTERACTIVE) == RM_INTERACTIVE)

#endif	/* RM_H */
