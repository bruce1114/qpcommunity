#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<queue>
#include<sys/time.h>
#define ll long long

using namespace std;

struct WEDS{
    int topDiff,botDiff;
    vector<int> timeList;

    bool operator ==(const WEDS& b)const{
        if(timeList.size()==b.timeList.size()){
            for(int i=0;i<timeList.size();++i){
                if(timeList[i]!=b.timeList[i]) return false;
            }
            return true;
        }else return false;
    }

    bool operator <(const WEDS& b)const{
        for(int i=0;i<timeList.size()&&i<b.timeList.size();++i){
            if(timeList[i]!=b.timeList[i]) return timeList[i]<b.timeList[i];
        }
        return timeList.size()<b.timeList.size();
    }

    void reportSelf(){
        for(int i=0;i<timeList.size();++i){
            cout<<timeList[i]<<" ";
        }
    }
};

struct hashWEDS{
    size_t operator ()(const WEDS& theWEDS)const{
        size_t res=0;
        hash<int> hasher=hash<int>();
        for(int i=0;i<theWEDS.timeList.size();++i){
            res=res^hasher(theWEDS.timeList[i]);
        }
        return res;
    }
};

class Pcliques{
public:
    WEDS weds;
    vector<unordered_set<int>> cliques;//在weds上的所有clique，这些clique无边权，所以用节点集表示
};

//参数fixLenPcliques不是引用，因为后面要在fixLenPcliques里继续存东西
void generateExtremeLenPcliques(int kval,int percent,vector<Pcliques> fixLenPcliques,vector<pair<WEDS,unordered_set<int>>>& ans);
int isSubset(unordered_set<int>& smallset,unordered_set<int>& bigset);
void collectAns(vector<Pcliques>& fixLenPcliques,int start,int end,unordered_map<WEDS,vector<int>,hashWEDS>& prefix,unordered_map<WEDS,vector<int>,hashWEDS>& suffix,vector<pair<WEDS,unordered_set<int>>>& ans,int percent);
//pre和suc需要等长且互为前后缀
bool isPeriodic(WEDS& pre,WEDS& suc,int percent);
void intersection(unordered_set<int>& aset,unordered_set<int>& bset,unordered_set<int>& anset);
void generateNewLength(int kval,vector<Pcliques>& fixLenPcliques,int start,int end,unordered_map<WEDS,vector<int>,hashWEDS>& prefix,unordered_map<WEDS,vector<int>,hashWEDS>& suffix,int percent);

//for check
int checkredundant(vector<pair<WEDS,unordered_set<int>>>& source);
//timelist是有序的
bool checkWEDSinTimelist(const WEDS& theweds,vector<int>& timelist);

class Staticgraph{
public:
    void addedge(int aid,int bid);
    void enumClique(int kval,vector<unordered_set<int>>& holdcliques);
    void bk_pivot(unordered_set<int> cand,unordered_set<int> curclique,unordered_set<int> X,vector<unordered_set<int>>& holdcliques,int kval);
    void enumKCore(int kval,vector<int>& holdkcores);
    void enumKCores(int kval,vector<vector<int>>& holdkcores);

    //debug
    void reportself();
    void reportsize(int& totalnode,int& totaledge);

    unordered_map<int,vector<int>> adjlist;
};

class Tempgraph{
public:
    unordered_map<int,unordered_map<int,int>> adjMatrix;
    vector<vector<pair<int,int>>> adjList;//pair<int,int> 分别是邻接点，时序边标号
    vector<vector<int>> edgePool;
    vector<unordered_set<int>> edgePoolTimeSet;
    int edgecnt;
    int nodecnt;
    int cntlayers;
    vector<bool> validv;
    vector<bool> valide;
    vector<int> degree;
    vector<Staticgraph> staticGraphs;
    unordered_map<WEDS,int,hashWEDS> WEDStoLayer;
    vector<WEDS> layerToWEDS;
    int computeTime;

    void readGraph(string filename);
    Tempgraph();
    void computeKcore(int kval);
    void computeDegree();
    void computeWPTque(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet);//这两个候选集实现方法不同
    void computeWPTset(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet);//这两个候选集实现方法不同
    void computeWPTOracle(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet);
    void computeWPTEDS(vector<int>& timeList,int kval,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet,unordered_map<int,int>& cntTimesOfTime);//percent=0时适用
    void buildOracle(int percent,vector<int>& timeSeq,unordered_map<int,vector<pair<int,pair<int,int> > > >& theL);
    void getAnsFrmLnoRepeat(unordered_map<int,vector<pair<int,pair<int,int> > > >& theL,int sigma,vector<int>& timeSeq,unordered_set<WEDS,hashWEDS>& holdSet);
    void updateMaxLengthRev(int startNode,vector<int>& positionInSingleL,vector<int>& positionInSingleRevL,vector<pair<int,pair<int, int>>>& singleRevL,vector<int>& maxLengthRev,vector<int>& maxLength);
    void getPartAnsViaDfs(int start,int needLen,vector<int>& timeSeq,vector<pair<int,pair<int,int> > >& theDAG,vector<int>& thePosInDAG,vector<int>& theMaxLength,vector<int>& curPath,vector<vector<int> >& partAns);
    void WPNCluster(int sigma,int percent,int kval,unordered_map<int,unordered_set<WEDS,hashWEDS>>& theNodeToWPT,unordered_map<int,unordered_map<int,vector<WEDS>>>& theNodeToIWPT,unordered_map<int,unordered_map<int,int> >& theNodeToKeyTimeDegree);
    void computeSingleWPT(int nodeId,int sigma,int percent,int kval,unordered_map<int,unordered_set<WEDS,hashWEDS>>& theNodeToWPT,unordered_map<int,unordered_map<int,int> >& nodeToKeyTimeDegree);
    void updateNodeWPT(int nodeid,int kval,unordered_map<int,unordered_set<WEDS,hashWEDS>>& theNodeToWPT,unordered_map<int,unordered_map<int,vector<WEDS>>>& theNodeToIWPT,unordered_map<int,unordered_map<int,int> >& nodeToKeyTimeDegree,int eid);
    void computeSingleIWPT(int nodeId,unordered_set<WEDS,hashWEDS>& theWPT,unordered_map<int,unordered_map<int,vector<WEDS>>>& theNodeToIWPT);

    void transformByNode(int kval,int sigma,unordered_map<int,unordered_set<WEDS,hashWEDS>>& theNodeToWPT);

    void computefixLenPcliques(int kval,vector<Pcliques>& fixLenPcliques);
    void computefixLenPkcores(int kval,vector<pair<WEDS,vector<int>>>& fixlenPkcores);

    void MPC_WPN(int kval,int sigma,int percent);
    void MPC_WPK(int kval,int sigma,int percent);

    void MPC_direct(int kval,int sigma,int percent,vector<pair<WEDS,unordered_set<int>>>& ans);
    void computeSingleWPTdirect(int nodeId,int sigma,int percent,int kval,unordered_set<WEDS,hashWEDS>& ans);
    void bk_pivot_direct(unordered_set<int> cand,unordered_set<int> curclique,unordered_set<int> X,vector<pair<WEDS,unordered_set<int>>>& holdpcliques,int kval,const WEDS& thelayer,unordered_set<int>& deleted);
    bool checkWEDSinNode(const WEDS& theweds,int thenode,int kval,unordered_set<int>& deleted);
    void MPC_direct_firstlayer(int kval,int sigma,int percent,vector<pair<WEDS,unordered_set<int>>>& ans);
    // bool checkWEDSinTimelist(const WEDS& theweds,int eid);
    void MPC_direct_degeneracy(int kval,int sigma,int percent,vector<pair<WEDS,unordered_set<int>>>& ans);
    void MPC_direct_noorder(int kval,int sigma,int percent,vector<pair<WEDS,unordered_set<int>>>& ans);

    void MPCO_direct(int kval,int sigma,int percent,vector<pair<WEDS,vector<int>>>& ans);
    void MPCO_direct_1(int kval,int sigma,int percent,vector<pair<WEDS,vector<int>>>& ans);
    void MPCO_direct_cnt_total_qpcg(int kval,int sigma,int percent,vector<pair<WEDS,vector<int>>>& ans);


    //debug
    void reportNodes(int kval);
    void reportTransformedGraph();
    void reportTempgraph();
    void reportsize();
    void reportdevalidsize(int kval);
    void reportstaticsize();
    void getAnsFrmLwithRepeat(unordered_map<int,vector<pair<int,pair<int,int> > > >& theL,int sigma,vector<int>& timeSeq,unordered_set<WEDS,hashWEDS>& holdSet);
    void getAnsFrmLwithOptimize(unordered_map<int,vector<pair<int,pair<int,int> > > >& theL,int sigma,vector<int>& timeSeq,unordered_set<WEDS,hashWEDS>& holdSet);

    //for baseline
    void discoverans(vector<Pcliques>& fixLenPcliques,vector<pair<WEDS,unordered_set<int>>>& ans,int percent);
    bool checkexistance(int timestamp,unordered_set<int>& theclique);
};

int gcd(int a,int b){
    if(b==0) return a;
    else return gcd(b,a%b);
}

struct Frac{
    int top,bot;
    Frac(int _top=1,int _bot=1):top(_top),bot(_bot){}

    bool operator == (const int& r) const{
        return top==r*bot;
    }

    bool operator <= (const int& r) const{
        return top<=r*bot;
    }

    bool operator >= (const int& r) const{
        return top>=r*bot;
    }

    Frac& operator = (const int& rval){
        top=rval;bot=1;
        return *this;
    }

    void simplify(){
        int gcdVal=gcd(top,bot);
        top/=gcdVal;
        bot/=gcdVal;
    }

    bool isInt(){
        return (top%bot)==0;
    }

    int getUp(){//向上取整
        int res=top/bot;
        if((top%bot)!=0) res++;
        return res;
    }

    int getDown(){//向下取整
        return top/bot;
    }

    string toString(){
        return string(to_string(top)+string("/")+to_string(bot));
    }
};

Tempgraph::Tempgraph(){
    edgecnt=0;
    nodecnt=0;
    cntlayers=0;
    computeTime=0;
}

void Staticgraph::reportself(){
    unordered_map<int, std::vector<int>>::iterator it=adjlist.begin();
    while(it!=adjlist.end()){
        int nodeid=it->first;
        for(int i=0;i<it->second.size();++i){
            int neiid=it->second[i];
            if(nodeid<neiid) cout<<nodeid<<" "<<neiid<<endl;
        }
        it++;
    }
}

void Staticgraph::reportsize(int& totalnode,int& totaledge){
    unordered_map<int,vector<int>>::iterator it=adjlist.begin();
    totalnode=totaledge=0;
    while(it!=adjlist.end()){
        totalnode++;
        totaledge+=it->second.size();
        it++;
    }
    totaledge/=2;
}

bool Tempgraph::checkexistance(int timestamp,unordered_set<int>& theclique){
    unordered_set<int>::iterator it=theclique.begin();
    while(it!=theclique.end()){
        unordered_set<int>::iterator that=it;
        that++;
        while(that!=theclique.end()){
            int eid=adjMatrix[*it][*that];
            vector<int>::iterator it=lower_bound(edgePool[eid].begin(),edgePool[eid].end(),timestamp);
            if(it==edgePool[eid].end()||(*it)!=timestamp) return false;
            that++;
        }
        it++;
    }
    return true;
}

void Tempgraph::discoverans(vector<Pcliques>& fixLenPcliques,vector<pair<WEDS,unordered_set<int>>>& ans,int percent){
    //先计算所有weds的差的上下界
    for(int i=0;i<fixLenPcliques.size();++i){
        WEDS& nowweds=fixLenPcliques[i].weds;
        nowweds.botDiff=nowweds.topDiff=nowweds.timeList[1]-nowweds.timeList[0];
        for(int j=1;j<nowweds.timeList.size()-1;++j){
            int curdiff=nowweds.timeList[j+1]-nowweds.timeList[j];
            if(nowweds.botDiff>curdiff) nowweds.botDiff=curdiff;
            if(nowweds.topDiff<curdiff) nowweds.topDiff=curdiff;
        }
    }

    for(Pcliques& pcliques:fixLenPcliques){
        for(unordered_set<int>& clique:pcliques.cliques){
            WEDS& curweds=pcliques.weds;
            int newtop=(100*curweds.botDiff+curweds.botDiff*percent)/100;
            int newbot=(100*curweds.topDiff+99+percent)/(100+percent);
            bool found=false;
            for(int i=newbot;i<=newtop;++i){
                int former=curweds.timeList[0]-i;
                int later=curweds.timeList[curweds.timeList.size()-1]+i;
                if(checkexistance(former,clique)||checkexistance(later,clique)){
                    found=true;
                    break;
                }
            }
            if(found==false){
                //是极长极大
                ans.push_back(pair<WEDS,unordered_set<int>>(curweds,clique));
            }
        }
    }
}

void Tempgraph::reportTempgraph(){
    for(int i=0;i<nodecnt;++i){
        for(int j=0;j<adjList[i].size();++j){
            int neiid=adjList[i][j].first;
            int eid=adjList[i][j].second;
            if(i>neiid) continue;

            cout<<"vertex: "<<i<<" "<<neiid<<endl;
            cout<<"timestamp: ";
            for(int k=0;k<edgePool[eid].size();++k){
                cout<<edgePool[eid][k]<<" ";
            }
            cout<<endl;
        }
    }
}

void Tempgraph::reportNodes(int kval){
    cout<<"existing nodes:"<<endl;
    for(int i=0;i<nodecnt;++i){
        if(validv[i]) cout<<i<<" ";
    }
    cout<<endl;
}

void Tempgraph::reportsize(){
    cerr<<"temporal graph size:"<<endl;
    cerr<<"total node: "<<nodecnt<<endl;
    cerr<<"total edge in de-temporal graph: "<<edgePool.size()<<endl;
    int tempedgecnt=0;
    for(int i=0;i<edgePool.size();++i) tempedgecnt+=edgePool[i].size();
    cerr<<"total temporal edge: "<<tempedgecnt<<endl;
}

void Tempgraph::reportdevalidsize(int kval){
    cerr<<"valid size:"<<endl;
    int totalnode=0,totaledge=0;
    for(int i=0;i<nodecnt;++i){
        if(validv[i]) totalnode++;
    }

    for(int i=0;i<nodecnt;++i){
        if(validv[i]==false) continue;

        for(int j=0;j<adjList[i].size();++j){
            pair<int,int> neighbor=adjList[i][j];
            if(validv[neighbor.first]==false||neighbor.first<i) continue;
            totaledge++;
        }
    }
    cerr<<"total node: "<<totalnode<<endl;
    cerr<<"total edge: "<<totaledge<<endl;
}

void Tempgraph::reportstaticsize(){
    cerr<<"static size:"<<endl;
    int totalnode=0,totaledge=0;
    for(int i=0;i<staticGraphs.size();++i){
        int tempnode,tempedge;
        staticGraphs[i].reportsize(tempnode,tempedge);
        totalnode+=tempnode;
        totaledge+=tempedge;
    }
    cerr<<"totalnode: "<<totalnode<<endl;
    cerr<<"totaledge: "<<totaledge<<endl;
}

void Tempgraph::reportTransformedGraph(){
    for(int i=0;i<cntlayers;++i){
        cout<<"layer weds: ";
        WEDS layerweds=layerToWEDS[i];
        for(int j=0;j<layerweds.timeList.size();++j){
            cout<<layerweds.timeList[j]<<" ";
        }
        cout<<endl;

        staticGraphs[i].reportself();
    }
}

//kval是度数
bool Tempgraph::checkWEDSinNode(const WEDS& theweds,int thenode,int kval,unordered_set<int>& deleted){
    //debug
    return true;
    
    int incnt=0;
    // if(degree[thenode]<kval) return false;
    for(int i=0;i<adjList[thenode].size();++i){
        if(validv[adjList[thenode][i].first]==false&&deleted.find(adjList[thenode][i].first)==deleted.end()) continue;
        int theeid=adjList[thenode][i].second;
        if(checkWEDSinTimelist(theweds,edgePool[theeid])){
            incnt++;
            if(incnt>=kval) return true;
        }
    }
    return false;
}

void printset(unordered_set<int>& theset){
    unordered_set<int>::iterator it=theset.begin();
    while(it!=theset.end()){
        cerr<<*it<<" ";
        it++;
    }
    cerr<<endl;
}

void Tempgraph::bk_pivot_direct(unordered_set<int> cand,unordered_set<int> curclique,unordered_set<int> X,vector<pair<WEDS,unordered_set<int>>>& holdcliques,int kval,const WEDS& thelayer,unordered_set<int>& deleted){
    //debug
    // WEDS buglayer;
    // buglayer.timeList={1,3,5};
    // if(thelayer==buglayer){
    //     printset(cand);
    //     printset(curclique);
    //     printset(X);
    // }


    int truedegree=kval-1;
    if(cand.size()+curclique.size()<kval) return;//肯定不够kval
    if(cand.size()==0&&X.size()==0){//汇报答案
        holdcliques.push_back(pair<WEDS,unordered_set<int>>(thelayer,curclique));
        return;
    }

    //在cand和X中选择pivot
    int bigsize=0;
    int bigsizeNode=-1;
    unordered_set<int>::iterator it=cand.begin();
    while(it!=cand.end()){
        int tempcnt=0;
        for(int i=0;i<adjList[*it].size();++i){
            int neiid=adjList[*it][i].first;
            if(validv[neiid]==false) continue;
            if(checkWEDSinTimelist(thelayer,edgePool[adjList[*it][i].second])){
                if(checkWEDSinNode(thelayer,neiid,truedegree,deleted)){
                    if(cand.find(neiid)!=cand.end()) tempcnt++;
                }
            }
        }
        if(tempcnt>bigsize){
            bigsize=tempcnt;
            bigsizeNode=*it;
        }
        it++;
    }
    it=X.begin();
    while(it!=X.end()){
        // vector<int>& neighbors=adjlist[*it];
        int tempcnt=0;
        for(int i=0;i<adjList[*it].size();++i){
            int neiid=adjList[*it][i].first;
            if(validv[neiid]==false) continue;
            if(checkWEDSinTimelist(thelayer,edgePool[adjList[*it][i].second])){
                if(checkWEDSinNode(thelayer,neiid,truedegree,deleted)){
                    if(cand.find(neiid)!=cand.end()) tempcnt++;
                }
            }
        }
        if(tempcnt>bigsize){
            bigsize=tempcnt;
            bigsizeNode=*it;
        }
        it++;
    }

    unordered_set<int> tempInter;
    if(bigsize>0){
        for(int i=0;i<adjList[bigsizeNode].size();++i){
            int neiid=adjList[bigsizeNode][i].first;
            if(validv[neiid]==false) continue;
            if(checkWEDSinTimelist(thelayer,edgePool[adjList[bigsizeNode][i].second])){
                if(checkWEDSinNode(thelayer,neiid,truedegree,deleted)){
                    if(cand.find(neiid)!=cand.end()) tempInter.insert(neiid);
                }
            }
        }
        // for(int i=0;i<adjlist[bigsizeNode].size();++i){
        //     if(cand.find(adjlist[bigsizeNode][i])!=cand.end()) tempInter.insert(adjlist[bigsizeNode][i]);
        // }
    }
    unordered_set<int> candcp=cand;
    it=candcp.begin();
    while(it!=candcp.end()){
        if(tempInter.find(*it)!=tempInter.end()){
            it++;
            continue;
        }
        unordered_set<int> newcand,newX;

        for(int i=0;i<adjList[*it].size();++i){
            int neiid=adjList[*it][i].first;
            if(validv[neiid]==false){
                if(X.find(neiid)!=X.end()){
                    if(checkWEDSinTimelist(thelayer,edgePool[adjList[*it][i].second])){
                        if(checkWEDSinNode(thelayer,neiid,truedegree,deleted)){
                            newX.insert(neiid);
                        }
                    }
                }
                continue;
            }
            if(checkWEDSinTimelist(thelayer,edgePool[adjList[*it][i].second])){
                if(checkWEDSinNode(thelayer,neiid,truedegree,deleted)){
                    if(cand.find(neiid)!=cand.end()) newcand.insert(neiid);
                    if(X.find(neiid)!=X.end()) newX.insert(neiid);
                }
            }
        }

        //test
        // for(int i=0;i<adjList[*it].size();++i){
        //     int neiid=adjList[*it][i].first;
        //     if(X.find(neiid)!=X.end()){
        //         if(checkWEDSinTimelist(thelayer,edgePool[adjList[*it][i].second])){
        //             if(checkWEDSinNode(thelayer,neiid,truedegree,deleted)){
        //                 newX.insert(neiid);
        //             }
        //         }
        //     }else if(cand.find(neiid)!=cand.end()){
        //         if(checkWEDSinTimelist(thelayer,edgePool[adjList[*it][i].second])){
        //             if(checkWEDSinNode(thelayer,neiid,truedegree,deleted)){
        //                 newcand.insert(neiid);
        //             }
        //         }
        //     }
        // }

        // for(int i=0;i<adjlist[*it].size();++i){
        //     if(cand.find(adjlist[*it][i])!=cand.end()) newcand.insert(adjlist[*it][i]);
        //     if(X.find(adjlist[*it][i])!=X.end()) newX.insert(adjlist[*it][i]);
        // }
        // unordered_set<int> newcur=curclique;
        curclique.insert(*it);
        bk_pivot_direct(newcand,curclique,newX,holdcliques,kval,thelayer,deleted);
        cand.erase(*it);X.insert(*it);
        curclique.erase(*it);
        it++;
    }
}

void Tempgraph::computeSingleWPTdirect(int nodeId,int sigma,int percent,int kval,unordered_set<WEDS,hashWEDS>& ans){
    //test varify pivot direct// only here
    // WEDS temp;
    // temp.timeList={1,2,3};
    // ans=unordered_set<WEDS,hashWEDS>({temp});
    // return;

    unordered_map<int,int> cntTimesOfTime;
    for(int i=0;i<adjList[nodeId].size();++i){
        if(validv[adjList[nodeId][i].first]==false) continue;
        int edgeid=adjList[nodeId][i].second;

        for(int j=0;j<edgePool[edgeid].size();++j){
            if(cntTimesOfTime.find(edgePool[edgeid][j])!=cntTimesOfTime.end()){
                cntTimesOfTime[edgePool[edgeid][j]]++;
            }else{
                cntTimesOfTime[edgePool[edgeid][j]]=1;
            }
        }
    }

    vector<int> availableTimeset;
    unordered_map<int,int>::iterator it=cntTimesOfTime.begin();
    while(it!=cntTimesOfTime.end()){
        if(it->second>=kval) availableTimeset.push_back(it->first);
        it++;
    }
    if(availableTimeset.size()<sigma) return;
    sort(availableTimeset.begin(),availableTimeset.end());

    //debug
    computeTime++;
    // cout<<availableTimeset.size()<<endl;

    // computeWPTOracle(availableTimeset,sigma,percent,ans);
    // computeWPTque(availableTimeset,sigma,percent,ans);
    // computeWPTset(availableTimeset,sigma,percent,ans);
    #ifdef oracle
    computeWPTOracle(availableTimeset,sigma,percent,ans);
    #endif
    #ifdef candque
    computeWPTque(availableTimeset,sigma,percent,ans);
    #endif
    #ifdef candset
    computeWPTset(availableTimeset,sigma,percent,ans);
    #endif


    //debug
    // unordered_set<WEDS,hashWEDS>::iterator that=ans.begin();
    // while(that!=ans.end()){
    //     if(that->timeList[0]==7556&&that->timeList[1]==7619){
    //         for(auto val:that->timeList){
    //             cerr<<val<<" ";
    //         }
    //         cerr<<endl<<"seg: ";
    //         for(auto val:availableTimeset){
    //             cerr<<val<<" ";
    //         }
    //         cerr<<endl;
    //     }
    //     that++;
    // }
}

void Tempgraph::MPC_direct_noorder(int kval,int sigma,int percent,vector<pair<WEDS,unordered_set<int>>>& ans){
    int truedegree=kval-1;
    computeKcore(truedegree);

    unordered_set<int> deleted;
    for(int i=0;i<nodecnt;++i){
        int nownode=i;
        if(validv[nownode]==false) continue;

        unordered_set<WEDS,hashWEDS> curSet;
        computeSingleWPTdirect(nownode,sigma,percent,truedegree,curSet);

        //debug
        // if(curSet.size()>0) cout<<"weds size: "<<curSet.size()<<endl;

        unordered_set<WEDS,hashWEDS>::iterator it=curSet.begin();
        while(it!=curSet.end()){

            //检查it是否是强周期
            // int incnt=0;
            // for(int i=0;i<adjList[nownode].size();++i){
            //     if(validv[adjList[nownode][i].first]==false&&deleted.find(adjList[nownode][i].first)==deleted.end()) continue;
            //     int theeid=adjList[nownode][i].second;
            //     if(checkWEDSinTimelist(*it,edgePool[theeid])){
            //         incnt++;
            //         if(incnt>=truedegree) break;
            //     }
            // }
            // if(incnt<truedegree){
            //     it++;
            //     continue;
            // }

            unordered_set<int> curclique;
            unordered_set<int> cand;
            unordered_set<int> X;
            curclique.insert(nownode);


            //准备符合layer的cand
            for(int j=0;j<adjList[nownode].size();++j){
                if(validv[adjList[nownode][j].first]==false){
                    if(deleted.find(adjList[nownode][j].first)!=deleted.end()){
                        if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                            if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                                X.insert(adjList[nownode][j].first);
                            }
                        }
                    }
                    continue;
                }
                if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                    if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                        cand.insert(adjList[nownode][j].first);
                    }
                }
            }
            bk_pivot_direct(cand,curclique,X,ans,kval,*it,deleted);
            it++;
        }

        //删除nownode
        validv[nownode]=false;
        degree[nownode]=0;
        deleted.insert(nownode);
        queue<int> que;
        que.push(nownode);
        while(!que.empty()){
            int curnode=que.front();que.pop();

            for(int i=0;i<adjList[curnode].size();++i){
                pair<int,int> neighbor=adjList[curnode][i];
                if(validv[neighbor.first]==false) continue;

                valide[neighbor.second]=false;//delete edge
                degree[neighbor.first]--;
                if(degree[neighbor.first]<truedegree){
                    validv[neighbor.first]=false;
                    que.push(neighbor.first);
                    deleted.insert(neighbor.first);
                }
            }
        }
    }
}

//kval是clique size，而不是度数
void Tempgraph::MPC_direct_degeneracy(int kval,int sigma,int percent,vector<pair<WEDS,unordered_set<int>>>& ans){
    int truedegree=kval-1;
    computeKcore(truedegree);
    vector<int> bin,verts,pos,permutation,tmpDegree;
    int bigd=0,validcnt=0;
    for(int i=0;i<nodecnt;++i){
        if(validv[i]){
            validcnt++;
            if(bigd<degree[i]) bigd=degree[i];
        }
    }
    bin.resize(bigd+2,0);
    verts.resize(validcnt+1);
    pos.resize(nodecnt+1);
    tmpDegree.resize(nodecnt+1);
    permutation.resize(validcnt);

    for(int i=0;i<nodecnt;++i){
        if(validv[i]){
            bin[degree[i]]++;
            tmpDegree[i]=degree[i];
        }
    }

    for(int i=1;i<=bigd;++i) bin[i]+=bin[i-1];
    bin[bigd+1]=bin[bigd];
    for(int i=0;i<nodecnt;++i){
        if(validv[i]){
            pos[i]=bin[degree[i]]-1;
            verts[--bin[degree[i]]]=i;
        }
    }

    for(int i=0;i<validcnt;++i){
        int vid=verts[i];
        permutation[i]=vid;
        for(int j=0;j<adjList[vid].size();++j){
            if(validv[adjList[vid][j].first]){
                int nid=adjList[vid][j].first;
                if(tmpDegree[nid]>tmpDegree[vid]){
                    int binFront=verts[bin[tmpDegree[nid]]];
                    if(binFront!=nid){
                        pos[binFront]=pos[nid];
                        pos[nid]=bin[tmpDegree[nid]];
                        verts[bin[tmpDegree[nid]]]=nid;
                        verts[pos[binFront]]=binFront;
                    }
                    bin[tmpDegree[nid]]++;
                    tmpDegree[nid]--;
                }
            }
        }
    }

    unordered_set<int> deleted;
    for(int i=0;i<permutation.size();++i){
        int nownode=permutation[i];
        if(validv[nownode]==false) continue;

        unordered_set<WEDS,hashWEDS> curSet;
        computeSingleWPTdirect(nownode,sigma,percent,truedegree,curSet);

        //debug
        // if(curSet.size()>0) cout<<"weds size: "<<curSet.size()<<endl;

        unordered_set<WEDS,hashWEDS>::iterator it=curSet.begin();
        while(it!=curSet.end()){

            unordered_set<int> curclique;
            unordered_set<int> cand;
            unordered_set<int> X;
            curclique.insert(nownode);


            //准备符合layer的cand
            for(int j=0;j<adjList[nownode].size();++j){
                if(validv[adjList[nownode][j].first]==false){
                    if(deleted.find(adjList[nownode][j].first)!=deleted.end()){
                        if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                            if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                                X.insert(adjList[nownode][j].first);
                            }
                        }
                    }
                    continue;
                }
                if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                    if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                        cand.insert(adjList[nownode][j].first);
                    }
                }
            }
            bk_pivot_direct(cand,curclique,X,ans,kval,*it,deleted);
            it++;
        }

        //删除nownode
        validv[nownode]=false;
        degree[nownode]=0;
        deleted.insert(nownode);
        queue<int> que;
        que.push(nownode);
        while(!que.empty()){
            int curnode=que.front();que.pop();

            for(int i=0;i<adjList[curnode].size();++i){
                pair<int,int> neighbor=adjList[curnode][i];
                if(validv[neighbor.first]==false) continue;

                valide[neighbor.second]=false;//delete edge
                degree[neighbor.first]--;
                if(degree[neighbor.first]<truedegree){
                    validv[neighbor.first]=false;
                    que.push(neighbor.first);
                    deleted.insert(neighbor.first);
                }
            }
        }
    }
}

//kval是clique size，而不是度数
void Tempgraph::MPC_direct(int kval,int sigma,int percent,vector<pair<WEDS,unordered_set<int>>>& ans){
    #ifdef cntCheckEdgeLayer
    int cntcheck=0;
    #endif

    #ifdef cntvisitnei
    int cntvisit=0;
    #endif


    int truedegree=kval-1;
    computeKcore(truedegree);

    vector<pair<int,int>> leftNodeAndDegree;//pii 度数和节点mark
    for(int i=0;i<nodecnt;++i){
        if(validv[i]) leftNodeAndDegree.push_back(pair<int,int>(degree[i],i));
    }
    sort(leftNodeAndDegree.begin(),leftNodeAndDegree.end());

    unordered_set<int> deleted;
    for(int i=0;i<leftNodeAndDegree.size();++i){
        int nownode=leftNodeAndDegree[i].second;
        if(validv[nownode]==false) continue;

        unordered_set<WEDS,hashWEDS> curSet;
        computeSingleWPTdirect(nownode,sigma,percent,truedegree,curSet);

        //debug
        // if(curSet.size()>0) cout<<"weds size: "<<curSet.size()<<endl;

        unordered_set<WEDS,hashWEDS>::iterator it=curSet.begin();
        while(it!=curSet.end()){

            //检查it是否是强周期
            // int incnt=0;
            // for(int i=0;i<adjList[nownode].size();++i){
            //     if(validv[adjList[nownode][i].first]==false&&deleted.find(adjList[nownode][i].first)==deleted.end()) continue;
            //     int theeid=adjList[nownode][i].second;
            //     if(checkWEDSinTimelist(*it,edgePool[theeid])){
            //         incnt++;
            //         if(incnt>=truedegree) break;
            //     }
            // }
            // if(incnt<truedegree){
            //     it++;
            //     continue;
            // }

            unordered_set<int> curclique;
            unordered_set<int> cand;
            unordered_set<int> X;
            curclique.insert(nownode);


            #ifdef cntvisitnei
            cntvisit++;
            #endif

            //准备符合layer的cand
            for(int j=0;j<adjList[nownode].size();++j){
                if(validv[adjList[nownode][j].first]==false){
                    if(deleted.find(adjList[nownode][j].first)!=deleted.end()){
                        #ifdef cntCheckEdgeLayer
                        cntcheck++;
                        #endif
                        if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                            if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                                X.insert(adjList[nownode][j].first);
                            }
                        }
                    }
                    continue;
                }
                #ifdef cntCheckEdgeLayer
                cntcheck++;
                #endif
                if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                    if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                        cand.insert(adjList[nownode][j].first);
                    }
                }
            }
            bk_pivot_direct(cand,curclique,X,ans,kval,*it,deleted);
            it++;
        }

        //删除nownode
        validv[nownode]=false;
        degree[nownode]=0;
        deleted.insert(nownode);
        queue<int> que;
        que.push(nownode);
        while(!que.empty()){
            int curnode=que.front();que.pop();

            for(int i=0;i<adjList[curnode].size();++i){
                pair<int,int> neighbor=adjList[curnode][i];
                if(validv[neighbor.first]==false) continue;

                valide[neighbor.second]=false;//delete edge
                degree[neighbor.first]--;
                if(degree[neighbor.first]<truedegree){
                    validv[neighbor.first]=false;
                    que.push(neighbor.first);
                    deleted.insert(neighbor.first);
                }
            }
        }
    }
    #ifdef cntCheckEdgeLayer
    cout<<"cntcheck: "<<cntcheck<<endl;
    #endif

    #ifdef cntvisitnei
    cout<<"total visit: "<<cntvisit<<endl;
    #endif
}

void Tempgraph::MPCO_direct_cnt_total_qpcg(int kval,int sigma,int percent,vector<pair<WEDS,vector<int>>>& ans){
    computeKcore(kval);

    vector<pair<int,int>> leftNodeAndDegree;//pii 度数和节点mark
    for(int i=0;i<nodecnt;++i){
        if(validv[i]) leftNodeAndDegree.push_back(pair<int,int>(degree[i],i));
    }
    sort(leftNodeAndDegree.begin(),leftNodeAndDegree.end());

    ll cnt_total_qpcg=0;

    // unordered_set<WEDS,hashWEDS> visitlayer;
    unordered_map<WEDS,unordered_set<int>,hashWEDS> inLayerConnect;
    // unordered_set<int> deleted;
    for(int i=0;i<leftNodeAndDegree.size();++i){
        // #ifdef countcore
        // if(i%1000==0) cout<<i<<" over "<<leftNodeAndDegree.size()<<endl;
        // #endif
        int nownode=leftNodeAndDegree[i].second;
        if(validv[nownode]==false) continue;

        unordered_set<WEDS,hashWEDS> curSet;
        computeSingleWPTdirect(nownode,sigma,percent,kval,curSet);

        if(curSet.size()!=0){
            cnt_total_qpcg+=curSet.size();
            continue;
        }

        validv[nownode]=false;
        degree[nownode]=0;
        queue<int> que;
        que.push(nownode);
        while(!que.empty()){
            int curnode=que.front();que.pop();

            for(int i=0;i<adjList[curnode].size();++i){
                pair<int,int> neighbor=adjList[curnode][i];
                if(validv[neighbor.first]==false) continue;

                degree[neighbor.first]--;
                if(degree[neighbor.first]<kval){
                    validv[neighbor.first]=false;
                    que.push(neighbor.first);
                }
            }
        }
    }

    cout<<"total qpcg: "<<cnt_total_qpcg<<endl;

    #ifdef cntCheckEdgeLayer
    cout<<"cntcheck: "<<cntcheck<<endl;
    #endif

    #ifdef directcoreTotaltime
    cout<<"totalcoretime: "<<totaltime/1000<<endl;
    #endif

    #ifdef cntvisitnei
    cout<<"total visit: "<<cntvisit<<endl;
    #endif
}

//kval是度数
void Tempgraph::MPCO_direct(int kval,int sigma,int percent,vector<pair<WEDS,vector<int>>>& ans){
    computeKcore(kval);

    vector<pair<int,int>> leftNodeAndDegree;//pii 度数和节点mark
    for(int i=0;i<nodecnt;++i){
        if(validv[i]) leftNodeAndDegree.push_back(pair<int,int>(degree[i],i));
    }
    sort(leftNodeAndDegree.begin(),leftNodeAndDegree.end());

    unordered_set<WEDS,hashWEDS> visitlayer;
    // unordered_set<int> deleted;
    for(int i=0;i<leftNodeAndDegree.size();++i){
        int nownode=leftNodeAndDegree[i].second;
        if(validv[nownode]==false) continue;

        unordered_set<WEDS,hashWEDS> curSet;
        computeSingleWPTdirect(nownode,sigma,percent,kval,curSet);

        unordered_set<WEDS,hashWEDS>::iterator it=curSet.begin();
        while(it!=curSet.end()){
            if(visitlayer.find(*it)==visitlayer.end()){
                visitlayer.insert(*it);
                Staticgraph layergraph;
                for(int j=i;j<leftNodeAndDegree.size();++j){
                    int curnode=leftNodeAndDegree[j].second;
                    if(validv[curnode]==false) continue;

                    for(int k=0;k<adjList[curnode].size();++k){
                        int nei=adjList[curnode][k].first;
                        if(nei<curnode||validv[nei]==false) continue;
                        if(checkWEDSinTimelist(*it,edgePool[adjList[curnode][k].second])==false) continue;
                        layergraph.addedge(curnode,nei);
                    }
                }
                vector<int> holdCore;
                layergraph.enumKCore(kval,holdCore);
                if(holdCore.size()>0) ans.push_back(pair<WEDS,vector<int>>(*it,holdCore));

                //debug
                // if(it->timeList[0]==1&&it->timeList[1]==3&&it->timeList[2]==5){
                //     layergraph.reportself();
                // }
            }
            it++;
        }

        validv[nownode]=false;
        degree[nownode]=0;
        queue<int> que;
        que.push(nownode);
        while(!que.empty()){
            int curnode=que.front();que.pop();

            for(int i=0;i<adjList[curnode].size();++i){
                pair<int,int> neighbor=adjList[curnode][i];
                if(validv[neighbor.first]==false) continue;

                degree[neighbor.first]--;
                if(degree[neighbor.first]<kval){
                    validv[neighbor.first]=false;
                    que.push(neighbor.first);
                }
            }
        }
    }
}

//kval是度数
void Tempgraph::MPCO_direct_1(int kval,int sigma,int percent,vector<pair<WEDS,vector<int>>>& ans){
    computeKcore(kval);

    vector<pair<int,int>> leftNodeAndDegree;//pii 度数和节点mark
    for(int i=0;i<nodecnt;++i){
        if(validv[i]) leftNodeAndDegree.push_back(pair<int,int>(degree[i],i));
    }
    sort(leftNodeAndDegree.begin(),leftNodeAndDegree.end());

    //debug
    // for(auto val:leftNodeAndDegree){
    //     cout<<val.second<<" ";
    // }
    // cout<<endl;

    #ifdef directcoreTotaltime
    struct timeval start,end,astart,aend;
    int totaltime=0,purecoretime=0;
    #endif

    #ifdef cntCheckEdgeLayer
    int cntcheck=0;
    #endif

    #ifdef cntvisitnei
    int cntvisit=0;
    #endif

    #ifdef qpcg_rule2_cnt
    int cnt_qpcg=0,cnt_rule2=0;
    #endif

    // unordered_set<WEDS,hashWEDS> visitlayer;
    unordered_map<WEDS,unordered_set<int>,hashWEDS> inLayerConnect;
    // unordered_set<int> deleted;
    for(int i=0;i<leftNodeAndDegree.size();++i){
        int nownode=leftNodeAndDegree[i].second;
        if(validv[nownode]==false) continue;

        unordered_set<WEDS,hashWEDS> curSet;
        computeSingleWPTdirect(nownode,sigma,percent,kval,curSet);

        unordered_set<WEDS,hashWEDS>::iterator it=curSet.begin();
        while(it!=curSet.end()){
            if(inLayerConnect.find(*it)!=inLayerConnect.end()&&inLayerConnect[*it].find(nownode)!=inLayerConnect[*it].end()){
                it++;
                continue;
            }

            #ifdef qpcg_rule2_cnt
            cnt_qpcg++;
            #endif

            Staticgraph connectgraph;
            unordered_set<int> inque;
            queue<int> tque;
            tque.push(nownode);inque.insert(nownode);
            

            while(!tque.empty()){
                int curnode=tque.front();tque.pop();

                #ifdef directcoreTotaltime
                gettimeofday(&astart,NULL);
                #endif

                inLayerConnect[*it].insert(curnode);

                #ifdef directcoreTotaltime
                gettimeofday(&aend,NULL);
                totaltime+=(aend.tv_sec-astart.tv_sec)*1000000+(aend.tv_usec-astart.tv_usec);
                // purecoretime+=(aend.tv_sec-start.tv_sec)*1000+(aend.tv_usec-start.tv_usec)/1000;
                #endif

                #ifdef cntvisitnei
                cntvisit++;
                #endif
                for(int j=0;j<adjList[curnode].size();++j){
                    pair<int,int> neighbor=adjList[curnode][j];
                    if(validv[neighbor.first]==false) continue;
                    if(inLayerConnect[*it].find(neighbor.first)!=inLayerConnect[*it].end()) continue;

                    if(checkWEDSinTimelist(*it,edgePool[neighbor.second])==false){
                        continue;
                    }

                    connectgraph.addedge(curnode,neighbor.first);
                    // inLayerConnect[*it].insert(neighbor.first);
                    if(inque.find(neighbor.first)!=inque.end()) continue;
                    inque.insert(neighbor.first);
                    tque.push(neighbor.first);

                }
            }

            vector<vector<int>> holdCore;
            connectgraph.enumKCores(kval,holdCore);
            if(holdCore.size()>0){
                for(int j=0;j<holdCore.size();++j){
                    ans.push_back(pair<WEDS,vector<int>>(*it,holdCore[j]));
                }
            }
            
            it++;
        }

        validv[nownode]=false;
        degree[nownode]=0;
        queue<int> que;
        que.push(nownode);
        while(!que.empty()){
            int curnode=que.front();que.pop();

            for(int i=0;i<adjList[curnode].size();++i){
                pair<int,int> neighbor=adjList[curnode][i];
                if(validv[neighbor.first]==false) continue;

                degree[neighbor.first]--;
                if(degree[neighbor.first]<kval){
                    validv[neighbor.first]=false;
                    que.push(neighbor.first);
                }
            }
        }
    }

    #ifdef cntCheckEdgeLayer
    cout<<"cntcheck: "<<cntcheck<<endl;
    #endif

    #ifdef directcoreTotaltime
    cout<<"totalcoretime: "<<totaltime/1000<<endl;
    #endif

    #ifdef cntvisitnei
    cout<<"total visit: "<<cntvisit<<endl;
    #endif

    #ifdef qpcg_rule2_cnt
    cout<<"act qpcg: "<<cnt_qpcg<<" cnt rule2: "<<cnt_rule2<<endl;
    #endif
}

void Tempgraph::MPC_direct_firstlayer(int kval,int sigma,int percent,vector<pair<WEDS,unordered_set<int>>>& ans){
    int truedegree=kval-1;
    computeKcore(truedegree);

    vector<pair<int,int>> leftNodeAndDegree;//pii 度数和节点mark
    for(int i=0;i<nodecnt;++i){
        if(validv[i]) leftNodeAndDegree.push_back(pair<int,int>(degree[i],i));
    }
    sort(leftNodeAndDegree.begin(),leftNodeAndDegree.end());

    unordered_map<WEDS,unordered_set<int>,hashWEDS> hasPivot;
    unordered_set<int> deleted;
    for(int i=0;i<leftNodeAndDegree.size();++i){
        int nownode=leftNodeAndDegree[i].second;
        if(validv[nownode]==false) continue;

        unordered_set<WEDS,hashWEDS> curSet;
        computeSingleWPTdirect(nownode,sigma,percent,truedegree,curSet);

        //debug
        // if(curSet.size()>0) cout<<"weds size: "<<curSet.size()<<endl;

        unordered_set<WEDS,hashWEDS>::iterator it=curSet.begin();
        while(it!=curSet.end()){

            if(hasPivot.find(*it)!=hasPivot.end()){
                if(hasPivot[*it].find(nownode)!=hasPivot[*it].end()){
                    it++;
                    continue;
                }
            }else{
                for(int j=0;j<adjList[nownode].size();++j){
                    if(validv[adjList[nownode][j].first]==false){
                        continue;
                    }
                    if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                        if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                            hasPivot[*it].insert(adjList[nownode][j].first);
                        }
                    }
                }
            }

            unordered_set<int> curclique;
            unordered_set<int> cand;
            unordered_set<int> X;
            curclique.insert(nownode);


            //准备符合layer的cand
            for(int j=0;j<adjList[nownode].size();++j){
                if(validv[adjList[nownode][j].first]==false){
                    if(deleted.find(adjList[nownode][j].first)!=deleted.end()){
                        if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                            if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                                X.insert(adjList[nownode][j].first);
                            }
                        }
                    }
                    continue;
                }
                if(checkWEDSinTimelist(*it,edgePool[adjList[nownode][j].second])){
                    if(checkWEDSinNode(*it,adjList[nownode][j].first,truedegree,deleted)){
                        cand.insert(adjList[nownode][j].first);
                    }
                }
            }
            bk_pivot_direct(cand,curclique,X,ans,kval,*it,deleted);
            it++;
        }

        //删除nownode
        validv[nownode]=false;
        degree[nownode]=0;
        deleted.insert(nownode);
        queue<int> que;
        que.push(nownode);
        while(!que.empty()){
            int curnode=que.front();que.pop();

            for(int i=0;i<adjList[curnode].size();++i){
                pair<int,int> neighbor=adjList[curnode][i];
                if(validv[neighbor.first]==false) continue;

                valide[neighbor.second]=false;//delete edge
                degree[neighbor.first]--;
                if(degree[neighbor.first]<truedegree){
                    validv[neighbor.first]=false;
                    que.push(neighbor.first);
                    deleted.insert(neighbor.first);
                }
            }
        }
    }
}

void Tempgraph::MPC_WPK(int kval,int sigma,int percent){
    struct timeval start,end;

    // computeDegree();
    computeKcore(kval);

    unordered_map<int,unordered_set<WEDS,hashWEDS>> nodeToWPT;
    unordered_map<int,unordered_map<int,int> > nodeToKeyTimeDegree;

    gettimeofday(&start,NULL);
    for(int i=0;i<nodecnt;++i){
        if(validv[i]){
            computeSingleWPT(i,sigma,percent,kval,nodeToWPT,nodeToKeyTimeDegree);
        }
    }
    gettimeofday(&end,NULL);
    cerr<<"compute all wpt time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;

    gettimeofday(&start,NULL);
    transformByNode(kval,sigma,nodeToWPT);
    gettimeofday(&end,NULL);
    cerr<<"trans graph time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;
}

// #define REPORT

void Tempgraph::MPC_WPN(int kval,int sigma,int percent){
    struct timeval start,end;

    // computeDegree();
    computeKcore(kval);

    // debug
    #ifdef REPORT
    cerr<<"after kcore:"<<endl;
    reportdevalidsize(kval);
    #endif

    unordered_map<int,unordered_set<WEDS,hashWEDS>> nodeToWPT;
    unordered_map<int,unordered_map<int,vector<WEDS>>> nodeToIWPT;
    unordered_map<int,unordered_map<int,int> > nodeToKeyTimeDegree;

    gettimeofday(&start,NULL);
    WPNCluster(sigma,percent,kval,nodeToWPT,nodeToIWPT,nodeToKeyTimeDegree);
    gettimeofday(&end,NULL);
    #ifdef REPORT
    reportdevalidsize(kval);
    #endif

    // //debug
    // cout<<"after wpncluster:"<<endl;
    // reportNodes(kval);

    gettimeofday(&start,NULL);
    transformByNode(kval,sigma,nodeToWPT);
    gettimeofday(&end,NULL);
    #ifdef REPORT
    reportstaticsize();
    #endif
    //done

    // //debug
    // cout<<"transformed graphs:"<<endl;
    // reportTransformedGraph();
}

void Tempgraph::computefixLenPkcores(int kval,vector<pair<WEDS,vector<int>>>& fixlenPkcores){
    for(int i=0;i<cntlayers;++i){
        vector<vector<int>> holdCore;
        staticGraphs[i].enumKCores(kval,holdCore);
        if(holdCore.size()>0){
            for(int j=0;j<holdCore.size();++j){
                fixlenPkcores.push_back(pair<WEDS,vector<int>>(layerToWEDS[i],holdCore[j]));
            }
        }
    }
}

void Tempgraph::computefixLenPcliques(int kval,vector<Pcliques>& fixLenPcliques){
    for(int i=0;i<cntlayers;++i){
        fixLenPcliques.push_back(Pcliques());
        fixLenPcliques[fixLenPcliques.size()-1].weds=layerToWEDS[i];
        staticGraphs[i].enumClique(kval,fixLenPcliques[fixLenPcliques.size()-1].cliques);

        if(fixLenPcliques[fixLenPcliques.size()-1].cliques.size()==0){
            fixLenPcliques.pop_back();//实际上这一层没有周期clique，所以撤销
        }
    }
}

void Staticgraph::bk_pivot(unordered_set<int> cand,unordered_set<int> curclique,unordered_set<int> X,vector<unordered_set<int>>& holdcliques,int kval){    
    if(cand.size()+curclique.size()<kval) return;//肯定不够kval
    if(cand.size()==0&&X.size()==0){//汇报答案
        holdcliques.push_back(curclique);
        return;
    }

    //在cand和X中选择pivot
    int bigsize=0;
    int bigsizeNode=-1;
    unordered_set<int>::iterator it=cand.begin();
    while(it!=cand.end()){
        vector<int>& neighbors=adjlist[*it];
        int tempcnt=0;
        for(int i=0;i<neighbors.size();++i){
            if(cand.find(neighbors[i])!=cand.end()) tempcnt++;
        }
        if(tempcnt>bigsize){
            bigsize=tempcnt;
            bigsizeNode=*it;
        }
        it++;
    }
    it=X.begin();
    while(it!=X.end()){
        vector<int>& neighbors=adjlist[*it];
        int tempcnt=0;
        for(int i=0;i<neighbors.size();++i){
            if(cand.find(neighbors[i])!=cand.end()) tempcnt++;
        }
        if(tempcnt>bigsize){
            bigsize=tempcnt;
            bigsizeNode=*it;
        }
        it++;
    }

    unordered_set<int> tempInter;
    if(bigsize>0){
        for(int i=0;i<adjlist[bigsizeNode].size();++i){
            if(cand.find(adjlist[bigsizeNode][i])!=cand.end()) tempInter.insert(adjlist[bigsizeNode][i]);
        }
    }
    unordered_set<int> candcp=cand;
    it=candcp.begin();
    while(it!=candcp.end()){
        if(tempInter.find(*it)!=tempInter.end()){
            it++;
            continue;
        }
        unordered_set<int> newcand,newX;
        for(int i=0;i<adjlist[*it].size();++i){
            if(cand.find(adjlist[*it][i])!=cand.end()) newcand.insert(adjlist[*it][i]);
            if(X.find(adjlist[*it][i])!=X.end()) newX.insert(adjlist[*it][i]);
        }
        unordered_set<int> newcur=curclique;
        newcur.insert(*it);
        bk_pivot(newcand,newcur,newX,holdcliques,kval);
        cand.erase(*it);X.insert(*it);
        it++;
    }

}

void Staticgraph::enumClique(int kval,vector<unordered_set<int>>& holdcliques){
    unordered_set<int> nodeset;
    unordered_map<int,vector<int>>::iterator it=adjlist.begin();
    while(it!=adjlist.end()){
        nodeset.insert(it->first);
        it++;
    }
    bk_pivot(nodeset,unordered_set<int>(),unordered_set<int>(),holdcliques,kval);
}

void Staticgraph::enumKCore(int kval,vector<int>& holdkcores){
    int nodecnt=adjlist.size();
    unordered_map<int,int> degree;
    unordered_set<int> invalidv;
    unordered_map<int,vector<int>>::iterator it=adjlist.begin();
    while(it!=adjlist.end()){
        degree[it->first]=it->second.size();
        it++;
    }
    queue<int> que;
    it=adjlist.begin();
    while(it!=adjlist.end()){
        int curnode=it->first;
        if(degree[curnode]<kval){
            invalidv.insert(curnode);
            que.push(curnode);
        }
        it++;
    }

    while(!que.empty()){
        int nownode=que.front();que.pop();

        for(int i=0;i<adjlist[nownode].size();++i){
            int neighbor=adjlist[nownode][i];
            if(invalidv.find(neighbor)!=invalidv.end()) continue;

            degree[neighbor]=degree[neighbor]-1;
            if(degree[neighbor]==kval-1){
                invalidv.insert(neighbor);
                que.push(neighbor);
            }
        }
    }

    it=adjlist.begin();
    while(it!=adjlist.end()){
        if(invalidv.find(it->first)==invalidv.end()) holdkcores.push_back(it->first);
        it++;
    }
}

void Staticgraph::enumKCores(int kval,vector<vector<int>>& holdkcores){
    int nodecnt=adjlist.size();
    unordered_map<int,int> degree;
    unordered_set<int> invalidv;
    unordered_map<int,vector<int>>::iterator it=adjlist.begin();
    while(it!=adjlist.end()){
        degree[it->first]=it->second.size();
        it++;
    }
    queue<int> que;
    it=adjlist.begin();
    while(it!=adjlist.end()){
        int curnode=it->first;
        if(degree[curnode]<kval){
            invalidv.insert(curnode);
            que.push(curnode);
        }
        it++;
    }

    while(!que.empty()){
        int nownode=que.front();que.pop();

        for(int i=0;i<adjlist[nownode].size();++i){
            int neighbor=adjlist[nownode][i];
            if(invalidv.find(neighbor)!=invalidv.end()) continue;

            degree[neighbor]=degree[neighbor]-1;
            if(degree[neighbor]==kval-1){
                invalidv.insert(neighbor);
                que.push(neighbor);
            }
        }
    }

    unordered_set<int> added;
    it=adjlist.begin();
    while(it!=adjlist.end()){

        if(invalidv.find(it->first)==invalidv.end()&&added.find(it->first)==added.end()){
            vector<int> thecore;
            thecore.push_back(it->first);
            added.insert(it->first);

            queue<int> que;
            que.push(it->first);
            while(!que.empty()){
                int now=que.front();que.pop();
                for(int i=0;i<adjlist[now].size();++i){
                    if(invalidv.find(adjlist[now][i])==invalidv.end()&&added.find(adjlist[now][i])==added.end()){
                        added.insert(adjlist[now][i]);
                        thecore.push_back(adjlist[now][i]);
                        que.push(adjlist[now][i]);
                    }
                }
            }
            holdkcores.push_back(thecore);
        }
        it++;
    }
}

void Staticgraph::addedge(int aid,int bid){
    //不会有重复边
    adjlist[aid].push_back(bid);
    adjlist[bid].push_back(aid);
}

bool checkWEDSinTimelist(const WEDS& theweds,vector<int>& timelist){
    for(int i=0;i<theweds.timeList.size();++i){
        vector<int>::iterator it=lower_bound(timelist.begin(),timelist.end(),theweds.timeList[i]);
        if(it==timelist.end()||(*it)!=theweds.timeList[i]) return false;
    }

    return true;
}

void Tempgraph::transformByNode(int kval,int sigma,unordered_map<int,unordered_set<WEDS,hashWEDS>>& theNodeToWPT){
    for(int i=0;i<nodecnt;++i){
        if(validv[i]==false) continue;

        //debug
        // cout<<"weds num: "<<theNodeToWPT[i].size()<<endl;

        for(int j=0;j<adjList[i].size();++j){
            pair<int,int> neighbor=adjList[i][j];
            if(validv[neighbor.first]==false||neighbor.first<i) continue;//如果邻居小于当前的i，说明这条边已经被处理过了
            if(edgePool[neighbor.second].size()<sigma) continue;//边的时间戳太少

            vector<WEDS> interOfNodes;
            //取节点的weds的交集
            if(theNodeToWPT[i].size()<theNodeToWPT[neighbor.first].size()){
                unordered_set<WEDS,hashWEDS>::iterator it=theNodeToWPT[i].begin();
                while(it!=theNodeToWPT[i].end()){
                    if(theNodeToWPT[neighbor.first].find(*it)!=theNodeToWPT[neighbor.first].end()) interOfNodes.push_back(*it);
                    it++;
                }
            }else{
                unordered_set<WEDS,hashWEDS>::iterator it=theNodeToWPT[neighbor.first].begin();
                while(it!=theNodeToWPT[neighbor.first].end()){
                    if(theNodeToWPT[i].find(*it)!=theNodeToWPT[i].end()) interOfNodes.push_back(*it);
                    it++;
                }
            }
            
            if(interOfNodes.size()==0) continue;
            for(int k=0;k<interOfNodes.size();++k){
                if(checkWEDSinTimelist(interOfNodes[k],edgePool[neighbor.second])){//说明形成了一个周期边
                    if(WEDStoLayer.find(interOfNodes[k])==WEDStoLayer.end()){
                        WEDStoLayer[interOfNodes[k]]=cntlayers++;
                        staticGraphs.push_back(Staticgraph());
                        layerToWEDS.push_back(interOfNodes[k]);
                    }
                    staticGraphs[WEDStoLayer[interOfNodes[k]]].addedge(i,neighbor.first);
                }
            }
        }
    }
}

bool isPeriodic(WEDS& pre,WEDS& suc,int percent){
    int newdiff=pre.timeList[1]-pre.timeList[0];
    if(newdiff<suc.botDiff){
        if(suc.topDiff*100>100*newdiff+percent*newdiff) return false;
        else return true;
    }else if(newdiff>suc.topDiff){
        if(100*newdiff<=100*suc.botDiff+suc.botDiff*percent) return true;
        else return false;
    }else return true;
}

void collectAns(vector<Pcliques>& fixLenPcliques,int start,int end,unordered_map<WEDS,vector<int>,hashWEDS>& prefix,unordered_map<WEDS,vector<int>,hashWEDS>& suffix,vector<pair<WEDS,unordered_set<int>>>& ans,int percent){
    for(int i=start;i<=end;++i){
        WEDS& nowweds=fixLenPcliques[i].weds;
        vector<unordered_set<int>>& cliques=fixLenPcliques[i].cliques;
        WEDS pre,suf;
        pre.timeList.assign(nowweds.timeList.begin(),nowweds.timeList.end()-1);
        suf.timeList.assign(nowweds.timeList.begin()+1,nowweds.timeList.end());

        //前驱和后继列表，当中的大部分应该都能构成近似周期序列
        vector<int> * prelist=nullptr;
        vector<int> * suclist=nullptr;
        bool haspre=(suffix.find(pre)!=suffix.end());
        bool hassuc=(prefix.find(suf)!=prefix.end());
        if(haspre) prelist=&suffix[pre];
        if(hassuc) suclist=&prefix[suf];
        for(int j=0;j<cliques.size();++j){//可能内外循环交替会稍微好一点？(todo)
            bool found=false;
            if(haspre){
                for(int k=0;k<(*prelist).size();++k){
                    if(isPeriodic(fixLenPcliques[(*prelist)[k]].weds,nowweds,percent)==false) continue;
                    for(int o=0;o<fixLenPcliques[(*prelist)[k]].cliques.size();++o){
                        if(isSubset(cliques[j],fixLenPcliques[(*prelist)[k]].cliques[o])<=0){
                            found=true;
                            break;
                        }
                    }
                    if(found) break;
                }
                if(found) continue;
            }
            if(hassuc){
                for(int k=0;k<(*suclist).size();++k){
                    if(isPeriodic(nowweds,fixLenPcliques[(*suclist)[k]].weds,percent)==false) continue;
                    for(int o=0;o<fixLenPcliques[(*suclist)[k]].cliques.size();++o){
                        if(isSubset(cliques[j],fixLenPcliques[(*suclist)[k]].cliques[o])<=0){
                            found=true;
                            break;
                        }
                    }
                    if(found) break;
                }
                if(found) continue;
            }
            
            //走到这意味着是答案
            ans.push_back(pair<WEDS,unordered_set<int>>(nowweds,cliques[j]));
        }
    }
}

int isSubset(unordered_set<int>& smallset,unordered_set<int>& bigset){
    if(smallset.size()>bigset.size()) return 1;

    unordered_set<int>::iterator it=smallset.begin();
    while(it!=smallset.end()){
        if(bigset.find(*it)==bigset.end()) return 1;
        it++;
    }
    if(smallset.size()==bigset.size()) return 0;
    else return -1;
}

void intersection(unordered_set<int>& aset,unordered_set<int>& bset,unordered_set<int>& anset){
    if(aset.size()<bset.size()){
        unordered_set<int>::iterator it=aset.begin();
        while(it!=aset.end()){
            if(bset.find(*it)!=bset.end()) anset.insert(*it);
            it++;
        }
    }else{
        unordered_set<int>::iterator it=bset.begin();
        while(it!=bset.end()){
            if(aset.find(*it)!=aset.end()) anset.insert(*it);
            it++;
        }
    }
}

int checkredundant(vector<pair<WEDS,unordered_set<int>>>& source){
    unordered_map<WEDS,vector<WEDS>,hashWEDS> group;
    for(auto& thepair:source){
        auto& theclique=thepair.second;
        WEDS holdclique;
        unordered_set<int>::iterator it=theclique.begin();
        while(it!=theclique.end()){
            holdclique.timeList.push_back(*it);
            it++;
        }

        sort(holdclique.timeList.begin(),holdclique.timeList.end());
        group[holdclique].push_back(thepair.first);
    }

    unordered_map<WEDS,vector<WEDS>,hashWEDS>::iterator it=group.begin();
    int redundant=0;
    while(it!=group.end()){
        vector<WEDS>& thelist=it->second;
        for(int i=0;i<thelist.size();++i){
            for(int j=0;j<thelist.size();++j){
                if(i==j) continue;

                if(checkWEDSinTimelist(thelist[i],thelist[j].timeList)){
                    redundant++;
                    break;
                }
            }
        }

        it++;
    }

    // cerr<<"redundant num: "<<redundant<<endl;
    return redundant;
}

void generateNewLength(int kval,vector<Pcliques>& fixLenPcliques,int start,int end,unordered_map<WEDS,vector<int>,hashWEDS>& prefix,unordered_map<WEDS,vector<int>,hashWEDS>& suffix,int percent){
    for(int i=start;i<=end;++i){
        // WEDS& nowweds=fixLenPcliques[i].weds;
        // vector<unordered_set<int>>& cliques=fixLenPcliques[i].cliques;
        //注释掉以上的原因是，fixlenpcliques会被更新，引用其中的元素可能会失效

        WEDS suf;
        suf.timeList.assign(fixLenPcliques[i].weds.timeList.begin()+1,fixLenPcliques[i].weds.timeList.end());
        if(prefix.find(suf)!=prefix.end()){//有后继
            vector<int>& suclist=prefix[suf];
            for(int j=0;j<suclist.size();++j){
                if(isPeriodic(fixLenPcliques[i].weds,fixLenPcliques[suclist[j]].weds,percent)==false) continue;
                vector<unordered_set<int>> tempRes;
                for(int k=0;k<fixLenPcliques[i].cliques.size();++k){//开始两两匹配
                    for(int o=0;o<fixLenPcliques[suclist[j]].cliques.size();++o){
                        unordered_set<int> interans;
                        intersection(fixLenPcliques[i].cliques[k],fixLenPcliques[suclist[j]].cliques[o],interans);
                        if(interans.size()<kval) continue;
                        tempRes.push_back(interans);
                    }
                }

                //去除tempres中被包含的set，同时将其作为临时答案
                fixLenPcliques.push_back(Pcliques());
                Pcliques& lastItem=fixLenPcliques[fixLenPcliques.size()-1];
                lastItem.weds=fixLenPcliques[i].weds;
                lastItem.weds.timeList.push_back(fixLenPcliques[suclist[j]].weds.timeList[fixLenPcliques[suclist[j]].weds.timeList.size()-1]);
                if(lastItem.weds.botDiff>fixLenPcliques[suclist[j]].weds.botDiff) lastItem.weds.botDiff=fixLenPcliques[suclist[j]].weds.botDiff;
                if(lastItem.weds.topDiff<fixLenPcliques[suclist[j]].weds.topDiff) lastItem.weds.topDiff=fixLenPcliques[suclist[j]].weds.topDiff;

                for(int k=0;k<tempRes.size();++k){
                    //判断位于k的元素是否被包含
                    int bigsame=k;
                    bool found=false;
                    for(int o=0;o<tempRes.size();++o){
                        if(o==k) continue;
                        int cmpres=isSubset(tempRes[k],tempRes[o]);
                        if(cmpres<0){
                            found=true;
                            break;
                        }else if(cmpres==0){
                            if(bigsame<o) bigsame=o;
                        }
                    }
                    if(found) continue;
                    if(bigsame==k){//说明位于k的元素是tempres中最后一个（若有其它相等的元素）
                        lastItem.cliques.push_back(tempRes[k]);
                    }
                }

                if(lastItem.cliques.size()==0){//实际上没有答案
                    fixLenPcliques.pop_back();
                }else{//对新的fixlenpclique构建前后缀映射
                    WEDS tpre,tsuf;
                    tpre.timeList.assign(lastItem.weds.timeList.begin(),lastItem.weds.timeList.end()-1);
                    tsuf.timeList.assign(lastItem.weds.timeList.begin()+1,lastItem.weds.timeList.end());
                    prefix[tpre].push_back(fixLenPcliques.size()-1);
                    suffix[tsuf].push_back(fixLenPcliques.size()-1);
                }
            }
        }
    }
}

void generateExtremeLenPcliques(int kval,int percent,vector<Pcliques> fixLenPcliques,vector<pair<WEDS,unordered_set<int>>>& ans){
    //为fixLenPcliques中的每个weds计算topdiff和botdiff
    for(int i=0;i<fixLenPcliques.size();++i){
        WEDS& nowweds=fixLenPcliques[i].weds;
        nowweds.botDiff=nowweds.topDiff=nowweds.timeList[1]-nowweds.timeList[0];
        for(int j=1;j<nowweds.timeList.size()-1;++j){
            int curdiff=nowweds.timeList[j+1]-nowweds.timeList[j];
            if(nowweds.botDiff>curdiff) nowweds.botDiff=curdiff;
            if(nowweds.topDiff<curdiff) nowweds.topDiff=curdiff;
        }
    }

    //构建前缀和后缀的映射，便于快速组合更长的weds
    unordered_map<WEDS,vector<int>,hashWEDS> prefix,suffix;
    for(int i=0;i<fixLenPcliques.size();++i){
        WEDS& nowweds=fixLenPcliques[i].weds;
        WEDS pre,suf;
        pre.timeList.assign(nowweds.timeList.begin(),nowweds.timeList.end()-1);
        suf.timeList.assign(nowweds.timeList.begin()+1,nowweds.timeList.end());
        prefix[pre].push_back(i);
        suffix[suf].push_back(i);
    }

    int start=0,end=fixLenPcliques.size()-1;
    while(start<=end){//统计答案与生成新长度交替进行
        collectAns(fixLenPcliques,start,end,prefix,suffix,ans,percent);
        generateNewLength(kval,fixLenPcliques,start,end,prefix,suffix,percent);
        start=end+1;
        end=fixLenPcliques.size()-1;
    }
    //done
    
}

void Tempgraph::computeSingleIWPT(int nodeId,unordered_set<WEDS,hashWEDS>& theWPT,unordered_map<int,unordered_map<int,vector<WEDS>>>& theNodeToIWPT){
    unordered_set<WEDS,hashWEDS>::iterator it=theWPT.begin();
    theNodeToIWPT[nodeId]=unordered_map<int,vector<WEDS>>();
    while(it!=theWPT.end()){
        const vector<int>& plist=it->timeList;
        for(int i=0;i<plist.size();++i){
            theNodeToIWPT[nodeId][plist[i]].push_back(*it);
        }
        it++;
    }
}

void Tempgraph::updateNodeWPT(int nodeid,int kval,unordered_map<int,unordered_set<WEDS,hashWEDS>>& theNodeToWPT,unordered_map<int,unordered_map<int,vector<WEDS>>>& theNodeToIWPT,unordered_map<int,unordered_map<int,int> >& nodeToKeyTimeDegree,int eid){
    if(theNodeToWPT.find(nodeid)==theNodeToWPT.end()) return;//象征性的检查一下

    vector<int>& timelist=edgePool[eid];
    if(theNodeToIWPT.find(nodeid)==theNodeToIWPT.end()){
        computeSingleIWPT(nodeid,theNodeToWPT[nodeid],theNodeToIWPT);
    }

    unordered_map<int,int>& keyTimeDegree=nodeToKeyTimeDegree[nodeid];
    unordered_map<int,vector<WEDS>>& theIWPT=theNodeToIWPT[nodeid];
    unordered_set<WEDS,hashWEDS>& theWPT=theNodeToWPT[nodeid];

    for(int i=0;i<timelist.size();++i){
        if(keyTimeDegree.find(timelist[i])!=keyTimeDegree.end()){
            if(keyTimeDegree[timelist[i]]<kval) continue;//它已经不是keytime了

            if((--keyTimeDegree[timelist[i]])<kval){
                for(int j=0;j<theIWPT[timelist[i]].size();++j){
                    if(theWPT.find(theIWPT[timelist[i]][j])!=theWPT.end()){
                        theWPT.erase(theIWPT[timelist[i]][j]);
                        if(theWPT.size()==0) return;
                    }
                }
            }
        }
    }
}

void Tempgraph::computeSingleWPT(int nodeId,int sigma,int percent,int kval,unordered_map<int,unordered_set<WEDS,hashWEDS>>& theNodeToWPT,unordered_map<int,unordered_map<int,int> >& nodeToKeyTimeDegree){
    if(validv[nodeId]==false||theNodeToWPT.find(nodeId)!=theNodeToWPT.end()) return;
    theNodeToWPT[nodeId]=unordered_set<WEDS,hashWEDS>();

    //test varify pivot direct// only here
    // WEDS temp;
    // temp.timeList={1,2,3};
    // theNodeToWPT[nodeId]=unordered_set<WEDS,hashWEDS>({temp});
    // return;

    unordered_map<int,int> cntTimesOfTime;
    for(int i=0;i<adjList[nodeId].size();++i){
        if(validv[adjList[nodeId][i].first]==false) continue;
        int edgeid=adjList[nodeId][i].second;

        for(int j=0;j<edgePool[edgeid].size();++j){
            if(cntTimesOfTime.find(edgePool[edgeid][j])!=cntTimesOfTime.end()){
                cntTimesOfTime[edgePool[edgeid][j]]++;
            }else{
                cntTimesOfTime[edgePool[edgeid][j]]=1;
            }
        }
    }

    vector<int> availableTimeset;
    unordered_map<int,int>::iterator it=cntTimesOfTime.begin();
    while(it!=cntTimesOfTime.end()){
        if(it->second>=kval) availableTimeset.push_back(it->first);
        it++;
    }
    if(availableTimeset.size()<sigma) return;
    sort(availableTimeset.begin(),availableTimeset.end());
    
    unordered_set<WEDS,hashWEDS> &curSet=theNodeToWPT[nodeId];

    //debug
    computeTime++;
    // cout<<availableTimeset.size()<<endl;

    // //debug
    // if(availableTimeset.size()==100||availableTimeset.size()==512||availableTimeset.size()==1090){
    //     for(auto val:availableTimeset){
    //         cout<<val<<" ";
    //     }
    //     cout<<endl;
    // }

    #ifdef oracle
    computeWPTOracle(availableTimeset,sigma,percent,curSet);
    #endif
    #ifdef candque
    computeWPTque(availableTimeset,sigma,percent,curSet);
    #endif
    #ifdef candset
    computeWPTset(availableTimeset,sigma,percent,curSet);
    #endif
    // computeWPTEDS(availableTimeset,kval,sigma,percent,curSet,cntTimesOfTime);

    //准备nodeToKeyTimeDegree
    nodeToKeyTimeDegree[nodeId]=unordered_map<int,int>();
    unordered_set<WEDS,hashWEDS>::iterator that=curSet.begin();
    while(that!=curSet.end()){
        const vector<int>& plist=that->timeList;
        for(int i=0;i<plist.size();++i){
            nodeToKeyTimeDegree[nodeId][plist[i]]=cntTimesOfTime[plist[i]];
        }
        that++;
    }
}

void Tempgraph::WPNCluster(int sigma,int percent,int kval,unordered_map<int,unordered_set<WEDS,hashWEDS>>& theNodeToWPT,unordered_map<int,unordered_map<int,vector<WEDS>>>& theNodeToIWPT,unordered_map<int,unordered_map<int,int> >& theNodeToKeyTimeDegree){
    vector<pair<int,int>> leftNodeAndDegree;//pii 度数和节点mark
    for(int i=0;i<nodecnt;++i){
        if(validv[i]) leftNodeAndDegree.push_back(pair<int,int>(degree[i],i));
    }
    sort(leftNodeAndDegree.begin(),leftNodeAndDegree.end());

    queue<int> que;
    for(int i=0;i<leftNodeAndDegree.size();++i){
        int nownode=leftNodeAndDegree[i].second;
        if(validv[nownode]==false) continue;

        computeSingleWPT(nownode,sigma,percent,kval,theNodeToWPT,theNodeToKeyTimeDegree);
        if(theNodeToWPT[nownode].size()==0){
            que.push(nownode);
            validv[nownode]=false;
            degree[nownode]=0;
        }

        while(!que.empty()){

            int frontnode=que.front();que.pop();

            for(int j=0;j<adjList[frontnode].size();++j){//处理被删的frontnode的所有邻居
                pair<int,int> neighbor=adjList[frontnode][j];
                if(validv[neighbor.first]==false) continue;//邻居不存在
                degree[neighbor.first]--;
                if(degree[neighbor.first]==kval-1){//度数不满足
                    que.push(neighbor.first);
                    validv[neighbor.first]=false;
                    continue;
                }

                if(theNodeToWPT.find(neighbor.first)==theNodeToWPT.end()) continue;//wpt没被算过，所以无需更新wpt
                updateNodeWPT(neighbor.first,kval,theNodeToWPT,theNodeToIWPT,theNodeToKeyTimeDegree,neighbor.second);
                if(theNodeToWPT[neighbor.first].size()==0){
                    que.push(neighbor.first);
                    validv[neighbor.first]=false;
                    degree[neighbor.first]=0;
                }
            }
        }
    }
}

void Tempgraph::getPartAnsViaDfs(int start,int needLen,vector<int>& timeSeq,vector<pair<int,pair<int,int> > >& theDAG,vector<int>& thePosInDAG,vector<int>& theMaxLength,vector<int>& curPath,vector<vector<int> >& partAns){
    int frmIndex=theDAG[start].first;
    int l=theDAG[start].second.first;
    int r=theDAG[start].second.second;
    curPath.push_back(timeSeq[frmIndex]);
    if(needLen==1){
        partAns.push_back(curPath);
    }else if(needLen==2){
        while(l<=r){
            curPath.push_back(timeSeq[l]);
            partAns.push_back(curPath);
            curPath.pop_back();
            l++;
        }
    }else{
        needLen--;
        while(l<=r){
            if(theMaxLength[l]>=needLen){
                getPartAnsViaDfs(thePosInDAG[l],needLen,timeSeq,theDAG,thePosInDAG,theMaxLength,curPath,partAns);
            }
            l++;
        }
    }

    curPath.pop_back();
}

void Tempgraph::updateMaxLengthRev(int startNode,vector<int>& positionInSingleL,vector<int>& positionInSingleRevL,vector<pair<int,pair<int, int>>>& singleRevL,vector<int>& maxLengthRev,vector<int>& maxLength){

}

void Tempgraph::getAnsFrmLwithOptimize(unordered_map<int,vector<pair<int,pair<int,int> > > >& theL,int sigma,vector<int>& timeSeq,unordered_set<WEDS,hashWEDS>& holdSet){
    unordered_map<int,vector<pair<int,pair<int,int> > > >::iterator it=theL.begin();

    vector<int> maxLength(timeSeq.size());//正向的最长路径
    vector<int> maxLengthRev(timeSeq.size());//反向的最长路径
    vector<int> positionInSingleL(timeSeq.size());//正向的frmIndex在L中的位置//注意，叶节点不在这个数组里！！下同
    vector<int> positionInSingleRevL(timeSeq.size());//反向的frmIndex在RevL（利用L来构建）中的位置

    queue<int> updateQue;//删keypath后更新maxlengthrev用

    //debug
    int tempcnt=0;
    int temptimecnt=0;
    // struct timeval tstart,tend;

    while(it!=theL.end()){
        //debug
        #ifdef BAR
        tempcnt++;
        if(tempcnt%100==0) cerr<<tempcnt<<" of "<<theL.size()<<endl;
        #endif

        int curDiff=it->first;
        vector<pair<int,pair<int,int> > >& singleL=it->second;
        if(singleL.size()==0){
            it++;
            continue;
        }
        vector<pair<int,pair<int,int> > > singleRevL;
        vector<pair<int,int> > keyPath;//关键路径

        //清除所需位置的残留
        for(int i=0;i<singleL.size();++i){
            int l=singleL[i].second.first;
            int r=singleL[i].second.second;
            int frmIndex=singleL[i].first;
            positionInSingleL[frmIndex]=i;
            maxLengthRev[frmIndex]=1;

            if(timeSeq[l]-timeSeq[frmIndex]==curDiff){
                keyPath.push_back(pair<int,int>(frmIndex,l));//得到关键路径
            }

            // while(l<=r){
            //     maxLength[l]=1;
            //     positionInSingleRevL[l]=-1;
            //     l++;
            // }
        }

        for(int i=0;i<maxLength.size();++i){
            maxLength[i]=1;
            positionInSingleRevL[i]=-1;
        }

        if(keyPath.size()==0){//没有关键路径
            it++;
            continue;
        }

        for(int i=singleL.size()-1;i>=0;--i){
            int l=singleL[i].second.first;
            int r=singleL[i].second.second;
            int frmIndex=singleL[i].first;
            int longest=0;

            while(l<=r){
                if(longest<maxLength[r]){
                    longest=maxLength[r];
                }

                //build singleRevL
                if(positionInSingleRevL[r]==-1){
                    singleRevL.push_back(pair<int,pair<int,int> >(r,pair<int,int>(frmIndex,frmIndex)));
                    positionInSingleRevL[r]=singleRevL.size()-1;
                }else{
                    singleRevL[positionInSingleRevL[r]].second.first=frmIndex;//更新下界
                }
                r--;
            }

            maxLength[frmIndex]=longest+1;
        }

        //更新maxlengthrev
        for(int i=singleRevL.size()-1;i>=0;--i){
            int l=singleRevL[i].second.first;
            int r=singleRevL[i].second.second;
            int frmIndex=singleRevL[i].first;
            int longest=0;
            while(l<=r){
                if(longest<maxLengthRev[l]){
                    longest=maxLengthRev[l];
                }
                l++;
            }
            maxLengthRev[frmIndex]=longest+1;
        }

        //debug
        // gettimeofday(&tstart,NULL);
        
        for(int i=0;i<keyPath.size();++i){
            int longestTail=maxLength[keyPath[i].second]-1;
            int preval=0;
            if(longestTail<sigma-2){
                preval=sigma-2-longestTail;
            }
            for(int pre=preval;pre<=sigma-2;++pre){
                //pre为keypath前面还需多少个点
                int post=sigma-2-pre;//post为keypath后面还需多少个点
                if(maxLengthRev[keyPath[i].first]-1<pre) break;

                //获得两头的答案
                vector<vector<int> > preAns,postAns;
                vector<int> curPath;
                if(maxLengthRev[keyPath[i].first]<=1){
                    preAns.push_back(vector<int>({timeSeq[keyPath[i].first]}));
                }else{
                    getPartAnsViaDfs(positionInSingleRevL[keyPath[i].first],pre+1,timeSeq,singleRevL,positionInSingleRevL,maxLengthRev,curPath,preAns);
                }
                if(maxLength[keyPath[i].second]<=1){
                    postAns.push_back(vector<int>({timeSeq[keyPath[i].second]}));
                }else{
                    getPartAnsViaDfs(positionInSingleL[keyPath[i].second],post+1,timeSeq,singleL,positionInSingleL,maxLength,curPath,postAns);
                }
                //拼装答案
                if(postAns.size()==0) continue;//实际上不可能
                for(int j=0;j<preAns.size();++j){
                    reverse(preAns[j].begin(),preAns[j].end());
                    for(int k=0;k<postAns.size();++k){
                        WEDS singleAns;
                        vector<int>& theTimeList=singleAns.timeList;

                        theTimeList=preAns[j];
                        theTimeList.resize(sigma);
                        for(int o=pre+1;o<sigma;++o) theTimeList[o]=postAns[k][o-pre-1];
                        holdSet.insert(singleAns);

                        // //test
                        // if(theTimeList[0]==1&&theTimeList[1]==11&&theTimeList[2]==21&&theTimeList[3]==31){
                        //     for(int v=0;v<singleL.size();++v){
                        //         cout<<singleL[v].first<<"->["<<singleL[v].second.first<<","<<singleL[v].second.second<<"] ";
                        //     }
                        //     cout<<"-----"<<endl;
                        // }
                    }
                }
            }

            continue;//不删边，不更新maxlengthrev

            //删掉当前keypath//其实可以判断一下是不是一定删，最坏情况是一定删
            if(i==keyPath.size()-1) break;//最后一个keypath不用删了
            singleL[positionInSingleL[keyPath[i].first]].second.first++;
            singleRevL[positionInSingleRevL[keyPath[i].second]].second.second--;

            // 更新maxlengthrev
            // updateQue.push(keyPath[i].second);
            // while(!updateQue.empty()){
            //     int nowNode=updateQue.front();
            //     updateQue.pop();

            //     int frmPosRev=positionInSingleRevL[nowNode];
            //     int l=singleRevL[frmPosRev].second.first;
            //     int r=singleRevL[frmPosRev].second.second;
            //     int longest=0;
            //     while(l<=r){
            //         if(maxLengthRev[l]>longest){
            //             longest=maxLengthRev[l];
            //         }
            //         l++;
            //     }

            //     if(maxLengthRev[nowNode]!=longest+1){
            //         maxLengthRev[nowNode]=longest+1;
            //         if(maxLength[nowNode]>1){//有祖先存在
            //             int frmPos=positionInSingleL[nowNode];
            //             l=singleL[frmPos].second.first;
            //             r=singleL[frmPos].second.second;
            //             while(l<=r){
            //                 updateQue.push(l);l++;
            //             }
            //         }
            //     }
            // }

            //更新maxlengthrev的可能改进
            updateQue.push(keyPath[i].second);
            while(!updateQue.empty()){
                int nownode=updateQue.front();updateQue.pop();

                int frmPosRev=positionInSingleRevL[nownode];
                int l=singleRevL[frmPosRev].second.first;
                int r=singleRevL[frmPosRev].second.second;
                int longest=0;
                while(l<=r){
                    if(maxLengthRev[l]>longest) longest=maxLengthRev[l];
                    l++;
                }

                if(maxLengthRev[nownode!=longest+1]){
                    maxLengthRev[nownode]=longest+1;
                    if(longest+1<sigma-1){//它的后继的maxlength可能变成sigma-1。如果只是变成sigma及以上的话，不用更新//唯一的区别
                        if(maxLength[nownode]>1){//有祖先存在
                            int frmPos=positionInSingleL[nownode];
                            l=singleL[frmPos].second.first;
                            r=singleL[frmPos].second.second;
                            while(l<=r){
                                updateQue.push(l);l++;
                            }
                        }
                    }
                }
            }

        }

        //debug
        // gettimeofday(&tend,NULL);
        // temptimecnt+=(tend.tv_sec-tstart.tv_sec)*1000+(tend.tv_usec-tstart.tv_usec)/1000;
        
        it++;
    }

    //debug
    // cerr<<"part time: "<<temptimecnt<<endl;
}

void Tempgraph::getAnsFrmLwithRepeat(unordered_map<int,vector<pair<int,pair<int,int> > > >& theL,int sigma,vector<int>& timeSeq,unordered_set<WEDS,hashWEDS>& holdSet){
    unordered_map<int,vector<pair<int,pair<int,int> > > >::iterator it=theL.begin();

    vector<int> maxLength(timeSeq.size());//正向的最长路径
    vector<int> maxLengthRev(timeSeq.size());//反向的最长路径
    vector<int> positionInSingleL(timeSeq.size());//正向的frmIndex在L中的位置//注意，叶节点不在这个数组里！！下同
    vector<int> positionInSingleRevL(timeSeq.size());//反向的frmIndex在RevL（利用L来构建）中的位置

    queue<int> updateQue;//删keypath后更新maxlengthrev用

    //debug
    // int tempcnt=0;

    while(it!=theL.end()){
        //debug
        // tempcnt++;
        // if(tempcnt%100==0) cerr<<tempcnt<<" of "<<theL.size()<<endl;

        int curDiff=it->first;
        vector<pair<int,pair<int,int> > >& singleL=it->second;
        if(singleL.size()==0){
            it++;
            continue;
        }
        vector<pair<int,pair<int,int> > > singleRevL;
        vector<pair<int,int> > keyPath;//关键路径

        //清除所需位置的残留
        for(int i=0;i<singleL.size();++i){
            int l=singleL[i].second.first;
            int r=singleL[i].second.second;
            int frmIndex=singleL[i].first;
            positionInSingleL[frmIndex]=i;
            maxLengthRev[frmIndex]=1;

            if(timeSeq[l]-timeSeq[frmIndex]==curDiff){
                keyPath.push_back(pair<int,int>(frmIndex,l));//得到关键路径
            }

            while(l<=r){
                maxLength[l]=1;
                positionInSingleRevL[l]=-1;
                l++;
            }
        }

        if(keyPath.size()==0){//没有关键路径
            it++;
            continue;
        }

        for(int i=singleL.size()-1;i>=0;--i){
            int l=singleL[i].second.first;
            int r=singleL[i].second.second;
            int frmIndex=singleL[i].first;
            int longest=0;

            while(l<=r){
                if(longest<maxLength[r]){
                    longest=maxLength[r];
                }

                //build singleRevL
                if(positionInSingleRevL[r]==-1){
                    singleRevL.push_back(pair<int,pair<int,int> >(r,pair<int,int>(frmIndex,frmIndex)));
                    positionInSingleRevL[r]=singleRevL.size()-1;
                }else{
                    singleRevL[positionInSingleRevL[r]].second.first=frmIndex;//更新下界
                }
                r--;
            }

            maxLength[frmIndex]=longest+1;
        }

        //更新maxlengthrev
        for(int i=singleRevL.size()-1;i>=0;--i){
            int l=singleRevL[i].second.first;
            int r=singleRevL[i].second.second;
            int frmIndex=singleRevL[i].first;
            int longest=0;
            while(l<=r){
                if(longest<maxLengthRev[l]){
                    longest=maxLengthRev[l];
                }
                l++;
            }
            maxLengthRev[frmIndex]=longest+1;
        }

        for(int i=0;i<keyPath.size();++i){
            int longestTail=maxLength[keyPath[i].second]-1;
            int preval=0;
            if(longestTail<sigma-2){
                preval=sigma-2-longestTail;
            }
            for(int pre=preval;pre<=sigma-2;++pre){
                //pre为keypath前面还需多少个点
                int post=sigma-2-pre;//post为keypath后面还需多少个点
                if(maxLengthRev[keyPath[i].first]-1<pre) break;

                //获得两头的答案
                vector<vector<int> > preAns,postAns;
                vector<int> curPath;
                if(maxLengthRev[keyPath[i].first]<=1){
                    preAns.push_back(vector<int>({timeSeq[keyPath[i].first]}));
                }else{
                    getPartAnsViaDfs(positionInSingleRevL[keyPath[i].first],pre+1,timeSeq,singleRevL,positionInSingleRevL,maxLengthRev,curPath,preAns);
                }
                if(maxLength[keyPath[i].second]<=1){
                    postAns.push_back(vector<int>({timeSeq[keyPath[i].second]}));
                }else{
                    getPartAnsViaDfs(positionInSingleL[keyPath[i].second],post+1,timeSeq,singleL,positionInSingleL,maxLength,curPath,postAns);
                }
                //拼装答案
                if(postAns.size()==0) continue;//实际上不可能
                for(int j=0;j<preAns.size();++j){
                    reverse(preAns[j].begin(),preAns[j].end());
                    for(int k=0;k<postAns.size();++k){
                        WEDS singleAns;
                        vector<int>& theTimeList=singleAns.timeList;

                        theTimeList=preAns[j];
                        theTimeList.resize(sigma);
                        for(int o=pre+1;o<sigma;++o) theTimeList[o]=postAns[k][o-pre-1];
                        holdSet.insert(singleAns);

                        // //test
                        // if(theTimeList[0]==1&&theTimeList[1]==11&&theTimeList[2]==21&&theTimeList[3]==31){
                        //     for(int v=0;v<singleL.size();++v){
                        //         cout<<singleL[v].first<<"->["<<singleL[v].second.first<<","<<singleL[v].second.second<<"] ";
                        //     }
                        //     cout<<"-----"<<endl;
                        // }
                    }
                }
            }

            continue;//不删边，不更新maxlengthrev

            //删掉当前keypath//其实可以判断一下是不是一定删，最坏情况是一定删
            if(i==keyPath.size()-1) break;//最后一个keypath不用删了
            singleL[positionInSingleL[keyPath[i].first]].second.first++;
            singleRevL[positionInSingleRevL[keyPath[i].second]].second.second--;

            // 更新maxlengthrev
            // updateQue.push(keyPath[i].second);
            // while(!updateQue.empty()){
            //     int nowNode=updateQue.front();
            //     updateQue.pop();

            //     int frmPosRev=positionInSingleRevL[nowNode];
            //     int l=singleRevL[frmPosRev].second.first;
            //     int r=singleRevL[frmPosRev].second.second;
            //     int longest=0;
            //     while(l<=r){
            //         if(maxLengthRev[l]>longest){
            //             longest=maxLengthRev[l];
            //         }
            //         l++;
            //     }

            //     if(maxLengthRev[nowNode]!=longest+1){
            //         maxLengthRev[nowNode]=longest+1;
            //         if(maxLength[nowNode]>1){//有祖先存在
            //             int frmPos=positionInSingleL[nowNode];
            //             l=singleL[frmPos].second.first;
            //             r=singleL[frmPos].second.second;
            //             while(l<=r){
            //                 updateQue.push(l);l++;
            //             }
            //         }
            //     }
            // }

            //更新maxlengthrev的可能改进
            updateQue.push(keyPath[i].second);
            while(!updateQue.empty()){
                int nownode=updateQue.front();updateQue.pop();

                int frmPosRev=positionInSingleRevL[nownode];
                int l=singleRevL[frmPosRev].second.first;
                int r=singleRevL[frmPosRev].second.second;
                int longest=0;
                while(l<=r){
                    if(maxLengthRev[l]>longest) longest=maxLengthRev[l];
                    l++;
                }

                if(maxLengthRev[nownode!=longest+1]){
                    maxLengthRev[nownode]=longest+1;
                    if(longest+1<sigma-1){//它的后继的maxlength可能变成sigma-1。如果只是变成sigma及以上的话，不用更新//唯一的区别
                        if(maxLength[nownode]>1){//有祖先存在
                            int frmPos=positionInSingleL[nownode];
                            l=singleL[frmPos].second.first;
                            r=singleL[frmPos].second.second;
                            while(l<=r){
                                updateQue.push(l);l++;
                            }
                        }
                    }
                }
            }

        }

        it++;
    }
}

void Tempgraph::getAnsFrmLnoRepeat(unordered_map<int,vector<pair<int,pair<int,int> > > >& theL,int sigma,vector<int>& timeSeq,unordered_set<WEDS,hashWEDS>& holdSet){
    unordered_map<int,vector<pair<int,pair<int,int> > > >::iterator it=theL.begin();

    vector<int> maxLength(timeSeq.size());//正向的最长路径
    vector<int> maxLengthRev(timeSeq.size());//反向的最长路径
    vector<int> positionInSingleL(timeSeq.size());//正向的frmIndex在L中的位置//注意，叶节点不在这个数组里！！下同
    vector<int> positionInSingleRevL(timeSeq.size());//反向的frmIndex在RevL（利用L来构建）中的位置

    queue<int> updateQue;//删keypath后更新maxlengthrev用

    //debug
    int tempcnt=0;
    struct timeval start,end;

    while(it!=theL.end()){
        //debug
        #ifdef BAR
        tempcnt++;
        if(tempcnt%100==0) cerr<<tempcnt<<" of "<<theL.size()<<endl;
        #endif

        int curDiff=it->first;
        vector<pair<int,pair<int,int> > >& singleL=it->second;
        if(singleL.size()==0){
            it++;
            continue;
        }
        vector<pair<int,pair<int,int> > > singleRevL;
        vector<pair<int,int> > keyPath;//关键路径

        //清除所需位置的残留
        for(int i=0;i<singleL.size();++i){
            int l=singleL[i].second.first;
            int r=singleL[i].second.second;
            int frmIndex=singleL[i].first;
            positionInSingleL[frmIndex]=i;
            maxLengthRev[frmIndex]=1;

            if(timeSeq[l]-timeSeq[frmIndex]==curDiff){
                keyPath.push_back(pair<int,int>(frmIndex,l));//得到关键路径
            }

            while(l<=r){
                maxLength[l]=1;
                positionInSingleRevL[l]=-1;
                l++;
            }
        }

        if(keyPath.size()==0){//没有关键路径
            it++;
            continue;
        }

        for(int i=singleL.size()-1;i>=0;--i){
            int l=singleL[i].second.first;
            int r=singleL[i].second.second;
            int frmIndex=singleL[i].first;
            int longest=0;

            while(l<=r){
                if(longest<maxLength[r]){
                    longest=maxLength[r];
                }

                //build singleRevL
                if(positionInSingleRevL[r]==-1){
                    singleRevL.push_back(pair<int,pair<int,int> >(r,pair<int,int>(frmIndex,frmIndex)));
                    positionInSingleRevL[r]=singleRevL.size()-1;
                }else{
                    singleRevL[positionInSingleRevL[r]].second.first=frmIndex;//更新下界
                }
                r--;
            }

            maxLength[frmIndex]=longest+1;
        }

        //更新maxlengthrev
        for(int i=singleRevL.size()-1;i>=0;--i){
            int l=singleRevL[i].second.first;
            int r=singleRevL[i].second.second;
            int frmIndex=singleRevL[i].first;
            int longest=0;
            while(l<=r){
                if(longest<maxLengthRev[l]){
                    longest=maxLengthRev[l];
                }
                l++;
            }
            maxLengthRev[frmIndex]=longest+1;
        }

        for(int i=0;i<keyPath.size();++i){
            int longestTail=maxLength[keyPath[i].second]-1;
            int preval=0;
            if(longestTail<sigma-2){
                preval=sigma-2-longestTail;
            }
            for(int pre=preval;pre<=sigma-2;++pre){
                //pre为keypath前面还需多少个点
                int post=sigma-2-pre;//post为keypath后面还需多少个点
                if(maxLengthRev[keyPath[i].first]-1<pre) break;

                //获得两头的答案
                vector<vector<int> > preAns,postAns;
                vector<int> curPath;
                if(maxLengthRev[keyPath[i].first]<=1){
                    preAns.push_back(vector<int>({timeSeq[keyPath[i].first]}));
                }else{
                    getPartAnsViaDfs(positionInSingleRevL[keyPath[i].first],pre+1,timeSeq,singleRevL,positionInSingleRevL,maxLengthRev,curPath,preAns);
                }
                if(maxLength[keyPath[i].second]<=1){
                    postAns.push_back(vector<int>({timeSeq[keyPath[i].second]}));
                }else{
                    getPartAnsViaDfs(positionInSingleL[keyPath[i].second],post+1,timeSeq,singleL,positionInSingleL,maxLength,curPath,postAns);
                }
                //拼装答案
                if(postAns.size()==0) continue;//实际上不可能

                //debug
                #ifdef combine_time
                gettimeofday(&start,NULL);
                #endif
                
                for(int j=0;j<preAns.size();++j){
                    reverse(preAns[j].begin(),preAns[j].end());
                    for(int k=0;k<postAns.size();++k){
                        WEDS singleAns;
                        vector<int>& theTimeList=singleAns.timeList;

                        theTimeList.resize(sigma);
                        // for(int l=preAns[j].size()-1,o=0;l>=0;--l,++o) theTimeList[o]=preAns[j][l];
                        for(int l=0;l<preAns[j].size();++l) theTimeList[l]=preAns[j][l];
                        // theTimeList=preAns[j];
                        // theTimeList.resize(sigma);
                        for(int o=pre+1;o<sigma;++o) theTimeList[o]=postAns[k][o-pre-1];
                        holdSet.insert(singleAns);

                        // //test
                        // if(theTimeList[0]==1&&theTimeList[1]==11&&theTimeList[2]==21&&theTimeList[3]==31){
                        //     for(int v=0;v<singleL.size();++v){
                        //         cout<<singleL[v].first<<"->["<<singleL[v].second.first<<","<<singleL[v].second.second<<"] ";
                        //     }
                        //     cout<<"-----"<<endl;
                        // }
                    }
                }
            }

            //删掉当前keypath//其实可以判断一下是不是一定删，最坏情况是一定删
            if(i==keyPath.size()-1) break;//最后一个keypath不用删了
            singleL[positionInSingleL[keyPath[i].first]].second.first++;
            singleRevL[positionInSingleRevL[keyPath[i].second]].second.second--;

            // 更新maxlengthrev
            // updateQue.push(keyPath[i].second);
            // while(!updateQue.empty()){
            //     int nowNode=updateQue.front();
            //     updateQue.pop();

            //     int frmPosRev=positionInSingleRevL[nowNode];
            //     int l=singleRevL[frmPosRev].second.first;
            //     int r=singleRevL[frmPosRev].second.second;
            //     int longest=0;
            //     while(l<=r){
            //         if(maxLengthRev[l]>longest){
            //             longest=maxLengthRev[l];
            //         }
            //         l++;
            //     }

            //     if(maxLengthRev[nowNode]!=longest+1){
            //         maxLengthRev[nowNode]=longest+1;
            //         if(maxLength[nowNode]>1){//有祖先存在
            //             int frmPos=positionInSingleL[nowNode];
            //             l=singleL[frmPos].second.first;
            //             r=singleL[frmPos].second.second;
            //             while(l<=r){
            //                 updateQue.push(l);l++;
            //             }
            //         }
            //     }
            // }

            //更新maxlengthrev的可能改进
            updateQue.push(keyPath[i].second);
            while(!updateQue.empty()){
                int nownode=updateQue.front();updateQue.pop();

                int frmPosRev=positionInSingleRevL[nownode];
                int l=singleRevL[frmPosRev].second.first;
                int r=singleRevL[frmPosRev].second.second;
                int longest=0;
                while(l<=r){
                    if(maxLengthRev[l]>longest) longest=maxLengthRev[l];
                    l++;
                }

                if(maxLengthRev[nownode]!=longest+1){
                    maxLengthRev[nownode]=longest+1;
                    if(longest+1<sigma-1){//它的后继的maxlength可能变成sigma-1。如果只是变成sigma及以上的话，不用更新//唯一的区别
                        if(maxLength[nownode]>1){//有祖先存在
                            int frmPos=positionInSingleL[nownode];
                            l=singleL[frmPos].second.first;
                            r=singleL[frmPos].second.second;
                            while(l<=r){
                                updateQue.push(l);l++;
                            }
                        }
                    }
                }
            }

        }

        it++;
    }
}

void Tempgraph::computeWPTOracle(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet){

    if(timeList.size()<sigma) return;

    #ifdef build_oracle_time
    struct timeval start,end;
    #endif

    unordered_map<int,vector<pair<int,pair<int,int> > > > L;//pair<int,pair<int,int> >意思是timestamp的下标和它的activate区间（下标表示）
    
    #ifdef build_oracle_time
    gettimeofday(&start,NULL);
    #endif
    buildOracle(percent,timeList,L);
    
    #ifdef build_oracle_time
    gettimeofday(&end,NULL);
    cout<<"build oracle time: "<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;
    #endif

    getAnsFrmLnoRepeat(L,sigma,timeList,holdSet);
    // getAnsFrmLwithOptimize(L,sigma,timeList,holdSet);
    // getAnsFrmLwithRepeat(L,sigma,timeList,holdSet);

}

void Tempgraph::buildOracle(int percent,vector<int>& timeSeq,unordered_map<int,vector<pair<int,pair<int,int> > > >& theL){
    unordered_map<int,unordered_map<int,int>> lpos;
    unordered_set<int> dSet;
    for(int i=0;i<timeSeq.size()-1;++i){
        for(int j=i+1;j<timeSeq.size();++j){
            int d=timeSeq[j]-timeSeq[i];
            dSet.insert(d);
            if(lpos.find(d)==lpos.end()){
                lpos[d]=unordered_map<int,int>();
            }
        }
    }

    for(int i=0;i<timeSeq.size()-1;++i){
        for(int j=i+1;j<timeSeq.size();++j){
            int d=timeSeq[j]-timeSeq[i];
            int top=100*d;
            int bot=100+percent;
            int l=top/bot,r=d;
            if(top%bot!=0) l++;

            for(int d=l;d<=r;++d){
                if(dSet.find(d)!=dSet.end()){
                    vector<pair<int,pair<int,int> > >& singleList=theL[d];
                    unordered_map<int,int>& singlePos=lpos[d];

                    if(singlePos.find(i)==singlePos.end()){
                        singleList.push_back(pair<int,pair<int,int>>(i,pair<int,int>(j,j)));
                        singlePos[i]=singleList.size()-1;
                    }else{
                        int pos=singlePos[i];
                        singleList[pos].second.second=j;
                    }
                }
            }
        }
    }
}

void Tempgraph::computeWPTEDS(vector<int>& timeList,int kval,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet,unordered_map<int,int>& cntTimesOfTime){
    //不管percent是多少，都当成等差数列
    if(timeList.size()<sigma) return;

    for(int i=0;i+1<timeList.size();++i){
        for(int j=i+1;j<timeList.size();++j){
            int diff=timeList[j]-timeList[i];
            int followingval=timeList[j];
            bool yes=true;
            for(int k=0;k<sigma-2;++k){
                followingval+=diff;
                if(cntTimesOfTime.find(followingval)==cntTimesOfTime.end()||cntTimesOfTime[followingval]<kval){
                    yes=false;
                    break;
                }
            }
            if(yes){
                WEDS tempWEDS;
                // WEDS tempWEDS(availableTimeSet[i],difference,len);
                int start=timeList[i];
                for(int k=0;k<sigma;++k){
                    tempWEDS.timeList.push_back(start);
                    start+=diff;
                }
                holdSet.insert(tempWEDS);
            }
        }
    }
}

void Tempgraph::computeWPTque(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet){
    //需要检查调用的必要性，比如timelist不存在，或已经被计算
    
    if(timeList.size()<sigma) return;

    deque<WEDS> candidateQue;
    for(int i=1;i<timeList.size();++i){//从第二个开始才能形成和处理候选集
        int queSize=candidateQue.size();
        while(queSize--){
            // T nowDiff=holdTime[i]-it->timeList[it->timeList.size()-1];
            WEDS frontItem=candidateQue.front();
            candidateQue.pop_front();
            int nowDiff=timeList[i]-frontItem.timeList[frontItem.timeList.size()-1];
            if(nowDiff<frontItem.botDiff){//突破下界
                Frac newTop((100+percent)*nowDiff,100);//新的上界

                if(newTop>=frontItem.topDiff){//还符合极差极限
                    WEDS newer=frontItem;
                    newer.botDiff=nowDiff;
                    newer.timeList.push_back(timeList[i]);

                    if(newer.timeList.size()==sigma){
                        holdSet.insert(newer);//找到一个
                    }else{
                        candidateQue.push_back(newer);
                    }
                }
                candidateQue.push_back(frontItem);
            }else if(nowDiff>frontItem.topDiff){//突破最大值
                Frac oldTop((100+percent)*frontItem.botDiff,100);//原来的上界
                if(oldTop>=nowDiff){
                    WEDS newer=frontItem;
                    newer.topDiff=nowDiff;
                    newer.timeList.push_back(timeList[i]);

                    if(newer.timeList.size()==sigma){
                        holdSet.insert(newer);
                    }else{
                        candidateQue.push_back(newer);
                    }

                    if(!(oldTop==nowDiff)){
                        candidateQue.push_back(frontItem);//frontItem本人还有机会
                    }//else frontItem没机会了
                }//else frontItem没机会了
            }else{
                WEDS newer=frontItem;
                newer.timeList.push_back(timeList[i]);

                if(newer.timeList.size()==sigma){
                    holdSet.insert(newer);
                }else{
                    candidateQue.push_back(newer);
                }
                candidateQue.push_back(frontItem);
            }
        }

        if(i<timeList.size()-1){//最后一个timestamp就不用了，除非要长度为2的近似周期
            for(int j=0;j<i;++j){
                WEDS newer;
                newer.timeList={timeList[j],timeList[i]};
                newer.topDiff=newer.botDiff=timeList[i]-timeList[j];
                candidateQue.push_back(newer);
            }
        }
    }

}

void Tempgraph::computeWPTset(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet){
    //需要检查调用的必要性，比如timelist不存在，或已经被计算
    if(timeList.size()<sigma) return;

    unordered_set<WEDS,hashWEDS> candidateSet;
    for(int i=1;i<timeList.size();++i){//从第二个开始才能形成和处理候选集

        //debug
        #ifdef BAR
        if(i%10==0) cerr<<i<<" of "<<timeList.size()<<endl;
        #endif

        unordered_set<WEDS,hashWEDS>::iterator it=candidateSet.begin();
        vector<WEDS> waitToBeAdd;
        vector<WEDS> waitToBeDelete;
        while(it!=candidateSet.end()){
            int nowDiff=timeList[i]-it->timeList[it->timeList.size()-1];
            if(nowDiff<it->botDiff){//突破下界
                Frac tempTop((100+percent)*nowDiff,100);//暂时的新上界
                if(tempTop>=it->topDiff){//还符合极差极限
                    WEDS newer=*it;
                    newer.botDiff=nowDiff;
                    newer.timeList.push_back(timeList[i]);

                    if(newer.timeList.size()==sigma){
                        holdSet.insert(newer);//找到一个
                    }else{
                        waitToBeAdd.push_back(newer);//等待被添加到set里
                    }
                }
            }else if(nowDiff>it->topDiff){//突破上界
                Frac oldTop((100+percent)*it->botDiff,100);
                if(oldTop>=nowDiff){
                    WEDS newer=*it;
                    newer.topDiff=nowDiff;
                    newer.timeList.push_back(timeList[i]);

                    if(newer.timeList.size()==sigma){
                        holdSet.insert(newer);//找到一个
                    }else{
                        waitToBeAdd.push_back(newer);
                    }

                    if(oldTop==nowDiff){
                        waitToBeDelete.push_back(*it);//it本人已经没机会了，即使它的后代有机会
                    }
                }else{
                    waitToBeDelete.push_back(*it);//it本人已经没机会了
                }
            }else{
                WEDS newer=*it;
                newer.timeList.push_back(timeList[i]);

                if(newer.timeList.size()==sigma){
                    holdSet.insert(newer);
                }else{
                    waitToBeAdd.push_back(newer);
                }
            }

            it++;
        }

        for(int j=0;j<waitToBeDelete.size();++j){
            candidateSet.erase(waitToBeDelete[j]);
        }
        for(int j=0;j<waitToBeAdd.size();++j){
            candidateSet.insert(waitToBeAdd[j]);
        }

        if(i<timeList.size()-1){//最后一个timestamp就不用了，除非要长度为2的近似周期
            for(int j=0;j<i;++j){
                WEDS newer;
                newer.timeList={timeList[j],timeList[i]};
                newer.topDiff=newer.botDiff=timeList[i]-timeList[j];
                candidateSet.insert(newer);
            }
        }
    }

}

void Tempgraph::readGraph(string filename){
    ifstream fin;
    fin.open(filename,ios::binary);
    if(!fin.is_open()){
        cerr<<"can not open file"<<endl;
        return;
    }

    //获得文件大小
    ll totalSize=fin.seekg(0,ios::end).tellg();
    char *buf=new char[totalSize+5];

    fin.seekg(0,ios::beg).read(buf,totalSize);
    fin.close();

    //处理buf里的数据，添加所有边
    bool isWin=false;
    vector<int> holdLine;
    vector<vector<int> > holdLines;
    int val=0;
    for(ll i=0;i<totalSize;++i){
        switch (buf[i])
        {
        case 32:
            {
                holdLine.push_back(val);
                val=0;
            }
        break;
        case 10:
            {
                if(!isWin){
                    holdLine.push_back(val);
                    val=0;
                    holdLines.push_back(holdLine);
                    holdLine.clear();
                }
            }
        break;
        case 13:
            {
                holdLine.push_back(val);
                val=0;
                holdLines.push_back(holdLine);
                holdLine.clear();
                isWin=true;
            }
        break;
        default:
            {
                int tval=buf[i]-'0';
                val=val*10+tval;
            }
        break;
        }
    }
    //最后一条边，当不以新行结尾时
    if(holdLine.size()>0){
        holdLine.push_back(val);
        holdLines.push_back(holdLine);
    }

    delete buf;
    cerr<<(isWin?"win read":"linux read")<<endl;

    //真正的构建图
    for(int i=0;i<holdLines.size();++i){
        int a=holdLines[i][0];
        int b=holdLines[i][1];
        int t=holdLines[i][2];

        if(adjMatrix.find(a)==adjMatrix.end()||adjMatrix[a].find(b)==adjMatrix[a].end()){
            adjMatrix[a][b]=adjMatrix[b][a]=edgecnt++;
            edgePool.push_back(vector<int>());

            int bigvertex=a;
            if(b>bigvertex) bigvertex=b;
            if(adjList.size()<=bigvertex){
                adjList.resize(bigvertex+1);
            }

            adjList[a].push_back(pair<int,int>(b,edgecnt-1));
            adjList[b].push_back(pair<int,int>(a,edgecnt-1));
        }

        int edgemark=adjMatrix[a][b];
        edgePool[edgemark].push_back(t);
    }

    //每条边的timestamp排序，去重
    for(int i=0;i<edgePool.size();++i){
        sort(edgePool[i].begin(),edgePool[i].end());
        int numT=unique(edgePool[i].begin(),edgePool[i].end())-edgePool[i].begin();
        edgePool[i].resize(numT);
    }

    //为每条边的timestamp建立set
    // edgePoolTimeSet.resize(edgePool.size());
    // for(int i=0;i<edgePool.size();++i){
    //     for(int j=0;j<edgePool[i].size();++j){
    //         edgePoolTimeSet[i].insert(edgePool[i][j]);
    //     }
    // }

    validv.resize(adjList.size(),true);
    valide.resize(edgePool.size(),true);
    nodecnt=adjList.size();

    //read done

}

void Tempgraph::computeDegree(){
    degree.resize(adjList.size());
    for(int i=0;i<adjList.size();++i) degree[i]=adjList[i].size();
}

void Tempgraph::computeKcore(int kval){
    queue<int> que;
    for(int i=0;i<nodecnt;++i){
        if(validv[i]&&degree[i]<kval){
            validv[i]=false;
            que.push(i);
        }
    }

    while(!que.empty()){
        int nownode=que.front();que.pop();

        for(int i=0;i<adjList[nownode].size();++i){
            pair<int,int> neighbor=adjList[nownode][i];
            if(validv[neighbor.first]==false) continue;

            valide[neighbor.second]=false;//delete edge
            degree[neighbor.first]--;
            if(degree[neighbor.first]==kval-1){
                validv[neighbor.first]=false;
                que.push(neighbor.first);
            }
        }
    }
}
