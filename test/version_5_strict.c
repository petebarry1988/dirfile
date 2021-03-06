/* Copyright (C) 2010-2011 D. V. Wiebe
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
/* CheckStandards Version 5 strictness */
#include "test.h"

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int cb(gd_parser_data_t *pdata, void *ll)
{
  ((int*)ll)[pdata->linenum - 1] = 1;
  return GD_SYNTAX_IGNORE;
}

int main(void)
{
  const char *filedir = "dirfile";
  const char *format = "dirfile/format";
  const char *data = "dirfile/ar";
  const char *format_data =
    "/VERSION 5\n"
    "ENDIAN little\n"
    "X<r RAW UINT8 8\n"
    "/ENCODING raw\n"
    "/REFERENCE ar\n"
    "/PROTECT none\n"
    "ar RAW UINT8 8\n";
  uint16_t c[8];
  int ll[7] = {0, 0, 0, 0, 0, 0, 0};
  uint16_t d[8];
  unsigned char data_data[256];
  int fd, n, error, m, error2, i, r = 0;
  DIRFILE *D;

  memset(c, 0, 8);
  rmdirfile();
  mkdir(filedir, 0777);

  for (fd = 0; fd < 256; ++fd)
    data_data[fd] = (unsigned char)fd;

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  fd = open(data, O_CREAT | O_EXCL | O_WRONLY | O_BINARY, 0666);
  write(fd, data_data, 256);
  close(fd);

  D = gd_cbopen(filedir, GD_RDONLY | GD_PEDANTIC, cb, ll);
  n = gd_getdata(D, "ar", 5, 0, 1, 0, GD_UINT16, c);
  error = gd_error(D);

  m = gd_getdata(D, "FILEFRAM", 5, 0, 8, 0, GD_UINT16, d);
  error2 = gd_error(D);

  gd_close(D);

  unlink(data);
  unlink(format);
  rmdir(filedir);

  CHECKI(error,0);
  CHECKI(error2,0);
  CHECKI(n,8);

  for (i = 0; i < 7; ++i)
    if ((i == 2 || i == 3 || i == 4 || i == 5)) {
      CHECKIi(i,!ll[i],0);
    } else
      CHECKIi(i,ll[i],0);

  for (i = 0; i < 8; ++i)
    CHECKIi(i,c[i], 40 + i);

  CHECKI(m,8);

  for (i = 0; i < 8; ++i)
    CHECKIi(i,d[i], 5 + i);

  return r;
}
