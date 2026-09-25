#include <iostream>

using namespace std;

int Add(int a, int b)
{
    return a + b;
}

void PrintResult(int result)
{
    cout<< "Result: " << result <<endl;
}

int main()
{
    int result = Add(10, 12);

    PrintResult(result);

    return 0;
}