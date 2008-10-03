/* Attempt to write UINT8 */
#include "../src/getdata.h"

#include <inttypes.h>
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
  return 77; /* writing not supported */
#if 0
  const char* filedir = __TEST__ "dirfile";
  const char* format = __TEST__ "dirfile/format";
  const char* data = __TEST__ "dirfile/data.txt";
  const char* format_data = "data RAW UINT8 8\n";
  uint8_t c[8], d;
  int fd, i;
  struct stat buf;

  memset(c, 0, 8);
  mkdir(filedir, 0777);

  for (i = 0; i < 8; ++i)
    c[i] = (uint8_t)(40 + i);

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  DIRFILE* D = dirfile_open(filedir, GD_RDWR | GD_TEXT_ENCODED);
  int n = putdata(D, "data", 5, 0, 1, 0, GD_UINT8, c);
  int error = get_error(D);

  dirfile_close(D);

  if (stat(data, &buf))
    return 1;

  fd = open(data, O_RDONLY);
  i = 0;
  while (read(fd, &d, sizeof(uint8_t))) {
    if (i < 40 || i > 48) {
      if (d != 0)
        return 1;
    } else if (d != i)
      return 1;
    i++;
  }
  fclose(stream);

  unlink(data);
  unlink(format);
  rmdir(filedir);

  if (error)
    return 1;
  if (n != 8)
    return 1;

  return 0;
#endif
}
