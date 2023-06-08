#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <algorithm>
#include <functional>

enum class DataSample{op_code, a, b, c};

struct Sample{
std::array<int, 4> reg_before, reg_after;
std::map<DataSample, int> data_map;
unsigned int match_count =0;
};

struct Instruction{
std::map<DataSample, int> data_map;
};

void load_data(const std::string& input_path, std::vector<Sample>& input_samples, std::vector<Instruction>& instructions_input){
    std::fstream input_file(input_path);
    std::string line;
    int empty_lines_count, line_num = 0;


    std::regex pattern("\\d+");

    std::smatch matches;
    Sample parsed_sample;
    bool loading_test_program = false;

    while (getline(input_file, line))
    {
        if(line.empty())
        {
            empty_lines_count++;
            line_num = 0;
            if(empty_lines_count ==2){
                loading_test_program = true;
            }
            continue;
        }
        empty_lines_count = 0;
        line_num++;
        if(!loading_test_program){
            if(line_num == 1){
                unsigned i = 0;
                std::sregex_iterator iterator(line.begin(), line.end(), pattern);
                std::sregex_iterator end;
                while(iterator != end){
                    std::smatch match = *iterator;
                    parsed_sample.reg_before[i] = stoi(match.str());
                    ++iterator;
                    ++i;
                }
            }else if(line_num == 2){
                unsigned i = 0;
                std::sregex_iterator iterator(line.begin(), line.end(), pattern);
                std::sregex_iterator end;
                while(iterator != end){
                    std::smatch match = *iterator;
                    parsed_sample.data_map.insert({static_cast<DataSample>(i), stoi(match.str())});
                    ++iterator;
                    ++i;
                }
            }else if(line_num == 3){
                unsigned i = 0;
                std::sregex_iterator iterator(line.begin(), line.end(), pattern);
                std::sregex_iterator end;
                while(iterator != end){
                    std::smatch match = *iterator;
                    parsed_sample.reg_after[i] = stoi(match.str());
                    ++iterator;
                    ++i;
                }
                input_samples.push_back(parsed_sample);
                parsed_sample = Sample();
            }
        }else{
            Instruction parsed_instruction;
            unsigned i = 0;
            std::sregex_iterator iterator(line.begin(), line.end(), pattern);
            std::sregex_iterator end;
            while(iterator != end){
                std::smatch match = *iterator;
                parsed_instruction.data_map.insert({static_cast<DataSample>(i), stoi(match.str())});
                ++iterator;
                ++i;
            }
            instructions_input.push_back(parsed_instruction);
        }
    }
}

std::function<std::array<int,4>(const Sample&)> addr = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        int result = example_input.reg_before[example_input.data_map.at(DataSample::a)] + example_input.reg_before[example_input.data_map.at(DataSample::b)];
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = result;
            return output;
};

std::function<std::array<int,4>(const Sample&)> addi = [](const Sample& example_input) -> std::array<int,4> {

        std::array<int, 4> output = example_input.reg_before;
        int result = example_input.reg_before[example_input.data_map.at(DataSample::a)] + example_input.data_map.at(DataSample::b);
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = result;
            return output;
};

std::function<std::array<int,4>(const Sample&)> mulr = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        int result = example_input.reg_before[example_input.data_map.at(DataSample::a)] * example_input.reg_before[example_input.data_map.at(DataSample::b)];
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = result;
            return output;
};

std::function<std::array<int,4>(const Sample&)> muli = [](const Sample& example_input) -> std::array<int,4> {

        std::array<int, 4> output = example_input.reg_before;
        int result = example_input.reg_before[example_input.data_map.at(DataSample::a)] * example_input.data_map.at(DataSample::b);
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = result;
            return output;
};

std::function<std::array<int,4>(const Sample&)> banr = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        int result = example_input.reg_before[example_input.data_map.at(DataSample::a)] & example_input.reg_before[example_input.data_map.at(DataSample::b)];
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = result;
            return output;
};

std::function<std::array<int,4>(const Sample&)> bani = [](const Sample& example_input) -> std::array<int,4> {

        std::array<int, 4> output = example_input.reg_before;
        int result = example_input.reg_before[example_input.data_map.at(DataSample::a)] & example_input.data_map.at(DataSample::b);
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = result;
            return output;
};

std::function<std::array<int,4>(const Sample&)> borr = [](const Sample& example_input) -> std::array<int,4> {

        std::array<int, 4> output = example_input.reg_before;
        int result = example_input.reg_before[example_input.data_map.at(DataSample::a)] | example_input.reg_before[example_input.data_map.at(DataSample::b)];
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = result;
            return output;
};

std::function<std::array<int,4>(const Sample&)> bori = [](const Sample& example_input) -> std::array<int,4> {

        std::array<int, 4> output = example_input.reg_before;
        int result = example_input.reg_before[example_input.data_map.at(DataSample::a)] | example_input.data_map.at(DataSample::b);
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = result;
            return output;
};

std::function<std::array<int,4>(const Sample&)> setr = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = example_input.reg_before[example_input.data_map.at(DataSample::a)];
            return output;
};

std::function<std::array<int,4>(const Sample&)> seti = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        output[static_cast<int>(example_input.data_map.at(DataSample::c))] = example_input.data_map.at(DataSample::a);
            return output;
};

std::function<std::array<int,4>(const Sample&)> gtir = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        if(example_input.data_map.at(DataSample::a) > example_input.reg_before[example_input.data_map.at(DataSample::b)]){
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 1;
        }else{
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 0;
        }
            return output;
};

std::function<std::array<int,4>(const Sample&)> gtri = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        if(example_input.reg_before[example_input.data_map.at(DataSample::a)] > example_input.data_map.at(DataSample::b)){
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 1;
        }else{
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 0;
        }
            return output;
};

std::function<std::array<int,4>(const Sample&)> gtrr = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        if(example_input.reg_before[example_input.data_map.at(DataSample::a)] > example_input.reg_before[example_input.data_map.at(DataSample::b)]){
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 1;
        }else{
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 0;
        }
            return output;
};

std::function<std::array<int,4>(const Sample&)> eqir = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        if(example_input.data_map.at(DataSample::a) == example_input.reg_before[example_input.data_map.at(DataSample::b)]){
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 1;
        }else{
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 0;
        }
            return output;
};

std::function<std::array<int,4>(const Sample&)> eqri = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        if(example_input.reg_before[example_input.data_map.at(DataSample::a)] == example_input.data_map.at(DataSample::b)){
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 1;
        }else{
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 0;
        }
            return output;
};

std::function<std::array<int,4>(const Sample&)> eqrr = [](const Sample& example_input) -> std::array<int,4> {
        std::array<int, 4> output = example_input.reg_before;
        if(example_input.reg_before[example_input.data_map.at(DataSample::a)] ==
                example_input.reg_before[example_input.data_map.at(DataSample::b)]){
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 1;
        }else{
            output[static_cast<int>(example_input.data_map.at(DataSample::c))] = 0;
        }
            return output;
};

std::array<std::function<std::array<int,4>(const Sample&)>*, 16> functions_array = {&addr, &addi, &mulr, &muli, &banr, &bani, 
&borr, &bori, &setr, &seti, &gtir, &gtri, &gtrr, &eqir, &eqri, &eqrr};

int main(){
    std::vector<Sample> input_samples;
    std::vector<Instruction> input_instructions;
    load_data("input.txt", input_samples, input_instructions);

    unsigned behaving_samples_count = 0;
    std::map<unsigned int, std::function<std::array<int,4>(const Sample&)>*> functions_map;
    std::map<int, std::vector<std::vector<std::function<std::array<int,4>(const Sample&)>*>>> op_codes_matches_map;

    for(auto i = 0U; i<16; ++i){
        std::vector<std::vector<std::function<std::array<int,4>(const Sample&)>*>> matching_funcs_sets;
        op_codes_matches_map.insert({i, matching_funcs_sets});
    }

    for (auto& sample : input_samples){
        std::vector<std::function<std::array<int,4>(const Sample&)>*> matching_funcs_for_this_sample;
        int op_code = sample.data_map.at(DataSample::op_code);
        for(const auto& func_addr : functions_array){
            if((*func_addr)(sample) == sample.reg_after ){
                sample.match_count++;
                matching_funcs_for_this_sample.push_back(func_addr);
            }
        }
        if(sample.match_count >= 3){
            ++behaving_samples_count;
        }
        
        op_codes_matches_map.at(op_code).push_back(matching_funcs_for_this_sample);
    }

    for(auto i = 0U; i<16; ++i){
            for(auto& matches_vector : op_codes_matches_map.at(i)){
                int one_more_more = matches_vector.size();
                std::sort(matches_vector.begin(), matches_vector.end());
            }
    }

    while(functions_map.size() < 16){
        for(auto i = 0U; i<16; ++i){
            if(functions_map.find(i) != functions_map.end()){
                continue;
            }
            std::vector<std::function<std::array<int,4>(const Sample&)>*> intersection_vector = op_codes_matches_map.at(i)[0];
            for(const auto& values : functions_map){
                std::vector<std::function<std::array<int,4>(const Sample&)>*>::iterator  iter =  std::find(intersection_vector.begin(), intersection_vector.end(), values.second);
                if(iter != intersection_vector.end()){
                    intersection_vector.erase(iter);
                }
            }
            for(auto j = 1U; j<op_codes_matches_map.at(i).size(); ++j){
                std::vector<std::function<std::array<int,4>(const Sample&)>*> helper_vector;
                std::set_intersection(intersection_vector.begin(), intersection_vector.end(), op_codes_matches_map.at(i)[j].begin(),
                                        op_codes_matches_map.at(i)[j].end(), back_inserter(helper_vector));
                intersection_vector = helper_vector;
                if(intersection_vector.size() == 1){
                    functions_map.insert({i, intersection_vector[0]});
                    break;
                }
            } 
        }
    }

    std::cout << "Number of samples that behave like 3 or more opcodes: " << behaving_samples_count << std::endl;

    std::array<int, 4> initial_reg_state = {0,0,0,0};
    Sample dummy_sample;
    dummy_sample.reg_before = initial_reg_state;

    for(const auto& instruction : input_instructions){
        dummy_sample.data_map = instruction.data_map;
        dummy_sample.reg_before = (*(functions_map.at(static_cast<unsigned int>(dummy_sample.data_map.at(DataSample::op_code)))))(dummy_sample);
    }

    std::cout << "Register 0 after program execution is: " << dummy_sample.reg_before[0] << std::endl;

    return 0;
}