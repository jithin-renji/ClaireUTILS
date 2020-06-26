/* uname: Print system information */

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>
#include <sys/utsname.h>

#include "uname.h"

char progname[256] = "";

struct option opts[] = {
        {"all",                no_argument, 0, 'a'},
        {"sys-name",           no_argument, 0, 's'},
        {"node-name",          no_argument, 0, 'n'},
        {"release",            no_argument, 0, 'r'},
        {"kernel-version",     no_argument, 0, 'v'},
        {"machine",            no_argument, 0, 'm'},
        {"help",               no_argument, 0, 'H'},
        {"version",            no_argument, 0, 'V'},
        {0,                              0, 0,  0}
};

void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int flags = 0;
        int opt = 0;

        while ((opt = getopt_long(argc, argv, "asonrvmHV", opts, NULL)) != -1) {
                switch (opt) {
                case 'a':
                        flags |= ALL_FLAGS;
                        break;

                case 's':
                case 'o':
                        flags |= SHOW_SYSNAME;
                        break;

                case 'n':
                        flags |= SHOW_NODENAME;
                        break;

                case 'r':
                        flags |= SHOW_RELEASE;
                        break;

                case 'v':
                        flags |= SHOW_VERSION;
                        break;

                case 'm':
                        flags |= SHOW_MACHINE;
                        break;

                case 'H':
                        help();
                        exit(EXIT_SUCCESS);
                        break;

                case 'V':
                        version();
                        exit(EXIT_SUCCESS);
                        break;

                default:
                        fprintf(stderr,
                                "Try '%s --help' for more information\n",
                                progname);
                        exit(EXIT_FAILURE);
                }
        }

        if (flags == 0)
                flags = SHOW_SYSNAME;

        struct utsname uname_buf;
        uname(&uname_buf);

        if (CHKF_SHOW_SYSNAME(flags))
                printf("%s ", uname_buf.sysname);

        if (CHKF_SHOW_NODENAME(flags))
                printf("%s ", uname_buf.nodename);

        if (CHKF_SHOW_RELEASE(flags))
                printf("%s ", uname_buf.release);

        if (CHKF_SHOW_VERSION(flags))
                printf("%s ", uname_buf.version);

        if (CHKF_SHOW_MACHINE(flags))
                printf("%s ", uname_buf.machine);

        puts("");
}

void help (void)
{
        printf("Usage: %s [OPTION]\n\n", progname);

        printf("Options:\n"
               "\t-a, --all\t\tPrint all the information in the following order\n"
               "\t-s, -o, --sysname\tPrint the operating system's name\n"
               "\t-n, --node-name\t\tPrint the name of the system\n"
               "\t-r, --release\t\tPrint kernel release information\n"
               "\t-v, --kernel-version\tPrint kernel version information\n"
               "\t-m, --machine\t\tPrint machine type\n"
               "\t-h, --help\t\tShow this help message and exit\n"
               "\t-V, --version\t\tShow version information and exit\n");
}

void version (void)
{
        printf("%s (ClaireUTILS) v0.1\n", progname);

        printf("Copyright (C) 2020 Jithin Renji\n"
"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n\n"
"Written by Jithin Renji.\n");
}
