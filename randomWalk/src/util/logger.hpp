#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


#define WC2_LOG(...) logger::log(__FILE__, __LINE__, __VA_ARGS__)
#define WC2_WARN(...) logger::warn(__FILE__, __LINE__, __VA_ARGS__)
#define WC2_ERROR(...) logger::error(__FILE__, __LINE__, __VA_ARGS__)

#define LOG_TO_CONSOLE
//#define LOG_TO_FILE

namespace logger {
    std::string pad(int n);

#ifdef LOG_TO_FILE
    inline std::ofstream fileStream;

    inline void fileOpen() {
        fileStream.open(global::basePath + "data/latestlog.rtf");
        fileStream << "{\\rtf1\\ansi {\\colortbl;\\red0\\green0\\blue0;\\red255\\green170\\blue0;\\red255\\green0\\blue0;}";
    }

    inline void fileClose() {
        fileStream << "}";
        fileStream.close();
    }
#endif //LOG_TO_FILE

    template<typename... Args>
    inline void log(const char* file, int line, Args&&... args) {
        std::string stringFile(file);
        std::string_view filename(stringFile.substr(stringFile.find_last_of("/\\") + 1));

        time_t timestamp = time(&timestamp);
        struct tm datetime = *localtime(&timestamp);

#ifdef LOG_TO_CONSOLE
        std::cout << "[" << pad(datetime.tm_hour) << ":" << pad(datetime.tm_min) << ":" << pad(datetime.tm_sec) << "] " << filename << " at line " << line << ": ";
        (std::cout << ... << args) << "\n";
#endif

#ifdef LOG_TO_FILE
        fileStream << "\\cf1[" << pad(datetime.tm_hour).c_str() << ":" << pad(datetime.tm_min).c_str() << ":" << pad(datetime.tm_sec).c_str() << "] " << filename.data() << " at line " << std::to_string(line).c_str() << ": ";
        (fileStream << ... << args) << "\\line";
#endif

    }

    template<typename... Args>
    inline void warn(const char* file, int line, Args&&... args) {
        std::string stringFile(file);
        std::string_view filename(stringFile.substr(stringFile.find_last_of("/\\") + 1));

        time_t timestamp = time(&timestamp);
        struct tm datetime = *localtime(&timestamp);

#ifdef LOG_TO_CONSOLE
        std::cout << "\033[93m[" << pad(datetime.tm_hour) << ":" << pad(datetime.tm_min) << ":" << pad(datetime.tm_sec) << "] " << filename << " at line " << line << ": ";
        (std::cout << ... << args) << "\033[m\n";
#endif

#ifdef LOG_TO_FILE
        fileStream << "\\cf2[" << pad(datetime.tm_hour).c_str() << ":" << pad(datetime.tm_min).c_str() << ":" << pad(datetime.tm_sec).c_str() << "] " << filename.data() << " at line " << std::to_string(line).c_str() << ": ";
        (fileStream << ... << args) << "\\line";
#endif

    }

    template<typename... Args>
    inline void error(const char* file, int line, Args&&... args) {
        std::string stringFile(file);
        std::string_view filename(stringFile.substr(stringFile.find_last_of("/\\") + 1));

        time_t timestamp = time(&timestamp);
        struct tm datetime = *localtime(&timestamp);

#ifdef LOG_TO_CONSOLE
        std::cout << "\033[91m[" << pad(datetime.tm_hour) << ":" << pad(datetime.tm_min) << ":" << pad(datetime.tm_sec) << "] " << filename << " at line " << line << ": ";
        (std::cout << ... << args) << "\033[m\n";
#endif

#ifdef LOG_TO_FILE
        fileStream << "\\cf3[" << pad(datetime.tm_hour).c_str() << ":" << pad(datetime.tm_min).c_str() << ":" << pad(datetime.tm_sec).c_str() << "] " << filename.data() << " at line " << std::to_string(line).c_str() << ": ";
        (fileStream << ... << args) << "\\line";
#endif
    }

    void GLFWErrorCallback(int code, const char* description);
}