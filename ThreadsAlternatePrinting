
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <list>

using namespace std;

static mutex lock_print;

list<tuple<int,bool>> pt;

auto thFunction=[](int threadid){
    
    //wait like a job Q till list is empty 
    while(pt.size())
    {
        auto [number,thread_id]=pt.front();
        
        //if the top of the list belongs to current thread
        
        if(thread_id==threadid)
        {
            //lock and print
            lock_guard<mutex> lock(lock_print);
            cout<<"Thread "<<to_string(threadid)<<" printing "<<number<<endl;
            
            //erase the front of the list so that alternate thread can pickup from list
            pt.erase(pt.begin());
        }//auto unlocks
    }
    

};


int main(int argc, const char * argv[]) {
 
    
    // can pass diffrent objects and there functions if object there has to be a static member for locking

    //create two threads.
    thread th1(thFunction,0);
    thread th2(thFunction,1);
    
    for(int i=0;i<=100;i++)
    {
  
        //assign task marking who should pickup  
        if(i%2 ==0)
        {
            
            pt.emplace_back(make_tuple(i,0));
        }
        else
        {
            pt.emplace_back(make_tuple(i,1));
        }
         //thJoin(pt,PARELLEL_THREADS);
    }
    
    
    
     th1.join();
     th2.join();
    
    return 0;
}
