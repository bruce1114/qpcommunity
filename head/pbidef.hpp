#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<queue>
#define ll long long

using namespace std;

struct EDS{
    int start,diff;
    int len;
    EDS(int _start=-1,int _diff=-1,int _len=-1):
    start(_start),diff(_diff),len(_len){}

    bool operator == (const EDS& b)const{
        return start==b.start&&diff==b.diff;
    }

    bool operator <(const EDS& b)const{
        if(start==b.start) return diff<b.diff;
        return start<b.start;
    }
};

struct hashEDS{
    size_t operator ()(const EDS& theEDS)const{
        hash<int> hasher=hash<int>();
        return hasher(theEDS.start)^hasher(theEDS.diff);
    }
};

struct PureWEDS{//与下面的结构相似
    vector<int> timeList;

    bool operator == (const PureWEDS& b)const{
        if(timeList.size()==b.timeList.size()){
            for(int i=0;i<timeList.size();++i){
                if(timeList[i]!=b.timeList[i]) return false;
            }
            return true;
        }else return false;
    }

    bool operator < (const PureWEDS& b)const{
        for(int i=0;i<timeList.size()&&i<b.timeList.size();++i){
            if(timeList[i]!=b.timeList[i]) return timeList[i]<b.timeList[i];
        }
        return timeList.size()<b.timeList.size();
    }
};

struct hashPureWEDS{
    size_t operator ()(const PureWEDS& theWEDS)const{
        size_t res=0;
        hash<int> hasher=hash<int>();
        for(int i=0;i<theWEDS.timeList.size();++i){
            res=res^hasher(theWEDS.timeList[i]);
        }
        return res;
    }
};

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

class TempBipartite{
public:

    int bound;
    vector<vector<int> > adjList;
    unordered_map<int,unordered_map<int,int> > adjMatrix;
    vector<vector<int> > edgePool;//with timestamp list
    int edgeCnt;
    // vector<int> degreea,degreeb;
    vector<int> degree;
    // vector<bool> valida,validb;
    vector<bool> validv,valide;

    TempBipartite();
    //txt文件，每一行的格式：frm to t
    void readGraph(string fileName);
    void addEdge(vector<int>& line);
    void readedGraph(vector<vector<int> >& holdLines);
    void computeDegree();
    void handleTimestamps();
    void cleanEdges(int sigma);
    void computePQcore(int p,int q);
    void periodPruneVertex();
    void computeVertexPT(int vid,vector<vector<int> >& adjList);
    void computeVertexDegreeTS(int vid,vector<vector<int> >& adjList,vector<int>& holdTS);
    void computeWPTque(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet);//这两个候选集实现方法不同
    void computeWPTset(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet);//这两个候选集实现方法不同
    void computeWPTAmir(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet);
    void buildLwithDlist(int percent,vector<int>& timeSeq,vector<int>& dList,unordered_map<int,vector<pair<int,pair<int,int> > > >& theL);
    void getAnsFrmLnoRepeat(unordered_map<int,vector<pair<int,pair<int,int> > > >& theL,int sigma,vector<int>& timeSeq,unordered_set<WEDS,hashWEDS>& holdSet);
    void getPartAnsViaDfs(int start,int needLen,vector<int>& timeSeq,vector<pair<int,pair<int,int> > >& theDAG,vector<int>& thePosInDAG,vector<int>& theMaxLength,vector<int>& curPath,vector<vector<int> >& partAns);

    void completeStaticPrune(int sigma);
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

void TempBipartite::completeStaticPrune(int sigma){
    for(int i=0;i<edgePool.size();++i){
        if(edgePool[i].size()<sigma){
            valide[i]=false;
        }
    }

    
}

void TempBipartite::getPartAnsViaDfs(int start,int needLen,vector<int>& timeSeq,vector<pair<int,pair<int,int> > >& theDAG,vector<int>& thePosInDAG,vector<int>& theMaxLength,vector<int>& curPath,vector<vector<int> >& partAns){
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

void TempBipartite::getAnsFrmLnoRepeat(unordered_map<int,vector<pair<int,pair<int,int> > > >& theL,int sigma,vector<int>& timeSeq,unordered_set<WEDS,hashWEDS>& holdSet){
    unordered_map<int,vector<pair<int,pair<int,int> > > >::iterator it=theL.begin();

    vector<int> maxLength(timeSeq.size());//正向的最长路径
    vector<int> maxLengthRev(timeSeq.size());//反向的最长路径
    vector<int> positionInSingleL(timeSeq.size());//正向的frmIndex在L中的位置//注意，叶节点不在这个数组里！！下同
    vector<int> positionInSingleRevL(timeSeq.size());//反向的frmIndex在RevL（利用L来构建）中的位置

    queue<int> updateQue;//删keypath后更新maxlengthrev用

    while(it!=theL.end()){
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

            //删掉当前keypath//其实可以判断一下是不是一定删，最坏情况是一定删
            if(i==keyPath.size()-1) break;//最后一个keypath不用删了
            singleL[positionInSingleL[keyPath[i].first]].second.first++;
            singleRevL[positionInSingleRevL[keyPath[i].second]].second.second--;

            updateQue.push(keyPath[i].second);
            while(!updateQue.empty()){
                int nowNode=updateQue.front();
                updateQue.pop();

                int frmPosRev=positionInSingleRevL[nowNode];
                int l=singleRevL[frmPosRev].second.first;
                int r=singleRevL[frmPosRev].second.second;
                int longest=0;
                while(l<=r){
                    if(maxLengthRev[l]>longest){
                        longest=maxLengthRev[l];
                    }
                    l++;
                }

                if(maxLengthRev[nowNode]!=longest+1){
                    maxLengthRev[nowNode]=longest+1;
                    if(maxLength[nowNode]>1){//有祖先存在
                        int frmPos=positionInSingleL[nowNode];
                        l=singleL[frmPos].second.first;
                        r=singleL[frmPos].second.second;
                        while(l<=r){
                            updateQue.push(l);l++;
                        }
                    }
                }
            }
        }

        it++;
    }
}

void TempBipartite::buildLwithDlist(int percent,vector<int>& timeSeq,vector<int>& dList,unordered_map<int,vector<pair<int,pair<int,int> > > >& theL){
    //参数vector<int> dList主要在生成L时，不必要遍历dlist，而是二分查找到对应的dlist区间

    for(int i=0;i<timeSeq.size()-1;++i){

        //每个元素的phase 1
        vector<pair<int,pair<Frac,int> > > diffRangeVec;//pair<int,pair<Frac,int>>指i到int(index)这个diff所拥有的d范围
        for(int j=i+1;j<timeSeq.size();++j){
            int actualDiff=timeSeq[j]-timeSeq[i];
            diffRangeVec.push_back(pair<int,pair<Frac,int> >(j,pair<Frac,int>(Frac(actualDiff*100,100+percent),actualDiff)));//还没考虑分数的化简//但后面只用到了分数与整数的比较，风险较小
        }

        //每个元素的phase 2
        vector<pair<pair<int,int>,pair<Frac,Frac> > > separateDiffRange;

        //初始化
        int firstRange=0,lastRange=1;
        int firstRangeL,lastRangeR;
        Frac l,r;
        l=diffRangeVec[0].second.first;

        while(lastRange<diffRangeVec.size()){
            firstRangeL=diffRangeVec[firstRange].first;
            lastRangeR=diffRangeVec[lastRange-1].first;

            if(diffRangeVec[lastRange].second.first<=diffRangeVec[firstRange].second.second){
                //有公共区间
                r=diffRangeVec[lastRange].second.first;
                lastRange++;
            }else{
                r=diffRangeVec[firstRange].second.second;
                firstRange++;
            }

            separateDiffRange.push_back(pair<pair<int,int>,pair<Frac,Frac> >(pair<int,int>(firstRangeL,lastRangeR),pair<Frac,Frac>(l,r)));
            l=r;

            if(firstRange==lastRange){//与初始化时类似
                lastRange++;
                l=diffRangeVec[firstRange].second.first;
            }
        }

        while(firstRange<lastRange){//收尾工作
            r=diffRangeVec[firstRange].second.second;
            separateDiffRange.push_back(pair<pair<int,int>,pair<Frac,Frac> >(pair<int,int>(diffRangeVec[firstRange].first,diffRangeVec[lastRange-1].first),pair<Frac,Frac>(l,r)));
            l=r;
            firstRange++;
        }

        //将phase 2的结果加入L里
        int lastEnd=0;
        for(int j=0;j<separateDiffRange.size();++j){
            //获取左右整数边界
            int leftLimit=separateDiffRange[j].second.first.getUp();
            int rightLimit=separateDiffRange[j].second.second.getDown();

            if(leftLimit>rightLimit) continue;

            int leftLimitInDlist=lower_bound(dList.begin()+lastEnd,dList.end(),leftLimit)-dList.begin();
            // int leftLimitInDlist=lower_bound(dList.begin(),dList.end(),leftLimit)-dList.begin();
            if(leftLimitInDlist==dList.size()) break;
            int rightLimitInDlist=upper_bound(dList.begin()+leftLimitInDlist,dList.end(),rightLimit)-dList.begin();
            // int rightLimitInDlist=upper_bound(dList.begin(),dList.end(),rightLimit)-dList.begin();
            rightLimitInDlist--;

            lastEnd=rightLimitInDlist;
            if(leftLimitInDlist>rightLimitInDlist) continue;//区间内部没有d

            //判断左边界是否与上一个的右边界重合并且是否将左边界加入L，因为有可能上一个的右边界更符合
            if(j>0&&separateDiffRange[j].second.first==dList[leftLimitInDlist]){
                if(separateDiffRange[j-1].second.second==dList[leftLimitInDlist]){
                    if(separateDiffRange[j].first.second>=separateDiffRange[j-1].first.second&&separateDiffRange[j].first.first<=separateDiffRange[j-1].first.first){
                        theL[dList[leftLimitInDlist]].push_back(pair<int,pair<int,int> >(i,separateDiffRange[j].first));
                    }
                    leftLimitInDlist++;
                }
            }

            if(leftLimitInDlist>rightLimitInDlist) continue;//原始区间中，上一个的尾巴等于下一个的头时形成的长度为1的区间//不全是这种

            //对于中间的属于D(S)的整数，直接加入L
            while(leftLimitInDlist<rightLimitInDlist){
                theL[dList[leftLimitInDlist]].push_back(pair<int,pair<int,int> >(i,separateDiffRange[j].first));
                leftLimitInDlist++;
            }

            //与上上段相似
            if(j+1<separateDiffRange.size()&&separateDiffRange[j].second.second==dList[rightLimitInDlist]){
                if(separateDiffRange[j+1].second.first==dList[rightLimitInDlist]){
                    if(separateDiffRange[j].first.second>=separateDiffRange[j+1].first.second&&separateDiffRange[j].first.first<=separateDiffRange[j+1].first.first){
                        theL[dList[rightLimitInDlist]].push_back(pair<int,pair<int,int> >(i,separateDiffRange[j].first));
                    }
                    leftLimitInDlist++;
                }
            }

            if(leftLimitInDlist==rightLimitInDlist){//如果rightlimit不和下一个接壤，别忘了正规地处理rightlimit
                theL[dList[leftLimitInDlist]].push_back(pair<int,pair<int,int> >(i,separateDiffRange[j].first));
            }
        }
    }
}

void TempBipartite::computeWPTAmir(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet){

    if(timeList.size()<sigma) return;

    unordered_map<int,vector<pair<int,pair<int,int> > > > L;//pair<int,pair<int,int> >意思是timestamp的下标和它的activate区间（下标表示）
    //获得D(S)
    vector<int> dList;//只用于buildLwithDlist
    for(int i=0;i+1<timeList.size();++i){
        for(int j=i+1;j<timeList.size();++j){
            int theDiff=timeList[j]-timeList[i];
            if(L.find(theDiff)==L.end()){
                dList.push_back(theDiff);
                L[theDiff]=vector<pair<int,pair<int,int> > >();
            }
        }
    }

    sort(dList.begin(),dList.end());
    buildLwithDlist(percent,timeList,dList,L);

    getAnsFrmLnoRepeat(L,sigma,timeList,holdSet);

}

void TempBipartite::computeWPTque(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet){
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

void TempBipartite::computeWPTset(vector<int>& timeList,int sigma,int percent,unordered_set<WEDS,hashWEDS>& holdSet){
    //需要检查调用的必要性，比如timelist不存在，或已经被计算
    if(timeList.size()<sigma) return;

    unordered_set<WEDS,hashWEDS> candidateSet;
    for(int i=1;i<timeList.size();++i){//从第二个开始才能形成和处理候选集
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

void TempBipartite::computeVertexDegreeTS(int vid,vector<vector<int> >& adjList,vector<int>& holdTS){
    unordered_map<int,int> cntTimesOfTime;
    for(int i=0;i<adjList[vid].size();++i){
        int neiId=adjList[vid][i];

    }
}

void TempBipartite::computeVertexPT(int vid,vector<vector<int> >& adjList){

}

TempBipartite::TempBipartite(){
    edgeCnt=0;
    bound=0;
}

void TempBipartite::cleanEdges(int sigma){
    for(int i=0;i<bound;++i){
        int a=i;
        for(int j=0;j<adjList[a].size();++j){
            int b=adjList[a][j];
            int eid=adjMatrix[a][b];
            if(edgePool[eid].size()<sigma){
                valide[eid]=false;
                degree[a]--;
                degree[b]--;
            }
        }
    }
}

void TempBipartite::periodPruneVertex(){
    unordered_map<int,unordered_set<EDS,hashEDS> > vertexaToPT,vertexbToPT;

}

void TempBipartite::computeDegree(){
    for(int i=0;i<adjList.size();++i) degree.push_back(adjList[i].size());
}

void TempBipartite::handleTimestamps(){
    for(int i=0;i<edgePool.size();++i){
        sort(edgePool[i].begin(),edgePool[i].end());
        int numT=unique(edgePool[i].begin(),edgePool[i].end())-edgePool[i].begin();
        edgePool[i].resize(numT);
    }
}

void TempBipartite::readedGraph(vector<vector<int> >& holdLines){
    bound=0;
    for(int i=0;i<holdLines.size();++i){
        if(bound<holdLines[i][0]){
            bound=holdLines[i][0];
        }
    }

    bound++;
    for(int i=0;i<holdLines.size();++i){
        int a=holdLines[i][0];
        int b=holdLines[i][1];
        b=b+bound;
        int t=holdLines[i][2];

        if(adjMatrix.find(a)==adjMatrix.end()||adjMatrix[a].find(b)==adjMatrix[a].end()){
            adjMatrix[a][b]=adjMatrix[b][a]=edgeCnt++;
            edgePool.push_back(vector<int>());

            if(adjList.size()<=b){
                adjList.resize(b+1);
            }
            adjList[a].push_back(b);
            adjList[b].push_back(a);
        }

        int edgeMark=adjMatrix[a][b];
        edgePool[edgeMark].push_back(t);
    }
}

void TempBipartite::readGraph(string fileName){
    ifstream fin;
    fin.open(fileName,ios::binary);
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
    if(holdLine.size()==3){
        holdLines.push_back(holdLine);
        holdLine.clear();
    }

    delete buf;
    cerr<<(isWin?"win read":"linux read")<<endl;

    //真正地构建图
    readedGraph(holdLines);
    //每条边的timestamp排序，去重
    handleTimestamps();
    //得到底图的节点度数
    computeDegree();

    validv.resize(adjList.size(),true);
    valide.resize(edgePool.size(),true);

    //read done
}

void TempBipartite::computePQcore(int p,int q){

    queue<int> que;
    for(int i=0;i<adjList.size();++i){
        if(i<bound){
            if(degree[i]<q) que.push(i);
        }else{
            if(degree[i]<p) que.push(i);
        }
    }

    while(!que.empty()){
        int nowid=que.front();que.pop();
        validv[nowid]=false;//delete nowid
        for(int i=0;i<adjList[nowid].size();++i){
            int neiId=adjList[nowid][i];
            if(valide[adjMatrix[nowid][neiId]]==false||validv[neiId]==false) continue;//边或点已经被删除，实际上只需判断边
            degree[neiId]--;
            valide[adjMatrix[nowid][neiId]]=false;

            if(neiId<bound){
                if(degree[neiId]==q-1) que.push(neiId);
            }else{
                if(degree[neiId]==p-1) que.push(neiId);
            }
            
        }
    }

    // queue<int> quea,queb;
    // for(int i=0;i<vertexa.size();++i){
    //     if(degreea[i]<q) quea.push(i);
    // }
    // for(int i=0;i<vertexb.size();++i){
    //     if(degreeb[i]<p) queb.push(i);
    // }

    // while(!quea.empty()||!queb.empty()){
    //     while(!quea.empty()){
    //         //会不会已经被删除？不会，因为入队的那一排节点里不会有一个节点如两次队，因为只有度数在(p/q)-1时入队
    //         int cura=quea.front();quea.pop();
    //         valida[cura]=false;//delete cura

    //         for(int i=0;i<vertexa[cura].size();++i){
    //             int neib=vertexa[cura][i];
    //             if(valide[adjMatrix[cura][neib]]==false||validb[neib]==false) continue;//边或点已经被删除，实际上只需判断边

    //             degreeb[neib]--;
    //             valide[adjMatrix[cura][neib]]=false;
    //             if(degreeb[neib]==p-1) queb.push(neib);
    //         }
    //     }

    //     //逻辑相同
    //     while(!queb.empty()){
    //         //会不会已经被删除？不会，因为入队的那一排节点里不会有一个节点如两次队，因为只有度数在(p/q)-1时入队
    //         int curb=queb.front();queb.pop();
    //         validb[curb]=false;

    //         for(int i=0;i<vertexb[curb].size();++i){
    //             int neia=vertexb[curb][i];
    //             if(valide[adjMatrix[neia][curb]]==false||valida[neia]==false) continue;

    //             degreea[neia]--;
    //             valide[adjMatrix[neia][curb]]=false;
    //             if(degreea[neia]==q-1) quea.push(neia);
    //         }
    //     }
    // }
}