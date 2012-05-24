#ifndef BOAT_H
#define BOAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "obj/obj.h"

/* forward declare instead of #include "obj.h" */
struct _OBJMesh;

/* Struct to hold the position, heading and mesh of the boat */
typedef struct
{
	Vec3f pos;			/* Position of the boat */
	float heading;		/* The boat's heading, in degrees */
	struct _OBJMesh *mesh;		/* The boat's mesh */

	float speed;		/* Forward speed of the boat */
	float maxSpeed;		/* Maximum forward speed of the boat */
	float acceleration;	/* Forward acceleration of the boat (constant) */
	float turnVelocity;	/* Turning speed of the boat, in degrees per second */
	float turningAcceleration; /* Turning acceleration (constant) */

	float displacement; /* How deep in the water the boat will sit */

	float roll;			/* How much the boat has rotated from side to side, in degrees */
	float pitch;        /* How much the boat has rotated up and down, in degrees */
} Boat;

/* Initialises the given boat with default values, and loads the obj file given by the filename */
void initBoat(Boat *boat, const char *meshFilename, Vec3f);

/* Draws the given boat */
void drawBoat(Boat *boat, float*);

/* Updates the boat's position based on movement flags */
void updateBoat(Boat *boat, bool up, bool down, bool left, bool right, float dt);
	
void drawMesh(OBJMesh *mesh);

#ifdef __cplusplus
}
#endif

#endif
