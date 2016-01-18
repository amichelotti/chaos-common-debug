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
#if defined CHAOS && defined __cplusplus 

#include <chaos/common/global.h>
#define DPRINT(str,ARGS...) {char dbg[1024]; snprintf(dbg,sizeof(dbg),str, ##ARGS);LDBG_<<"["<<__PRETTY_FUNCTION__<<"]"<<" "<< dbg;}
#define DERR(str,ARGS...)  {char dbg[1024]; snprintf(dbg,sizeof(dbg),str, ##ARGS);LERR_<<"["<<__PRETTY_FUNCTION__<<"]"<<"## "<< dbg;}
#else
#define DPRINT(str,ARGS...) printf("[%.12Lu,x%lx] \033[38;5;148m%s\033[39m :" str "\n",(unsigned long long)::common::debug::getUsTime(),(unsigned long)pthread_self(), __PRETTY_FUNCTION__, ##ARGS)
#define DERR(str,ARGS...) printf("# [%.12Lu,x%lx] \033[38;5;148m%s\033[39m :" str "\n",(unsigned long long)::common::debug::getUsTime(),(unsigned long)pthread_self(),__PRETTY_FUNCTION__,##ARGS)
#endif
#else
#define DPRINT(str,ARGS...) 
#define DERR(str,ARGS...) 
#endif

#if defined CHAOS && defined __cplusplus 
#include <chaos/common/global.h>
#define PRINT(str,ARGS...) {char dbg[256]; snprintf(dbg,sizeof(dbg),str, ##ARGS);LAPP_<< dbg;}
#define ERR(str,ARGS...)  {char dbg[256]; snprintf(dbg,sizeof(dbg),str, ##ARGS);LERR_<<"["<<__PRETTY_FUNCTION__<<"]"<< "## "<<dbg;}

#else
#define PRINT(str,ARGS...) printf("*" str "\n",##ARGS)
#define ERR(str,ARGS...) printf("# \"%s\":" str "\n",__PRETTY_FUNCTION__,##ARGS)
#endif
// include your class/functions headers here

#ifdef __cplusplus
extern "C" {
  namespace common {
    namespace debug {
      
      uint64_t getUsTime();
    }}
}
namespace common {
  namespace debug {
    // put your code here

    
    struct basic_timed {
      uint64_t last_update_time_us,old_update_time;
      uint64_t mod_time(){return (::common::debug::getUsTime()-last_update_time_us);}
      bool hasModified(){return ((last_update_time_us-old_update_time)>0); }
      virtual const char*get_name(){return "";}
      basic_timed(){last_update_time_us=old_update_time=0;}
    };

    template <typename T>
      struct timed_value:public basic_timed {
	T value;
	const char*name;
    timed_value(const char *_name="noname"):name(_name){/*DPRINT("creating new timed variable with %s",name);*/}
    timed_value(T val,const char *_name="noname"):name(_name){set(val);/*DPRINT("creating new timed variable with %s and setting value",name);*/}
	void set_name(const char *_name){name = _name;}
	void set(T val){value= val; old_update_time=last_update_time_us;last_update_time_us=::common::debug::getUsTime();}
	T get(uint64_t* val=0){if(val) *val = last_update_time_us;return value;}
	T& operator=(T val){set(val); DPRINT("updating %s at time %llu last update %llu us ago",name,(unsigned long long )last_update_time_us,last_update_time_us-old_update_time);return value;}
	operator T(){return value;}
	const char*get_name(){return name;}
	  
    };
#define INITIALIZE_TIMED(var,value) { var=value;var.set_name(#var); DPRINT("initializing timed \"%s\"",var.get_name());}
  }
}


#endif
#endif
