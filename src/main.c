#include <stdio.h>
#include <stdlib.h>
#include "encoder.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>

int print_usage(const char *name)
{
    printf("usage: %s from_file [cp1251|iso-8859-5|koi8] target_file\n", name);
    return EXIT_FAILURE;
} 


utf_8 encode(int symbol, const utf_8 *charmap)
{

    utf_8 result = {1,{symbol}};
    if (!(0x80 & symbol)) {
        return result;
    }
    
    int offset = symbol - 134;
    //printf("offset: %d\n",offset);
    return charmap[offset];
}

const utf_8* actual_charmap(const char* encode)
{
    if (strcmp(encode, "cp1252") == 0) {
        return cp1252;
    } else {
        return NULL;
    }
}

int main(int argc, char **argv)
{
    char * ifp;
    char * ofp;
    utf_8 code;
    FILE * ifile;
    FILE * ofile;

    if (argc > 4) {
        return print_usage(argv[0]);
    }

    ifp = "/dev/stdin";
    ofp = "/dev/stdout";
    ifile = fopen(ifp, "rb");
    char * encoding;
    encoding = "cp1252";

    if (ifile == NULL) {
        perror("can't open from file");
        return EXIT_FAILURE;
    }

    /*ofile = fopen(ofp, "w");
    if (ofile == NULL) {
        perror("can't open target file or create");
        return EXIT_FAILURE;
    }*/

    const utf_8 *charmap = actual_charmap(encoding);
    if (charmap == NULL) {
        return print_usage(argv[0]);
    }

    int symbol = 0;
    while ((symbol = fgetc(ifile)) != EOF)
    {
        utf_8 code = encode(symbol, charmap);
        for (size_t i = 0; i < code.size; i++) 
        {
            int err = fputc(code.bytes[i], stdout);
            if (err == EOF) {
                perror("write error");
                return EXIT_FAILURE;
            }
        }
    }
    
    fclose(ifile);
    
    return EXIT_SUCCESS;
}
