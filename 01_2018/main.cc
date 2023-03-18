#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main ()
{
    vector<int> deviation_list;
    vector<long> frequencies;
    ifstream input_file("input.txt");
    int frequency_deviation = 0;
    int input_list;
    size_t input_list_len = 0;
    bool searching = true;
    unsigned int loop_count = 1;
    int result = 0;
    
    while(input_file >> input_list){
        frequency_deviation += input_list;
        frequencies.push_back(frequency_deviation);
        deviation_list.push_back(input_list);
        input_list_len++;
    }
    cout<< "Frequency deviation equals to: "<<frequency_deviation<<endl;

    while(searching){
        for(unsigned int j = 0; j<input_list_len; j++){
            frequency_deviation +=  deviation_list[j];
            frequencies.push_back(frequency_deviation);
            for(unsigned int i = (frequencies.size() - 2); i>0; i--){
                if(frequencies[j+(loop_count*input_list_len)] == frequencies[i]){
                result = frequencies[j+(loop_count*input_list_len)];
                searching = false;
                break;
                }
            }
            if(searching == false){
                break;
            }
        }
        loop_count++;
    }

    cout<< "Frequency that occurs twice first is: " << result <<endl;
    
    return 0;
}