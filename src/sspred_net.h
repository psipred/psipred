#define IPERGRP (41)

#define WINL (-7)
#define WINR (7)

#define NUM_IN	((WINR-WINL+1)*IPERGRP)	/* number of input units */
#define NUM_HID (75)			/* number of hidden units */
#define NUM_OUT (3) 			/* number of output units */

#define TOTAL		(NUM_IN + NUM_HID + NUM_OUT)
