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

#define FILES_PER_LINE		3

/* Flags */
#define LIST_LONG		1 << 0
#define LIST_ALL		2 << 0
#define LIST_COLORED		3 << 0

#endif	/* LS_H */
