/* wc: Report line word and byte counts for given files */

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

int main (int argc, char **argv)
{
	size_t lines = 0;
	size_t words = 0;
	size_t bytes = 0;

	size_t total_lines = 0;
	size_t total_words = 0;
	size_t total_bytes = 0;

	if (argc < 2) {
		char ch = 0;
		while ((ch = fgetc(stdin)) != EOF) {
			bytes += 1;
			if (ch == ' ') {
				words += 1;
			} else if (ch == '\n') {
				words += 1;
				lines += 1;
			}
		}
		printf("%ld \t%ld \t%ld -\n", lines, words, bytes);
	} else {
		char **files_ptr = argv + 1;
		while (*files_ptr != NULL) {
			FILE *fp = fopen(*files_ptr, "r");
			if (fp == NULL) {
				perror(*files_ptr);
				exit(EXIT_FAILURE);
			}

			char ch = 0;
			while ((ch = fgetc(fp)) != EOF) {
				bytes += 1;
				if (ch == ' ') {
					words += 1;
				} else if (ch == '\n') {
					words += 1;
					lines += 1;
				}
			}

			total_lines += lines;
			total_words += words;
			total_bytes += bytes;

			printf("%ld \t%ld \t%ld \t%s\n", lines, words, bytes,
					*files_ptr);

			lines = 0;
			words = 0;
			bytes = 0;

			++files_ptr;
			fclose(fp);
		}
	}

	return 0;
}
