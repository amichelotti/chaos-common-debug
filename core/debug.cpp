 /*
 *	debug.cpp
 *	!CHAOS
 *	Created automatically
 *
 *    	Copyright 2012 INFN, National Institute of Nuclear Physics
 *
 *    	Licensed under the Apache License, Version 2.0 (the "License");
 *    	you may not use this file except in compliance with the License.
 *    	You may obtain a copy of the License at
 *
 *    	http://www.apache.org/licenses/LICENSE-2.0
 *
 *    	Unless required by applicable law or agreed to in writing, software
 *    	distributed under the License is distributed on an "AS IS" BASIS,
 *    	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    	See the License for the specific language governing permissions and
 *    	limitations under the License.
 */

#include "debug.h"
#ifndef _WIN32
#include <sys/time.h>
#else
#include "winsock2.h"
int gettimeofdayWin(struct timeval * tp, struct timezone * tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970 
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}
#endif
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
 
#endif
namespace common {
  namespace debug {
    uint64_t getUsTime(){
      struct timeval tv;
      uint64_t ret;
      static uint64_t  first_call=0;
#ifndef _WIN32
      gettimeofday(&tv,NULL);
#else
	  gettimeofdayWin(&tv, NULL);
#endif
      ret = ((uint64_t)tv.tv_sec) * 1000000LL;
      ret+= tv.tv_usec;
      if(first_call==0){
        first_call = ret;
      }
      return ret-first_call;
    }
  }
}
#ifdef __cplusplus
}
#endif
