#ifndef MAKEDIR
#define MAKEDIR

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "types/posix_header.h"


#define BLOCK_SIZE 512

void set_checksum(struct posix_header *hd);

int makedir(int fd_out, char * path_tar, char * path_cwd, char * dir_name);


#endif
