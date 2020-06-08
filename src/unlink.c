/* unlink: Call the unlink() function to remove the specified file  */

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
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

void help (const char *progname);
void version (const char *progname);

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "%s: Missing operand\n", argv[0]);
		fprintf(stderr, "Try `%s --help` for more information\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	char **opt_ptr = argv + 1;
	while (*opt_ptr != NULL) {
		if (strcmp(*opt_ptr, "--help") == 0) {
			help(argv[0]);
			exit(EXIT_SUCCESS);
		} else if (strcmp(*opt_ptr, "--version") == 0) {
			version(argv[0]);
			exit(EXIT_SUCCESS);
		} else if (*opt_ptr[0] == '-') {
			fprintf(stderr,
				"%s: invalid option '%s'\n",
				argv[0], *opt_ptr);
			fprintf(stderr,
				"Try `%s --help` for more information\n",
				argv[0]);

			exit(EXIT_FAILURE);
		}

		++opt_ptr;
	}

	const char *fname = *(argv + 1);
	int ret = unlink(fname);

	if (ret == -1) {
		perror("Could not unlink file");
		exit(EXIT_FAILURE);
	}

	return 0;
}

void help (const char *progname)
{
	printf("Usage: %s FILE\n\
or:    %s OPTION\n\n", progname, progname);
	
	printf("Options:\n");
	printf("\t--help\tShow this help message and help\n"
	       "\t--version\tShow version information and help\n");
}

void version (const char *progname)
{
	printf("Copyright (C) 2020 Jithin Renji\n"
"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n\n"
"Written by Jithin Renji.\n");
}
