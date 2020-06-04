/* ls.h: Defintions for the `ls` program */

/*
 *	Copyright (C) 2020 Jithin Renji
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * */

#ifndef LS_H
#define LS_H

/* Flags */
#define LS_LONG			1
#define LS_ALL			1 << 1
#define LS_ALL_NOT_DODD		1 << 2
#define LS_COLORED		1 << 3

#define CHKF_LONG(flags) ((flags & LS_LONG) == LS_LONG)
#define CHKF_ALL(flags) ((flags & LS_ALL) == LS_ALL)
#define CHKF_ALL_NOT_DODD(flags) ((flags & LS_ALL_NOT_DODD) == LS_ALL_NOT_DODD)
#define CHKF_COLORED(flags) ((flags & LS_COLORED) == LS_COLORED)

#endif	/* LS_H */
