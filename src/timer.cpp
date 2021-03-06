/* Copyright 2011-2014 Kyle Michel, Logan Ward, Christopher Wolverton
 *
 * Contact: Kyle Michel (kylemichel@gmail.com)
 *			Logan Ward (LoganWard2012@u.northwestern.edu)
 *
 *
 * This file is part of Mint.
 *
 * Mint is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Mint is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with Mint.  If not, see
 * <http://www.gnu.org/licenses/>.
 */



#ifdef MINT_MPI
	#include <mpi.h>
#endif
#include "timer.h"
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <sys/time.h>



/* void Timer::start()
 *
 * Start the timer
 */

void Timer::start()
{
	#ifdef MINT_MPI
		_start = MPI_Wtime();
	#else
		timeval temptime;
		gettimeofday(&temptime, NULL);
		_start = temptime.tv_sec + temptime.tv_usec/1000000.0;
	#endif
}



/* double Timer::currentNumber()
 *
 * Return the current time
 */

double Timer::currentNumber()
{
	#ifdef MINT_MPI
		return MPI_Wtime() - _start;
	#else
		timeval temptime;
		gettimeofday(&temptime, NULL);
		return temptime.tv_sec + temptime.tv_usec/1000000.0 - _start;
	#endif
}



/* Word Timer::current(bool formatTime, int precision)
 *
 * Return the current time
 */

Word Timer::current(bool formatTime, int precision)
{
	
	// Get the current time
	double time = currentNumber();
	
	// If printing as seconds only, then save and return
	Word res;
	char buffer[25];
	if (!formatTime)
	{
		char set[10];
	    sprintf(set, "%s%d%s", "%.", precision, "f%s");
	    sprintf(buffer, set, time, " second");
		res += buffer;
		if (fabs(time - 1) >= pow(10.0, -precision) - pow(10.0, -(precision + 4)))
			res += 's';
		return res;
	}
	
	// Turn seconds into minutes
    int minutes = (int) floor(time / 60.0);
    double seconds = time - 60 * minutes;
    
    // Turn minutes into hours
    int hours = (int) floor(minutes / 60.0);
    minutes -= 60 * hours;
    
    // Turn hours into days
    int days = (int) floor(hours / 24.0);
    hours -= 24 * days;

	// Make into Word object
	if (days)
	{
        if (days == 1)
            sprintf(buffer, "%d%s", days, " day, ");
        else
            sprintf(buffer, "%d%s", days, " days, ");
		res += buffer;
	}
	if ((hours) || (days))
	{
		if (hours == 1)
            sprintf(buffer, "%d%s", hours, " hour, ");
        else
            sprintf(buffer, "%d%s", hours, " hours, ");
		res += buffer;
	}
	if ((minutes) || (hours) || (days))
	{
		if (minutes == 1)
            sprintf(buffer, "%d%s", minutes, " minute, ");
        else
            sprintf(buffer, "%d%s", minutes, " minutes, ");
		res += buffer;
	}
	char set[10];
    sprintf(set, "%s%d%s", "%.", precision, "f%s");
    sprintf(buffer, set, seconds, " second");
	res += buffer;
	if (fabs(seconds - 1) >= pow(10.0, -precision) - pow(10.0, -(precision + 4)))
		res += 's';
	
	// Return result
	return res;
}
