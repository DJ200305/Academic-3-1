#ifndef SCOPE_TABLE
#define SCOPE_TABLE
#include<bits/stdc++.h>
#include "2105084_sdbm_hash.hpp"
#include "2105084_symbol_info.hpp"
using namespace std;
class ScopeTable{
    int id;
    int bucket_size;
    SymbolInfo **sytype;
    ScopeTable *parent_scope;
    unsigned int (*hashfunc)(const string,unsigned int);
    double collision_count = 0;
    public:
        ScopeTable(int id, int bucket_size,unsigned int (*hashfunc)(const string,unsigned int)){
            this->id = id;
            this->bucket_size = bucket_size;
            this->parent_scope = nullptr;
            this->hashfunc = hashfunc;
            sytype = new SymbolInfo*[bucket_size];
            for(int i=0; i<bucket_size; i++){
                sytype[i] = nullptr;
            }
        }
        ~ScopeTable(){
            for(int i=0; i<bucket_size; i++){
                if(sytype[i] != nullptr){
                    delete sytype[i];
                }
            }
            delete[] sytype;
        }
        int getId(){
            return id;
        }
        int getBucketSize(){
            return bucket_size;
        }
        void setParentScope(ScopeTable* parent_scope){
            this->parent_scope = parent_scope;
        }
        ScopeTable* getParentScope(){
            return parent_scope;
        }
        int get_index(string name){
            unsigned int hash = hashfunc(name,bucket_size);
            return (hash) % bucket_size;
            
        }
        int get_index_in_chain(string name){
            int got = -1;
            int index = get_index(name);
            SymbolInfo* temp = sytype[index];
            int i = 0;
            while(temp != nullptr){
                if(temp->getName() == name){
                    got = i;
                    break;
                }
                temp = temp->next;
                i++;
            }   
            return got;
        }
        bool insert(string name,string type){
            int ind = get_index(name);
            if(sytype[ind] == nullptr){
                
                sytype[ind] = new SymbolInfo(name,type);
                //cout<<"Inserted in ScopeTable# "<<this->getId()<<" at position "<<ind<<","<<this->getId()<<endl;
                return true;
            }
            else{
                long long int cnt = 0;
                SymbolInfo * temp = sytype[ind];
                if(temp->getName() == name){
                    return false;
                }
                while(temp->next != nullptr){
                    if(temp->getName() == name){
                        cnt = 0;
                        //cout<<"Already available in ScopeTable#"<<this->getId()<<" at position "<<ind<<","<<this->getId()<<endl;
                        return false;
                    }
                    temp = temp->next;
                }
                if(temp->getName() == name){
                    return false;
                }
                temp->next = new SymbolInfo(name,type);
                return true;
            }
        }
        double get_col_count(){
            for(int i=0;i<bucket_size;i++){
                if(sytype[i] == nullptr) continue;
                SymbolInfo* temp = sytype[i];
                temp = temp->next;
                while(temp != nullptr){
                    collision_count++;
                    temp = temp->next;
                }
            }
            return collision_count/bucket_size;
        }
        SymbolInfo* lookUp(string symbol){
            int ind = get_index(symbol);
            if(sytype[ind] == nullptr){
                return nullptr;
            }
            else{
                SymbolInfo* temp = sytype[ind];
                while(temp != nullptr){
                    if(temp->getName() == symbol){
                        return temp;
                    }
                    temp = temp->next;
                }
                return nullptr;
            }
        }
        bool Delete(string symbol){
            int ind = get_index(symbol);
            if(sytype[ind] == nullptr){
                //cout<<"Not found in current ScopeTable"<<endl;
                return false;
            }
            else{
                SymbolInfo* temp = sytype[ind];
                SymbolInfo* prev = nullptr;
                while(temp != nullptr){
                    if(temp->getName() == symbol){
                        if(temp->next == nullptr){
                            delete temp;
                            sytype[ind] = nullptr;
                            //cout<<"Found at ScopeTable#"<<this->getId()<<" at position "<<ind<<","<<this->getId()<<"and deletion done"<<endl;
                            return true;
                        }
                        else{
                            if(prev == nullptr){
                                sytype[ind] = temp->next;
                                delete temp;
                                //cout<<"Found at ScopeTable#"<<this->getId()<<" at position "<<ind<<","<<this->getId()<<"and deletion done"<<endl;
                                return true;
                            }
                            else{
                                prev->next = temp->next;
                                delete temp;
                                //cout<<"Found at ScopeTable#"<<this->getId()<<" at position "<<ind<<","<<this->getId()<<"and deletion done"<<endl;
                                return true;
                            }
                        }

                    }
                    prev = temp;
                    temp = temp->next;
                }
                //cout<<"Not found in current ScopeTable"<<endl;
                return false;
            }
        }
        void print(int indent = 1){
            for(int i=0;i<indent;i++){
                cout<<"\t";
            }
            cout<<"ScopeTable#"<<" "<<this->getId()<<endl;
            for(int i=0;i<bucket_size;i++){
                for(int j=0;j<indent;j++){
                    cout<<"\t";
                }
                cout<<(i+1)<<"--> ";
                SymbolInfo* temp = sytype[i];
                while(temp != nullptr){
                    cout<<"<"<<temp->getName()<<","<<temp->getType()<<"> ";
                    temp = temp->next;
                }
                cout<<endl;
            }
        }
};
#endif