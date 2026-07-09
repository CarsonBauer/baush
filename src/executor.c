#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include "executor.h"
#include "builtInCommands.h"

int executeCommands(Command **cmds, int count)
{
  int prevReadFd = -1;

  for (int i = 0; i < count; i++)
  {
    Command *cmd = &(*cmds)[i];
    int pipefd[2];
    BuiltIn builtinCmds[] = {
      { "cd", builtInCd },
      { "echo", builtInEcho },
      { NULL, NULL }
    };

    if (i < count - 1)
    {
      if (pipe(pipefd) < 0)
      {
        perror("pipe");
        _exit();
      }
    }

    pid_t pid = fork();

    if (pid < 0)
    {
      perror("fork");
      _exit();
    }

    if (pid == 0)
    {
      if (cmd->inputFile) {
        int fd = open(cmd->inputFile, O_RDONLY);

        if (fd < 0) {
            perror("open input");
            _exit();
        }

        dup2(fd, STDIN_FILENO);
        close(fd);
      }
      else if (prevReadFd != -1) 
      {
        dup2(prevReadFd, STDIN_FILENO);
      }

      if (cmd->outputFile) {
        int fd = open(cmd->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
      else if (i < count - 1) 
      {
        dup2(pipefd[1], STDOUT_FILENO);
      }

      if (prevReadFd != -1)
      {
        close(prevReadFd);
      }

      if (i < count - 1)
      {
        close(pipefd[0]);
        close(pipefd[1]);
      }

      for (int i = 0; builtinCmds[i].name != NULL; i++) 
      { 
        if (strcmp(cmd->argv[0], builtinCmds[i].name) == 0) 
        {
          return builtinCmds[i].func(cmd);
        }
      }

      execvp(cmd->argv[0], cmd->argv);

      if (errno == ENOENT)
      {
        fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
      }
      else 
      {
        perror(cmd->argv[0]);
      }

      _exit(1);
    }

    if (prevReadFd != -1)
    {
      close(prevReadFd);
    }

    if (i < count - 1)
    {
      close(pipefd[1]);
      prevReadFd = pipefd[0];
    }
  }

  for (int i = 0; i < count; i++)
  {
    wait(NULL);
  }

  return 0;
}
