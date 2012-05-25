#ifndef BOAT_H
#define BOAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "obj/obj.h"
#include "cannon_ball.h"
	
#define MAX_CANNON_BALLS 50
#define BOAT_RADIUS 4
#define COLLISION_OFFSET 4
#define BALL_RADIUS 0.5
#define DAMAGE_FACTOR 10
#define MAX_DAMAGE 50

/* forward declare instead of #include "obj.h" */
struct _OBJMesh;

/* Struct to hold the position, heading and mesh of the boat */
typedef struct
{
	float radius;
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
	int nBalls;
	int currentBall;
	CannonBall *balls;
	int damage;
	
} Boat;

/* Initialises the given boat with default values, and loads the obj file given by the filename */
void initBoat(Boat *boat, const char *meshFilename, Vec3f);

/* Draws the given boat */
void drawBoat(Boat *boat, float*);

/* Updates the boat's position based on movement flags */
void updateBoat(Boat *boat, bool up, bool down, bool left, bool right, float dt, bool *fireleft, bool *fireright);
	
void drawMesh(OBJMesh *mesh);
	
	void initBall(Boat* boat, bool left);
	void drawAllBalls(Boat* boat);
	void updateAllBalls(Boat* boat);
	void ballHitBoat(CannonBall *ball, Boat *boat);
	void ballsHitBoat(Boat *boat1, Boat *boat2);
	bool boatDestroyed(Boat *boat);
	bool boatsCollided(Boat *boat1, Boat *boat2);
	
#ifdef __cplusplus
}
#endif

#endif
