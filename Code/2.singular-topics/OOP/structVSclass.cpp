#include <iostream>
using namespace std;

// Struct example
struct Student
{
    string name;
    int age;

    void display()
    {
        cout << "Student (struct): " << name 
             << ", Age: " << age << endl;
    }
};


// Class example
class Teacher
{
    string name;
    int age;

public:
    void setData(string n, int a)
    {
        name = n;
        age = a;
    }

    void display()
    {
        cout << "Teacher (class): " << name 
             << ", Age: " << age << endl;
    }
};


int main()
{
    // Struct object
    Student s;

    // Direct access is allowed because struct members are public by default
    s.name = "Al";
    s.age = 21;

    s.display();


    // Class object
    Teacher t;

    // Direct access is NOT allowed:
    // t.name = "Jason";  // Error: private by default

    t.setData("Jason", 29);
    t.display();

    return 0;
}
