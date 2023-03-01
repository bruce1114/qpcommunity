// #ifdef amir
// #ifdef candque
// #ifdef candset
// #ifdef REPORT
// #ifdef BAR
// #ifdef combine_time
// #ifdef outcore
// #ifdef moretime
// #ifdef cntCheckEdgeLayer
// #ifdef directcoreTotaltime
// #ifdef cntvisitnei
// #ifdef countcore
// #ifdef buildl_naive
// #ifdef qpcg_rule2_cnt
#define amir
#define moretime
#define qpcg_rule2_cnt
// #define cntvisitnei
// #define countcore
// #define directcoreTotaltime
// #define cntCheckEdgeLayer
// #define candset
// #define outcore
#include"head/def.hpp"

int main(){
    // filename,kval,sigma,percent,wait
    string filename;
    cin>>filename;
    // filename="smalldatasets/"+filename+".csv";
    filename="../dataset/"+filename+".csv";

    struct timeval start,end,astart,aend;
    
    gettimeofday(&start,NULL);
    Tempgraph tgraph;
    tgraph.readGraph(filename);
    gettimeofday(&end,NULL);
    tgraph.reportsize();
    // cout<<"read graph time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;


    int kval,sigma,percent;
    cin>>kval>>sigma>>percent;


    int wait=1;
    cin>>wait;
    
    //剪枝and构建静态图
    
    // cout<<"update maxlengthrev optimization"<<endl;
    // cout<<"getAnsFrmLnoRepeat"<<endl;



    gettimeofday(&astart,NULL);
    // gettimeofday(&start,NULL);
    tgraph.computeDegree();
    switch(wait){
    case 1:
        {
            // cout<<"computeWPT candidate"<<endl;
            tgraph.MPC_WPN(kval-1,sigma,percent);//k-clique的度数是k-1
        }
    break;
    case 2:
        tgraph.MPC_WPK(kval-1,sigma,percent);
    break;
    case 3:
        {
            // cout<<"mpc direct"<<endl;



            vector<pair<WEDS,unordered_set<int>>> fixlenAns;
            if(wait==3) tgraph.MPC_direct(kval,sigma,percent,fixlenAns);
            else if(wait==4) tgraph.MPC_direct_degeneracy(kval,sigma,percent,fixlenAns);
            else if(wait==5) tgraph.MPC_direct_noorder(kval,sigma,percent,fixlenAns);
            // tgraph.MPC_direct_firstlayer(kval,sigma,percent,fixlenAns);

            gettimeofday(&end,NULL);
            // cout<<"total mine fixlenpclique time direct: "<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<endl;
            cout<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<" ";
            // cout<<"total fixlenpclique number: "<<fixlenAns.size()<<endl;
            cout<<fixlenAns.size()<<" ";
            // cout<<"direct "<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<" "<<tgraph.computeTime<<endl;

            
            // cout<<"all pcliques:"<<endl;
            // vector<WEDS> allpcliques;
            // for(int i=0;i<fixlenAns.size();++i){
            //     vector<int> cliquenodes;
            //     unordered_set<int>& curclique=fixlenAns[i].second;
            //     unordered_set<int>::iterator it=curclique.begin();
            //     while(it!=curclique.end()){
            //         cliquenodes.push_back(*it);
            //         it++;
            //     }

            //     sort(cliquenodes.begin(),cliquenodes.end());
            //     WEDS combine=fixlenAns[i].first;
            //     combine.timeList.push_back(-1);
            //     for(int val:cliquenodes) combine.timeList.push_back(val);
            //     allpcliques.push_back(combine);
            // }
            // sort(allpcliques.begin(),allpcliques.end());

            // for(int i=0;i<allpcliques.size();++i){
            //     for(int j=0;j<allpcliques[i].timeList.size();++j){
            //         cout<<allpcliques[i].timeList[j];
            //         if(j!=allpcliques[i].timeList.size()-1) cout<<" ";
            //     }
            //     cout<<endl;
            // }



            // cout<<"computetimes: "<<tgraph.computeTime<<endl;

            //compute extre mapclique
            gettimeofday(&start,NULL);
            vector<Pcliques> fixLenPcliques;
            unordered_map<WEDS,int,hashWEDS> wtoi;
            int wtoicnt=0;
            for(int i=0;i<fixlenAns.size();++i){
                WEDS& theweds=fixlenAns[i].first;
                unordered_set<int>& theclique=fixlenAns[i].second;
                if(wtoi.find(theweds)==wtoi.end()){
                    Pcliques newpclique;
                    newpclique.weds=theweds;
                    fixLenPcliques.push_back(newpclique);
                    wtoi[theweds]=wtoicnt++;
                }
                fixLenPcliques[wtoi[theweds]].cliques.push_back(theclique);
            }
            vector<pair<WEDS,unordered_set<int>>> extremeAns;
            generateExtremeLenPcliques(kval,percent,fixLenPcliques,extremeAns);
            gettimeofday(&end,NULL);
            // cout<<"total extre mpclique: "<<extremeAns.size()<<endl;
            cout<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<" ";
            cout<<extremeAns.size()<<endl;

            // cout<<"all extreme len pcliques:"<<endl;
            // vector<WEDS> printans;
            // for(auto &lpclique:extremeAns){
            //     vector<int> cliquenodes;
            //     unordered_set<int>::iterator it=lpclique.second.begin();
            //     while(it!=lpclique.second.end()){
            //         cliquenodes.push_back(*it);
            //         it++;
            //     }

            //     sort(cliquenodes.begin(),cliquenodes.end());
            //     WEDS combine=lpclique.first;
            //     combine.timeList.push_back(-1);
            //     for(auto val:cliquenodes){
            //         combine.timeList.push_back(val);
            //     }

            //     printans.push_back(combine);
            // }

            // sort(printans.begin(),printans.end());

            // for(int i=0;i<printans.size();++i){
            //     for(int j=0;j<printans[i].timeList.size();++j){
            //         cout<<printans[i].timeList[j];
            //         if(j!=printans[i].timeList.size()-1) cout<<" ";
            //     }
            //     if(i!=printans.size()-1) cout<<endl;
            // }

            return 0;
        }
    break;
    case 4:
        {
            //pcore
            gettimeofday(&start,NULL);
            tgraph.MPC_WPN(kval,sigma,percent);

            gettimeofday(&astart,NULL);
            vector<pair<WEDS,vector<int>>> pcores;
            tgraph.computefixLenPkcores(kval,pcores);
            gettimeofday(&end,NULL);

            int lasttime=(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000;
            #ifdef outcore
            cerr<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<" ";
            #ifdef moretime
            cerr<<lasttime<<" ";
            #endif
            cerr<<pcores.size()<<endl;
            #endif

            #ifndef outcore
            cout<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<" ";
            #ifdef moretime
            cout<<lasttime<<" ";
            #endif
            cout<<pcores.size()<<endl;
            #endif

            #ifdef outcore
            vector<WEDS> printans;
            for(auto &pcore:pcores){
                WEDS combine=pcore.first;
                combine.timeList.push_back(-1);

                sort(pcore.second.begin(),pcore.second.end());
                for(auto val:pcore.second){
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
            #endif

            return 0;
        }
    break;
    case 5:
        {
            gettimeofday(&start,NULL);
            vector<pair<WEDS,vector<int>>> pcores;
            // tgraph.MPCO_direct(kval,sigma,percent,pcores);
            tgraph.MPCO_direct_1(kval,sigma,percent,pcores);
            gettimeofday(&end,NULL);

            #ifdef countcore
            cout<<"connected cores: "<<pcores.size()<<endl;
            #endif


            vector<pair<WEDS,vector<int>>> convertans;
            unordered_map<WEDS,vector<int>,hashWEDS> wedsgroup;
            for(auto &pcore:pcores){
                for(auto val:pcore.second){
                    wedsgroup[pcore.first].push_back(val);
                }
            }
            unordered_map<WEDS,vector<int>,hashWEDS>::iterator it=wedsgroup.begin();
            while(it!=wedsgroup.end()){
                convertans.push_back(pair<WEDS,vector<int>>(it->first,it->second));
                it++;
            }

            #ifdef outcore
            cerr<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<" "<<convertans.size()<<endl;
            #endif

            #ifndef outcore
            cout<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<" "<<convertans.size()<<endl;
            #endif

            #ifdef outcore
            vector<WEDS> printans;
            for(auto &pcore:convertans){
                WEDS combine=pcore.first;
                combine.timeList.push_back(-1);

                sort(pcore.second.begin(),pcore.second.end());
                for(auto val:pcore.second){
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
            #endif

            return 0;
        }
    break;
    case 6:
        {
            unordered_set<int> validnode;
            for(int i=0;i<tgraph.adjList[44803].size();++i){
                validnode.insert(tgraph.adjList[44803][i].first);
            }
            validnode.insert(44803);
            Staticgraph degraph;
            for(int i=0;i<tgraph.adjList.size();++i){
                if(validnode.find(i)==validnode.end()) continue;
                for(int j=0;j<tgraph.adjList[i].size();++j){
                    if(i<tgraph.adjList[i][j].first&&validnode.find(tgraph.adjList[i][j].first)!=validnode.end()) degraph.addedge(i,tgraph.adjList[i][j].first);
                }
            }

            vector<unordered_set<int>> holdcliques;
            degraph.enumClique(kval,holdcliques);


            //print
            vector<WEDS> allcliques;
            for(int i=0;i<holdcliques.size();++i){
                WEDS temp;
                unordered_set<int>& curclique=holdcliques[i];
                unordered_set<int>::iterator it=curclique.begin();
                while(it!=curclique.end()){
                    temp.timeList.push_back(*it);
                    it++;
                }
                sort(temp.timeList.begin(),temp.timeList.end());
                allcliques.push_back(temp);
            }
            sort(allcliques.begin(),allcliques.end());

            for(int i=0;i<allcliques.size();++i){
                for(int j=0;j<allcliques[i].timeList.size();++j){
                    cout<<allcliques[i].timeList[j];
                    if(j!=allcliques[i].timeList.size()-1) cout<<" ";
                }
                // if(i!=allpcliques.size()-1) cout<<endl;
                cout<<endl;
            }

            return 0;
        }
    break;
    case 7:
        {
            vector<pair<WEDS,vector<int>>> pcores;
            tgraph.MPCO_direct_cnt_total_qpcg(kval,sigma,percent,pcores);
            return 0;
        }
    break;
    }
    // gettimeofday(&end,NULL);
    // int temptime=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;


    //挖掘所有fixlenpclique
    gettimeofday(&start,NULL);
    vector<Pcliques> fixLenPcliques;
    tgraph.computefixLenPcliques(kval,fixLenPcliques);
    gettimeofday(&end,NULL);
    int lasttime=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
    // cout<<"mine fixlenpclique time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;
    // cout<<"total mine fixlenpclique time: "<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<endl;
    cout<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<" ";

    #ifdef moretime
    cout<<lasttime<<" ";
    #endif
    // cout<<"computetimes: "<<tgraph.computeTime<<endl;
    // temptime+=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;

    // cout<<"transf "<<(end.tv_sec-astart.tv_sec)*1000+(end.tv_usec-astart.tv_usec)/1000<<" "<<tgraph.computeTime<<endl;

    //debug
    // cout<<"all pcliques:"<<endl;
    // vector<WEDS> allpcliques;
    // for(int i=0;i<fixLenPcliques.size();++i){

    //     for(int j=0;j<fixLenPcliques[i].cliques.size();++j){

    //         vector<int> cliquenodes;
    //         // cout<<"clique "<<j<<": ";
    //         unordered_set<int>& curclique=fixLenPcliques[i].cliques[j];
    //         unordered_set<int>::iterator it=curclique.begin();
    //         while(it!=curclique.end()){
    //             cliquenodes.push_back(*it);
    //             it++;
    //         }
    //         // cout<<endl;
    //         sort(cliquenodes.begin(),cliquenodes.end());
    //         WEDS combine=fixLenPcliques[i].weds;
    //         combine.timeList.push_back(-1);
    //         for(int val:cliquenodes) combine.timeList.push_back(val);
    //         allpcliques.push_back(combine);
    //     }
    // }
    // sort(allpcliques.begin(),allpcliques.end());

    // for(int i=0;i<allpcliques.size();++i){
    //     for(int j=0;j<allpcliques[i].timeList.size();++j){
    //         cout<<allpcliques[i].timeList[j];
    //         if(j!=allpcliques[i].timeList.size()-1) cout<<" ";
    //     }
    //     // if(i!=allpcliques.size()-1) cout<<endl;
    //     cout<<endl;
    // }



    //组合所有极长mpclique
    gettimeofday(&start,NULL);
    vector<pair<WEDS,unordered_set<int>>> extremeAns;
    generateExtremeLenPcliques(kval,percent,fixLenPcliques,extremeAns);
    gettimeofday(&aend,NULL);
    // cout<<"mine extre mpclique time: "<<(aend.tv_sec-start.tv_sec)*1000+(aend.tv_usec-start.tv_usec)/1000<<endl;
    // #ifndef REPORT
    // cout<<"total mine extre mpclique time: "<<(aend.tv_sec-astart.tv_sec)*1000+(aend.tv_usec-astart.tv_usec)/1000<<endl;
    // #endif
    // cout<<"total extre mpclique: "<<extremeAns.size()<<endl;



    int totalfixlenpclique=0;
    for(Pcliques pclique:fixLenPcliques){
        totalfixlenpclique+=pclique.cliques.size();
    }
    // cout<<"total fixlenpclique: "<<totalfixlenpclique<<endl;
    cout<<totalfixlenpclique<<" "<<(aend.tv_sec-start.tv_sec)*1000+(aend.tv_usec-start.tv_usec)/1000<<" "<<extremeAns.size()<<endl;
    
    


    //debug
    // cout<<"all extreme len pcliques:"<<endl;
    // vector<WEDS> printans;
    // for(auto &lpclique:extremeAns){
    //     vector<int> cliquenodes;
    //     unordered_set<int>::iterator it=lpclique.second.begin();
    //     while(it!=lpclique.second.end()){
    //         cliquenodes.push_back(*it);
    //         it++;
    //     }

    //     sort(cliquenodes.begin(),cliquenodes.end());
    //     WEDS combine=lpclique.first;
    //     combine.timeList.push_back(-1);
    //     for(auto val:cliquenodes){
    //         combine.timeList.push_back(val);
    //     }

    //     printans.push_back(combine);
    // }
    // sort(printans.begin(),printans.end());
    // for(int i=0;i<printans.size();++i){
    //     for(int j=0;j<printans[i].timeList.size();++j){
    //         cout<<printans[i].timeList[j];
    //         if(j!=printans[i].timeList.size()-1) cout<<" ";
    //     }
    //     if(i!=printans.size()-1) cout<<endl;
    // }

    // int redundant=checkredundant(extremeAns);
    // cout<<"redundant num: "<<redundant<<endl;
    // cout<<"redundant ratio: "<<1.0*redundant/extremeAns.size()<<endl;


    return 0;

}