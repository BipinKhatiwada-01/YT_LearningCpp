//Very satisfying to finish cuz I wrote it on my own.

#include <iostream>

class Damageable {
    public:
    virtual void takeDamage(int d) = 0;

    virtual ~Damageable() = default;
};

class Enemy 
    : public Damageable {
        private:
        const char* name;
        int health;

        public:

        void EnemyStatus(){
            std::cout<< name << "'s health is " << health << "\n";
        }

        void takeDamage(int d) override {

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

        void giveName(){
            std::cout<< name << " is the name\n";
        }

        virtual void attack() = 0;

        Enemy() : name("Enemy"), health(100){}
        Enemy(const char* n, int val) : name(n), health(val){}

};

class Goblin
    : public Enemy {
        public:
        void attack() override {
            std::cout<< "Goblin attacked with a dagger\n";
        }

        Goblin() : Enemy("Goblin", 150) {}
        
};

class Dragon
    : public Enemy {
        public:

        void attack() override{
            std::cout<< "Dragon breathes fire at the player\n";
        }

        Dragon() : Enemy("Dragon", 300) {}
};

class Dummy 
    : public Damageable {
        public:
        void takeDamage(int d) override {
                if(d < 0 ){
                    std::cout<< "Invalid damage points\n";
                    return;
                }

                std::cout << "Dummy took a damage of " << d << "\n";
            
        }
};

void Damage(Damageable& dmag, int d){
    dmag.takeDamage(d);
}

void fight(Enemy& enemy){
    enemy.attack();
}



int main() {
  
    Goblin gob;
    Enemy& fun = gob;

    Dragon dragg;
    Enemy& drag = dragg;

    Dummy victim;
    Damageable& vic = victim;

    Damage(vic, 20);
    Damage(vic, 30);
    Damage(vic, 50);
    Damage(vic, 90);

    fight(drag);
    drag.EnemyStatus();
    drag.takeDamage(350);
    drag.EnemyStatus();
    drag.takeDamage(10);
    drag.EnemyStatus();

    fight(fun);
    fun.EnemyStatus();
    Damage(fun, 150);
    fun.EnemyStatus();
    fun.takeDamage(60);
    fun.EnemyStatus();
    fun.takeDamage(50);
    fun.EnemyStatus();

}