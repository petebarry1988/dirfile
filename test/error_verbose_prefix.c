/* Copyright (C) 2012 D. V. Wiebe
 *
 ***************************************************************************
 *
 * This file is part of the GetData project.
 *
 * GetData is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * GetData is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GetData; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "test.h"

#include <stdlib.h>
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
#if !defined HAVE_MKFIFO || !defined HAVE_WORKING_FORK || defined __CYGWIN__
  return 77; /* skip */
#else
  const char *filedir = "dirfile";
  const char *fifo = "dirfile/fifo";
  const char *format = "dirfile/format";
  int e1, e2, r = 0, status;
  FILE *stream;
  pid_t pid;
  DIRFILE *D;

  rmdirfile();
  mkdir(filedir, 0777);
  close(open(format, O_CREAT | O_EXCL | O_WRONLY, 0666));
  mkfifo(fifo, 0666);

  /* read our standard error */
  if ((pid = fork()) == 0) {
    char string[1024];
    stream = fopen(fifo, "r");

    fgets(string, 1024, stream);
    CHECKS(string, "getdata-test: libgetdata: Field not found: data\n");
    return r;
  }

  /* retarget stderr */
  freopen(fifo, "w", stderr);

  D = gd_open(filedir, GD_RDONLY);
  e1 = gd_error(D);
  gd_flags(D, GD_VERBOSE, 0);
  gd_verbose_prefix(D, "getdata-test: ");
  gd_validate(D, "data");
  e2 = gd_error(D);
  gd_close(D);

  fputs("\n", stderr);
  fflush(stderr);

  /* restore stderr */
  freopen("/dev/stderr", "w", stderr);

  unlink(fifo);
  unlink(format);
  rmdir(filedir);

  CHECKI(e1, 0);
  CHECKI(e2, GD_E_BAD_CODE);

  wait(&status);
  if (status)
    r = 1;

  return r;
#endif
}
