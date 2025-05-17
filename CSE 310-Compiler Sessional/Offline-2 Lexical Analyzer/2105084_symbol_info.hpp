#ifndef SYMBOL_INFO_HPP
#define SYMBOL_INFO_HPP
#include<bits/stdc++.h>
using namespace std;
class SymbolInfo{
    private:
        string name;
        string type;
    public:
    SymbolInfo* next;
    SymbolInfo(string name, string type){
        this->name = name;
        this->type = type;
        this->next = nullptr;
    }
    string getName(){
        return name;
    }
    string getType(){
        return type;
    }
};
#endif