#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rinst.h"
#include "iinst.h"


typedef enum I_TYPE {
    R_Type,
    I_Type,
    J_Type,
    FP_Type // floating point
} I_TYPE;

char* itype_to_string(I_TYPE itype) {
    switch(itype) {
        case R_Type:
            return "R-Type";
        case I_Type:
            return "I-Type";
        case J_Type:
            return "J-Type";
        case FP_Type:
            return "FP-Type";
        default:
            return "Invalid Itype";
    }
}

I_TYPE populate_itype(const char* istring) {

    // Check memory safety
    if (istring == NULL) {
        perror("populate_itype() could not read istring.");
    }

    // Check floating point opcode "010001"
    if (strncmp(istring, "010001", 6) == 0) {
        return FP_Type;
    }
    
    // R Type -> Opcode 000000
    if (strncmp(istring, "000000", 6) == 0) {
        return R_Type;
    }

    // J Type
    if ((strncmp(istring, "000010", 6) == 0) || (strncmp(istring, "000011", 6) == 0)) {
        return J_Type;
    }

    // All else I Type
    return I_Type;


}


char* get_istring(FILE *file) {
    char* buffer = (char*)malloc(33 * sizeof(char));
    if (buffer == NULL) {
        perror("Memory allocation failed in get_istring()");
        return NULL;
    }

    if (fgets(buffer, 33, file) == NULL) {
        free(buffer);
        return NULL;
    }

    size_t len = strlen(buffer);

    // If exactly 32 chars were read and the next char is a newline, consume it
    if (len == 32) {
        int c = fgetc(file);
        if (c != '\n' && c != EOF) {
            ungetc(c, file);  // put back anything unexpected
        }
    }

    if (len < 32) {
        free(buffer);
        return NULL;
    }

    buffer[32] = '\0';
    return buffer;
}

int main() {
    
    const char *input_filename = "a.txt";
    const char *output_filename = "output.txt";

    FILE *file = fopen(input_filename, "r");
    if (file == NULL) {
        perror("Could not open file");
        return 1;
    }

    while (1) {
        char *istring = get_istring(file);
        if (istring == NULL) {
            break;
        }

        //printf("Istring: %s\n", istring);
        I_TYPE itype = populate_itype(istring);
        
        char* its = itype_to_string(itype);
        //printf("%s\n", its);
        //printf("\n");
        
        if (itype == R_Type) { 
            write_rtype(istring, output_filename);
        }
        else if (itype == I_Type) {
            write_itype(istring, output_filename);
        }
        free(istring);
    }

    fclose(file);
    return 0;
}
