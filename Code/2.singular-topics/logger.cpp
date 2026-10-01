#include<iostream>

using namespace std;

class Logger{
    public:
    const int LogLevelError = 0;
    const int LogLevelWarn = 1;
    const int LogLevelInfo = 2;
    
    private:
    int m_logLevel;

    public:
    void SetLevel(int level){
        m_logLevel = level;
    }

       void Error(const char* message)
    {
        if (m_logLevel >= LogLevelError)
            std::cout << "[ERROR]: " << message << std::endl;
    }

    void Warn(const char* message)
    {
        if (m_logLevel >= LogLevelWarn)
            std::cout << "[WARNING]: " << message << std::endl;
    }

    void Info(const char* message)
    {
        if (m_logLevel >= LogLevelInfo)
            std::cout << "[INFO]: " << message << std::endl;
    }
};

int main(){

    Logger log;

    log.SetLevel(log.LogLevelWarn);

    log.Error("Something went wrong.");
    log.Warn("Something might be wrong.");
    log.Info("Everything is running normally.");

}

