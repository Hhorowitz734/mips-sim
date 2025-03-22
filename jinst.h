#ifndef JINST
#define JINST
#include <stdio.h>
#include "util.h"

typedef enum EXACT_INST_J {
    J,

    J_ERR



} EXACT_INST_J;

const char* exact_inst_j_to_string(EXACT_INST_J inst) {
    switch(inst) {
        case J: return "J";
        case J_ERR: return "J_ERR";
        default: return "ERR";
    }
}

EXACT_INST_J opcode_to_exact_inst_j(int opcode) {
    switch(opcode) {
        case 2: return J;
        default: return J_ERR;
    }
}


typedef struct J_Instruction {
    int opcode;
    int address;

    EXACT_INST_J exact_instruction;
} J_Instruction;



J_Instruction* populate_j_instruction(const char* istring) {
    J_Instruction* inst = (J_Instruction*)malloc(sizeof(J_Instruction));

    inst->opcode = substring_as_int(istring, 0, 5, false);
    inst->address = substring_as_int(istring, 6, 31, false);
    inst->exact_instruction = opcode_to_exact_inst_j(inst->opcode);

    return inst;
}





void write_jtype(const char* istring, const char* output_filename) {

    FILE *outputfile = fopen(output_filename, "a");
    if (outputfile == NULL) {
        perror("Error opening output file");
        exit(0);
    }

    fwrite(istring, sizeof(char), 6, outputfile);  
    fprintf(outputfile, " ");    
    fwrite(istring + 6, sizeof(char), 26, outputfile); 
    fprintf(outputfile, "\t");

    J_Instruction* inst = populate_j_instruction(istring);
    fprintf(outputfile, "%s ", exact_inst_j_to_string(inst->exact_instruction));
    fprintf(outputfile, "%d", inst->address);

    fprintf(outputfile, "\n");

    free(inst);
    fclose(outputfile);
}


#endif
