# mik64-converter

A C++ library that converts Markdown strings to HTML.

## Features

- Map-Based String Replacement: Efficiently replaces specific keywords within strings using pre-defined map pairs.
- Nested Tag Support: Supports nested tags within other tags. It internally utilizes a `std::vector` to ensure opening and closing tags are perfectly matched in order.
- Flexible Tag Configuration (`|` Delimiter): Allows you to specify opening and closing tags using the `|` delimiter. If you only need a single tag, simply place the `|` at the very end of the string.
- Macro Support: Allows multiple parameters using the `(param1,param2)` syntax (e.g. `link(url,text)`). Add `{{data}}` placeholders to your templateMap to use this feature. Extra parameters beyond the number of placeholders are ignored.

## How to use

```cpp
#include "mik64_converter.h"

int main(){    
    std::map<std::string, std::string> templateMap = {
        {"box", "<div class=\"box\">|</div>"},
        {"p",   "<p>|</p>"},
        {"list","<ol>|</ol>"}
    };
    std::map<std::string, std::string> childElementMap = {
        {"", "p"},
        {"list", "li"},
        {"bold", ""}
    };
    mik64::Converter c(templateMap, childElementMap, true, 2, false);
    std::ifstream file("input/test.md");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::ostringstream result = c.convert(content);
    std::cout << result.str();
}
```

### Converter

- `templateMap`: A map used to define HTML tag templates
- `childElementMap`: A map used to define default child elements per tag
- `tagIndentation`: Applies indentation based on nesting depth
- `defaultIndentation`: Applies a base indentation level to all lines
- `fallbackToRawText`: If true, unknown tags are output as raw text instead of being skipped

## Example Input

### map

```
std::map<std::string, std::string> templateMap = {
    {"box", "<div class=\"box\">|</div>"},
    {"link", "<a href=\"{{data}}\">{{data}}</a>|"},
    {"bold", "<strong>|</strong>"},
    {"p", "<p>|</p>"},
    {"li", "<li>|</li>"},
    {"list", "<ul>|</ul>"}
};
std::map<std::string, std::string> childElementMap = {
    {"", "p"},
    {"list", "li"},
    {"bold", ""}
};
```

### text

```
box{
    // you can use comment like this.
    test1
    // c is unused: not enough placeholders.
    link(link,hello,c)
    // Ignores leading whitespaces.
    link    (link2,wo,c)
    list   {
        rl
        d!
    }
}
```

## Example Output

```
    <div class="box">
        <p>test1</p>
        <a href="link">hello</a>
        <a href="link2">wo</a>
        <ul>
            <li>rl</li>
            <li>d!</li>
        </ul>
    </div>
```


## Building

This project uses C++17 and requires a C++ compiler

### Prerequisites

- C++17 compatible compiler
- Windows: MinGW-w64 or MSVC
- Cmake 3.15+

## License

MIT License