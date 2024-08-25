#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::istringstream;
using std::string;

string trim(const std::string& str)
{
    auto start = str.begin();

    while(start != str.end() && std::isspace(*start)) 
    {
        start++;
    }

    auto end = str.end();

    do{
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return string(start, end + 1);
}

int main()
{
    string input, token;
    std::vector<std::string> tokens;
    
   while(true)
    {
        cout << "$ ";
        std::getline(std::cin, input);

        input = trim(input);

        if(input == "exit")
            break;
        
        istringstream mystream(input);

        while(mystream >> token)
        {
            tokens.push_back(token);
        }

        if(!tokens.empty())
        {
            string command = tokens[0];
            std::vector<std::string> arguments(tokens.begin() + 1, tokens.end());

            cout << "COMMAND : " << command << std::endl;

        }
        cout << std::endl;

    }

    return 0;
}