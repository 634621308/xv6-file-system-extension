#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"

static char*
basename(char *path)
{
  char *p = path + strlen(path);
  while(p > path && *(p-1) == '/')
    p--;
  while(p > path && *(p-1) != '/')
    p--;
  return p;
}

static void
find(char *path, char *namef, char typef)
{
  struct stat st;
  char actual;

  if(lstat(path, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    return;
  }

  if(st.type == T_FILE)
    actual = 'f';
  else if(st.type == T_DIR)
    actual = 'd';
  else if(st.type == T_SYMLINK)
    actual = 'l';
  else
    actual = '?';

  if((!namef || strcmp(basename(path), namef) == 0) &&
     (typef == 0 || actual == typef)){
    printf("%s\n", path);
  }

  if(st.type != T_DIR)
    return;

  int fd = open(path, O_RDONLY);
  if(fd < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  char buf[512];
  struct dirent de;
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      fprintf(2, "find: path too long\n");
      continue;
    }
    strcpy(buf, path);
    char *p = buf + strlen(buf);
    *p++ = '/';
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    find(buf, namef, typef);
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  char *path;
  char *namef = 0;
  char typef = 0;
  int i = 1;

  if(argc < 2){
    fprintf(2, "usage: find path [-name name] [-type f|d|l]\n");
    exit(1);
  }

  path = argv[i++];
  while(i < argc){
    if(strcmp(argv[i], "-name") == 0){
      if(i + 1 >= argc){
        fprintf(2, "find: missing name\n");
        exit(1);
      }
      namef = argv[i+1];
      i += 2;
    } else if(strcmp(argv[i], "-type") == 0){
      if(i + 1 >= argc){
        fprintf(2, "find: missing type\n");
        exit(1);
      }
      typef = argv[i+1][0];
      i += 2;
    } else {
      fprintf(2, "find: unknown option %s\n", argv[i]);
      exit(1);
    }
  }

  find(path, namef, typef);
  exit(0);
}

