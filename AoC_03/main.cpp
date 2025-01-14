#include <iostream>
#include <fstream>
#include <regex>
#include <string>

using namespace std;

#define DoDont

int main()
{
    string mulString;
    string fullData;
    ifstream data("input.txt");
    bool bDoMul = true;

    string operation;
    int total = 0;

    // there could be linebreaks...
    while (getline(data, mulString))
    {
        // do regex to find proper mul() functions
        // mul(xxx,xxx) = "mul(\d{1,3},\d{1,3})"
        // add do() and don't() = "|do\(\)|don't\(\)"

#ifdef DoDont
        regex mul_regex("mul\\(\\d{1,3},\\d{1,3}\\)|do\\(\\)|don't\\(\\)");
#else
        regex mul_regex("mul\\(\\d{1,3},\\d{1,3}\\)");
#endif
        regex val_regex("\\d{1,3}");
        sregex_iterator match = sregex_iterator(mulString.begin(), mulString.end(), mul_regex);

        while (match != sregex_iterator())
        {
            // check for do() and don't() then do the operation if do() is enabled
            operation = (*match).str();
#ifdef DoDont
            if (operation == "do()")
                bDoMul = true;
            else if (operation == "don't()")
                bDoMul = false;
            else if (bDoMul)
#endif
            {
                sregex_iterator nums = sregex_iterator(operation.begin(), operation.end(), val_regex);

                total += stoi((*nums).str()) * stoi((*(++nums)).str());
            }
            
            ++match;
        }
    }

    cout << total;

    return 0;
}