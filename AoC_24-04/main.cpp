#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define X_MAS

const static std::string XMAS = "XMAS";

bool searchLine(const std::string& xmasSearch, const int& currentIdx, const int& width, const int& lastCol, const int offset, int iterations = 1)
{
    int newIdx = currentIdx + offset;
    int newCol = newIdx % width;

    // new index is out of bounds
    if (newIdx <= 0 || newIdx >= xmasSearch.size())
        return false;

    // no wrapping
    if (abs(newCol - lastCol) > 1)
        return false;

    if (xmasSearch[newIdx] == XMAS[iterations])
    {
        if (iterations == XMAS.size() - 1)
            return true;

        return searchLine(xmasSearch, newIdx, width, newCol, offset, ++iterations);
    }

    return false;
}

bool searchX(const std::string& xmasSearch, const int& currentIdx, const int& width, const int& height, const int& col)
{
    // no wrapping
    if (col == 0 || col == width - 1)
        return false;

    int row = currentIdx / width;
    if (row == 0 || row == height - 1)
        return false;

    const char* corners[4] = {&xmasSearch[currentIdx - width - 1], &xmasSearch[currentIdx - width + 1], &xmasSearch[currentIdx + width - 1], &xmasSearch[currentIdx + width + 1]};

    if (((*corners[0] == 'M' && *corners[3] == 'S') || (*corners[0] == 'S' && *corners[3] == 'M')) &&
        ((*corners[1] == 'M' && *corners[2] == 'S') || (*corners[1] == 'S' && *corners[2] == 'M')))
            return true;

    return false;
}

int main ()
{
    std::string xmasSearch;
    std::ifstream data("input.txt");
    int searchWidth = 0;
    int searchHeight = 0;

    // pull the entire search into memory
    std::string tempString;
    while (std::getline(data, tempString))
    {
        ++searchHeight;
        xmasSearch.append(tempString);
    }
    searchWidth = tempString.size();

    int col = 0;
    int XMASFound = 0;
    int searchIdx = 0;
    char* found = nullptr;

    // at this point we have a single string with all the seach puzzle in it as well as the width and height to index across rows
    int offsets[] = {-searchWidth - 1, -searchWidth, -searchWidth + 1, -1, +1, searchWidth - 1, searchWidth, searchWidth + 1};
    while (searchIdx++ < xmasSearch.size())
    {
        col = searchIdx % searchWidth;

        found = &xmasSearch[searchIdx];
        
#ifndef X_MAS
        if (*found != 'X')
            continue;

        for (const int& offset : offsets)
        {
            XMASFound += searchLine(xmasSearch, searchIdx, searchWidth, col, offset, 1);
        }
#else
        if (*found == 'A')
            XMASFound += searchX(xmasSearch, searchIdx, searchWidth, searchHeight, col);
#endif
    }

    std::cout << XMASFound;
    return 0;
}