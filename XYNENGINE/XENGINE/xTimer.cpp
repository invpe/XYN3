//////////////////////////////////////////////////////////////////////////////
// CTimer.cpp
// =========
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
#include "xmain.h"

namespace XTimer
{  
//#include <stdlib.h>

/*!
* Creates the CTimer instance
* @return CTimer instance
*/	 
CTimer *CreateInstance()
{
	return( new CTimer() ); 
}

/*!
* CTimer Constructor
* Queries the frequencies and initializes the default values
*/
CTimer::CTimer()
{
#ifdef WIN32
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart = 0;
    endCount.QuadPart = 0;
#else
    startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;
#endif

    stopped = 0;
    startTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
}

/*!
* CTimer destructor
*/
CTimer::~CTimer()
{
}

/*!
* Starts the timer
*/
void CTimer::start()
{
    stopped = 0; // reset stop flag
#ifdef WIN32
    QueryPerformanceCounter(&startCount);
#else
    gettimeofday(&startCount, NULL);
#endif
}

/*!
* Stops the timer
*/
void CTimer::stop()
{
    stopped = 1; // set timer stopped flag

#ifdef WIN32
    QueryPerformanceCounter(&endCount);
#else
    gettimeofday(&endCount, NULL);
#endif
}

/*!
* Compute elapsed time in micro-second resolution.
* Other getElapsedTime will call this first, then convert to correspond resolution.
*/
double CTimer::getElapsedTimeInMicroSec()
{
#ifdef WIN32
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
    if(!stopped)
        gettimeofday(&endCount, NULL);

    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

    return endTimeInMicroSec - startTimeInMicroSec;
}

/*!
* Divide elapsedTimeInMicroSec by 1000
*/
double CTimer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

/*!
* Divide elapsedTimeInMicroSec by 1000000
*/
double CTimer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

/*!
* Same as getElapsedTimeInSec()
*/
double CTimer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}


}//ns