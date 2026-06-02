#include "mik64_converter.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
int main(){
    std::map<std::string, std::string> templateMap = {
        {"box", "<div class=\"box\">|</div>"},
        {"link", "<a href=\"{{data}}\">|</a>"},
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
    mik64::Converter c(templateMap, childElementMap, true, 1);
    std::ifstream file("input/test.md");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::ostringstream result = c.convert(content);
    std::ofstream outFile("output/output.html");
    outFile << result.str();
    outFile.close();
    return 0;
}