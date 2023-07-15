#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include "lexer.h"
using namespace std;



bool isPunctuator(char ch)					//check if the given character is a punctuator or not
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
        ch == '&' || ch == '|')
    {
        return true;
    }
    return false;
}

bool validIdentifier(char* str)						//check if the given identifier is valid or not
{
    //if first character of string is a digit or a special character, identifier is not valid
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || str[0] == '\'' || str[0] == '"' ||isPunctuator(str[0]) == true)
    {
        return false;
    }
    int i,len = strlen(str);
    //if length is one, validation is already completed, hence return true
    if (len == 1)
    {
        return true;
    }
        //identifier cannot contain special characters
    else
    {
        for (i = 1 ; i < len ; i++)
        {
            if (isPunctuator(str[i]) == true)
            {
                return false;
            }
        }
    }
    return true;
}

bool isOperator(char ch)							//check if the given character is an operator or not
{
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '<' || ch == '>' || ch == '&' ||
        ch == '.' || ch == '@' || ch == '/' ||
        ch == ':' || ch == '=' || ch == '~' ||
        ch == '|' || ch == '$' || ch == '!' ||
        ch == '#' || ch == '%' || ch == '^' ||
        ch == '_' || ch == '[' || ch == ']' ||
        ch == '{' || ch == '}' || ch == '"' ||
        ch == '`' || ch == '?' || ch == '(' ||
        ch == ')'
            )
    {
        return true;
    }
    return false;
}

bool isKeyword(char *str)						//check if the given substring is a keyword or not
{
    if (!(strcmp(str, "let")) || (!strcmp(str, "in")) ||
        (!strcmp(str, "fn")) || (!strcmp(str, "where")) ||
        (!strcmp(str, "aug")) || (!strcmp(str, "or")) ||
        (!strcmp(str, "not")) || (!strcmp(str, "gr")) ||
        (!strcmp(str, "ge")) || (!strcmp(str, "ls")) ||
        (!strcmp(str, "le")) || (!strcmp(str, "eq")) ||
        (!strcmp(str, "ne")) || (!strcmp(str, "true")) ||
        (!strcmp(str, "false")) || (!strcmp(str, "nil")) ||
        (!strcmp(str, "dummy")) || (!strcmp(str, "within")) ||
        (!strcmp(str, "and")) || (!strcmp(str, "rec")) ||
        (!strcmp(str, ".")))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isNumber(char* str)							//check if the given substring is a number or not
{
    int len = strlen(str),numOfDecimal = 0;
    if (len == 0)
    {
        return false;
    }
    if (str[0] == '0' && len != 1)
    {
        return false;
    }
    for (int i = 0 ; i < len ; i++)
    {
        if (numOfDecimal > 1 && str[i] == '.')
        {
            return false;
        } else if (numOfDecimal <= 1)
        {
            numOfDecimal++;
        }
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' || (str[i] == '-' && i > 0))
        {
            return false;
        }
    }
    return true;
}

char* subString(char* realStr, int l, int r)				//extract the required substring from the main string
{
    int i;

    char* str = (char*) malloc(sizeof(char) * (r - l + 2));

    for (i = l; i <= r; i++)
    {
        str[i - l] = realStr[i];
        str[r - l + 1] = '\0';
    }
    return str;
}


vector<Token> lexicalAnalyzer(char* str)						//parse the expression
{
    int left = 0, right = 0;
    int len = strlen(str);
    vector<Token> tokens;
    while (right <= len && left <= right) {
        if (str[right] == '\\' && str[right + 1] == '\\')
        {
            right += 2;
            string txt = "";
            while (right <= len && str[right] != '\n')
            {
                txt += str[right];
                right++;
            }
            Token token {COMMENT, txt};
            tokens.push_back(token);
            right++;
            left=right;
        }

        if (str[right] == '-' && str[right + 1] == '>')
        {
            string txt(1, str[right]);
            txt += str[right + 1];
            Token token {OPERATOR, txt};
            tokens.push_back(token);
            right += 2;
            left=right;
        }

        if (isPunctuator(str[right]) == false && (str[right] == '\'' || str[right] == '"')) // detect strings
        {
            right++;
            string txt = "";
            while (right <= len && str[right] != '\'' )
            {
                txt += str[right];
                right++;
            }
            Token token {STRING, txt};
            tokens.push_back(token);
            right++;
            left=right;
        }

        if (isPunctuator(str[right]) == false)			//if character is a digit or an alphabet
        {
            right++;
        }

        if (isPunctuator(str[right]) == true && left == right)		//if character is a punctuator
        {
            if (isOperator(str[right]) == true)
            {
                string  character = string(1, str[right]);
                Token token {OPERATOR, character};
                tokens.push_back(token);
            }
            right++;
            left = right;
        } else if (isPunctuator(str[right]) == true && left != right
                   || (right == len && left != right)) 			//check if parsed substring is a keyword or identifier or number
        {
            char* sub = subString(str, left, right - 1);   //extract substring

            if (isKeyword(sub) == true)
            {
//                cout<< sub <<" IS A KEYWORD\n";
                Token token {KEYWORD, sub};
                tokens.push_back(token);
            }
            else if (isNumber(sub) == true)
            {
//                cout<< sub <<" IS A NUMBER\n";
                Token token {NUMBER, sub};
                tokens.push_back(token);
            }
            else if (validIdentifier(sub) == true
                     && isPunctuator(str[right - 1]) == false)
            {
//                cout<< sub <<" IS A VALID IDENTIFIER\n";
                Token token {IDENTIFIER, sub};
                tokens.push_back(token);
            }
            else if (validIdentifier(sub) == false
                     && isPunctuator(str[right - 1]) == false)
            {
                cout<< sub <<" IS NOT A VALID IDENTIFIER\n";
            }

            left = right;
        }
    }
    return tokens;
}

int main()
{
    cout << "Enter input: " << endl;

    string input;
    getline(cin, input);  // Read input from the user

    // Convert string to char*
    char* inputChar = new char[input.length() + 1];
    strcpy(inputChar, input.c_str());
    vector<Token> tokens = lexicalAnalyzer(inputChar);
    for (const auto& token : tokens) {
        cout << "Type: " << token.type << ", Lexeme: " << token.lexeme << endl;
    }

    return 0;
}