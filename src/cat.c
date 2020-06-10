/* cat: ConCATenate files and print to standard output */

/*
 *      Copyright (C) 2020 Jithin Renji
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * */

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
        if (argc < 2) {
                char ch = 0;
                while ((ch = fgetc(stdin)) != EOF) {
                        putc(ch, stdout);
                }
        } else {
                char** fnames_ptr = argv + 1;

                while (*fnames_ptr != NULL) {
                        FILE* fp = fopen(*fnames_ptr, "r");

                        if (fp == NULL) {
                                perror(*fnames_ptr);
                                exit(EXIT_FAILURE);
                        }
                        
                        char ch = 0;
                        while ((ch = fgetc(fp)) != EOF) {
                                putc(ch, stdout);
                        }

                        fnames_ptr++;
                        fclose(fp);
                }
        }

        return 0;
}
