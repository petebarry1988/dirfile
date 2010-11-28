/* Test gd_fragmentname out-of-range handling */
#include "test.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
  const char* filedir = __TEST__ "dirfile";
  const char* format = __TEST__ "dirfile/format";
  const char* format1 = __TEST__ "dirfile/format1";
  const char* format_data = "INCLUDE format1\n";
  const char* format1_data = "data RAW UINT8 11\n";
  const char* form0 = NULL;
  const char* form1 = NULL;
  int fd, error, r = 0;
  DIRFILE *D;

  mkdir(filedir, 0777);

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  fd = open(format1, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format1_data, strlen(format1_data));
  close(fd);

  D = gd_open(filedir, GD_RDONLY);
  form0 = gd_fragmentname(D, -3000);
  form1 = gd_fragmentname(D, 1000);
  error = gd_error(D);
  gd_close(D);

  unlink(format1);
  unlink(format);
  rmdir(filedir);

  CHECKP(form0);
  CHECKP(form1);
  CHECKI(error, GD_E_BAD_INDEX);

  return r;
}
