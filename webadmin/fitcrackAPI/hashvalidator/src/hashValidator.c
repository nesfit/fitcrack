#include <stdio.h>
#include "types.h"
#include "interface.h"



int fitcrackHashValidator(int parserResult, char* hash  ) {
    
    if (parserResult == 0) {
        printf("%s OK\n", hash);
    } else {
        printf("%s %s\n", hash, strparser (parserResult));
    }
    return 0;
}

