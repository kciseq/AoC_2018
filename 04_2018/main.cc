#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

enum EventType : unsigned char{
    ShiftStart, FallingAsleep, WakingUp
};

struct Event{
    int guardId;
    EventType eventType;
    unsigned int definitiveTime;
    bool operator < (const Event& rhs) const {
        return definitiveTime < rhs.definitiveTime;
    }
};

struct Guard{
    int guardId;
    std::array<unsigned int, 60> asleep_data;
    uint8_t minute_slept_most;
    unsigned int times_slept_on_that_min;
    unsigned int total_mins_asleep;
};

void load_data(std::string path, std::vector<Event> &events, std::vector<unsigned int> &ids)
{
    std::ifstream input_file(path);
    std::string line;

    while(std::getline(input_file, line)){
        Event event;
        const char delim1 = '#';
        const char delim2 = ' ';
        size_t position = 0;
        int month = stoi(line.substr(6,2));
        int day = stoi(line.substr(9,2));
        int hour = stoi(line.substr(12,2));
        int minute = stoi(line.substr(15,2));

        // different months have different day counts
        unsigned int minutes_count_months[] = {0, (31)*24*60, ((1*31)+28)*24*60, ((2*31)+28)*24*60, ((2*31)+28+30)*24*60, ((3*31)+28+30)*24*60,
        ((3*31)+28+(2*30))*24*60, ((4*31)+28+(2*30))*24*60, ((5*31)+28+(2*30))*24*60, ((5*31)+28+(3*30))*24*60, ((6*31)+28+(3*30))*24*60,
        ((6*31)+28+(4*30))*24*60};

        event.definitiveTime = minutes_count_months[month - 1] + (day - 1)*24*60 + hour * 60 + minute;
        
        if(line.substr(19,5) == "wakes"){
        event.eventType = WakingUp;
        }else if(line.substr(19,5) == "falls"){
            event.eventType = FallingAsleep;
        }else if(line.substr(19,5) == "Guard"){
            event.eventType = ShiftStart;
            position = line.find(delim1,18);
            line.erase(0, position);            
            position = line.find(delim2,0);
            event.guardId = stoi(line.substr(1,position-1));
            bool id_recorded = false;
            // adding id to ids vector
            for(auto& id : ids)
            {
                if(event.guardId == id)
                {
                    id_recorded = true;
                }
            }
            if(id_recorded == false)
            {
                ids.push_back(event.guardId);
            }    
        }
        events.push_back(event);
    }
}

void get_data_on_guards(std::vector<Event> &sorted_events, std::vector<unsigned int> &ids, std::vector<Guard> &guard_data)
{
    for(unsigned i = 1; i < sorted_events.size(); ++i){
        if(sorted_events[i].eventType == EventType::WakingUp || sorted_events[i].eventType == EventType::FallingAsleep){
            sorted_events[i].guardId = sorted_events[i-1].guardId;
        }       
    }

    unsigned int mins_asleep;
    unsigned int index=0;

    for(auto &id : ids){
        Guard this_guard_data;
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
    std::vector<Event> events;
    std::vector<unsigned int> ids;
    std::unordered_map<unsigned int, Guard> guards_map;
    std::vector<Guard> guard_data;


    load_data("input.txt", events, ids);
    std::sort(events.begin(), events.end());
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