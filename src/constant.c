/* Copyright (C) 2008-2010 D. V. Wiebe
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
#include "internal.h"

int gd_get_carray_slice(DIRFILE* D, const char *field_code_in,
    unsigned int start, size_t n, gd_type_t return_type, void *data_out)
gd_nothrow
{
  gd_entry_t *entry;
  char* field_code;
  int repr;

  dtrace("%p, \"%s\", %i, %" PRNsize_t ", 0x%x, %p", D, field_code_in,
      (int)start, n, return_type, data_out);

  if (D->flags & GD_INVALID) {
    _GD_SetError(D, GD_E_BAD_DIRFILE, 0, NULL, 0, NULL);
    dreturn("%i", -1);
    return -1;
  }

  _GD_ClearError(D);

  entry = _GD_FindFieldAndRepr(D, field_code_in, &field_code, &repr, NULL, 1,
      1);

  if (D->error) {
    dreturn("%i", -1);
    return -1;
  }

  if (entry->field_type != GD_CARRAY_ENTRY &&
      entry->field_type != GD_CONST_ENTRY)
  {
    _GD_SetError(D, GD_E_BAD_FIELD_TYPE, GD_E_FIELD_BAD, NULL, 0, field_code);
  } else if (start + n > ((entry->field_type == GD_CONST_ENTRY) ? 1 :
      entry->EN(scalar,array_len)))
  {
    _GD_SetError(D, GD_E_BOUNDS, 0, NULL, 0, NULL);
  } else if (!D->error)
    _GD_DoField(D, entry, repr, start, n, return_type, data_out);

  if (field_code != field_code_in)
    free(field_code);

  if (D->error) {
    dreturn("%i", -1);
    return -1;
  }

  dreturn("%i", 0);
  return 0;
}

int gd_get_carray(DIRFILE *D, const char *field_code_in, gd_type_t return_type,
    void *data_out) gd_nothrow
{
  gd_entry_t *entry;
  char* field_code;
  int repr;

  dtrace("%p, \"%s\", 0x%x, %p", D, field_code_in, return_type, data_out);

  if (D->flags & GD_INVALID) {
    _GD_SetError(D, GD_E_BAD_DIRFILE, 0, NULL, 0, NULL);
    dreturn("%i", -1);
    return -1;
  }

  _GD_ClearError(D);

  entry = _GD_FindFieldAndRepr(D, field_code_in, &field_code, &repr, NULL, 1,
      1);

  if (D->error) {
    dreturn("%i", -1);
    return -1;
  }

  if (entry->field_type != GD_CARRAY_ENTRY &&
      entry->field_type != GD_CONST_ENTRY)
  {
    _GD_SetError(D, GD_E_BAD_FIELD_TYPE, GD_E_FIELD_BAD, NULL, 0, field_code);
  } else if (!D->error)
    _GD_DoField(D, entry, repr, 0, (entry->field_type == GD_CONST_ENTRY) ? 1 :
        entry->EN(scalar,array_len), return_type, data_out);

  if (field_code != field_code_in)
    free(field_code);

  if (D->error) {
    dreturn("%i", -1);
    return -1;
  }

  dreturn("%i", 0);
  return 0;
}

int gd_get_constant(DIRFILE* D, const char *field_code_in,
    gd_type_t return_type, void *data_out) gd_nothrow
{
  return gd_get_carray_slice(D, field_code_in, 0, 1, return_type, data_out);
}

size_t gd_carray_len(DIRFILE *D, const char *field_code_in) gd_nothrow
{
  gd_entry_t *entry;
  char* field_code;
  int repr;

  dtrace("%p, \"%s\"", D, field_code_in);

  if (D->flags & GD_INVALID) {
    _GD_SetError(D, GD_E_BAD_DIRFILE, 0, NULL, 0, NULL);
    dreturn("%i", 0);
    return 0;
  }

  _GD_ClearError(D);

  entry = _GD_FindFieldAndRepr(D, field_code_in, &field_code, &repr, NULL, 1,
      1);

  if (D->error) {
    dreturn("%i", 0);
    return 0;
  }

  if (entry->field_type != GD_CARRAY_ENTRY &&
      entry->field_type != GD_CONST_ENTRY)
  {
    _GD_SetError(D, GD_E_BAD_FIELD_TYPE, GD_E_FIELD_BAD, NULL, 0, field_code);
  }

  if (field_code != field_code_in)
    free(field_code);

  if (D->error) {
    dreturn("%i", 0);
    return 0;
  }

  dreturn("%" PRNsize_t, (entry->field_type == GD_CONST_ENTRY) ? 1 :
    entry->EN(scalar,array_len));
  return (entry->field_type == GD_CONST_ENTRY) ? 1 :
    entry->EN(scalar,array_len);
}

int gd_put_carray_slice(DIRFILE* D, const char *field_code_in,
    unsigned int first, size_t n, gd_type_t data_type, const void *data_in)
gd_nothrow
{
  int i;
  gd_entry_t *entry;
  int repr;
  char* field_code;

  dtrace("%p, \"%s\", %i, %" PRNsize_t ", 0x%x, %p", D, field_code_in, first, n,
      data_type, data_in);

  if (D->flags & GD_INVALID) {
    _GD_SetError(D, GD_E_BAD_DIRFILE, 0, NULL, 0, NULL);
    dreturn("%i", -1);
    return -1;
  }

  if ((D->flags & GD_ACCMODE) != GD_RDWR) {
    _GD_SetError(D, GD_E_ACCMODE, 0, NULL, 0, NULL);
    dreturn("%i", -1);
    return -1;
  }

  _GD_ClearError(D);

  entry = _GD_FindFieldAndRepr(D, field_code_in, &field_code, &repr, NULL, 1,
      1);

  if (D->error) {
    dreturn("%i", -1);
    return -1;
  }

  if (entry->field_type != GD_CARRAY_ENTRY &&
      entry->field_type != GD_CONST_ENTRY)
  {
    _GD_SetError(D, GD_E_BAD_FIELD_TYPE, GD_E_FIELD_BAD, NULL, 0, field_code);
  } else if (first + n > ((entry->field_type == GD_CONST_ENTRY) ? 1 :
        entry->EN(scalar,array_len)))
  {
    _GD_SetError(D, GD_E_BOUNDS, 0, NULL, 0, NULL);
  } else
    _GD_DoFieldOut(D, entry, repr, first, n, data_type, data_in);

  if (field_code != field_code_in)
    free(field_code);

  if (D->error) {
    dreturn("%i", -1);
    return -1;
  }

  /* Flag all clients as needing recalculation */
  for (i = 0; i < entry->e->u.scalar.n_client; ++i)
    entry->e->u.scalar.client[i]->e->calculated = 0;

  /* Clear the client list */
  free(entry->e->u.scalar.client);
  entry->e->u.scalar.client = NULL;
  entry->e->u.scalar.n_client = 0;

  dreturn("%i", 0);
  return 0;
}

int gd_put_carray(DIRFILE* D, const char *field_code_in, gd_type_t data_type,
    const void *data_in) gd_nothrow
{
  int i;
  gd_entry_t *entry;
  int repr;
  char* field_code;

  dtrace("%p, \"%s\", 0x%x, %p", D, field_code_in, data_type, data_in);

  if (D->flags & GD_INVALID) {
    _GD_SetError(D, GD_E_BAD_DIRFILE, 0, NULL, 0, NULL);
    dreturn("%i", -1);
    return -1;
  }

  if ((D->flags & GD_ACCMODE) != GD_RDWR) {
    _GD_SetError(D, GD_E_ACCMODE, 0, NULL, 0, NULL);
    dreturn("%i", -1);
    return -1;
  }

  _GD_ClearError(D);

  entry = _GD_FindFieldAndRepr(D, field_code_in, &field_code, &repr, NULL, 1,
      1);

  if (D->error) {
    dreturn("%i", -1);
    return -1;
  }

  if (entry->field_type != GD_CARRAY_ENTRY &&
      entry->field_type != GD_CONST_ENTRY)
  {
    _GD_SetError(D, GD_E_BAD_FIELD_TYPE, GD_E_FIELD_BAD, NULL, 0, field_code);
  } else
    _GD_DoFieldOut(D, entry, repr, 0,
        (entry->field_type == GD_CONST_ENTRY) ? 1 : entry->EN(scalar,array_len),
        data_type, data_in);

  if (field_code != field_code_in)
    free(field_code);

  if (D->error) {
    dreturn("%i", -1);
    return -1;
  }

  /* Flag all clients as needing recalculation */
  for (i = 0; i < entry->e->u.scalar.n_client; ++i)
    entry->e->u.scalar.client[i]->e->calculated = 0;

  /* Clear the client list */
  free(entry->e->u.scalar.client);
  entry->e->u.scalar.client = NULL;
  entry->e->u.scalar.n_client = 0;

  dreturn("%i", 0);
  return 0;
}

int gd_put_constant(DIRFILE* D, const char *field_code_in, gd_type_t data_type,
    const void *data_in) gd_nothrow
{
  return gd_put_carray_slice(D, field_code_in, 0, 1, data_type, data_in);
}

/* vim: ts=2 sw=2 et tw=80
*/
