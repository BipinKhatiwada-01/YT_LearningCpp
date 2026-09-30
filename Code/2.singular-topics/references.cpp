#include <iostream>
using namespace std;

void deposit(int& balance)
{
    int amount;
    cout<<"Money to be Deposited: $";
    cin>>amount;
    balance = balance + amount;
}

int main()
{
    int balance = 1000;

    cout << "Before deposit: $" << balance << endl;

    deposit(balance);

    cout << "After deposit: $" << balance << endl;

    return 0;
}
