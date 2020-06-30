/* id.h: Definitions for the `id` program. */

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

#ifndef ID_H
#define ID_H

/* Flags */
#define SHOW_UID        1
#define SHOW_GID        1 << 1
#define SHOW_GIDS       1 << 2

#define SHOW_NAME       1 << 3

/* Check flags macros */
#define CHKF_SHOW_UID(flags) ((flags & SHOW_UID) == SHOW_UID)
#define CHKF_SHOW_GID(flags) ((flags & SHOW_GID) == SHOW_GID)
#define CHKF_SHOW_GIDS(flags) ((flags & SHOW_GIDS) == SHOW_GIDS)

#define CHKF_SHOW_NAME(flags) ((flags & SHOW_NAME) == SHOW_NAME)

#endif	/* ID_H */
