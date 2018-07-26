/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_.h"

void
net_domain_cylinder_string (char *domain, char *nstring, struct POLY *pDomain)
{
  int pseudodim;
  double pseudosize;
  double *parms = ut_alloc_1d (10);

  net_domain_cylinderparms (domain, nstring, parms, &pseudodim, &pseudosize);
  net_domain_cylinder (parms, pDomain);

  if (pseudodim != -1)
  {
    (*pDomain).PseudoDim = pseudodim;
    (*pDomain).PseudoSize = pseudosize;
  }

  ut_free_1d (parms);

  return;
}

void
net_domain_cylinderparms (char *domain, char *nstring, double *parms,
                          int *ppseudodim, double *ppseudosize)
{
  int i, varqty;
  char **vars = NULL;
  char *fct = NULL;

  parms[2] = -1;
  *ppseudodim = -1;
  *ppseudosize = -DBL_MAX;

  ut_string_function_separate (domain, &fct, NULL, &vars, &varqty);

  if (!strncmp (fct, "cylinder", 8))
  {
    if (varqty != 2 && varqty != 3)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

    for (i = 0; i < varqty; i++)
      ut_string_real (vars[i], parms + i);
  }

  else if (!strcmp (fct, "circle"))
  {
    if (varqty != 1 && varqty != 2)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

    for (i = 0; i < varqty; i++)
      ut_string_real (vars[i], parms + i + 1);
    parms[0] = 1e-6 * parms[1];

    *ppseudodim = 2;
    *ppseudosize = parms[0];
  }

  else
    ut_error_reportbug ();

  if (ut_num_d2ri (parms[2]) != -1 && ut_num_d2ri (parms[2]) < 3)
  {
    ut_print_message (1, 2, "Increasing the number of facets to 3.\n");
    parms[2] = 3;
  }

  if (ut_num_d2ri (parms[2]) == -1)
  {
    double cl;
    int varqty = 1;
    char **vars = ut_alloc_2d_char (1, 10);
    double *vals = ut_alloc_1d (1);
    double res;

    strcpy (vars[0], "vol");
    if (!strcmp (fct, "cylinder"))
      vals[0] = M_PI * parms[0] * parms[0] * parms[1] / 4;
    else if (!strcmp (fct, "circle"))
      vals[0] = M_PI * parms[1] / 4;
    else
      abort ();

    if (!strcmp (nstring, "from_morpho"))
      ut_print_messagewnc (2, 72, "The number of facets must be specified in `-morpho cylinder' when using `-n from_morpho'.");

    ut_math_eval (nstring, 1, vars, vals, &res);
    res = ut_num_max (res, 1);

    ut_free_2d_char (vars, varqty);
    ut_free_1d (vals);

    if (!strcmp (fct, "cylinder"))
      rcl2cl (1, 3, M_PI * parms[0] * parms[0] * parms[1] / 4, ut_num_d2ri (res), NULL, &cl);
    else if (!strcmp (fct, "circle"))
      rcl2cl (1, 3, M_PI * parms[1] / 4, ut_num_d2ri (res), NULL, &cl);
    else
      abort ();

    parms[2] = (int) floor (M_PI * parms[1] / cl);
    parms[2] = ut_num_max_int (parms[2], 12);
  }
  parms[2] += 2;

  ut_free_1d_char (fct);
  ut_free_2d_char (vars, varqty);

  return;
}

void
net_domain_cylinder (double *parms, struct POLY *pDomain)
{
  double **eq = ut_alloc_2d (ut_num_d2ri (parms[2]), 4);

  net_domain_cylinder_planes (parms[0], parms[1] / 2, parms[2] - 2, eq);
  net_domain_clip (pDomain, eq, parms[2]);
  ut_free_2d (eq, parms[2]);

  return;
}

void
net_domain_stdtriangle_string (char *domain, struct POLY *pDomain)
{
  double *parms = ut_alloc_1d (2);

  net_domain_stdtriangleparms (domain, parms);
  net_domain_stdtriangle (parms, pDomain);

  ut_free_1d (parms);

  return;
}

void
net_domain_stdtriangleparms (char *domain, double *parms)
{
  int varqty;
  char **vars = NULL;

  ut_string_function_separate (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  ut_string_real (vars[0], parms);

  parms[0] += 4;

  ut_free_2d_char (vars, varqty);

  return;
}

void
net_domain_stdtriangle (double *parms, struct POLY *pDomain)
{
  int qty = ut_num_d2ri (parms[0]);
  double **eq = ut_alloc_2d (qty, 4);

  net_domain_stdtriangle_planes (qty - 4, eq);
  net_domain_clip (pDomain, eq, qty);

  (*pDomain).PseudoDim = 2;
  (*pDomain).PseudoSize = 1e-6;

  ut_free_2d (eq, qty);

  return;
}

void
net_domain_sphere_string (char *domain, struct POLY *pDomain)
{
  double *parms = ut_alloc_1d (2);

  net_domain_sphereparms (domain, parms);
  net_domain_sphere (parms, pDomain);

  ut_free_1d (parms);

  return;
}

void
net_domain_sphereparms (char *domain, double *parms)
{
  int i, varqty;
  char **vars = NULL;

  parms[1] = 100;

  ut_string_function_separate (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1 && varqty != 2)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  for (i = 0; i < varqty; i++)
    ut_string_real (vars[i], parms + i);

  parms[0] += 4;

  if (parms[1] != -1 && parms[1] < 4)
  {
    ut_print_message (1, 2, "Increasing the number of facets to 4.\n");
    parms[1] = 4;
  }

  ut_free_2d_char (vars, varqty);

  return;
}

void
net_domain_sphere (double *parms, struct POLY *pDomain)
{
  double **eq = ut_alloc_2d (parms[1], 4);

  net_domain_sphere_planes (parms[0] / 2, parms[1], eq);
  net_domain_clip (pDomain, eq, parms[1]);

  ut_free_2d (eq, parms[1]);

  return;
}

void
net_domain_rodrigues_string (char *domain, struct POLY *pDomain)
{
  char *crysym = NULL;

  net_domain_rodriguesparms (domain, &crysym);
  net_domain_rodrigues (crysym, pDomain);

  ut_free_1d_char (crysym);

  return;
}

void
net_domain_rodriguesparms (char *domain, char **pcrysym)
{
  int varqty;
  char **vars = NULL;

  ut_string_function_separate (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  ut_string_string (vars[0], pcrysym);

  ut_free_2d_char (vars, varqty);

  return;
}

void
net_domain_rodrigues (char *crysym, struct POLY *pDomain)
{
  int i, eqqty, n = ol_crysym_qty (crysym);
  double **eq = NULL;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();
  double *r = ol_r_alloc ();
  double theta, thetamax;
  double *O = ut_alloc_1d (4);
  double **cubesize = ut_alloc_2d (3, 2);
  srand48 (1);

  thetamax = tan (0.99 * M_PI * 0.5);
  for (i = 0; i < 3; i++)
    ut_array_1d_set_2 (cubesize[i], -thetamax, thetamax);
  net_domain_cube (cubesize, pDomain);

  if (!strcmp (crysym, "m-3"))
    ut_print_message (1, 2, "Face positions are randomized a bit to avoid degeneracy.\n");

  eqqty = 0;
  for (i = 2; i <= n; i++)
  {
    ol_q_crysym (q1, crysym, i, q2);
    ol_q_rtheta_rad (q2, r, &theta);
    if (!strcmp (crysym, "m-3"))
      theta += 1e-3 * (2 * drand48 () - 1);

    eq = ut_realloc_2d_addline (eq, ++eqqty, 4);

    ut_array_1d_memcpy (eq[eqqty - 1] + 1, 3, r);
    eq[eqqty - 1][0] = tan (theta / 4);
    if (eq[eqqty - 1][0] < 0)
      ut_array_1d_scale (eq[eqqty - 1], 4, -1);

    if (theta > M_PI - OL_EPS)
    {
      eq = ut_realloc_2d_addline (eq, ++eqqty, 4);
      ut_array_1d_memcpy (eq[eqqty - 1], 4, eq[eqqty - 2]);
      ut_array_1d_scale (eq[eqqty - 1] + 1, 3, -1);
    }
  }

  if (eqqty > 0)
    net_domain_clip (pDomain, eq, eqqty);

  ut_free_2d (cubesize, 3);
  ut_free_1d (O);
  ol_r_free (r);
  ol_q_free (q1);
  ol_q_free (q2);
  ut_free_2d (eq, eqqty);

  return;
}

void
net_domain_planes_string (char *domain, int dim, struct POLY *pDomain)
{
  int eqqty;
  double **eqs = NULL;

  net_domain_planesparms (domain, dim, &eqs, &eqqty);
  net_domain_planes (eqs, eqqty, pDomain);

  ut_free_2d (eqs, eqqty);

  return;
}

void
net_domain_planesparms (char *domain, int dim, double ***peqs, int *peqqty)
{
  int i, varqty, shift;
  char **vars = NULL, *filename = NULL;
  FILE *file = NULL;

  ut_string_function_separate (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  ut_string_string (vars[0], &filename);

  file = ut_file_open (filename, "r");

  if (fscanf (file, "%d", peqqty) != 1)
    abort ();

  if (dim == 2)
  {
    shift = 2;
    (*peqqty) += 2;
  }
  else
    shift = 0;

  (*peqs) = ut_alloc_2d (*peqqty, 4);

  // recording z0 and z1 faces
  if (dim == 2)
  {
    (*peqs)[0][3] = -1;
    (*peqs)[1][3] = 1;
    (*peqs)[1][0] = 1e-6;
  }

  // recording faces
  for (i = shift; i < *peqqty; i++)
  {
    ut_array_1d_fscanf (file, (*peqs)[i], 4);
    if (ut_array_1d_norm ((*peqs)[i] + 1, 3) == 0)
      ut_print_message (2, 3, "Face %d: normal norm is zero.\n", i - 1);
    ut_array_1d_scale ((*peqs)[i], 4, 1. / ut_vector_norm ((*peqs)[i] + 1));
  }

  ut_free_2d_char (vars, varqty);
  ut_file_close (file, filename, "r");
  ut_free_1d_char (filename);

  return;
}

void
net_domain_planes (double **eqs, int eqqty, struct POLY *pDomain)
{
  net_domain_clip (pDomain, eqs, eqqty);

  return;
}
