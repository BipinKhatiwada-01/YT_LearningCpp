#include <iostream>

int main()
{
    for (int i = 1; i <= 10; i++)
    {
        if (i == 8)
            continue; // Skip 8

        if (i == 9)
            break; // Stop the loop at 9

        std::cout << "Checking number: " << i << std::endl;
    }

    return 0;
}
