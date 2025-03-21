#ifndef RINST
#define RINST
#include <stdio.h>

typedef enum EXACT_INST {
    // Arithmetic Instructions
    ADD,
    ADDU,
    SUB,
    SUBU,

    // Multiplication & Division
    MULT,
    MULTU,
    DIV,
    DIVU,

    // Logical Operations
    AND,
    OR,
    XOR,
    NOR,

    // Shift Operations
    SLL,
    SRL,
    SRA,

    // Comparison Instructions
    SLT,
    SLTU,

    // Jump Instructions
    JR,

    ERR
} EXACT_INST;


typedef struct R_Instruction {
    char* istring;
    
    EXACT_INST exact_instruction; 

    int rs;
    int rt;
    int rd;
    int shamt;
    int funct;

} R_Instruction;


const char* exact_inst_to_string(EXACT_INST inst) {
    switch (inst) {
        // Arithmetic Instructions
        case ADD: return "ADD";
        case ADDU: return "ADDU";
        case SUB: return "SUB";
        case SUBU: return "SUBU";

        // Multiplication & Division
        case MULT: return "MULT";
        case MULTU: return "MULTU";
        case DIV: return "DIV";
        case DIVU: return "DIVU";

        // Logical Operations
        case AND: return "AND";
        case OR: return "OR";
        case XOR: return "XOR";
        case NOR: return "NOR";

        // Shift Operations
        case SLL: return "SLL";
        case SRL: return "SRL";
        case SRA: return "SRA";

        // Comparison Instructions
        case SLT: return "SLT";
        case SLTU: return "SLTU";

        // Jump Instructions
        case JR: return "JR";

        // Error / Unknown Instruction
        case ERR:
        default: return "ERR"; 
    }
}

EXACT_INST funct_to_exact_inst(int funct) {
    switch (funct) {
        case 32:
            return ADD;
        case 33:
            return ADDU;
        case 34:
            return SUB;
        case 35:
            return SUBU;
        case 36:
            return AND;
        case 37:
            return OR;
        case 38:
            return XOR;
        case 39:
            return NOR;
        case 0:
            return SLL;
        case 2:
            return SRL;
        case 3:
            return SRA;
        case 8:
            return JR;
        case 42:
            return SLT;
        case 43:
            return SLTU;
        default:
            return ERR;
    }
}



int substring_as_int(const char *str, int i, int j, bool little_endian) {
    // Check that range is in bounds 
    if (i < 0 || j < i || j >= strlen(str)) {
        printf("Invalid range\n");
        return -1;  // Error case
    }

    int result = 0;

    // Little endian
    if (little_endian) {
        int power = 0;
        for (int k = i; k <= j; k++, power++) {
            if (str[k] == '1') { result += 1 << power; }
        }
        return result;
    }
 
    for (int k = i; k <= j; k++) {
        result = (result << 1) | (str[k] - '0');  // Shift left and add bit
    }

    return result;
}


R_Instruction* populate_r_instruction(const char* istring) {
    
    R_Instruction* inst = (R_Instruction*)malloc(sizeof(R_Instruction));

    inst->rs = substring_as_int(istring, 6, 10, false);
    inst->rt = substring_as_int(istring, 11, 15, false); 
    inst->rd = substring_as_int(istring, 16, 20, false);
    inst->shamt = substring_as_int(istring, 21, 25, false);
    inst->funct = substring_as_int(istring, 26, 31, false);
    inst->exact_instruction = funct_to_exact_inst(inst->funct);

    return inst;
}

void write_rtype(const char* istring, const char* output_filename) {

    FILE *outputfile = fopen(output_filename, "a");
    if (outputfile == NULL) {
        perror("Error opening output file");
        exit(0);
    }
    
    // Write instruction split up
    fwrite(istring, sizeof(char), 6, outputfile);  
    fprintf(outputfile, " ");    
    fwrite(istring + 6, sizeof(char), 5, outputfile);
    fprintf(outputfile, " ");    
    fwrite(istring + 11, sizeof(char), 5, outputfile);
    fprintf(outputfile, " ");    
    fwrite(istring + 16, sizeof(char), 5, outputfile);
    fprintf(outputfile, " ");    
    fwrite(istring + 21, sizeof(char), 5, outputfile);
    fprintf(outputfile, " ");    
    fwrite(istring + 26, sizeof(char), 5, outputfile);
    fprintf(outputfile, "\t");    

    // Decode + write decoded instruction
    R_Instruction* inst = populate_r_instruction(istring);
    fprintf(outputfile, "%s", exact_inst_to_string(inst->exact_instruction)); 
    fprintf(outputfile, " ");   
    fprintf(outputfile, "$t%d,", (inst->rd - 8));
    fprintf(outputfile, " ");    
    fprintf(outputfile, "$t%d,", (inst->rs - 8));
    fprintf(outputfile, " ");   
    fprintf(outputfile, "$t%d", (inst->rt - 8)); 


    
    fclose(outputfile);
    return;
}

#endif
