#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int fabric_map[1500][1500] = {0};
int input_data[1267][4] = {0};

void load_data(string path)
{
    ifstream input_file(path);
    string line, offset_x, offset_y, width, height;
    
    int i = 0;
    string str_element;
    while(getline(input_file, line)){
        regex rgx("@");
        sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
        sregex_token_iterator end;
        iter++;
        line = *iter;
        regex rgx2(":");
        sregex_token_iterator iter2(line.begin(),line.end(),rgx2,-1);
        offset_x = *iter2;
        iter2++;
        width = *iter2;
        regex rgx3(",");
        sregex_token_iterator iter3(offset_x.begin(),offset_x.end(),rgx3,-1);
        offset_x = *iter3;
        iter3++;
        offset_y = *iter3;
        input_data[i][0] = stoi(offset_x);
        input_data[i][1] = stoi(offset_y);
        regex rgx4("x");
        sregex_token_iterator iter4(width.begin(),width.end(),rgx4,-1);
        width = *iter4;
        iter4++;
        height = *iter4;
        input_data[i][2] = stoi(width);
        input_data[i][3] = stoi(height);
        i++;
    }

}

int main()
{
    int count_inches = 0;
    load_data("input.txt");
    for(int k = 0; k < 1267; k++)
    {
        for(int i = input_data[k][1]; i < (input_data[k][1] + input_data[k][3]); i++)
        {
            for(int j = input_data[k][0]; j < (input_data[k][0] + input_data[k][2]); j++)
            {
                fabric_map[i][j]++;
            }
        }
    }

    for(int i = 0; i < 1500; i++)
    {
        for (int j = 0; j < 1500; j++)
        {
            if(fabric_map[i][j]>1)
            {
                count_inches++;
            }
        }
    }

    cout<< "Result is: "<<count_inches<<endl;
    
    
    
    return 0;
}