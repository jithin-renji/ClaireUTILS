/* ls: List directory contents */

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
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include "ls.h"
#include "general.h"

void list (const char *dir_path, int flags);
void help (const char *progname);

int main (int argc, char **argv)
{
	int flags;
	int opt;

	while ((opt = getopt(argc, argv, "laCH")) != -1) {
		switch (opt) {
		case 'l':
			flags |= LIST_LONG;
			break;

		case 'a':
			flags |= LIST_ALL;
			break;

		case 'C':
			flags |= LIST_COLORED;
			break;

		case 'H':
			help(argv[0]);
			exit(EXIT_SUCCESS);
			break;
		
		default:
			printf("Try `%s -H` for more information.\n",
				argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (argv[optind] == NULL) {
		list(".", flags);
	} else {
		list(argv[optind], flags);
	}
}

void list (const char *dir_path, int flags)
{
	DIR *dir = opendir(dir_path);

	if (dir == NULL) {
		perror(dir_path);
		exit(EXIT_FAILURE);
	}

	char files[200][256];
	struct dirent *dp;
	int nfinished = 0;
	while ((dp = readdir(dir)) != NULL) {
		if ((flags & LIST_ALL) != LIST_ALL && dp->d_name[0] == '.') {
			/* Do nothing */
		} else {
			strcpy(files[nfinished], dp->d_name);
			nfinished += 1;
		}
	}

	int i = 0;
	while (i < nfinished) {
		struct stat *statbuf = malloc(sizeof(struct stat));
		int ret = stat(files[i], statbuf);
		if (ret == -1) {
			perror("");
			exit(EXIT_FAILURE);
		}
		
		if ((statbuf->st_mode & S_IFMT) == S_IFDIR
		    && (flags & LIST_COLORED) == LIST_COLORED) {
			printf(B_BLUE "%s\n" RESET, files[i]);
		} else {
			printf("%s\n", files[i]);
		}

		i += 1;
		free(statbuf);
	}
}

void help (const char *progname)
{
	printf("Usage: %s [OPTION]... [FILE]...\n\n", progname);
}
