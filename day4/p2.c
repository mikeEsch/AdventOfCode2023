
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long sum = 0;

#define NUM_CARDS 216
unsigned int win[10];
unsigned int avail[25];
unsigned int addCards[NUM_CARDS];

unsigned char searchNumber(char * p_line,
    unsigned int * p_number, size_t * p_numStartCol, size_t * p_numChars) {
    size_t lineLen = strlen(p_line);
    unsigned char result = 0;
    unsigned char found = 0;
    size_t col;

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
        } else {
            if (found) {
                result = 1;
                break;
            }
        }
    }
    if (col == lineLen && found == 1) {
        result = 1;
    }

    return (result);
}


int main(void) {
    char *p_buffer = NULL;
    size_t len = 0;
    unsigned int cardNum = 0;
    for (unsigned int i = 0; i<NUM_CARDS; i++) addCards[i] = 0;
    while (getline(&p_buffer, &len, stdin)>=0 && cardNum<NUM_CARDS) {
        size_t str_len = strlen(p_buffer);
        if (str_len > 0) {
            if (str_len >= 1) {
                if (p_buffer[str_len-1] == '\n') {
                    p_buffer[str_len-1] = '\0';
                    str_len -= 1;
                }
            }
            unsigned int sumCards = (addCards[cardNum] + 1);
            printf("Card #%d - num:%d - %s\n", (cardNum+1), sumCards, p_buffer);
            unsigned int col = 10;
            unsigned int number;
            size_t startCol, numChars;
            printf(" - Win:");
            for (int i = 0; i<10; i++) {
                if (searchNumber(&p_buffer[col], &number, &startCol, &numChars)) {
                    win[i] = number;
                    printf(" %d", number);
                    col += (startCol+numChars);
                } else {
                    printf("\nError: col: %d\n", col);
                    exit(-1);
                }
            }
            printf("\n - Avial:");
            for (int i=0; i<25; i++) {
                if (searchNumber(&p_buffer[col], &number, &startCol, &numChars)) {
                    avail[i] = number;
                    printf(" %d", number);
                    col += (startCol+numChars);
                } else {
                    printf("\nError: col: %d\n", col);
                    exit(-1);
                }
            }
            printf("\n");

            unsigned int value = 0;
            printf(" - Found: ");
            for (unsigned int i = 0; i<10; i++) {
                for (unsigned int n = 0; n<25; n++) {
                    if(win[i] == avail[n]) {
                        printf(" %d", win[i]);
                        ++value;
                        break;
                    }
                }
            }
            printf("\n");
            sum += sumCards;
            for (unsigned int i = 0; i < value; i++) {
                unsigned int idx = cardNum + 1 + i;
                if (idx < NUM_CARDS) {
                    addCards[idx] += sumCards;
                }
            }
            printf(" - value: %d numCards: %d sum: %lu\n", value, sumCards, sum);
            ++cardNum;
        }
    }


    if (p_buffer != NULL) {
        free(p_buffer);
    }
    printf("Result: %lu\n", sum);
}