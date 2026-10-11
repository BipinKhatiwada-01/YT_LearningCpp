#include <iostream>

class Enemy{
    public:
    int health;
    int position;

    Enemy() 
        : health(100), position(0){
        std::cout<< health << " is health and " << position << " is position" << "\n";
    }

    Enemy(int value, int cord) 
        : health(value), position(cord){
        std::cout<< health << " is health and " << position << " is position" << "\n";
    }

    ~Enemy(){
        std::cout<<"Destroying Enemy which had health of " << health <<" and in position " << position<< "\n";
    }

};

int main() {
    
    Enemy en1;
    Enemy en2(150, 4);

    {
        Enemy en3(300, 10);
        std::cout << "Inside battle\n";
    }

    std::cout << "Remaining battle continues outside\n";
    std::cout << "Victory\n";
}

//I wrote the code myself. Docs by AI; Idk why I am explaing this-