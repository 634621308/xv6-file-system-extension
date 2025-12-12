#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static char*
typestr(short type)
{
  switch(type){
  case T_DIR:
    return "dir";
  case T_FILE:
    return "file";
  case T_DEVICE:
    return "dev";
  case T_SYMLINK:
    return "symlink";
  default:
    return "unknown";
  }
}

int
main(int argc, char *argv[])
{
  int i = 1;
  int nofollow = 0;
  struct stat st;

  if(argc > 1 && strcmp(argv[1], "-L") == 0){
    nofollow = 1;
    i = 2;
  }

  if(argc <= i){
    fprintf(2, "usage: stat [-L] path...\n");
    exit(1);
  }

  for(; i < argc; i++){
    char *path = argv[i];
    int ret = nofollow ? lstat(path, &st) : stat(path, &st);
    if(ret < 0){
      fprintf(2, "stat: cannot stat %s\n", path);
      continue;
    }
    printf("%s: type=%s inode=%d size=%d nlink=%d\n",
           path, typestr(st.type), st.ino, (int)st.size, st.nlink);
  }
  exit(0);
}

