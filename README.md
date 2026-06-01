# mik64-converter

A C++ library that converts Markdown strings to HTML.

## Features

- Map-Based String Replacement: Efficiently replaces specific keywords within strings using pre-defined map pairs.
- Nested Tag Support: Supports nested tags within other tags. It internally utilizes a `std::vector` to ensure opening and closing tags are perfectly matched in order.
- Flexible Tag Configuration (`|` Delimiter): Allows you to specify opening and closing tags using the `|` delimiter. If you only need a single tag, simply place the `|` at the very end of the string.

## How to use

```cpp
#include "mik64_converter.h"

int main(){    
    std::map<std::string, std::string> templateMap = {
        {"box", "<div class=\"box\">|</div>"},
        {"p",   "<p>|</p>"},
        {"list","<ol>|</ol>"}
    };
    mik64::Converter c(templateMap);
    std::ifstream file("input/test.md");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::ostringstream result = c.convert(content);
    std::cout << result.str();
}
```

## Building

This project uses C++17 and requires a C++ compiler

### Prerequisites

- C++17 compatible compiler
- Windows: MinGW-w64 or MSVC
- Cmake 3.15+

## Example Input

### map
```
std::map<std::string, std::string> templateMap = {
    {"box", "<div class=\"box\">|</div>"},
    {"link", "<a href=\"{{data}}\">|</a>"},
    {"p", "<p>|</p>"},
    {"list", "<ol>|</ol>"}
};
```

### md
```
box{
    a
    list{
        b
        c
        d
    }
}
```

## Example Output
```
<div class="box">
a
<ol>
b
c
d
</ol>
</div>
```

## License

MIT License