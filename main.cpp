#define oracle
// #define candset
#define moretime
#define outputans
#define coreedge
#include"head/def.hpp"

void simpleOutput(vector<WEDS>& allpcliques){
    for(int i=0;i<allpcliques.size();++i){
        for(int j=0;j<allpcliques[i].timeList.size();++j){
            int val=allpcliques[i].timeList[j];
            if(val==-1) cout<<"#";
            else cout<<val;
            if(j!=allpcliques[i].timeList.size()-1) cout<<" ";
        }
        // if(i!=allpcliques.size()-1) cout<<endl;
        cout<<endl;
    }
}

void outputCoreEdge(vector<WEDS>& allpcores){
    for(int i=0;i<allpcores.size();++i){
        cout<<"t:"<<endl;
        int j;
        for(j=0;j<allpcores[i].timeList.size();++j){
            int val=allpcores[i].timeList[j];
            if(val==-1) break;
            else if(j!=0) cout<<" ";
            cout<<val;
        }
        ++j;
        cout<<endl<<"e:"<<endl;
        for(;j<allpcores[i].timeList.size();j+=2){
            cout<<allpcores[i].timeList[j]<<" "<<allpcores[i].timeList[j+1]<<endl;
        }
    }
}

int main(int argc, char *argv[]){
    // filename,kval,sigma,percent,wait
    string filename;
    filename=argv[1];
    // cin>>filename;
    // filename="smalldatasets/"+filename+".csv";
    filename="smalldatasets/smallgraphs/"+filename;

    struct timeval start,end,astart,aend;
    
    gettimeofday(&start,NULL);
    Tempgraph tgraph;
    tgraph.readGraph(filename);
    gettimeofday(&end,NULL);
    tgraph.reportsize();
    // cout<<"read graph time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;


    int kval,sigma,percent;
    // cin>>kval>>sigma>>percent;
    kval=stoi(argv[2]);
    sigma=stoi(argv[3]);
    percent=stoi(argv[4]);


    int wait=1;
    // cin>>wait;
    wait=stoi(argv[5]);

    gettimeofday(&astart,NULL);
    // gettimeofday(&start,NULL);
    tgraph.computeDegree();
    switch(wait){
    case 1:
        {
            tgraph.MPC_WPN(kval-1,sigma,percent);//k-clique的度数是k-1
            vector<Pcliques> fixLenPcliques;
            tgraph.computefixLenPcliques(kval,fixLenPcliques);
            gettimeofday(&end,NULL);
            cerr<<"total time: "<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<endl;
            int totalres=0;
            for(int i=0;i<fixLenPcliques.size();++i) totalres+=fixLenPcliques[i].cliques.size();
            cerr<<"total result: "<<totalres<<endl;

            #ifdef outputans
            cerr<<"all pcliques:"<<endl;
            vector<WEDS> allpcliques;
            for(int i=0;i<fixLenPcliques.size();++i){

                for(int j=0;j<fixLenPcliques[i].cliques.size();++j){

                    vector<int> cliquenodes;
                    // cout<<"clique "<<j<<": ";
                    unordered_set<int>& curclique=fixLenPcliques[i].cliques[j];
                    unordered_set<int>::iterator it=curclique.begin();
                    while(it!=curclique.end()){
                        cliquenodes.push_back(*it);
                        it++;
                    }
                    // cout<<endl;
                    sort(cliquenodes.begin(),cliquenodes.end());
                    WEDS combine=fixLenPcliques[i].weds;
                    combine.timeList.push_back(-1);
                    for(int val:cliquenodes) combine.timeList.push_back(val);
                    allpcliques.push_back(combine);
                }
            }
            sort(allpcliques.begin(),allpcliques.end());

            simpleOutput(allpcliques);
            #endif
        }
    break;
    case 2:
        {
            vector<pair<WEDS,unordered_set<int>>> fixlenAns;
            tgraph.MPC_direct(kval,sigma,percent,fixlenAns);
            // tgraph.MPC_direct_firstlayer(kval,sigma,percent,fixlenAns);

            gettimeofday(&end,NULL);
            cerr<<"total time: "<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<endl;
            // cout<<"total fixlenpclique number: "<<fixlenAns.size()<<endl;
            cerr<<"total result: "<<fixlenAns.size()<<endl;

            #ifdef outputans
            cerr<<"all pcliques:"<<endl;
            vector<WEDS> allpcliques;
            for(int i=0;i<fixlenAns.size();++i){
                vector<int> cliquenodes;
                unordered_set<int>& curclique=fixlenAns[i].second;
                unordered_set<int>::iterator it=curclique.begin();
                while(it!=curclique.end()){
                    cliquenodes.push_back(*it);
                    it++;
                }

                sort(cliquenodes.begin(),cliquenodes.end());
                WEDS combine=fixlenAns[i].first;
                combine.timeList.push_back(-1);
                for(int val:cliquenodes) combine.timeList.push_back(val);
                allpcliques.push_back(combine);
            }
            sort(allpcliques.begin(),allpcliques.end());

            simpleOutput(allpcliques);
            #endif
            return 0;
        }
    break;
    case 3:
        {
            //pcore
            tgraph.MPC_WPN(kval,sigma,percent);
            vector<pair<WEDS,vector<int>>> pcores;
            tgraph.computefixLenPkcores(kval,pcores);
            gettimeofday(&end,NULL);

            cerr<<"total time: "<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<endl;
            cerr<<"total result: "<<pcores.size()<<endl;

            #ifdef outputans
            vector<WEDS> printans;
            for(auto &pcore:pcores){
                WEDS combine=pcore.first;
                combine.timeList.push_back(-1);

                #ifndef coreedge
                sort(pcore.second.begin(),pcore.second.end());
                #endif
                for(auto val:pcore.second){
                    combine.timeList.push_back(val);
                }

                printans.push_back(combine);
            }

            sort(printans.begin(),printans.end());

            #ifdef coreedge
            outputCoreEdge(printans);
            #else
            simpleOutput(printans);
            #endif
            #endif

            return 0;
        }
    break;
    case 4:
        {
            vector<pair<WEDS,vector<int>>> pcores;
            // tgraph.MPCO_direct(kval,sigma,percent,pcores);
            tgraph.MPCO_direct_1(kval,sigma,percent,pcores);
            gettimeofday(&end,NULL);

            cerr<<"total time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;
            cerr<<"total result: "<<pcores.size()<<endl;

            #ifdef outputans
            vector<WEDS> printans;
            for(auto &pcore:pcores){
                WEDS combine=pcore.first;
                combine.timeList.push_back(-1);

                #ifndef coreedge
                sort(pcore.second.begin(),pcore.second.end());
                #endif
                for(auto val:pcore.second){
                    combine.timeList.push_back(val);
                }

                printans.push_back(combine);
            }

            sort(printans.begin(),printans.end());

            #ifdef coreedge
            outputCoreEdge(printans);
            #else
            simpleOutput(printans);
            #endif

            #endif

            return 0;
        }
    break;
    }
    // gettimeofday(&end,NULL);
    // int temptime=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;




    return 0;

}