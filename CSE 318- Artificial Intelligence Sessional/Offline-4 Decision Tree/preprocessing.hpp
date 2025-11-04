#include<bits/stdc++.h>
#include "onlinec2.hpp"    
using namespace std;

class Preprocessing{
    
public:
    static vector<Row>start(vector<Row>&dataset){
        int len = dataset[0].data.size();
        map<pair<string,int>,int>mp;
        for(int i=0;i<dataset.size();i++){
            for(int j=0;j<len;j++){
                if(dataset[i].data[j] == "?" || dataset[i].data[j] == " ?"){
                    continue;
                }
                mp[{dataset[i].data[j],j}]++;
            }
        }
        for(int i=0;i<dataset.size();i++){
            for(int j=0;j<len;j++){
                if(dataset[i].data[j] == "?"){
                    int mx = -1;
                    string mxval;
                    for(auto x:mp){
                        if(x.first.second == j && x.second > mx){
                            mx = x.second;
                            mxval = x.first.first;
                        }
                    }
                    dataset[i].data[j] = mxval;
                }
            }
        }
        return dataset;
    }
    static vector<Row>discretizeiris(vector<Row>&dataset){
        int len = dataset[0].data.size();
        for(int i=0;i<dataset.size();i++){
            string spl = dataset[i].data[0];
            double dspl = stod(spl);
            if(dspl >= 4.3 && dspl <= 5.5){
                dataset[i].data[0] = "smallsepallength";
            }
            else if(dspl > 5.5 && dspl <= 6.8){
                dataset[i].data[0] = "mediumsepallength";
            }
            else{
                dataset[i].data[0] = "largesepallength";
            }
        }
        for(int i=0;i<dataset.size();i++){
            string spl = dataset[i].data[1];
            double dspl = stod(spl);
            if(dspl >= 3 && dspl <= 3.27){
                dataset[i].data[1] = "smallsepalwidth";
            }
            else if(dspl > 3.28 && dspl <= 3.55){
                dataset[i].data[1] = "mediumsepalwidth";
            }
            else{
                dataset[i].data[1] = "largesepalwidth";
            }
        }
        for(int i=0;i<dataset.size();i++){
            string spl = dataset[i].data[2];
            double dspl = stod(spl);
            if(dspl >= 1 && dspl <= 2.77){
                dataset[i].data[2] = "smallpetallength";
            }
            else if(dspl > 2.78 && dspl <= 4.55){
                dataset[i].data[2] = "mediumpetallength";
            }
            else{
                dataset[i].data[2] = "largepetallength";
            }
        }
        for(int i=0;i<dataset.size();i++){
            string spl = dataset[i].data[3];
            double dspl = stod(spl);
            if(dspl >= 0.1 && dspl <= 0.64){
                dataset[i].data[3] = "smallpetalwidth";
            }
            else if(dspl > 0.64 && dspl <= 1.28){
                dataset[i].data[3] = "mediumpetalwidth";
            }
            else{
                dataset[i].data[3] = "largepetalwidth";
            }
        }
        return dataset;
    }
};