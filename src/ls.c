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
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

#include "ls.h"
#include "general.h"

void get_permissions (mode_t mode, char *out_str);
void list (const char *dir_path, int flags);
void help (const char *progname);

int main (int argc, char **argv)
{
	int flags;
	int opt;

	while ((opt = getopt(argc, argv, "laACH")) != -1) {
		switch (opt) {
		case 'l':
			flags |= LS_LONG;
			break;

		case 'a':
			flags |= LS_ALL;
			break;

		case 'A':
			flags |= LS_ALL | LS_ALL_NOT_DODD;
			break;

		case 'C':
			flags |= LS_COLORED;
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

void get_permissions(mode_t mode, char *out_str)
{
	/* Check file type
	 * TODO: Add support for socket and symbolic
	 * 	 links too */
	if (S_ISDIR(mode)) {
		strcpy(out_str, "d");
	} else if (S_ISCHR(mode)) {
		strcpy(out_str, "c");
	} else if (S_ISBLK(mode)) {
		strcpy(out_str, "b");
	} else if (S_ISFIFO(mode)) {
		strcpy(out_str, "p");
	} else if (S_ISREG(mode)) {
		strcpy(out_str, "-");
	} else {
		strcpy(out_str, "?");
	}

	/* Check if owner can read */
	if ((mode & S_IRUSR) == S_IRUSR) {
		strcat(out_str, "r");
	} else {
		strcat(out_str, "-");
	}

	/* Check if owner can write */
	if ((mode & S_IWUSR) == S_IWUSR) {
		strcat(out_str, "w");
	} else {
		strcat(out_str, "-");
	}

	/* Check if owner can execute */
	if ((mode & S_IXUSR) == S_IXUSR) {
		if ((mode & S_ISUID) == S_ISUID) {
			strcat(out_str, "s");
		} else {
			strcat(out_str, "x");
		}
	} else {
		if ((mode & S_ISUID) == S_ISUID) {
			strcat(out_str, "S");
		} else {
			strcat(out_str, "-");
		}
	}

	/* Check if group can read */
	if ((mode & S_IRGRP) == S_IRGRP) {
		strcat(out_str, "r");
	} else {
		strcat(out_str, "-");
	}

	/* Check if group can write */
	if ((mode & S_IWGRP) == S_IWGRP) {
		strcat(out_str, "w");
	} else {
		strcat(out_str, "-");
	}

	/* Check if group can execute */
	if ((mode & S_IXGRP) == S_IXGRP) {
		if ((mode & S_ISGID) == S_ISGID) {
			strcat(out_str, "s");
		} else {
			strcat(out_str, "x");
		}
	} else {
		if ((mode & S_ISGID) == S_ISGID) {
			strcat(out_str, "S");
		} else {
			strcat(out_str, "-");
		}
	}

	/* Check if others can read */
	if ((mode & S_IROTH) == S_IROTH) {
		strcat(out_str, "r");
	} else {
		strcat(out_str, "-");
	}

	/* Check if others can write */
	if ((mode & S_IWOTH) == S_IWOTH) {
		strcat(out_str, "w");
	} else {
		strcat(out_str, "-");
	}

	/* Check if others can execute */
	if ((mode & S_IXOTH) == S_IXOTH) {
		strcat(out_str, "x");
	} else {
		strcat(out_str, "-");
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
		if ((flags & LS_ALL_NOT_DODD) == LS_ALL_NOT_DODD
			   && (strcmp(dp->d_name, ".") == 0
			   || strcmp(dp->d_name, "..") == 0)) {

			/* Do nothing */


		} else if ((flags & LS_ALL) != LS_ALL
			   && dp->d_name[0] == '.') {

			/* Do nothing */

		} else {
			strcpy(files[nfinished], dp->d_name);
			nfinished += 1;
		}
	}

	int i = 0;
	while (i < nfinished) {
		struct stat statbuf;

		/* FIXME: Allocate memory dynamically */
		char full_fname[256] = "";
		strcat(full_fname, dir_path);
		strcat(full_fname, "/");
		strcat(full_fname, files[i]);

		int ret = stat(full_fname, &statbuf);
		if (ret == -1) {
			perror(full_fname);
			exit(EXIT_FAILURE);
		}

		/* Full permissions string
		 * Eg: -rw-rw-r-- */
		char permissions[11] = "";

		/* If color output is being asked,
		 * the color specified in this string
		 * will be used */
		char color[10] = "";
		
		mode_t mode = statbuf.st_mode;
		uid_t uid = statbuf.st_uid;
		gid_t gid = statbuf.st_gid;

		get_permissions(mode, permissions);

		struct passwd *usr_info = getpwuid(uid);
		struct group *grp_info = getgrgid(gid);

		if ((flags & LS_COLORED) == LS_COLORED) {
			if (S_ISDIR(mode)) {
				strcpy(color, B_BLUE);
			}
		}

		if ((flags & LS_LONG) == LS_LONG) {
			printf("%s\t%s\t%s\t", permissions, usr_info->pw_name
					     , grp_info->gr_name);
		}

		printf("%s%s\n" RESET , color, files[i]);

		i += 1;
	}
}

void help (const char *progname)
{
	printf("Usage: %s [OPTION]... [FILE]...\n\n", progname);
}
