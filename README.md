# Config.h

*Config.h* is a single-header library written in C for parsing configuration files .

Parsing configuration files is common task for all kinds of applications.
This library parses human-readable configuration files for your existing or future C applications.
Using *Config.h* in your application is easy. Just add `Config.h` to your project directory and include it.

*Config.h* is licensed under the [ISC license](https://github.com/MartinWeigel/Config.h/blob/master/LICENSE-ISC.txt)


## Minimal Example
```C
#define CONFIG_IMPLEMENTATION
#include "Config.h"

int main(int argc, const char **argv)
{
    // Declare your variables
    int cInt = 0;
    double cDouble = 0.f;

    // Load values from configuration file to variables
    Config_load("example.txt", (ConfigItem[]) {
        CONFIG_INT("anInt", &cInt),
        CONFIG_DOUBLE("aDouble", &cDouble),
        CONFIG_END()
    });

    // Print the read values from the configuration file
    printf("Read config: anInt is %d and aDouble is %f\n", cInt, cDouble);
    return EXIT_SUCCESS;
}
```

An example with all data types and a callback can be found in [example.c](https://github.com/MartinWeigel/Config.h/blob/master/example.c).


## Configuration Format
```Markdown
# CONFIGURATION FILE EXAMPLE

# Each config item should be written in its own line.
# Format: <key> = <value>
# Keys should not contain any whitespaces.

# All lines starting with # are comments.
# Empty lines are allowed to structure the document.

# You can define different configuration types
aBool   = 1
aString = Hello World
aDouble = 42.777
anInt   = 42

# Namespaces are just a prefix for the key
namespace.key1  = value1
namespace.key2  = value2
otherspace.key1 = 1
otherspace.key2 = 2
```

## Features

- Single-header library for easy integration into existing projects
- Human-readable configuration file format
- Support of different variable types: `bool`, `int`, `double`, and `string`
- Simple interface with only one public function: `Config_load`
- Support of function callbacks for advanced configuration control
- Liberal license for free and commercial software
