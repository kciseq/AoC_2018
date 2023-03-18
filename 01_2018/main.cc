#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

int main ()
{
    auto start = chrono::high_resolution_clock::now();
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

    unsigned int count = input_list_len;

    while(searching){
            frequency_deviation +=  deviation_list[count%input_list_len];
            frequencies.push_back(frequency_deviation);
            for(unsigned int i = (frequencies.size() - 2); i>0; i--){
                if(frequencies[count] == frequencies[i]){
                result = frequencies[count];
                searching = false;
                break;
                }
            }
            count++;
        }
    cout<< "Frequency that occurs twice first is: " << result <<endl;

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);

    cout << "Execution time in seconds is: " << duration.count() << endl; 


    
    return 0;
}