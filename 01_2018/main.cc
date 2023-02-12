#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main ()
{
    ifstream input_file("input.txt");
    int i = 0;
    long frequency_deviation = 0;
    int input_list;
    string line;
    while(getline(input_file, line)){
        input_list = stoi(line);
        cout<<input_list<< endl;
        frequency_deviation += input_list;
        i++;    
    }
    cout<< "Frequency deviation equals to: "<<frequency_deviation<<endl;
    return 0;
}