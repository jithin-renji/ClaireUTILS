/* uname.h: Defintions for the `uname` program */

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

#ifndef UNAME_H
#define UNAME_H

/* Flags */
#define SHOW_SYSNAME         1
#define SHOW_NODENAME        1 << 1
#define SHOW_RELEASE         1 << 2
#define SHOW_VERSION         1 << 3
#define SHOW_MACHINE         1 << 4

#define ALL_FLAGS            SHOW_SYSNAME | SHOW_NODENAME | SHOW_RELEASE | \
                             SHOW_VERSION | SHOW_MACHINE

/* Check flag macros */
#define CHKF_SHOW_SYSNAME(flags) ((flags & SHOW_SYSNAME) == SHOW_SYSNAME)
#define CHKF_SHOW_NODENAME(flags) ((flags & SHOW_NODENAME) == SHOW_NODENAME)
#define CHKF_SHOW_RELEASE(flags) ((flags & SHOW_RELEASE) == SHOW_RELEASE)
#define CHKF_SHOW_VERSION(flags) ((flags & SHOW_VERSION) == SHOW_VERSION)
#define CHKF_SHOW_MACHINE(flags) ((flags & SHOW_MACHINE) == SHOW_MACHINE)

#endif /* UNAME_H */
