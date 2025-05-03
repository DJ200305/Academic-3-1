#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE
#include<bits/stdc++.h>
#include "2105084_scope_table.hpp"
#include "2105084_symbol_info.hpp"
using namespace std;
string preprocessing(string &str){
    int i=0;
    string temptype = "";
    while(true){
        if(str[i] == ' '){
            break;
        }
        temptype += str[i];
        i++;
    }
    str = str.substr(i+1,str.length());
    return temptype;
}
class SymbolTable{
    private:
        unsigned int bucket_size;
        int scope_id;
        unsigned int (*hashfunc)(const string,unsigned int);
        double col_cnt = 0;
        double scope_cnt = 0;
    public:
        ScopeTable * curr;
        SymbolTable(int bucket_size,unsigned int(*hashfunc)(const string,unsigned int)){
            this->bucket_size = bucket_size;
            this->curr = nullptr;
            this->scope_id = 0;
            this->hashfunc = hashfunc;
            EnterScope();
        }
        ~SymbolTable(){
            delAllScope();
        }
        double ratio(){
            return col_cnt/scope_cnt;
        }
        double get_total_col_count(){
            return col_cnt;
        }
        void EnterScope(){
            scope_cnt++;
            if(curr == nullptr){
                this->scope_id++;
                curr = new ScopeTable(this->scope_id,this->bucket_size,this->hashfunc);
                cout<<"ScopeTable# 1"<<" created"<<endl;
            }
            else{
                this->scope_id++;
                ScopeTable* temp = new ScopeTable(this->scope_id,this->bucket_size,this->hashfunc);
                temp->setParentScope(curr);
                curr = temp;
                cout<<"ScopeTable# "<<temp->getId()<<" created"<<endl;
            }
        }
        void ExitScope(){
            if(curr == nullptr){
                cout<<"No ScopeTable to exit"<<endl;
            }
            else{
                this->col_cnt += curr->get_col_count();
                
                ScopeTable* temp = curr->getParentScope();
                cout<<"ScopeTable# "<<curr->getId()<<" removed"<<endl;
                delete curr;
                curr = temp;
            }
        }
        void delAllScope(){
            while(curr != nullptr){
                //cout<<"ScopeTable# "<<curr->getId()<<" removed"<<endl;
                cout<<"\t";
                ExitScope();
            }
        }
        bool Insert(string symbol,string type){
            // SymbolInfo* sy = Lookup(symbol);
            // if(sy != nullptr){
            //     return false;
            // }
            int pos = type.find(' ');
            if(pos == string::npos){
                bool done = curr->insert(symbol,type);
                return done;
            }
            string temptype = preprocessing(type);
            if(temptype == "FUNCTION"){
                int i = 0;
                string sep = "";
                while(i < type.length()){
                    while(i < type.length() && type[i] == ' ') i++;
                    if(i >= type.length()) break;
                    while(i < type.length() && type[i] != ' '){
                        sep += type[i];
                        i++;
                    }
                    sep += ',';
                }
                int n = sep.length()-1;
                string returnType = "";
                string args = "";
                i = 0;
                for(i=0;i<n;i++){
                    if(sep[i] == ',') break;
                    returnType += sep[i];
                }
                int j = 0;
                for(j=sep.length()-1;j>=0;j--){
                    if(sep[j] == ',') break;
                }
                sep.erase(j,sep.length());
                args = sep.substr(i+1,sep.length());
                string type1 = temptype+","+returnType+"<=="+"("+args+")";
                bool done = curr->insert(symbol,type1);
                return done;
            }
            else if(temptype == "STRUCT" || temptype == "UNION"){
                bool sw = false;
                int p = 0;
                while(p < type.length() && isspace(type[p])) p++;
                type = type.substr(p);
                for(int i=0;i<type.length();i++){
                    if(!sw && type[i] == ' '){
                        type[i] = ',';
                        sw = true;
                    }
                    if(sw && type[i] == ' '){
                        type[i] = ';';
                        sw = false;
                    }
                }
                string struct_params = "";
                int i=0;
                while(i < type.length()){
                    struct_params += "(";
                    while(i < type.length() && type[i] != ';'){
                        struct_params += type[i];
                        i++;
                    }
                    if(i == type.length()-1){
                        struct_params += ")";
                    }
                    else{
                        struct_params += "),";
                    }
                    i++;
                }
                int j = 0;
                for(j=struct_params.length()-1;j>=0;j--){
                    if(struct_params[j] == ',') break;
                }
                struct_params.erase(j,struct_params.length());
                string type1 = temptype+","+"{"+struct_params+"}";
                bool done = curr->insert(symbol,type1);
                return done;
            }
            return false;
        }
        bool Remove(string symbol){
            return curr->Delete(symbol);
        }
        SymbolInfo* Lookup(string symbol){
            ScopeTable* temp = curr;
            SymbolInfo* sy = nullptr;
            while(temp != nullptr){
                sy = temp->lookUp(symbol);
                if(sy != nullptr){
                    cout<<"'"<<symbol<<"'"<<" found in ScopeTable# "<<temp->getId()<<" at position "<<temp->get_index(symbol)+1<<", "<<temp->get_index_in_chain(symbol)+1<<endl;
                    return sy;
                }
                temp = temp->getParentScope();
            }
            if(sy == nullptr){
                cout<<"'"<<symbol<<"'"<<" not found in any of the ScopeTables"<<endl;
            }
            return sy;
        }
        void PrintCurrScope(){
            //cout<<"in\n";
            curr->print();
        }
        void PrintAllScopeH(ScopeTable* temp,int indent){
            if(temp == nullptr){
                return;
            }
            temp->print(indent);
            PrintAllScopeH(temp->getParentScope(),indent+1);
        }
        void PrintAllScope(){
            PrintAllScopeH(curr,1);
        }
};
#endif