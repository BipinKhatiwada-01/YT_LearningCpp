#include<iostream>

void Log(const char* message);

int main(){

    Log("Log: Hello World");
    std::cout<<"Normal: Hello World"<<std::endl;
    std::cin.get(); // Holds the program until enter button on keyboard or any input device(?) is clicked.

}