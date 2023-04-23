#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

int main()
{
    static const unsigned int kLenOfStr = 26;
    static const unsigned int kArraySize = 'z' - 'a' + 1;
    std::array<char, kArraySize> letters;
    std::vector<std::string> inputs;
    
    bool found = false;
    
    unsigned int count_of_2 = 0, count_of_3 = 0;
    unsigned int result = 0;

    /***************************Reading the input data*****************************/
    {
        ifstream input_file("input.txt");
        string line;
        while(input_file >> line)
        {
            inputs.push_back(line);
        }
    }
    
    /****************************First part of the task***************************/
    for(unsigned int i = 0; i < inputs.size(); i++)
    {
        for(unsigned int j = 0; j< kLenOfStr; j++)
        {
            letters[inputs[i][j]]++;
        }
        bool not_incremented_2 = true;
        bool not_incremented_3 = true;
        for(unsigned int j = 'a'; j < 'z' + 1; j++)
        {
           if((letters[j] ==2) && not_incremented_2)
           {
                count_of_2++;
                not_incremented_2 = false;
           }
           if((letters[j] ==3) && not_incremented_3)
           {
                count_of_3++;
                not_incremented_3 = false;
           }
           letters[j] =0;
        }
    }
    result = count_of_2 * count_of_3;
    std::cout<< " Result is: " << result << endl;

    /**********************Second part of the task**************************/
    std::string result_string;
    for(unsigned int i = 0; i < inputs.size(); i++)
    {
        std::array<bool, kLenOfStr> mask;
        mask.fill(false);
        for(unsigned int j = i+1; j < inputs.size(); j++)
        {
            unsigned int count = 0;
            for(unsigned int k = 0; k < kLenOfStr; k++)
            {
                if(inputs[i][k] == inputs[j][k])
                {
                    mask[k] = true;
                    count++;
                }
            }
            if(count == (kLenOfStr - 1))
            {
                for(unsigned int k = 0; k < kLenOfStr; k++)
                {
                    if(mask[k] == true)
                    {
                        result_string.push_back(inputs[i][k]);
                    }
                }
                std::cout<< " Result code is: " << result_string << endl;
                return 0;
            }
        }
    }
    std::cout<< "Result not found." << endl;
    return 0;
}
