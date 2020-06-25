/* cat: ConCATenate files and print to standard output */

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
#include <fcntl.h>
#include <getopt.h>

#include "cat.h"

char progname[256] = "";

struct option opts[] = {
        {"show-ends",   no_argument, 0, 'E'},
        {"help",        no_argument, 0, 'H'},
        {"version",     no_argument, 0, 'V'},
        {0,             0,           0,  0}
};

int cat (const char *fname, int flags);
void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int flags = 0;
        int opt = 0;
        while ((opt = getopt_long(argc, argv, "EHV", opts, NULL)) != -1) {
                switch (opt) {
                case 'E':
                        flags |= SHOW_ENDS;
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

        if (argv[optind] == NULL) {
                cat("-", flags);
        } else {
                for (int i = optind; i < argc; i++) {
                        cat(argv[i], flags);
                }
        }
        return 0;
}

int cat (const char *fname, int flags)
{
        int fd;
        fd = strcmp(fname, "-") == 0 ? STDIN_FILENO : open(fname, O_RDONLY);

        if (fd == -1) {
                fprintf(stderr, "%s: '%s': ", progname, fname);
                perror("");
                return -1;
        }

        int ch = 0;
        const char *out_str = "$";
        while (read(fd, &ch, 1) > 0) {
                if (CHKF_SHOW_ENDS(flags) && ch == '\n')
                        write(STDOUT_FILENO, out_str, strlen(out_str));

                write(STDOUT_FILENO, &ch, 1);
        }

        return 0;
}

void help (void)
{
        printf("Usage: %s [FILENAME]...\n", progname);
        printf("   or: %s OPTION\n\n", progname);

        printf("Options:\n"
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
