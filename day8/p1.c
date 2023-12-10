
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_LINES 1000
#define NUM_CHARS 4

typedef struct {
    char pos[NUM_CHARS];
    char left[NUM_CHARS];
    char right[NUM_CHARS];
} T_DIRECTION;

unsigned int lineNum = 0;
T_DIRECTION dirs[NUM_LINES];

T_DIRECTION* findDirs(char* pos) {
    T_DIRECTION* result = NULL;
    if(pos != NULL) {
        for (unsigned int i=0; i<lineNum; i++) {
            if (strncmp(pos, dirs[i].pos, NUM_CHARS) == 0) {
                result = &dirs[i];
                break;
            }
        }
    }
    return (result);
}

int main(void) {
    char *p_directions = NULL;
    char *p_buffer = NULL;
    size_t len = 0;
    unsigned long stepCount = 0;
    unsigned long result = 0;


    while (getline(&p_directions, &len, stdin)>=0) {
        size_t str_len = strlen(p_directions);
        if (str_len > 0) {
            if (str_len >= 1) {
                if (p_directions[str_len-1] == '\n') {
                    p_directions[str_len-1] = '\0';
                    str_len -= 1;
                }
                break;
            }
        }
    }
    printf("%s\n", p_directions);

    while (getline(&p_buffer, &len, stdin)>=0 && lineNum < NUM_LINES) {
        size_t str_len = strlen(p_buffer);
        if (str_len > 0) {
            if (str_len >= 1) {
                if (p_buffer[str_len-1] == '\n') {
                    p_buffer[str_len-1] = '\0';
                    str_len -= 1;
                }
            }
        }
        len = strlen(p_buffer);
        if (len>0) {
            printf("%s - ", p_buffer);
            unsigned int state = 0;
            unsigned int idx = 0;
            for (unsigned int i=0; i<len && state<=5; i++) {
                char c = p_buffer[i];
                printf("%c", c);
                switch (state) {
                    case 0:
                        if (c >= 'A' && c <= 'Z') {
                            dirs[lineNum].pos[0] = c;
                            idx = 0;
                            state = 1;
                        }
                        break;
                    case 1:
                        if (++idx < 3) {
                            if (c >= 'A' && c <= 'Z') {
                                dirs[lineNum].pos[idx] = c;
                            }
                        } else {
                            if (c == ' ') {
                                dirs[lineNum].pos[3] = '\0';
                                state = 2;
                            } else {
                                printf("Parse error!!!\n");
                                exit(-1);
                            }
                        }
                        break;
                    case 2:
                        if (c >= 'A' && c <= 'Z') {
                            dirs[lineNum].left[0] = c;
                            idx = 0;
                            state = 3;
                        }
                        break;
                    case 3:
                        if (++idx < 3) {
                            if (c >= 'A' && c <= 'Z') {
                                dirs[lineNum].left[idx] = c;
                            }
                        } else {
                            if (c == ',') {
                                dirs[lineNum].left[3] = '\0';
                                state = 4;
                            } else {
                                printf("Parse error!!!\n");
                                exit(-1);
                            }
                        }
                        break;
                    case 4:
                        if (c >= 'A' && c <= 'Z') {
                            dirs[lineNum].right[0] = c;
                            idx = 0;
                            state = 5;
                        }
                        break;
                    case 5:
                        if (++idx < 3) {
                            if (c >= 'A' && c <= 'Z') {
                                dirs[lineNum].right[idx] = c;
                            }
                        } else {
                            if (c == ')') {
                                dirs[lineNum].right[3] = '\0';
                                state = 6;
                            } else {
                                printf("Parse error!!!\n");
                                exit(-1);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            printf(" - %s %s %s\n", dirs[lineNum].pos, dirs[lineNum].left, dirs[lineNum].right);
        }
        ++lineNum;
    }

    T_DIRECTION* p_map_dir = findDirs("AAA");
    unsigned int found = 0;
    unsigned int error = 0;
    unsigned int dir_len = strlen(p_directions);
    unsigned int dir_idx = 0;
    printf("Step: %lu - Pos: %s L: %s R: %s - ",
        stepCount ,p_map_dir->pos, p_map_dir->left, p_map_dir->right);
    while (p_map_dir != NULL && !found && !error) {
        char d = p_directions[dir_idx];
        printf("%c\n", d);
        char * p_dir = NULL;
        if (d == 'R') {
            p_dir = p_map_dir->right;
        } else if (d == 'L') {
            p_dir = p_map_dir->left;
        } else {
            printf("Dir error!!!\n");
            error = 1;
        }
        p_map_dir = findDirs(p_dir);
        if(p_map_dir != NULL) {
            ++stepCount;
            if (strcmp("ZZZ", p_map_dir->pos)==0) {
                found = 1;
                printf("Step: %lu - Pos: %s L: %s R: %s\n ",
                    stepCount ,p_map_dir->pos, p_map_dir->left, p_map_dir->right);
            } else {
                printf("Step: %lu - Pos: %s L: %s R: %s - ",
                    stepCount ,p_map_dir->pos, p_map_dir->left, p_map_dir->right);
            }
        } else {
            printf("Dir error!!!\n");
            error = 1;
        }
        if(++dir_idx >= dir_len) dir_idx = 0;
    }
    if (p_buffer != NULL) {
        free(p_buffer);
    }
    if (p_directions != NULL) {
        free(p_directions);
    }
    if (found) {
        printf("Result: %lu\n", stepCount);
    }
}
