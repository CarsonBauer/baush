#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "parser.h"

int parseCommands(char *line, Command **cmds, int *count, int *capacity)
{
  const char *delim = "|";
  char *saveptr;
  char *token = strtok_r(line, delim, &saveptr);

  while (token != NULL)
  {
    while (*token == ' ') token++;
    Command cmd;
    char *copy = strdup(token);
    parseCommand(copy, &cmd);
    
    if (*count >= *capacity)
    {
      *capacity *= 2;
      *cmds = realloc(*cmds, *capacity * sizeof(Command));
    }

    (*cmds)[(*count)++] = cmd;
    token = strtok_r(NULL, delim, &saveptr);
  }
  return 0;
}

int parseCommand(char *line, Command *cmd)
{
  const char *delim = " \n\t";
  char *saveptr;
  char *token = strtok_r(line, delim, &saveptr);
  int i = 0;

  cmd->inputFile = NULL;
  cmd->outputFile = NULL;

  while (token != NULL)
  {
    if (strcmp(token, ">") == 0)
    {
      cmd->outputFile = strdup(strtok_r(NULL, delim, &saveptr));
      break;
    }
    else if (strcmp(token, "<") == 0)
    {
      cmd->inputFile = strdup(strtok_r(NULL, delim, &saveptr));
      break;
    }

    cmd->argv[i] = strdup(token);

    token = strtok_r(NULL, delim, &saveptr);
    i++;
  }

  cmd->argv[i] = NULL;
  cmd->argc = i;

  return 0;
}

int redirectIO(char* filePath, int io)
{
  int fd; 
  if (io == 0)
  {
    fd = open(filePath, O_RDONLY);
  } else {
    fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  }

  if (dup2(fd, io) < 0)
  {
    perror("dup2");
  }
  close(fd);

  return 0;
}
