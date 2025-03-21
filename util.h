#ifndef UTIL
#define UTIL



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

#endif
