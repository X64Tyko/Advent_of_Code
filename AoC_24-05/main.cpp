#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <vector>

int main ()
{
    std::ifstream data("input.txt");
    std::map<int, std::vector<int>> ruleMap;
    std::vector<std::vector<int>> updates;
    int totalFixed = 0;
    int runningTotal = 0;

    std::string tempString;
    bool bImportingRules = true;
    std::regex valRegex("\\d+");
    while(getline(data, tempString))
    {
        if (tempString == "")
        {
            bImportingRules = false;
            continue;
        }

        std::sregex_iterator valIter = std::sregex_iterator(tempString.begin(), tempString.end(), valRegex);
        if (bImportingRules)
        {
            int k = std::stoi((*valIter).str());
            ruleMap[k].push_back(std::stoi((*(++valIter)).str()));
        }
        else
        {
            std::vector<int> tempVec;
            while (valIter != std::sregex_iterator())
            {
                tempVec.push_back(std::stoi((*valIter).str()));
                ++valIter;
            }

            updates.push_back(tempVec);
        }
    }

    std::vector<std::vector<int>>::iterator updateIter = updates.begin();
    std::vector<int>::iterator workingIter;
    std::vector<int>::const_iterator testIter;
    std::vector<int>::const_iterator ruleIter;

    bool bUpdateValid = true;
    bool bFoundBroken = false;
    while (updateIter < updates.end())
    {
        bUpdateValid = true;
        bFoundBroken = false;

        // iterate each update, grab the dictionary entry for each element and test all previous elements for matches
        std::vector<int>& workingUpdate = *updateIter;
        workingIter = workingUpdate.begin() + 1; // we can skip the first element, it's before everything
        while (workingIter < workingUpdate.end())
        {
            testIter = workingUpdate.begin();
            while (testIter != workingIter)
            {
                ruleIter = ruleMap[*workingIter].begin();
                while (ruleIter != ruleMap[*workingIter].end())
                {
                    if (*testIter == *ruleIter)
                    {
                        bUpdateValid = false;
                        bFoundBroken = true;
                        int tempVal =*workingIter;
                        workingUpdate.erase(workingIter);
                        workingUpdate.insert(testIter, tempVal);
                        break;
                    }
                    ++ruleIter;
                }

                if (!bUpdateValid)
                    break;
                ++testIter;
            }

            if (!bUpdateValid)
            {
                // reset and test again after swap.
                bUpdateValid = true;
                workingIter = workingUpdate.begin() + 1;
                continue;
            }
            ++workingIter;
        }

        if (bUpdateValid)
        {
            int center = (workingUpdate.size() / 2);
            if (bFoundBroken)
                totalFixed += workingUpdate[center];
            else   
                runningTotal += workingUpdate[center];
        }

        ++updateIter;
    }

    std::cout << " total in order: " << runningTotal << " : total fixed: " << totalFixed;

    return 0;
}