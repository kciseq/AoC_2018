#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

struct myEventStructure{
    int guardId;
    uint8_t eventType;
    unsigned int definitiveTime;
};

struct myGuardStructure{
    int guardId;
    std::array<unsigned int, 60> asleep_data;
    uint8_t minute_slept_most;
    unsigned int times_slept_on_that_min;
    unsigned int total_mins_asleep;
};



void load_data(string path, std::vector<myEventStructure> &events, std::vector<unsigned int> &ids)
{
    myEventStructure event;
    ifstream input_file(path);
    string line;
    string month, day, hour, minute, guard_number;
    uint8_t event_type; // event: 0 start shift, 1 falls asleep, 2 wakes up
    string delim1 = "#";
    string delim2 = " ";
    size_t position = 0;

    while(getline(input_file, line)){
        unsigned int time_min_definitive = 0;
        month = line.substr(6,2);
        day = line.substr(9,2);
        hour = line.substr(12,2);
        minute = line.substr(15,2);
    
        // different months have different day counts
        switch (stoi(month))
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

        time_min_definitive += (stoi(day) - 1)*24*60;
        time_min_definitive += (stoi(hour))*60;
        time_min_definitive += (stoi(minute));

        event.definitiveTime = time_min_definitive;
        
        if(line.substr(19,5) == "wakes"){
        event_type = 2;
        }else if(line.substr(19,5) == "falls"){
            event_type = 1;

        }else if(line.substr(19,5) == "Guard"){
            event_type = 0;
            position = line.find(delim1,18);
            line.erase(0, position);            
            position = line.find(delim2,0);
            guard_number = line.substr(1,position-1);
            event.guardId = stoi(guard_number);
            bool id_recorded = false;
            // adding id to ids vector
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
        event.eventType = event_type;
        events.push_back(event);
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

void get_data_on_guards(std::vector<myEventStructure> &sorted_events, std::vector<unsigned int> &ids, std::vector<myGuardStructure> &guard_data)
{
    unsigned int mins_asleep;
    unsigned int index=0;

    for(auto &id : ids){
        myGuardStructure this_guard_data;
        this_guard_data.total_mins_asleep = 0;
        this_guard_data.guardId = id;
        this_guard_data.asleep_data = {};
        guard_data.push_back(this_guard_data);
    }

    for(unsigned int i = 1; i < sorted_events.size(); ++i)
    {
        // checking to which guard the event relates
        for(unsigned int j = 0; j < ids.size(); ++j)
        {
            if(sorted_events[i].guardId == ids[j])
            {
                index = j;
            }
        }
        // previous event and this event are:
        // starts shift, falls asleep - irrelevant
        // falls asleep, wakes up - relevant
        unsigned int minute_of_falling = 0;
        unsigned int minute_of_waking = 0;
        if((sorted_events[i].eventType == 2)&&((sorted_events[i-1].eventType == 1))){
            minute_of_falling = ((sorted_events[i-1].definitiveTime)%(24*60));
            minute_of_waking = (sorted_events[i].definitiveTime%(24*60));
            mins_asleep = minute_of_waking - minute_of_falling;
            for(unsigned int j = minute_of_falling; j < minute_of_waking; ++j)
            {
                guard_data[index].asleep_data[j]++;
            }
            guard_data[index].total_mins_asleep += mins_asleep;
        }

        // falls asleep, another shift starts - relevant, means that waking minute is = 60
        if((sorted_events[i].eventType == 0)&&((sorted_events[i-1].eventType == 1))){
            minute_of_falling = (sorted_events[i-1].definitiveTime%(24*60));
            minute_of_waking = 60;
            
            for(unsigned int j = minute_of_falling; j < minute_of_waking; ++j)
            {
                guard_data[index].asleep_data[j]++;
            }
            mins_asleep = minute_of_waking - minute_of_falling;
            guard_data[index].total_mins_asleep += mins_asleep; 
        }
    }

    for(auto &guard : guard_data)
    {
    // which minute he slept the most
    uint8_t min_slept_most = 0;
    unsigned int times_slept = 0;
    for(unsigned int i = 0; i < 60; ++i)
    {
        if(guard.asleep_data[i] > times_slept){
            min_slept_most = i;
            times_slept = guard.asleep_data[i];
        }
    }
    guard.minute_slept_most = min_slept_most;
    guard.times_slept_on_that_min = times_slept;
    }
}

int main(){
    std::vector<myEventStructure> events;
    std::vector<unsigned int> ids;
    std::vector<myGuardStructure> guard_data;


    load_data("input.txt", events, ids);
    sort_data(events);
    get_data_on_guards(events, ids, guard_data);

    unsigned int total_mins_asleep = guard_data[0].total_mins_asleep;
    unsigned int interesting_id = guard_data[0].guardId;
    unsigned int minute_slept_most = guard_data[0].minute_slept_most;
    unsigned int times_slept_one_minute = guard_data[0].times_slept_on_that_min;

    // find the guard that has most minutes asleep, which minute he sleeps most
    for(auto &guard : guard_data)
    {
        if(guard.total_mins_asleep > total_mins_asleep)
        {
            total_mins_asleep = guard.total_mins_asleep;
            interesting_id = guard.guardId;
            minute_slept_most = guard.minute_slept_most;
        }
    }

    unsigned int result = interesting_id * minute_slept_most;
    std::cout<< "Result is: " << result << std::endl;

    // find the guard that sleeps most on the same minute
    for(auto &guard : guard_data)
    {
        if(guard.times_slept_on_that_min > times_slept_one_minute)
        {
            times_slept_one_minute = guard.times_slept_on_that_min;
            interesting_id = guard.guardId;
            minute_slept_most = guard.minute_slept_most;
        }
    }

    result = interesting_id * minute_slept_most;

    std::cout<< "Second result is: " << result << std::endl; 
    return 0;
}