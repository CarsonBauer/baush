#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include "executor.h"
#include "parser.h"
#include "main.h"


int main()
{
  while(1)
  {
    char line[1024];
    char cwd[1024];
    int capacity = 2;
    int commandCount = 0;
    Command *cmds = malloc(capacity * sizeof(Command));

    struct passwd *pw = getpwuid(getuid());

    createDirectoryDisplay(cwd, sizeof(cwd));
    printf("%s@baush:~%s> ", pw->pw_name, cwd);
    fgets(line, sizeof(line), stdin);

    parseCommands(line, &cmds, &commandCount, &capacity);
    executeCommands(&cmds, commandCount);
  }
}

int createDirectoryDisplay(char *cwd, size_t size)
{
    if (getcwd(cwd, size) == NULL) {
        perror("getcwd");
        return -1;
    }

    char *home = getenv("HOME");

    if (home != NULL && strncmp(cwd, home, strlen(home)) == 0) {
        memmove(cwd, cwd + strlen(home), strlen(cwd) - strlen(home) + 1);
    }

    return 0;
}

