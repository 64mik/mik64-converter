#include "mik64_converter.h"
#include "mlog.h"
#include "logger_factory.h"
namespace mik64 {

//public:
Converter::Converter(const std::map<std::string, std::string>& templateMap, const std::map<std::string, std::string>& childElementMap, bool tagIndentation, int defaultIndentation, bool fallbackToRawText) : templateMap_(templateMap), childElementMap_(childElementMap), tagIndentation_(tagIndentation), defaultIndentation_(defaultIndentation), fallbackToRawText_(fallbackToRawText) {
    mik64::Logger::getInstance().setWriters(mik64::LoggerFactory::getWriters({"console", "file"}));
    MLOG_INFO("Converter initialized.");
}
Converter::~Converter() {
    MLOG_INFO("Converter destroyed.");
}

std::ostringstream Converter::convert(const std::string& str) {
    std::istringstream iss(str);
    std::ostringstream result;
    std::string line;
    std::pair<std::string, std::string> tag;    //<open>|<close>
    std::string element;    //box, list, p etc. from templateMap

    while (std::getline(iss, line)) {
        //del front whitespace
        line.erase(0, line.find_first_not_of(" \t"));  
        if (line.empty()) {
            result << "\n";
            continue;
        }
        //comment line
        if (line.find(markers_.ignore_marker) == 0) {   
            continue;
        }

        //open tag
        if (line.find(markers_.start_marker) != std::string::npos) {
            MLOG_INFO("Processing line: " + line);
            element = line.substr(0, line.find(markers_.start_marker));
            element.erase(element.find_last_not_of(" \t") + 1);
            if(templateMap_[element]!=""){
                MLOG_INFO("Found tag: " + element);
                tag = getTag(element);
                append(result, tag.first, tagStack_.size());
                tagStack_.push_back(element);
            }
            else{
                MLOG_WARNING("Tag not found: " + element);
                if(fallbackToRawText_){
                    append(result, line, tagStack_.size());
                }
            }
            continue;
        }
        //close tag
        else if (line.find(markers_.end_marker) != std::string::npos) {
            if (!tagStack_.empty()) {
                tag = getTag(tagStack_.back());
                append(result, tag.second, tagStack_.size()-1);
                tagStack_.pop_back();
            }
            if (fallbackToRawText_){
                append(result, markers_.end_marker, tagStack_.size()-1);
            }
            continue;
        }
        //macro
        if (line.find(markers_.func_start_marker) != std::string::npos ) {
            element = line.substr(0, line.find(markers_.func_start_marker));
            element.erase(element.find_last_not_of(" \t") + 1);
            size_t startPos = line.find(markers_.func_start_marker);
            size_t endPos = line.find(markers_.func_end_marker);
            if(templateMap_[element]!="" && endPos != std::string::npos){
                tag = getTag(element);
                std::string macroLine = tag.first+tag.second;
                std::string parameters = line.substr(startPos + 1, endPos - startPos - 1);
                MLOG_INFO("Processing macro: " + line.substr(0, startPos) + " with parameters: " + parameters);
                while (!parameters.empty()) {
                    size_t commaPos = parameters.find(",");
                    std::string item;
                    item = parameters.substr(0, commaPos);
                    MLOG_INFO("Parameter: " + item);
                    size_t pos = macroLine.find("{{data}}");
                    //replace first occurrence of {{data}} with item
                    if(pos != std::string::npos){
                        macroLine = macroLine.replace(pos, 8, item);
                    }
                    //not enough placeholder
                    else{
                        MLOG_WARNING("Placeholder {{data}} is less than expected: " + macroLine);
                        break;
                    }
                    //remove processed parameter
                    if(commaPos != std::string::npos){
                        parameters = parameters.substr(commaPos + 1);
                    }
                }
                //not enough parameters
                if(macroLine.find("{{data}}") != std::string::npos){
                    MLOG_WARNING("parameters are less than required for placeholders: " + macroLine);
                    while(macroLine.find("{{data}}") != std::string::npos){
                        macroLine = macroLine.replace(macroLine.find("{{data}}"), 8, "");
                    }
                }
                //use unknown macro
                if(macroLine == tag.first+tag.second){
                    MLOG_INFO("unknown macro: " + line);
                    append(result, line, tagStack_.size());
                    continue;
                }
                append(result, macroLine, tagStack_.size());
            }
            //unknown tag
            else{
                if(fallbackToRawText_){
                    append(result, line, tagStack_.size()-1);
                }
            }
            continue;
        }   

        //normal line
        tag = getChildTag(tagStack_.empty() ? "" : tagStack_.back());
        append(result, tag.first + line + tag.second, tagStack_.size());
    }
    return result;
}

//private:
void Converter::append(std::ostringstream& src, const std::string& str, int indent) {
    if(!tagIndentation_){
        indent = 0;
    }
    indent += defaultIndentation_;
    for (int i = 0; i < indent; ++i) {
        src << "    ";
    }
    MLOG_INFO("Appending line: " + str + " with indentation: " + std::to_string(indent));
    src << str << "\n";
}
//get open and close tag from templateMap
std::pair<std::string, std::string> Converter::getTag(const std::string& tag){
    std::string openTag, closeTag;
    auto it = templateMap_.find(tag);
    if(it != templateMap_.end()) {
        size_t pos = it->second.find("|");
        openTag = it->second.substr(0, pos);
        closeTag = it->second.substr(pos + 1);
    } else {
        MLOG_WARNING("Tag not found in templateMap: " + tag);
        openTag = "";
        closeTag = "";
    }
    return std::make_pair(openTag, closeTag);
}
std::pair<std::string, std::string> Converter::getChildTag(const std::string& tag){
    std::string openTag, closeTag, element;
    auto it = childElementMap_.find(tag);
    if(it != childElementMap_.end()) {
        element = it->second;
    } else {
        element = childElementMap_[""];
    }
    openTag = templateMap_[element].substr(0, templateMap_[element].find("|"));
    closeTag = templateMap_[element].substr(templateMap_[element].find("|") + 1);
    return std::make_pair(openTag, closeTag);
}

}