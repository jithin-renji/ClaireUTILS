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
#include <dirent.h>

#define FILES_PER_LINE		3

void list (const char *dir_path, int should_list_long,
	   int should_list_all, int should_list_colored);
void help (const char *progname);

int main (int argc, char **argv)
{
	int list_long = 0;
	int list_all = 0;
	int list_colored = 0;

	int opt;
	while ((opt = getopt(argc, argv, "laCH")) != -1) {
		switch (opt) {
		case 'l':
			list_long = 1;
			break;

		case 'a':
			list_all = 1;
			break;

		case 'C':
			list_colored = 1;
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
		list(".", list_long, list_all, list_colored);
	} else {
		list(argv[optind], list_long, list_all, list_colored);
	}
}

void list (const char *dir_path, int should_list_long,
	   int should_list_all, int should_list_colored)
{
	DIR *dir = opendir(dir_path);

	if (dir == NULL) {
		perror(dir_path);
		exit(EXIT_FAILURE);
	}

	struct dirent *dp;
	int nlisted_in_line = 0;
	while ((dp = readdir(dir)) != NULL) {
		if (!should_list_all && dp->d_name[0] == '.') {
			/* Do nothing */
		} else {
			if (nlisted_in_line < FILES_PER_LINE) {
				printf("%s\t", dp->d_name);
				nlisted_in_line += 1;
			} else {
				printf("\n%s\t", dp->d_name);
				nlisted_in_line = 1;
			}
		}
	}

	printf("\n");
}

void help (const char *progname)
{
	printf("Usage: %s [OPTION]... [FILE]...\n\n", progname);
}
