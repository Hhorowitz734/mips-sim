#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef enum I_TYPE {
    R_Type,
    I_Type,
    J_Type,
    FP_Type // floating point
} I_TYPE;

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
    I_TYPE type;

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

EXACT_INST decompose_rtype(const char *istring) {
    // Extract funct (last 6 bytes)
    if (strncmp(istring + 26, "100000", 6) == 0) return ADD;
    if (strncmp(istring + 26, "100001", 6) == 0) return ADDU;
    if (strncmp(istring + 26, "100010", 6) == 0) return SUB;
    if (strncmp(istring + 26, "100011", 6) == 0) return SUBU;
    if (strncmp(istring + 26, "011000", 6) == 0) return MULT;
    if (strncmp(istring + 26, "011001", 6) == 0) return MULTU;
    if (strncmp(istring + 26, "011010", 6) == 0) return DIV;
    if (strncmp(istring + 26, "011011", 6) == 0) return DIVU;
    if (strncmp(istring + 26, "100100", 6) == 0) return AND;
    if (strncmp(istring + 26, "100101", 6) == 0) return OR;
    if (strncmp(istring + 26, "100110", 6) == 0) return XOR;
    if (strncmp(istring + 26, "100111", 6) == 0) return NOR;
    if (strncmp(istring + 26, "000000", 6) == 0) return SLL;
    if (strncmp(istring + 26, "000010", 6) == 0) return SRL;
    if (strncmp(istring + 26, "000011", 6) == 0) return SRA;
    if (strncmp(istring + 26, "101010", 6) == 0) return SLT;
    if (strncmp(istring + 26, "101011", 6) == 0) return SLTU;
    if (strncmp(istring + 26, "001000", 6) == 0) return JR;

    // Default case: Unknown instruction
    return ERR;
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


char* get_istring(const char *filename) {

    // Open file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file");
        return NULL;
    }

    // Allocate memory for string
    char* buffer = (char*)malloc(33 * sizeof(char));
    if (buffer == NULL) {
        perror("Memory allocation failed in get_istring()");
        return NULL;
    }

    // Read out the buffer
    if (fgets(buffer, 33, file) == NULL) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[32] = '\0';
    fclose(file);
    return buffer;
}

int main() {
    
    const char *filename = "a.txt";
    char *istring = get_istring(filename);
    if (istring) {

        printf("Istring: %s\n", istring);
        I_TYPE itype = populate_itype(istring);
        
        char* its = itype_to_string(itype);
        printf("%s\n", its);
        printf("\n");
        
        EXACT_INST exact = decompose_rtype(istring);
        const char* itz = exact_inst_to_string(exact);
        printf("Exact Inst: %s\n", itz);
        
        int rs = substring_as_int(istring, 7, 11, true); // problem here
        printf("Rs: %d\n", rs);
        free(istring);
    }
    return 0;
}
