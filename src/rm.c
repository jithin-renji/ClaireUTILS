/* rm: Remove files and/or directories */

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
#include <getopt.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "rm.h"

/* Remove files and/or empty directories */
int rm (const char *fname, int flags);

/* This function is called when deleting
 * directories recursively */
int rm_recursive(const char *fname);

void help (const char *progname);
void version (const char *progname);

struct option long_opt[] = {
        {"interactive", no_argument, 0, 'i'},
        {"force",       no_argument, 0, 'f'},
        {"recursive",   no_argument, 0, 'r'},
        {"dir",         no_argument, 0, 'd'},
        {"verbose",     no_argument, 0, 'v'},
        {"version",     no_argument, 0, 'V'},
        {"help",        no_argument, 0, 'h'},

};

int main (int argc, char **argv)
{
        if (argc < 2) {
                fprintf(stderr, "%s: Missing operand\n", argv[0]);
                fprintf(stderr, "Try `%s --help` for more information.\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        int flags = 0;
        int opt = 0;

        while ((opt = getopt_long(argc, argv, "drfihV", long_opt, NULL)) != -1) {
                switch (opt) {
                case 'd':
                        flags |= RM_EMPTY_DIRS;
                        break;
                case 'r':
                        flags |= RM_RECURSIVE;
                        break;

                case 'f':
                        flags |= RM_FORCE;
                        break;

                case 'i':
                        flags |= RM_INTERACTIVE;
                        break;

                case 'h':
                        help(argv[0]);
                        exit(EXIT_SUCCESS);
                        break;

                case 'V':
                        version(argv[0]);
                        exit(EXIT_SUCCESS);
                        break;

                default:
                        printf("Try `%s --help` for more information.\n", argv[0]);
                        exit(EXIT_FAILURE);
                }
        }

        if (argv[optind] == NULL) {
                printf("%s: Missing operand\n", argv[0]);
                printf("Try `%s --help` for more information.\n", argv[0]);
        } else {
                char **fnames_ptr = argv + optind;
                while (*fnames_ptr != NULL) {
                        rm(*fnames_ptr, flags);
                        ++fnames_ptr;
                }
        }
}

int rm (const char *fname, int flags)
{
        int unlink_flags = 0;

        if (CHKF_RECURSIVE(flags) || CHKF_EMPTY_DIRS(flags)) {
                if (strcmp(fname, "/") == 0) {
                        fprintf(stderr, "Refusing to remove `/`\n");
                        return -1;
                }

                unlink_flags = AT_REMOVEDIR;
        }

        int ret_val = 0;
        char resp[3] = "y";
        if (!CHKF_FORCE(flags)) {
                if (CHKF_INTERACTIVE(flags)) {
                        printf("Remove '%s'? ", fname);

                        char *ret_str = fgets(resp, 3, stdin);
                        if (ret_str == NULL) {
                                resp[0] = 'n';
                        }
                }
        }

        if (CHKF_RECURSIVE(flags)) {
                ret_val = rm_recursive(fname);
        } else if (resp[0] == 'y' || resp[0] == 'Y') {
                ret_val = unlinkat(AT_FDCWD, fname, unlink_flags);
        
                if (ret_val == -1) {
                        if (!CHKF_FORCE(flags))
                                perror(fname);
                }
        }

        return ret_val;
}

int rm_recursive (const char *fname)
{
        DIR *dir_stream = opendir(fname);
        int ret_val = 0;
        char fpath[2048] = "";

        strcpy(fpath, fname);

        if (dir_stream != NULL) {
                struct dirent *dp;
                while ((dp = readdir(dir_stream)) != NULL) {
                        struct stat stat_buf;

                        if (strcmp(dp->d_name, ".") == 0 ||
                            strcmp(dp->d_name, "..") == 0) {
                                continue;
                        }

                        strcat(fpath, "/");
                        strcat(fpath, dp->d_name);
                        stat(fpath, &stat_buf);

                        mode_t mode = stat_buf.st_mode;

                        if (S_ISDIR(mode)) {
                                ret_val = unlinkat(AT_FDCWD, fpath, AT_REMOVEDIR);

                                if (ret_val == -1 && errno != ENOTEMPTY && errno != ENOTDIR) {
                                        perror(fpath);
                                } else if (ret_val == -1 && errno == ENOTEMPTY) {
                                        rm_recursive(fpath);
                                } else if (ret_val == -1 && errno == ENOTDIR) {
                                        ret_val = unlinkat(AT_FDCWD, fpath, 0);
                                }
                        } else {
                                ret_val = unlinkat(AT_FDCWD, fpath, 0);
                                if (ret_val == -1) {
                                        perror(fpath);
                                }
                        }

                        strcpy(fpath, fname);
                }

                ret_val = unlinkat(AT_FDCWD, fpath, AT_REMOVEDIR);
                if (ret_val == -1) {
                        perror(fpath);
                }
        } else {
                ret_val = unlinkat(AT_FDCWD, fpath, 0);
                if (ret_val == -1) {
                        perror(fpath);
                }
        }

        closedir(dir_stream);

        return ret_val;
}

void version (const char *progname)
{
        printf("%s (ClaireUTILS) v0.1\n", progname);

        printf("Copyright (C) 2020 Jithin Renji\n"
"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n\n"
"Written by Jithin Renji.\n");
}

void help (const char *progname)
{
        printf("Usage: %s [OPTION]... [FILE]...\n\n", progname);

        printf("Options:\n");
        printf("\t-i, --interactive\tAsk before removing\n"
               "\t-f, --force\tRemove files without prompting; don't complain about non-existent files\n"
               "\t-r, --recursive\tRemove directories recursively\n"
               "\t-d, --dir\tRemove empty directories\n"
               "\t-v, --verbose\tDisplay the names of the files that are being deleted\n"
               "\t-V, --version\tDisplay version information and exit\n"
               "\t-h, --help\tShow this help message and exit\n");
}
