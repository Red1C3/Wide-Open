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
    time.append(to_string((hours%24)+2));
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