/* touch.h: Defintions for the `touch` program */

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

#ifndef TOUCH_H
#define TOUCH_H

/* Flags */
#define ATIME_ONLY      1
#define MTIME_ONLY      1 << 1
#define NO_CREATE       1 << 2
#define NO_DEREFERENCE  1 << 3

/* Check flags macros */
#define CHKF_ATIME_ONLY(flags) ((flags & ATIME_ONLY) == ATIME_ONLY)
#define CHKF_MTIME_ONLY(flags) ((flags & MTIME_ONLY) == MTIME_ONLY)
#define CHKF_NO_CREATE(flags) ((flags & NO_CREATE) == NO_CREATE)
#define CHKF_NO_DEREFERENCE(flags) ((flags & NO_DEREFERENCE) == NO_DEREFERENCE)

#endif  /* TOUCH_H */
