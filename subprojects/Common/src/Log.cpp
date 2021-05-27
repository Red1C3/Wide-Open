/*MIT License

Copyright (c) 2021 Mohammad Issawi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#include<Log.h>
using namespace Common;
using namespace std;
using namespace chrono;
static inline string getTimeFormatted(){
    string time="";
    auto duration=system_clock::now().time_since_epoch();
    auto seconds=duration_cast<std::chrono::seconds>(duration).count();
    auto minutes=duration_cast<chrono::minutes>(duration).count();
    auto hours=duration_cast<chrono::hours>(duration).count();
    time.append(to_string((hours%24)+3));
    time.append(":");
    time.append(to_string(minutes%60));
    time.append(":");
    time.append(to_string(seconds%60));
    return time;
}
Log& Log::instance(){
    static Log INSTANCE;
    return INSTANCE;
}
void Log::loadLogFile(){
    stream=ofstream("debug.txt");
    isLoaded=true;
}
void Log::log(const char* msg){
    if(!isLoaded) loadLogFile();
    string time=getTimeFormatted();
    stream.write(time.c_str(),time.size());
    stream.write("-LOG-:",strlen("-LOG-:"));
    stream.write(msg,strlen(msg));
    stream.write("\n",1);
}
void Log::log(int numbers[],int count){
    string msg="Parameters for the previous log:";
    for(int i=0;i<count;i++){
        msg.append(to_string(numbers[i]));
        msg.append(",");
    }
    log(msg.c_str());
}
void Log::error(const char* error,bool isFatal){
    if(!isLoaded) loadLogFile();
    string time=getTimeFormatted();
    stream.write(time.c_str(),time.size());
    stream.write("-ERR-:",strlen("-LOG-:"));
    stream.write(error,strlen(error));
    stream.write("\n",1);
    if(isFatal){
        close();
        throw exception();
    }
}
void Log::close(){
    stream.flush();
    stream.close();
}