/* yes: Repeatedly output a string, until killed */

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

char progname[256] = "";

struct option opts[] = {
        {"help",    no_argument, 0, 'H'},
        {"version", no_argument, 0, 'V'},
        {0,         0,           0,  0}
};

void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int opt = 0;
        while ((opt = getopt_long(argc, argv, "HV", opts, NULL)) != -1) {
                switch (opt) {
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
                        break;
                }
        }

        if (argv[optind] == NULL) {
                while (1) {
                        puts("y");
                }
        } else {
                while (1) {
                        for (int i = 1; i < argc; i++) {
                                printf("%s ", argv[i]);
                        }

                        fputc('\n', stdout);
                }
        }
        return 0;
}

void help (void)
{
        printf("Usage: %s [STRING]...\n", progname);
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
