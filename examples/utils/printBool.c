#include <stdio.h>

typedef struct bool {
    unsigned value: 1;
} bool;

void printBool(bool b) {
    if (b.value == 0b0) {
        printf("Printing boolean: False\n");
    } else if (b.value == 0b1) {
        printf("Printing boolean: True\n");
    } else {
        printf("Printing boolean: Unknown value\n");
    }
}
