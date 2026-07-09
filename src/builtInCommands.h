#include <stddef.h>
#include "command.h"

typedef int (*builtInFunc)(Command*);

typedef struct {
  char *name;
  builtInFunc func;
} BuiltIn;

int builtInCd(Command *cd);
int builtInEcho(Command *cd);
int printEnvVars(char *arg);
