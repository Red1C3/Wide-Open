#pragma once
#include<fstream>
#include<chrono>
#include<string>
#include<string.h>
namespace Common{
    class Log{
    private:
        Log(){}
        bool isLoaded=false;
        void loadLogFile();
        std::ofstream stream;
    public:
        static Log& instance();
        void log(const char*);
        void log(int numbers[],int count);
        void error(const char*,bool isFatal=true);
        void close();
    };
}