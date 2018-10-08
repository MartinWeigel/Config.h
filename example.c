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

// Version: 2018-10-08

#define CONFIG_IMPLEMENTATION    // Include implementation in this file
//#define CONFIG_VERBOSE         // Print unhandled items, missing items, and multiples to stderr
//#define CONFIG_DEBUG           // Only for library testing
#include "Config.h"

// Callback checks if the given value is between 1 and 10 or exits
void exampleCallback(void* sender, const char* key, const char* value)
{
    printf("Inside callback for key: %s with value %s\n", key, value);

    // Save the value to the sender object (in this example: &callbackResult)
    int* result = sender;
    *result = atoi(value);

    // Exit program if it is out of bounds to avoid runtime errors
    if(*result < 1|| *result > 10) {
        fprintf(stderr, "Error parsing `%s`: Value `%d` is out of range [1, 10]\n", key, *result);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, const char **argv)
{
    // Declare variables that will be set using Config_load().
    // They keep their initial value, if their ConfigItem does not match.
    // Strings should be always be NULL and will be allocated inside Config.h.
    // A default value for a string can be set afterwards by checking for NULL.
    bool cBool = 0;
    int cInt = 0;
    double cDouble = 0.f;
    const char* cString = NULL;
    const char* cKey1 = NULL;
    const char* cKey2 = NULL;
    int callbackResult = 0;

    // Load values from configuration file to variables
    Config_load("example.txt", (ConfigItem[]) {
        CONFIG_BOOL("aBool", &cBool),
        CONFIG_STRING("aString", &cString),
        CONFIG_DOUBLE("aDouble", &cDouble),
        CONFIG_INT("anInt", &cInt),
        CONFIG_STRING("namespace.key1", &cKey1),
        CONFIG_STRING("namespace.key2", &cKey2),
        CONFIG_CALLBACK("otherspace.key1", &exampleCallback, &callbackResult),
        CONFIG_END()
    });

    // Print all values
    printf("\n");
    printf("aBool:    %d\n", cBool);
    printf("aString:  %s\n", cString);
    printf("aDouble:  %f\n", cDouble);
    printf("anInt:    %d\n", cInt);
    printf("\n");
    printf("namespace.key1:   %s\n", cKey1);
    printf("namespace.key2:   %s\n", cKey2);
    printf("otherspace.key1:  %d\n", callbackResult);
    return EXIT_SUCCESS;
}
