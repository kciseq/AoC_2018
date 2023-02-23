#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main ()
{
    unsigned int i = 0;
    long frequencies[996] = {};
    long deviation_list[996] = {};
    ifstream input_file("input.txt");
    long frequency_deviation = 0;
    int input_list;
    string line;
    uint8_t flag = 0;
    while(input_file >> input_list){
        frequency_deviation += input_list;
        frequencies[i] = frequency_deviation;
        deviation_list[i] = input_list;
        i++;
    }
    cout<< "Frequency deviation equals to: "<<frequency_deviation<<endl;
    for(unsigned int i = 1; i < 996; i++){
        for(unsigned int j = i+3; j < 996; j++){
            if((deviation_list[i] == deviation_list[j]) && (deviation_list[i-1] == deviation_list[j-1]) && (deviation_list[i+1] == deviation_list[j+1])){
                cout<< "Indices of pattern edges are " << i << " " << j << endl;
                flag = 1;
                //break;
            }

        }
        if (flag == 1)
        {
            //break;
        }
        
    }
    
    return 0;
}