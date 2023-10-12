/*
    Copyright (C) 2023 Hartmut Monien

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "flint.h"
#include "perm.h"
#include "ulong_extras.h"

int
main (void)
{
  int i, j;
  FLINT_TEST_INIT (state);


  flint_printf ("power....");
  fflush (stdout);

  /* check that a^n == a^n (calculated naively by compose) */

  for (i = 0; i < 10000; i++)
    {

      slong n, k, *a, *b, *c;

      n = n_randint (state, 100);
      k = n_randint (state, 100);

      a = _perm_init (n);
      b = _perm_init (n);
      c = _perm_init (n);

      _perm_randtest (a, n, state);

      /* compute power directly */

      for (j = 0; j < k; j++)
	{
	  _perm_compose (b, b, a, n);
	}

      _perm_power (c, a, k, n);

      /* _perm_print_pretty(b, n); */
      /* flint_printf("\n"); */

      /* _perm_print_pretty(c, n); */
      /* flint_printf("\n\n"); */
   
      if (!_perm_equal (b, c, n))
	{
	  flint_printf ("FAIL:\n");
	  flint_printf ("a: ");
	  _perm_print (a, n);
	  flint_printf("\n");
	  _perm_print_pretty (a, n);
	  flint_printf ("\n\n");
	  flint_printf ("b: ");
	  _perm_print_pretty (b, n);
	  flint_printf ("\n\n");
	  flint_printf ("c: ");
	  _perm_print_pretty (c, n);
	  flint_printf ("\n\n");
	  fflush (stdout);
	  flint_abort ();
	}

      _perm_clear(a);
      _perm_clear(b);
      _perm_clear(c);
    }

  FLINT_TEST_CLEANUP(state);
  
  flint_printf("PASS\n");
  return 0;
}
