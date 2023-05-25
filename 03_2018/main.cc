#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string input_path = "input.txt";
unsigned int fabric_map[1500][1500] = {0};

struct Claim{
unsigned int id;
unsigned int offset_x;
unsigned int offset_y;
unsigned int width;
unsigned int height;
};

void load_data(const std::string& path, std::vector<Claim>& claims_vector)
{
    std::ifstream input_file(path);
    std::string line;
    
    while(getline(input_file, line)){
        Claim claim;

        size_t delim_1_pos, delim_2_pos, delim_3_pos, delim_4_pos;
        const char delim_1 = '@';
        const char delim_2 = ',';
        const char delim_3 = ':';
        const char delim_4 = 'x';


        delim_1_pos = line.find(delim_1);
        delim_2_pos = line.find(delim_2);
        delim_3_pos = line.find(delim_3);
        delim_4_pos = line.find(delim_4);

        claim.id = stoi(line.substr(1, delim_1_pos-1));
        claim.offset_x = stoi(line.substr(delim_1_pos + 1, delim_2_pos - (delim_1_pos + 1)));
        claim.offset_y = stoi(line.substr(delim_2_pos + 1, delim_3_pos - (delim_2_pos + 1)));
        claim.width = stoi(line.substr(delim_3_pos + 1, delim_4_pos - (delim_3_pos + 1)));
        claim.height = stoi(line.substr(delim_4_pos + 1, line.length() - (delim_4_pos + 1))); 

        claims_vector.push_back(claim);
    }

}

int main()
{
    unsigned int count_inches = 0;

    std::vector<Claim> input_claims;
    load_data(input_path, input_claims);

    for (auto& claim : input_claims)
    {
        for(int i = claim.offset_y; i < (claim.offset_y + claim.height); ++i){
            for(int j = claim.offset_x; j < (claim.offset_x + claim.width); ++j)
            {
                fabric_map[i][j]++;
            }
        }
    }


    for(unsigned int i = 0; i < 1500; ++i)
    {
        for (unsigned int j = 0; j < 1500; ++j)
        {
            if(fabric_map[i][j] > 1)
            {
                count_inches++;
            }
        }
    }
    
    bool no_overlap = true;
    int no_overlap_id;

    for(auto& claim : input_claims)
    {
        no_overlap = true;
        for(unsigned int i = claim.offset_y; i < (claim.offset_y + claim.height); ++i)
        {
            for(unsigned int j = claim.offset_x; j < (claim.offset_x + claim.width); ++j)
            {
                if(fabric_map[i][j] != 1)
                {
                    no_overlap = false;
                    break;
                }
            }
            if(no_overlap == false)
            {
                break;
            }
        }
        if(no_overlap == true)
        {
            no_overlap_id = claim.id;
            break;
        }
    }

    std::cout<< "Result is: "<< count_inches << std::endl;

    std::cout<< "Not overlapping id is: "<< no_overlap_id << std::endl;
    
    return 0;
}