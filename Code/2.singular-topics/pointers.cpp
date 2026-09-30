#include<iostream>

int main(){

    int x = 37;
    int* ptr =  &x;

    std::cout<<"************************************"<<std::endl;

    std::cout<<"ptr = "<<ptr<<std::endl;
    std::cout<<"&x = "<<&x<<std::endl;
    std::cout<<"x = "<<x<<std::endl;
    std::cout<<"*ptr = "<<*ptr<<std::endl;

    *ptr = 100;

    std::cout<<"************************************"<<std::endl;

    std::cout<<"ptr = "<<ptr<<std::endl;
    std::cout<<"&x = "<<&x<<std::endl;
    std::cout<<"x = "<<x<<std::endl;
    std::cout<<"*ptr = "<<*ptr<<std::endl;

    std::cout<<"************************************"<<std::endl;  
    
    std::cout<<"Enter value for *ptr = "<<std::endl;
    std::cin>>*ptr;
    std::cout<<"ptr = "<<ptr<<std::endl;
    std::cout<<"&x = "<<&x<<std::endl;
    std::cout<<"x = "<<x<<std::endl;
    std::cout<<"*ptr = "<<*ptr<<std::endl;

    std::cout<<"************************************"<<std::endl;  

    int** ptr2 = &ptr;
    **ptr2 = 7;

    std::cout<<"ptr = "<<ptr<<std::endl;
    std::cout<<"&x = "<<&x<<std::endl;
    std::cout<<"*ptr2 = "<<*ptr2<<std::endl;
    std::cout<<"x = "<<x<<std::endl;
    std::cout<<"*ptr = "<<*ptr<<std::endl;
    std::cout<<"**ptr = "<<**ptr2<<std::endl;
    std::cout<<"&ptr = "<<&ptr<<std::endl;
    std::cout<<"ptr2 = "<<ptr2<<std::endl;

    std::cout<<"************************************"<<std::endl;  

}