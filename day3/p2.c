
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long sum = 0;

#define NUM_LINES 3
char* lines[NUM_LINES] = { NULL, NULL, NULL };
size_t lineLengths[NUM_LINES];

unsigned char searchNumber(unsigned int lineNum, size_t startCol,
    unsigned int * p_number, size_t * p_numStartCol, size_t * p_numChars) {
    char * p_line = lines[lineNum];
    size_t lineLen = lineLengths[lineNum];
    unsigned char result = 0;
    unsigned char found = 0;
    size_t col;

    for (col = startCol; col < lineLen; col++) {
        char c = p_line[col];
        //printf("col: %zu c: %c\n", col, c);
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

    if (result == 1) {
        printf("Found number: %d\n", *p_number);
    }
    return (result);
}

unsigned char searchNumber_backward(unsigned int lineNum, size_t startCol,
    unsigned int * p_number, size_t * p_numStartCol, size_t * p_numChars) {
    char * p_line = lines[lineNum];
    size_t lineLen = lineLengths[lineNum];
    unsigned char result = 0;
    unsigned char found = 0;
    int col;
    unsigned int exp;

    if (startCol > 0) {
        for (col = startCol, exp = 0; col >= 0; col--) {
            char c = p_line[col];
            //printf("col: %d c: %c\n", col, c);
            if (c >= '0' && c <= '9') {
                if (!found) {
                    *p_numStartCol = col;
                    *p_number = c - '0';
                    *p_numChars = 1;
                    exp = 0;
                    found = 1;
                } else {
                    unsigned long value = c - '0';
                    exp += 1;
                    for (unsigned int i=exp; i>0; i--) value *= 10;
                    *p_number += value;
                    *p_numChars += 1;
                }
            } else {
                if (found) {
                    result = 1;
                    break;
                }
            }
        }
        if (col < 0 && found == 1) {
            result = 1;
        }
    }

    if (result == 1) {
        printf("Found number: %d\n", *p_number);
    }
    return (result);
}

unsigned char searchGear(unsigned int lineNum, size_t startCol,
    size_t * p_gearCol) {
    char * p_line = lines[lineNum];
    size_t lineLen = lineLengths[lineNum];
    unsigned char result = 0;
    size_t col;

    for (col = startCol; col < lineLen; col++) {
        char c = p_line[col];
        if (c == '*') {
            *p_gearCol = col;
            result = 1;
            break;
        }
    }

    return (result);
}


void eraseNumber(char *p_line, size_t startCol, size_t numChars) {
#if 1
    char* p = &p_line[startCol];
    for (size_t i=0; i<numChars; i++) {
        p[i] = 'X';
    }
#endif
}

void scanLine(unsigned int lineNum) {
    char * p_line = lines[lineNum];
    size_t lineLen = lineLengths[lineNum];
    unsigned int upperLineNum = (lineNum > 0)? lineNum-1:(NUM_LINES-1);
    char * p_upper_line = lines[upperLineNum];
    size_t upperLineLen = lineLengths[upperLineNum];
    unsigned int lowerLineNum = (lineNum < (NUM_LINES-1))? lineNum+1:0;
    char * p_lower_line = lines[lowerLineNum];
    size_t lowerLineLen = lineLengths[lowerLineNum];
    size_t startCol = 0;
    unsigned int number;
    size_t numStartCol;
    size_t numChars;
    size_t gearCol;

    if (p_line == NULL) return;

    printf("\t scanLine(lineNum: %d)\n", lineNum);
    if (p_upper_line != NULL) { 
        printf("\t%d\t%s\n", upperLineNum, p_upper_line);
    } else {
        printf("\t-----\n");
    }
    printf("\t%d\t%s\n", lineNum, p_line);
    if (p_lower_line != NULL) {
        printf("\t%d\t%s\n", lowerLineNum, p_lower_line);
    } else {
        printf("\t-----\n");
    }
    
    while (searchGear(lineNum, startCol, &gearCol)) {
        printf("Found gear in col: %zu\n", gearCol);
        unsigned int numAdjNumbers = 0;
        unsigned long ratio = 1;
        if (gearCol > 0) {
            printf("Search left\n");
            if (searchNumber_backward(lineNum, gearCol-1, &number, &numStartCol, &numChars)) {
                if (numStartCol == (gearCol-1)) {
                    ++numAdjNumbers;
                    ratio *= number;
                    printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                }
            }
        }
        if ((gearCol+1) < lineLen) {
            printf("Search right\n");
            if (searchNumber(lineNum, gearCol+1, &number, &numStartCol, &numChars)) {
                if (numStartCol == (gearCol+1)) {
                    ++numAdjNumbers;
                    ratio *= number;
                    printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers,number, ratio);
                }
            }
        }
        if (p_upper_line != NULL) {
            char c = p_upper_line[gearCol];
            if (c < '0' || c > '9') {
                if (gearCol > 0) {
                    printf("Search upper left\n");
                    if (searchNumber_backward(upperLineNum, gearCol-1, &number, &numStartCol, &numChars)) {
                        if (numStartCol == (gearCol-1)) {
                            ++numAdjNumbers;
                            if (numAdjNumbers <= 2) {
                                ratio *= number;
                                printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                            } else {
                                printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                            }
                        }
                    }
                }
                if (gearCol < (upperLineLen-1)) {
                    printf("Search upper right\n");
                    if (searchNumber(upperLineNum, gearCol+1, &number, &numStartCol, &numChars)) {
                        if (numStartCol == (gearCol+1)) {
                            ++numAdjNumbers;
                            if (numAdjNumbers <= 2) {
                                ratio *= number;
                                printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                            } else {
                                printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                            }
                        }
                    }
                }
            } else {
                if (gearCol > 0) {
                    printf("Search upper\n");
                    c = p_upper_line[gearCol-1];
                    size_t i;
                    for (i = gearCol-1; i > 0;) {
                        if (c < '0' || c > '9') {
                            if (searchNumber(upperLineNum, i, &number, &numStartCol, &numChars)) {
                                ++numAdjNumbers;
                                if (numAdjNumbers <= 2) {
                                    ratio *= number;
                                    printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                                    break;
                                } else {
                                    printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                                }
                            }
                        } else {
                            i--;
                            c = p_upper_line[i];
                        }
                    }
                    if (i == 0) {
                        if (searchNumber(upperLineNum, 0, &number, &numStartCol, &numChars)) {
                            ++numAdjNumbers;
                            if (numAdjNumbers <= 2) {
                                ratio *= number;
                                printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                            } else {
                                printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                            }
                        }
                    }
                } else {
                    if (searchNumber(upperLineNum, gearCol, &number, &numStartCol, &numChars)) {
                        ++numAdjNumbers;
                        if (numAdjNumbers <= 2) {
                            ratio *= number;
                            printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                        } else {
                            printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                        }
                    }
                }
            }
        }

        if (p_lower_line != NULL) {
            char c = p_lower_line[gearCol];
            if (c < '0' || c > '9') {
                if (gearCol > 0) {
                    printf("Search lower left\n");
                    if (searchNumber_backward(lowerLineNum, gearCol-1, &number, &numStartCol, &numChars)) {
                        if (numStartCol == (gearCol-1)) {
                            ++numAdjNumbers;
                            if (numAdjNumbers <= 2) {
                                ratio *= number;
                                printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                            } else {
                                printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                            }
                        }
                    }
                }
                if (gearCol < (lowerLineLen-1)) {
                    printf("Search lower right\n");
                    if (searchNumber(lowerLineNum, gearCol+1, &number, &numStartCol, &numChars)) {
                        if (numStartCol == (gearCol+1)) {
                            ++numAdjNumbers;
                             if (numAdjNumbers <= 2) {
                                ratio *= number;
                                printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                            } else {
                                printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                            }
                       }
                    }
                }
            } else {
                if (gearCol > 0) {
                    printf("Search lower\n");
                    c = p_lower_line[gearCol-1];
                    size_t i;
                    for (i = gearCol-1; i > 0;) {
                        if (c < '0' || c > '9') {
                            if (searchNumber(lowerLineNum, i, &number, &numStartCol, &numChars)) {
                                ++numAdjNumbers;
                                if (numAdjNumbers <= 2) {
                                    ratio *= number;
                                    printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                                    break;
                                } else {
                                    printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                                }
                            }
                        } else {
                            i--;
                            c = p_lower_line[i];
                        }
                    }
                    if (i == 0) {
                        if (searchNumber(lowerLineNum, 0, &number, &numStartCol, &numChars)) {
                            ++numAdjNumbers;
                            if (numAdjNumbers <= 2) {
                                ratio *= number;
                                printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                            } else {
                                printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                            }
                        }
                    }
                } else {
                    if (searchNumber(lowerLineNum, gearCol, &number, &numStartCol, &numChars)) {
                        ++numAdjNumbers;
                        if (numAdjNumbers <= 2) {
                            ratio *= number;
                            printf("\t\t   Adj. number #%d: %d Ratio: %lu\n", numAdjNumbers, number, ratio);
                            break;
                        } else {
                            printf("\t\t   Adj. number #%d: %d\n", numAdjNumbers, number);
                        }
                    }
                }
            }
        }
        if (numAdjNumbers == 2) {
            sum += ratio;
            printf(" Added %lu -> Sum: %lu\n", ratio, sum);
        }
        startCol = gearCol + 1;
    }
}

int main(void) {
    char *p_buffer = NULL;
    size_t len = 0;
    unsigned int lineNum = 0;
    unsigned int numLines = 0;
    unsigned int lineNumber = 0;
    while (getline(&p_buffer, &len, stdin)>=0) {
        size_t str_len = strlen(p_buffer);
        if (str_len > 0) {
            if (str_len >= 1) {
                if (p_buffer[str_len-1] == '\n') {
                    p_buffer[str_len-1] = '\0';
                    str_len -= 1;
                }
            } 
            lineNumber++;
            if (numLines < NUM_LINES) {
                numLines++;
            }
            printf("%d\t- %s - %d/%d\n", lineNumber, p_buffer, lineNum, numLines);
            if (str_len > 0) {
                lines[lineNum] = p_buffer;
                lineLengths[lineNum] = str_len;
            } else {
                lines[lineNum] = NULL;
            }
            if (numLines >= 3) {
                scanLine((lineNum > 0)? lineNum-1:(NUM_LINES-1));
            } else if (numLines == 2) {
                scanLine((lineNum > 0)? lineNum-1:(NUM_LINES-1));
            }
            if (++lineNum >= NUM_LINES) {
                lineNum = 0;
            }
            p_buffer = lines[lineNum];
        }
    }
    if (lines[lineNum] != NULL) {
        free(lines[lineNum]);
        lines[lineNum] = NULL;
    }
    scanLine((lineNum-1)%NUM_LINES);

    for (lineNum = 0; lineNum < NUM_LINES; lineNum++) {
        p_buffer = lines[lineNum];
        if (p_buffer != NULL) {
            free(p_buffer);
        }
    }
    printf("Result: %lu\n", sum);
}