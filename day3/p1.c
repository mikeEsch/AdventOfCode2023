
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
    
    while (searchNumber(lineNum, startCol, &number, &numStartCol, &numChars)) {
        printf("\t\tFound number: %d startCol: %zu numChars: %zu\n", number, numStartCol, numChars);
        if (numStartCol > 0) {
            char c = p_line[numStartCol-1];
            printf("%c", c);
            if (c != '.') {
                sum += number;
                eraseNumber(p_line, numStartCol, numChars);
                startCol = numStartCol + numChars;
                printf("\t\t   Added number: %d Sum: %lu\n", number, sum);
                continue;
            }
            printf("\n");
        }
        if (numStartCol + numChars < lineLen) {
            char c = p_line[numStartCol+numChars];
            printf("%c", c);
            if (c != '.') {
                sum += number;
                eraseNumber(p_line, numStartCol, numChars);
                startCol = numStartCol + numChars;
                printf("\t\t   Added number: %d Sum: %lu\n", number, sum);
                continue;
            }
            printf("\n");
        }
        if (p_upper_line != NULL) {
            size_t start = (numStartCol>0)? (numStartCol-1):0;
            size_t stop = (numStartCol + numChars + 1 < upperLineLen)? (numStartCol + numChars + 1):(numStartCol + numChars);
            unsigned char added = 0;
            for (size_t col = start; (col < stop) && (added == 0); col++) {
                char c = p_upper_line[col];
                printf("%c ", c);
                if (c != '.' && c != 'X' && (c < '0' || c > '9')) {
                    sum += number;
                    eraseNumber(p_line, numStartCol, numChars);
                    startCol = numStartCol + numChars;
                    added = 1;
                    printf("\t\t   Added number: %d Sum: %lu", number, sum);
                }
            }
            printf("\n");
            if (added > 0) continue;
        }
        if (p_lower_line != NULL) {
            size_t start = (numStartCol>0)? (numStartCol-1):0;
            size_t stop = (numStartCol + numChars + 1 < lowerLineLen)? (numStartCol + numChars + 1):(numStartCol + numChars);
            unsigned char added = 0;
            for (size_t col = start; (col < stop) && (added == 0); col++) {
                char c = p_lower_line[col];
                printf("%c ", c);
                if (c != '.' && c != 'X' && (c < '0' || c > '9')) {
                    sum += number;
                    eraseNumber(p_line, numStartCol, numChars);
                    startCol = numStartCol + numChars;
                    added = 1;
                    printf("\t\t   Added number: %d Sum: %lu", number, sum);
                }
            }
            printf("\n");
            if (added > 0) continue;
        }
        printf("\t\tDiscarded number: %d\n", number);
        startCol = numStartCol + numChars;
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