#include "sys.h"

fn_ptr sys_call_table[5] = {
	sys_fork,sys_exit,sys_kill,sys_pause,sys_time
};