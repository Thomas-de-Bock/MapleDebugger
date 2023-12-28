#include<string.h>
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

struct debug_cmd {
  const char* name;
  int (*cmd)(pid_t, char**);
};

struct debug_brk {
  const char* tag;
  int8_t opcode;
  void* addr;
};

extern struct debug_cmd debug_cmds[10];


struct debug_cmd new_debug_cmd(const char* _name, int (*_cmd)(pid_t, char**));
int init_debug_cmds();
int (*get_debug_cmd(const char* name))(pid_t, char**);

int dbg_step(pid_t target, char** args);
int dbg_regs(pid_t target, char** args);
int dbg_peekr(pid_t target, char** args);
int dbg_cont(pid_t target, char** args);


