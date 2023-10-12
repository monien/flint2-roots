/*
    Copyright (C) 2023 Hartmut Monien

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "flint.h"
#include "fmpz_mat.h"
#include "perm.h"
#include "ulong_extras.h"

int
main (void)
{
  int i;
  
  FLINT_TEST_INIT (state);


  flint_printf ("mat....");
  fflush (stdout);

  for (i = 0; i < 1000; i++)
    {
      int flag = 1;
      slong n, *a, *b;

      fmpz_t tmp;
      fmpz_mat_t a_mat, b_mat;

      n = n_randint (state, 10);

      a = _perm_init (n);
      b = _perm_init (n);

      fmpz_init(tmp);
      
      fmpz_mat_init(a_mat, n, n);
      fmpz_mat_init(b_mat, n, n);

      _perm_randtest (a, n, state);
      _perm_mat(a_mat, a, n);
      
      flag = fmpz_mat_inv(a_mat, tmp, a_mat);

      if( !flag ) {
	flint_printf ("FAIL:\n");
	flint_printf ("a: ");
	_perm_print_pretty (a, n);
	flint_printf ("\n\n");
	fflush (stdout);
	flint_abort ();
      }

      if ( fmpz_cmp_ui(tmp, 0) < 0 ) {
	fmpz_mat_neg(a_mat, a_mat);
      }
      
      _perm_inv(b, a, n);
      _perm_mat(b_mat, b, n);

      if( !fmpz_mat_equal(a_mat, b_mat) ) {
	flint_printf ("FAIL:\n");
	flint_printf ("a: ");
	_perm_print_pretty (a, n);
	flint_printf ("\n\n");
	fflush (stdout);
	flint_abort ();
      }

      _perm_clear(a);
      _perm_clear(b);
      fmpz_mat_clear(a_mat);
      fmpz_mat_clear(b_mat);
      fmpz_clear(tmp);
    }

  FLINT_TEST_CLEANUP(state);
  
  flint_printf("PASS\n");
  return 0;

}
