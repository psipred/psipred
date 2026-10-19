/*
 * Network dimensions for sspred_avpred.c.
 *
 * This architecture was selected by genetic optimization and outperformed
 * the alternatives evaluated. The connectivity described below is part
 * of that result, and the trained weights depend on it, so it should be
 * preserved as-is rather than "regularized."
 *
 * First hidden stage
 * ------------------
 * The first hidden stage is a locally connected layer of NUM_CONV ReLU
 * units. Each unit has its own weights (no weight sharing) and reads a
 * contiguous span of CWIDTH * IPERGRP activations starting at
 *
 *     start(k) = (k * IPERGRP) / CDEPTH        (integer division)
 *
 * where k = i - NUM_IN is the unit's index within the stage. Weight
 * loading and evaluation both use start(k).
 *
 * Start offsets advance by IPERGRP / CDEPTH (< 1) activations per unit,
 * so receptive fields are placed at single-feature resolution rather
 * than snapped to residue boundaries. The highest-numbered units
 * (k = 1172 through 1214) have spans that extend past the input block
 * into the leading units of this same stage. For example, the final
 * unit reads slots 1106 through 1474, which covers input features
 * 1106-1434 and first-stage units 0-39 (slots 1435-1474). Units are
 * evaluated in index order, so every slot is computed before it is
 * read. The network is therefore a feedforward DAG with intra-stage
 * connections rather than a strictly layered network.
 */

#define IPERGRP (41)   /* Per residue: 20 profile + 20 identity + 1 unknown/padding. */

#define WINR (17)      /* Right extent of the 35-residue input window. */
#define WINL (-WINR)   /* Left extent of the input window. */

#define CWIDTH (9)     /* Sets each first-stage unit's fan-in to 9 * 41 = 369. */
#define CDEPTH (45)    /* Controls first-stage unit count and feature-offset spacing. */

#define NUM_IN ((WINR-WINL+1)*IPERGRP)  /* 1435 input features. */
#define NUM_CONV (CDEPTH*((WINR-WINL+1)-CWIDTH+1)) /* 1215 first-stage ReLU units. */
#define NUM_HID (40)   /* Dense ReLU units, each connected to all NUM_CONV units. */
#define NUM_OUT (3)    /* Independent sigmoid outputs in C, H, E order. */

#define TOTAL (NUM_IN + NUM_CONV + NUM_HID + NUM_OUT) /* 2693 activation slots. */
