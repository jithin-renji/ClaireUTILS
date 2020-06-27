/* mv: Move (rename) files */

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
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef linux
#  include <linux/limits.h>
#else
#  include <sys/syslimits.h>
#endif

#include "mv.h"
#include "linked_list.h"

char progname[256] = "";

struct option long_opts[] = {
        {"force",       no_argument, 0, 'f'},
        {"verbose",     no_argument, 0, 'v'},
        {"help",        no_argument, 0, 'h'},
        {"version",     no_argument, 0, 'V'},
        {0,             0,           0,  0}
};

void base_name (char *out, char *inpath);
int move_files_into_dir (struct node *files_list, const char *dir_name,
                         int flags);
void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int flags = 0;
        int opt = 0;
        while ((opt = getopt_long(argc, argv, "fvhV", long_opts, NULL)) != -1) {
                switch (opt) {
                case 'f':
                        flags |= MV_FORCE;
                        break;

                case 'v':
                        flags |= MV_VERBOSE;
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

        if (argc < 3) {
                fprintf(stderr, "%s: Too few arguments\n", progname);
                fprintf(stderr, "Try `%s --help` for more information.\n", progname);
                exit(EXIT_FAILURE);
        }

        if (argv[optind] == NULL || argc < 3) {
                fprintf(stderr, "%s: Missing operand\n", progname);
                fprintf(stderr, "Try `%s --help` for more information.\n", progname);
                exit(EXIT_FAILURE);
        }

        /* The command is in the form mv [OPTION]... FILE1 FILE2 */
        if (argc - optind == 2) {
                struct stat in_statbuf;
                struct stat out_statbuf;

                char in_file[PATH_MAX] = "";
                char out_file[PATH_MAX] = "";

                strcpy(in_file, argv[optind]);
                strcpy(out_file, argv[optind + 1]);

                int in_stat_ret = stat(in_file, &in_statbuf);
                int out_stat_ret = stat(out_file, &out_statbuf);

                if (in_stat_ret == -1) {
                        fprintf(stderr, "%s: '%s': ", progname, in_file);
                        perror("");
                        exit(EXIT_FAILURE);
                } else if (out_stat_ret == -1 && errno != ENOENT) {
                        fprintf(stderr, "%s: '%s': ", progname, out_file);
                        perror("");
                        exit(EXIT_FAILURE);
                }

                /* If the output file/dir does not exist, we can't
                 * really check if the output is a directory or not.
                 * So, this is set to -1, if `out_file` does not exist */
                mode_t out_mode = out_stat_ret == -1 ? -1 : out_statbuf.st_mode;

                if (S_ISDIR(out_mode)) {
                        char in_file_basename[PATH_MAX];

                        strcat(out_file, "/");
                        base_name(in_file_basename, in_file);
                        strcat(out_file, in_file_basename);
                }

                int mv_ret = rename(in_file, out_file);
                if (mv_ret == -1) {
                        fprintf(stderr, "%s: Could not move '%s' to '%s': ",
                                progname, in_file, out_file);

                        perror("");
                        exit(EXIT_FAILURE);
                }

                if (CHKF_MV_VERBOSE(flags)) {
                        printf("Moved '%s' to '%s'\n", in_file, out_file);
                }

        } else {
                const char *dir_name = argv[argc - 1];
                struct node *files = malloc(sizeof(struct node));

                node_init(files);

                for (int i = optind; i < argc - 1; i++) {
                        list_append(files, argv[i]);
                }

                move_files_into_dir(files, dir_name, flags);
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

int move_files_into_dir (struct node *files, const char *dir_name,
                         int flags)
{
        struct node *file = files;
        while (file != NULL) {
                int fd = open(file->str, O_RDONLY);
                if (fd == -1) {
                        fprintf(stderr, "%s: `%s`: ", progname, file->str);
                        perror("");
                        return -1;
                }

                close(fd);
                file = file->next;
        }

        int dir_fd = open(dir_name, O_RDONLY | O_DIRECTORY);
        if (dir_fd == -1) {
                fprintf(stderr, "%s: `%s`: ", progname, dir_name);
                perror("");
                return -1;
        }
        close(dir_fd);

        file = files;
        while (file != NULL) {
                char renamed_fname[PATH_MAX];
                char infile_basename[PATH_MAX];

                strcpy(renamed_fname, dir_name);
                strcat(renamed_fname, "/");

                base_name(infile_basename, file->str);

                strcat(renamed_fname, infile_basename);

                int ret = rename(file->str, renamed_fname);
                if (ret == -1) {
                        fprintf(stderr, "%s: Could not move `%s` to `%s`:",
                                progname, file->str, renamed_fname);

                        perror("");
                        return -1;
                }

                if (CHKF_MV_VERBOSE(flags)) {
                        printf("Moved '%s' to '%s'\n",
                                file->str, renamed_fname);
                }

                file = file->next;
        }

        return 0;
}

void help (void)
{
        printf("Usage: %s [OPTION] FILE1 FILE2\n", progname);
        printf("   or: %s [OPTION] FILE... DIRECTORY\n\n", progname);

        printf("Options:\n"
               "\t-f, --force\tDo not prompt before overwriting\n"
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
