#include "mik64_converter.h"
#include "mlog.h"
#include "logger_factory.h"
namespace mik64 {

//public:
Converter::Converter(const std::map<std::string, std::string>& templateMap) : templateMap_(templateMap) {
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
    std::string staging;
    std::pair<std::string, std::string> tag;
    std::string element;

    while (std::getline(iss, line)) {
        //앞 공백 제거
        line.erase(0, line.find_first_not_of(" \t"));  
        if (line.empty()) {
            result << "\n";
            continue;
        }
        //주석
        if (line.find(markers_.ignore_marker) == 0) {   
            continue;
        }

        //열기
        if (line.find(markers_.start_marker) != std::string::npos) {
            element = line.substr(0, line.find(markers_.start_marker));
            element.erase(element.find_last_not_of(" \t") + 1);  //뒤 공백 제거
            if(templateMap_[element]!=""){  //존재하는 테그면
                MLOG_INFO("Found tag: " + element);
                tag = getTag(element);
                tagStack_.push_back(tag);
                result << tag.first << "\n";    //열기 부분만 출력하고 스텍에 저장
                continue;
            }
        }
        //닫기
        else if (line.find(markers_.end_marker) != std::string::npos) {
            if (!tagStack_.empty()) {
                result << tagStack_.back().second << "\n";
                tagStack_.pop_back();
                continue;
            }
        }

        //함수 호출
        if (line.find(markers_.func_start_marker) != std::string::npos ) {
            size_t startPos = line.find(markers_.func_start_marker);
            size_t endPos = line.find(markers_.func_end_marker);

            if(templateMap_[line.substr(0, startPos)]!="" && endPos != std::string::npos){    //존재하는 함수면
                std::string prameter = line.substr(startPos + 1, endPos - startPos - 1);
                //매개변수 치환 및 삽입
                //일단 그냥 출력으로
                result << line << "\n";
            }
            continue;
        }   
        result << line << "\n";    //테그가 없는 일반 텍스트는 그대로 출력
    }
    return result;
}

//private:
void Converter::replace(std::string& s, const std::string& key, const std::string& value) {
    size_t pos;
    while ((pos = s.find(key)) != std::string::npos)
        s.replace(pos, key.length(), value);
}
//열기 닫기 테그 얻어오는 함수
std::pair<std::string, std::string> Converter::getTag(const std::string& tag){
    std::string openTag, closeTag;
    auto it = templateMap_.find(tag);
    if(it != templateMap_.end()) {
        size_t pos = it->second.find("|");
        openTag = it->second.substr(0, pos);
        closeTag = it->second.substr(pos + 1);
    } else {
        openTag = "";
        closeTag = "";
    }
    return std::make_pair(openTag, closeTag);
}
}