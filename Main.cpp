// Interface module
#include <iostream>
#include <iomanip>
#include "Shift.h"
#include "WorkCal.h"
using namespace std;

string marker(int s){
  string m = "";
  if(s==0 || s==1) {return "X";}
  if(s==-1) {return "L";}
  if(s%3 ==0) { m = 'N'+m;}
  if(s%5 ==0) { m = 'D'+m;}
  if(s%7 ==0) { m = 'M'+m;}
  if(s%11 ==0){ m = 'E'+m;}  
  return m;
}
string week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

class ShiftManagement {
public:
	Shift shift;
	int days, N, k, aw, s;
public:
	ShiftManagement(){
		cout << "Shift Management\n"<<endl;
		cout<<"No of days :";
		cin>> days;
		cout<<"No of Staffs: ";
		cin>> N;
		cout<<"Min offdays for staff: ";
		cin>> k;
		cout<<"Min Staffs required per day: ";
		cin>> aw;
		cout<<"Number of shifts: ";
		cin>> s;
		shift.set(days, N, k, aw, s);
		shift.schedule();
		SetStartDate();
		Display();
		choice();
	}
	void choice(){
		int ch;
		cout << "Options: \n"
		     << "  1: ApplyLeave\n"
		     << "  2: FillDay\n" 
		     << "  3: Display Schedule\n" 
		     << "  4: Display Day Schedule\n"
		     << "  5: Display Staff Schedule\n"
		     << "  6: Exit\n"
	         << ">>>>";
		cin >> ch;
		switch(ch){
			case 1:
			    ApplyLeave();
			    choice();
			case 2:
			    FillDay();
			    choice();
			case 3:
				Display();
				choice();
			case 4:
				DaySchedule();
				choice();
			case 5:
				StaffSchedule();
				choice();
			case 6:
			    exit(0);
			default:
			    cout <<"Invalid Input!";
			    choice();
		}
	}
	void Display(){
	  std::cout<< "    Staff Schedule" << std::endl;
	  std::cout<<"\n                Staffs" << std::endl;
	  std::cout<<std::setw(11)<<"Date" <<std::setw(5)<<"Days";
	  for (int i=0;i<N;i++){  std::cout <<"S" <<std::setw(3)<< std::left <<i;}
	  std::cout<<"SC"<<endl;
	  // print shift for staff
	  for (int j=0;j<days;j++){
	  	Date temp = shift.shiftcal().workday(j+1);
	    std::cout <<std::left<<std::setw(2)<<temp.Day()<<"/" <<std::setw(2)<<temp.Month()
	              <<'['<<week[temp.Weekday()]<<std::setw(2)<<']'
	              <<std::setw(5)<<j+1;
	    for (int i=0;i<N;i++){ 
	      std::cout <<std::setw(4)<<std::left<<marker(shift.getShift()[j][i]);
	    }
	    std::cout <<std::setw(4)<<std::left<<shift.getShift()[j][N];
	    std::cout<<endl;
	  }
	  // print total shift worked by staffs
	  std::cout << std::setw(16)<<std::left<<"ShiftsWorked";
	  for (int i=0;i<N;i++){ 
	      std::cout <<std::setw(4)<<std::left<<shift.getShift()[days][i];
	  }
	  std::cout<<endl;
	  // print total off days for staffs
	  std::cout <<std::setw(16)<<std::left<<"Offdays";
	  for (int i=0;i<N;i++){ 
	      std::cout <<std::setw(4)<<std::left<<shift.getShift()[days+1][i];
	  }
	  std::cout<<endl <<endl;
	}
	void FillDay(){
		int day;
		cout<<"Day: ";
		cin >> day;
		cout << "Filled by Staff "<< shift.fill(day) <<endl;
	}
	void ApplyLeave(){
		int sid, day;
		cout<<"Staff ID: ";
		cin >> sid;
		cout<<"Day: ";
		cin >> day;
		shift.leave(sid, day);
		cout << "Leave Processed.\n";
	}
	void SetStartDate(){
		int year, month, day;
		cout<<"Start Date:\n";
		cout<<"Year Month Day: ";
		cin >> year >> month >> day;
		shift.setStartDate(year, month, day);
		cout << "Date Set\n";
	}
	void DaySchedule(){
		int day;
		cout << "Day: ";
		cin >> day;
		Date temp = shift.shiftcal().workday(day);
		std::cout<<"\n                Staffs" << std::endl;
	    std::cout<<std::setw(11)<<"Date" <<std::setw(5)<<"Day";
	    for (int i=0;i<N;i++){  std::cout <<"S" <<std::setw(3)<< std::left <<i;}
	    std::cout<<"SC"<<endl;
		
	    std::cout <<std::left<<std::setw(2)<<temp.Day()<<"/" <<std::setw(2)<<temp.Month()
	              <<'['<<week[temp.Weekday()]<<std::setw(2)<<']'
	              <<std::setw(5)<<day;
	    for (int i=0;i<N;i++){ 
	      std::cout <<std::setw(4)<<std::left<<marker(shift.getShift()[day-1][i]);
	    }
	    std::cout <<std::setw(4)<<std::left<<shift.getShift()[day-1][N];
	    std::cout<<endl<<endl;	
	}
	void StaffSchedule(){
		int staff;
		cout << "Staff: ";
		cin >> staff;
		std::cout<<"\n              Workday" <<endl;
		for (int i=0;i<days;i++){ 
	      std::cout <<std::setw(4)<<std::left<<i+1;
	    }
	    std::cout << std::endl;
	    for (int i=0;i<days;i++){ 
	      std::cout <<std::setw(4)<<std::left<<marker(shift.getShift()[i][staff]);
	    }
	    std::cout << std::endl;
	    std::cout <<"Shift Covered: "<<shift.getShift()[days][staff]<<std::endl;
	    std::cout <<"Total Dayoff: "<<shift.getShift()[days+1][staff]<<std::endl;
	    std::cout<<endl;	
	}
	
};
int main(){
	ShiftManagement interface = ShiftManagement();
}
