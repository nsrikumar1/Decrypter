#include "provided.h"
#include <string>
#include "MyHash.h"
#include <vector>
#include <iostream>
#include <cctype>
using namespace std;

class TranslatorImpl
{
    MyHash<char,char>* cur;
    MyHash<char,char>* curl;
    
    vector<MyHash<char,char>*> d;
    vector<MyHash<char,char>*> inlist;
    int i=0;
public:
    TranslatorImpl();
    ~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
};
TranslatorImpl::~TranslatorImpl()
{
    delete cur;
    delete curl;
    for (int i=0;i<d.size();i++)
    {
        delete d[d.size()-1];
        delete inlist[inlist.size()-1];
        d.pop_back();
        inlist.pop_back();
    }
    
}
TranslatorImpl::TranslatorImpl()
{
    cur=new MyHash<char,char>; //possibly add parentheses
    curl=new MyHash<char,char>; //possibly add parentheses
    string x="abcdefghijklmnopqrstuvwxyz";
    for (int i=0;i<x.size();i++)
    {
        
        cur->associate(tolower(x[i]),'?');
        curl->associate(tolower(x[i]),'?');
    }
}
bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    MyHash<char,char> *c=new MyHash<char,char>;  //possibly add parenteses
    MyHash<char,char> *cc=new MyHash<char,char>;
    
    string x="abcdefghijklmnopqrstuvwxyz";
    for (int i=0;i<x.size();i++)
    {
        cc->associate(tolower(x[i]),tolower(*curl->find(x[i])));
        c->associate(tolower(x[i]),tolower(*cur->find(x[i])));
    }
    
    
    if (ciphertext.size()!=plaintext.size())
    {
        delete cur;
        delete curl;
        cur=c;
        curl=cc;
        return false;
    }
    for (int i=0;i<plaintext.size();i++)
    {
        if (plaintext[i]=='\'' && ciphertext[i]=='\'')
            break;
        if (!isalpha(plaintext[i])||(!isalpha(ciphertext[i])))
        {
            delete cur;
            delete curl;
            cur=c;
            curl=cc;
            return false;
        }
    }
    for (int i=0;i<plaintext.size();i++)
    {
        if (c->find(tolower(ciphertext[i]))!=nullptr &&  *c->find(tolower(ciphertext[i]))!='?' && *c->find(tolower(ciphertext[i]))!=tolower(plaintext[i]))
        {
            delete cur;
            delete curl;
            cur=c;
            curl=cc;
            return false;
            
        }
        if (cc->find(tolower(plaintext[i]))!=nullptr &&  *cc->find(tolower(plaintext[i]))!='?' && *cc->find(tolower(plaintext[i]))!=tolower(ciphertext[i]))
        {
            delete cur;
            delete curl;
            cur=c;
            curl=cc;
            return false;
            
        }
    }
    for (int i=0;i<ciphertext.size();i++)
    {
        if (c->find(tolower(ciphertext[i]))!=nullptr && *c->find(tolower(ciphertext[i]))!='?' && *c->find(tolower(ciphertext[i]))!=tolower(plaintext[i]))
        {
            delete cur;
            delete curl;
            cur=c;
            curl=cc;
            return false;
            
        }
        else if ((cur->find(tolower(ciphertext[i]))!=nullptr && *cur->find(tolower(ciphertext[i]))!='?' && *cur->find(tolower(ciphertext[i]))!=tolower(plaintext[i])))
        {
            delete cur;
            delete curl;
            cur=c;
            curl=cc;
            
            return false;
        }
        else if (cur->find(tolower(ciphertext[i]))!=nullptr && *cur->find(tolower(ciphertext[i]))=='?')
        {
            if (*curl->find((tolower(plaintext[i])))!='?' && *curl->find((tolower(plaintext[i])))!=*cur->find((tolower(ciphertext[i]))))
            {
                
                delete cur;
                delete curl;
                cur=c;
                curl=cc;
                
                return false;
            }
            cur->associate(tolower(ciphertext[i]),tolower(plaintext[i]));
            curl->associate(tolower(plaintext[i]),tolower(ciphertext[i]));
        }
        
    }
    d.push_back(c);
    inlist.push_back(cc);
    return true;
}
bool TranslatorImpl::popMapping()
{
    string x="abcdefghijklmnopqrstuvwxyz";
    if (d.size()<=0)
        return false;
    for (int i=0;i<x.size();i++)
    {
        cur->associate(x[i],*d[d.size()-1]->find(x[i]));
        
        curl->associate(x[i],*inlist[inlist.size()-1]->find(x[i]));
        
    }
    delete d[d.size()-1];
    delete inlist[inlist.size()-1];
    d.pop_back();
    inlist.pop_back();
    
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string x="";
    for (int i=0;i<ciphertext.size();i++)
    {
        char* y=cur->find(tolower(ciphertext[i]));
        if (!isalpha(ciphertext[i]))
            x+=ciphertext[i];
        else if (y==nullptr)
            x+='?';
        else if (isupper(ciphertext[i]))
            x+=toupper(*cur->find(tolower(ciphertext[i])));
        else
            x+=*cur->find(ciphertext[i]);
    }
    return x;
}
//int main()
//{
//    Translator t;
//    if (t.pushMapping("ab","ba"))
//    cout<<t.getTranslation("1");
//    if (t.pushMapping("ac","ca"))
//    cout<<t.getTranslation("2");
//
//
//
//}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
