#include "mik64_converter.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <chrono>
std::map<std::string, std::string> html_templateMap = {
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
std::map<std::string, std::string> text_templateMap = {
    {"", ""}
};

auto test(int count){
    mik64::Converter html_c(html_templateMap, childElementMap, true, 1, false);
    mik64::Converter text_c(text_templateMap, text_templateMap, false, 0, false);
    std::ifstream file("input/test.md");
    std::ofstream outFile("output/output.html");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < count; i++){
        std::ostringstream html_result = html_c.convert(content);
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::milli> duration = end - start;
    std::ostringstream html_result = html_c.convert(content);
    std::ostringstream text_result = text_c.convert(content);
    outFile << html_result.str() << "\n\n\n" << text_result.str();
    outFile.close();
    return duration.count();
}

int main(){
    int count=10000;
    std::cout << "count("<<count<<"): " << test(count) << " ms\n";
    return 0;
}