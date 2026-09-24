#include <iostream>

int main()
{
    // Different variables can store different types of data.
    int age = 21;

    // float is useful when we need a number with a decimal part.
    float height = 5.9f;

    // double is also used for decimal numbers, but provides more precision than float.
    double balance = 1250.50;

    // char stores a single character.
    char grade = 'A';

    // bool stores one of two values: true or false.
    bool isStudent = true;

    // Print the values stored in our variables.
    std::cout << "Age: " << age << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Balance: " << balance << std::endl;
    std::cout << "Grade: " << grade << std::endl;
    std::cout << "Student: " << isStudent << std::endl;


    // Variables can be changed after they have been created.
    age = 22;

    // We can also use the current value of a variable to calculate a new value.
    balance = balance + 500;

    std::cout << "\nAfter changes:\n";
    std::cout << "Age: " << age << std::endl;
    std::cout << "Balance: " << balance << std::endl;


    // sizeof tells us how much memory a type takes on this system.
    // The exact size of some types can depend on the compiler and platform.
    std::cout << "\nMemory sizes:\n";

    std::cout << "int: " << sizeof(int) << " bytes\n";
    std::cout << "float: " << sizeof(float) << " bytes\n";
    std::cout << "double: " << sizeof(double) << " bytes\n";
    std::cout << "char: " << sizeof(char) << " byte\n";
    std::cout << "bool: " << sizeof(bool) << " byte\n";

    return 0;
}
