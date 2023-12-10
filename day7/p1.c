
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_HANDS 1000
#define NUM_CARDS 5

typedef struct {
    char card[NUM_CARDS+1];
    unsigned int bid;
    unsigned int value;
} T_HAND;

T_HAND hands[NUM_HANDS];

const char* cardValues = "AKQJT98765432";
#define NUM_VALUES strlen(cardValues)


unsigned char searchNumber(char * p_line,
    unsigned int * p_number, size_t * p_numStartCol, size_t * p_numChars) {
    unsigned char result = 0;
    if (p_line != NULL) {
        size_t lineLen = strlen(p_line);
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
    }
    return (result);
}

int main(void) {
    char *p_buffer = NULL;
    size_t len = 0;
    char * next = NULL;
    unsigned int handNum = 0;
    unsigned long result = 0;
    while (getline(&p_buffer, &len, stdin)>=0 && handNum<NUM_HANDS) {
        size_t str_len = strlen(p_buffer);
        if (str_len > 0) {
            if (str_len >= 1) {
                if (p_buffer[str_len-1] == '\n') {
                    p_buffer[str_len-1] = '\0';
                    str_len -= 1;
                }
            }
        }
        next = p_buffer;
        //printf("%s\n", p_buffer);
        char * hand = strsep(&next, " ");
        //printf("%s %s\n", hand, next);
        strncpy(hands[handNum].card, hand, NUM_CARDS+1);
        size_t startCol, numChars;
        searchNumber(next, &(hands[handNum].bid), &startCol, &numChars);
        printf("Card: %d card: %s bid: %d\n", handNum, hands[handNum].card, hands[handNum].bid);
        ++handNum;
    }
    if (p_buffer != NULL) {
        free(p_buffer);
    }

    for (unsigned int i=0; i<handNum; i++) {
        for (unsigned int n=0; n<NUM_CARDS; n++) {
            switch (hands[i].card[n]) {
                case 'A':
                    hands[i].card[n] = 0xF;
                    break;
                case 'K':
                    hands[i].card[n] = 0xD;
                    break;
                case 'Q':
                    hands[i].card[n] = 0xC;
                    break;
                case 'J':
                    hands[i].card[n] = 0xB;
                    break;
                case 'T':
                    hands[i].card[n] = 0xA;
                    break;
                case '9':
                    hands[i].card[n] = 9;
                    break;
                case '8':
                    hands[i].card[n] = 8;
                    break;
                case '7':
                    hands[i].card[n] = 7;
                    break;
                case '6':
                    hands[i].card[n] = 6;
                    break;
                case '5':
                    hands[i].card[n] = 5;
                    break;
                case '4':
                    hands[i].card[n] = 4;
                    break;
                case '3':
                    hands[i].card[n] = 3;
                    break;
                case '2':
                    hands[i].card[n] = 2;
                    break;
                default:
                    printf("Wrong card value!!!\n");
                    exit(-1);
                    break;
            }
        }
        hands[i].value = 0;
    }

    for (unsigned int i=0; i<handNum; i++) {
        unsigned int cardCount[NUM_CARDS];
        unsigned int cardIdx = 0;
        char cards[NUM_CARDS];
        memcpy(cards, hands[i].card, NUM_CARDS);
        memset(cardCount, 0, NUM_CARDS*sizeof(unsigned int));
        for (unsigned int n=0; n<NUM_CARDS; n++) {
            char c = cards[n];
            if (c != 0) {
                ++cardCount[cardIdx];
                for (unsigned int m = n+1; m<NUM_CARDS; m++) {
                    if (cards[m] == c) {
                        ++cardCount[cardIdx];
                        cards[m] = 0;
                    }
                }
            }
            ++cardIdx;
        }

        unsigned int found = 0;
        printf("card: %x %x %x %x %x count: %d %d %d %d %d",
            hands[i].card[0], hands[i].card[1], hands[i].card[2], hands[i].card[3], hands[i].card[4],
            cardCount[0], cardCount[1], cardCount[2], cardCount[3], cardCount[4]);

        for (unsigned int n=0; n<NUM_CARDS && !found; n++) {
            switch (cardCount[n]) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    if (n <4) {
                        for (unsigned int m=n+1; m<NUM_CARDS && !found; m++) {
                            switch (cardCount[m]) {
                                case 3:
                                    hands[i].value = 5;
                                    found = 1;
                                    break;
                                case 2:
                                    hands[i].value = 3;
                                    found = 1;
                                    break;
                            }
                        }
                    }
                    if (!found) {
                        hands[i].value = 2;
                        found = 1;
                    }
                    break;
                case 3:
                    if (n <3) {
                        for (unsigned int m=n+1; m<NUM_CARDS && !found; m++) {
                            if(cardCount[m]==2) {
                                hands[i].value = 5;
                                found = 1;
                            }
                        }
                        
                    }
                    if (!found) {
                        hands[i].value = 4;
                        found = 1;
                    }
                    break;
                case 4:
                    hands[i].value = 6;
                    found = 1;
                    break;
                case 5:
                    hands[i].value = 7;
                    found = 1;
                    break;
                default:
                    break;
            }
        }
        if (!found) hands[i].value = 1;
        printf(" value: %d\n", hands[i].value);
    }

    unsigned int swapped;
    do {
        swapped = 0;
        for (unsigned int i=0; i<handNum; i++) {
            for (unsigned int n=i+1; n<handNum; n++) {
                if (hands[i].value > hands[n].value) {
                    T_HAND dummy;
                    memcpy((void*)&dummy, (void*)&hands[i], sizeof(T_HAND));
                    memcpy((void*)&hands[i], (void*)&hands[n], sizeof(T_HAND));
                    memcpy((void*)&hands[n], (void*)&dummy, sizeof(T_HAND));
                    swapped = 1;
                } else if (hands[i].value == hands[n].value) {
                    for (unsigned int m = 0; m < NUM_CARDS; m++) {
                        if (hands[i].card[m] > hands[n].card[m]) {
                            T_HAND dummy;
                            memcpy((void*)&dummy, (void*)&hands[i], sizeof(T_HAND));
                            memcpy((void*)&hands[i], (void*)&hands[n], sizeof(T_HAND));
                            memcpy((void*)&hands[n], (void*)&dummy, sizeof(T_HAND));
                            swapped = 1;
                            break;
                        } else if (hands[i].card[m] < hands[n].card[m]) {
                            break;
                        }
                    }
                }
            }
        }
    } while (swapped);

    result = 0;
    for (unsigned int i=0; i<handNum; i++) {
        printf("rank: %d card: %x %x %x %x %x value: %d bid: %d\n",
            i, hands[i].card[0], hands[i].card[1], hands[i].card[2], hands[i].card[3], hands[i].card[4],
            hands[i].value, hands[i].bid);
        result += (hands[i].bid * (i+1));
    }

    printf("Result: %lu\n", result);
    return (0);
}
