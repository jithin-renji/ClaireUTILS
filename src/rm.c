/* rm: Remove files and/or directories */

/*
 *	Copyright (C) 2020 Jithin Renji
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "rm.h"

void help (const char *progname);
int rm (const char *fname, int flags);
int rm_recursive(const char *fname);

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "%s: Missing operand\n", argv[0]);
		fprintf(stderr, "Try `%s -h` for more information.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int flags = 0;
	int opt = 0;

	while ((opt = getopt(argc, argv, "drfih")) != -1) {
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

		default:
			printf("Try `%s -h` for more information.\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (argv[optind] == NULL) {
		printf("%s: Missing operand\n", argv[0]);
		printf("Try `%s -h` for more information.\n", argv[0]);
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
	int fd_flags = 0;
	int unlink_flags = 0;

	if (CHKF_RECURSIVE(flags) || CHKF_EMPTY_DIRS(flags)) {
		fd_flags = O_RDONLY | O_DIRECTORY;
		unlink_flags = AT_REMOVEDIR;
	} else {
		fd_flags = O_RDWR;
	}

	int fd = open(fname, fd_flags);

	if (fd == -1) {
		if (!CHKF_FORCE(flags))
			perror(fname);
		return -1;
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
			perror(fname);
		}
	}

	close(fd);
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
			perror("yikes");
		}
	}

	return ret_val;
}

void help (const char *progname)
{
	printf("Usage: %s [OPTION]... [FILE]...\n\n", progname);

	printf("Options:\n");
	printf("\t-i\tAsk before removing\n"
	       "\t-f\tRemove files without prompting; don't complain about non-existent files\n"
	       "\t-r\tRemove directories recursively\n"
	       "\t-d\tRemove empty directories\n");
}
