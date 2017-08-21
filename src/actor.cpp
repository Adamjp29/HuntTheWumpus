#include "actor.h"
void Player::move(){}
void Wumpus::move(){}
void Bat::move(){}
void Pit::move(){}
void Bat::move(int room){}
void Pit::move(int room){}

void Wumpus::move(int room){
    currentRoom = room;
}

void Player::move(int room){
    currentRoom = room;
}
