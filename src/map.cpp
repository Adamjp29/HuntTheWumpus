#include <random>
#include <ctime>
#include <iostream>
#include <vector>
#include <chrono>
#include "map.h"

using namespace std;

int totalIterCount = 0;
bool notFinished(std::vector<std::vector<int>> edgeListLocal)
{
    int totalConnections = 0;
    for (int i = 0; i < edgeListLocal.size(); i++)
        totalConnections += edgeListLocal.at(i).size();
    if (totalConnections == 60)
        return false;
    return true;
}

//testing function
void printDe(std::vector<std::vector<int>> edgeListLocal)
{
    for (int i = 0; i < edgeListLocal.size(); i++)
    {
        cout << "Room " << i << " is connected to: ";
        for (int j = 0; j < edgeListLocal.at(i).size(); j++)
        {
            if (j == edgeListLocal.at(i).size() - 1)
                cout << edgeListLocal.at(i)[j];
            else
                cout << edgeListLocal.at(i)[j] << "-";
        }
        cout << endl;
    }
}

std::vector<std::vector<int>> Map::initializeMap()
{
    int iterCount = 0;
    std::vector<std::vector<int>> edgeListLocal{20};
    std::vector<int> Room = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19};
    while (notFinished(edgeListLocal))
    {
        for (int i = 0; i < 20; i++)
        {
            int j = rand() % Room.size();
            if (edgeListLocal.at(i).size() < 3 && edgeListLocal.at(Room[j]).size() < 3 && Room[j] != i && find(edgeListLocal.at(i).begin(), edgeListLocal.at(i).end(), Room[j]) == edgeListLocal.at(i).end())
            {
                edgeListLocal.at(i).push_back(Room[j]);
                edgeListLocal.at(Room[j]).push_back(i);
                Room.erase(Room.begin() + i);
            }
        }
        iterCount++;
        if (iterCount > 15)
        {
            return edgeListLocal;
        }
    }
    
    this->validMap = true;
    return edgeListLocal;
}
