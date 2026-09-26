#include<iostream>

int main(){


    int x = 0;

    std::cout<<"Enter a value for x : ";
    std::cin>>x;

    bool CheckResult  = x == 5;

    if( CheckResult){
        std::cout<<"Hello, World"<<std::endl;
    }

    std::cout<<"Hi lol"<<std::endl;

    const char* ptr = "Hi"; //For now Idk how const allows char* ptr to store a string. Maybe makes ptr an array? idk- for now.

    if(ptr != nullptr){
        std::cout<<"Pointer is NOT null"<<std::endl;
    }

    if(ptr == "Hi"){
        std::cout<<"Pointer says 'Hi'"<<std::endl;
    }

    else if(ptr != "Hi"){
        std::cout<<"Pointer is NOT saying 'Hi'"<<std::endl;
    }

    else{
        std::cout<<"Cricket  Stridulations"<<std::endl;
    }


}


