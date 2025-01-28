#include <iostream>
#include <fstream>
#include <string>
#include <regex>

int main()
{
    std::string inputLine;
    std::ifstream data("input.txt");
    std::string stones;
    std::string newStones;

    while (std::getline(data, inputLine))
    {
        stones.append(inputLine);
    }

    std::regex number("\\d+");
    int numBlinks = 50;
    unsigned long long tempval = 0;
    std::sregex_iterator& start = std::sregex_iterator(stones.begin(), stones.end(), number);
    for(; numBlinks > 0; --numBlinks)
    {
        start = std::sregex_iterator(stones.begin(), stones.end(), number);
        do
        {
            std::string& stone = (*start).str();
            if (stone == "0")
            {
                stone = "1";
            }
            else if (stone.size() & 0x1)
            {
                tempval = std::stoull(stone) * 2024;
                stone = std::to_string(tempval);
            }
            else
            {
                std::string tempStone = std::to_string(std::stoull(stone.substr(0, stone.size() / 2))) + " ";
                tempStone.append(std::to_string(std::stoull(stone.substr(stone.size() / 2))));
                stone = tempStone;
            }
            newStones.append(stone + " ");
        } while(++start != std::sregex_iterator());
        stones = newStones;
        newStones.clear();
    }

    start = std::sregex_iterator(stones.begin(), stones.end(), number);
    std::cout << "\nNow : " << std::distance(start, std::sregex_iterator()) << " stones";
    return 0;
}