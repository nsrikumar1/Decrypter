#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class TokenizerImpl
{
    string sep;
    bool isstop(char i) const;
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
};

TokenizerImpl::TokenizerImpl(string separators)
{
    sep=separators;
}
bool TokenizerImpl::isstop(char d) const
{
    for (int i=0;i<sep.size();i++)
    {
        if (d==sep[i])
            return true;
    }
    return false;
}
vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> list;
    string help=s;
    int c=0;
    int ind=0;
    
    
    for (int i=0;i<s.size();i++)
    {
        if (i==0 && isstop(s[i]))
        {
            while (isstop(s[i]))
            {
                ind++;
                i++;
            }
            
            
            
        }
        if (i>=1 && isstop(s[i])&& isstop(s[i-1]))
        {
            ind++;
            
            continue;
            
        }
        else if (isstop(s[i]))
        {
            if (list.empty())
            {
                
                list.push_back(help.substr(ind,c));
                help=help.substr(c);
                c=0;
                
            }
            else
            {
                list.push_back(help.substr(1+ind,c-1));
                help=help.substr(c);
                c=0;
            }
            
        }
        else if (i==s.size()-1)
        {
            if (list.empty())
            {
                list.push_back(help.substr(ind,c+1));
                help=help.substr(c);
                c=0;
                
            }
            else
            {
                list.push_back(help.substr(1+ind,c));
                help=help.substr(c);
                c=0;
            }
            
        }
        
        
        c++;
    }
    return list;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}

