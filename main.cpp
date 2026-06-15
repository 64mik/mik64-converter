#include "mik64_converter.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
int main(){
    std::map<std::string, std::string> html_templateMap = {
        {"box", "<div class=\"box\">|</div>"},
        {"link", "<a href=\"{{data}}\">{{data}}</a>|"},
        {"bold", "<strong>|</strong>"},
        {"p", "<p>|</p>"},
        {"li", "<li>|</li>"},
        {"list", "<ul>|</ul>"}
    };
    std::map<std::string, std::string> text_templateMap = {
        {"", ""}
    };
    std::map<std::string, std::string> childElementMap = {
        {"", "p"},
        {"list", "li"},
        {"bold", ""}
    };
    mik64::Converter html_c(html_templateMap, childElementMap, true, 1);
    mik64::Converter text_c(text_templateMap, text_templateMap, true, 0, true);
    std::ifstream file("input/test.md");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::ostringstream html_result = html_c.convert(content);
    std::ostringstream text_result = text_c.convert(content);

    std::ofstream outFile("output/output.html");
    outFile << html_result.str() << "\n\n\n" << text_result.str();
    outFile.close();
    return 0;
}