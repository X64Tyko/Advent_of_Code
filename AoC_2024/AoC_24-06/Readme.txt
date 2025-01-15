https://adventofcode.com/2024/day/6

problems:
1) If there is no solution this code will loop forever. This can be fixed by the main loop also checking if we're caught in a loop and quitting if found.
2) Using a class for the guard is slower than a more functional approach, like storing the direction as [1, 0] and guard as [6, 8] and doing all the processing in the main while loop.
3) threading, findLoop() could be run on a separate thread(s) since all it's data is a copy of the original data.


Edit: Updated it to use simple threads.