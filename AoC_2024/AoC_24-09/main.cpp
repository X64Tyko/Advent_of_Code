#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

unsigned long long contiguousDefragment(std::string inputStr)
{
    unsigned long long checksum = 0;

    bool bFreeAtEnd = inputStr.length() % 2 == 0;

    int currentIndex = 0;
    int startIdx = 0;
    int endIdx = inputStr.length() - (bFreeAtEnd ? 2 : 1);

    int blocksToPlace = inputStr[startIdx] - '0';
    int fileBlocksRemaining = inputStr[endIdx] - '0';
    while(startIdx <= endIdx)
    {
        if (startIdx % 2 == 1)
        {
            // fill in free space from the end
            checksum += currentIndex * (int)(endIdx * 0.5f);
            --fileBlocksRemaining;
            --blocksToPlace;
        }
        else
        {
            // this is a file
            checksum += currentIndex * (int)(startIdx * 0.5f);
            --blocksToPlace;
        }

        // this is to address 0 free space
        while (blocksToPlace == 0)
        {
            ++startIdx;
            blocksToPlace = inputStr[startIdx] - '0';
        }

        if (fileBlocksRemaining == 0)
        {
            // skip the free space at the end.
            endIdx -= 2;
            fileBlocksRemaining = inputStr[endIdx] - '0';
        }

        if (startIdx == endIdx)
        {
            blocksToPlace = std::min(blocksToPlace, fileBlocksRemaining);
        }

        ++currentIndex;
    }

    return checksum;
}

unsigned long long Defragment(std::string inputStr)
{
    unsigned long long checksum = 0;
    std::vector<int> blockData;
    std::map<int, int> movedChunkIds;

    bool bFreeAtEnd = inputStr.length() % 2 == 0;

    int currentIndex = 0;
    int startIdx = 0;
    int endIdx = inputStr.length() - (bFreeAtEnd ? 2 : 1);
    int fillerIdx = 0;

    int blocksToPlace = inputStr[startIdx] - '0';
    int fileBlocksRemaining = 0;
    while (startIdx <= endIdx)
    {
        if (startIdx % 2 == 1)
        {
            //std::cout<< "attempting to fill blocks: " << blocksToPlace << "\n";
            fillerIdx = inputStr.length() - (bFreeAtEnd ? 2 : 1);
            // fill in free space from the end
            while (fillerIdx > startIdx && blocksToPlace > 0)
            {
                fileBlocksRemaining = inputStr[fillerIdx] - '0';
                //std::cout << "checking size: " << fileBlocksRemaining << "\n";
                // skip any chunks that have already been moved.
                if (movedChunkIds.count(fillerIdx) == 0 && fileBlocksRemaining <= blocksToPlace)
                {
                    //std::cout << "moving block: " << fillerIdx << "\n";
                    blocksToPlace -= fileBlocksRemaining;
                    movedChunkIds[fillerIdx] = 1;

                    while (fileBlocksRemaining > 0)
                    {
                        //std::cout << (int)(fillerIdx * 0.5f);
                        checksum += currentIndex * (int)(fillerIdx * 0.5f);
                        ++currentIndex;
                        --fileBlocksRemaining;
                    }
                    //std::cout << "New free size: " << blocksToPlace << "\n";
                }

                fillerIdx -= 2;
            }

            // for (int i = 0; i < blocksToPlace; ++i)
            //     std::cout << '.';
            currentIndex += blocksToPlace;
            blocksToPlace = 0;
        }
        else if (movedChunkIds.count(startIdx) == 0) // again, skip chunks that have been moved
        {
            // this is a file
            //std::cout << (int)(startIdx * 0.5f);
            checksum += currentIndex * (int)(startIdx * 0.5f);
            --blocksToPlace;
            ++currentIndex;
        }
        else
        {
            // for (int i = 0; i < blocksToPlace; ++i)
            //     std::cout << '.';
            currentIndex += blocksToPlace;
            blocksToPlace = 0;
        }

        // this is to address 0 free space
        while (blocksToPlace == 0)
        {
            ++startIdx;
            blocksToPlace = inputStr[startIdx] - '0';
        }

        //std::cout << startIdx << "\n";
    }

    return checksum;
}

int main()
{
    std::string inputStr;
    std::ifstream data("input.txt");
    std::string lineString;

    while (std::getline(data, lineString))
    {
        inputStr.append(lineString);
    }
    data.close();

    unsigned long long CD_Checksum = contiguousDefragment(inputStr);
    unsigned long long D_Checksum = Defragment(inputStr);

    std::cout << "\nCD Checksum: " << CD_Checksum;
    std::cout << "\nD Checksum: " << D_Checksum;

    return 0;
}