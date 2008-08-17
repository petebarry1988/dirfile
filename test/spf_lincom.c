/* The SPF of a lincom should equal the SPF of the first field */
#include "../src/getdata.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
  const char* filedir = __FILE__ "dirfile";
  const char* format = __FILE__ "dirfile/format";
  const char* format_data =
    "in1 RAW UINT8 11\n"
    "in2 RAW UINT8 13\n"
    "lincom LINCOM 2 in1 1 0 in2 1 0\n";
  int fd;

  mkdir(filedir, 0777);

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  DIRFILE* D = dirfile_open(filedir, GD_RDONLY);
  unsigned int spf = get_samples_per_frame(D, "lincom");
  dirfile_close(D);

  unlink(format);
  rmdir(filedir);

  return (spf != 11);
}
