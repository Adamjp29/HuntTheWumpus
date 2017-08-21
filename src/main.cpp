#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include "map.h"
#include "actor.h"

using namespace std;
Player *p;
Map m;
bool gameOver = false;

std::vector<Actor*> actors;
std::vector<std::vector<int>> edgeList;

void readInput();

void printDebug(std::vector<std::vector<int>>,std::vector<Actor*>); 
void printAdjacentRooms(int currentRoom);
void printEnvrionment(vector<int> adjRooms);

vector<int> getAdjacentRooms (int currentRoom);

void checkOldSafe(int oldRoom);
void checkNewSafe(int newRoom);
bool checkValidShoot( int curRoom, int r1, int r2, int r3);
bool checkValidMove(int curRoom, int r1);
bool wumpUpdateArrow(int room);

std::vector<Actor*> initializeActors (Map& m);

enum{wumpusIndex, b1Index, b2Index, p1Index, p2Index, playerIndex};

int main()
{
    srand(time(NULL));
    while(m.validMap == false){
        edgeList = m.initializeMap();
    }
    actors = initializeActors(m);
    cout << "Type a command of the form m# to move, s#-#-# to shoot, or debug to print game information." << endl;
    cout << "You have begun in room " << p->currentRoom << endl;
    vector<int>adjRooms = getAdjacentRooms(p->currentRoom);
    printAdjacentRooms(p->currentRoom);
    printEnvrionment(adjRooms);

    while (p->remainingArrows > 0 && actors[wumpusIndex]->getStatus() == 1 && actors[playerIndex]->getStatus() == 1)	
    {
        readInput();
    }
}

void readInput()
{
    int r1, r2, r3;

    bool bothAlive = true;
    std::string command;
    while (bothAlive && cin >> command)
    {
        int hyp1Index = command.find('-');
        int hyp2Index = command.find('-',hyp1Index+1);
        int hyp3Index = command.find('-',hyp2Index+1);
        //does not get the case where letters are entered after 5th position 
        //user should follow instructions better
        if (command[0]== 's' 
            && (hyp1Index == 2 || hyp1Index == 3)
            && (hyp2Index - hyp1Index == 2 || hyp2Index - hyp1Index == 3)
            && hyp3Index == -1 
            && isdigit(command[1])
            && (isdigit(command[2]) || command[2] == '-')
            && (isdigit(command[3]) || command[3] == '-')
            && (isdigit(command[4]) || command[4] == '-')
            && (isdigit(command[5]) || command[5] == '-'))
        {
            if (hyp1Index == 2){
                string roomString = command.substr(1,1);
                r1 = atoi(roomString.c_str());
            }
            if (hyp1Index == 3 ){
                string roomString = command.substr(1,2);
                r1 = atoi(roomString.c_str());
            }
            //2nd room is 1 digit long
            if (hyp2Index - hyp1Index == 2){
                string roomString = command.substr(hyp1Index+1,1);
                r2 = atoi(roomString.c_str());
            }
            //2nd room is 2 digits long
            if (hyp2Index - hyp1Index == 3){
                string roomString = command.substr(hyp1Index+1,2);
                r2 = atoi(roomString.c_str());
            }
            //last room is 1 digit
            if (command.size() - hyp2Index == 2){
                string roomString = command.substr(hyp2Index+1,1);
                r3 = atoi(roomString.c_str());
            }
            //last room is 2 digits
            if (command.size() - hyp2Index == 3){
                string roomString = command.substr(hyp2Index+1,2);
                r3 = atoi(roomString.c_str());
            }
        

            if(checkValidShoot(p->currentRoom, r1, r2, r3)){
                
                int wumpMoveChance = rand() % 100;
                if (wumpMoveChance < 3){
                    int oldRoom = actors[wumpusIndex]->currentRoom;
                    int newRoom = getAdjacentRooms(actors[wumpusIndex]->currentRoom)[rand()%3];
                    checkOldSafe(oldRoom); 
                    checkNewSafe(newRoom);
                    actors[wumpusIndex]->move(newRoom);
            
                    if (p->currentRoom == actors[wumpusIndex]->currentRoom ){
                        cout << "The wumpus has entered your room and you have died" << endl;
                        p->status = 0;
                        bothAlive = false;
                        break;
                    }
                }
                bool bAlive = true;
                p->arrowLocation = r1;
                bAlive = wumpUpdateArrow(r1);
                if (bAlive == false){
                    bothAlive = false;
                    p->status = 0;
                    break;
                }

                p->arrowLocation = r2;

                bAlive = wumpUpdateArrow(r2);
                if (bAlive == false){
                    
                    bothAlive = false;
                    p->status=0;
                    break;
                }
                p->arrowLocation = r3;
                bAlive = wumpUpdateArrow(r3);
                if (bAlive == false){
		    p->status=0;
                    bothAlive = false;
                    break;
                }
            
                p->remainingArrows-=1;
                cout << "You did not hit the wumpus and have " << p->remainingArrows << " arrows remaining" << endl;
                if(p->remainingArrows == 0){
                    cout << "You have run out of arrows and lost the game" << endl;
                    bothAlive = false;
                    break;
                }
            }
            else {
                cout << "That is not a valid sequence of rooms" << endl;
            }
        }
        else if (command[0] == 'm' && isdigit(command[1]))
        {
            if (isdigit(command[2])){
                string roomString = command.substr(1,2);
                r1 = atoi(roomString.c_str());
            }
            else{
                string roomString = command.substr(1,1);
                r1 = atoi(roomString.c_str());
            }
            if (checkValidMove(p->currentRoom, r1)){
                int wumpMoveChance = rand() % 100;
                if (wumpMoveChance < 3){
                    //Put this code in the adj checks
                    int oldRoom = actors[wumpusIndex]->currentRoom;
                    int newRoom = getAdjacentRooms(actors[wumpusIndex]->currentRoom)[rand()%3];
                    checkOldSafe(oldRoom); 
                    checkNewSafe(newRoom);
                    actors[wumpusIndex]->move(newRoom);
            
                    if (p->currentRoom == actors[wumpusIndex]->currentRoom ){
                        cout << "The wumpus has entered your room and you have died" << endl;
                        p->status = 0;
                        bothAlive = false;
                        break;
                    }
                }
                p->move(r1);
                if (p->currentRoom == actors[wumpusIndex]->currentRoom ){
                    cout << "You entered a room with the wumpus and have died." << endl;
                    p->status = 0;
                    bothAlive = false;
                    break;
                }
                if (p->currentRoom == actors[b1Index]->currentRoom || p->currentRoom == actors[b2Index]->currentRoom){
                    cout << "You entered a room with a bat" << endl;
                    p->currentRoom = m.safeRooms[rand() % m.safeRooms.size()];
                }
                else{
                    if (p->currentRoom == actors[p1Index]->currentRoom || p->currentRoom == actors[p2Index]->currentRoom){
                        cout << "You entered a room with a pit" << endl;
                        int pitDeath = rand() % 100;
                        if (pitDeath < 40){
                            cout << "You fall to your death" << endl;
                            p->status = 0;
                            bothAlive = false;
                            break;
                        }
                        else{
                            cout << "But you managed to avoid it" << endl;
                        }
                    }
                }
                cout << "You are now in room " << p->currentRoom << endl;
                vector<int>adjRooms = getAdjacentRooms(p->currentRoom);
                printAdjacentRooms(p->currentRoom);
                printEnvrionment(adjRooms);
                
            }
            else{
                cout << "That is not a valid room to move to" << endl;
            }
                
        }
        else if (command == "debug")
        {
            printDebug(edgeList,actors);
            
        }
        else
        {
            cout << "Invalid command \n";
            
        }  
    }
    return;
}

bool checkValidShoot(int curRoom, int r1, int r2, int r3)
{
     if (find(edgeList.at(curRoom).begin(), edgeList.at(curRoom).end(), r1) != edgeList.at(curRoom).end()){
          if (find(edgeList.at(r1).begin(), edgeList.at(r1).end(), r2) != edgeList.at(r1).end()){
               if (find(edgeList.at(r2).begin(), edgeList.at(r2).end(), r3) != edgeList.at(r2).end()){
                   return true;
               }
          }
     }
     return false;
}
bool checkValidMove(int curRoom, int r1)
{
    if (find(edgeList.at(curRoom).begin(), edgeList.at(curRoom).end(), r1) != edgeList.at(curRoom).end())
         return true;
     else
         return false;
}
std::vector<Actor*> initializeActors(Map& m)
{
    int wumpRoom = m.safeRooms[rand() % m.safeRooms.size()];
    auto it = std::find(m.safeRooms.begin(),m.safeRooms.end(),wumpRoom);
    m.safeRooms.erase(it);
    Wumpus *w = new Wumpus(wumpRoom);
    actors.push_back(w);

    int bat1Room = m.safeRooms[rand() % m.safeRooms.size()];
    it = std::find(m.safeRooms.begin(),m.safeRooms.end(),bat1Room);
    m.safeRooms.erase(it);
    Bat *b1 = new Bat(bat1Room);
    actors.push_back(b1);

    int bat2Room = m.safeRooms[rand() % m.safeRooms.size()];
    it = std::find(m.safeRooms.begin(),m.safeRooms.end(),bat2Room);
    m.safeRooms.erase(it);
    Bat *b2 = new Bat(bat2Room);
    actors.push_back(b2);

    int pit1Room = m.safeRooms[rand() % m.safeRooms.size()];
    it = std::find(m.safeRooms.begin(),m.safeRooms.end(),pit1Room);
    m.safeRooms.erase(it);
    Pit *p1 = new Pit(pit1Room);
    actors.push_back(p1);

    int pit2Room = m.safeRooms[rand() % m.safeRooms.size()];
    it = std::find(m.safeRooms.begin(),m.safeRooms.end(),pit2Room);
    m.safeRooms.erase(it);
    Pit *p2 = new Pit(pit2Room);
    actors.push_back(p2);

    int pRoom = m.safeRooms[rand() % m.safeRooms.size()];
    p = new Player(pRoom);  
    actors.push_back(p);
    return actors;
}

void printDebug(std::vector<std::vector<int>> edgeList, std::vector<Actor*> actors)
{
    for (int i = 0; i < edgeList.size(); i++)
    {
        cout << "Room " << i << " is connected to: ";
        for (int j = 0; j < edgeList.at(i).size(); j++)
        {
            if (j == edgeList.at(i).size() - 1)
                cout << edgeList.at(i).at(j);
            else
                cout << edgeList.at(i).at(j) << "-";
        }
        cout << endl;
    }
    cout << "You have " << p->remainingArrows << " arrows remaining" << endl;
    cout << "Room " << actors.at(wumpusIndex)->currentRoom << " contains the wumpus" << endl;
    cout << "Room " << actors.at(b1Index)->currentRoom << " contains a bat" << endl;
    cout << "Room " << actors.at(b2Index)->currentRoom << " contains a bat" << endl;
    cout << "Room " << actors.at(p1Index)->currentRoom << " contains a pit" << endl;
    cout << "Room " << actors.at(p2Index)->currentRoom << " contains a pit" << endl;
    cout << "Room " << actors.at(playerIndex)->currentRoom << " contains the player" << endl;
}
vector <int> getAdjacentRooms (int currentRoom){
    vector<int> adjRooms;
    for (int i = 0; i<edgeList.at(currentRoom).size(); i++){
        adjRooms.push_back(edgeList.at(currentRoom).at(i));
    }
    return adjRooms;
}

void printAdjacentRooms (int currentRoom){
    vector<int> adjRooms;
    for (int i = 0; i<edgeList.at(currentRoom).size(); i++){
        adjRooms.push_back(edgeList.at(currentRoom).at(i));
    }
    cout << "The adjacent rooms are: ";
    for (auto e : adjRooms)
        cout << e << " ";
    cout << endl;
}

void printEnvrionment(vector<int> adjRooms){
    int batAlreadyPrinted=0;
    int pitAlreadyPrinted=0;
    for (int i = 0; i < actors.size()-1; i++){
        if (find(adjRooms.begin(), adjRooms.end(), actors[i]->currentRoom) != adjRooms.end()){
	    if (i == 1){
		batAlreadyPrinted = 1;
	    }
	    if (i == 3){
		pitAlreadyPrinted = 1;
	    }
	    if (i == 0 || i == 1 || (i == b2Index && batAlreadyPrinted == 0) || i == 3 || (i == p2Index && pitAlreadyPrinted==0)){
            cout << actors[i]->getMessage() << endl;
            }
	}
     }
}

void checkNewSafe(int newRoom){
    if(   newRoom == actors[b1Index]->currentRoom 
        ||newRoom == actors[b2Index]->currentRoom
        ||newRoom == actors[p1Index]->currentRoom
        ||newRoom == actors[p2Index]->currentRoom){}
    else{
            auto it = std::find(m.safeRooms.begin(),m.safeRooms.end(),newRoom);
            m.safeRooms.erase(it);
    }
}

void checkOldSafe(int oldRoom){
    if(   oldRoom == actors[b1Index]->currentRoom 
        ||oldRoom == actors[b2Index]->currentRoom
        ||oldRoom == actors[p1Index]->currentRoom
        ||oldRoom == actors[p2Index]->currentRoom){}
    else{
        m.safeRooms.push_back(actors[wumpusIndex]->currentRoom);
    }
}

bool wumpUpdateArrow(int room){
    vector<int> adjToWump = getAdjacentRooms(actors[wumpusIndex]->currentRoom);
    if ( find(adjToWump.begin(),adjToWump.end(),room) != adjToWump.end() ){
         int wumpMoveChance = rand() % 100;
         if (wumpMoveChance < 20){
             int oldRoom = actors[wumpusIndex]->currentRoom;
             int newRoom = getAdjacentRooms(actors[wumpusIndex]->currentRoom)[rand()%3];
             checkOldSafe(oldRoom); 
             checkNewSafe(newRoom);
             actors[wumpusIndex]->move(newRoom);
                    
            if (p->currentRoom == actors[wumpusIndex]->currentRoom ){
                cout << "The wumpus has entered your room and you have died" << endl;
                p->status = 0;
                return false;
            }
        }                  
    }
    if (actors[wumpusIndex]->currentRoom == room){
        cout << "You have shot the wumpus and won the game" << endl;
        return false;
    }
    return true;
}
