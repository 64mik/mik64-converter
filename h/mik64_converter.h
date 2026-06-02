#ifndef CONVERTER_H
#define CONVERTER_H
#include <string>
#include <map>
#include <vector>
namespace mik64 {
    class Converter {
        public: 
            Converter() = delete;
            Converter(const std::map<std::string, std::string>& templateMap, bool tagIndentation = true, int defaultIndentation = 0);
            ~Converter();
            std::ostringstream convert(const std::string& string);
            inline void setTemplateMap(const std::map<std::string, std::string>& templateMap) {
                templateMap_ = templateMap;
            }
        private:
            void append(std::ostringstream& oss, const std::string& str, int indent = 0);
            void replace(std::string& s, const std::string& key, const std::string& value);
            std::pair<std::string, std::string> getTag(const std::string& tag);
            std::map<std::string, std::string> templateMap_;
            std::vector<std::pair<std::string, std::string>> tagStack_;
            struct markers{
                std::string start_marker="{";
                std::string end_marker="}";
                std::string func_start_marker="(";
                std::string func_end_marker=")";
                std::string ignore_marker="//";
            } markers_;
            bool tagIndentation_ = true;
            int defaultIndentation_ = 0;
};
}
#endif // CONVERTER_H