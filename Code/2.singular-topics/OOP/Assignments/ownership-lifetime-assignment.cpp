#include <iostream>
#include <iterator>
#include <utility>
#include <vector>
#include <memory>

class Enemy{
    private:
    const char* name;
    int health;

    public:

        void EnemyStatus(){
            std::cout<< name << "'s health is " << health << "\n";
        }
        
        bool isDefeated() const {
            return health == 0;
        }

        void takeDamage(int d) {

            if(d < 0){
                std::cout<< "Invalid damage points\n";
                return;
            }      
            else if(health == 0){
                std::cout << "Don't toy around with a dead " << name << " body too much..\n";
            }  
            else if(d > health){
                std::cout<< name << " took damage of " << d << "\n";
                health = 0;
            }
            else if(health > 0){
                std::cout<< name << " took damage of " << d << "\n";
                health -= d;
            }
        }

        virtual void attack()= 0;

        Enemy() : name("Enemy"), health(100){}
        Enemy(const char* n, int val) : name(n), health(val){}

        virtual ~Enemy(){
            std::cout<<"Enemy Destroyed\n";
        }
};

class Goblin :
    public Enemy{
        public:

        void attack() override {
            std::cout<<"Goblin stabs with a dagger !\n";
        }
  
        Goblin() : Enemy("Goblin", 150){}

        ~Goblin() override {
            std::cout<<"Goblin Destroyed\n";
        }

};

class Dragon
    : public Enemy {
        public:

        void attack() override{
            std::cout<< "Dragon breathes fire at the player\n";
        }

        Dragon() : Enemy("Dragon", 300) {}

        ~Dragon() override{
            std::cout<<"Dragon Destroyed\n";
        }
};


int main(){

    std::vector<std::unique_ptr<Enemy>> enemies;

    auto gob = std::make_unique<Goblin>();
    auto dragg = std::make_unique<Dragon>();

    enemies.push_back(std::move(gob));
    enemies.push_back(std::move(dragg));

    for(const auto& enemy : enemies){
        enemy->attack();
        enemy->EnemyStatus();
    }

    enemies[0]->takeDamage(50);
    enemies[1]->takeDamage(200);

    enemies[0]->attack();
    enemies[0]->takeDamage(120);

    enemies[0]->attack();
    enemies[0]->takeDamage(120);

    int i = 0;

while (i < enemies.size())
{
    if (enemies[i]->isDefeated())
    {
       enemies.erase(std::next(enemies.begin(), i));

    }
    else
    {
        i++;
    }
}

}

