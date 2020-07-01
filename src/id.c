/* id: Return user identity */

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

#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>
#include <pwd.h>
#include <grp.h>

#include "id.h"

char progname[256] = "";

struct option long_opts[] = {
        {"user",    no_argument, 0, 'u'},
        {"group",   no_argument, 0, 'g'},
        {"groups",  no_argument, 0, 'G'},
        {"name",    no_argument, 0, 'n'},
        {"help",    no_argument, 0, 'H'},
        {"version", no_argument, 0, 'V'},
        {0,         0,           0,  0 }
};

int show_id (int flags);
void help (void);
void version (void);

static inline void exit_if_ugG_set (_Bool ugG_set)
{
        if (ugG_set) {
                fprintf(stderr,
                        "%s: Wrong usage\n"
                        "Try '%s --help' for more information\n",
                        progname, progname);
                exit(EXIT_FAILURE);
        }
}

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        /* Flag to check if any on of -ugG was set */
        _Bool ugG_set = 0;
        int flags = 0;
        int opt = 0;
        while ((opt = getopt_long(argc, argv,
                                 "augGnHV", long_opts, NULL)) != -1) {
                switch (opt) {
                case 'a':
                        /* Ignored for compatability with other
                         * implementations */
                        break;

                case 'u':
                        exit_if_ugG_set(ugG_set);
                        flags |= SHOW_UID;
                        ugG_set = 1;
                        break;

                case 'g':
                        exit_if_ugG_set(ugG_set);
                        flags |= SHOW_GID;
                        ugG_set = 1;
                        break;

                case 'G':
                        exit_if_ugG_set(ugG_set);
                        flags |= SHOW_GIDS;
                        ugG_set = 1;
                        break;

                case 'n':
                        flags |= SHOW_NAME;
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
                                "Try `%s --help` for more information.\n",
                                progname);
                        exit(EXIT_FAILURE);
                        break;
                }
        }

        
        if (!ugG_set && CHKF_SHOW_NAME(flags)) {
                fprintf(stderr,
                        "%s: Please at least one among -ugG\n"
                        "Try '%s --help' for more information\n",
                        progname, progname);
                exit(EXIT_FAILURE);
        }

        show_id(flags);

        return 0;
}

int show_id (int flags)
{
        uid_t ruid = getuid();
        uid_t euid = geteuid();

        struct passwd *pwd = getpwuid(ruid);

        gid_t egid = getegid();
        struct group *grp = getgrgid(egid);

        gid_t groups[1024];
        int ngroups = 1024;
        int ret = getgrouplist(pwd->pw_name, egid, groups, &ngroups);

        if (ret == -1) {
                fprintf(stderr,
                        "%s: User is part of too many groups!\n",
                        progname);

                return -1;
        }

        if (flags == 0) {
                printf("uid=%u(%s) gid=%u(%s) ",
                       ruid, pwd->pw_name, egid, grp->gr_name);

                printf("groups=");
                struct group *sgrp = NULL;
                for (int i = 0; i < ngroups; i++) {
                        sgrp = getgrgid(groups[i]);
                        printf("%u(%s)", groups[i],  sgrp->gr_name);
                        if (i != ngroups - 1) {
                                printf(",");
                        }
                }

                puts("");
        } else {
                if (CHKF_SHOW_UID(flags)) {
                        if (CHKF_SHOW_NAME(flags))
                                printf("%s\n", pwd->pw_name);
                        else
                                printf("%u\n", euid);
                } else if (CHKF_SHOW_GID(flags)) {
                        if (CHKF_SHOW_NAME(flags))
                                printf("%s\n", grp->gr_name);
                        else
                                printf("%u\n", egid);
                } else if (CHKF_SHOW_GIDS(flags)) {
                        for (int i = 0; i < ngroups; i++) {
                                struct group *sgrp = getgrgid(groups[i]);
                                if (CHKF_SHOW_NAME(flags))
                                        printf("%s", sgrp->gr_name);
                                else
                                        printf("%u", groups[i]);

                                if (i != ngroups - 1)
                                        printf(" ");
                        }

                        puts("");
                }
        }

        return 0;
}

void help (void)
{
        printf("Usage: %s [OPTION] [USERNAME]\n\n", progname);

        printf("Options:\n"
               "\t-a\t\tIgnored for compatability with other implementations\n"
               "\t-u, --user\tPrint effective user ID as a number\n"
               "\t-g, --group\tPrint the effective group ID as a number\n"
               "\t-G, --groups\tPrint all group IDs as numbers\n"
               "\t-n, --name\tPrint name(s) instead of number(s) for -ugG\n"
               "\t-H, --help\tShow this help message and exit\n"
               "\t-V, --version\tShow version information and exit\n");
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
