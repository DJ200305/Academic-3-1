#include<bits/stdc++.h>
#include "2105084_symbol_table.hpp"
using namespace std;
int main(int argc, char* argv[]){
    unsigned int (*hashfunc)(const string,unsigned int);
    
    string in,out,hashfunc_name = "SDBM";
    if(argc == 4){
        in = argv[2];
        out = argv[3];
        hashfunc_name = argv[1];
        freopen(in.c_str(), "r", stdin);
        freopen(out.c_str(), "w", stdout);
        if(hashfunc_name == "BKDR"){
            hashfunc = Hash::BKDRHash;
        }
        else if(hashfunc_name == "DJB"){
            hashfunc = Hash::DJBHash;
        }
        else if(hashfunc_name == "SDBM"){
            hashfunc = Hash::SDBMHash;
        }
        else{
            hashfunc = Hash::SDBMHash;
        }
    }
    else if(argc == 3){
        in = argv[1];
        out = argv[2];
        freopen(in.c_str(), "r", stdin);
        freopen(out.c_str(), "w", stdout);
        hashfunc = Hash::SDBMHash;
    }
    else{
        cout<<"Invalid number of arguments"<<endl;
        return 0;
    }

    int n;
    cin>>n;
    cout<<"\t";
    SymbolTable* st = new SymbolTable(n,hashfunc);
    cin.ignore();
    string line;
    int command = 0;
    double ratio = 0.0;
    while(getline(cin,line)){
        istringstream iss(line);
        string cmd;
        iss>>cmd;
        command++;
        if(cmd == "I"){
            string symbol,type,temp;
            iss>>symbol;
            bool first = true;
            while (iss >> temp) {
               if (!first) type += " ";
               type += temp;
               first = false;
            }  
            cout<<"Cmd "<<command<<": "<<line<<endl;
            cout<<"\t";
            if(type == ""){
                cout<<"Number of parameters mismatch for the command I"<<endl;
                continue;
            }
            bool done = st->Insert(symbol,type);
            if(done){
                cout<<"Inserted in ScopeTable# "<<st->curr->getId()<<" at position "<<st->curr->get_index(symbol)+1<<", "<<st->curr->get_index_in_chain(symbol)+1<<endl;
            }
            else{
                cout<<"'"<<symbol<<"'"<<" already exists in the current ScopeTable"<<endl;
            }
        }
        else if(cmd == "D"){
            string symbol,extra;
            iss>>symbol;
            cout<<"Cmd "<<command<<": "<<line<<endl;
            cout<<"\t";
            if(symbol == ""){
                cout<<"Number of parameters mismatch for the command D"<<endl;
                continue;
            }
            else if(iss>>extra){
                cout<<"Number of parameters mismatch for the command D"<<endl;
                continue;
            }
            int ind = st->curr->get_index(symbol);
            int ind_chain = st->curr->get_index_in_chain(symbol);
            bool done = st->Remove(symbol);
            if(done){
                cout<<"Deleted "<<"'"<<symbol<<"'"<<" from ScopeTable# "<<st->curr->getId()<<" at position "<<ind+1<<", "<<ind_chain+1<<endl;
            }
            else{
                cout<<"Not found in the current ScopeTable"<<endl;
            }
        }
        else if(cmd == "L"){
            string symbol,extra;
            iss>>symbol;
            cout<<"Cmd "<<command<<": "<<line<<endl;
            cout<<"\t";
            if(symbol == ""){
                cout<<"Number of parameters mismatch for the command L"<<endl;
                continue;
            }
            else if(iss>>extra){
                cout<<"Number of parameters mismatch for the command L"<<endl;
                continue;
            }
            SymbolInfo* sy = st->Lookup(symbol);
        }
        else if(cmd == "P"){
            string type,extra;
            iss>>type;
            cout<<"Cmd "<<command<<": "<<line<<endl;
            if(iss>>extra){
                cout<<"Number of parameters mismatch for the command P"<<endl;
                continue;
            }
            //cout<<"in1\n";
            if(type == "A"){
                st->PrintAllScope();
            }
            else if(type == "C"){
                st->PrintCurrScope();
            }
            else{
                cout<<"Number of parameters mismatch for the command P"<<endl;
                continue;
            }
            //st->curr->print();
        }
        else if(cmd == "E"){
            cout<<"Cmd "<<command<<": "<<cmd<<endl;
            cout<<"\t";
            st->ExitScope();
        }
        else if(cmd == "S"){
            cout<<"Cmd "<<command<<": "<<cmd<<endl;
            cout<<"\t";
            st->EnterScope();
        }
        else if(cmd == "Q"){
            cout<<"Cmd "<<command<<": "<<cmd<<endl;
            ratio = st->ratio();
            st->delAllScope();
        }
        else{
            break;
        }
    }
    freopen("2105084_report.txt", "a",stdout);
    cout<<"Hash Function used: "<<hashfunc_name<<endl;
    cout<<"Collision Ratio: "<<fixed << setprecision(4)<<ratio<<endl;
    cout<<"Hash Function source: ";
    if(hashfunc_name == "BKDR"){
        cout<<"From Brian Kernighan and Dennis Ritchies book: The C Programming Language"<<endl;
        cout<<endl;
    }
    else if(hashfunc_name == "DJB"){
        cout<<"An algorithm produced by Professor Daniel J. Bernstein and shown first to the world on the usenet newsgroup comp.lang.c"<<endl;
        cout<<endl;
    }
    else{
        cout<<"From Specification"<<endl;
        cout<<endl;
    }
}