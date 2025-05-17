#ifndef SCOPE_TABLE
#define SCOPE_TABLE
#include<bits/stdc++.h>
#include "2105084_sdbm_hash.hpp"
#include "2105084_symbol_info.hpp"
using namespace std;
class ScopeTable{
    string id;
    int bucket_size;
    SymbolInfo **sytype;
    ScopeTable *parent_scope;
    string hashfunc;
    double collision_count = 0;
    public:
    ofstream& lout;
        ScopeTable(string id, int bucket_size,string hashfunc,ofstream& lout) : lout(lout){
            this->id = id;
            this->bucket_size = bucket_size;
            this->parent_scope = nullptr;
            this->hashfunc = hashfunc;
            //this->lout = lout;
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
        string getId(){
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
            if(this->hashfunc == "SDBM"){
                return (Hash::SDBMHash(name.c_str(),bucket_size)) % bucket_size;
            }
            return 0;
            
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
                    lout<<"< "<<name<<" : "<<type<<" >"<<" already exists in ScopeTable# "<<this->getId()<<" at position "<<ind<<", "<<cnt<<endl;
                    lout<<endl;
                    return false;
                }
                while(temp->next != nullptr){
                    if(temp->getName() == name){
                        lout<<"< "<<name<<" : "<<type<<" >"<<" already exists in ScopeTable# "<<this->getId()<<" at position "<<ind<<", "<<cnt<<endl;
                        lout<<endl;
                        return false;
                    }
                    temp = temp->next;
                    cnt++;
                }
                if(temp->getName() == name){
                    lout<<"< "<<name<<" : "<<type<<" >"<<" already exists in ScopeTable# "<<this->getId()<<" at position "<<ind<<", "<<cnt<<endl;
                    lout<<endl;
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
            
                lout<<"ScopeTable # "<<this->getId()<<endl;
            
            
            for(int i=0;i<bucket_size;i++){
                SymbolInfo* temp = sytype[i];
                if(temp != nullptr){
                    lout<<i<<" --> ";
                }
                while(temp != nullptr){
                    lout<<"< "<<temp->getName()<<" : "<<temp->getType()<<" >";
                    temp = temp->next;
                }
                if(sytype[i] != nullptr){
                    lout<<endl;
                }
            }
        }
};
#endif