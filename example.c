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

// Version: 2018-12-01

#define CONFIG_IMPLEMENTATION       // Include implementation in this file
//#define CONFIG_VERBOSE            // Prints all verbose messages to stderr
//#define CONFIG_VERBOSE_DUPLICATE  // Print if config contains key duplicates to stderr
//#define CONFIG_VERBOSE_MISSING    // Print if key is missing in config to stderr
//#define CONFIG_VERBOSE_UNUSED     // Print if a config key is not used in application stderr
//#define CONFIG_DEBUG              // Only for library testing
#include "Config.h"

#define STRING_SIZE (80)

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

int main()
{
    // Declare variables that will be set using Config_load().
    // They keep their initial value, if their ConfigItem does not match.
    bool cBool = 0;
    int cInt = 0;
    uint64_t cUInt64 = 0;
    double cDouble = 0.f;
    char cString[STRING_SIZE], cKey1[STRING_SIZE], cKey2[STRING_SIZE];
    const char* cNewString = NULL;
    int callbackResult = 0;

    // Load values from configuration file to variables
    Config_load("example.txt", (ConfigItem[]) {
        CONFIG_BOOL("aBool", &cBool),
        CONFIG_STRING("aString", &cString, STRING_SIZE),
        CONFIG_NEW_STRING("aNewString", &cNewString),
        CONFIG_DOUBLE("aDouble", &cDouble),
        CONFIG_INT("anInt", &cInt),
        CONFIG_UINT64("aUInt64", &cUInt64),
        CONFIG_STRING("namespace.key1", &cKey1, STRING_SIZE),
        CONFIG_STRING("namespace.key2", &cKey2, STRING_SIZE),
        CONFIG_CALLBACK("otherspace.key1", &exampleCallback, &callbackResult),
        CONFIG_END()
    });

    // Print all values
    printf("\nSTRINGS:\n");
    printf("\taString:    %s\n", cString);
    printf("\taNewString: %s\n", cNewString);

    printf("\nNUMBERS:\n");
    printf("\taBool:      %d\n", cBool);
    printf("\taDouble:    %f\n", cDouble);
    printf("\tanInt:      %d\n", cInt);
    printf("\taUInt64:    %" PRIu64 "\n", cUInt64);

    printf("\nNAMESPACES:\n");
    printf("\tnamespace.key1:   %s\n", cKey1);
    printf("\tnamespace.key2:   %s\n", cKey2);
    printf("\totherspace.key1:  %d\n", callbackResult);
    return EXIT_SUCCESS;
}
