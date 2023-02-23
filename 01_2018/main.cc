#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main ()
{
    unsigned int i = 0;
    
    int deviation_list[996] = {};
    long frequencies[200*996] = {0};
    int results[200*996] = {0};
    ifstream input_file("input.txt");
    int frequency_deviation = 0;
    int input_list;
    string line;
    uint8_t flag = 0;
    unsigned int counter;
    while(input_file >> input_list){
        frequency_deviation += input_list;
        frequencies[i] = frequency_deviation;
        deviation_list[i] = input_list;
        i++;
    }
    cout<< "Frequency deviation equals to: "<<frequency_deviation<<endl;

    for(unsigned int i = 0; i < 199; i++){
        for(unsigned int j = 0; j<996; j++){
            frequency_deviation +=  deviation_list[j];
            frequencies[996+(i*996)+j] = frequency_deviation;
        }
    }

    for(unsigned int i = 0; i < 200 * 996; i++){
        for(unsigned int j = i+1; j < 200 * 996; j++){
            if(frequencies[i] == frequencies[j]){
                results[counter] = j;
                counter++;
            }
        }
    }

    unsigned int min = results[0];
    for(unsigned i = 0; i<counter; i++){
        if(results[i]< min){
            min = results[i];
        }
    }

    cout<< "Frequency that occurs twice first is: " << frequencies[min]<<endl;
    
    return 0;
}