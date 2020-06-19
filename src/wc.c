/* wc: Report line word and byte counts for given files */

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
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

#include "wc.h"

char progname[256] = "";

struct option opts[] = {
        {"bytes",   no_argument, 0, 'c'},
        {"lines",   no_argument, 0, 'l'},
        {"words",   no_argument, 0, 'w'},
        {"help",    no_argument, 0, 'H'},
        {"version", no_argument, 0, 'V'}
};

int wc (const char *fname, int flags);
void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int flags = 0;
        int opt = 0;
        while ((opt = getopt_long(argc, argv, "clwHV", opts, NULL)) != -1) {
                switch (opt) {
                case 'c':
                        flags |= WC_BYTES;
                        break;

                case 'l':
                        flags |= WC_LINES;
                        break;

                case 'w':
                        flags |= WC_WORDS;
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
                        fprintf(stderr, "Try '%s --help' for more"
                                        " information\n", progname);
                        exit(EXIT_FAILURE);
                        break;
                }
        }

        if (flags == 0) {
                flags = WC_ALL_FLAGS;
        }

        if (argv[optind] == NULL) {
                wc("-", flags);
        } else {
                for (int i = optind; argv[i] != NULL; i++) {
                        wc(argv[i], flags);
                }
        }
}

int wc (const char *fname, int flags)
{
        int fd;
        fd = strcmp(fname, "-") == 0 ? STDIN_FILENO : open(fname, O_RDONLY);

        if (fd == -1) {
                fprintf(stderr, "%s: '%s': ", progname, fname);
                perror("");
                return -1;
        }

        size_t byte_count = 0;
        size_t word_count = 0;
        size_t line_count = 0;

        _Bool line_empty = 1;
        _Bool saw_space = 0;

        int ch = 0;
        while (read(fd, &ch, 1) > 0) {
                if (ch == '\n') {
                        line_count += 1;
                        word_count += line_empty || saw_space ? 0 : 1;
                        line_empty = 1;
                } else if (isspace(ch) && !line_empty && !saw_space) {
                        word_count += 1;
                        saw_space = 1;
                        saw_space = 1;
                } else if (!isspace(ch)) {
                        line_empty = 0;
                        saw_space = 0;
                }

                byte_count += 1;
        }

        if (CHKF_WC_ALL_FLAGS(flags))
                printf("  ");

        if (CHKF_WC_LINES(flags))
                printf("%ld  ", line_count);


        if (CHKF_WC_WORDS(flags))
                printf("%ld  ", word_count);

        if (CHKF_WC_BYTES(flags))
                printf("%ld  ", byte_count);

        if (strcmp(fname, "-") != 0)
                printf("  %s", fname);

        puts("");
        close(fd);
        return 0;
}

void help (void)
{
        printf("Usage: %s [OPTION] FILE...\n", progname);

        printf("Options:\n"
               "\t-c, --bytes\tPrint byte count\n"
               "\t-l, --lines\tPrint line count\n"
               "\t-w, --words\tPrint word count\n"
               "\t-h, --help\tShow this help message and exit\n"
               "\t-V, --version\tShow version information and exit\n");
}

void version (void)
{
        printf("%s (ClaireUTILS) v0.1\n", progname);

        printf("Copyright (C) 2020 Jithin Renji\n"
"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n""This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n\n"
"Written by Jithin Renji.\n");
}
