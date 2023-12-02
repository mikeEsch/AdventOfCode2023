
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long sum = 0;
unsigned long game = 0;

unsigned char parseFetchResult(char * token) {
    unsigned char result = 0;
    char * p_remain = token;
    char * p_start = token;
    unsigned int numRed = 0;
    unsigned int numBlue = 0;
    unsigned int numGreen = 0;

    printf("%s\n", token);

    do {
        p_start = strsep(&p_remain, ",");
        if (p_start != NULL) {
            if (p_remain != NULL) {
                p_remain += 1;
            }
            //printf("\t%s\n", p_start);
            char * s = p_start;
            p_start = strsep(&s, " ");
            unsigned int number;
            if (sscanf(p_start, "%d", &number) == 1) {
                if (numRed == 0 && strncmp(s, "red",3) == 0) {
                    numRed = number;
                    continue;
                }
                if (numBlue == 0 && strncmp(s, "blue",4) == 0) {
                    numBlue = number;
                    continue;
                }
                if (numGreen == 0 && strncmp(s, "green",5) == 0) {
                    numGreen = number;
                    continue;
                }
            }
        }
    } while (p_start != NULL);

    if (numRed <= 12 && numGreen <= 13 && numBlue <= 14) result = 1;

    printf("\tred: %d blue: %d green: %d - %s\n", numRed, numBlue, numGreen, (result)? "OK":"NOK");
    
    return (result);
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
                char * token;
                char * p_remain = p_buffer;
                token = strsep(&p_remain, ":");
                if (token != NULL) {
                    unsigned char result = 1;
                    if (sscanf(token, "Game %lu", &game) == 1) {
                        if (p_remain != NULL) {
                            p_remain += 1;
                        }
                        printf("Game: %lu - %s\n", game, p_remain);
                        do {
                            token = strsep(&p_remain, ";");
                            if (token != NULL) {
                                if (p_remain != NULL) {
                                    p_remain += 1;
                                }
                                if (parseFetchResult(token) == 0) {
                                    result = 0;
                                }
                            }
                        } while (token != NULL);

                        if (result) {
                            sum += game;
                        }

                    } else {
                        printf("Error - parsing: %s\n", p_buffer);
                    }
                }
            }
        }
    }
    if (p_buffer != NULL) {
        free(p_buffer);
    }
    printf("Result: %lu\n", sum);
}