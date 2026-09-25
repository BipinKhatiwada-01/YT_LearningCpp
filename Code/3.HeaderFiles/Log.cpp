#include "Log.h"
#include<iostream>

using namespace std;

void InitLog(){
    Log("Initializing Log");
}

void Log(const char* message){
    cout << "Message: " << message << endl;
}