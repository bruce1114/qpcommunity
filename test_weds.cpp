// #ifdef amir
// #ifdef candque
// #ifdef candset
// #ifdef REPORT
// #ifdef BAR
// #ifdef build_oracle_time
// #ifdef combine_time
// #ifdef buildl_naive
#define buildl_naive
#define build_oracle_time
#define oracle
// #define candidate

#include"head/def.hpp"
#include<sys/time.h>
#include<unistd.h>



void readFile(string filename,vector<int>& hold){
    ifstream fin;
    fin.open(filename.c_str(),ios::in);
    int val;
    while(fin>>val){
        hold.push_back(val);
    }
}

int main(){
    // cerr<<"update maxlengthrev optimization"<<endl;
    // cerr<<"getAnsFrmLwithRepeat"<<endl;
    // cerr<<"getAnsFrmLnoRepeat"<<endl;
    // cerr<<"getAnsFrmLwithOptimize"<<endl;
    Tempgraph tb;
    struct timeval start,end;

    string filename="../dataset/weds/randseq_1000_50";
    // cout<<"enter dataset name: ../dataset/weds/?"<<endl;
    cin>>filename;
    filename="../dataset/weds/"+filename;

    vector<int> timeList;
    readFile(filename,timeList);

    unordered_set<WEDS,hashWEDS> resultst1,resultst2,resultst3;
    int sigma=3;
    int percent=20;
    // cout<<"enter sigma and percent?"<<endl;
    cin>>sigma>>percent;

    // int times;
    // cin>>times;

    #ifdef oracle
    gettimeofday(&start,NULL);
    // for(int i=0;i<times;++i) tb.computeWPTAmir(timeList,sigma,percent,resultst3),resultst3.clear();
    tb.computeWPTAmir(timeList,sigma,percent,resultst3);
    gettimeofday(&end,NULL);

    cout<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<" "<<endl;

    #endif

    // cout<<resultst3.size()<<endl;

    // if(percent==0){
    //     unordered_map<int, int> cntTimesOfTime;
    //     for(auto val:timeList){
    //         cntTimesOfTime[val]=1;
    //     }
    //     gettimeofday(&start,NULL);
    //     // tb.computeWPTque(timeList,sigma,percent,resultst1);
    //     tb.computeWPTEDS(timeList,1,sigma,percent,resultst1,cntTimesOfTime);
    //     gettimeofday(&end,NULL);

    //     cout<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<endl;
    // }


    #ifdef candidate
    gettimeofday(&start,NULL);
    // for(int i=0;i<times;++i) tb.computeWPTset(timeList,sigma,percent,resultst2), resultst2.clear();
    tb.computeWPTset(timeList,sigma,percent,resultst2);
    gettimeofday(&end,NULL);

    cout<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<" "<<endl;
    #endif


    //检查是否一样
    #ifdef oracle
    #ifdef candidate
    if(percent==0){
        if(resultst1.size()==resultst2.size()){
            unordered_set<WEDS,hashWEDS>::iterator it=resultst1.begin();
            while(it!=resultst1.end()){
                if(resultst2.find(*it)==resultst2.end()){
                    cout<<"1 and 2 are different"<<endl;
                    break;
                }
                it++;
            }
            cout<<"1 and 2 are the same with ans size: "<<resultst1.size()<<endl;
        }else{
            cout<<"1 and 2 are different"<<endl;
        }
    }

    gettimeofday(&start,NULL);

    bool same=true;
    if(resultst2.size()==resultst3.size()){
        unordered_set<WEDS,hashWEDS>::iterator it=resultst2.begin();
        
        while(it!=resultst2.end()){
            if(resultst3.find(*it)==resultst3.end()){
                // cout<<"2 and 3 are different"<<endl;
                // for(auto val:it->timeList){
                //     cout<<val<<" ";
                // }
                // cout<<endl;
                same=false;
                break;
            }
            it++;
        }
    }else{
        same=false;
    }

    gettimeofday(&end,NULL);
    if(!same) cout<<"different ";
    cout<<resultst3.size()<<" ";

    cout<<(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000<<" "<<endl;
    #endif
    #endif

    sleep(1);
}
