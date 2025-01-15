#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <regex>

/// @brief recursive function to test our +, *, and || operationsbetween each value pair
/// @param testVal the target value, passed through so we can test the total value at the end of the operation chain
/// @param opIter the iterator to the next value to do an operation with
/// @param end the end of our list of operation values
/// @param runningTotal the recursive total so far.
/// @return true if a set of operations totaled the testVal
bool testOperations(const unsigned long long& testVal, std::vector<int>::const_iterator& opIter, std::vector<int>::const_iterator& end, const unsigned long long& runningTotal = 0)
{
    // We've reached the end of our chain, does the operations performed match the testVal?
    if (opIter == end)
    {
        //std::cout << "finished with val: " << runningTotal << " target: " << testVal << "\n\n";
        return testVal == runningTotal;
    }

    //std::cout << "running total: " << runningTotal << " next val: " << *startVal << "\n";

    return testOperations(testVal, opIter + 1, end, runningTotal + *opIter) || testOperations(testVal, opIter + 1, end, runningTotal * *opIter) || testOperations(testVal, opIter + 1, end, std::stoull(std::to_string(runningTotal) + std::to_string(*opIter)));
}

int main()
{
    std::string lineString;
    std::ifstream data("input.txt");
    std::vector<int> operations;
    unsigned long long total = 0;
    unsigned long long testVal = 0; 

    std::regex numRegex("\\d+");

    while (std::getline(data, lineString))
    {
        operations.clear();

        // pull out our test value first
        std::sregex_iterator valIter = std::sregex_iterator(lineString.begin(), lineString.end(), numRegex);
        testVal = std::stoull((*valIter).str());

        // iterate over out operator values
        while (++valIter != std::sregex_iterator())
        {
            operations.push_back(std::stoi((*valIter).str()));
        }

        total += testVal * testOperations(testVal, operations.begin() + 1, operations.end(), *operations.begin());
    }

    std::cout << total;
    return 0;
}