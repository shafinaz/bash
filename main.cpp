#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using std::cout;
using std::istringstream;
using std::string;

void handleCD(const std::vector<std::string>& tokens)
{
    if (tokens.size() != 2)
    {
        std::cerr << "Error: cd command requires exactly one argument. \n";
        return;
    }

    const char* path = tokens[1].c_str();
    if (chdir(path) != 0)
    {
        std::cerr << "Error: Failed to change directory. \n";
    }
}

bool quoteInputs(const string& input, std::vector<std::string>& tokens)
{
    string token;
    bool inQuotes = false;
    char quoteChar = '\0';

    for(auto c : input) {
        if(c == '\'' || c == '"')
        {
            if (inQuotes && c == quoteChar) 
            {
                inQuotes = false;
            }
            else if (!inQuotes) 
            {
                inQuotes = true;
                quoteChar = c;
            }
            else
            {
                token += c;
            }
        }
        else if (std::isspace(c) && !inQuotes)
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else
        {
            token += c;
        }
    }

    if (inQuotes)
    {
        std::cerr << "Error : Mismatched quotes. \n";
        return false;
    }

    if (!token.empty())
    {
        tokens.push_back(token);
    }

    return true;

}

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
    
   while(true)
    {
        std::cerr << "$ ";

        if (!std::getline(std::cin, input))
        {
            if (std::cin.eof())
            {
                break;
            }
            else
            {
                std::cerr << "Error reading input. \n";
                continue;
            }
        }

        /* Check for command line length */
        if(input.length() > 1000) {
            std::cerr << "Error : Command line exceeds 1000 characters. \n";
            continue;
        }

        input = trim(input);

        if(input == "exit")
            break;
        
        std::vector<std::string> tokens;
        if(!quoteInputs(input, tokens)) {
            continue;
        }

        /* Check for arguments' size */
        if(tokens.size() > 100) {
            std::cerr << "Error : Arguments exceeds 100 count. \n";
            continue;
        }

        if(!tokens.empty())
        {
            string command = tokens[0];

            if(command == "cd")
            {
                handleCD(tokens);
            }
            else
            {
                std::vector<char*> arguments;

                for (auto& tkn : tokens)
                {
                    arguments.push_back(&tkn[0]);
                }
                arguments.push_back(nullptr);

                pid_t pid = fork();

                if (pid < 0) {
                    std::cerr << "Error : Fork failed. \n";
                }
                else if (pid == 0)
                {
                    /* Child process */
                    execvp(command.c_str(), arguments.data());

                    std::cerr << "Error : Command execution failed. \n";
                    exit(EXIT_FAILURE);

                }
                else{
                    /* Parent process */
                    int status;
                    waitpid(pid, &status, 0);

                    if(WIFEXITED(status))
                    {
                        int exitCode = WEXITSTATUS(status);
                        if(exitCode != 0)
                        {
                            std::cerr << "Command exited with code " << exitCode << std::endl;
                        }

                    }   
                    else if (WIFSIGNALED(status))
                    {
                        int signalNumber = WTERMSIG(status);
                        std::cerr << "Command terminated by signal " << signalNumber << std::endl;
                    }
                }
            }
        }
        cout << std::endl;

    }

    return 0;
}