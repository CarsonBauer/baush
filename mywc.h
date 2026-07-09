struct FileInfo {
  int bytesRead;
  int wordsRead;
  int linesRead;
};

struct FileInfo processFilesV2(char *fileName);
int processFiles(int printLines, int printWords, int printBytes, char *fileName);
int printFileInfo(char* fileName, int printLines, int printWords, int printBytes, struct FileInfo fileInfo);
