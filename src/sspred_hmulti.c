/* PSIPRED 4.0 - Neural Network Prediction of Secondary Structure */

/* Copyright (C) 2000 David T. Jones - Created : January 2000 */
/* Original Neural Network code Copyright (C) 1990 David T. Jones */

/* 2nd Level Prediction Module */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "ssdefs.h"
#include "sspred_net2.h"

#define MAXSEQLEN 10000

char           *wtfnm;

float           activation[TOTAL], bias[TOTAL], *weight[TOTAL];

float           profile[MAXSEQLEN][3];

char            seq[MAXSEQLEN];

int             seqlen, nprof;


void
                err(char *s)
{
    fprintf(stderr, "%s\n", s);
}

void
                fail(char *s)
{
    err(s);
    exit(1);
}

void            compute_output()
{
    int             i, j;
    float            netinp, *tp, omax, sum;

    for (i = NUM_IN; i < NUM_IN + NUM_HID; i++)
    {
	netinp = bias[i];
	tp = weight[i];
	for (j = 0; j < NUM_IN; j++)
	    netinp += activation[j] * tp[j];

	/* Trigger neuron */
	activation[i] = rectifier(netinp);
    }

    for (i = NUM_IN + NUM_HID; i < TOTAL; i++)
    {
	netinp = bias[i];
	tp = weight[i];
	for (j = NUM_IN; j < NUM_IN + NUM_HID; j++)
	    netinp += activation[j] * tp[j];

	/* Trigger neuron */
	activation[i] = logistic(netinp);
    }
}


/*
 * load weights - load all link weights from a disk file
 */
void
load_wts(char *fname)
{
    int             i, j;
    double          t, chksum = 0.0;
    FILE           *ifp;

    if (!(ifp = fopen(fname, "r")))
	fail("Cannot open weight file!\n");

    /* Load input units to hidden layer weights */
    for (i = NUM_IN; i < NUM_IN + NUM_HID; i++)
	for (j = 0; j < NUM_IN; j++)
	{
	    fscanf(ifp, "%lf", &t);
	    weight[i][j] = t;
	}

    /* Load hidden layer to output units weights */
    for (; i < TOTAL; i++)
	for (j = NUM_IN; j < NUM_IN + NUM_HID; j++)
	{
	    fscanf(ifp, "%lf", &t);
	    weight[i][j] = t;
	}

    /* Load bias weights */
    for (j = NUM_IN; j < TOTAL; j++)
    {
	fscanf(ifp, "%lf", &t);
	bias[j] = t;
    }

    fclose(ifp);
}

void
init(void)
{
    int             i;

    for (i = NUM_IN; i < TOTAL; i++)
	if (!(weight[i] = calloc(TOTAL - NUM_OUT, sizeof(float))))
	  fail("init: Out of Memory!");
}

/* Convert AA letter to numeric code (0-20) */
int
                aanum(int ch)
{
    static int      aacvs[] =
    {
	999, 0, 20, 4, 3, 6, 13, 7, 8, 9, 20, 11, 10, 12, 2,
	20, 14, 5, 1, 15, 16, 20, 19, 17, 20, 18, 20
    };

    return (isalpha(ch) ? aacvs[ch & 31] : 20);
}

/* Main prediction routine */
void
                predict(int niters, float dca, float dcb, char *outname)
{
    int             aa, a, b, nb, i, j, k, n, winpos;
    char            pred, predsst[MAXSEQLEN], lastpreds[MAXSEQLEN], *che = "CHE";
    float           score_c[MAXSEQLEN], score_h[MAXSEQLEN], score_e[MAXSEQLEN], bestsc, score, conf[MAXSEQLEN], predq3, av_c, av_h, av_e;
    FILE *ofp;

    ofp = fopen(outname, "w");
    if (!ofp)
      fail("Cannot open output file!");

    fputs("# PSIPRED VFORMAT (PSIPRED V4.0)\n\n", ofp);
    
    if (niters < 1)
      niters = 1;

    do {
	memcpy(lastpreds, predsst, seqlen);
	av_c = av_h = av_e = 0.0;
	for (winpos = 0; winpos < seqlen; winpos++)
	{
	    av_c += profile[winpos][0];
	    av_h += profile[winpos][1];
	    av_e += profile[winpos][2];
	}
	av_c /= seqlen;
	av_h /= seqlen;
	av_e /= seqlen;
	for (winpos = 0; winpos < seqlen; winpos++)
	{
	    for (j = 0; j < NUM_IN; j++)
		activation[j] = 0.0;
	    activation[(WINR - WINL + 1) * IPERGRP] = av_c;
	    activation[(WINR - WINL + 1) * IPERGRP + 1] = av_h;
	    activation[(WINR - WINL + 1) * IPERGRP + 2] = av_e;
	    activation[(WINR - WINL + 1) * IPERGRP + 3] = log((double)seqlen);
	    for (j = WINL; j <= WINR; j++)
	    {
		if (j + winpos >= 0 && j + winpos < seqlen)
		{
		    for (aa = 0; aa < 3; aa++)
			activation[(j - WINL) * IPERGRP + aa] = profile[j + winpos][aa];
		}
		else
		    activation[(j - WINL) * IPERGRP + 3] = 1.0;
	    }
	    compute_output();
	    if (activation[TOTAL - NUM_OUT] > dca * activation[TOTAL - NUM_OUT + 1] && activation[TOTAL - NUM_OUT] > dcb * activation[TOTAL - NUM_OUT + 2])
		pred = 'C';
	    else if (dca * activation[TOTAL - NUM_OUT + 1] > activation[TOTAL - NUM_OUT] && dca * activation[TOTAL - NUM_OUT + 1] > dcb * activation[TOTAL - NUM_OUT + 2])
		pred = 'H';
	    else
		pred = 'E';
	    predsst[winpos] = pred;
	    score_c[winpos] = activation[TOTAL - NUM_OUT];
	    score_h[winpos] = activation[TOTAL - NUM_OUT + 1];
	    score_e[winpos] = activation[TOTAL - NUM_OUT + 2];
	}
	
	for (winpos = 0; winpos < seqlen; winpos++)
	{
	    profile[winpos][0] = score_c[winpos];
	    profile[winpos][1] = score_h[winpos];
	    profile[winpos][2] = score_e[winpos];
	}
    } while (memcmp(predsst, lastpreds, seqlen) && --niters);
    
    for (winpos = 0; winpos < seqlen; winpos++)
	conf[winpos] = (2*MAX(MAX(score_c[winpos], score_h[winpos]), score_e[winpos])-(score_c[winpos]+score_h[winpos]+score_e[winpos])+MIN(MIN(score_c[winpos], score_h[winpos]), score_e[winpos]));
    
    /* Filter remaining singleton helix/strand assignments */
    for (winpos = 0; winpos < seqlen; winpos++)
	if (winpos && winpos < seqlen - 1 && predsst[winpos] != 'C' && predsst[winpos - 1] == predsst[winpos + 1] && conf[winpos] < 0.5*(conf[winpos-1]+conf[winpos+1]))
	    predsst[winpos] = predsst[winpos - 1];
    
    for (winpos = 0; winpos < seqlen; winpos++)
    {
	if (winpos && winpos < seqlen - 1 && predsst[winpos - 1] == 'C' && predsst[winpos] != predsst[winpos + 1])
	    predsst[winpos] = 'C';
	if (winpos && winpos < seqlen - 1 && predsst[winpos + 1] == 'C' && predsst[winpos] != predsst[winpos - 1])
	    predsst[winpos] = 'C';
    }
    
    for (winpos=0; winpos<seqlen; winpos++)
	fprintf(ofp, "%4d %c %c  %6.3f %6.3f %6.3f\n", winpos + 1, seq[winpos], predsst[winpos], score_c[winpos], score_h[winpos], score_e[winpos]);
    
    fclose(ofp);
    
    nb = seqlen / 60 + 1;
    j = 1;
    for (b = 0; b < nb; b++)
    {
	printf("\nConf: ");
	for (i = 0; i < 60; i++)
	{
	    if (b * 60 + i >= seqlen)
		break;
	    j = b * 60 + i + 1;
	    putchar(MIN((char)(10.0*conf[j-1]+'0'), '9'));
	}

	printf("\nPred: ");

	for (i = 0; i < 60; i++)
	{
	    if (b * 60 + i >= seqlen)
		break;
	    j = b * 60 + i + 1;
	    putchar(predsst[j - 1]);
	}

	printf("\n  AA: ");

	for (i = 0; i < 60; i++)
	{
	    if (b * 60 + i >= seqlen)
		break;
	    j = b * 60 + i + 1;
	    putchar(seq[j - 1]);
	}

	printf("\n      ");

	for (i = 0; i < 56; i++)
	{
	    if (b * 60 + i + 5 > seqlen)
		break;
	    j = b * 60 + i + 5;
	    if (!(j % 10))
	    {
		printf("%5d", j);
		i += 4;
	    }
	    else
		printf(" ");
	}
	putchar('\n');

	putchar('\n');
    }
}

/* Read PSI AA frequency data */
int             getss(FILE * lfil)
{
    int             i, j, naa;
    float pv[3];
    char            buf[256], *p;

    naa = 0;
    while (!feof(lfil))
    {
	if (!fgets(buf, 256, lfil))
	    break;
	seq[naa] = buf[5];
	if (sscanf(buf + 11, "%f%f%f", &pv[0], &pv[1], &pv[2]) != 3)
	    break;
	
	if (!nprof)
	{
	    profile[naa][0] = pv[0];
	    profile[naa][1] = pv[1];
	    profile[naa][2] = pv[2];
	}
	else
	{
	    profile[naa][0] += pv[0];
	    profile[naa][1] += pv[1];
	    profile[naa][2] += pv[2];
	}
	
	naa++;
    }
    
    nprof++;
    
    if (!naa)
	fail("Bad psipred pass1 file format!");
    
    return naa;
}

int main(int argc, char **argv)
{
    int             i;
    FILE           *ifp;

    /* malloc_debug(3); */
    if (argc < 7)
	fail("usage : psipass2 weight-file itercount DCA DCB outputfile ss-infile ...");

    init();
    load_wts(wtfnm = argv[1]);
    
    for (i=6; i<argc; i++)
    {
	ifp = fopen(argv[i], "r");
	if (!ifp)
	    fail("Cannot open input file!");
	seqlen = getss(ifp);
	fclose(ifp);
    }
    
    for (i=0; i<seqlen; i++)
    {
	profile[i][0] /= nprof;
	profile[i][1] /= nprof;
	profile[i][2] /= nprof;
    }
    
    puts("# PSIPRED HFORMAT (PSIPRED V4.0)");
    predict(atoi(argv[2]), (float)atof(argv[3]), (float)atof(argv[4]), argv[5]);
    
    return 0;
}
