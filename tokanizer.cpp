#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> tokenize(const std::string &input)
{
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string line;

    while (std::getline(iss, line))
    {
        std::istringstream lineIss(line);
        std::string token;

        while (lineIss >> token)
        {
            // Check if the token is a comment
            if (token[0] == '/' && token[1] == '/')
            {
                // Skip the rest of the line
                break;
            }

            // Remove any whitespace characters from the token
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());

            // Separate the token based on specific symbols
            std::string delimiter = "";
            size_t pos = 0;
            while ((pos = token.find_first_of(delimiter, pos)) != std::string::npos)
            {
                std::string extractedToken = token.substr(pos, 1);
                tokens.push_back(extractedToken);
                pos++;
            }

            if (!token.empty())
            {
                tokens.push_back(token);
            }
        }
    }

    return tokens;
}

int main()
{
    std::string input;
    std::cout << "Enter a sentence: ";
    std::getline(std::cin, input);

    std::vector<std::string> tokens = tokenize(input);

    std::cout << "Tokens:\n";
    for (const std::string &token : tokens)
    {
        std::cout << token << '\n';
    }

    return 0;
}
