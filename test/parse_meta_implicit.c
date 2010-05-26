/* Parser check */
#include "test.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
  const char* filedir = __TEST__ "dirfile";
  const char* format = __TEST__ "dirfile/format";
  const char* format_data =
    "parent RAW UINT8 1\n"
    "parent/child CONST UINT8 1\n";
  int fd;
  signed char c;
  int r = 0;

  mkdir(filedir, 0777);

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  DIRFILE* D = gd_open(filedir, GD_RDONLY | GD_VERBOSE);
  int error = gd_error(D);
  CHECKI(error,0);

  gd_get_constant(D, "parent/child", GD_INT8, &c);
  int error2 = gd_error(D);
  CHECKI(error2,0);
  CHECKI(c,1);

  gd_close(D);

  unlink(format);
  rmdir(filedir);

  return r;
}
