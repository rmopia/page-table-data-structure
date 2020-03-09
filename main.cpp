/* Name: Robert Mopia
RedID: 817646343
edoras id: misc0252 */
#define BADFLAG 1
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "pagetable.h"
#include "level.h"
#include "map.h"
#include "conversions.h"
#include "byu_tracereader.c"

using namespace std;

int frame = 0; /* global var stored in map, used for translation */

void LogicalToPhysical(unsigned int addr, int offset_mask, int frame, int page_size){
    cout << DecToHex(addr) << " -> ";
    /* offset obtained from logical addr and num of bits of offset */
    int offset = addr & offset_mask;
    /* virtual addr converted to physical addr */
    int result = (frame)*(page_size) + offset;
    cout << DecToHex(result) << endl;
}

int main(int argc, char **argv){
    /* command line vars used after page initialization & insertion*/
    int Option, addr_limit = 0;
    bool p_bool = 0, t_bool = 0;
    char *res_filename;

    while((Option = getopt(argc, argv, "n:p:t")) != -1){
        switch (Option) {
        case 'n': /* Number of addresses to process */
            addr_limit = atoi(optarg);
        break;
        case 'p': /* produce map of pages */
            res_filename = optarg;
            FILE *fPtr;
            /* if file does not exist then we exit */
            if(fPtr = fopen(res_filename, "r")){
                fclose(fPtr);
                p_bool = 1;
            }
            else{
                exit(BADFLAG);
            }
        break;
        case 't': /* Show address translation */
            /* address translation done after page insertions */
            t_bool = 1;
        break;
        default:
            /* invalid arguments throw user out */
            cout << "invalid argument found." << endl;
            exit(BADFLAG);
        }
    }

    /* first mandatory argument, optind is defined by getopt */
    int opt_idx = optind;
    char *filename;
    /* vars used to store misc. command line arguments */
    int levels = 0, page_num_bits = 0;
    int level_bits[5];
    int level_idx = 0;

    /* run while loop to obtain remaining arguments */
    while(argv[opt_idx] != nullptr){
        if(isdigit(*argv[opt_idx])){
            page_num_bits += atoi(argv[opt_idx]);
            level_bits[level_idx] = atoi(argv[opt_idx]);
            level_idx++;
            levels++;
        }
        else{
            filename = argv[opt_idx];
        }
        opt_idx++;
    }

    /* pagetable used by levels as a center of information */
    /* pagetable initialized with root node level */
    Pagetable *pt = new Pagetable(levels, level_bits);
    Level *level_zero = new Level(pt, 0, pt->EntryCount[0]);
    /* link pagetable to level with pointer */
    pt->RootNodePtr = level_zero;

    /* set file ptr and address array ptr for trace file */
    FILE *trfile;
    p2AddrTr *addrPtr = (p2AddrTr*)malloc(sizeof(p2AddrTr));
    if(trfile = fopen(filename, "r")){
        /* reads values in trace file that are inserted into page table */
        while(NextAddress(trfile, addrPtr) != 0){
            if(PageInsert(pt, addrPtr->addr, frame) != false){
                frame++;
            }
        }
        fclose(trfile);
    }
    else{
        /* if file does not exist or is inaccessible we exit */
        exit(BADFLAG);
    }

    /* number of bits not used by page number declaration */
    int offset_bits = 32 - page_num_bits;
    /* page size determined by offset bits */
    int page_size = pow(2, offset_bits);
    /* bitmask used to find and isolate offset */
    int offset_bitmask = page_size - 1;

    /* logical to physical address translation flag */
    if(t_bool == 1){
        FILE *filePtr;
        /* reopens and rereads original file */
        filePtr = fopen(filename, "r");
        p2AddrTr *aPtr = (p2AddrTr*)malloc(sizeof(p2AddrTr));
        int addr_counter = 0;
        if(addr_limit != 0){
            /* only processes addr_counter amount of addresses */
            while(NextAddress(filePtr, aPtr) && addr_counter < addr_limit){
                /* map object used to find frame */
                Map *mpPtr = new Map();
                mpPtr = PageLookUp(pt, aPtr->addr);
                LogicalToPhysical(aPtr->addr, offset_bitmask, mpPtr->frame, page_size);
                addr_counter++;
            }
        }
        else{
            while(NextAddress(filePtr, aPtr)){
                Map *mpPtr = new Map();
                mpPtr = PageLookUp(pt, aPtr->addr);
                LogicalToPhysical(aPtr->addr, offset_bitmask, mpPtr->frame, page_size);
            }
        }
        fclose(filePtr);
    }
    /* write to specified file with ordered hex addresses and their frames */
    if(p_bool == 1){
        // do something w/ res file
    }

    return 0;
}
