/* mkdir: Make new directories */

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
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "mkdir.h"

char progname[256] = "";

struct option long_opts[] = {
        {"parents",     no_argument, 0, 'p'},
        {"verbose",     no_argument, 0, 'v'},
        {"help",        no_argument, 0, 'h'},
        {"version",     no_argument, 0, 'V'},
        {0,             0,           0,  0}
};

int make_dir (const char *dname, int flags);
void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int flags = 0;
        int opt = 0;
        while ((opt = getopt_long(argc, argv, "pvhV", long_opts, NULL)) != -1) {
                switch (opt) {
                case 'p':
                        flags |= MK_PARENTS;
                        break;

                case 'v':
                        flags |= MK_VERBOSE;
                        break;

                case 'h':
                        help();
                        exit(EXIT_FAILURE);
                        break;

                case 'V':
                        version();
                        exit(EXIT_FAILURE);
                        break;

                default:
                        fprintf(stderr, "Try `%s --help` for more information\n", progname);
                        exit(EXIT_FAILURE);
                        break;
                }
        }

        if (argv[optind] == NULL) {
                fprintf(stderr, "%s: Missing operand\n", progname);
                fprintf(stderr, "Try `%s --help` for more information.\n", progname);
                exit(EXIT_FAILURE);
        }

        int ret = 0;
        for (int i = optind; argv[i] != NULL; i++) {
                int md_ret = make_dir(argv[i], flags);
                if (md_ret == -1)
                        ret = 1;
        }

        return ret;
}

int make_dir (const char *dname, int flags)
{
        if (!CHKF_MK_PARENTS(flags)) {
                int ret = mkdir(dname, 0775);
                if (ret == -1) {
                        fprintf(stderr, "%s: Could not create directory `%s`: ",
                                        progname, dname);
                        perror("");
                        return -1;
                }

                if (CHKF_MK_VERBOSE(flags)) {
                        printf("%s: Created directory `%s`\n", progname, dname);
                }
        } else {
                char dname_cpy[PATH_MAX] = "";
                char full_dpath[PATH_MAX] = "";

                strcpy(dname_cpy, dname);

                char *tok = strtok(dname_cpy, "/");
                while (tok != NULL) {
                        strcat(full_dpath, tok);
                        mkdir(full_dpath, 0775);

                        if (CHKF_MK_VERBOSE(flags)) {
                                printf("%s: Created directory `%s`\n", progname,
                                                full_dpath);
                        }

                        strcat(full_dpath, "/");
                        tok = strtok(NULL, "/");
                }
        }

        return 0;
}

void help (void)
{
        printf("Usage: %s [OPTION]... DIRECTORY...\n\n", progname);

        printf("Options:\n"
               "\t-p, --parents\tMake parent diretories if they don't exist\n"
               "\t-v, --verbose\tOutput a message for every directory that is being created\n"
               "\t-h, --help\tShow this help message and exit\n"
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
