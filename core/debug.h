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
#ifdef _WIN32
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif
#else
#include <pthread.h>
#endif
#ifdef DEBUG
#if defined CHAOS && defined __cplusplus 

#include <chaos/common/global.h>
#define DPRINT(str,...) {char dbg[1024]; snprintf(dbg,sizeof(dbg),str, ##__VA_ARGS__);LDBG_<<"["<<__PRETTY_FUNCTION__<<"]"<<" "<< dbg;}
#define DERR(str,...)  {char dbg[1024]; snprintf(dbg,sizeof(dbg),str, ##__VA_ARGS__);LERR_<<"["<<__PRETTY_FUNCTION__<<"]"<<"## "<< dbg;}
#else
#if defined __cplusplus
#define DPRINT(str,...) printf("[%.12llu,x%lx] \033[38;5;148m%s\033[39m :" str "\n",(unsigned long long)::common::debug::getUsTime(),(unsigned long)pthread_self(), __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define DERR(str,...) printf("# [%.12llu,x%lx] \033[38;5;148m%s\033[39m :" str "\n",(unsigned long long)::common::debug::getUsTime(),(unsigned long)pthread_self(),__PRETTY_FUNCTION__,##__VA_ARGS__)
#else
#define DPRINT(str,...) printf("[%.12llu,x%lx] \033[38;5;148m%s\033[39m :" str "\n",(unsigned long long)getUsTime(),(unsigned long)pthread_self(), __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define DERR(str,...) printf("# [%.12llu,x%lx] \033[38;5;148m%s\033[39m :" str "\n",(unsigned long long)getUsTime(),(unsigned long)pthread_self(),__PRETTY_FUNCTION__,##__VA_ARGS__)
#endif
#endif
#else
#define DPRINT(str,...) 
#define DERR(str,...) 
#endif

#if defined CHAOS && defined __cplusplus 
#include <chaos/common/global.h>
#define PRINT(str,...) {char dbg[256]; snprintf(dbg,sizeof(dbg),str, ##__VA_ARGS__);LAPP_<< dbg;}
#define ERR(str,...)  {char dbg[256]; snprintf(dbg,sizeof(dbg),str, ##__VA_ARGS__);LERR_<<"["<<__PRETTY_FUNCTION__<<"]"<< "## "<<dbg;}

#else
#define PRINT(str,...) printf("*" str "\n",##__VA_ARGS__)
#define ERR(str,...) printf("# \"%s\":" str "\n",__PRETTY_FUNCTION__,##__VA_ARGS__)
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
	T& operator=(T val){set(val); DPRINT("updating %s at time %lu last update %llu us ago",name,(unsigned long long )last_update_time_us,last_update_time_us-old_update_time);return value;}
	operator T(){return value;}
	const char*get_name(){return name;}
	  
    };
#define INITIALIZE_TIMED(var,value) { var=value;var.set_name(#var); DPRINT("initializing timed \"%s\"",var.get_name());}
  }
}

#else
  uint64_t getUsTime();
#endif
#endif
