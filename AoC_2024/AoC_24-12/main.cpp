#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>

struct AreaNode
{
    AreaNode(int inIdx)
    : nodeIdx(inIdx)
    , corners(0)
    {}

    int nodeIdx;
    int corners;
    int connections[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

    // calculating corners, or vertices, for the shape directly correlates to the number of sides.
    // we can calculate each section of a shape to see if it contains any interior or exterior corners.
    void CalculateCorners()
    {
        for (int c = 0; c < 4; ++c)
        {
            if (connections[c] == -1 && connections[(c + 1) % 4] == -1)
                ++corners;

            if (connections[c] >= 0 && connections[(c + 1) % 4] >= 0 && connections[c + 4] == -1)
                ++corners;
        }
    }

    bool operator< (const AreaNode& other) const
    {
        return nodeIdx < other.nodeIdx;
    }
};

struct Area
{
    Area(char inID)
    : id(inID)
    , count(0)
    , fences(0)
    , corners(0)
    {}

    char id;
    int count;
    int fences;
    int corners;

    void AddCorners(const AreaNode& inNode)
    {
        corners += inNode.corners;
    }
};

int main()
{
    using namespace std;

    string inputLine;
    ifstream data("input.txt");
    string dataArray;
    int width = 0;
    int height = 0;

    while (getline(data, inputLine))
    {
        width = inputLine.size();
        ++height;

        dataArray.append(inputLine);
    }

    data.close();

    // This will keep us from traversing the same sections over and over
    int totalSize = dataArray.size();
    bool* traversedIndices = new bool[totalSize];
    for (int i = totalSize - 1; i >= 0; --i)
        traversedIndices[i] = false;

    queue<int> nextAreaNodeQueue;
    vector<Area> uniqueAreas;
    AreaNode newNode(0);
    int currentIdx = 0;
    int areaIdx = 0;
    int tempIdx = 0;
    int col = 0;

    // we cardinal directions in order then diagonals in order, this is for calculating corners.
    int tests[8] = {-width, 1, width, -1, -width + 1, width + 1, width - 1, -width - 1};

    while (currentIdx < totalSize)
    {
        Area newArea = Area(dataArray[currentIdx]);
        traversedIndices[currentIdx] = true;
        nextAreaNodeQueue.push(currentIdx);
        do
        {
            // check up down left and right for similar areas
            areaIdx = nextAreaNodeQueue.front();
            nextAreaNodeQueue.pop();

            newNode = AreaNode(areaIdx);
            col = areaIdx % width;
            ++newArea.count;
            newArea.fences += 4;

            for (int i = 0; i < 8; ++i)
            {
                tempIdx = areaIdx + tests[i];

                // no wrapping
                if (abs((tempIdx % width) - col) > 1)
                    continue;

                if (tempIdx >= 0 && tempIdx < totalSize && dataArray[tempIdx] == newArea.id)
                {
                    newNode.connections[i] = tempIdx;
                    if (i < 4)
                    {
                        --newArea.fences;
                        if (traversedIndices[tempIdx] == false)
                        {
                            nextAreaNodeQueue.push(tempIdx);
                            traversedIndices[tempIdx] = true;
                        }
                    }
                }
            }
            newNode.CalculateCorners();
            newArea.AddCorners(newNode);

        } while (nextAreaNodeQueue.size() > 0);

        uniqueAreas.push_back(newArea);
        // keep iterating until we get to the next starting point for an area.
        while (traversedIndices[++currentIdx] == true) {}
    }

    delete[] traversedIndices;

    long long totalCost = 0;
    long long discountCost = 0;
    for (auto& area : uniqueAreas)
    {
        totalCost += area.count * area.fences;
        discountCost += area.count * area.corners;
    }

    cout << "total cost: " << totalCost << "\ndiscount: " << discountCost;
    return 0;
}