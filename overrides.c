#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef int (*open_t)(const char *pathname, int flags, mode_t mode);
static open_t real_open;

char *proc_mnt;
int proc_mnt_len;

int open(const char *pathname, int flags, mode_t mode) {
  // FIXME safety
  char tmp[4000];
  if (real_open == NULL) {
    proc_mnt = getenv("REMOTE_PROC_MNT");
    if (proc_mnt == NULL) {
      fprintf(stderr, "REMOTE_PROC_MNT not defined\n");
      return -1;
    }
    proc_mnt_len = strlen(proc_mnt);
    real_open = dlsym(RTLD_NEXT, "open");
    assert(real_open != NULL);
  }
  if (strncmp(pathname, "/proc", 5) == 0) {
    strcpy(tmp, proc_mnt);
    strcpy(tmp + proc_mnt_len, pathname + 5);
  } else {
    strcpy(tmp, pathname);
  }
  return (*real_open)(tmp, flags, mode);
}
