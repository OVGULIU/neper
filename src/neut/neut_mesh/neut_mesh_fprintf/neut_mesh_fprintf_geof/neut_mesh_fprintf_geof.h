/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FPRINTF_GEOF

  extern void neut_mesh_fprintf_geof (FILE * file, struct NODES Nodes,
				      struct MESH Mesh0D, struct MESH Mesh1D,
				      struct MESH Mesh2D, struct MESH Mesh3D,
				      struct NSET NSet0D, struct NSET NSet1D,
				      struct NSET NSet2D, char *nsets,
				      char *fasets, char *dim,
				      struct PART Part,
				      struct BOUNDARY Bound);

#endif				// NEUT_MESH_FPRINTF_GEOF

#ifdef __cplusplus
}
#endif