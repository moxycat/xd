#include <stdio.h>
#include <string.h>

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

void dump_as_array(char *filename) {
    FILE *f;
    unsigned char buf[16];
    unsigned int read = 0;

    f = fopen(filename, "rb");
    printf("%s:\n", filename);
    if (!f) {
        printf("cannot open");
        fclose(f);
        return;
    }

    printf("unsigned char data[] = {\n");

    while (!feof(f)) {
        read = fread(buf, sizeof(unsigned char), 16, f);
        if (!read) break;

        printf("        ");
        for (unsigned int i = 0; i < read; ++i) {
            if (i > 0) printf(", ");
            printf("%#02x", buf[i]);
        }
        printf("\n");
    }
    printf("};\n");
    fclose(f);
    return;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("usage: %s [-c] files..\n", argv[0]);
        printf(" -c  print in a c-style array format.\n");
        return 0;
    }
    int c_mode = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-c")) {
            c_mode = 1;
            continue;
        }
        if (c_mode) dump_as_array(argv[i]);
        else dump(argv[i]);
    }
    return 0;
}
