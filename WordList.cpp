#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <algorithm>

#include <iostream>
using namespace std;

unsigned int hash(const std::string& s);
unsigned int hash(const int& i);
unsigned int hash(const char& c);
class WordListImpl
{
    MyHash<std::string,std::vector<std::string>>* h;
    std::string patternize(std::string s) const;
    int isrepeat(std::string s, int x) const;
    std::string tolow(std::string s) const ;
public:
    WordListImpl();
    ~WordListImpl();
    bool loadWordList(std::string filename);
    bool contains(std::string word) const;
    std::vector<std::string> findCandidates(std::string cipherWord, std::string currTranslation) const;
private:
};
WordListImpl::WordListImpl()
{
    h=new MyHash<string,vector<string>>;
}
std::string WordListImpl::tolow(std::string s) const
{
    string d=s;
    for (int i=0;i<d.size();i++)
    {
        d[i]=tolower(d[i]);
    }
    
    return d;
}
WordListImpl::~WordListImpl()
{
    delete h;
}
int WordListImpl::isrepeat(std::string s, int x) const
{
    for (int i=0;i<s.size();i++)
    {
        if (tolower(s[i])==tolower(s[x]) && s[x]!='\'' && i<x)
            return i;
        
    }
    return -1;
}
std::string WordListImpl::patternize(std::string s) const
{
    int c=0;
    
    std::string d=s;
    std::string arr="ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";
    for (int i=0;i<d.size();i++)
    {
        if (isupper(d[i]))
            tolower(d[i]);
    }
    string n=d;
    
    for (int i=0;i<d.size();i++)
    {
        if (n[i]=='\'') //check this out
        {
            continue;
        }
        if (n[i]=='?')
        {
            c++;
            continue;
        }
        
        if (isrepeat(n,i)==-1)
        {
            d[i]=arr[c];
            c++;
        }
        else
        {
            int x=isrepeat(n,i);
            d[i]=d[x];
        }
        
        
    }
    
    return d;
}
bool WordListImpl::loadWordList(std::string filename)
{
    h->reset();
    
    
    std::ifstream File;
    std::string x;
    std::string sum;
    File.open(filename);
    if (!File)
    {
        return false;
    }
    while (File >> x)
    {
        sum = sum + x+" ";
    }
    
    Tokenizer g(" ,;:.!()[]{}-\"#$%^&");
    std::vector<std::string> d;
    d=g.tokenize(sum);
    
    for (int i=0;i<d.size();i++)
    {
        
        const std::string c=d[i];
        std::vector<std::string> * l=h->find(patternize(c));
        if (l==nullptr)
        {
            vector<std::string> v;
            v.push_back(c);
            h->associate(patternize(c),v);
            
        }
        else
        {
            
            l->push_back(c);
            
            h->associate(patternize(c),*l);
            
        }
        
    }
    return true;
}

bool WordListImpl::contains(std::string word) const
{
    string change=word;
    std::string wordc=word;
    for (int i=0;i<word.size();i++)
    {
        tolower(wordc[i]);
    }
    std::string n=patternize(wordc);
    
    std::vector<std::string> * l=h->find(n);
    if (l==nullptr)
        return false;
    
    std::vector<std::string>  c=*l;
    
    for (int i=0;i<c.size();i++)
    {
        
        if (tolow(c[i])==tolow(change))
            return true;
    }
    return false;
}

std::vector<std::string> WordListImpl::findCandidates(std::string cipherWord, std::string currTranslation) const
{
    std::string pattern =cipherWord;
    for (int i=0;i<pattern.size();i++)
    {
        if (isalpha(pattern[i]))
            tolower(pattern[i]);
    }
    std::string patter =currTranslation;
    for (int i=0;i<pattern.size();i++)
    {
        if (isalpha(patter[i]))
            tolower(patter[i]);
    }
    pattern=patternize(pattern);
    
    std::vector<std::string> * l=h->find(pattern);
    std::vector<std::string> d;
    
    if (l==nullptr)
        return d;
    d=*l;
    
    
    std::vector<std::string> ret;
    for (int i=0;i<d.size();i++)
    {
        
        bool ok=true;
        for (int j=0;j<patter.size();j++)
        {
            if (tolower(d[i][j])!=tolower(patter[j]) && patter[j]!='?')
            {
                ok=false;
                break;
            }
        }
        if (ok)
            ret.push_back(d[i]);
    }
    
    return ret;
}

//int main()
//{
//    WordList w;
//    w.loadWordList("/Users/nikhilsrikumar/Downloads/skeleton 4/wordlist.txt");
//    vector<string>s=w.findCandidates("dqupb", "??rst");
//    for (int i=0;i<s.size();i++)
//    {
//        cout<<s[i]<<endl;
//
//    }
//}
//int main()
//{
//    WordList w;
//    w.loadWordList("/Users/nikhilsrikumar/Downloads/skeleton 4/wordlist.txt");
//    cout<<(w.contains("dinner"));
//}

//***** hash functions for string, int, and char *****
unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(std::string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(std::string word) const
{
    return m_impl->contains(word);
}

std::vector<std::string> WordList::findCandidates(std::string cipherWord, std::string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}



