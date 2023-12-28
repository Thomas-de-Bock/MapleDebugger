#include "debugger.h"

int main(int argc, char** argv) {
  uint8_t* image = read_elf(argv[1]);
  struct elf_info* elf = get_elf_info(image);
  print_elf_info(elf);
  free(elf);

  if(argc < 2) {
    printf("No target specified");
    return -1;
  }
  init_debug_cmds();
  pid_t target = init_target(argv[1]);
  debug_target(target);
}


int debug_console(pid_t target) {
  int read;
  char* inp_buffer = NULL;
  unsigned int inp_len;
  while(true) {
    printf("maple> ");
    read = getline(&inp_buffer, &inp_len, stdin);
    if(read == -1) {
      printf("Invalid input\n");
      continue;
    }
    char* dbg_cmd = strtok(inp_buffer, "\n");
    int (*dbg_cmd_fun)(pid_t, char**) = (get_debug_cmd(inp_buffer));
    dbg_cmd_fun(target, &inp_buffer);
    if(dbg_cmd_fun == dbg_step) {
      return 0;
    }
  }
}

int debug_target(pid_t target) {
  int wait_status = -1;
  wait(&wait_status);
  while(WIFSTOPPED(wait_status)) {
    debug_console(target);
    wait(&wait_status);
  }

  return 0;
}

pid_t init_target(const char* target_name) {
  printf("Starting target: %s\n", target_name);
  pid_t target = fork();
  if(target < 0) {
    printf("Could not create process\n");
    return -1;
  }
  if(target != 0) {
    printf("Created new target: PID[%d]\n", target);
    return target;   // Running on parent process, return created process
  }
  
  // Make new child process traceable
  if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
    printf("Target could not be traced\n");
    return -1;
  }
  
  // Replace child's current target (this code) with target process
  execl(target_name, target_name, 0);
  return 0;
}
