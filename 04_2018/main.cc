#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int input_data[1040][6] = {0};

void load_data(string path)
{
    ifstream input_file(path);
    string line;
    string month, day, hour, minute, guard_number, event; // event -0 start shift, 1 falls asleep, 2 wakes up
    size_t len;
    
    
    int i = 0;
    string delim1 = "#";
    string delim2 = " ";
    string check;
    size_t position = 0;

    while(getline(input_file, line)){
        len = line.length();
        month = line.substr(6,2);
        day = line.substr(9,2);
        hour = line.substr(12,2);
        minute = line.substr(15,2);
        input_data[i][0] = stoi(month);
        input_data[i][1] = stoi(day);
        input_data[i][2] = stoi(hour);
        input_data[i][3] = stoi(minute);


        if(line.substr(19,5) == "wakes"){
        input_data[i][4] = 2;


        }else if(line.substr(19,5) == "falls"){
            input_data[i][4] = 1;

        }else if(line.substr(19,5) == "Guard"){
            input_data[i][4] = 0;
            position = line.find(delim1,18);
            line.erase(0, position);            
            position = line.find(delim2,0);
            check = line.substr(1,position-1);
            input_data[i][5] = stoi(check);
        }
        i++;
    }
}

void sort_data()
{

}

int main(){
    load_data("input.txt");

    return 0;
}