/* rm: Remove files and/or directories */

/*
 *	Copyright (C) 2020 Jithin Renji
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "rm.h"

void help (const char *progname);
int rm (const char *fname, int flags);

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
		perror(fname);
		return -1;
	}

	char resp[3] = "y";

	if (CHKF_INTERACTIVE(flags)) {
		printf("Remove '%s'? ", fname);
		fgets(resp, 3, stdin);
	}

	if (resp[0] == 'y' || resp[0] == 'Y') {
		int ret = unlinkat(AT_FDCWD, fname, unlink_flags);

		if (ret == -1) {
			perror(fname);
			return -1;
		}
	}

	close(fd);
	return 0;
}

void help (const char *progname)
{
	printf("Usage: %s [OPTION]... [FILE]...\n\n", progname);

	printf("Options:\n");
	printf("\t-r\tRemove directories recursively\n"
	       "\t-d\tRemove empty directories\n"
	       "\t-i\tAsk before removing\n");
}
