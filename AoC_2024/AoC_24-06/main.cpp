#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <set>

// Plan: Pull map into memory using a 2D array to represent the map.
// Store guard location as X,Y coords.
// Given the guards facing check the spot in front of them, then either rotate or move.
// each space the guard moves is stored in a set of pairs so that each coord is only counted once.

 enum Direction {
    up = 0,
    right = 1,
    down = 2,
    left = 3,
    MAX = 4
};

struct TravelPoint
{
public:
    char travelDirection;
    int X;
    int Y;

    bool operator== (const TravelPoint& other) const
    {
        return travelDirection == other.travelDirection &&
            X == other.X &&
            Y == other.Y;
    }

    bool operator< (const TravelPoint& other) const
    {
        return X != other.X ? X < other.X :
            Y != other.Y ? Y < other.Y :
                travelDirection < other.travelDirection;
    }

    bool operator> (const TravelPoint& other) const
    {
        return X != other.X ? X > other.X :
            Y != other.Y ? Y > other.Y :
                travelDirection > other.travelDirection;
    }

    TravelPoint(char direction, int x, int y)
        : travelDirection(direction),
        X(x),
        Y(y)
    {}
};

class Guard
{
    TravelPoint currentPoint;

    std::vector<TravelPoint> traveledPoints;

public:
    Guard() 
        : currentPoint(Direction::up, -1, -1)
    {}

    char getDirection() const { return currentPoint.travelDirection; }
    int GetX() const { return currentPoint.X; }
    int GetY() const { return currentPoint.Y; }

    void SetX(int newX) { currentPoint.X = newX; }
    void SetY(int newY) { currentPoint.Y = newY; }

    int GetNumTraveledPoints() const { return traveledPoints.size();  }

    std::pair<int, int> GetNextCoord() const
    {
        switch (currentPoint.travelDirection)
        {
            case Direction::up:
                return std::make_pair(currentPoint.X, currentPoint.Y - 1);
                break;
            
            case Direction::right:
                return std::make_pair(currentPoint.X + 1, currentPoint.Y);
                break;
            
            case Direction::down:
                return std::make_pair(currentPoint.X, currentPoint.Y + 1);
                break;
            
            case Direction::left:
                return std::make_pair(currentPoint.X - 1, currentPoint.Y);
                break;
        }
    }

    void DoMove(const char& nextTile, std::pair<int, int> nextCoord)
    {
        if (nextTile == '#')
        {
            currentPoint.travelDirection = (currentPoint.travelDirection + 1) % Direction::MAX;
        }
        else
        {
            traveledPoints.push_back(currentPoint);
            currentPoint.X = nextCoord.first;
            currentPoint.Y = nextCoord.second;
        }
    }

    bool HasTraveledPoint() const 
    {
        for (auto& prevPoint : traveledPoints)
        {
            if (prevPoint == currentPoint)
                return true;
        }

        return false;
    }
};

/// @brief use a copy of the guard to check if we can find a loop after "pretending" there was a blocker in front of us
/// @param grid Copy of the grid so we can add in the new blocker
/// @param guard a copy of the guard so that we can keep previous state
/// @return true if we find a loop
bool findLoop(std::vector<std::string> grid, Guard guard, std::pair<int, int> nextCoord)
{
    grid[nextCoord.second][nextCoord.first] = '#';

    while ( nextCoord.first >= 0 && nextCoord.first < grid[0].size() && nextCoord.second >= 0 && nextCoord.second < grid.size())
    {
        guard.DoMove(grid[nextCoord.second][nextCoord.first], nextCoord);
        if (guard.HasTraveledPoint())
            return true;
        nextCoord = guard.GetNextCoord();
    }

    return false;
}

int main()
{
    std::vector<std::string> grid;
    std::string streamString;
    std::ifstream data("input.txt");
    std::set<std::pair<int, int>> traveledLocations;
    std::set<std::pair<int, int>> loopPoints;

    Guard guard;
    int gridHeight = 0;
    int gridWidth = 0;
    
    while (std::getline(data, streamString))
    {
        grid.push_back(streamString);
        if (guard.GetX() == -1)
        {
            guard.SetY(gridHeight);
            guard.SetX(streamString.find('^'));
        }
        
        ++gridHeight;
        gridWidth = streamString.size();
    }

    std::cout << "Grid Size: " << gridWidth << " : " << gridHeight << "\n";

    std::cout << "guard at: " << guard.GetX() << " : " << guard.GetY() << "\n";

    traveledLocations.insert(std::make_pair(guard.GetX(), guard.GetY()));

    std::pair<int, int> nextCoord = guard.GetNextCoord();
    while ( nextCoord.first >= 0 && nextCoord.first < grid[0].size() && nextCoord.second >= 0 && nextCoord.second < grid.size())
    {
        // try to find if we can loop by putting a blocker in front of the guard
        if (grid[nextCoord.second][nextCoord.first] != '#')
        {
            bool bValid = true;
            for (auto& point : traveledLocations)
            {
                // if we've already traveled to this point we can't use it as a blocker
                if (point == nextCoord)
                {
                    bValid = false;
                    break;
                }
            }

            if (bValid && findLoop(grid, guard, nextCoord))
                loopPoints.insert(nextCoord);
        }

        // continue moving
        guard.DoMove(grid[nextCoord.second][nextCoord.first], nextCoord);
        traveledLocations.insert(std::make_pair(guard.GetX(), guard.GetY()));
        nextCoord = guard.GetNextCoord();
    }

    std::cout <<"Traveled points: " << traveledLocations.size() << " Possible Loops: " << loopPoints.size();
    return 0;
}