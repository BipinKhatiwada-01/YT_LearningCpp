#include <iostream>

class Entity {
public:
    inline static int count = 0;

   //below is  a constructor.
    Entity() { 
        ++count;
    }
};

int main() {
    Entity a;
    Entity b;
    Entity c;

    std::cout << "Entities created: " << Entity::count << '\n';
}