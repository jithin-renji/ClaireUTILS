/* wc.h: Definitions for the `wc` program. */

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

#ifndef WC_H
#define WC_H

/* Flags */
#define WC_BYTES              1
#define WC_WORDS              1 << 1
#define WC_LINES              1 << 2

#define WC_ALL_FLAGS          WC_LINES | WC_WORDS | WC_BYTES

/* Check flag macros */
#define CHKF_WC_BYTES(flags) ((flags & WC_BYTES) == WC_BYTES)
#define CHKF_WC_WORDS(flags) ((flags & WC_WORDS) == WC_WORDS)
#define CHKF_WC_LINES(flags) ((flags & WC_LINES) == WC_LINES)

#define CHKF_WC_ALL_FLAGS(flags) ((flags & (WC_ALL_FLAGS)) == (WC_ALL_FLAGS))

#endif	/* PROG_H */
