/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <ctype.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"
#include "sig.h"

#define SHELL_PROMPT "rhsh"
// the maximum number of arguments, including the '&' for background commands
// and the null terminateor.
#define MAX_ARGS 16

char cmd_name[256]; // global for command name, if we run multiple background jobs maybe use an array

char *
get_prompt_line(void)
{
  static char *line = 0;
  char *cmd         = 0;
  char *end         = 0;

  if(line) {
    free(line);
    line = 0;
  }

  line = readline(SHELL_PROMPT " $ ");

  // skip if just spaces or starts with newline
  cmd = line;
  while(*cmd && (isspace(*cmd) || *cmd == '\n'))
    cmd++;

  if(!*cmd)
    return 0;

  // remove trailing spaces
  end = cmd + strlen(cmd);
  while(end != cmd && isspace(*end)) {
    *end = 0;
    end--;
  }

  // can't reach here with everything being spaces because the first loop would
  // have caught that.

  // check if we'd want to exit
  if(!strncmp(cmd, "exit", 4) || !strncmp(cmd, "quit", 4)) {
    if(line)
      free(line);
    exit(EXIT_SUCCESS);
  }

  // Keep this here as a helper when testing with simpleshell, it allows you to
  // use the arrow keys to navigate your command history.
  add_history(cmd);
  return cmd;
}

void
process_command(char *cmd)
{
  printf("Received from the shell the command: %s\n", cmd);

  if(cmd[strlen(cmd) - 1] == '&') {
    cmd[strlen(cmd) - 1] = 0;
    start_bg_command(cmd);
  } else {
    start_fg_command(cmd);
  }
}

int
generate_exec_args(char *cmd, char *argv[])
{
  // TODO:
  // =====
  //  Implement this function...
  char* token = strtok(cmd, " "); // Split on spaces
  int i = 0;
  while(token != NULL) {
    //fprintf(stdout, "test: %s\n", token);
    argv[i] = token;
    //fprintf(stdout, "test argvi: %s\n", argv[i]);
    token = strtok(NULL, " ");
    i++;
  }
  argv[i] = NULL;
  return i;
}

int
start_fg_command(char *cmd)
{
  // TODO:
  // =====
  //   Implement code to start a foreground command.
  // 1. Get the array of arguments. The first arg is the command the rest are the args
  char* argv[MAX_ARGS];
  generate_exec_args(cmd, argv);

  //fprintf(stdout, "Running foreground command %s with %d arguments", argv[0], num_args);
  int rc = fork();
  if(rc == 0) { // child
    //fprintf(stdout, "Running foreground command %s with %d arguments", argv[0], num_args);
    execvp(argv[0], argv);
    perror("execvp failed");
    exit(1);
  } else { // parent
    //fprintf(stdout, "Running foreground command %s with %d arguments", argv[0], num_args);
    int wstatus;
    waitpid(rc, &wstatus, 0);

    if(WIFEXITED(wstatus)) {
      return WEXITSTATUS(wstatus);
    }
  }
  return -1;
}

void sigchld_handler(int sig) {
  int wstatus;
  wait(&wstatus);
  fprintf(stdout, "Background command %s finished!\n", cmd_name);
}

void
start_bg_command(char *cmd)
{
  // TODO:
  // =====
  //   Implement code to start a background command.
  char* argv[MAX_ARGS];
  strncpy(cmd_name, cmd, sizeof(cmd_name));
  generate_exec_args(cmd, argv);
  //
  setsighandler(SIGCHLD, sigchld_handler);
  int rc = fork();
  if(rc == 0) { // child
    execvp(argv[0], argv);
    perror("execvp failed");
    exit(1);
  } else { // parent
    //int wstatus;
    //waitpid(rc, &wstatus, 0);

    //if(WIFEXITED(wstatus)) {
    //  return WEXITSTATUS(wstatus);
    //}
  }
}
