
#include <stdio.h>
#include <stdint.h>

int main (int argc, char **argv) {
    FILE *f_in;
    uint16_t d16;
    uint32_t dn;

    if (argc != 3) {
        fprintf (stderr, "usage: uint16_hpp input-file data-name\n");
        return -1;
    }

    f_in = fopen (argv[1], "rb");
    if (! f_in) { 
        fprintf (stderr, "ERROR: fopen rb: %s\n", argv[1]);
        return -1;
    }
 
    printf ("const uint16_t %s [] = {\n", argv[2]);

    dn = 0;
    while (fread (&d16, 2, 1, f_in) == 1) {
        printf (" 0x%04X,", d16);
        dn ++;
        if (! (dn % 8))
            printf ("\n");
    }

    if (dn % 8) 
        printf ("\n");

    printf ("};\nconst uint32_t %s_n = %u;\n\n", argv[2], dn);
 
    return 0;
}

