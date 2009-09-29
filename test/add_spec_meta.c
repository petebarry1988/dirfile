/* add a barth-style meta field with add_spec */
#include "../src/getdata.h"

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
  int r = 0;
  gd_entry_t e;

  DIRFILE* D = dirfile_open(filedir, GD_RDWR | GD_CREAT | GD_VERBOSE);
  dirfile_add_spec(D, "INDEX/meta CONST UINT8 2", 0);
  int error = get_error(D);
  unsigned char val;

  /* check */
  int n = get_nfields(D);

  if (n != 1)
    r = 1;

  get_entry(D, "INDEX/meta", &e);
  if (get_error(D))
    r = 1;
  else {
    if (e.field_type != GD_CONST_ENTRY) {
      fprintf(stderr, "field_type = %i\n", e.field_type);
      r = 1;
    }
    if (e.fragment_index != 0) {
      fprintf(stderr, "fragment_index = %i\n", e.fragment_index);
      r = 1;
    }
    if (e.const_type != GD_UINT8) {
      fprintf(stderr, "const_type = %i\n", e.const_type);
      r = 1;
    }
    get_constant(D, "INDEX/meta", GD_UINT8, &val);
    if (val != 2) {
      fprintf(stderr, "val = %i\n", val);
      r = 1;
    }
    dirfile_free_entry_strings(&e);
  }

  dirfile_close(D);

  unlink(format);
  rmdir(filedir);

  if (r)
    return 1;

  return (error != GD_E_OK);
}
