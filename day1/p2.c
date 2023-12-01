
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
        } else if ((value >= 'A' && value <= 'Z')
            || (value >= 'a' && value <= 'z')) {
            unsigned char n=0;
            for(; n<9; n++) {
                const char* p_number = numbers[n];
                size_t l = strlen(p_number);
                if(l <= (len - i)) {
                    unsigned char m=0;
                    for(; m<l; m++) {
                        if(p_number[m] != p_line[i+m]) {
                            break;
                        }
                    }
                    if(m == l) {
                        if (num > 0) {
                            lastNum = n+1;
                            printf("%d ", lastNum);
                        } else {
                            firstNum = n+1;
                            printf("%d ", firstNum);
                        }
                        ++num;
                        break;
                    }
                }
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