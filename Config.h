// Copyright (C) 2018 Martin Weigel <mail@MartinWeigel.com>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

// Version: 2018-09-05

#pragma once
#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
// TYPE DEFINITIONS FOR INTERNAL USAGE
//******************************************************************************
typedef enum ConfigType ConfigType;
typedef struct ConfigItem ConfigItem;


//******************************************************************************
// PUBLIC INTERFACE
//******************************************************************************
/**
 * Parses the input file.
 * @filepath:
 *      Path to the configuration file.
 * @items:
 *      Callback to a user-defined function that is called for each valid line.
 */
void Config_parse(char* filepath, ConfigItem* items);

/**
 * Callback definition for CONFIG_CALLBACK(key, callback).
 * @sender:
 *      Pointer to a user-specified object.
 * @key:
 *      The key of the read config.
 * @value:
 *      The value of the read config.
 */
typedef void Config_callback(void* sender, const char* key, const char* value);

/*
 * Macro definitions to create config items
 */
#define CONFIG_END()                            { CONFIGTYPE_END, NULL, NULL, NULL, false }
#define CONFIG_BOOL(key, valuePtr)              { CONFIGTYPE_BOOL, key, valuePtr, NULL, false }
#define CONFIG_INT(key, valuePtr)               { CONFIGTYPE_INT, key, valuePtr, NULL, false }
#define CONFIG_DOUBLE(key, valuePtr)            { CONFIGTYPE_DOUBLE, key, valuePtr, NULL, false }
#define CONFIG_STRING(key, valuePtr)            { CONFIGTYPE_STRING, key, valuePtr, NULL, false }
#define CONFIG_CALLBACK(key, callback, sender)  { CONFIGTYPE_CALLBACK, key, sender, callback, false }



//******************************************************************************
// Internal definition of data structures
//******************************************************************************
typedef enum ConfigType
{
    CONFIGTYPE_END,
    CONFIGTYPE_BOOL,
    CONFIGTYPE_INT,
    CONFIGTYPE_DOUBLE,
    CONFIGTYPE_STRING,
    CONFIGTYPE_CALLBACK
} ConfigType;

typedef struct ConfigItem {
    ConfigType type;
    char* key;
    void *pointer;
    void *callback;
    bool isSet;
} ConfigItem;



//******************************************************************************
// Implementation section. Include using:
// #define CONFIG_IMPLEMENTATION
// #include "Config.h"
//******************************************************************************
#ifdef CONFIG_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

const int CONFIG_BUFFERSIZE = 256;
const char CONFIG_EQUALSIGN = '=';
const char CONFIG_COMMENTSIGN = '#';

void Config_handleItem(ConfigItem* item, char* value)
{
    char* lastParsedChar = NULL;
    switch(item->type) {
        case CONFIGTYPE_BOOL: {
            int intValue = strtol(value, &lastParsedChar, 10);
            // Check for errors
            if(*lastParsedChar != '\0' || intValue < 0 || intValue > 1) {
                printf("[CONFIG] Value for key '%s' is not an bool: %s\n", item->key, value);
                exit(EXIT_FAILURE);
            } 
            bool* output = (bool*)item->pointer;
            *output = intValue;
            break;
        }
        case CONFIGTYPE_INT: {
            int* output = (int*)item->pointer;
            *output = strtol(value, &lastParsedChar, 10);
            // Check for errors
            if(*lastParsedChar != '\0') {
                printf("[CONFIG] Value for key '%s' is not an integer: %s\n", item->key, value);
                exit(EXIT_FAILURE);
            }
            break;
        }
        case CONFIGTYPE_DOUBLE: {
            double* output = (double*)item->pointer;
            *output = strtod(value, &lastParsedChar);
            // Check for errors
            if(*lastParsedChar != '\0') {
                printf("[CONFIG] Value for key '%s' is not an double: %s\n", item->key, value);
                exit(EXIT_FAILURE);
            }
            break;
        }
        case CONFIGTYPE_STRING: {
            char** output = (char**)item->pointer;
            *output = malloc(sizeof(value));
            strcpy(*output, value);
            break;
        }
        case CONFIGTYPE_CALLBACK: {
            Config_callback* output = (Config_callback*)item->callback;
            output(item->pointer, item->key, value);
            break;
        } 
        default:
            fprintf(stderr, "[CONFIG] Invalid config type: %d\n", item->type);
            exit(EXIT_FAILURE);
    }
}

char* Config_trimString(char *input)
{
    // Trim leading space
    while(isspace((unsigned char)*input))
        input++;

    if(*input != 0) {
        // Trim trailing space
        char *end = input + strlen(input) - 1;
        while(end > input && isspace((unsigned char)*end))
            end--;
        // Write new null terminator character
        end[1] = '\0';
    }
    return input;
}

void Config_parse(char* filepath, ConfigItem* items)
{
    // Load the configuration file
    FILE* file = fopen(filepath, "r");
    if(file == NULL) {
        fprintf(stderr, "[CONFIG] Unable to open '%s': %s\n", filepath, strerror(errno));
        exit(EXIT_FAILURE);
    }

    uint32_t lineNumber = 0;
    char buffer[CONFIG_BUFFERSIZE];
    #ifdef CONFIG_VERBOSE
    bool handlerFound;
    #endif
    while(fgets(buffer, CONFIG_BUFFERSIZE, file)) {
        lineNumber++;
        // Get key from the line
        char* startPointer = Config_trimString(buffer);
        if(*startPointer == 0) {
            #ifdef CONFIG_DEBUG
            fprintf(stderr, "[CONFIG] Line %d: Empty line\n", lineNumber);
            #endif
            continue;
        } else if(*startPointer == CONFIG_COMMENTSIGN) {
            #ifdef CONFIG_DEBUG
            fprintf(stderr, "[CONFIG] Line %d: Comment '%s'\n", lineNumber, startPointer);
            #endif
            continue;
        }

        // We have some non-comment content in the line, let's parse
        char* equalSign = strchr(startPointer, CONFIG_EQUALSIGN);
        if(equalSign == NULL) {
            fprintf(stderr, "[CONFIG] PARSING ERROR! No '=' found in line %d\n", lineNumber);
            exit(EXIT_FAILURE);
        }

        // Equal sign position is used as a separator
        *equalSign = '\0';
        char* keyPtr = Config_trimString(startPointer);
        char* valPtr = Config_trimString(equalSign+1);

        // Check that key and value are set
        if(*keyPtr == '\0') {
            fprintf(stderr, "[CONFIG] PARSING ERROR! No key given in line %d.\n", lineNumber);
            exit(EXIT_FAILURE);
        }
        if(*valPtr == '\0') {
            fprintf(stderr, "[CONFIG] PARSING ERROR! No value given in line %d.\n", lineNumber);
            exit(EXIT_FAILURE);
        }

        #ifdef CONFIG_DEBUG
        fprintf(stderr, "[CONFIG] Line %d: Parsed item (%s : %s)\n", lineNumber, keyPtr, valPtr);
        #endif

        // Check if key is in specified items
        #ifdef CONFIG_VERBOSE
        handlerFound = false;
        #endif
        ConfigItem* item = items;
        while(item->type != CONFIGTYPE_END) {
            // Check if item matches the key
            if(strcmp(item->key, keyPtr) == 0) {  
                #ifdef CONFIG_DEBUG
                fprintf(stderr, "[CONFIG] Line %d: Found handler for (%s : %s)\n", lineNumber, keyPtr, valPtr);
                #endif  

                // Check if key was already set
                #ifdef CONFIG_VERBOSE
                if(item->isSet) {
                    fprintf(stderr, "[CONFIG] Key duplicate in line %d: %s\n", lineNumber, keyPtr);
                }
                #endif

                #ifdef CONFIG_VERBOSE
                item->isSet = true;
                handlerFound = true;
                #endif

                // Handle item depending on its type
                Config_handleItem(item, valPtr);  
                break;
            }
            item++;
        }

        // Check if all handlers were set or print error
        #ifdef CONFIG_VERBOSE
        if(!handlerFound) {
            fprintf(stderr, "[CONFIG] Key in line %d not handled: %s\n", lineNumber, keyPtr);
        }
        #endif
    }
    fclose(file);

    // Check that all items were successfully read from config
    #ifdef CONFIG_VERBOSE
    ConfigItem* item = items;
    while(item->type != CONFIGTYPE_END) {
        if(!item->isSet) {
            fprintf(stderr, "[CONFIG] Config does not contain key: %s\n", item->key);
        }
        item++;
    }
    #endif
}

#endif
