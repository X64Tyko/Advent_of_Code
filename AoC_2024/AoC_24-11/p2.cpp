#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>

typedef unsigned long long ull;
#define BLINKS 75

int main()
{
    std::string inputLine;
    std::ifstream data("input.txt");
    std::map<ull, ull> map1;
    std::map<ull, ull> map2;
    
    std::regex number("\\d+");
    while (std::getline(data, inputLine))
    {
        std::sregex_iterator& start = std::sregex_iterator(inputLine.begin(), inputLine.end(), number);
        do
        {
            map1[std::stoull((*start).str())] = 1;
        } while (++start != std::sregex_iterator());
    }

    std::map<ull, ull>* currentMap = &map1;
    std::map<ull, ull>* newMap = &map2;
    std::map<ull, ull>* tempMap;
    std::string tempValString;
    ull tempVal = 0;

    for (int i = 0; i < BLINKS; ++i)
    {
        (*newMap).clear();
        for (auto& pair : *currentMap)
        {
            tempValString = std::to_string(pair.first);
            if (pair.first == 0)
            {
                (*newMap)[1] = pair.second;
            }
            else if (tempValString.size() & 0x1)
            {
                (*newMap)[pair.first * 2024] = pair.second;
            }
            else
            {
                tempVal = std::stoull(tempValString.substr(0, tempValString.size() / 2));
                (*newMap)[tempVal] += pair.second;
                tempVal = std::stoull(tempValString.substr(tempValString.size() / 2));
                (*newMap)[tempVal] += pair.second;
            }
        }
        tempMap = currentMap;
        currentMap = newMap;
        newMap = tempMap;
    }

    ull TotalStones = 0;
    for (auto& pair : *currentMap)
    {
        TotalStones += pair.second;
    }

    std::cout << TotalStones;
    return 0;
}