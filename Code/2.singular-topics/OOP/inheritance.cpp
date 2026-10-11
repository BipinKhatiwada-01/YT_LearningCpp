#include <iostream>

class Sword {
public:
    int damage = 25;
};

class Character {
public:
    int health = 100;

    Character()  { std::cout << "Character constructed\n"; }
    ~Character() { std::cout << "Character destroyed\n"; }

    void TakeDamage(int damage) { health -= damage; }
};

class Warrior : public Character {
public:
    Sword weapon;   // has-a

    Warrior()  { std::cout << "Warrior constructed\n"; }
    ~Warrior() { std::cout << "Warrior destroyed\n"; }
};

int main() {
    Warrior samwell;
    Warrior frodo;

    samwell.TakeDamage(30);

    std::cout << "samwell health: " << samwell.health << '\n';  // 70
    std::cout << "frodo health: " << frodo.health << '\n';      // 100
    std::cout << "samwell weapon damage: " << samwell.weapon.damage << '\n';
}