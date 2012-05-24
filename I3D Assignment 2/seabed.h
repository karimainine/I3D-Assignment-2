#ifndef SEABED_H
#define SEABED_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "utils.h"
	
	/* The Grid struct is used to hold the grid of vertices representing
	 the waves */
	typedef struct
	{
		int rows;		/* No. of vertices per row (tessellation) */
		int cols;		/* No. of vertices per col (tessellation) */
		float size;		/* Size of the grid in GL coords (width and height are equal) */
		int nVertices;		/* Total no. of vertices */
		int nIndices;		/* Total no. of indices */
		Vec3f *vertices;	/* 1d array of vertices */
		Vec3f *normals;		/* 1d array of normal vectors, maps to
									 locations of vertices */
		int *indices;		/* 1d array of indices */
	} Terrain;
	
	/* Initialises a 2d grid of the given size, divided into the given
	 number of rows and cols */
	void initTerrain(Terrain *terrain, int rows, int cols, float size, float height_offset);
		
	/* Deletes all memory dynamically allocated by initGrid */
	void cleanupTerrain(Terrain *terrain);
	
	/* Draws a given grid */
	void drawTerrain(Terrain *terrain);
	
	void calcTerrainNormals(Terrain* terrain);
	
	Vec3f getCrossProduct(Vec3f vector1, Vec3f vector2);
	
	/* Draws normal vectors of the grid as lines, for debugging
	 purposes */
	void drawTerrainNormals(Terrain *terrain, float size);
	
#ifdef __cplusplus
}
#endif

#endif