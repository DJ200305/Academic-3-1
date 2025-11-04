#include<bits/stdc++.h>
#include "preprocessing.hpp"
#include "onlinec2.hpp"
using namespace std;
vector<Row>LoadFromCsv(string fname){
    vector<Row>rows;
    ifstream in(fname);
    int first = 0;
    int id = 0;
    if (!in.is_open()) {
        cerr << "Error opening file: " << fname << endl;
        return rows;
    }
    else{
        string line;
        while(getline(in,line)){
            if(first == 0){
                first++;
                continue;
            }
            stringstream ss(line);
            string value;
            vector<string> row;
            while(getline(ss,value,',')){
                if(id == 0){
                    id++;
                    continue;
                }
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                row.push_back(value);
            }
            id = 0;
            if(row.size() > 0){
                string output = row.back();
                row.pop_back();
                rows.push_back(Row(row,output));
            }
        }
    }
    return rows;
}
void write(vector<Row>&dataset,string fname){
    ofstream out(fname);
    if (!out.is_open()) {
        cerr << "Error opening file: " << fname << endl;
        return;
    }
    for(auto x:dataset){
        for(int i=0;i<x.data.size();i++){
            out<<x.data[i];
            if(i != x.data.size() - 1){
                out<<",";
            }
        }
        out<<","<<x.output<<endl;
    }
    out.close();
}
void writeresults(string fname,int depth,string criteria,int nodes,int mdepth,double acc){
    ofstream out(fname,ios::app);
    if(!out.is_open()){
        cout<<"Error opening file\n";
        return;
    }
    out<<"Criteria: "<<criteria<<", Max Depth: "<<depth<<endl;
    
        out<<"Node: "<<nodes<<", Depth: "<<mdepth<<endl;
    
    out<<"Avg Accuracy: "<<acc<<endl;
    out.close();
}
int main(int argc,char* argv[]){
    //vector<Row>dataset1 = LoadFromCsv("Datasets/Iris.csv");
    //vector<Row>dataset1 = LoadFromCsv("Datasets/adult.data");
    
    //vector<Row>dataset = Preprocessing::start(dataset1);
    //write(dataset,"Datasets/adult_processed.csv");
    //vector<Row>dataset = Preprocessing::discretizeiris(dataset1);
    //write(dataset,"Datasets/iris_discretized.csv");
    //vector<Row>dataset = LoadFromCsv("Datasets/iris_discretized.csv");
    vector<Row>dataset = LoadFromCsv("Datasets/iris_discretized.csv");
        vector<Row>train,test;
    int train_size = dataset.size() * 0.8;
    int test_size = dataset.size() - train_size;
    
    double avg = 0.0;
    double sum = 0.0;
    double sum_nodes = 0.0;
    double sum_depth = 0.0;
    
    for(int k=0;k<20;k++){
        vector<bool>ok(dataset.size(),false);
        srand(time(0));   
        for(int i=0;i<train_size;){
        int ind = rand() % dataset.size();
        if(!ok[ind]){
            train.push_back(dataset[ind]);
            ok[ind] = true;
            i++;
        }
    }
    for(int i=0;i<dataset.size();i++){
        if(!ok[i]){
            test.push_back(dataset[i]);
            ok[i] = true;
        }
    }
    int dpth = atoi(argv[2]);
    if(dpth == 0){
        dpth = -1;
    }
    DT dt(train,test,dpth,argv[1]);
    sum += dt.accuracy();
    sum_nodes += dt.getnodes();
    sum_depth += dt.getdepth();
    cout<<"Run "<<k+1<<": "<<endl;
    cout<<"Nodes: "<<dt.getnodes()<<endl;
    cout<<"Depth: "<<dt.getdepth()<<endl;
    train.clear();
    test.clear();
}
    cout<<sum_nodes<<endl;
    cout<<sum_depth<<endl;
    double avg_nodes = sum_nodes / 20.0;
    double avg_depth = sum_depth / 20.0;
    avg = sum / 20.0;
    cout<<avg<<endl;
    writeresults("results.csv",atoi(argv[2]),argv[1],avg_nodes,avg_depth,avg*(100.0));
    // cout<<"Training set:"<<endl;
    // for(auto x:train){
    //     for(auto y:x.data){
    //         cout<<y<<" ";
    //     }
    //     cout<<"-> "<<x.output<<endl;
    // }
    // cout<<"Testing set:"<<endl;
    // for(auto x:test){
    //     for(auto y:x.data){
    //         cout<<y<<" ";
    //     }
    //     cout<<"-> "<<x.output<<endl;
    // }
}