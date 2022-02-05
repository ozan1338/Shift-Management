#include <iostream>
#include <assert.h>
#include "WorkCal.h"
#include "NP/NPQueue.h"
#ifndef SHIFT_H
#define SHIFT_H
using namespace std;

class Shift{
  public:
    int days;                          // num of work days
    int N;                             // num of staff
    int k;                             // min offdays required per staff
    int aw;                            // min staff required per day
    int curr;                          // working day
    int num_shifts;                    // num of shifts in a day
    bool doubleshift;  
    WorkCal cal;                
    int** shifts;                      // matrix of day staff shifts
    NPQueue<int, int> priorStaff;      // priorityqueue for staffs
    NPQueue<int, int> priorDay;        // priorityqueue for days

  public:
    Shift(){}
    Shift(int days, int N, int k, int aw, int num_shifts){
      set(days, N, k, aw, num_shifts);
      schedule();
    }
    ~Shift(){
      for(int i = 0; i < days; ++i) {
        delete [] shifts[i];
      }
      delete [] shifts;
    }
    void set(int days, int N, int k, int aw, int num_shifts){
      assert(k<days && aw < N && num_shifts<=4);
      this->days = days;                  
      this->k = k;                        
      this->N = N;                        
      this->aw = aw;                      
      this->num_shifts = num_shifts;     
      this->doubleshift = num_shifts > 2; // enable double shift if num_shifts>2
      this->curr = 0; 
      this->cal = WorkCal();                    
      this->shifts = new int*[days+2];    
      for (int j=0; j<days+2; j++){
        shifts[j] = new int[N+1];
      }
	}
    // schedule shift
    void schedule(){
      int val[4] = {3,5,7,11};//[N,D,E,M]           // prime no representation for shifts
      for (int i=0; i<days; i++){                   // set num staff to all work day to total staff
        shifts[i][N] = N;
      }
      int s;
      num_shifts>2 ? s=1:s=0;
      for (int i=0; i<N; i++){;
        for (int j=0; j<days; j++){                  // assign shifts to staffs 
            shifts[j][i] = val[(i+s*j%7)%num_shifts];// with shift circulation and same weakday shift
        }
        for (int j=0; j<k; j++){                    // get k off day for each staff
          int off = (i+j*(days/k))%days;
          shifts[off][i] = 0;                       // offday = 0
          shifts[off][N] -= 1;                      // decrement num staff in offday
        }
        shifts[days][i] = days-k;                   // set num shift worked for staff i to days-k
        shifts[days+1][i] = k;                      // set total off days for staff i to k
      }
      setPriority();                                // set priorities for days and staff
      if ((N*(days-k)/days)>=aw){
        for (int i=0; i<days; i++)
          if(shifts[i][N] < aw){ fill(i+1); }
      }else if (doubleshift){                      // double shift to satisfy constraint aw
        for (int i=0; i<days; i++)
          if(shifts[i][N] < aw){ doubleShift(i+1); }
      }else{
      	cout << "Requirements cannot be fully satisfied!" <<endl;
	  }
    }
    

    // get leave for staff 
    void leave(int wid, int day){
      assert(curr<=day-1 && day-1>=0 && day-1<days && wid>=0 && wid<N);
      // if staff has shift
      if (shifts[day-1][wid] > 1){
        leavehelp(wid, day);
      }
      shifts[day-1][wid] = -1;                             // leave representation = -1
    }
    // find off day for staff
    int getdayoff(int wid){
      assert(wid>=0 && wid<N);                                 
      int day = priorDay.top();
      for (int i=0; i<priorDay.length()-1; i++){          // look in priorDay for offday
        if (day>=curr && shifts[day][wid]>0 &&            // staff has to have only one shift on that day
         shifts[day][wid]<=11) { 
           dayoffhelp(day, wid);
           return day+1;
		    }
        day = priorDay.getNext();
      }
      return -1;
    }
    
    // fill day with more staff 
    int fill(int day){
      assert(curr<=day-1 && day-1>=0 && day-1<days);
      int s = priorStaff.top();
      //int pos=priorStaff.currPos();
      
      for (int i=0; i<N-1; i++){                         // look in priorStaff
        if (shifts[day-1][s]==0){                        // find staff that has day off
          fillhelp(day, s);
          return s;
        }
		    s = priorStaff.getNext();
      }
      
      if (doubleshift){                                 // make doubleshift if staff not found and doubleshift allowed
		return doubleShift(day);
      }
      return -1;
    }
    
    // make staff work double shift
    int doubleShift(int day){
      assert(curr<=day-1 && day-1>=0 && day-1<days);
      if(!doubleshift){ return -1; }                       
      int s = priorStaff.top();
      for (int i=0; i<N-1; i++){                           // look in priorStaff
        if (shifts[day-1][s]>1 && shifts[day-1][s]<=11){ // find staff having only one shift    
		      doublehelp(day, s);
		      return s;
		    }
        s = priorStaff.getNext();
      }
      return -1;
    }
    
    // move to next workday
    void next(){
      assert(curr+1<days);
      priorDay.remove(curr, shifts[curr][N]);                 // remove passed day from priority queue
      curr++;
    }

    // get current work day
    int getWorkday(){
      return curr+1;
    }
    // get shifts for day
    int* getDayShift(int day){
      assert(day-1>=0 && day-1<days);
      return shifts[day-1];
    }
    // get staff shift
    int* getStaffShift(int wid){
      assert(wid>=0 && wid<N);
      int* staffshift = new int[days];
      for (int i=0; i<days; i++){
          staffshift[i] = shifts[i][wid];
      }
      return staffshift;
    }
    // get complete work shift
    int** getShift(){
      return shifts;
    }
    // set start date
    void setStartDate(int year, int month, int day ){
      cal = WorkCal(year, month, day);
    }  
    // get workday calander
    WorkCal shiftcal(){
      return cal;
    }
  private:
    // set priority for staffs and days
    void setPriority(){
      for (int i=0; i<N; i++){                               // set priorityqueue for staff
        priorStaff.enqueue(i, days-k);
      } 
      for (int j=0; j<days; j++){                            // set priorityqueue for workday
        priorDay.enqueue(j, shifts[j][N]);
      }
      priorStaff.top();
      priorDay.top();
    }
    void leavehelp(int wid, int day){
  	  int ns = 1;                                         // num of shift for staff wid on day
      shifts[day-1][wid] > 11 ? ns=2:ns=ns;
      shifts[days+1][wid] += 1;                           // increase off day for staff
      int prior = 2*(days-k)-shifts[days][wid];           // current priority of staff
      shifts[days][wid]-=ns;priorStaff.pup(wid,prior,ns); // decrease shift worked and increase priority for staff by ns
      priorDay.pdown(day-1, shifts[day-1][N], ns);        // decrease priority for day by ns
	    shifts[day-1][N] -=ns;                              // decrease num staff 
	    shifts[day-1][wid] = -1; 
      if(shifts[day-1][N]<aw) { fill(day); }              // fill day if num staff in day less than req 
	  }
    void dayoffhelp(int day, int wid){
  	  shifts[day][wid] = 1;                                // mark dayoff by 1
      shifts[days+1][wid] += 1;                            // increase offdays for staff by 1
      priorDay.pdown(day, shifts[day][N]);                 //decrease priority for day
      shifts[day][N] -= 1;                                 // decrease shift covered 
      if (shifts[day][N]<aw){ fill(day+1);}                // fill day if day has less shift covered than aw
      int prior = 2*(days-k)-shifts[days][wid];            // current priority of staff
      shifts[days][wid] -= 1; priorStaff.pup(wid,prior);   // decrease shift worked and increase priority for staff
	  }
	  void fillhelp(int day, int s){
  	  int val[4] = {3,5,7,11};
      shifts[day-1][s] = val[(s+(day-1)%7)%num_shifts];    // fill staff day with shift 
      shifts[days+1][s] -= 1;                              // decrease offdays for staff by 1
      int prior = 2*(days-k)-shifts[days][s];              // current priority of staff
      int pvd = shifts[day-1][N];
      shifts[day-1][N] += 1;                               // increase shift covered for day
	    shifts[days][s] += 1; priorStaff.pdown(s, prior);    // increase shift worked and decrease priority for staff
      if (shifts[days+1][s]<k){ getdayoff(s); }            // find dayoff for staff if staff has less off day than required
      priorDay.pup(day-1, pvd);                            // increase priority for day 
      if (shifts[day-1][N]<aw){ fill(day);}                // fill day if day has less shift covered than required
	  }
	  void doublehelp(int day, int s){
    	int val[4] = {3,5,7,11};
    	shifts[day-1][s] *= val[(s+(day-1)%7+1)%num_shifts];  // fill staff day with another shift
    	int prior = 2*(days-k)-shifts[days][s];               // current priority of staff
      shifts[days][s] += 1; priorStaff.pdown(s, prior);     // increase shift worked and decrease priority for staff
      priorDay.pup(day-1, shifts[day-1][N]);                // increase priority for day
      shifts[day-1][N] += 1;                                // increase shift covered 
      if (shifts[day-1][N]<aw){ doubleShift(day);}          // make double shift if day still has less less shift than aw
	  }   
 };
#endif
