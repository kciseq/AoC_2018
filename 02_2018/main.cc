#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

int main()
{
    static const int kLenOfStr = 26;
    ifstream input_file("input.txt");
    string line;
    unsigned int input_size = 0;
    std::vector<char> letters(255);
    std::vector<std::string> inputs;
    bool not_incremented_2;
    bool not_incremented_3;
    bool found = false;
    std::vector<char*> counts(input_size);
    char* ptr = nullptr;
    
    int count_of_2 = 0, count_of_3 = 0;
    int result = 0;
    int count = 0;

    /***************************Reading the input data*****************************/
    while(input_file >> line){
        inputs.push_back(line);
        ptr = new char[kLenOfStr];
        counts.push_back(ptr);
        input_size++;
    }
    
    /****************************First part of the task***************************/
    for(unsigned int i = 0; i < input_size; i++)
    {
        for(unsigned int j = 0; j< kLenOfStr; j++)
        {
            letters[inputs[i][j]]++;
            *(counts[i]+j) = inputs[i][j];    
        }
        not_incremented_2 = true;
        not_incremented_3 = true;
        for(unsigned int j = 0; j< kLenOfStr; j++)
        {
           if((letters[*(counts[i]+j)] ==2) && not_incremented_2){
                count_of_2++;
                letters[*(counts[i]+j)] =0;
                not_incremented_2 = false;
           }
           if((letters[*(counts[i]+j)] ==3) && not_incremented_3){
                count_of_3++;
                letters[*(counts[i]+j)] =0;
                not_incremented_3 = false;
           }
           letters[*(counts[i]+j)] =0;
        }
        delete[] counts[i];
    }
    result = count_of_2 * count_of_3;
    std::cout<< " Result is: " << result << endl;

    /**********************Second part of the task**************************/
    std::string result_string;
    for(unsigned int i = 0; i < input_size; i++)
    {
        for(unsigned int j = i+1; j < input_size; j++)
        {
            ptr = new char[kLenOfStr];
            count = 0;
            for(unsigned int k = 0; k < kLenOfStr; k++)
            {
                if(inputs[i][k] == inputs[j][k])
                {
                    *(ptr+k) = 1;
                    count++;
                }
            }
            if(count == (kLenOfStr - 1))
            {
                for(unsigned int k = 0; k < kLenOfStr; k++)
                {
                    if(*(ptr+k) == 1)
                    {
                        result_string.push_back(inputs[i][k]);
                    }
                }
                found = true;
                break;
            }
            delete[] ptr;
        }
        if(found == true){
            break;
        }
    }
    std::cout<< " Result code is: " << result_string << endl;
    return 0;
}
