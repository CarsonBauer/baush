#include "command.h"

int parseCommands(char *line, Command **cmds, int *count, int *capacity);
int parseCommand(char *line, Command *cmd);
int redirectIO(char* filePath, int io);
