
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_LINES 100
#define NUM_INT 100


unsigned int numLines = 0;
long array[NUM_LINES][NUM_INT];
unsigned int intNums[NUM_LINES];

unsigned char searchNumber(char * p_line,
    long * p_number, size_t * p_numStartCol, size_t * p_numChars) {
    unsigned char result = 0;
    if (p_line != NULL) {
        size_t lineLen = strlen(p_line);
        unsigned char found = 0;
        size_t col;
        unsigned char negative = 0;
        for (col = 0; col < lineLen; col++) {
            char c = p_line[col];
            if (c >= '0' && c <= '9') {
                if (!found) {
                    *p_numStartCol = col;
                    *p_number = c - '0';
                    *p_numChars = 1;
                    found = 1;
                } else {
                    *p_number = *p_number * 10 + (c - '0');
                    *p_numChars += 1;
                }
            } else if (!found && c == '-') {
                *p_numStartCol = col;
                *p_numChars = 1;
                negative = 1;
            } else {
                if (found) {
                    if (negative) {
                        *p_number *= -1;
                    }
                    result = 1;
                    break;
                }
            }
        }
        if (col == lineLen && found == 1) {
            if (negative) {
                *p_number *= -1;
            }
            result = 1;
        }
    }
    //printf(" -- %s %ld\n", p_line, *p_number);
    return (result);
}


int main(void) {
    char *p_buffer = NULL;
    long result = 0;
    size_t len;
    unsigned int error = 0;
    unsigned long inputLineNum = 0;

    while (getline(&p_buffer, &len, stdin)>=0 && !error) {
        size_t str_len = strlen(p_buffer);
        if (str_len > 0) {
            if (str_len >= 1) {
                if (p_buffer[str_len-1] == '\n') {
                    p_buffer[str_len-1] = '\0';
                    str_len -= 1;
                }
            }
        }
        printf("** %s\n", p_buffer);
        char* next = p_buffer;
        unsigned int intNum = 0;
        size_t startCol;
        size_t numChars;
        printf(" -");
        while ((len = strlen(next)) && (intNum < NUM_INT)) {
            if (searchNumber(next, &(array[0][intNum]), &startCol, &numChars)) {
                printf(" %ld", array[0][intNum]);
                ++intNum;
                next += (startCol + numChars);
            } else {
                printf("Parse error in input line: %lu intNum: %d\n", inputLineNum, intNum);
                error = 1;
            }
        }
        if (intNum < NUM_INT) {
            printf(" : %d\n", intNum);
            intNums[0] = intNum;
        } else {
            printf("Array overflow in input line: %lu intNum: %d\n", inputLineNum, intNum);
            error = 1;
        }

        if (!error) {
            numLines = 1;
            unsigned char finish = 0;
            for(unsigned int i = 0; i < numLines && i < (NUM_LINES-1) && !finish; i++) {
                unsigned int numInts = intNums[i];
                printf(" --");
                if (numInts >= 3) {
                    long refNum;
                    finish = 1;
                    for(intNum=0; intNum<(numInts-1); intNum++) {
                        array[i+1][intNum] = array[i][intNum+1] - array[i][intNum];
                        printf(" %ld", array[i+1][intNum]);
                        if (intNum > 0) {
                            if (array[i+1][intNum] != refNum) {
                                finish = 0;
                            }
                            ++intNums[i+1];
                        } else {
                            refNum = array[i+1][intNum];
                            intNums[i+1] = 1;
                        }
                    }
                } else if (numInts == 2) {
                    array[i+1][0] = array[i][1] - array[i][0];
                    printf(" %ld", array[i+1][0]);
                    intNums[i+1] = 1;
                    finish = 1;
                } else {
                    printf("Only one number in: %lu\n", inputLineNum);
                    error = 1;
                }
                if (!finish && !error) {
                    ++numLines;
                }
                printf(" : %d\n", intNum);
            }
        }

        if (numLines >= (NUM_LINES-1)) {
                    printf("Array overlow, too less lines!\n");
                    error = 1;
        }

        if (!error) {
            long nextValue = array[numLines][0];
            printf("NextValue:");
            do {
                --numLines;
                nextValue = array[numLines][0] - nextValue;
                printf(" %ld", nextValue);
            } while (numLines);
            result += nextValue;
            printf(" : Accumulated: %ld\n", result);
        }
        ++inputLineNum;
    }
    if (p_buffer != NULL) {
        free(p_buffer);
    }


    printf("Result: %ld\n", result);
}
