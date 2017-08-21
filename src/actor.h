#ifndef ACTOR_H
#define ACTOR_H

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
struct Actor{
    int currentRoom; 
    virtual void move()=0;
    virtual void move(int room)=0;
    virtual std::string getMessage(){}
    virtual int getStatus(){}
    Actor(){}
    Actor(int startRoom): currentRoom{startRoom} {}
};

struct Bat : Actor
{

    std::string getMessage(){
        std::string message = "I hear a bat";
        return message;
    }
    void move();
    void move(int);
    Bat(int startRoom): Actor(startRoom) {}
};

struct Wumpus : public Actor
{
private:
    int status=1;

public:
    std::string getMessage(){
        std::string message = "I smell the wumpus";
        return message;
    }

    int getStatus(){
        return status;
    }
    Wumpus(int startRoom): Actor(startRoom) {} 
    void move();
    void move(int);
};
struct Pit :  Actor
{

    std::string getMessage(){
        std::string message = "I feel a breeze";
        return message;
    }
    Pit(int startRoom): Actor(startRoom) {}
    void move();
    void move(int);
};

struct Player :  Actor
{
    int remainingArrows=5;
    int status=1;
    int arrowLocation;

    std::string getMessage(){
        std::string message = "";
        return message;
    }

    int getStatus(){
        return status;
    }

    Player(int startRoom): Actor(startRoom) {}
    void move();
    void move(int room);
};

#endif
