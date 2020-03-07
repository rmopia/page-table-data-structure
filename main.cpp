#define BADFLAG 1
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include "structures.h"
#include "byutr.h"

using namespace std;

int frame = 0; // used when in map

int main(int argc, char **argv){
    int Option;
    int value;

    while ( (Option = getopt(argc, argv, "n:p:t")) != -1) {
        switch (Option) {
        case 'n': /* Number of addresses to process */
            value = atoi(optarg);
        // optarg will contain the string following -n
        // Process appropriately (e.g. convert to integer atoi(optarg))
        break;
        case 'p': /* produce map of pages */
        // optarg contains name of page file…
        break;
        case 't': /* Show address translation */
        // No argument this time, just set a flag
        break;
        default:
        // print something about the usage and die…
        exit(BADFLAG); // BADFLAG is an error # defined in a header
        }
    }
    /* first mandatory argument, optind is defined by getopt */
    /* run while loop to obtain remaining arguments */
    int opt_idx = optind;
    int levels = 0;

    int level_bits[5];
    int index = 0;
    char *filename;

    while(argv[opt_idx] != nullptr){
        if(isdigit(*argv[opt_idx])){
            level_bits[index] = atoi(argv[opt_idx]);
            index++;
            levels++;
        }
        else{
            filename = argv[opt_idx];
        }
        opt_idx++;
    }

    cout << filename << endl;
    FILE *pfile;
    pfile = fopen(filename, "r");

    size_t n = sizeof(level_bits)/sizeof(level_bits[0]);

	for (size_t i = 0; i < levels; i++) {
        if(level_bits[i] != 0){
            cout << level_bits[i] << endl;
        }
	}

    // pagetable used by levels as a center of information
    Pagetable *pt = new Pagetable(levels, level_bits);

    Level *level_zero = new Level(pt, 0, pt->EntryCount[0]);

    pt->RootNodePtr = level_zero;

    string hex_thang = "0xFEFFFEC2";
    unsigned int dec_thang = HexToDec(hex_thang);

    PageInsert(pt, dec_thang, frame);

    PageLookUp(pt, dec_thang);

/*
    string example_hex = "0x3c654321";
    string example_mask = "0x0FC00000";

    unsigned int dec, dec_mask;
    dec = HexToDec(example_hex);
    dec_mask = HexToDec(example_mask);

    cout << LogicalToPage(dec, dec_mask, 22) << endl;
*/
}
