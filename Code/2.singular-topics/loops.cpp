#include <iostream>

int main(){

    // FOR LOOP
    // Good when you know how many times you want to loop
    for (int i = 0; i < 5; i++)
    {
        std::cout << "For loop: " << i << std::endl;
    }


    // WHILE LOOP
    // Good when you want to keep going while a condition is true
    bool playing = true;

    while (playing)
    {
        char answer;

        std::cout << "Do you want to keep playing? (y/n): ";
        std::cin >> answer;

        if (answer == 'n')
        {
            playing = false;
        }
    }


    // DO WHILE LOOP
    // Runs at least once, then checks the condition
    char answer;

    do
    {
        std::cout << "Enter 'y' to continue or 'n' to stop: ";
        std::cin >> answer;

    } while (answer != 'n');


    std::cin.get();
}


