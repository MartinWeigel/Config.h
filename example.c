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

#define CONFIG_IMPLEMENTATION
//#define CONFIG_VERBOSE
//#define CONFIG_NO_EXIT_ON_ERROR
#include "Config.h"

// Callback checks if the given value is between 1 and 10 or exits
void exampleCallback(void* sender, const char* key, const char* value)
{
    printf("Inside callback for key '%s' with value %s\n", key, value);

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
    char cString[80] = "DEFAULT";
    const char* cNewString = NULL;
    int cInt, nKey1, nKey2, oKey1;
    int8_t cInt8 = 0;
    int16_t cInt16 = 0;
    int32_t cInt32 = 0;
    int64_t cInt64 = 0;
    uint8_t cUInt8 = 0;
    uint16_t cUInt16 = 0;
    uint32_t cUInt32 = 0;
    uint64_t cUInt64 = 0;
    bool cBool = 0;
    double cDouble = 0.f;
    int callbackResult = 0;

    // Load values from configuration file to variables
    Config_load("example.txt", (ConfigItem[]) {
        CONFIG_STRING("aString", &cString, 80),
        CONFIG_NEW_STRING("aNewString", &cNewString),

        CONFIG_INT("anInt", &cInt),
        CONFIG_INT8("signed.int8", &cInt8),
        CONFIG_INT16("signed.int16", &cInt16),
        CONFIG_INT32("signed.int32", &cInt32),
        CONFIG_INT64("signed.int64", &cInt64),
        CONFIG_UINT8("unsigned.int8", &cUInt8),
        CONFIG_UINT16("unsigned.int16", &cUInt16),
        CONFIG_UINT32("unsigned.int32", &cUInt32),
        CONFIG_UINT64("unsigned.int64", &cUInt64),

        CONFIG_DOUBLE("aDouble", &cDouble),
        CONFIG_BOOL("aBool", &cBool),
        CONFIG_CALLBACK("aCallback", &exampleCallback, &callbackResult),

        CONFIG_INT("namespace.key1", &nKey1),
        CONFIG_INT("namespace.key2", &nKey2),
        CONFIG_INT("otherspace.key1", &oKey1),
        CONFIG_END()
    });

    // Print all values
    printf("\nSTRINGS:\n");
    printf("\taString:    %s\n", cString);
    printf("\taNewString: %s\n", cNewString);

    printf("\nSIGNED INT:\n");
    printf("\tint:      %d\n", cInt);
    printf("\tint8:     %"PRId8"\n", cInt8);
    printf("\tint16:    %"PRId16"\n", cInt16);
    printf("\tint32:    %"PRId32"\n", cInt32);
    printf("\tint64:    %"PRId64"\n", cInt64);

    printf("\nUNSIGNED INT:\n");
    printf("\tuint8:     %"PRIu8"\n", cUInt8);
    printf("\tuint16:    %"PRIu16"\n", cUInt16);
    printf("\tuint32:    %"PRIu32"\n", cUInt32);
    printf("\tuint64:    %"PRIu64"\n", cUInt64);

    printf("\nOTHER NUMBERS:\n");
    printf("\taDouble:    %f\n", cDouble);
    printf("\taBool:      %d\n", cBool);
    printf("\taCallback:  %d\n", callbackResult);

    printf("\nNAMESPACES:\n");
    printf("\tnamespace.key1:   %d\n", nKey1);
    printf("\tnamespace.key2:   %d\n", nKey2);
    printf("\totherspace.key1:  %d\n", oKey1);
    return EXIT_SUCCESS;
}
