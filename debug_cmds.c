#include "debug_cmds.h"
#include <sys/ptrace.h>
#include <sys/user.h>

struct debug_cmd debug_cmds[10];

struct debug_cmd new_debug_cmd(const char* _name, int (*_cmd)(pid_t, char**)) {
  struct debug_cmd new_cmd;
  new_cmd.name = _name;
  new_cmd.cmd = _cmd;
  return new_cmd;
}

int init_debug_cmds() {
  debug_cmds[0] = new_debug_cmd("step", dbg_step);
  debug_cmds[1] = new_debug_cmd("regs", dbg_regs);
  debug_cmds[2] = new_debug_cmd("peekr", dbg_peekr);
  debug_cmds[3] = new_debug_cmd("cont", dbg_cont);
  return 0;
}

int (*get_debug_cmd(const char* name))(pid_t, char**) {
  for(int i = 0; i < 10; i++) {
    if(!strcmp(debug_cmds[i].name, name)) {
      return debug_cmds[i].cmd;
    }
  }
}

int dbg_step(pid_t target, char** args) {
  ptrace(PTRACE_SINGLESTEP, target, NULL, NULL);
  printf("Stepped to next instruction\n");
  return 0;
}

int dbg_regs(pid_t target, char** args) {
  struct user_regs_struct regs;
  ptrace(PTRACE_GETREGS, target, NULL, &regs);
  printf("rax: %#010x\nrbx: %#010x\nrcx: %#010x\nrdx: %#010x\nrsp: %#010x\nrbp: %#010x\nrsi: %#010x\nrdi: %#010x\nrip: %#010x\nr8: %#010x\nr9: %#010x\nr10: %#010x\nr11: %#010x\nr12: %#010x\nr13: %#010x\nr14: %#010x\nr15: %#010x\n",
         regs.rax, regs.rbx, regs.rcx, regs.rdx, regs.rsp, regs.rbp, regs.rsi, regs.rdi, regs.rip, regs.r8, regs.r9, regs.r10, regs.r11, regs.r12, regs.r13, regs.r14, regs.r15);
  return 0;
}

int dbg_peekr(pid_t target, char** args) {
  struct user_regs_struct regs;
  ptrace(PTRACE_GETREGS, target, NULL, &regs);
  printf("%#010x\n", ptrace(PTRACE_PEEKTEXT, target, regs.rip, NULL));
  return 0;
}

int dbg_cont(pid_t target, char** args) {
  ptrace(PTRACE_CONT, target, NULL, NULL);
  printf("Continuing... [UNIMPLEMENTED]\n");
  return 0;
}
