
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* numbers[9] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};


unsigned long sum = 0;
unsigned long lineNum = 0;

void parseLine(char *p_line, size_t len) {
    unsigned char firstNum = 0;
    unsigned char lastNum = 0;
    unsigned char num = 0;
    unsigned char result;
    lineNum++;
    printf("%lu - %s - ", lineNum, p_line);
    for(size_t i=0; i<len; i++) {
        unsigned char value = (unsigned char)p_line[i];
        if (value >= '0' && value <='9') {
            if (num > 0) {
                lastNum = value - '0';
                printf("%d ", lastNum);
            } else {
                firstNum = value - '0';
                printf("%d ", firstNum);
            }
            ++num;
        } else if (value >= 'a' && value <= 'z') {
            unsigned int rowCheck = 0x01FF;
            size_t remain = len - i;               
            unsigned char col = 0;
            char found = -1;
            //printf("\n%s\n", &p_line[i]);
            while ((found < 0) && rowCheck && (remain >= 0)) {
                //printf("col: %d\n", col);
                unsigned int rowMask = 0x1;
                for (unsigned char row = 0; row < 9; row++) {
                    if ((rowCheck & rowMask)) {
                        //printf("row: %d check: %x mask: %x num: %s ", row, rowCheck, rowMask, numbers[row]);
                        const char * p_number = numbers[row];
                        if ((col < strlen(p_number))) {
                            if (p_number[col] != p_line[i+col]) {
                                rowCheck &= ~rowMask;
                                //printf("No match!\n");
                            } else {
                                //printf("Match!\n");
                            }
                        } else {
                            found = (char)row+1;
                            //printf("Found: %d\n", found);
                            break;
                        }
                    }
                    rowMask <<= 1;
                }
                col++;
                remain--;
            }

            if (found >= 0) {
                if (num > 0) {
                    lastNum = (unsigned char)found;
                    printf("%d ", lastNum);
                } else {
                    firstNum = (unsigned char)found;
                    printf("%d ", firstNum);
                }
                num++;
            }
        }
    }
    if (num == 1) {
        lastNum = firstNum;
    }
    result = (firstNum*10 + lastNum);
    sum += result;
    printf(" - %d -> %d -> %lu\n", num, result, sum);
}

int main(void) {
    char *p_buffer = NULL;
    size_t len = 0;
    while (getline(&p_buffer, &len, stdin)>=0) {
        size_t str_len = strlen(p_buffer);
        if (str_len > 0) {
            if (str_len >= 1) {
                if (p_buffer[str_len-1] == '\n') {
                    p_buffer[str_len-1] = '\0';
                    str_len -= 1;
                }
            } 
            if (str_len > 0) {
                parseLine(p_buffer, str_len);
            }
        }
    }
    if (p_buffer != NULL) {
        free(p_buffer);
    }
    printf("Result: %lu\n", sum);
}