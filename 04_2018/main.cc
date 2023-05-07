#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


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

void load_data(const std::string &path, std::vector<Event> &events, std::vector<unsigned int> &ids)
{
    std::ifstream input_file(path);
    std::string line;
    const unsigned minutes_per_day = 24 * 60;

    const std::array<unsigned, 12> minutes_count_months = [](auto x){
        std::array<unsigned, 12> result_array {0, 31, 28, 31, 30, 31, 31, 30, 31, 30, 31, 30};
        for (auto i = 1u; i< result_array.size(); ++i){
            result_array[i] *= minutes_per_day * x;
            result_array[i] += result_array[i-1];
        }
        return result_array;
    }(1);

    while(std::getline(input_file, line)){
        Event event;
        const char delim1 = '#';
        const char delim2 = ' ';
        const int month = stoi(line.substr(6,2));
        const int day = stoi(line.substr(9,2));
        const int hour = stoi(line.substr(12,2));
        const int minute = stoi(line.substr(15,2));
        const int minutes_this_month = (day - 1)*minutes_per_day + hour * 60 + minute;

        event.definitiveTime = minutes_count_months[month - 1] + minutes_this_month;
        const std::string_view line_fragment = std::string_view(line).substr(19,5);
        if(line_fragment == "wakes"){
        event.eventType = WakingUp;
        }else if(line_fragment == "falls"){
            event.eventType = FallingAsleep;
        }else if(line_fragment == "Guard"){
            event.eventType = ShiftStart;
            event.guardId = stoi(line.substr(line.find(delim1,0)+1,line.find(delim2, line.find(delim1,0))-1));
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

void get_data_on_guards(std::vector<Event> &sorted_events, std::vector<unsigned int> &ids, std::unordered_map<unsigned int, Guard> &data_map)
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
        data_map[id] = this_guard_data;
    }

    for(unsigned int i = 1; i < sorted_events.size(); ++i)
    {
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
                data_map[sorted_events[i].guardId].asleep_data[j]++;
            }
            data_map[sorted_events[i].guardId].total_mins_asleep += mins_asleep;
        }

        // falls asleep, another shift starts - relevant, means that waking minute is = 60
        if((sorted_events[i].eventType == 0)&&((sorted_events[i-1].eventType == 1))){
            minute_of_falling = (sorted_events[i-1].definitiveTime%(24*60));
            minute_of_waking = 60;
            
            for(unsigned int j = minute_of_falling; j < minute_of_waking; ++j)
            {
                data_map[sorted_events[i-1].guardId].asleep_data[j]++;
            }
            mins_asleep = minute_of_waking - minute_of_falling;
            data_map[sorted_events[i-1].guardId].total_mins_asleep += mins_asleep; 
        }
    }

    for(auto &guard : data_map)
    {
    // which minute he slept the most
    uint8_t min_slept_most = 0;
    unsigned int times_slept = 0;
    for(unsigned int i = 0; i < 60; ++i)
    {
        if(guard.second.asleep_data[i] > times_slept){
            min_slept_most = i;
            times_slept = guard.second.asleep_data[i];
        }
    }
    guard.second.minute_slept_most = min_slept_most;
    guard.second.times_slept_on_that_min = times_slept;
    }
}

int main(){
    std::vector<Event> events;
    std::vector<unsigned int> ids;
    std::unordered_map<unsigned int, Guard> guards_map;

    load_data("input.txt", events, ids);
    std::sort(events.begin(), events.end());
    get_data_on_guards(events, ids, guards_map);

    unsigned int total_mins_asleep = guards_map[0].total_mins_asleep;
    unsigned int interesting_id = guards_map[0].guardId;
    unsigned int minute_slept_most = guards_map[0].minute_slept_most;
    unsigned int times_slept_one_minute = guards_map[0].times_slept_on_that_min;

    // find the guard that has most minutes asleep, which minute he sleeps most
    for(auto &guard : guards_map)
    {
        if(guard.second.total_mins_asleep > total_mins_asleep)
        {
            total_mins_asleep = guard.second.total_mins_asleep;
            interesting_id = guard.second.guardId;
            minute_slept_most = guard.second.minute_slept_most;
        }
    }

    unsigned int result = interesting_id * minute_slept_most;
    std::cout<< "Result is: " << result << std::endl;

    // find the guard that sleeps most on the same minute
    for(auto &guard : guards_map)
    {
        if(guard.second.times_slept_on_that_min > times_slept_one_minute)
        {
            times_slept_one_minute = guard.second.times_slept_on_that_min;
            interesting_id = guard.second.guardId;
            minute_slept_most = guard.second.minute_slept_most;
        }
    }

    result = interesting_id * minute_slept_most;

    std::cout<< "Second result is: " << result << std::endl; 
    return 0;
}