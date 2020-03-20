/* Name: Robert Mopia
RedID: 817646343
edoras id: misc0252 */
#define BADFLAG 1
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <cmath>
#include <fstream>
#include <bits/stdc++.h>
#include "pagetable.h"
#include "level.h"
#include "map.h"
#include "conversions.h"
#include "byu_tracereader.c"

using namespace std;

void PageTableTraversal(Level *lvl, ofstream& writeFile){
    /* is at leaf node, so we look at map pointer */
    if(lvl->CurrentDepth+1 == lvl->PageTablePtr->LevelCount){
        for(int i = 0; i < lvl->PageTablePtr->
            EntryCount[lvl->CurrentDepth]; i++){
            if(lvl->MapPtr[i].FrameIndex.first != 0){
                writeFile << DecToHex(i) << " -> " <<
                    DecToHex(lvl->MapPtr[i].FrameIndex.second) << '\n';
            }
        }
    }
    /* not at leaf node, so we recurse when we find a ptr */
    else{
        for(int i = 0; i < lvl->PageTablePtr->EntryCount[lvl->CurrentDepth]; i++){
            if(lvl->NextLevelPtr[i].NextLevel != nullptr){
                PageTableTraversal(lvl->NextLevelPtr[i].NextLevel, writeFile);
            }
        }
    }
}

void PageTableTraversal(Pagetable *pt, ofstream& writeFile){
    PageTableTraversal(pt->RootNodePtr, writeFile);
}

int frame = 0; /* global var stored in map, used for translation */

int main(int argc, char **argv){
    /* command line vars used after page initialization & insertion*/
    int Option, addr_limit = 0;
    bool p_bool = 0, t_bool = 0;
    /* char ptr stores filename we want to write to */
    char *file_name;
    /* filename we will write to */
    char *res_filename;

    while((Option = getopt(argc, argv, "n:p:t")) != -1){
        switch (Option) {
        case 'n': /* Number of addresses to process */
            addr_limit = atoi(optarg);
        break;
        case 'p': /* produce map of pages */
            file_name = optarg;
            /* char ptr iterates over char array & finds any '.' char */
            char *ext_check;
            ext_check = strrchr(file_name, '.');
            if(ext_check != NULL){
                res_filename = file_name;
                p_bool = 1;
            }
            /* no extension found, so we add one */
            else{
                /* concatenate .odf extension to non-file string */
                char *ext = ".odf";
                /* provide sufficient size of char array */
                /* +2 included for null terminators */
                char *filestr;
                filestr = new char[int(strlen(ext)+strlen(file_name)+2)];
                strcpy(filestr, file_name);
                strcat(filestr, ext);
                res_filename = filestr;
                p_bool = 1;
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
    /* size of level bits determined by total args - optind - 1 for trfile */
    int level_bits[argc - optind - 1];
    int level_idx = 0;

    /* run while loop to obtain remaining arguments */
    while(argv[opt_idx] != nullptr){
        /* if is a number then add to an array */
        if(isdigit(*argv[opt_idx])){
            page_num_bits += atoi(argv[opt_idx]);
            level_bits[level_idx] = atoi(argv[opt_idx]);
            level_idx++;
            levels++;
        }
        else{
            /* if not digit, we assume optarg is a trace file */
            filename = argv[opt_idx];
        }
        opt_idx++;
    }

    /* pagetable used by levels as a center of information */
    /* pagetable initialized with root node level */
    Pagetable *pt = new Pagetable(levels, level_bits);
    Level *level_zero = new Level(pt, 0, pt->EntryCount[0]);
    /* link pagetable to initial level with pointer */
    pt->RootNodePtr = level_zero;

    /* set file ptr and address array ptr for trace file */
    FILE *trfile;
    p2AddrTr *addrPtr = (p2AddrTr*)malloc(sizeof(p2AddrTr));
    int adr_counter = 0;

    /* read through trace file if exists */
    if((trfile = fopen(filename, "r"))){
        if(addr_limit != 0){
            /* only processes specific amount of addresses */
            while(NextAddress(trfile, addrPtr) && adr_counter < addr_limit){
                /* if the current frame is added we iterate it */
                if(PageInsert(pt, addrPtr->addr, frame) != false){
                    frame++;
                }
                adr_counter++;
            }
        }
        else{
            /* reads values in trace file that are inserted into page table */
            while(NextAddress(trfile, addrPtr) != 0){
                if(PageInsert(pt, addrPtr->addr, frame) != false){
                    frame++;
                }
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
        /* reopens and rereads original file with original ptr */
        trfile = fopen(filename, "r");
        p2AddrTr *aPtr = (p2AddrTr*)malloc(sizeof(p2AddrTr));
        int addr_counter = 0;
        if(addr_limit != 0){
            /* only processes addr_counter amount of addresses */
            while(NextAddress(trfile, aPtr) && addr_counter < addr_limit){
                /* map object used to find addresses frame */
                Map *mpPtr = new Map();
                mpPtr = PageLookUp(pt, aPtr->addr);
                LogicalToPhysicalAddr(aPtr->addr, offset_bitmask,
                                      mpPtr->frame, page_size);
                addr_counter++;
            }
        }
        else{
            while(NextAddress(trfile, aPtr)){
                Map *mpPtr = new Map();
                mpPtr = PageLookUp(pt, aPtr->addr);
                LogicalToPhysicalAddr(aPtr->addr, offset_bitmask,
                                      mpPtr->frame, page_size);
            }
        }
        fclose(trfile);
    }
    /* write to specified file with ordered hex addresses and their frames */
    if(p_bool == 1){
        /* rather than file ptrs we use ofstream to output hex strings */
        /* will write to .odf file if not specified */
        ofstream writeFile(res_filename);
        PageTableTraversal(pt, writeFile);
        writeFile.close();
    }

    /* print out basic summary */
    cout << "Page size: " << page_size << endl;
    cout << "Number of levels: " << pt->LevelCount << endl;
    cout << "Entries: ";
    for(int i = 0; i < pt->LevelCount; i++){
        cout << pt->EntryCount[i] << " ";
    }
    cout << endl;
    cout << "Shifts: ";
    for(int i = 0; i < pt->LevelCount; i++){
        cout << pt->ShiftAry[i] << " ";
    }
    cout << endl;

    return 0;
}
