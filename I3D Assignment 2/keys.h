#ifndef KEYS_H
#define KEYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

/* Struct to store the state of movement keys for the boat */
typedef struct
{
	/* Flags to determine if keys are pressed */
	bool up;
	bool down;
	bool left;
	bool right;
} Keys;


/* Initialises key states to false */
void initKeys(Keys *keys);


#ifdef __cplusplus
}
#endif

#endif
