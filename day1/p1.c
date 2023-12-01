
#include "stdio.h"
#include "string.h"


unsigned long sum = 0;

void parseLine(char *p_line, size_t len) {
    unsigned char firstNum = 0;
    unsigned char lastNum = 0;
    unsigned char num = 0;
    unsigned char result;
    for(size_t i=0; i<strlen(p_line); i++) {
        unsigned char value = (unsigned char)p_line[i];
        if (value >= '0' && value <='9') {
            if (num > 0) {
                lastNum = value - '0';
            } else {
                firstNum = value - '0';
            }
            ++num;
        }
    }
    if (num == 1) {
        lastNum = firstNum;
    }
    result = (firstNum*10 + lastNum);
    sum += result;
    printf("%s;%d -> %d -> %lu\n", p_line, num, result, sum);
}

int main(void) {
    char *p_buffer = NULL;
    size_t len = 0;
    while (getline(&p_buffer, &len, stdin)>=0) {
        parseLine(p_buffer, len);
    }
    printf("Result: %lu\n", sum);
}