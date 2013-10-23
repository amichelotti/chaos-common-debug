/*	
 *	debug.h
 *	!CHAOS
 *	Created by automatically 
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
#ifndef __common_debug_h__
#define __common_debug_h__
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#ifdef DEBUG
#define DPRINT(str,ARGS...) printf("[%.12llu,x%lx] \033[38;5;148m%s\033[39m :" str,common::debug::getUsTime(),(unsigned long)pthread_self(), __FUNCTION__, ##ARGS)
#define DERR(str,ARGS...) printf("# [%.12llu,x%lx] \033[38;5;148m%s\033[39m :" str,common::debug::getUsTime(),(unsigned long)pthread_self(),__FUNCTION__,##ARGS)
#else
#define DPRINT(str,ARGS...) 
#define DERR(str,ARGS...) 
#endif
#define PRINT(str,ARGS...) printf("*" str,##ARGS)
#define ERR(str,ARGS...) printf("# \"%s\":" str,__FUNCTION__,##ARGS)

// include your class/functions headers here

namespace common {
  namespace debug {
    // put your code here
    uint64_t getUsTime();
    
    struct basic_timed {
      uint64_t last_update_time_us;
      uint64_t mod_time(){return last_update_time_us;}
      virtual const char*get_name(){return "";}
      basic_timed(){last_update_time_us=-1;}
    };

    template <typename T>
      struct timed_value:public basic_timed {
	T value;
	const char*name;
	timed_value(const char *_name="noname"):name(_name){}
    timed_value(T val,const char *_name="noname"):name(_name){set(val);}
	void set_name(const char *_name){name = _name;}
	void set(T &val){value= val; last_update_time_us=common::debug::getUsTime();}
	T get(uint64_t* val=0){if(val) *val = last_update_time_us;return value;}
	T& operator=(T& val){set(val); return value;}
	operator T(){return value;}
	const char*get_name(){return name;}
	  
      };
#define INITIALIZE_TIMED(var,value) \
    var=value;var.set_name(#var); DPRINT("initializing timed \"" #var "\"\n")
  };
};

#endif
