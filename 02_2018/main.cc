#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    const int len_of_str = 26;
    ifstream input_file("input.txt");
    string line;
    int counts[len_of_str];
    int count_of_2 = 0, count_of_3 = 0;
    int result = 0;
    while(getline(input_file,line)){
        for(int k = 0; k < len_of_str; k++)
            {
                counts[k] = 0;
            }
        for(int i = 0; i< len_of_str; i++)
        {
            
            for(int j=0; j < i; j++)
            {
                if(line[i] == line[j])
                {
                    counts[i] +=1;
                }
            }
            for(int j=i+1; j < len_of_str; j++)
            {
                if(line[i] == line[j])
                {
                    counts[i] +=1;
                }
            }
        }
        for(int k = 0; k < len_of_str; k++)
            {
                if(counts[k] == 1) // 1 if occurs twice
                {
                    count_of_2 +=1;
                    break;
                }
            }
            for(int k = 0; k < len_of_str; k++)
            {
                if(counts[k] == 2) // 2 if occurs 3 times
                {
                    count_of_3 +=1;
                    break;
                }
            }

    }
    result = count_of_2 * count_of_3;
    cout<< " Result is: " << result << endl;
    return 0;
}