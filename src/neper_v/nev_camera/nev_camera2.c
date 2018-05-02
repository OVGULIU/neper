/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_camera_.h"

void
nev_camera_tess_expr_coo (struct TESS Tess, char *expr, double *coo)
{
  double *v = ut_alloc_1d (3);
  double *X = ut_alloc_1d (3);

  nev_camera_v (Tess.Dim, v);

  if (strchr (expr, 'x') || strchr (expr, 'y') || strchr (expr, 'z'))
    neut_tess_bboxcentre (Tess, X);

  nev_camera_expr_coo (X, v, expr, coo);

  ut_free_1d (v);
  ut_free_1d (X);

  return;
}

void
nev_camera_mesh_expr_coo (struct NODES Nodes, struct MESH Mesh,
			  char *expr, double *coo)
{
  double *v = ut_alloc_1d (3);
  double *X = ut_alloc_1d (3);
  (void) Mesh;

  nev_camera_v (neut_nodes_dim (Nodes), v);

  if (strchr (expr, 'x') || strchr (expr, 'y') || strchr (expr, 'z'))
  {
    if (Nodes.NodeQty > 0)
      neut_nodes_bboxcentre (Nodes, X);
    else
      ut_error_reportbug ();
  }

  nev_camera_expr_coo (X, v, expr, coo);

  ut_free_1d (v);
  ut_free_1d (X);

  return;
}

void
nev_camera_tesr_expr_coo (struct TESR Tesr, char *expr, double *coo)
{
  double *v = ut_alloc_1d (3);
  double *X = ut_alloc_1d (3);

  nev_camera_v (Tesr.Dim, v);

  if (strchr (expr, 'x') || strchr (expr, 'y') || strchr (expr, 'z'))
    neut_tesr_centre (Tesr, X);

  nev_camera_expr_coo (X, v, expr, coo);

  ut_free_1d (v);
  ut_free_1d (X);

  return;
}

void
nev_camera_point_expr_coo (struct POINT Point, char *expr, double *coo)
{
  double *v = ut_alloc_1d (3);
  double *X = ut_alloc_1d (3);

  nev_camera_v (Point.Dim, v);

  if (strchr (expr, 'x') || strchr (expr, 'y') || strchr (expr, 'z'))
    neut_point_centre (Point, X);

  nev_camera_expr_coo (X, v, expr, coo);

  ut_free_1d (v);
  ut_free_1d (X);

  return;
}

void
nev_camera_expr_sky (char *expr, double *coo)
{
  char **vals = NULL;
  int i, qty;

  ut_string_separate (expr, NEUT_SEP_DEP, &vals, &qty);

  if (qty != 3)
    ut_print_message (2, 2, "Expression `%s' could not be processed.\n",
		      expr);

  for (i = 0; i < 3; i++)
    ut_string_real (vals[i], &(coo[i]));

  ut_free_2d_char (vals, qty);

  return;
}
