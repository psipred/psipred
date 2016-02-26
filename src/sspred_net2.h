#define IPERGRP (4)

#define WINR (7)
#define WINL (-WINR)

#define NUM_IN	((WINR-WINL+1)*IPERGRP+3+1)	/* number of input units */
#define NUM_HID (25)			/* number of hidden units */
#define NUM_OUT (3) 			/* number of output units */

#define TOTAL		(NUM_IN + NUM_HID + NUM_OUT)
