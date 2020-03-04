#define BADFLAG 1
#include <iostream>
#include <unistd.h>
#include "structures.h"

using namespace std;

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
    int idx = optind;
    while(argv[idx] != nullptr){
        cout << argv[idx] << endl;
        idx++;
    }

    Pagetable *pt = new Pagetable(3);

    Level *level_zero = new Level(pt, 1, 256);
    //cout << level_zero->PageTablePtr->LevelCount << endl;
    //cout << level_zero->CurrentDepth << endl;

}
