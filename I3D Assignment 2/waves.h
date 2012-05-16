#ifndef RENDERING_H
#define RENDERING_H

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
} Grid;

/* Struct to model a sine function, waves are modelled as a sum of one
   or more sine waves */
typedef struct
{
	float A;		/* Amplitude of the wave */
	float k;		/* Wave period (in 2pi) */
	float w;		/* Angular frequency along the x axis */
} SineFunction;

/* Initialises a 2d grid of the given size, divided into the given
   number of rows and cols */
void initGrid(Grid *grid, int rows, int cols, float size);

/* Deletes all memory dynamically allocated by initGrid */
void cleanupGrid(Grid *grid);

/* Draws a given grid */
void drawGrid(Grid *grid);

/* Returns normal x,y,z and height w in a Vec4
   at the point on the wave given by x, z */
Vec4f calcSineValue(float x, float z);

/* Updates the given grid to apply a wave effect based on a sine wave,
   animates using dt */
void updateGrid(Grid *grid, float dt);

/* Draws normal vectors of the grid as lines, for debugging
   purposes */
void drawNormals(Grid *grid, float size);

#ifdef __cplusplus
}
#endif

#endif
