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
#include <time.h>
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

void convert_to_month_word (char *month_num_str);
void set_mdate (char *mdate_str, time_t mtime);

void convert_to_human_rd (char *out_str, off_t nbytes);

void list (const char *dir_path, int flags);

void help (const char *progname);

int main (int argc, char **argv)
{
	int flags = 0;
	int opt = 0;

	while ((opt = getopt(argc, argv, "lachCAH")) != -1) {
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
		case 'c':
			flags |= LS_COLORED;
			break;

		case 'h':
			flags |= LS_HUMAN_RD;
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
		int i = optind;
		while (i < argc) {
			printf("%s:\n", argv[i]);
			list(argv[i], flags);
			printf("\n");

			i += 1;
		}
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


/* Is there a better way to do this? */
void convert_to_month_word (char *month_num_str)
{
	if (strcmp(month_num_str, "01") == 0) {
		strcpy(month_num_str, "Jan");
	} else if (strcmp(month_num_str, "02") == 0) {
		strcpy(month_num_str, "Feb");
	} else if (strcmp(month_num_str, "03") == 0) {
		strcpy(month_num_str, "Mar");
	} else if (strcmp(month_num_str, "04") == 0) {
		strcpy(month_num_str, "Apr");
	} else if (strcmp(month_num_str, "05") == 0) {
		strcpy(month_num_str, "May");
	} else if (strcmp(month_num_str, "06") == 0) {
		strcpy(month_num_str, "Jun");
	} else if (strcmp(month_num_str, "07") == 0) {
		strcpy(month_num_str, "Jul");
	} else if (strcmp(month_num_str, "08") == 0) {
		strcpy(month_num_str, "Aug");
	} else if (strcmp(month_num_str, "09") == 0) {
		strcpy(month_num_str, "Sep");
	} else if (strcmp(month_num_str, "10") == 0) {
		strcpy(month_num_str, "Oct");
	} else if (strcmp(month_num_str, "11") == 0) {
		strcpy(month_num_str, "Nov");
	} else if (strcmp(month_num_str, "12") == 0) {
		strcpy(month_num_str, "Dec");
	}
}

void set_mdate (char *mdate_str, time_t mtime)
{
	char month[10];
	char day[5];
	char time[10];

	strftime(month, 10, "%m", localtime(&mtime));
	convert_to_month_word(month);

	strftime(day, 5, "%d", localtime(&mtime));
	strftime(time, 10, "%H:%M", localtime(&mtime));

	strcat(mdate_str, month);
	strcat(mdate_str, " ");
	strcat(mdate_str, day);
	strcat(mdate_str, " ");
	strcat(mdate_str, time);
}

void convert_to_human_rd (char *out_str, off_t nbytes)
{
	double human_rd_size = 0;
	if (nbytes >= 1000 && nbytes < 1000 * 1000) {	
		human_rd_size = (double) nbytes / 1000;
		snprintf(out_str, 25, "%.2f", human_rd_size);
		strcat(out_str, "K");
	} else {
		snprintf(out_str, 25, "%ld", nbytes);
	}
}

void list (const char *dir_path, int flags)
{
	DIR *dir;
	char files[200][256];
	struct dirent *dp;
	int nfinished = 0;

	dir = opendir(dir_path);

	if (dir == NULL) {
		perror(dir_path);
		exit(EXIT_FAILURE);
	}
	
	while ((dp = readdir(dir)) != NULL) {
		if (CHKF_ALL_NOT_DODD(flags) && (strcmp(dp->d_name, ".") == 0
			   || strcmp(dp->d_name, "..") == 0)) {

			/* Do nothing */


		} else if (!CHKF_ALL(flags) && dp->d_name[0] == '.') {

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
		size_t links = statbuf.st_nlink;
		uid_t uid = statbuf.st_uid;
		gid_t gid = statbuf.st_gid;
		struct passwd *usr_info = getpwuid(uid);
		struct group *grp_info = getgrgid(gid);

		char fsize_str[25];
		char mdate[256];

		get_permissions(mode, permissions);
		set_mdate(mdate, statbuf.st_mtime);

		if (CHKF_HUMAN_RD(flags)) {
			convert_to_human_rd(fsize_str, statbuf.st_size);
		} else {
			snprintf(fsize_str, 25, "%ld", statbuf.st_size);
		}

		if (CHKF_COLORED(flags)) {
			if (S_ISDIR(mode)) {
				strcpy(color, B_BLUE);
			}
		}

		if (CHKF_LONG(flags)) {
			printf("%s  %ld\t%s\t%s\t%s\t%s\t", permissions,
					links, usr_info->pw_name,
					grp_info->gr_name, fsize_str,
					mdate);
		}

		printf("%s%s\n" RESET , color, files[i]);

		strcpy(mdate, "");
		strcpy(fsize_str, "");

		i += 1;
	}

	closedir(dir);
}

void help (const char *progname)
{
	printf("Usage: %s [OPTION]... [FILE]...\n\n", progname);
}
