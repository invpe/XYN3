//////////////////////////////////////////////////////////////////////////////
// CTimer.h
// =======
// High Resolution CTimer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H_DEF
#define TIMER_H_DEF

/*!
* XTimer namespace
*/
namespace XTimer
{  
#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

/*!
* High resolution timer class
*/
class CTimer
{
public:
	/*!
	* Default constructor
	*/	
    CTimer();             
	/*!
	* Default destructor
	*/		                       
    ~CTimer();            
	/*!
	* Start the timer measurements
	*/	
    void   start();               
	/*!
	* Stop the timer measurements
	*/	
    void   stop();                             
	/*!
	* @return Returns time elapsed in seconds
	*/	
	double getElapsedTime();                   
	/*!
	* @return Returns time elapsed in seconds
	*/	
	double getElapsedTimeInSec();              
	/*!
	* @return Returns time elapsed in milliseconds
	*/	
	double getElapsedTimeInMilliSec();         
	/*!
	* @return Returns time elapsed in microseconds
	*/	
	double getElapsedTimeInMicroSec();         


protected:


private:
	/*!
	* Keeps the start time in micro-seconds
	*/	
    double startTimeInMicroSec;                 // starting time in micro-second
	/*!
	* Keeps the end time in micro-seconds
	*/	
	double endTimeInMicroSec;                   // ending time in micro-second
	/*!
	* Stop flag
	*/	
    int    stopped;                             // stop flag 
#ifdef WIN32
	/*!
	* CTimer frequency
	*/	 
    LARGE_INTEGER frequency;                   
	/*!
	* CTimer start time
	*/	 
	LARGE_INTEGER startCount;                  
	/*!
	* CTimer end time
	*/	 
    LARGE_INTEGER endCount;                    
#else
    timeval startCount;                        
    timeval endCount;                            
#endif
};

/*!
* Creates the CTimer instance
*/	
CTimer *CreateInstance();


}//ns
#endif // TIMER_H_DEF
