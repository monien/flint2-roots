/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2010 Fredrik Johansson

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "flint.h"
#include "ulong_extras.h"


void check(mp_limb_t n, int s1, int s2)
{
    if (s1 != s2)
    {
        printf("FAIL:\n");
        printf("%lu: got %d instead of %d\n", n, s1, s2); 
        abort();
    }
}

int main(void)
{
    int s, k;

    printf("is_squarefree....");
    fflush(stdout);

    check(0, n_is_squarefree(0), 0);
    check(1, n_is_squarefree(1), 1);
    check(2, n_is_squarefree(2), 1);
    check(3, n_is_squarefree(3), 1);
    check(4, n_is_squarefree(4), 0);
    check(5, n_is_squarefree(5), 1);

    check(16, n_is_squarefree(16), 0);
    check(25, n_is_squarefree(25), 0);
    check(49, n_is_squarefree(49), 0);
    check(16*3, n_is_squarefree(16*3), 0);
    check(25*3, n_is_squarefree(25*3), 0);
    check(49*3, n_is_squarefree(49*3), 0);

    check(101*103, n_is_squarefree(101*103), 1);
    check(101*101, n_is_squarefree(101*101), 0);
    check(101*103*4, n_is_squarefree(101*103*4), 0);
    check(101*103*5, n_is_squarefree(101*103*5), 1);
    check(101*103*103*5, n_is_squarefree(101*103*103*5), 0);
    check(101*103*25, n_is_squarefree(101*103*25), 0);

    s = 0;
    for (k = 0; k <= 10000; k++)
        s += n_is_squarefree(k);

    if (s != 6083)
    {
        printf("FAIL:\n");
        printf("expected %d squarefree numbers <= 10000 (got %d)\n", 6083, s);
        abort();
    }

    printf("PASS\n");
    return 0;
}
