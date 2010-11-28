#define GD_C89_API
#include "test.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
  const char* filedir = __TEST__ "dirfile";
  const char* format = __TEST__ "dirfile/format";
  int error, ge_error, r = 0;
  gd_entry_t e;
  double div[2] = {3.2, 3.1};

  DIRFILE* D = gd_open(filedir, GD_RDWR | GD_CREAT | GD_VERBOSE);
  gd_add_phase(D, "new", "in", 3, 0);
  gd_madd_crecip(D, "new", "meta", "in1", div);
  error = gd_error(D);

  /* check */
  gd_entry(D, "new/meta", &e);
  ge_error = gd_error(D);
  CHECKI(ge_error, 0);
  if (!r) {
    CHECKI(e.field_type, GD_RECIP_ENTRY);
    CHECKS(e.in_fields[0], "in1");
    CHECKF(e.EN(recip,cdividend)[0], div[0]);
    CHECKF(e.EN(recip,cdividend)[1], div[1]);
    CHECKI(e.comp_scal, 1);
    CHECKI(e.fragment_index, 0);
    gd_free_entry_strings(&e);
  }

  gd_close(D);

  unlink(format);
  rmdir(filedir);

  CHECKI(error, GD_E_OK);
  return r;
}
