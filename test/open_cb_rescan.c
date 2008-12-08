/* Parser check */
#include "../src/getdata.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

static int saw_callback = 0;

int callback(const DIRFILE *dirfile __attribute (( unused )),
    int suberror __attribute__ (( unused )), char *line)
{
  if (saw_callback)
    return GD_SYNTAX_ABORT;

  saw_callback = 1;

  sprintf(line, "/REFERENCE data\n");

  return GD_SYNTAX_RESCAN;
}

int main(void)
{
  const char* filedir = __TEST__ "dirfile";
  const char* format = __TEST__ "dirfile/format";
  const char* format_data = "BADDIRECTIVE BADTYPE\n";
  int fd;

  mkdir(filedir, 0777);

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  DIRFILE* D = dirfile_cbopen(filedir, GD_RDONLY, callback);
  int error = get_error(D);
  dirfile_close(D);

  unlink(format);
  rmdir(filedir);

  if (!saw_callback)
    return 1;

  return (error != GD_E_BAD_REFERENCE);
}