/* cp: Copy files */

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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

#include <linux/limits.h>

#include "cp.h"
#include "linked_list.h"

char progname[256] = "";

struct option opts[] = {
        {"recursive",   no_argument, 0, 'r'},
        {"verbose",     no_argument, 0, 'v'},
        {"help",        no_argument, 0, 'H'},
        {"version",     no_argument, 0, 'V'},
        {0,             0,           0,  0}
};

void base_name (char *out, char *in_path);
int copy_file_to_file (char *in_file, char *out_file, int flags);
int copy_files_into_dir (struct node *files, char *dir_name,
                           int flags);

void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int flags = 0;
        int opt = 0;
        while ((opt = getopt_long(argc, argv, "rvHV", opts, NULL)) != -1) {
                switch (opt) {
                case 'r':
                        flags |= CP_RECURSIVE;
                        break;

                case 'v':
                        flags |= CP_VERBOSE;
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
                        break;
                }
        }

        if (argv[optind] == NULL || argc - optind < 2) {
                fprintf(stderr, "%s: Missing operand\n", progname);
                fprintf(stderr, "Try '%s --help' for more information\n",
                                progname);
                exit(EXIT_FAILURE);
        }


        /* The command is of the form cp FILE1 FILE2,
         *                         or cp FILE  DIRECTORY */
        if (argc - optind == 2) {
                copy_file_to_file(argv[optind], argv[optind + 1], flags);
        } else { /* cp FILE... DIRECTORY */
                char *dir_name = argv[argc - 1];
                struct node *files = malloc(sizeof(struct node));

                node_init(files);

                for (int i = optind; i < argc - 1; i++) {
                        list_append(files, argv[i]);
                }

                copy_files_into_dir(files, dir_name, flags);
                list_destroy(files);
        }

        return 0;
}

void base_name (char *out, char *in_path)
{
        char *inpath_ptr = in_path;
        char *inpath_name_ptr = in_path;

        while (*inpath_ptr != '\0') {
                if (*inpath_ptr == '/') {
                        if (*(inpath_ptr + 1) != '\0')
                                inpath_name_ptr = inpath_ptr + 1;
                }

                ++inpath_ptr;
        }

        strcpy(out, inpath_name_ptr);
}

int copy_file_to_file (char *in_file, char *out_file, int flags)
{
        struct stat in_statbuf;
        mode_t in_mode;

        struct stat out_statbuf;
        mode_t out_mode;


        int in_ret = stat(in_file, &in_statbuf);
        int out_ret = stat(out_file, &out_statbuf);

        if (in_ret == -1) {
                fprintf(stderr, "%s: '%s': ", progname, in_file);
                perror("");

                return -1;
        } else if (out_ret == -1 && errno != ENOENT) {
                fprintf(stderr, "%s: '%s': ", progname, out_file);
                perror("");

                return -1;
        }

        in_mode = in_statbuf.st_mode;
        out_mode = out_ret == -1 ? -1 : out_statbuf.st_mode;

        if (S_ISDIR(in_mode) && !CHKF_CP_RECURSIVE(flags)) {
                fprintf(stderr, "%s: Omitting directory '%s'\n",
                                progname, in_file);
                fprintf(stderr, "Hint: Maybe, specify '-r'?\n");
                fprintf(stderr, "Try '%s --help' for more information\n",
                                progname);

                return -1;
        } else if (S_ISDIR(out_mode)) {
                char in_file_basename[PATH_MAX];

                strcat(out_file, "/");
                base_name(in_file_basename, in_file);
                strcat(out_file, in_file_basename);
        }

        int in_fd = open(in_file, O_RDONLY);
        int out_fd = open(out_file, O_WRONLY | O_CREAT, in_mode);

        if (in_fd == -1) {
                fprintf(stderr, "%s: '%s': ", progname, in_file);
                perror("");
                return -1;
        } else if (out_fd == -1) {
                fprintf(stderr, "%s: '%s': ", progname, out_file);
                perror("");
                return -1;
        }

        int ch = 0;
        while (read(in_fd, &ch, 1) > 0) {
                write(out_fd, &ch, 1);
        }

        if (CHKF_CP_VERBOSE(flags)) {
                printf("Copied '%s' to '%s'\n", in_file, out_file);
        }

        close(in_fd);
        close(out_fd);
        return 0;
}

int copy_files_into_dir (struct node *files, char *dir_name, int flags)
{
        struct node *file = files;
        while (file != NULL) {
                struct stat stat_buf;
                int stat_ret = stat(file->str, &stat_buf);
                char out_dir[PATH_MAX];

                strcpy(out_dir, dir_name);

                if (stat_ret == -1) {
                        fprintf(stderr, "%s: '%s': ", progname, file->str);
                        perror("");

                        return -1;
                }

                int fd = open(out_dir, O_RDONLY | O_DIRECTORY);
                if (fd == -1) {
                        fprintf(stderr, "%s: '%s': ", progname, file->str);
                        perror("");

                        return -1;
                }

                int copy_ret = copy_file_to_file(file->str, out_dir, flags);
                if (copy_ret == -1) {
                        return -1;
                }

                file = file->next;
        }

        return 0;
}

void help (void)
{
        printf("Usage: %s [OPTION] FILE1 FILE2\n", progname);
        printf("   or: %s [OPTION] FILE or DIRECTORY... DIRECTORY\n\n",
                       progname);

        printf("Options:\n"
               "\t-v, --verbose\tPrint a message for every file that is being moved\n"
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
