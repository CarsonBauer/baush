#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "mywc.h"
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int printLines = 0;
  int printWords = 0;
  int printBytes = 0;
  char **files = malloc(argc * sizeof(char*));
  int fileCount = 0;

  if (argc < 2)
  {
    printf("No arguments provided, file name is a required argument\n");
    return EXIT_FAILURE;
  }
  else
  {
    for (int i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "-l") == 0)
      {
        printLines = 1;
      }

      if (strcmp(argv[i], "-w") == 0)
      {
        printWords = 1;
      }

      if (strcmp(argv[i], "-c") == 0)
      {
        printBytes = 1;
      }

      if (strncmp(argv[i], "-", 1))
      {
        files[fileCount] = strdup(argv[i]);
        fileCount++;
      }
    }
  }

  if (printLines == 0 && printWords == 0 && printBytes == 0)
  {
    printLines = printWords = printBytes = 1;
  }

  if (fileCount == 0)
  {
    printf("No file provided, file name is a required argument\n");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < fileCount; i++)
  {
    struct FileInfo fileInfo = processFilesV2(files[i]);
    printFileInfo(files[i], printLines, printWords, printBytes, fileInfo);
  }

  return EXIT_SUCCESS;

}

struct FileInfo processFilesV2(char *fileName)
{
  int fd = open(fileName, O_RDONLY);
  char buffer[4096];
  ssize_t bytes;
  ssize_t bytesRead = 0;
  int linesRead = 0;
  int wordsRead = 0;
  struct FileInfo fileInfo;
  int insideWord = 0;

  while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
  {
    for(int i = 0; i < bytes; i++)
    {
      if (buffer[i] != ' ')
      {
        if (insideWord == 0)
        {
          wordsRead++;
        }

        insideWord = 1;
      }
      else 
      {
        insideWord = 0;
      }
    }

    linesRead++;
    bytesRead = bytesRead + bytes;
  }

  fileInfo.bytesRead = bytesRead;
  fileInfo.linesRead = linesRead;
  fileInfo.wordsRead = wordsRead;
  
  return fileInfo;
}

int printFileInfo(char* fileName, int printLines, int printWords, int printBytes, struct FileInfo fileInfo)
{
  if (printLines)
  {
    printf("%d ", fileInfo.linesRead);
  }

  if (printWords)
  {
    printf("%d ", fileInfo.wordsRead);
  }

  if (printBytes)
  {
    printf("%d ", fileInfo.bytesRead);
  }

  printf("%s\n", fileName);
  
  return 0;
}
