#define IPERGRP (41)

#define WINR (17)
#define WINL (-WINR)

#define CWIDTH (9)                                      /* 1st Hidden Layer width (window size) */
#define CDEPTH (45)                                     /* 1st Hidden Layer depth (number of hidden units per window position) */

#define NUM_IN	((WINR-WINL+1)*IPERGRP)	                /* number of input units */
#define NUM_CONV (CDEPTH*((WINR-WINL+1)-CWIDTH+1))	/* number of 1st Hidden Layer units */
#define NUM_HID (40)	                                /* number of 2nd Hidden layer units */
#define NUM_OUT (3) 			                /* number of output units */

#define TOTAL		(NUM_IN + NUM_CONV + NUM_HID + NUM_OUT)
