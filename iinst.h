#ifndef IINST
#define IINST
#include <stdio.h>
#include "util.h"

typedef enum EXACT_INST_I {
    // Branch
    BEQ, BNE, BGEZ, BGTZ, BLEZ, BLTZ,

    // Add Immediate
    ADDI, ADDIU,

    // Store/Load
    SW, LW,

    // SLTI
    SLTI,

    // Error
    I_ERR
} EXACT_INST_I;


typedef struct I_Instruction {
    char* istring;
    
    int opcode;
    int rs;
    int rt;
    int immediate;
    EXACT_INST_I exact_instruction;

} I_Instruction;



const char* exact_inst_i_to_string(EXACT_INST_I inst) {
    switch (inst) {
        case BEQ:   return "BEQ";
        case BNE:   return "BNE";
        case BGEZ:  return "BGEZ";
        case BGTZ:  return "BGTZ";
        case BLEZ:  return "BLEZ";
        case BLTZ:  return "BLTZ";
        case ADDI:  return "ADDI";
        case ADDIU: return "ADDIU";
        case LW:    return "LW";
        case SW:    return "SW";
        case SLTI:  return "SLTI";
        case I_ERR:
        default:    return "I_ERR";
    }
}

 

EXACT_INST_I opcode_rt_to_exact_inst_i(int opcode, int rt) {
    switch (opcode) {
        case 4:  return BEQ;
        case 5:  return BNE;
        case 6:  return BLEZ;
        case 7:  return BGTZ;
        case 8:  return ADDI;
        case 9:  return ADDIU;
        case 10: return SLTI;
        case 35: return LW;
        case 43: return SW;
        case 1:  // special case
            if (rt == 0) return BLTZ;
            else if (rt == 1) return BGEZ;
            else return I_ERR;
        default:
            return I_ERR;
    }
}

I_Instruction* populate_i_instruction(const char* istring) {
    I_Instruction* inst = (I_Instruction*)malloc(sizeof(I_Instruction));

    inst->opcode = substring_as_int(istring, 0, 5, false);
    inst->rs = substring_as_int(istring, 6, 10, false);
    inst->rt = substring_as_int(istring, 11, 15, false);
    inst->immediate = substring_as_int(istring, 16, 31, false);
    inst->exact_instruction = opcode_rt_to_exact_inst_i(inst->opcode, inst->rt);

    return inst;
}



void write_itype(const char* istring, const char* output_filename) {

    FILE *outputfile = fopen(output_filename, "a");
    if (outputfile == NULL) {
        perror("Error opening output file");
        exit(0);
    }
    
    // Write instruction fields
    fwrite(istring, sizeof(char), 6, outputfile);
    fprintf(outputfile, " ");
    fwrite(istring + 6, sizeof(char), 5, outputfile);
    fprintf(outputfile, " ");
    fwrite(istring + 11, sizeof(char), 5, outputfile);
    fprintf(outputfile, " ");
    fwrite(istring + 16, sizeof(char), 16, outputfile);
    fprintf(outputfile, "\t");

    // Decode and write instruction
    I_Instruction* inst = populate_i_instruction(istring);
   
    fprintf(outputfile, "%s ", exact_inst_i_to_string(inst->exact_instruction));
    if (inst->exact_instruction == BEQ || inst->exact_instruction == BNE ||
        inst->exact_instruction == BGEZ || inst->exact_instruction == BLTZ ||
        inst->exact_instruction == BLEZ || inst->exact_instruction == BGTZ) {
        // Branch form: rs, rt, offset
        fprintf(outputfile, "$t%d, ", (inst->rs - 8));
        fprintf(outputfile, "$t%d, ", (inst->rt - 8));
        fprintf(outputfile, "%d", inst->immediate);
    } else {
        // Standard I-type: rt, rs, immediate
        fprintf(outputfile, "$t%d, ", (inst->rt - 8));
        fprintf(outputfile, "$t%d, ", (inst->rs - 8));
        fprintf(outputfile, "%d", inst->immediate);
    }


    fprintf(outputfile, "\n");

    free(inst);
    fclose(outputfile);
}


#endif
