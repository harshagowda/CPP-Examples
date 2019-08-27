#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <iterator>
using namespace std;


//static mutex thread_m;

mutex lock_print;
volatile atomic<bool> list_in_use;
mutex lock_cv;
std::condition_variable dont_loop_wait;

list<tuple<int,int>> pt;
volatile atomic<int> counter=0;
auto thFunction=[](int threadid){
    
    while(list_in_use)
    {
        //wait loop need to avoided

        counter++;
        if(pt.size())
        {
            
            
            auto first = pt.begin();
            auto [number,thread_id]=*first;
            if(thread_id==threadid)
            {

                    lock_guard<mutex> lock(lock_print);
                
                    cout<<"Thread "<<to_string(threadid)<<" printing "<<number<<endl;
                    pt.erase(first);
                    
                    if(pt.size()==0)
                    {
                        list_in_use=false;
                    }
                    dont_loop_wait.notify_all();

                
            }
            else
            {
                //reduce wait loop sleep on condition variable
                unique_lock<mutex> lockc(lock_cv);
                dont_loop_wait.wait(lockc);
            }
        }

    }

};

#define NUM_THREADS 7



int main(int argc, const char * argv[]) {
 
    
    // can pass diffrent objects and there functions if object there has to be a static member for locking
    //thread_m.lock();
    
    list_in_use=true;
    vector<thread> allthreads;
    
    for(int num_thread_counter=0;num_thread_counter<NUM_THREADS;num_thread_counter++)
    {
        allthreads.emplace_back(thread(thFunction,num_thread_counter));
    }
    
    
    //add task in queue
    for(int i=0;i<=100;i++)
    {
  
        for(int num_thread_counter=0;num_thread_counter<NUM_THREADS;num_thread_counter++)
        {
            if(i%NUM_THREADS ==num_thread_counter)
            {
                pt.emplace_back(make_tuple(i,num_thread_counter));
            }
        }
        
    }
    
    dont_loop_wait.notify_all();
    
    for(auto a_thread=allthreads.begin();a_thread!=allthreads.end();a_thread++)
    {
        (*a_thread).join();
    }
    
    cout<<"counter "<<counter<<endl;
    return 0;
}
