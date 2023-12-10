
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long loc = ~0UL;

#define NUM_SEEDS 20
unsigned long seeds[NUM_SEEDS];
unsigned int numSeeds = 0;

typedef struct {
    unsigned long destination;
    unsigned long source;
    unsigned long range;
} T_RANGE;

typedef enum {
    e_seed =0,
    e_seed2soil = 1,
    e_soil2fertilizer = 2,
    e_fertilizer2water = 3,
    e_water2light = 4,
    e_light2temperature = 5,
    e_temperature2humidity = 6,
    e_humidity2location = 7,
    e_none = 255
} T_ENUM_INPUT_TYPE;
unsigned int inputType = e_none;
unsigned int inputMask = 0b111111111;

#define MAP_SIZE 100
T_RANGE seed2soil[MAP_SIZE];
unsigned int num_seed2soil = 0;
T_RANGE soil2fertilizer[MAP_SIZE];
unsigned int num_soil2fertilizer = 0;
T_RANGE fertilizer2water[MAP_SIZE];
unsigned int num_fertilizer2water = 0;
T_RANGE water2light[MAP_SIZE];
unsigned int num_water2light = 0;
T_RANGE light2temperature[MAP_SIZE];
unsigned int num_light2temperature = 0;
T_RANGE temperature2humidity[MAP_SIZE];
unsigned int num_temperature2humidity = 0;
T_RANGE humidity2location[MAP_SIZE];
unsigned int num_humidity2location = 0;



unsigned char searchNumber(char * p_line,
    unsigned long * p_number, size_t * p_numStartCol, size_t * p_numChars) {
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

unsigned long findDestInMap(unsigned long source, T_RANGE* p_map, unsigned int numLines) {
    if (p_map != NULL) {
        unsigned int i;
        T_RANGE* p_line = p_map;
        for (i=0; i<numLines; i++, p_line++) {
            printf("line# %d: source: %lu src-start: %lu dest-start: %lu range: %lu\n",
                i, source, p_line->source, p_line->destination, p_line->range);
            if (p_line->source <= source) {
                if ((p_line->source + p_line->range) > source) {
                    unsigned long offset = source - p_line->source;
                    unsigned long dest = p_line->destination + offset;
                    printf("line# %d: source: %lu src-start: %lu offset: %lu dest-start: %lu dest: %lu\n",
                        i, source, p_line->source, offset, p_line->destination, dest);
                    return (dest);
                }
            }
        }

    } else {
        printf("Parameter Error!!!\n");
    }
    printf("%lu %lu\n", source, source);
    return (source);
}

unsigned int parseMap(char * p_line, T_RANGE* p_map, unsigned int * map_line) {
    unsigned long number;
    size_t startCol;
    size_t numChars;
    unsigned int numCount = 0;
    char * next = p_line;
    T_RANGE* p_map_line = &p_map[*map_line];
    if (*map_line < MAP_SIZE) {
        while (searchNumber(next, &number, &startCol, &numChars)) {
            switch (numCount) {
                case 0:
                    p_map_line->destination = number;
                    printf("d: %lu ", p_map_line->destination);
                    break;
                case 1:
                    p_map_line->source = number;
                    printf("\ts: %lu ", p_map_line->source);
                    break;
                case 2:
                    p_map_line->range = number;
                    printf("\tr: %lu ", p_map_line->range);
                    break;
                default:
                    printf("\nError: Map line size failure!!!\n");
                    break;
            }
            next += (startCol + numChars);
            ++numCount;
        }
        printf("\n");
    } else {
        printf("\nError: Map array to small!!!\n");
    }
    return (numCount);
}


int main(void) {
    char *p_buffer = NULL;
    size_t len = 0;
    char * next = NULL;
    while (getline(&p_buffer, &len, stdin)>=0 && inputMask) {
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
        switch(inputType) {
            case e_none:
                {
                    char * result;
                    result = strsep(&next, ":");
                    printf("%s\n", result);
                    if ( next != NULL) {
                        if (strcmp(result, "seeds") == 0) {
                            inputType = e_seed;
                            numSeeds = 0;
                            unsigned long number;
                            size_t startCol;
                            size_t numChars;
                            printf("Seeds:\n");
                            while (searchNumber(next, &number, &startCol, &numChars)) {
                                printf("%lu\n", number);
                                if (numSeeds < NUM_SEEDS) {
                                    seeds[numSeeds++] = number;
                                    next += (startCol + numChars);
                                } else {
                                    printf("\nError: Seeds array to small!!!\n");
                                }
                            }
                            inputMask &= ~((unsigned int)0x1<<e_seed);
                            inputType = e_none;
                        } else if (strcmp(result, "seed-to-soil map") == 0) {
                            inputType = e_seed2soil;
                        } else if (strcmp(result, "soil-to-fertilizer map") == 0) {
                            inputType = e_soil2fertilizer;
                        } else if (strcmp(result, "fertilizer-to-water map") == 0) {
                            inputType = e_fertilizer2water;
                        } else if (strcmp(result, "water-to-light map") == 0) {
                            inputType = e_water2light;
                        } else if (strcmp(result, "light-to-temperature map") == 0) {
                            inputType = e_light2temperature;
                        } else if (strcmp(result, "temperature-to-humidity map") == 0) {
                            inputType = e_temperature2humidity;
                        } else if (strcmp(result, "humidity-to-location map") == 0) {
                            inputType = e_humidity2location;
                        }
                    } else {
                        // To-Do
                    }
                }
                break;
            case e_seed:
                break;
            case e_seed2soil:
                {
                    if (num_seed2soil < MAP_SIZE) {
                        unsigned int numCount = parseMap(p_buffer, seed2soil, &num_seed2soil);
                        if (numCount >= 3) {
                            ++num_seed2soil;
                        } else if (numCount == 0) {
                            inputMask &= ~((unsigned int)0x1<<e_seed2soil);
                            inputType = e_none;
                        } 
                    } else {
                        printf("\nError: Map array to small!!!\n");
                    }
                }
                break;
            case e_soil2fertilizer:
                {
                    if (num_soil2fertilizer < MAP_SIZE) {
                        unsigned int numCount = parseMap(p_buffer, soil2fertilizer, &num_soil2fertilizer);
                        if (numCount >= 3) {
                            ++num_soil2fertilizer;
                        } else if (numCount == 0) {
                            inputMask &= ~((unsigned int)0x1<<e_soil2fertilizer);
                            inputType = e_none;
                        } 
                    } else {
                        printf("\nError: Map array to small!!!\n");
                    }
                }
                break;
            case e_fertilizer2water:
                {
                    if (num_fertilizer2water < MAP_SIZE) {
                        unsigned int numCount = parseMap(p_buffer, fertilizer2water, &num_fertilizer2water);
                        if (numCount >= 3) {
                            ++num_fertilizer2water;
                        } else if (numCount == 0) {
                            inputMask &= ~((unsigned int)0x1<<e_fertilizer2water);
                            inputType = e_none;
                        } 
                    } else {
                        printf("\nError: Map array to small!!!\n");
                    }
                }
                break;
            case e_water2light:
                {
                    if (num_water2light < MAP_SIZE) {
                        unsigned int numCount = parseMap(p_buffer, water2light, &num_water2light);
                        if (numCount >= 3) {
                            ++num_water2light;
                        } else if (numCount == 0) {
                            inputMask &= ~((unsigned int)0x1<<e_water2light);
                            inputType = e_none;
                        } 
                    } else {
                        printf("\nError: Map array to small!!!\n");
                    }
                }
                break;
            case e_light2temperature:
                {
                    if (num_light2temperature < MAP_SIZE) {
                        unsigned int numCount = parseMap(p_buffer, light2temperature, &num_light2temperature);
                        if (numCount >= 3) {
                            ++num_light2temperature;
                        } else if (numCount == 0) {
                            inputMask &= ~((unsigned int)0x1<<e_light2temperature);
                            inputType = e_none;
                        } 
                    } else {
                        printf("\nError: Map array to small!!!\n");
                    }
                }
                break;
            case e_temperature2humidity:
                {
                    if (num_temperature2humidity < MAP_SIZE) {
                        unsigned int numCount = parseMap(p_buffer, temperature2humidity, &num_temperature2humidity);
                        if (numCount >= 3) {
                            ++num_temperature2humidity;
                        } else if (numCount == 0) {
                            inputMask &= ~((unsigned int)0x1<<e_temperature2humidity);
                            inputType = e_none;
                        } 
                    } else {
                        printf("\nError: Map array to small!!!\n");
                    }
                }
                break;
            case e_humidity2location:
                {
                    if (num_humidity2location < MAP_SIZE) {
                        unsigned int numCount = parseMap(p_buffer, humidity2location, &num_humidity2location);
                        if (numCount >= 3) {
                            ++num_humidity2location;
                        } else if (numCount == 0) {
                            inputMask &= ~((unsigned int)0x1<<e_humidity2location);
                            inputType = e_none;
                        } 
                    } else {
                        printf("\nError: Map array to small!!!\n");
                    }
                }
                break;
            default:
                break;
        }
    }
    if (p_buffer != NULL) {
        free(p_buffer);
    }

    unsigned int i;
    printf("\n ---- \n");
    for( i=0; i<numSeeds; i++) {
        unsigned long seed = seeds[i];
        printf("seed: %lu\n", seed);
        unsigned long soil = findDestInMap(seed, seed2soil, num_seed2soil);
        printf("soil: %lu\n", soil);
        unsigned long fertilizer = findDestInMap(soil, soil2fertilizer, num_soil2fertilizer);
        printf("fertilizer: %lu\n", fertilizer);
        unsigned long water = findDestInMap(fertilizer, fertilizer2water, num_fertilizer2water);
        printf("water: %lu\n", water);
        unsigned long light = findDestInMap(water, water2light, num_water2light);
        printf("light: %lu\n", light);
        unsigned long temperature = findDestInMap(light, light2temperature, num_light2temperature);
        printf("temperature: %lu\n", temperature);
        unsigned long humanity = findDestInMap(temperature, temperature2humidity, num_temperature2humidity);
        printf("humanity: %lu\n", humanity);
        unsigned long location = findDestInMap(humanity, humidity2location, num_humidity2location);
        printf("location: %lu\n", location);

        if (loc > location) loc = location;

        printf("seed: %lu soil: %lu fert: %lu water: %lu light: %lu temp: %lu human: %lu loc: %lu -> loc: %lu\n",
            seed, soil, fertilizer, water, light, temperature, humanity, location, loc);

    }
    printf("Result: %lu\n", loc);
    return (0);
}
