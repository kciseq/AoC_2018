#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    static const int kLenOfStr = 26;
    ifstream input_file("input.txt");
    string line;
    std::vector<char> letters(255);
    bool not_incremented_2;
    bool not_incremented_3;
    int counts[kLenOfStr];
    int count_of_2 = 0, count_of_3 = 0;
    int result = 0;
    while(input_file >> line){
        for(unsigned int i = 0; i< kLenOfStr; i++)
        {
            letters[line[i]]++;
            counts[i] =line[i];  
        }
        not_incremented_2 = true;
        not_incremented_3 = true;
        for(unsigned int i = 0; i< kLenOfStr; i++)
        {
           if((letters[counts[i]] ==2) && not_incremented_2){
                count_of_2++;
                letters[counts[i]] =0;
                counts[i] = 0;
                not_incremented_2 = false;
           }
           if(letters[counts[i]] ==3 && not_incremented_3){
                count_of_3++;
                letters[counts[i]] =0;
                counts[i] = 0;
                not_incremented_3 = false;
           }
           letters[counts[i]] =0;
           counts[i] = 0;
        }

    }
    result = count_of_2 * count_of_3;
    cout<< " Result is: " << result << endl;
    return 0;
}