// Author:		Joe Biggins
// Date:		Jul 29 2021
#include <stdio.h>
#include <stdlib.h>
#include "encoder.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <locale.h>
#include <wchar.h>

int print_usage(const char *name)
{
    printf("usage: %s from_file target_file\n", name);
    return EXIT_FAILURE;
} 

utf_8 encode(int symbol, const utf_8 *charmap)
{
    utf_8 result = {1,{symbol}};
    if (!(0x80 & symbol)) {
        return result;
    } else if ((127 < symbol) && (symbol < 256)) {
		int offset = symbol - 128;
		utf_8 return_val = charmap[offset];
		return return_val;
	} else { 
		exit(EXIT_FAILURE);
	}
}

const utf_8* actual_charmap(int enc_flag) {
	// This function is unecessary now, since only bold cp1252
	// but added to allow support for multiple encodings in future.
	if (enc_flag == 1) {
		return cp1252;
	} else {
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv)
{

	int flag;			// UNECESSARY -- only one option currently.
	int symbol;			// dec code of char read from input;
    char * ifp;			// ptr to input file path
    char * ofp;			// ptr to output file path
    utf_8 code;			// struct to hold result of converted char
    FILE * ifile;		// input file ptr
    FILE * ofile;		// output file ptr

	// INPUT:	either stdin or file passed in via cmd line arg.
	// OUTPUT:	either stdout or file passed in via cmd line args.
	// TODO: 	add handling to timeout when waiting from input from stdin
	// 			and none is provided.
	switch (argc) {
		case 2:
			ifp = argv[1];
			ofp = "/dev/stdout";
			flag = 1;
			break;
		case 3:
			ifp = argv[1];
			ofp = argv[2];
			flag = 1;
			break;
		default:
			ifp = "/dev/stdin";
			ofp = "/dev/stdout";
			flag = 1;
			break;
	}
    setlocale(LC_ALL, "en_US.UTF-8");
	

	// initialize and validate input file
    ifile = fopen(ifp, "rb");
    if ( ifile == NULL ) {
        perror("can't open from file");
        return EXIT_FAILURE;
    }

	// initialize and validate output file
    ofile = fopen(ofp, "w");
    if ( ofile == NULL ) {
        perror("can't open target file or create");
        return EXIT_FAILURE;
    }

	
    const utf_8 *charmap = actual_charmap(1);
    while ((symbol = fgetc(ifile)) != EOF) {
        code = encode(symbol, charmap);
        for (size_t i = 0; i < code.size; i++) {
			//printf("code.bytes[%ld]: %d\n",i,code.bytes[i]);
            int err = fputc(code.bytes[i], ofile);
            if (err == EOF) {
                perror("write error");
                return EXIT_FAILURE;
            }
        }
    }
    
    fclose(ifile);
    fclose(ofile);
    return EXIT_SUCCESS;
}
