
#include <iostream>



template<typename KeyType, typename ValueType>

class MyHash

{
    
    struct Node
    {
        KeyType k;
        ValueType v;
        Node* next=nullptr;
        Node* prev=nullptr;
    };
    
    Node** hash;
    
    int n;
    
    int maxsize;
    
    double maxLoadFactor;
    
    
public:
    
    MyHash(double maxLoadFactor = 0.5)
    {
        if (maxLoadFactor>2.0)
            maxLoadFactor=2.0;
        if (maxLoadFactor<=0)
            maxLoadFactor=0.5;
        n=0;
        
        maxsize=100;
        
        this->maxLoadFactor=maxLoadFactor;
        
        hash=new Node*[maxsize];
        
        for (int i=0;i<maxsize;i++)
        {
            hash[i]=nullptr;
            
        }
    }
    
    ~MyHash()
    {
        for (int i=0;i<maxsize;i++)
        {
            if (hash[i]==nullptr)
                continue;
            Node *p=hash[i];
            while (p!=nullptr)
            {
                Node* g=p->next;
                delete p;
                p=g;
            }
        }
        delete []hash;
        
    }
    
    unsigned int getBucketNumber(const KeyType& key) const
    {
        unsigned int hash(const KeyType& k);
        unsigned int h=hash(key);
        return h % maxsize;
    }
    
    void reset()
    {
        for (int i=0;i<maxsize;i++)
        {
            if (hash[i]==nullptr)
                continue;
            Node *p=hash[i];
            while (p!=nullptr)
            {
                Node* g=p->next;
                
                delete p;
                p=nullptr;
                p=g;
                
            }
            
        }
        
        delete [] hash;
        maxsize=100;
        n=0;
        hash=new Node*[maxsize];
        
        for (int i=0;i<maxsize;i++)
        {
            hash[i]=nullptr;
            
        }
        
    }
    
    void associate(const KeyType& key, const ValueType& value)
    {
        
        int num=getBucketNumber(key);
        if (hash[num]==nullptr && maxLoadFactor*maxsize>=n)
        {
            
            hash[num]=new Node();
            hash[num]->k=key;
            hash[num]->v=value;
            hash[num]->prev=nullptr;
            hash[num]->next=nullptr;
            n++;
        }
        else if (hash[num]==nullptr && maxLoadFactor*maxsize<n)
        {
            
            hash[num]=new Node();
            hash[num]->k=key;
            hash[num]->v=value;
            hash[num]->prev=nullptr;
            hash[num]->next=nullptr;
            n++;
            Node** copy=hash;
            
            n=0;
            maxsize=maxsize*2;
            hash=new Node*[maxsize];
            for (int i=0;i<maxsize;i++)
            {
                hash[i]=nullptr;
            }
            Node *p;
            
            for (int i=0;i<maxsize/2;i++)
            {
                
                if (copy[i]==nullptr)
                    continue;
                p=copy[i];
                while (p!=nullptr)
                {
                    associate(p->k,p->v);
                    
                    p=p->next;
                }
            }
            
            for (int i=0;i<maxsize/2;i++)
            {
                if (copy[i]==nullptr)
                    continue;
                p=copy[i];
                while (p!=nullptr)
                {
                    Node* g=p->next;
                    delete p;
                    p=g;
                }
            }
            delete []copy;
            
        }
        else
        {
            
            while (hash[num]!=nullptr)
            {
                if (hash[num]->k==key)
                {
                    
                    
                    hash[num]->v=value;
                    while (hash[num]!=nullptr)
                    {
                        if (hash[num]->prev==nullptr)
                            break;
                        hash[num]=hash[num]->prev;
                    }
                    return;
                }
                if (hash[num]->next==nullptr)
                    break;
                hash[num]=hash[num]->next;
            }
            
            Node *g=new Node();
            g->k=key;
            g->v=value;
            g->prev=hash[num];
            g->next=nullptr;
            hash[num]->next=g;
            hash[num]=hash[num]->next;
            
            while (hash[num]!=nullptr)
            {
                if (hash[num]->prev==nullptr)
                    break;
                hash[num]=hash[num]->prev;
            }
            n++;
            Node** copy=hash;
            
            
            if (maxsize*maxLoadFactor<n)
            {
                
                n=0;
                maxsize=maxsize*2;
                hash=new Node*[maxsize];
                for (int i=0;i<maxsize;i++)
                {
                    hash[i]=nullptr;
                }
                Node *p;
                
                for (int i=0;i<maxsize/2;i++)
                {
                    
                    if (copy[i]==nullptr)
                        
                        continue;
                    
                    p=copy[i];
                    
                    while (p!=nullptr)
                    {
                        associate(p->k,p->v);
                        
                        p=p->next;
                    }
                }
                
                
                for (int i=0;i<maxsize/2;i++)
                {
                    if (copy[i]==nullptr)
                        continue;
                    p=copy[i];
                    while (p!=nullptr)
                    {
                        Node* g=p->next;
                        delete p;
                        p=g;
                    }
                }
                delete []copy;
                
                
            }
        }
        
    }
    int getNumItems() const
    {
        return n;
    }
    double getLoadFactor() const
    {
        return 1.0*n/maxsize;
    }
    const ValueType* find(const KeyType& key) const
    
    {
        
        int num=getBucketNumber(key);
        if (hash[num]==nullptr)
            return nullptr;
        
        Node* temp=hash[num];
        
        while (temp!=nullptr)
        {
            if (temp->k==key)
            {
                return &(temp->v);
            }
            temp=temp->next;
        }
        return nullptr;
    }
    
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
        
    }
    
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
};









