/* Copyright (C) 2008-2011 D. V. Wiebe
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
/* Retreiving the number of fields of a field should succeed cleanly */
#include "test.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(void)
{
  const char *filedir = "dirfile";
  const char *format = "dirfile/format";
  const char *format_data =
    "raw1 RAW UINT8 1\n"
    "META raw1 linterp1 LINTERP raw2 table\n"
    "META raw1 linterp2 LINTERP raw3 table\n"
    "META raw1 linterp3 LINTERP raw4 table\n"
    "META raw1 const CONST UINT8 1\n"
    "META raw1 string STRING value\n"
    "META raw1 string2 STRING value\n"
    "raw2 RAW UINT8 1\n"
    "raw3 RAW UINT8 1\n"
    "raw4 RAW UINT8 1\n"
    "const CONST UINT8 1\n"
    "string STRING value\n"
    "string2 STRING value\n";
  int fd, error, r = 0;
  unsigned int nfields;
  DIRFILE *D;

  rmdirfile();
  mkdir(filedir, 0777);

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  D = gd_open(filedir, GD_RDONLY | GD_VERBOSE);
  nfields = gd_nmfields(D, "raw1");
  error = gd_error(D);
  gd_close(D);

  unlink(format);
  rmdir(filedir);

  CHECKI(error, 0);
  CHECKU(nfields, 6);

  return r;
}
