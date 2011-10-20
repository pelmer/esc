/*==========================================================================
 *  This code is Copyright (C) 2005, Jurgen A. Doornik.
 *  Permission to use this code for non-commercial purposes
 *  is hereby given, provided proper reference is made to:
 *		Doornik, J.A. (2005), "An Improved Ziggurat Method to Generate Normal
 *          Random Samples", mimeo, Nuffield College, University of Oxford,
 *			and www.doornik.com/research.
 *		or the published version when available.
 *	This reference is still required when using modified versions of the code.
 *  This notice should be maintained in modified versions of the code.
 *	No warranty is given regarding the correctness of this code.
 *==========================================================================*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "zigtimer.h"

/*--------------------------- timer functions ------------------------------*/

#if defined(__linux__) | defined (__APPLE__)
#include <sys/times.h>
#include <time.h>

clock_t unix_clock (void)
{
    struct tms buf;
    if (times (&buf) < 0)
        return clock();

    return buf.tms_utime + buf.tms_stime;
}

#define clock unix_clock
#include <unistd.h>
#ifdef CLOCKS_PER_SEC
#undef CLOCKS_PER_SEC /* to stop warnings, always 1000000 under POSIX */
#endif
#define CLOCKS_PER_SEC (sysconf (_SC_CLK_TCK))
#endif

static char s_sTime[24];
static clock_t s_clockStart, s_clockStop;
static double s_dClockPerHundred = 0;


void    StartTimer(void)
{
	if (s_dClockPerHundred == 0)
		s_dClockPerHundred = 100.0 / CLOCKS_PER_SEC;
		
	s_clockStop = s_clockStart = clock();
}
void    StopTimer(void)
{
    s_clockStop = clock();
}

static double  ClockStartDblHsec(void)
{
    return s_clockStart * s_dClockPerHundred;
}
static double  ClockStopDblHsec(void)
{
    return s_clockStop * s_dClockPerHundred;
}

static char *  GetTimeSpan(double dTime2, double dTime1)
{
    int m, s, h, len, bzeros;  unsigned long tnew;
    double  duration;
       
    duration = fabs(dTime2 - dTime1);             /* difference in hundreds */
    tnew = (unsigned long)(duration / 100.0);      /* difference in seconds */
    
    h = (int)(duration - 100.0 * tnew);                         /* hundreds */
    s = tnew % 60;  tnew /= 60;                                  /* seconds */
    m = tnew % 60;  tnew /= 60;             /* minutes, now tnew is # hours */
    
    len = bzeros = 0;
    if (tnew)
    {   sprintf(s_sTime, "%2d:", (int)tnew);  len = (int)strlen(s_sTime);
        bzeros = 1;
    }
    if (m || tnew)
    {   sprintf(s_sTime + len, bzeros ? "%02d:" : "%2d:", m);  len += 3;
        bzeros = 1;
    }
    sprintf(s_sTime + len, bzeros ? "%02d" : "%2d", s); len += 2;
    if (h)
        sprintf(s_sTime + len, ".%02d", h);

return s_sTime;
}
char * GetLapsedTime(void)
{
    return GetTimeSpan(ClockStopDblHsec(), ClockStartDblHsec());
}
/*-------------------------- END timer functions ---------------------------*/
