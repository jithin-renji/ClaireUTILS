/* touch: Change file timestamps */

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
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <getopt.h>

#include "touch.h"

char progname[256];

struct option long_opts[] = {
        {"atime-only",     no_argument, 0, 'a'},
        {"mtime-only",     no_argument, 0, 'm'},
        {"no-dereference", no_argument, 0, 'h'},
        {"no-create",      no_argument, 0, 'c'},
        {"help",           no_argument, 0, 'H'},
        {"version",        no_argument, 0, 'v'},
        {0,                0,           0,  0}
};

/* Use utimensat() */

int touch (const char *fname, int flags);
void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int flags = 0;
        int opt = 0;
        while ((opt = getopt_long(argc, argv, "amhcHv", long_opts, NULL)) != -1) {
                switch (opt) {
                case 'a':
                        flags |= ATIME_ONLY;
                        break;

                case 'm':
                        flags |= MTIME_ONLY;
                        break;

                case 'c':
                        flags |= NO_CREATE;
                        break;

                case 'h':
                        flags |= NO_DEREFERENCE;
                        break;

                case 'H':
                        help();
                        exit(EXIT_SUCCESS);
                        break;

                case 'v':
                        version();
                        exit(EXIT_FAILURE);
                        break;

                default:
                        fprintf(stderr, "Try `%s --help` for more information.\n", progname);
                        exit(EXIT_FAILURE);
                        break;
                }
        }

        if (argv[optind] == NULL) {
                fprintf(stderr, "%s: Missing operand\n", progname);
                fprintf(stderr, "Try `%s --help` for more information.\n", progname);
                exit(EXIT_FAILURE);
        }

        int i = optind;
        while (i < argc) {
                touch(argv[i], flags);
                i += 1;
        }
}

int touch (const char *fname, int flags)
{
        int open_flags = 0;

        /* Default file creation mode: -rw-rw-r-- , ie. 664 */
        mode_t mode = 0664;
        int fd = -1;
        int utimensat_flags = 0;

        if (!CHKF_NO_CREATE(flags)) {
                open_flags |= O_CREAT;
        }

        fd = open(fname, open_flags, mode);
        if (fd == -1 && errno != ENOENT) {
                fprintf(stderr, "%s: ", progname);
                perror(fname);
                return -1;
        }
        close(fd);

        if (CHKF_NO_DEREFERENCE(flags)) {
                utimensat_flags |= AT_SYMLINK_NOFOLLOW;
        }

        struct timespec times[2] = {
                {.tv_nsec = UTIME_NOW},
                {.tv_nsec = UTIME_NOW}
        };

        if (CHKF_ATIME_ONLY(flags)) {
                times[1].tv_nsec = UTIME_OMIT;
        } else if (CHKF_MTIME_ONLY(flags)) {
                times[0].tv_nsec = UTIME_OMIT;
        }

        int ret = utimensat(AT_FDCWD, fname, times, utimensat_flags);
        if (ret == -1) {
                fprintf(stderr, "%s: ", progname);
                perror(fname);
                return -1;
        }

        return 0;
}

void help (void)
{
        printf("Usage: %s [FILE]...\n", progname);
        printf("   or: %s OPTION\n\n", progname);

        printf("Options:\n"
               "\t-a, --atime-only\tOnly change the access time\n"
               "\t-m, --mtime-only\tOnly change the modification time\n"
               "\t-c, --no-create\t\tDo not create new file if FILE does not exist\n"
               "\t-h, --no-dereference\t\tDo not follow symbolic links\n"
               "\t-H, --help\t\tShow this help message and exit\n"
               "\t-v, --version\t\tShow version information and exit\n");
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
