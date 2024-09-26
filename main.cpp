#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

vector<string> trim(string input)
{
    vector<string> output;
    string word;
    stringstream ss(input);

    while(ss >> word)
    {
        output.push_back(word);
    }

    return output;
    
}

int main()
{
    string input;
    string command;

    while(true)
    {
        cerr << "$ ";
        getline(cin, input, '\n');

        if(input.size() > 1000){
            cerr << "Error : Command input exceeds 1000 characters \n";
            continue;
        }

        vector<string> tokens = trim(input);

        if(tokens.empty())
            continue;
        
        if(tokens.size() > 100){
            cerr << "Error : Command arguments exceed 100 \n";
            continue;
        }
            
        command = tokens[0];

        if(command == "exit")
            break;
        
        std::vector<char*> arguments;

        for (auto& tkn : tokens)
        {
            arguments.push_back(&tkn[0]);
        }
        arguments.push_back(nullptr);

        pid_t id = fork();

        if(id < 0)
        {
            cerr << "Error : Fork failed \n";
        } 
        else if (id == 0)
        {
            /* Child process */
            execvp(command.c_str(), arguments.data());

            //std::cerr << "Error : Command execution failed. \n";
            //exit(EXIT_FAILURE);
        }
        else
        {
            /* Parent process */
        }      

    }

    return 0;
}