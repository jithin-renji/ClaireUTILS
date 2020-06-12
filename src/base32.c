/* base32: Use base32 to encode a file */

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
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

char progname[256] = "";

struct option long_opts[] = {
        {"help",        no_argument, 0, 'h'},
        {"version",     no_argument, 0, 'v'},
        {0,             0,           0,  0}
};

int open_and_encode (const char *fname);
void base32_encode_and_print (const char *fname);
void help (void);
void version (void);

int main (int argc, char **argv)
{
        strcpy(progname, argv[0]);

        int opt = 0;
        while ((opt = getopt_long(argc, argv, "hv", long_opts, NULL)) != -1) {
                switch (opt) {
                case 'h':
                        help();
                        exit(EXIT_FAILURE);
                        break;

                case 'v':
                        version();
                        exit(EXIT_FAILURE);
                        break;

                default:
                        fprintf(stderr, "Try `%s --help` for more information\n", progname);
                        exit(EXIT_FAILURE);
                        break;
                }
        }

        if (argv[optind] == NULL) {
                fprintf(stderr, "%s: STDIN input is not supported yet!\n", progname);
                fprintf(stderr, "Try `%s --help` for more information.\n", progname);
                exit(EXIT_FAILURE);
        }

        if (argc != optind + 1) {
                fprintf(stderr, "%s: Extra operand `%s`\n", progname, argv[optind]);
                fprintf(stderr, "Try `%s --help` for more information.\n", progname);
                exit(EXIT_FAILURE);
        }

        open_and_encode(argv[optind]);

        return 0;
}

int open_and_encode (const char *fname)
{
        int fd = open(fname, O_RDONLY);
        if (fd == -1) {
                fprintf(stderr, "%s: ", progname);
                perror(fname);
                return -1;
        }

        char block[6];
        int i = 0;
        int ret = -1;
        while ((ret = read(fd, &block[i], 1)) == 1) {
                if (i == 4) {
                        block[i + 1] = '\0';
                        base32_encode_and_print(block);

                        i = 0;
                        memset(block, '\0', 6);
                        continue;
                }

                i += 1;
        }

        if (i != 0) {
                base32_encode_and_print(block);
        }

        puts("");
        return 0;
}

void base32_encode_and_print (const char *str)
{
        char base32_alphabet[] = {
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                'Y', 'Z', '2', '3', '4', '5', '6', '7'
        };

        int block_len = strlen(str);

        /* The block is stored here for actual use */
        long all_bits = 0;

        /* all_bits & INITIAL_MASK gives the first character */
        const long INITIAL_MASK = 0x000000f800000000;

        /* The final encoded string */
        char encoded[9] = "";

        memset(encoded, '\0', 9);

        for (int i = 0; i <= 4; i++) {
                long ch = (long) str[i];
                all_bits |= ch << (8 * (4 - i));
        }

        for (int i = 0; i < 8; i++) {
                int b32_i = (all_bits & (INITIAL_MASK >> 5 * i)) >> (35 - (5 * i));
                encoded[i] = base32_alphabet[b32_i];
        }

        /*
         * For imperfect blocks, ie. blocks that are not exactly 40 bits
         * long, we have to pad the output. These are the only possibilites,
         * if the block happens to be imperfect:
         *      - The block is exactly 8 bits long (ie. 1 char length). The
         *        result will be 2 characters, followed by 6 '=' padding chars.
         *
         *      - The block is exactly 16 bits long (ie. 2 char length). The
         *        result will be 4 characters, followed by 4 '=' padding chars.
         *
         *      - The block is exactly 32 bits long (ie. 3 char length). The
         *        result will be 7 characters, followed by padding.
         */
        int j = 0;
        switch (block_len) {
        case 1:
                j = 2;
                break;

        case 2:
                j = 4;
                break;

        case 3:
                j = 5;
                break;

        case 4:
                j = 7;
                break;

        default:
                /* The block is perfect, so we don't set `j` */
                break;
        }

        /* Only if `j` has a value other than 0, it means that the block
         * is imperfect. */
        if (j != 0) {
                for (int i = j; i < 8; i++) {
                        encoded[i] = '=';
                }
        }

        printf("%s", encoded);
}

void help (void)
{
        printf("Usage: %s [OPTION]\n\n", progname);

        printf("Options:\n"
               "\t-h, --help\tShow this help message and exit\n"
               "\t-v, --version\tShow version information and exit\n");
}

void version (void)
{
        printf("%s (ClaireUTILS) v0.1\n", progname);

        printf("Copyright (C) 2020 Jithin Renji\n"
"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n\n"
"Written by Jithin Renji.\n");
}
