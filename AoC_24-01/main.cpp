#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;

int main()
{
    string inputLine;
    map<int, pair<int, int>> similarityMap;
    map<int, pair<int, int>>::iterator pairIter;

    ifstream input("input.txt");

    if (input.is_open())
    {
        while (getline(input, inputLine))
        {
            int sepIdx = inputLine.find(" ");
            int idA = stoi(inputLine.substr(0, sepIdx));
            similarityMap.try_emplace(idA, make_pair(0, 0));
            pairIter = similarityMap.find(idA);
            (*pairIter).second.first++;

            
            int idB = stoi(inputLine.erase(0, sepIdx + 3));
            similarityMap.try_emplace(idB, make_pair(0, 0));
            pairIter = similarityMap.find(idB);
            (*pairIter).second.second++;
        }
    }

    unsigned long long tally = 0;
    pairIter = similarityMap.begin();
    while (pairIter != similarityMap.end())
    {
        tally += (*pairIter).first * (*pairIter).second.first * (*pairIter).second.second;
        ++pairIter;
        //cout << tally << endl;
    }

    cout << tally;

    return 0;
}
