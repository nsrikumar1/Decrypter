#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class DecrypterImpl
{
    WordList *w;
    Translator *t;
    Tokenizer *l;
    
    bool isfull(string s);
    int untranslated(string x);
    int isrepeat(std::string s, int x) const;
    bool beenchosen(vector<string> d,string s);
    vector<string> helper(string& s,vector<string>& d,vector<string>& a,vector<string>& b,bool& f, int &l,string &original);
public:
    ~DecrypterImpl();
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
};
int DecrypterImpl::isrepeat(std::string s, int x) const
{
    for (int i=0;i<s.size();i++)
    {
        if (tolower(s[i])==tolower(s[x]) && s[x]!='\'' && i<x)
            return i;
        
    }
    return -1;
}
bool DecrypterImpl::beenchosen(vector<string> d,string s)
{
    for (int i=0;i<d.size();i++)
    {
        if (s==d[d.size()-1])
            return true;
    }
    return false;
}
DecrypterImpl::DecrypterImpl()
{
    l=new Tokenizer(" ,;:.!()[]{}-\"#$%^&\"0123456789");
    t=new Translator();
    w=new WordList();
    
}
int DecrypterImpl::untranslated(string s)
{
    int c=0;
    for (int i=0;i<s.size();i++)
    {
        if(s[i]=='?')
            c++;
        
    }
    return c;
}
bool DecrypterImpl::load(string filename)
{
    return w->loadWordList(filename);
}
bool DecrypterImpl::isfull(string s)
{
    for (int i=0;i<s.size();i++)
    {
        if (s[i]=='?')
            return false;
    }
    return true;
}
DecrypterImpl::~DecrypterImpl()
{
    delete l;
    delete t;
    delete w;
    
}
vector<string> DecrypterImpl::helper(string &ciphertext,vector<string> &tokenized,vector<string> &chosen,vector<string> &solutions,bool&first,int& levels,string &original)
{
    vector<string> words;
    words=l->tokenize(ciphertext);
    
    if (!words.empty())
    {
        int max=0;
        string chose="";
        string untrans="";
        if (untranslated(ciphertext)==0)
        {
            for (int i=0;i<words.size();i++)
            {
                if (words[i].size()>max)
                {
                    max=words[i].size();
                    chose=words[i];
                    tokenized=l->tokenize(ciphertext);
                    untrans=chose;
                    original=ciphertext;
                    first=true;
                }
            }
            
        }
        else
        {
            for (int i=0;i<words.size();i++)
            {
                
                if (untranslated((words[i]))>max && !beenchosen(chosen,words[i]))
                {
                    max=untranslated((words[i]));
                    untrans=tokenized[i];
                    chose=words[i];
                }
            }
        }
        
        string trans= t->getTranslation(chose);
        
        vector<string> candidates;
        if (first)
            candidates=w->findCandidates(chose,trans);
        else
        {
            
            candidates=w->findCandidates(untrans,chose);
            
        }
        
        if (candidates.size()==0)
        {
            
            for (int i=0;i<chosen.size();i++)
            {
                chosen.pop_back();
            }
            
            return candidates;
        }
        //good till here
        
        for (int i=0;i<candidates.size();i++)
        {
            
            bool goodWord=true;
            
            
            
            if(t->pushMapping(untrans,candidates[i]))
            {
                
                string message=t->getTranslation(original);
                
                vector<string> fullwords=l->tokenize(message);
                if (isfull(message))
                {
                    
                    for (int j=0;j<fullwords.size();j++)
                    {
                        if (!w->contains(fullwords[j]))
                        {
                            goodWord=false;
                            t->popMapping();
                            break;
                        }
                        
                    }
                    if (goodWord)
                    {
                        
                        t->popMapping();
                        
                        solutions.push_back(message);
                        sort(solutions.begin(),solutions.end());
                    }
                    levels--;
                    
                }
                else
                {
                    
                    for (int j=0;j<fullwords.size();j++)
                    {
                        if (isfull(fullwords[j]) && !w->contains(fullwords[j]))
                        {
                            
                            goodWord=false;
                            
                            t->popMapping();
                            break;
                        }
                        
                    }
                    if (goodWord)
                    {
                        
                        
                        chosen.push_back(t->getTranslation(chose));
                        
                        first=false;
                        if (helper(message,tokenized,chosen,solutions,first,levels,original).size()==0)
                        {
                            t->popMapping();
                            continue;
                        }
                        if (levels!=0)
                        {
                            levels--;
                            t->popMapping();
                        }
                        
                    }
                }
                
            }
            
            
            if (i==candidates.size())
            {
                vector<string> empty;
                t->popMapping();
                return empty;
            }
            
            
        }
        
        
    }
    
    
    
    return solutions;
}
vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    bool first;
    int levels;
    string original;
    vector <string> tokenized;
    vector <string> solutions;
    vector <string> chosen;
    string cipher=ciphertext;
    
    
    return helper(cipher,tokenized,chosen,solutions,first,levels,original);
}

//int main()
//{
//    Decrypter d;
//    d.load("/Users/nikhilsrikumar/Downloads/skeleton 4/wordlist.txt");
//    vector<string> l=d.crack("  'cd");
//
//    for (int i=0;i<l.size();i++)
//        cout<<l[i]<<endl;
//
//
//
//
//
//}
//int main()
//{
//    Tokenizer t(" ,;:.!()[]{}-\"#$%^&");
//    vector<string> k=t.tokenize("-a ");
//    cout<<k[0]<<'l';
//    //<<" "<<k[1]<<" "<<k[2];
//
//}
//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}


