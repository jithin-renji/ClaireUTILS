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

#include "rm.h"

void help (const char *progname);
void rm (const char *fname, int flags);

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "%s: Missing operand\n", argv[0]);
		fprintf(stderr, "Try `%s -h` for more information.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int flags = 0;
	int opt = 0;

	while ((opt = getopt(argc, argv, "rfih")) != -1) {
		switch (opt) {
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
		rm(argv[optind], flags);
	}
}

void rm (const char *fname, int flags)
{
	printf("%s\n", fname);
}

void help (const char *progname)
{
	printf("Usage: %s [OPTION]... [FILE]...\n\n", progname);
}
