#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

enum ELevelTrajectory
{
    None,
    Rising,
    Falling
};

bool bCheckLevels(vector<int>& inLevels, int numSkips = 0, int startIdx = 0, int skipIdx = -1)
{
    int levelA = 0;
    int levelB = 0;
    bool bSafeLevels = true;
    int diff = 0;
    ELevelTrajectory levelTrajectory = None;
    ELevelTrajectory tempTrajectory = None;

    for (int i = startIdx; i < inLevels.size() - 1; ++i)
    {
        // this prevents us from testing i+1 against i+1
        if (i == skipIdx)
        {
            continue;
        }

        levelA = inLevels[i];
        levelB = inLevels[i + 1 == skipIdx ? i + 2 : i + 1];

        diff = abs(levelA - levelB);
        if (diff < 1 || diff > 3)
        {
            bSafeLevels = false;
        }

        tempTrajectory = levelA < levelB ? Rising : Falling;
        if (levelTrajectory != None && levelTrajectory != tempTrajectory)
        {
            bSafeLevels = false;
        }

        if (!bSafeLevels)
        {
            if (numSkips > 0)
            {
                return i == inLevels.size() - 2
                || bCheckLevels(inLevels, 0, 0, max(0, i - 1))
                || bCheckLevels(inLevels, 0, 0, i)
                || bCheckLevels(inLevels, 0, 0, i + 1);
            }
            break;
        }

        levelTrajectory = tempTrajectory;
    }

    return bSafeLevels;
}

int main ()
{
    ifstream data("input.txt");
    int safeCount = 0;
    string lineData;
    vector<int> levels = {66, 68, 70, 71, 72, 72, 75, 75};
    int sepIdx = 0;

    while(getline(data, lineData))
    {
        levels.clear();

        // pull level data into a vector array
        do
        {
            sepIdx = lineData.find(" ");
            levels.push_back(stoi(lineData.substr(0, sepIdx)));
            lineData.erase(0, sepIdx == string::npos ? sepIdx : sepIdx + 1);

        } while (lineData.size() > 0);

        // increment if the level was safe
        if (bCheckLevels(levels, 0))
            safeCount++;
    }

    cout << safeCount;

    return 0;
}