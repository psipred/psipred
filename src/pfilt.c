/* pfilt - remove various non-globular/biased regions from a FASTA file */

/* V1.4 */

/* Author: David T. Jones, Bioinformatics Unit, University College London,
   March 2002 */

/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details
 */

/* Currently removes transmembrane segments, coiled-coil and low-complexity
   regions. It also performs constrained filtering of biased-composition
   regions */

/* This is needed on 32-bit Linux systems */
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define BUFLEN 1048576
#define MAXSEQLEN 131072

/* CWIN = complexity window */
#define CWIN 12

/* FWIN = frequency window - N.B. aamean/aasd values assume FWIN=100! */
#define FWIN 100

#define FALSE 0
#define TRUE 1

int nofrag = FALSE, tmfilt = TRUE, coilfilt = TRUE, biasfilt = FALSE, complexfilt = TRUE;

const char     *rescodes = "ARNDCQEGHILKMFPSTWYVBZX";

/* Scaled log-likelihood ratios for coiled-coil heptat repeat */
const float     ccoilmat[23][7] =
{
    {249, 310, 74, 797, -713, 235, -102},
    {13, 389, 571, -2171, 511, 696, 611},
    {207, 520, 768, -1624, 502, 887, 725},
    {-2688, 743, 498, -1703, -409, 458, 337},
    {-85, -6214, -954, -820, -1980, -839, -2538},
    {-1167, 828, 845, -209, 953, 767, 949},
    {-1269, 1209, 1097, -236, 1582, 1006, 1338},
    {-2476, -1537, -839, -2198, -1877, -1002, -2079},
    {-527, -436, -537, -171, -1180, -492, -926},
    {878, -1343, -1064, -71, -911, -820, -1241},
    {1097, -1313, -1002, 1348, -673, -665, -576},
    {209, 785, 597, -492, 739, 522, 706},
    {770, -502, -816, 365, -499, -783, -562},
    {-713, -2590, -939, -447, -2079, -2513, -3270},
    {-5521, -2225, -4017, -5115, -4605, -5521, -4961},
    {-1102, -283, -72, -858, -309, -221, -657},
    {-1624, -610, -435, -385, -99, -441, -213},
    {-2718, -2748, -2733, -291, -5115, -2162, -4268},
    {276, -2748, -2513, 422, -1589, -2137, -2343},
    {421, -736, -1049, -119, -1251, -1049, -1016},
    {-431, 638, 642, -1663, 147, 695, 549},
    {-1217, 1036, 979, -223, 1316, 894, 1162},
    {0, 0, 0, 0, 0, 0, 0}
};

/* Sample Means for proteins < 100 aa long */
const float     aamean[20] =
{
    7.38, 5.76, 4.32, 4.69, 2.84, 3.73, 6.08, 6.19, 2.10, 6.34,
    9.33, 7.20, 2.67, 4.11, 4.35, 6.67, 5.13, 1.25, 3.21, 6.63
};

/* Sample Standard Deviations for proteins < 100 aa long */
const float     aasd[20] =
{
    4.00, 3.89, 2.39, 2.60, 3.59, 2.49, 3.33, 3.35, 1.95, 3.50,
    4.11, 4.19, 1.69, 2.60, 2.85, 3.23, 2.49, 1.30, 2.12, 2.91
};

/* Sample Means for 100-residue windows */
const float     aamean100[20] =
{
    7.44,5.08,4.69,5.36,1.54,3.93,6.24,6.34,2.24,6.09,
    9.72, 6.00,2.39,4.30,4.69,7.23,5.61,1.25,3.31,6.53
};

/* Sample Standard Deviations for 100-residue windows */
const float     aasd100[20] =
{
    4.02,3.05,2.87,2.71,1.88,2.63,3.46,3.45,1.79,3.19,
    3.77,3.64,1.71,2.62,3.00,3.63,2.83,1.32,2.18,2.92
};

/* MEMSAT's TM-Helix-Middle Propensities */
const int       transmemsc[23] =
{
    459, -2588, -1583, -2519, -1339,
    -1396, -2419, -30, -1859,
    1146, 730, -3645, 354, 468,
    -181, -523, -367, 1069, -83,
    783, -2051, -1908, 0
};

/* Dump a rude message to standard error and exit */
void
                fail(char *errstr)
{
    fprintf(stderr, "\n*** %s\n\n", errstr);
    exit(-1);
}

/* Convert AA letter to numeric code (0-22) */
int
                aanum(int ch)
{
    static int      aacvs[] =
    {
	999, 0, 20, 4, 3, 6, 13, 7, 8, 9, 22, 11, 10, 12, 2,
	22, 14, 5, 1, 15, 16, 22, 19, 17, 22, 18, 21
    };

    return (isalpha(ch) ? aacvs[ch & 31] : 22);
}

/* Actually do the filtering on a a sequence */
void            filtseq(char *desc, char *seq, int seqlen)
{
    int             i, j, k, n, l, tot, aafreq[23];
    char            cmask[MAXSEQLEN];

    for (i = 0; i < seqlen; i++)
	cmask[i] = FALSE;

    /* Filter transmembrane segments */
    if (tmfilt)
	for (i = 0; i <= seqlen - 20; i++)
	{
	    for (tot = l = 0; l < 20; l++)
		tot += transmemsc[seq[i + l]];
	    if (tot >= 7500)
		for (l = 0; l < 20; l++)
		    cmask[i + l] = TRUE;
	}
	
    /* Filter coiled-coils */
    if (coilfilt)
	for (i = 0; i <= seqlen - 21; i++)
	{
	    for (tot = 0, l = 0; l < 21; l++)
		tot += ccoilmat[seq[i + l]][l % 7];
	    if (tot > 10000)
	    {
		for (l = 0; l < 21; l++)
		    cmask[i + l] = TRUE;
	    }
	}

    /* Filter low-complexity regions */
    if (complexfilt)
	for (i = 0; i <= seqlen - CWIN; i++)
	{
	    for (j = 0; j < 22; j++)
		aafreq[j] = 0;
	    for (j = 0; j < CWIN; j++)
		aafreq[seq[i + j]]++;
	    for (tot = n = j = 0; j < 22; j++)
		if (aafreq[j])
		{
		    tot += aafreq[j];
		    n++;
		}
	    if (n)
		tot /= n;
	    else
		tot = 0;
	    if (tot > 3)
	    {
		for (j = 0; j < CWIN; j++)
		    cmask[i + j] = TRUE;
	    }
	}

    /* Filter biased 100-residue regions */
    if (biasfilt)
	for (i = 0; i <= seqlen - FWIN; i++)
	{
	    for (j = 0; j < 22; j++)
		aafreq[j] = 0;
	    for (j = 0; j < FWIN; j++)
		aafreq[seq[i + j]]++;
	    
	    for (j = 0; j < 20; j++)
		if (100.0 * aafreq[j] / FWIN > aamean100[j] + 5.0 * aasd100[j])
		    for (k = 0; k < FWIN; k++)
			if (seq[i + k] == j)
			    cmask[i + k] = TRUE;
	}

    /* Filter frequently occurring amino acids in proteins < 100 aa long */
    if (biasfilt && seqlen < FWIN)
    {
	for (j = 0; j < 22; j++)
	    aafreq[j] = 0;
	for (i = 0; i < seqlen; i++)
	    aafreq[seq[i]]++;

	for (j = 0; j < 20; j++)
	    if (100.0 * aafreq[j] / seqlen > aamean[j] + 4.0 * aasd[j])
		for (i = 0; i < seqlen; i++)
		    if (seq[i] == j)
			cmask[i] = TRUE;
    }

    /* Now mask out the combined regions in the sequence */
    for (i = 0; i < seqlen; i++)
	if (cmask[i])
	    seq[i] = 22;

    /* Output the masked sequence */
    desc[strlen(desc) - 1] = '\0';
    printf(">%s\n", desc);
    for (i = 0; i < seqlen; i++)
    {
        putchar(rescodes[seq[i]]);
	if (i && i != seqlen-1 && i%70 == 69)
	    putchar('\n');
    }
    putchar('\n');
}

int main(int argc, char **argv)
{
    int             ch, i, j, readlen, seqlen = 0;
    char            desc[BUFLEN], seq[MAXSEQLEN], buf[BUFLEN], *p;
    FILE           *ifp;

    if (argc < 2)
	fail("Usage: pfilt [-f] [-t] [-c] [-b] [-x] fasta-file");

    for (*argv++, argc--; argc && **argv == '-'; argv++, argc--)
	switch (*(*argv + 1))
	{
	case 'f':
	    nofrag = TRUE;
	    break;
	case 't':
	    tmfilt = FALSE;
	    break;
	case 'c':
	    coilfilt = FALSE;
	    break;
	case 'b':
	    biasfilt = TRUE;
	    break;
	case 'x':
	    complexfilt = FALSE;
	    break;
	default:
	    fail("Usage: pfilt [-t] [-c] [-b] [-x] fasta-file");
	}

    if (argc < 1)
	fail("Usage: pfilt [-t] [-c] [-b] [-x] fasta-file");

    ifp = fopen(argv[0], "r");
    if (!ifp)
	fail("Unable to open sequence file!");

    while (!feof(ifp))
    {
	if (!fgets(buf, BUFLEN, ifp))
	    break;
	buf[BUFLEN-1] = '\0';
	if (buf[0] == '>')
	{
	    readlen = strlen(buf);
	    if (readlen == BUFLEN-1 && buf[BUFLEN-2] != '\n')
	    {
		fprintf(stderr, "WARNING - description line truncated - increase BUFLEN!\n");
		while ((ch = fgetc(ifp)) != EOF && ch != '\n');
	    }
	    if (seqlen && (!nofrag || (!strstr(desc, "(fragment") && !strstr(desc, "(Fragment") && !strstr(desc, "(FRAGMENT"))))
		filtseq(desc, seq, seqlen);
	    seqlen = 0;
	    strcpy(desc, buf + 1);
	}
	else
	{
	    p = buf - 1;
	    while (*++p && seqlen < MAXSEQLEN)
		if (isalpha(*p))
		    seq[seqlen++] = aanum(*p);

	    if (seqlen == MAXSEQLEN)
		fail("Sequence too long - increase MAXSEQLEN!");
	}
    }
    fclose(ifp);

    if (seqlen && (!nofrag || (!strstr(desc, "(fragment") && !strstr(desc, "(Fragment") && !strstr(desc, "(FRAGMENT"))))
	filtseq(desc, seq, seqlen);

    return 0;
}
