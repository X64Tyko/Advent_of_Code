#include <iostream>
#include <fstream>
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

    bool operator< (const Coord& other) const
    {
        return (X <= other.X && Y < other.Y) ||
            (X < other.X && Y <= other.Y);
    }

    std::size_t operator()(const Coord& c) const
    {
        std::size_t h1 = std::hash<int>{}(c.X);
        std::size_t h2 = std::hash<int>{}(c.Y);
        return h1 ^ (h2 << 1); // Combine hashes
    }
};

// recursively scan the 4 cardinal directions for the next height.
void ScanTrail(const std::vector<std::string>& grid, const int gridY, const int gridX, const int nextHeight, std::unordered_set<Coord, Coord>& trailends, int& rating)
{
    // We've found an end, update totals and get out
    if (nextHeight > '9')
    {
        ++rating;
        trailends.insert(Coord(gridY, gridX));
        //std::cout << "Found Trail End at: " << gridX << " : " << gridY << "\n";
        return;
    }

    // test up down left and right, we need to count how many find a finish
    if (gridY > 0 && grid[gridY - 1][gridX] == nextHeight)
        ScanTrail(grid, gridY - 1, gridX, nextHeight + 1, trailends, rating);
    if (gridY < grid.size() - 1 && grid[gridY + 1][gridX] == nextHeight)
        ScanTrail(grid, gridY + 1, gridX, nextHeight + 1, trailends, rating);
    if (gridX > 0 && grid[gridY][gridX - 1] == nextHeight)
        ScanTrail(grid, gridY, gridX - 1, nextHeight + 1, trailends, rating);
    if (gridX < grid[gridY].size() - 1 && grid[gridY][gridX + 1] == nextHeight)
        ScanTrail(grid, gridY, gridX + 1, nextHeight + 1, trailends, rating);
}

int main()
{
    std::vector<std::string> grid;
    std::string lineString;
    std::ifstream data("input.txt");

    while (getline(data, lineString))
    {
        grid.push_back(lineString);
    }
    data.close();

    // store each trailhead as a pair< unique trailends, total rating >
    std::vector<std::pair<std::unordered_set<Coord, Coord>, int>> trailheads;

    int gridX = 0;
    int gridY = 0;
    for (; gridY < grid.size(); ++gridY)
    {
        for (gridX = 0; gridX < grid[gridY].size(); ++gridX)
        {
            if (grid[gridY][gridX] == '0')
            {
                int rating = 0;
                std::unordered_set<Coord, Coord> trailends;
                //std::cout << "Scanning starting at: " << gridX << " : " << gridY << "\n";
                ScanTrail(grid, gridY, gridX, '1', trailends, rating);
                trailheads.push_back(std::make_pair(trailends, rating));
                //std::cout << "Trailhead Score: " << trailHead.size() << "\n";
            }
        }
    }

    //std::cout << "Trailheads Found: " << trailheads.size() << "\n";
    
    // Count the unique trails found
    int totalTrails = 0;
    int totalRating = 0;
    for (auto& uset : trailheads)
    {
        totalTrails += uset.first.size();
        totalRating += uset.second;
    }

    std::cout << "Found Trails: " << totalTrails << "\tTotal Rating: " << totalRating;

    return 0;
}