#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "command.h"
#include "builtInCommands.h"

int builtInCd(Command *cd) {
  char **argv = cd->argv;

  if (argv[1] == NULL) {
    char *home = getenv("HOME");
    if (home == NULL)
    {
      fprintf(stderr, "cd: HOME not set\n");
      return -1;
    }
    return chdir(home);
  }

  if (chdir(argv[1]) != 0) {
    perror("cd");
    return -1;
  }
  return 0;
}

int builtInEcho(Command *cd) {
  char **argv = cd->argv;
  
  for (int i = 1; argv[i] != NULL; i++) {
    char *arg = argv[i];
    for (int j = 0; arg[j] != '\0'; j++) {
      if (arg[j] == '$') {
        j++;

        char var[256];
        int k = 0;

        while (arg[j] != '\0' && arg[j] != '$') {
            var[k++] = arg[j++];
        }
        var[k] = '\0';

        j--;

        char *val = getenv(var);
        if (val != NULL) {
            printf("%s", val);
        }
      } else {
        printf("%c", arg[j]);
      }
    }
    printf(" ");
  }

  printf("\n");

  return 0;
}
