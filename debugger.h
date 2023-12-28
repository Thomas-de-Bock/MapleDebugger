#include<stdio.h>
#include<sys/ptrace.h>
#include<sys/procfs.h>
#include<sys/msg.h>
#include<sys/user.h>
#include<sys/wait.h>
#include<unistd.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include<stdbool.h>
#include "debug_cmds.h"
#include "elfreader.h"

int debug_console(pid_t target);
pid_t init_target(const char* target_name);
int debug_target(pid_t target);
