/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_merge_.h"

void
neut_tess_merge_polyfaces (struct TESS *pTess, int poly)
{
  int i, k, npolyqty, *npolys = NULL;
  int comfaceqty, *comfaces = NULL;
  int setqty = 0, *setfaceqty = NULL, **setfaces = NULL;

  neut_tess_poly_neighpoly (*pTess, poly, &npolys, &npolyqty);

  for (i = 0; i < npolyqty; i++)
  {
    neut_tess_polypair_commonfaces (*pTess, poly, npolys[i], &comfaces, &comfaceqty);

    neut_tess_faces_contiguousfaces (*pTess, 1, comfaces, comfaceqty, &setqty,
        &setfaces, &setfaceqty);

    for (k = 0; k < setqty; k++)
      neut_tess_faces_merge (pTess, setfaces[k], setfaceqty[k]);

    ut_free_1d_int_ (&setfaceqty);
    ut_free_2d_int_ (&setfaces, setqty);
    setqty = 0;
  }

  ut_free_1d_int (npolys);
  ut_free_1d_int (comfaces);

  return;
}

void
neut_tess_merge_polyedges (struct TESS *pTess, int poly)
{
  int i;
  int ver, verqty, *vers = NULL;
  double angle;

  neut_tess_poly_vers (*pTess, poly, &vers, &verqty);

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];

    if ((*pTess).VerEdgeQty[ver] == 2)
    {
      neut_tess_edgepair_angle (*pTess, (*pTess).VerEdgeNb[ver][0],
                                (*pTess).VerEdgeNb[ver][1], &angle);

      if (ut_num_equal (angle, 0, 1e-6))
        neut_tess_edges_merge (pTess, (*pTess).VerEdgeNb[ver], 2);
    }
  }

  ut_free_1d_int (vers);

  return;
}

void
neut_tess_merge_ondomain (struct TESS *pTess)
{
  int i, j, k, edgeqty, *edges = NULL;
  int face, faceqty, *faces = NULL;
  int poly, polyqty, *polys = NULL;
  int setqty = 0, *setfaceqty = NULL, **setfaces = NULL;
  int *setedgeqty = NULL, **setedges = NULL;

  if ((*pTess).Dim == 3)
    for (i = 1; i <= (*pTess).DomFaceQty; i++)
    {
      neut_tess_domface_polys (*pTess, i, &polys, &polyqty);

      for (j = 0; j < polyqty; j++)
      {
	poly = polys[j];
	neut_tess_poly_domface_faces (*pTess, poly, i, &faces, &faceqty);
	neut_tess_faces_contiguousfaces (*pTess, 1, faces, faceqty, &setqty,
	    &setfaces, &setfaceqty);

	for (k = 0; k < setqty; k++)
	  neut_tess_faces_merge (pTess, setfaces[k], setfaceqty[k]);

	setqty = 0;
	ut_free_1d_int_ (&setfaceqty);
	ut_free_2d_int_ (&setfaces, setqty);
      }
    }

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    neut_tess_domedge_faces (*pTess, i, &faces, &faceqty);

    for (j = 0; j < faceqty; j++)
    {
      face = faces[j];
      neut_tess_face_domedge_edges (*pTess, face, i, &edges, &edgeqty);
      neut_tess_edges_contiguousedges (*pTess, 0, edges, edgeqty, &setqty,
	  &setedges, &setedgeqty);

      for (k = 0; k < setqty; k++)
	neut_tess_edges_merge (pTess, setedges[k], setedgeqty[k]);

      setqty = 0;
      ut_free_1d_int_ (&setedgeqty);
      ut_free_2d_int_ (&setedges, setqty);
    }
  }

  ut_free_1d_int (edges);
  ut_free_1d_int (faces);
  ut_free_1d_int (polys);

  return;
}

void
neut_tess_merge_finalize (struct TESS *pTess)
{
  neut_tess_compress (pTess);
  neut_tess_init_domtessface (pTess);
  neut_tess_init_domtessedge (pTess);
  neut_tess_init_domtessver (pTess);

  return;
}
