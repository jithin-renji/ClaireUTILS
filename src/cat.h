/* cat.h: Defintions for the `cat` program */

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

#ifndef CAT_H
#define CAT_H

/* Flags */
#define SHOW_ENDS       1

/* Check flag macros */
#define CHKF_SHOW_ENDS(flags) ((flags & SHOW_ENDS) == SHOW_ENDS)

#endif	/* CAT_H */
