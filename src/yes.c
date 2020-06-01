/* yes: Repeatedly output a string, until killed */

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

int main (int argc, char **argv)
{
	if (argc < 2) {
		while (1)
			puts("y");
	} else {
		size_t size = 0;
		for (int i = 1; i < argc; i++) {
			size += strlen(argv[i]);
		}

		char **argv_ptr = argv + 1;
		char str[size];
		strcpy(str, "");

		while (*argv_ptr != NULL) {
			strcat(str, *argv_ptr);
			strcat(str, " ");
			++argv_ptr;
		}
		while (1)		
			printf("%s\n", str);
	}

	return 0;
}
