#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    //Provided Addresses
    int addresses[] = {1, 256, 32768, 32769, 128, 65534, 33153};
    
    //number of addresses in the array
    int size = sizeof(addresses) / sizeof(addresses[0]);

    //go through each logical address

    for (int i = 0; i < size; i++) {

        //get the logical address
        int logical_addr = addresses[i];

        //masking only the right 16 bits
        logical_addr = logical_addr & 0xFFFF;
        
        //extract the page num
        int pg_num = (logical_addr >> 8) & 0xFF;

        //extracting the offset
        int offset = logical_addr & 0xFF;
        // display results
        printf("Logical Address: %d\n", logical_addr);
        printf("Page Number: %d\n", pg_num);
        printf("Offset: %d\n", offset);
        printf("------------------------\n");
    }
    return 0;
}