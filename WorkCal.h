// module for workday calander
#include <ctime>
#include <iostream>
#include <string>
#ifndef WORKCAL_H
#define WORKCAL_H


// Date Class
class Date { 
    public:
    	// member variables
        int day, month, year, weekday; 
    public:
    	// constructor
        Date(){ 
           std::time_t t = std::time(0);   
           std::tm* now = std::localtime(&t);
           this->day = now->tm_mday;
           this->month = now->tm_mon + 1;
           this->year = now->tm_year + 1900;
           this->weekday = now->tm_wday;
           
        }
    	Date(int year, int month, int day ){
           std::tm time_in = { 0, 0, 0, day, month-1, year - 1900 }; 
           std::time_t time_temp = std::mktime(&time_in);
           const std::tm * time_ptr = std::localtime(&time_temp);
           this->day = day;
           this->month = month;
           this->year = year;
           this->weekday = time_ptr->tm_wday;
           
        }
        Date(const Date& obj){
           this->day = obj.day;
           this->month = obj.month;
           this->year = obj.year;
           this->weekday = obj.weekday;
        }
    	
        int Day(){ return day; }
        int Month(){ return month; }
        int Year(){ return year; }
        int Weekday(){ return weekday; }
    
};

class WorkCal {
    private:
    	Date startday;
      bool leap(int year){
          return (year % 4==0)&&(year%100!=0)||(year%400==0);
      }
    public:
        WorkCal(){
            startday = Date();
        }
        WorkCal(int year, int month, int day){
            startday = Date(year, month, day);
      }
      // get date of workday
        Date workday(int day){
            Date date = startday;
            date.day = date.day + day - 1;
            date.weekday = (date.weekday + day -1)%7;
            int daysinMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (leap(date.year)) { daysinMonth[1]=29; }

            while(date.day > daysinMonth[date.month-1]){
                date.day -= daysinMonth[date.month-1];
                date.month += 1;
                if (date.month==13){
                    date.year += 1;
                    date.month = 1;
                    if (leap(date.year)){ daysinMonth[1]=29; }
                }
            }
            return date;
        }
};
#endif
