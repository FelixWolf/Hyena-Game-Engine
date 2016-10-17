#include <vector>
#include <string>
using namespace std;

namespace logging{
    
    enum level{
        none,
        fatal,
        error,
        warn,
        info,
        debug,
        trace
    };
    
    const std::vector<std::string> levelstr = {
        "",
        "FATAL",
        "ERROR",
        "WARN",
        "INFO",
        "DEBUG",
        "TRACE"
    };
    
    uint8_t logLevel = level::error;
    void setLevel(const uint8_t lvl){
        logLevel = lvl;
    }
    
    bool log(const uint8_t lvl, const char* msg){
        if(logLevel >= lvl){
            fprintf(stdout, "[%s] %s", levelstr[lvl].c_str(), msg);
            return true;
        }
        return false;
    }
}
