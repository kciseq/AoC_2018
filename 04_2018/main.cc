#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

int input_data[6] = {0};

struct myEventStructure{
    int guardId;
    uint8_t eventType;
    unsigned int definitiveTime;
};

struct myGuardStructure{
    std::vector<std::array<uint8_t,60>> asleep_data;
    uint8_t minute_slept_most;
    uint8_t times_slept_on_that_min;
};



void load_data(string path, std::vector<myEventStructure> &events, std::vector<unsigned int> &ids)
{
    myEventStructure event;
    ifstream input_file(path);
    string line;
    string month, day, hour, minute, guard_number, event_type; // event -0 start shift, 1 falls asleep, 2 wakes up
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
        input_data[0] = stoi(month);
        input_data[1] = stoi(day);
        input_data[2] = stoi(hour);
        input_data[3] = stoi(minute);
        // różne miesiące mają różną ilość dni i to mi psuje plan troche
        switch (input_data[0])
        {
        case 1:
            time_min_definitive = 0;
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

        time_min_definitive += (input_data[1] - 1)*24*60;
        time_min_definitive += (input_data[2])*60;
        time_min_definitive += (input_data[3]);

        event.definitiveTime = time_min_definitive;
        
        if(line.substr(19,5) == "wakes"){
        input_data[4] = 2;
        }else if(line.substr(19,5) == "falls"){
            input_data[4] = 1;

        }else if(line.substr(19,5) == "Guard"){
            input_data[4] = 0;
            position = line.find(delim1,18);
            line.erase(0, position);            
            position = line.find(delim2,0);
            check = line.substr(1,position-1);
            input_data[5] = stoi(check);
            event.guardId = input_data[5];
            bool id_recorded = false;
            for(unsigned int i = 0; i < ids.size(); ++i)
            {
                if(event.guardId == ids[i]){
                    id_recorded = true;
                }
            }
            if(id_recorded == false){
                ids.push_back(event.guardId);
            }    
        }
        event.eventType = input_data[4];
        events.push_back(event);
        i++;
    }
}

void sort_data(std::vector<myEventStructure> &events)
{
    std::vector<myEventStructure> sorted_events;
    for(unsigned int j = 0; j < events.size(); ++j)
    {
        unsigned int min_time = events[0].definitiveTime;
        unsigned int index = 0;
        for(unsigned int i = 0; i< events.size(); ++i)
        {
            if(events[i].definitiveTime < min_time)
            {
                min_time = events[i].definitiveTime;
                index = i;
            }
        }
        sorted_events.push_back(events[index]);
        if(sorted_events[sorted_events.size()-1].eventType != 0)
        {
            sorted_events[sorted_events.size()-1].guardId = sorted_events[sorted_events.size()-2].guardId;
        }
        events[index].definitiveTime = UINT32_MAX;
    }
    events = sorted_events;
}

int main(){
    std::vector<myEventStructure> events;
    std::vector<unsigned int> ids;
    std::array<std::array<uint8_t,60>, 10000> asleep_data; 

    for(auto row : asleep_data){
        row = {};
    }

    unsigned int total_mins_asleep[10000];

    load_data("input.txt", events, ids);
    sort_data(events);

    for(unsigned int i = 1; i < events.size(); ++i)
    {
        // poprzedni i ten event to albo
        // zaczyna zmiane, idzie spać - to mnie nie obchodzi
        // idzie spać, budzi się - to jest najważniejsze
        unsigned int minute_of_falling = 0;
        unsigned int minute_of_waking = 0;
        if((events[i].eventType == 2)&&((events[i-1].eventType == 1))){
            minute_of_falling = ((events[i-1].definitiveTime)%(24*60)); //-23*60;
            minute_of_waking = (events[i].definitiveTime%(24*60));//-23*60);
            total_mins_asleep[events[i].guardId] = minute_of_waking - minute_of_falling;
            for(unsigned int j = minute_of_falling; j < minute_of_waking; ++j)
            {
                asleep_data[events[i].guardId][j]++;
            }
        }
        // idzie spać, zaczyna zmianę. to znaczy że zmiana się zaczęła następnego dnia
        if((events[i].eventType == 0)&&((events[i-1].eventType == 1))){
            minute_of_falling = (events[i-1].definitiveTime%(24*60));//-23*60);
            minute_of_waking = 60;
            total_mins_asleep[events[i-1].guardId] += minute_of_waking - minute_of_falling;
            for(unsigned int j = minute_of_falling; j < minute_of_waking; ++j)
            {
                asleep_data[events[i-1].guardId][j]++;
            }
        }
    }

    // which guard slept the most
    unsigned int max_time_slept = total_mins_asleep[ids[0]];
    unsigned int guard_id = ids[0];
    for(unsigned int i = 0; i < ids.size(); ++i)
    {
        if(total_mins_asleep[ids[i]] > max_time_slept){
            max_time_slept = total_mins_asleep[ids[i]];
            guard_id = ids[i];
        }
    }
    // which minute he slept the most
    uint8_t min_slept_most = 0;
    uint8_t times_slept = 0;
    for(unsigned int i = 0; i < 60; ++i)
    {
        if(asleep_data[guard_id][i] > times_slept){
            min_slept_most = i;
            times_slept = asleep_data[guard_id][i];
        }
    }
    unsigned int result = guard_id * min_slept_most;
    std::cout<< "Result is: " << result << std::endl;

    // which guard slept the most on the same minute
    unsigned int index = ids[0];
    min_slept_most = 0;
    times_slept = 0;
    for(unsigned int i = 0; i < ids.size(); ++i)
    {
        for(unsigned int j = 0; j < 60; ++j)
        {
            if(times_slept < asleep_data[ids[i]][j])
            {
                times_slept = asleep_data[ids[i]][j];
                min_slept_most = j;
                index = ids[i];
            }
        }
    }
    result = index * min_slept_most;

    std::cout<< "Second result is: " << result << std::endl; 
    return 0;
}