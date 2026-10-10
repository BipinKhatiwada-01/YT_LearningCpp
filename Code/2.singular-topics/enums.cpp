#include <iostream>

enum class PlayerState {
    Idle,
    Walking,
    Jumping
};

int main() {

    PlayerState john = PlayerState::Walking;

    switch (john) {

        case PlayerState::Idle:
            std::cout<<"You might want to work on smoe new ideas mate\n";
                break;

        case PlayerState::Walking:
            std::cout<<"Who's gonna carry the boats\n";
                break;

        case PlayerState::Jumping:
            std::cout<<"Some kangaroo stuff\n";
                break;

        default:
            std::cout<<"Some undefined stuff\n";
                break;
    }
}