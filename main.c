#include <stdio.h>

char make_printable(char c) {
    if (c >= ' ' && c <= '~') return c;
    else return '.';
}

void dump(char *filename) {
    FILE *f;
    unsigned char buf[16];
    unsigned int read = 0;
    unsigned int offset = 0;
    
    f = fopen(filename, "rb");
    printf("%s:\n", filename);
    if (!f) {
        printf("cannot open");
        fclose(f);
        return;
    }

    while (!feof(f)) {
        read = fread(buf, sizeof(unsigned char), 16, f);
        if (!read) break;

        /* print hex */
        printf("%06x    ", offset);
        for (unsigned int i = 0; i < read; i++) {
            printf("%02x ", buf[i]);

            /* split the hex into two parts of 8 bytes each */
            if (i == 7) printf(" ");
        }
        /* fill out a line that isn't exactly 16 bytes */
        if (read < 16) {
            for (unsigned int i = read; i < 16; i++) {
                printf("   ");
                if (i == 7) printf(" ");
            }
        }
        printf("   ");

        /* print ascii */
        for (unsigned int i = 0; i < read; i++) {
            printf("%c", make_printable(buf[i]));
        }
        printf("\n");
        offset += read;
    }
    fclose(f);
    return;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("usage: %s files..\n", argv[0]);
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        dump(argv[i]);
    }
    return 0;
}