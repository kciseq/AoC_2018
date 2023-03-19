#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int input_data[1040][6] = {0};

struct myEventStructure{
    int guardId;
    uint8_t eventType;
    unsigned int definitiveTime;
};

std::vector<myEventStructure> events;

void load_data(string path)
{
    myEventStructure event;
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
        unsigned int time_min_definitive = 0;
        len = line.length();
        month = line.substr(6,2);
        day = line.substr(9,2);
        hour = line.substr(12,2);
        minute = line.substr(15,2);
        input_data[i][0] = stoi(month);
        input_data[i][1] = stoi(day);
        input_data[i][2] = stoi(hour);
        input_data[i][3] = stoi(minute);
        // różne miesiące mają różną ilość dni i to mi psuje plan troche
        switch (input_data[i][0])
        {
        case 1:
            time_min_definitive += 0*24*60;
            break;
        case 2:
            time_min_definitive += (31)*24*60;
            break;
        case 3:
            time_min_definitive += ((1*31)+28)*24*60;
            break;
        case 4:
            time_min_definitive += ((2*31)+28)*24*60;
            break;
        case 5:
            time_min_definitive += ((2*31)+28+30)*24*60;
            break;
        case 6:
            time_min_definitive += ((3*31)+28+30)*24*60;
            break;
        case 7:
            time_min_definitive += ((3*31)+28+(2*30))*24*60;
            break;
        case 8:
            time_min_definitive += ((4*31)+28+(2*30))*24*60;
            break;           
        case 9:
            time_min_definitive += ((5*31)+28+(2*30))*24*60;
            break;
        case 10:
            time_min_definitive += ((5*31)+28+(3*30))*24*60;
            break;
        case 11:
            time_min_definitive += ((6*31)+28+(3*30))*24*60;
            break;
        case 12:
            time_min_definitive += ((6*31)+28+(4*30))*24*60;
            break;                
        default:
            break;
        }

        time_min_definitive += (input_data[i][1]-1)*24*60;
        time_min_definitive += (input_data[i][2])*60;
        time_min_definitive += (input_data[i][3]);

        event.definitiveTime = time_min_definitive;
        
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