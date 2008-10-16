/* Try to read PHASE entry */
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
  const char* filedir = __TEST__ "dirfile";
  const char* format = __TEST__ "dirfile/format";
  const char* format_data = "shift CONST UINT8 3\ndata PHASE in1 shift\n";
  int fd;

  mkdir(filedir, 0777);

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  DIRFILE* D = dirfile_open(filedir, GD_RDONLY);
  gd_entry_t E;

  int n = get_entry(D, "data", &E);
  int error = get_error(D);

  dirfile_close(D);
  unlink(format);
  rmdir(filedir);

  if (error != GD_E_OK)
    return 1;
  if (n)
    return 1;
  if (strcmp(E.field, "data"))
    return 1;
  if (E.field_type != GD_PHASE_ENTRY)
    return 1;
  if (strcmp(E.in_fields[0], "in1"))
    return 1;
  if (E.shift != 3)
    return 1;

  return 0;
}