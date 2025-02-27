#include <iostream>
#include <fstream>
#include <string>

//#define WIDE_GRID

bool ProcessGridLine(std::string& InputLine);
void ProcessMove(char*& BotLoc, char***& MoveQueue, int DirectionOffset, bool bVerticalMove);
char* FindBotLoc(std::string& InGrid, const int& Width, const int& Height);
void DoMoves(std::string& Grid, std::string& Moves, int& Width, int& Height);

int main()
{
    using namespace std;

    string Grid;
    string Moves;
    string ReadLine;
    string WideLine;
    ifstream Data("input.txt");

    bool bReadingGrid = true;
    int Height = 0;
    int Width = 0;

    while(getline(Data, ReadLine))
    {
        if (bReadingGrid)
        {
            if (Width == 0)
                Width = ReadLine.size();

#ifdef WIDE_GRID
            WideLine.clear();
            for (string::const_iterator iter = ReadLine.begin(); iter != ReadLine.end(); ++iter)
            {
                switch (*iter)
                {
                    case '#':
                        WideLine.append("##");
                        break;
                    case 'O':
                        WideLine.append("[]");
                        break;
                    case '.':
                        WideLine.append("..");
                        break;
                    case '@':
                        WideLine.append("@.");
                        break;
                }
            }

            ReadLine = WideLine;
#endif
            Grid.append(ReadLine);

            if (Height > 0)
            {
                bReadingGrid = ProcessGridLine(ReadLine);
            }
            ++Height;
        }
        else
        {
            Moves.append(ReadLine);
        }
    }

#ifdef WIDE_GRID
    Width *= 2;
#endif

    DoMoves(Grid, Moves, Width, Height);

    int GPS = 0;
    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
#ifdef WIDE_GRID
            if (Grid[x + y * Width] == '[')
#else
            if (Grid[x + y * Width] == 'O')
#endif
            {
                GPS += 100 * y + x;
            }
        }
    }

    cout << "GPS Total: " << GPS;
    
    return 0;
}

/// @brief Check if this grid line is the bottom row
/// @param InputLine the row to check
/// @return true if there are more grid lines to process
bool ProcessGridLine(std::string& InputLine)
{
    for (int c = 0; c < InputLine.size(); ++c)
    {
        if (InputLine[c] != '#')
        {
            return true;
        }
    }

    return false;
}

/// @brief Called after the grid is assembled to find the address of the bot in the grid
/// @param InGrid The grid to search for the bot location
/// @param Width width of the grid
/// @param Height height of the grid
/// @return pointer to the bot address in the grid.
char* FindBotLoc(std::string& InGrid, const int& Width, const int& Height)
{
    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            if (InGrid[x + y * Width] == '@')
            {
                return &InGrid[x + y * Width];
            }
        }
    }

    return nullptr;
}

/// @brief See if we have an empty space at the end of the row we're pushing. If "WIDE_GRID" we create multiple Queues each time we push an offset barrel.
/// @param BotLoc pointer to the bots location in the Grid array, all future lookups stem from this address.
/// @param MoveQueue Our static 2d array of queues for each pushing line.
/// @param DirectionOffset The array offset for the direction we're pushing
/// @param bVerticalMove We only want to branch queues if we're attempting to push barrels vertically
void ProcessMove(char*& BotLoc, char***& MoveQueue, int DirectionOffset, bool bVerticalMove)
{
    char* NextLoc = MoveQueue[0][0] = BotLoc;
    int QueueSize = 0;
    int CurrentQueue = 0;
    int QueueWidth = 0;
    do
    {
        NextLoc = MoveQueue[CurrentQueue][0];
        QueueSize = 0;
        while (*NextLoc != '.')
        {
            MoveQueue[CurrentQueue][QueueSize++] = NextLoc;
            NextLoc += DirectionOffset;

            if (*NextLoc == '#')
                return;
#ifdef WIDE_GRID
            if (bVerticalMove)
            {
                if (*NextLoc == '[' && *MoveQueue[CurrentQueue][QueueSize - 1] != '[')
                    MoveQueue[++QueueWidth][0] = NextLoc + 1;
                else if (*NextLoc == ']' && *MoveQueue[CurrentQueue][QueueSize - 1] != ']')
                    MoveQueue[++QueueWidth][0] = NextLoc - 1;
            }
#endif

        }
        MoveQueue[CurrentQueue][QueueSize++] = NextLoc;
    } while (++CurrentQueue <= QueueWidth);

    // if we actually have room to move stuff, lets do it.
    do
    {
        for (QueueSize = 0, NextLoc = MoveQueue[QueueWidth][0]; *NextLoc != '.'; ++QueueSize)
        {
            NextLoc = MoveQueue[QueueWidth][QueueSize];
        }

        while (QueueSize > 0)
        {
            *NextLoc = *MoveQueue[QueueWidth][--QueueSize];
            NextLoc = MoveQueue[QueueWidth][QueueSize];
        }
        *NextLoc = '.';

    } while (--QueueWidth >= 0);

    BotLoc += DirectionOffset;
}

/// @brief Process the given moves string on the given grid
/// @param Grid The grid of walls and barrels
/// @param Moves The list of moves to perform
/// @param Width the width of the grid
/// @param Height the height of the grid
void DoMoves(std::string& Grid, std::string& Moves, int& Width, int& Height)
{
    using namespace std;

    char* BotLoc = FindBotLoc(Grid, Width, Height);
    int maxSize = max(Width, Height);
    char*** MoveQueue = new char**[maxSize];
    for (int i = 0; i < maxSize; ++i)
    {
        MoveQueue[i] = new char*[maxSize];
    }

    for( string::const_iterator MoveIt = Moves.begin(); MoveIt != Moves.end(); ++MoveIt)
    {
        switch(*MoveIt)
        {
            case '^':
                ProcessMove(BotLoc, MoveQueue, -Width, true);
                break;
            case '>':
                ProcessMove(BotLoc, MoveQueue, 1, false);
                break;
            case 'v':
                ProcessMove(BotLoc, MoveQueue, Width, true);
                break;
            case '<':
                ProcessMove(BotLoc, MoveQueue, -1, false);
                break;
        }

        // for (int y = 0; y < Height; ++y)
        // {
        //     cout << "\n";
        //     for (int x = 0; x < Width; ++x)
        //     {
        //         cout << Grid[x + y * Width];
        //     }
        // }
    }

    for (int i = 0; i < maxSize; ++i)
    {
        delete[] MoveQueue[i];
    }
    delete[] MoveQueue;
}