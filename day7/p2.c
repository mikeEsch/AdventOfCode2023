
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
        hands[handNum].value = 0;
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
                    hands[i].card[n] = 1;
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
        printf("card: %x %x %x %x %x count: %d %d %d %d %d value:",
            hands[i].card[0], hands[i].card[1], hands[i].card[2], hands[i].card[3], hands[i].card[4],
            cardCount[0], cardCount[1], cardCount[2], cardCount[3], cardCount[4]);

        for (unsigned int n=0; n<NUM_CARDS && !found; n++) {
            switch (cardCount[n]) {
                case 5:
                    hands[i].value = 7; // 5 same cards
                    found = 1;
                    break;
                case 4:
                    if (hands[i].card[n] == 0x1) {
                        hands[i].value = 7; // 4 Joker + 1 card
                        found = 1;
                    } else {
                        switch(hands[i].value) {
                            case 10: // 1 Joker
                               hands[i].value = 7; // 4 same cards + 1 Joker
                                found = 1;
                                break;
                            case 1:
                                hands[i].value = 6; // 4 same cards
                                found = 1;
                                break;
                            case 0:
                                hands[i].value = 6; // 4 same cards
                                break;
                            default:
                                printf("Error!\n");
                                exit(-1);
                        }
                    }
                    break;
                case 3:
                    if (hands[i].card[n] == 0x1) { // 3 Joker
                        switch (hands[i].value) {
                            case 2: // 2 same cards
                                hands[i].value = 7; // 3 Joker + 2 same card
                                found = 1;
                                break;
                            case 1:
                                hands[i].value = 6; // 3 Joker + 1 same card
                                found = 1;
                                break;
                            case 0:
                                hands[i].value = 40; // 3 Joker
                                break;
                            default:
                                printf("Error!\n");
                                exit(-1);
                        }
                    } else {
                        switch (hands[i].value) { // 3 same card
                            case 20: // 2 Joker
                                hands[i].value = 7; // 3 same card +2 Joker
                                found = 1;
                                break;
                            case 11: // 1 Joker + 1 card
                                hands[i].value = 6; // 3 same card + 1 Joker + 1 card
                                found = 1;
                                break;
                            case 10:
                                hands[i].value = 6; // 3 same card + 1 Joker
                                found = 1;
                                break;
                            case 2:
                                hands[i].value = 5; // 3 same card + 2 same card
                                found = 1;
                                break;
                            case 1:
                            case 0:
                                hands[i].value = 4; // 3 same card
                                break;
                            default:
                                printf("Error!\n");
                                exit(-1);
                        }
                    }
                    break;
                case 2:
                    if (hands[i].card[n] == 0x1) { // 2 Joker
                        switch (hands[i].value) {
                            case 4: // 3 same cards
                                hands[i].value = 7; // 2 Joker + 3 same card
                                found = 1;
                                break;
                            case 2: // 2 same cards
                                hands[i].value = 6; // 2 Joker + 2 same card
                                found = 1;
                                break;
                            case 1: // 1 card
                                hands[i].value = 21; // 2 Joker + 1 card
                                break;
                            case 0:
                                hands[i].value = 20; // 2 Joker
                                break;
                            default:
                                printf("Error!\n");
                                exit(-1);
                        }
                    } else {
                        switch (hands[i].value) { // 2 same card
                            case 40: // 3 Joker
                                hands[i].value = 7; // 2 same card + 3 Joker
                                found = 1;
                                break;
                            case 21: // 2 Joker + 1 card
                                hands[i].value = 6; // 2 same card + 2 Joker + 1 card
                                found = 1;
                                break;
                            case 20: // 2 Joker
                                hands[i].value = 6; // 2 same card + 2 Joker
                                found = 1;
                                break;
                            case 12:
                                hands[i].value = 5; // 2 same card + 1 Joker + 2 same card
                                found = 1;
                                break;
                            case 11:
                                hands[i].value = 4; // 2 same card + 1 Joker + 1 same card
                                found = 1;
                                break;
                            case 10:
                                hands[i].value = 12; // 2 same card + 1 Joker
                                break;
                            case 4: // 3 same cards
                                hands[i].value = 5; // 2 same card + 3 same card
                                found = 1;
                                break;
                            case 2:
                                hands[i].value = 3; // 2 same card + 2 same card
                                break;
                            case 1:
                            case 0:
                                hands[i].value = 2; // 2 same card
                                break;
                            default:
                                printf("Error!\n");
                                exit(-1);
                        }
                    }
                    break;
                case 1:
                    if (hands[i].card[n] == 0x1) { // 1 Joker
                        switch (hands[i].value) {
                            case 6: // 4 same cards
                                hands[i].value = 7; // 1 Joker + 4 same card
                                found = 1;
                                break;
                            case 4: // 3 same cards
                                hands[i].value = 6; // 1 Joker + 3 same card
                                found = 1;
                                break;
                            case 3: // 2 same cards + 2 same cards
                                hands[i].value = 5; // 1 Joker + 2 same card + 2 same cards
                                found = 1;
                                break;
                            case 2: // 2 same cards
                                hands[i].value = 12; // 1 Joker + 2 same card
                                break;
                            case 1: // 1 card
                                hands[i].value = 11; // 1 Joker + 1 card
                                break;
                            case 0:
                                hands[i].value = 10; // 1 Joker
                                break;
                            default:
                                printf("Error!\n");
                                exit(-1);
                        }
                    } else {
                        switch (hands[i].value) { // 1 card
                            case 40: // 3 Joker
                                hands[i].value = 6; // 1 card + 3 Joker
                                found = 1;
                                break;
                            case 21: // 2 Joker + 1 card
                                hands[i].value = 4; // 1 card + 2 Joker + 1 card
                                found = 1;
                                break;
                            case 20: // 2 Joker
                                hands[i].value = 21; // 1 card + 2 Joker
                                break;
                            case 12:
                                hands[i].value = 4; // 1 card + 1 Joker + 2 same card
                                found = 1;
                                break;
                            case 11:
                                hands[i].value = 11; // 1 card + 1 Joker + 1 same card
                                break;
                            case 10:
                                hands[i].value = 11; // 1 same card + 1 Joker
                                break;
                            case 6: // 4 same cards
                                hands[i].value = 6; // 1 card + 4 same card
                                found = 1;
                                break;
                            case 4: // 3 same cards
                                hands[i].value = 4; // 1 same card + 3 same card
                                break;
                            case 3:
                                hands[i].value = 3; // 1 same card + 2 same card + 2 same card
                                found = 1;
                                break;
                            case 2:
                                hands[i].value = 2; // 1 same card + 2 same card
                                break;
                            case 1:
                            case 0:
                                hands[i].value = 1; // 1 same card
                                break;
                            default:
                                printf("Error!\n");
                                exit(-1);
                        }
                    }
                    break;
                case 0:
                    break;
            }
            printf("  %d", hands[i].value);
        }
        switch (hands[i].value) {
            case 21: // 2 Joker + 1 same cards
                hands[i].value = 4;
                break;
            case 12: // 1 Joker + 2 same cards
                hands[i].value = 4;
                break;
            case 11: // 1 Joker + 1 card
                hands[i].value = 2;
                break;
        }
        if (hands[i].value > 7) {
            printf("Error!\n");
            exit(-1);
        }
        printf(" -> %d\n", hands[i].value);
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
