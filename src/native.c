/* Copyright (C) 2009-2012 D. V. Wiebe
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

gd_type_t _GD_NativeType(DIRFILE *restrict D, gd_entry_t *restrict E, int repr)
{
  gd_type_t type = GD_UNKNOWN;
  int i;

  dtrace("%p, %p, %i", D, E, repr);

  if (++D->recurse_level >= GD_MAX_RECURSE_LEVEL) {
    _GD_SetError(D, GD_E_RECURSE_LEVEL, GD_E_RECURSE_CODE, NULL, 0, E->field);
    D->recurse_level--;
    dreturn("%u", type);
    return type;
  }

  switch(E->field_type) {
    case GD_RAW_ENTRY:
      type = E->EN(raw,data_type);
      break;
    case GD_LINCOM_ENTRY:
      if (!E->e->calculated)
        _GD_CalculateEntry(D, E, 1);

      if (E->comp_scal) {
        type = GD_COMPLEX128;
        break;
      }

      for (i = 0; i < E->EN(lincom,n_fields); ++i) {
        if (_GD_BadInput(D, E, i, 1))
          break;

        if (_GD_NativeType(D, E->e->entry[i], E->e->repr[i]) & GD_COMPLEX) {
          type = GD_COMPLEX128;
          break;
        }
      }
      type = GD_FLOAT64;
      break;
    case GD_LINTERP_ENTRY:
      /* initialise the table, if necessary */
      if (E->e->u.linterp.table_len < 0) {
        _GD_ReadLinterpFile(D, E);
        if (D->error != GD_E_OK)
          break;
      }

      type = E->e->u.linterp.complex_table ? GD_COMPLEX128 : GD_FLOAT64;
      break;
    case GD_MULTIPLY_ENTRY:
    case GD_DIVIDE_ENTRY:
      if (_GD_BadInput(D, E, 0, 1) || _GD_BadInput(D, E, 1, 1))
        break;

      type = (_GD_NativeType(D, E->e->entry[0], E->e->repr[0]) & GD_COMPLEX
          || _GD_NativeType(D, E->e->entry[1], E->e->repr[1]) & GD_COMPLEX)
        ? GD_COMPLEX128 : GD_FLOAT64;
      break;
    case GD_RECIP_ENTRY:
      if (_GD_BadInput(D, E, 0, 1))
        break;

      type = ((_GD_NativeType(D, E->e->entry[0], E->e->repr[0]) & GD_COMPLEX)
          || E->comp_scal) ?  GD_COMPLEX128 : GD_FLOAT64;
      break;
    case GD_BIT_ENTRY:
    case GD_INDEX_ENTRY:
      type = GD_UINT64;
      break;
    case GD_PHASE_ENTRY:
    case GD_WINDOW_ENTRY:
    case GD_MPLEX_ENTRY:
      if (_GD_BadInput(D, E, 0, 1))
        break;

      type = _GD_NativeType(D, E->e->entry[0], E->e->repr[0]);
      break;
    case GD_POLYNOM_ENTRY:
      if (!E->e->calculated)
        _GD_CalculateEntry(D, E, 1);

      if (E->comp_scal) {
        type = GD_COMPLEX128;
        break;
      }

      if (_GD_BadInput(D, E, 0, 1))
        break;

      type = (_GD_NativeType(D, E->e->entry[0], E->e->repr[0]) & GD_COMPLEX) ?
        GD_COMPLEX128 : GD_FLOAT64;

      break;
    case GD_SBIT_ENTRY:
      type = GD_INT64;
      break;
    case GD_CONST_ENTRY:
    case GD_CARRAY_ENTRY:
      type = E->EN(scalar,const_type);
      break;
    case GD_STRING_ENTRY:
      type = GD_NULL;
      break;
    case GD_NO_ENTRY:
    case GD_ALIAS_ENTRY:
      _GD_InternalError(D);
  }

  /* representation */
  if (repr != GD_REPR_NONE) {
    if (type == GD_COMPLEX128)
      type = GD_FLOAT64;
    else if (type == GD_COMPLEX64)
      type = GD_FLOAT32;
  }

  /* catch errors */
  if (D->error)
    type = GD_UNKNOWN;

  D->recurse_level--;

  dreturn("0x%02x", type);
  return type;
}

gd_type_t gd_native_type(DIRFILE* D, const char* field_code_in) gd_nothrow
{
  gd_type_t type = GD_UNKNOWN;
  gd_entry_t* entry;
  int repr;
  char* field_code;

  dtrace("%p, \"%s\"", D, field_code_in);

  if (D->flags & GD_INVALID) {/* don't crash */
    _GD_SetError(D, GD_E_BAD_DIRFILE, 0, NULL, 0, NULL);
    dreturn("0x%x", GD_UNKNOWN);
    return GD_UNKNOWN;
  }

  _GD_ClearError(D);

  entry = _GD_FindFieldAndRepr(D, field_code_in, &field_code, &repr, NULL, 1,
      1);

  if (D->error) {
    dreturn("0x%x", GD_UNKNOWN);
    return GD_UNKNOWN;
  }

  type = _GD_NativeType(D, entry, repr);

  if (field_code != field_code_in)
    free(field_code);

  dreturn("0x%x", type);
  return type;
}
/* vim: ts=2 sw=2 et tw=80
*/
