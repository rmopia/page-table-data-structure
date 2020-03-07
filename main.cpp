#define BADFLAG 1
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include "structures.h"
#include "level.h"
//#include "byutr.h"
#include "byu_tracereader.c"

using namespace std;

int frame = 0; // used when in map

int main(int argc, char **argv){
    int Option, addr_num;
    bool p_bool, t_bool = 0;
    char *res_filename;

    while ( (Option = getopt(argc, argv, "n:p:t")) != -1) {
        switch (Option) {
        case 'n': /* Number of addresses to process */
            addr_num = atoi(optarg);
        break;
        case 'p': /* produce map of pages */
            res_filename = optarg;
            p_bool = 1;
        break;
        case 't': /* Show address translation */
            t_bool = 1;
        break;
        default:
            cout << "invalid argument found." << endl;
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

    // pagetable used by levels as a center of information
    Pagetable *pt = new Pagetable(levels, level_bits);
    Level *level_zero = new Level(pt, 0, pt->EntryCount[0]);
    pt->RootNodePtr = level_zero;

    FILE *trfile;
    trfile = fopen(filename, "r");
    p2AddrTr *addrPtr = (p2AddrTr*)malloc(sizeof(p2AddrTr));

    while(NextAddress(trfile, addrPtr) != 0){
        cout << addrPtr->addr << endl;
        PageInsert(pt, addrPtr->addr, frame);
        frame++;
    }

    fclose(trfile);

    unsigned int h = HexToDec("0xFEFFFEC2");

    PageInsert(pt, h, frame);

    Map *mp = new Map();

    mp = PageLookUp(pt, h);

    return 0;
}
