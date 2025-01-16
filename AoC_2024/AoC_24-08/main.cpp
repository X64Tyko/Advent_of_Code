#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <unordered_set>

struct Coord
{
    int X;
    int Y;

    Coord()
    : X(-1)
    , Y(-1)
    {}

    Coord(int x, int y)
    : X(x)
    , Y(y)
    {}

    Coord operator+ (const Coord& other) const
    {
        return Coord(X + other.X, Y + other.Y);
    }

    Coord operator- (const Coord& other) const
    {
        return Coord(X - other.X, Y - other.Y);
    }

    bool operator== (const Coord& other) const
    {
        return X == other.X && Y == other.Y;
    }

    std::size_t operator()(const Coord& c) const {
            std::size_t h1 = std::hash<int>{}(c.X);
            std::size_t h2 = std::hash<int>{}(c.Y);
            return h1 ^ (h2 << 1); // Combine hashes
        }
};

int main()
{
    std::map<char, std::vector<Coord>> antannae;
    std::unordered_set<Coord, Coord> antinodes;
    std::unordered_set<Coord, Coord> resonantAntinodes;
    std::string lineString;
    std::ifstream data("input.txt");

    int X = 0;
    int Y = 0;
    char charAtPos = 0;

    // Map all the known antannae by frequency
    while(std::getline(data, lineString))
    {
        X = 0;
        std::string::iterator charIter = lineString.begin();

        while (charIter != lineString.end())
        {
            charAtPos = *charIter;
            if ((charAtPos >= '0' && charAtPos <= '9') ||
                (charAtPos >= 'A' && charAtPos <= 'Z') ||
                (charAtPos >= 'a' && charAtPos <= 'z'))
            {
                antannae[charAtPos].push_back(Coord(X, Y));
            }

            ++charIter;
            ++X;
        }

        ++Y;
    }

    // at this point X and Y give us our width and height respectively so we can use them for bounds checks
    std::vector<Coord>::const_iterator coordIter;
    std::vector<Coord>::const_iterator coordCompIter;
    Coord dist;
    Coord possibleAntinode;

    int loopCount = 0;

    for (auto& frequencyList : antannae)
    {
        coordIter = frequencyList.second.begin();

        while (coordIter != frequencyList.second.end())
        {
            coordCompIter = coordIter + 1;
            while (coordCompIter != frequencyList.second.end())
            {
                dist = Coord((*coordIter).X - (*coordCompIter).X, (*coordIter).Y - (*coordCompIter).Y);
                possibleAntinode = *coordCompIter;
                loopCount = 0;
                while (possibleAntinode.X >= 0 && possibleAntinode.Y >= 0 && possibleAntinode.X < X && possibleAntinode.Y < Y)
                {
                    resonantAntinodes.insert(possibleAntinode);
                    possibleAntinode = possibleAntinode - dist;
                    
                    // This will add our antinode from part 1 to it's list
                    if (loopCount++ == 1)
                    {
                        antinodes.insert(possibleAntinode);
                    }
                }

                possibleAntinode = *coordIter;
                loopCount = 0;
                while (possibleAntinode.X >= 0 && possibleAntinode.Y >= 0 && possibleAntinode.X < X && possibleAntinode.Y < Y)
                {
                    resonantAntinodes.insert(possibleAntinode);
                    possibleAntinode = possibleAntinode + dist;
                    
                    // This will add our antinode from part 1 to it's list
                    if (loopCount++ == 1)
                    {
                        antinodes.insert(possibleAntinode);
                    }
                }
                ++coordCompIter;
            }
            ++coordIter;
        }
    }

    std::cout << "Found " << antinodes.size() << " Antinodes and " << resonantAntinodes.size() << " Resonant Antinodes";

    return 0;
}