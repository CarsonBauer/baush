#ifndef COMMAND_H
#define COMMAND_H

typedef struct {
  int argc;
  char *argv[64];
  char *inputFile;
  char *outputFile;
} Command;

#endif
