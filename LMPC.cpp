//用于极长极大pclique算法的验证
// #ifdef amir
// #ifdef candque
// #ifdef candset
// #ifdef REPORT
// #ifdef BAR
// #ifdef combine_time
#define amir
#include"head/def.hpp"

int main(){
    // filename,kval,sigma,percent
    string filename;
    // cout<<"filename?"<<endl;
    cin>>filename;
    // filename="smalldatasets/"+filename+".csv";
    filename="../dataset/"+filename+".csv";

    struct timeval start,end,astart,aend;
    
    gettimeofday(&start,NULL);
    Tempgraph tgraph;
    tgraph.readGraph(filename);
    gettimeofday(&end,NULL);
    tgraph.reportsize();
    // cerr<<"read graph time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;

    int kval,sigma,percent;
    // cout<<"kval sigma percent?"<<endl;
    cin>>kval>>sigma>>percent;

    vector<pair<WEDS,unordered_set<int>>> answer;
    // cerr<<"update maxlengthrev optimization"<<endl;
    gettimeofday(&astart,NULL);
    tgraph.computeDegree();
    do{
        cerr<<"sigma="<<sigma<<endl;
        tgraph.MPC_WPN(kval-1,sigma,percent);

        vector<Pcliques> fixLenPcliques;
        gettimeofday(&start,NULL);
        tgraph.computefixLenPcliques(kval,fixLenPcliques);
        gettimeofday(&end,NULL);
        cerr<<"mine fixlenpclique time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;
        if(fixLenPcliques.size()==0) break;

        gettimeofday(&start,NULL);
        tgraph.discoverans(fixLenPcliques,answer,percent);
        gettimeofday(&end,NULL);
        cerr<<"discover ans time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;
        cerr<<"ans num: "<<answer.size()<<endl;
        tgraph.staticGraphs.clear();
        tgraph.WEDStoLayer.clear();
        tgraph.layerToWEDS.clear();
        tgraph.cntlayers=0;
        sigma++;
    }while(true);
    gettimeofday(&aend,NULL);
    cerr<<"lmpc baseline time: "<<(aend.tv_sec-astart.tv_sec)*1000+(aend.tv_usec-astart.tv_usec)/1000<<endl;

    vector<WEDS> printans;
    for(auto &lpclique:answer){
        vector<int> cliquenodes;
        unordered_set<int>::iterator it=lpclique.second.begin();
        while(it!=lpclique.second.end()){
            cliquenodes.push_back(*it);
            it++;
        }

        sort(cliquenodes.begin(),cliquenodes.end());
        WEDS combine=lpclique.first;
        for(auto val:cliquenodes){
            combine.timeList.push_back(val);
        }

        printans.push_back(combine);
    }

    sort(printans.begin(),printans.end());

    for(int i=0;i<printans.size();++i){
        for(int j=0;j<printans[i].timeList.size();++j){
            cout<<printans[i].timeList[j];
            if(j!=printans[i].timeList.size()-1) cout<<" ";
        }
        if(i!=printans.size()-1) cout<<endl;
    }
}