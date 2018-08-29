# Config.h

*Config.h* is a single-header configuration file parser library written in C.

Parsing configuration files is common task for all kinds of applications.
This library can ease configuration file parsing in your existing or future C applications.

Using *Config.h* in your application is simple.
Just add `Config.h` to your project directory and include it using:

```C
    #define CONFIG_IMPLEMENTATION 
    #include "Config.h"
```

*Config.h* is licensed under the [ISC license](https://github.com/MartinWeigel/Config/blob/master/LICENSE-ISC.txt)


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
- Simple interface with only one public function: `Config_parse`
- Support of different variable types: `bool`, `int`, `double`, and `string`
- Support of function callbacks for advanced configuration control
- Liberal license for free and commercial software