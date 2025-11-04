#ifndef ONLINEC2_HPP
#define ONLINEC2_HPP
#include <bits/stdc++.h>
using namespace std;
class Row{
   public:
    vector<string> data;
    string output;
    Row(vector<string>d,string o){
        data = d;
        output = o;
    }
};
double entropy(vector<Row>&dataset){
    map<string,double>class_count;
    for(auto x:dataset){
        class_count[x.output]++;
    }
    double ent = 0.0;
    for(auto x:class_count){
        double p = x.second/dataset.size();
        ent += p*log2(p);
    }
    return ent*(-1.0);
}
double attribute(vector<Row>&dataset,int index){
   
    map<string,vector<Row>>parts;
    for(int i=0;i<dataset.size();i++){
        parts[dataset[i].data[index]].push_back(dataset[i]);
    }
    double ent = 0.0;
    for(auto x:parts){
        double p = x.second.size() / (double)dataset.size();
        ent += p * entropy(x.second);
    }
    return ent;
}
double giniindex(vector<Row>&dataset){
    map<string,double>class_count;
    for(auto x:dataset){
        class_count[x.output]++;
    }
    double res = 0.0;
    for(auto x:class_count){
        double p = x.second/dataset.size();
        res += p*p;
    }
    return 1-res;
}
double giniformula(vector<Row>&dataset,int ind){
    map<string,vector<Row>>parts;
    for(int i=0;i<dataset.size();i++){
        parts[dataset[i].data[ind]].push_back(dataset[i]);
    }
    double ent = 0.0;
    for(auto x:parts){
        double p = x.second.size() / (double)dataset.size();
        ent += p * giniindex(x.second);
    }
    return ent;
}
double informationGain(vector<Row>&row,int index){
    double entropy1 = entropy(row);
    double entropy2 = attribute(row, index);
    return entropy1 - entropy2;
}
double helperigr(vector<Row>&dataset,int ind){
    map<string,double>class_count;
    for(auto x:dataset){
        class_count[x.data[ind]]++;
    }
    double ent = 0.0;
    for(auto x:class_count){
        double p = x.second/dataset.size();
        ent += p*log2(p);
    }
    return ent*(-1.0);
}
double helpernwig(vector<Row>&dataset,int ind){
    set<string>vals;
    for(auto x:dataset){
        vals.insert(x.data[ind]);
    }
    return (double)vals.size();
}
double IGR(vector<Row>&row,int ind){
    double sp = helperigr(row,ind);
    double ig = informationGain(row,ind);
    return (sp == 0.0) ? 0.0 : ig/sp;
}
double NWIG(vector<Row>&row,int ind){
    double ig = informationGain(row,ind);
    double dis_vals = helpernwig(row,ind);
    return (dis_vals == 0.0) ? 0.0 : (ig/log2(dis_vals+1))*(1-((dis_vals-1))/(row.size()));
}

class Node{
public:    
    bool isleaf;
    string output;
    int f_ind;
    map<string,Node*>children;
    Node(){
        isleaf = false;
        output = "";
        f_ind = -1;
    }
};
class DT{
    vector<Row>train,test;
    Node* root;
    int maxdepth;
    int pnode;
    int pdepth;
    string criteria;
    string most;
public:
    DT(vector<Row>train,vector<Row>test,int maxdepth,string criteria){
        this->train = train;
        this->test = test;
        root = new Node();
        this->maxdepth = maxdepth;
        this->pnode = 0;
        this->pdepth = 0;
        this->criteria = criteria;
        this->most = handling(train);
    }
    string handling(vector<Row>&dataset){
         map<string,double>class_count;
    for(auto x:dataset){
        class_count[x.output]++;
    }
    string out;
    int mx = -1;
    for(auto x:class_count){
        if(x.second > mx){
            mx = x.second;
            out = x.first;
        }
    }
    return out;
    }
    string maximum(vector<Row>&dataset){
        map<string,double>class_count;
    for(auto x:dataset){
        class_count[x.output]++;
    }
    string out;
    int mx = -1;
    for(auto x:class_count){
        if(x.second > mx){
            mx = x.second;
            out = x.first;
        }
    }
    return out;
}
    bool same(vector<Row>&dataset){
        string out = dataset[0].output;
        for(auto x:dataset){
            if(x.output != out){
                return false;
            }
        }
        return true;
    }
    Node* tree(vector<Row>&dataset,vector<int>&findex,int depth){
        pnode++;
        Node *node = new Node();
        if(findex.size() == 0 || same(dataset) || depth == maxdepth){
            node->isleaf = true;
            node->output = maximum(dataset);
            return node;
        }
        int mxfeature = -1;
        double mxgain = -1.0;
        for(int i=0;i<findex.size();i++){
            double gain;
            if(criteria == "IGR"){
                gain = IGR(dataset,findex[i]);
            } 
            else if(criteria == "NWIG"){
                gain = NWIG(dataset,findex[i]);
            }
            else if(criteria == "GINI"){
                gain = giniformula(dataset,findex[i]);
            }
            else{
                gain = informationGain(dataset,findex[i]);
            }
            if(gain > mxgain){
                mxgain = gain;
                mxfeature = findex[i];
            }
        }
        if(mxfeature == -1){
            node->isleaf = true;
            node->output = maximum(dataset);
            return node;
        }
        node->f_ind = mxfeature;
        map<string,vector<Row>>parts;
        for(auto x:dataset){
            parts[x.data[mxfeature]].push_back(x);
        }
        vector<int>rest;
        for(int i=0;i<findex.size();i++){
            if(findex[i] != mxfeature){
                rest.push_back(findex[i]);
            }
        }
        for(auto x:parts){
            Node *child = tree(x.second, rest,depth+1);
            node->children[x.first] = child;
        }
        return node;
    }
    Node* treewithoutpruning(vector<Row>&dataset,vector<int>&findex,int temp){
        pnode++;
        if(temp > pdepth){
            pdepth = temp;
        }
        Node *node = new Node();
        if(findex.size() == 0 || same(dataset)){
            node->isleaf = true;
            node->output = maximum(dataset);
            return node;
        }
        int mxfeature = -1;
        double mxgain = -1.0;
        for(int i=0;i<findex.size();i++){
            double gain;
            if(criteria == "IGR"){
                gain = IGR(dataset,findex[i]);
            } 
            else if(criteria == "NWIG"){
                gain = NWIG(dataset,findex[i]);
            } 
            else{
                gain = informationGain(dataset,findex[i]);
            }
            if(gain > mxgain){
                mxgain = gain;
                mxfeature = findex[i];
            }
        }
        if(mxfeature == -1){
            node->isleaf = true;
            node->output = maximum(dataset);
            return node;
        }
        node->f_ind = mxfeature;
        map<string,vector<Row>>parts;
        for(auto x:dataset){
            parts[x.data[mxfeature]].push_back(x);
        }
        vector<int>rest;
        for(int i=0;i<findex.size();i++){
            if(findex[i] != mxfeature){
                rest.push_back(findex[i]);
            }
        }
        for(auto x:parts){
            Node *child = treewithoutpruning(x.second, rest,temp+1);
            node->children[x.first] = child;
        }
        return node;
    }
    
    string predict(Node* node,Row &row){
        if(node->isleaf){
            return node->output;
        }
        string val = row.data[node->f_ind];
        if(node->children.find(val) != node->children.end()){
            return predict(node->children[val], row);
        }
        return most; 
    }
    Node* training(){
        vector<int>findex(train[0].data.size());
        for(int i=0;i<train[0].data.size();i++){
            findex[i] = i;
        }
        if(maxdepth == -1){
            root = treewithoutpruning(train, findex,0);
        } else {
            root = tree(train, findex,0);
        }
        return root;
    }
    vector<pair<string,string>>evaluation(vector<Row>&dataset){
        Node* root1 = training();
        vector<pair<string,string>>results;
        for(auto x:dataset){
            string pred = predict(root1, x);
            results.push_back(make_pair(x.output, pred));
        }
        return results;
    }
    double accuracy(){
        vector<pair<string,string>>results = evaluation(test);
        int correct = 0;
        for(auto x:results){
            if(x.first == x.second){
        
                correct++;
            }
            //cout<<x.first<<" "<<x.second<<endl;
        }

        return (double)correct / test.size();
    }
    int getdepth(){
        return pdepth;
    }
    int getnodes(){
        return pnode;
    }
};
#endif