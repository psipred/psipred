/* PSIPRED 3.5 - Neural Network Prediction of Secondary Structure */

/* Copyright (C) 2000 David T. Jones - Created : January 2000 */
/* Original Neural Network code Copyright (C) 1990 David T. Jones */

/* Average Prediction Module */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "ssdefs.h"
#include "sspred_net.h"

void           *calloc(), *malloc();

char           *wtfnm;

int             nwtsum, fwt_to[TOTAL], lwt_to[TOTAL];
float           activation[TOTAL], bias[TOTAL], *weight[TOTAL];

int             profile[MAXSEQLEN][20];

int             seqlen;

char seq[MAXSEQLEN];

enum aacodes
{
    ALA, ARG, ASN, ASP, CYS,
    GLN, GLU, GLY, HIS, ILE,
    LEU, LYS, MET, PHE, PRO,
    SER, THR, TRP, TYR, VAL,
    UNK
};

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

void
compute_output(void)
{
    int             i, j;
    float           netinp;

    for (i = NUM_IN; i < TOTAL; i++)
    {
	netinp = bias[i];

	for (j = fwt_to[i]; j < lwt_to[i]; j++)
	    netinp += activation[j] * weight[i][j];

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
	for (j = fwt_to[i]; j < lwt_to[i]; j++)
	{
	    fscanf(ifp, "%lf", &t);
	    weight[i][j] = t;
	    chksum += t*t;
	}

    /* Load hidden layer to output units weights */
    for (i = NUM_IN + NUM_HID; i < TOTAL; i++)
	for (j = fwt_to[i]; j < lwt_to[i]; j++)
	{
	    fscanf(ifp, "%lf", &t);
	    weight[i][j] = t;
	    chksum += t*t;
	}

    /* Load bias weights */
    for (j = NUM_IN; j < TOTAL; j++)
    {
	fscanf(ifp, "%lf", &t);
	bias[j] = t;
	chksum += t*t;
    }

    /* Read expected checksum at end of file */
    if (fscanf(ifp, "%lf", &t) != 1 || ferror(ifp))
	fail("Weight file read error!");

    fclose(ifp);

    if ((int)t != (int)(chksum+0.5))
	fail("Corrupted weight file detected!");
}

/* Initialize network */
void
init(void)
{
    int             i, j;

    for (i = NUM_IN; i < TOTAL; i++)
	if (!(weight[i] = calloc(TOTAL - NUM_OUT, sizeof(float))))
	  fail("init: Out of Memory!");

    /* Connect input units to hidden layer */
    for (i = NUM_IN; i < NUM_IN + NUM_HID; i++)
    {
	fwt_to[i] = 0;
	lwt_to[i] = NUM_IN;
    }

    /* Connect hidden units to output layer */
    for (i = NUM_IN + NUM_HID; i < TOTAL; i++)
    {
	fwt_to[i] = NUM_IN;
	lwt_to[i] = NUM_IN + NUM_HID;
    }
}

/* Convert AA letter to numeric code (0-20) */
int
aanum(ch)
    int             ch;
{
    static const int      aacvs[] =
    {
	999, 0, 20, 4, 3, 6, 13, 7, 8, 9, 20, 11, 10, 12, 2,
	20, 14, 5, 1, 15, 16, 20, 19, 17, 20, 18, 20
    };

    return (isalpha(ch) ? aacvs[ch & 31] : 20);
}

/* Make 1st level prediction averaged over specified weight sets */
void
predict(int argc, char **argv)
{
    int             aa, i, j, k, n, winpos,ws;
    char fname[80], predsst[MAXSEQLEN];
    float           avout[MAXSEQLEN][3], conf, confsum[MAXSEQLEN];

    for (winpos = 0; winpos < seqlen; winpos++)
	avout[winpos][0] = avout[winpos][1] = avout[winpos][2] = confsum[winpos] = 0.0F;

    for (ws=2; ws<argc; ws++)
    {
	load_wts(argv[ws]);
	
	for (winpos = 0; winpos < seqlen; winpos++)
	{
	    for (j = 0; j < NUM_IN; j++)
		activation[j] = 0.0;
	    for (j = WINL; j <= WINR; j++)
	    {
		if (j + winpos >= 0 && j + winpos < seqlen)
		{
		    for (aa=0; aa<20; aa++)
			activation[(j - WINL) * IPERGRP + aa] = profile[j+winpos][aa]/1000.0;
		    aa = aanum(seq[j+winpos]);
		    if (aa < 20)
			activation[(j - WINL) * IPERGRP + 20 + aa] = 1.0;
		    else
			activation[(j - WINL) * IPERGRP + 40] = 1.0;
		}
		else
		    activation[(j - WINL) * IPERGRP + 40] = 1.0;
	    }
	    
	    compute_output();
	    
	    conf = 2.0 * MAX(MAX(activation[TOTAL - NUM_OUT], activation[TOTAL - NUM_OUT+1]), activation[TOTAL - NUM_OUT+2]) + MIN(MIN(activation[TOTAL - NUM_OUT], activation[TOTAL - NUM_OUT+1]), activation[TOTAL - NUM_OUT+2]) - activation[TOTAL - NUM_OUT] - activation[TOTAL - NUM_OUT+1] - activation[TOTAL - NUM_OUT+2];
	    
	    avout[winpos][0] += conf * activation[TOTAL - NUM_OUT];
	    avout[winpos][1] += conf * activation[TOTAL - NUM_OUT+1];
	    avout[winpos][2] += conf * activation[TOTAL - NUM_OUT+2];
	    confsum[winpos] += conf;
	  }
      }
    
    for (winpos = 0; winpos < seqlen; winpos++)
    {
	avout[winpos][0] /= confsum[winpos];
	avout[winpos][1] /= confsum[winpos];
	avout[winpos][2] /= confsum[winpos];
	if (avout[winpos][0] >= MAX(avout[winpos][1], avout[winpos][2]))
	    predsst[winpos] = 'C';
	else if (avout[winpos][2] >= MAX(avout[winpos][0], avout[winpos][1]))
	    predsst[winpos] = 'E';
	else
	    predsst[winpos] = 'H';
    }
    
    for (winpos = 0; winpos < seqlen; winpos++)
	printf("%4d %c %c  %6.3f %6.3f %6.3f\n", winpos + 1, seq[winpos], predsst[winpos], avout[winpos][0], avout[winpos][1], avout[winpos][2]);
}

/* Read PSI AA frequency data */
int             getmtx(FILE *lfil)
{
    int             aa, i, j, naa;
    char            buf[256], *p;
    
    if (fscanf(lfil, "%d", &naa) != 1)
	fail("Bad mtx file - no sequence length!");
    
    if (naa > MAXSEQLEN)
	fail("Input sequence too long!");
    
    if (fscanf(lfil, "%s", seq) != 1)
	fail("Bad mtx file - no sequence!");
    
    while (!feof(lfil))
    {
	if (!fgets(buf, 65536, lfil))
	    fail("Bad mtx file!");
	if (!strncmp(buf, "-32768 ", 7))
	{
	    for (j=0; j<naa; j++)
	    {
		if (sscanf(buf, "%*d%d%*d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%*d%d", &profile[j][ALA],  &profile[j][CYS], &profile[j][ASP],  &profile[j][GLU],  &profile[j][PHE],  &profile[j][GLY],  &profile[j][HIS],  &profile[j][ILE],  &profile[j][LYS],  &profile[j][LEU],  &profile[j][MET],  &profile[j][ASN],  &profile[j][PRO],  &profile[j][GLN],  &profile[j][ARG],  &profile[j][SER],  &profile[j][THR],  &profile[j][VAL],  &profile[j][TRP],  &profile[j][TYR]) != 20)
		    fail("Bad mtx format!");
		aa = aanum(seq[j]);
		if (aa < 20)
		    profile[j][aa] += 0000;
		if (!fgets(buf, 65536, lfil))
		    break;
	    }
	}
    }
    
    return naa;
}

main(int argc, char **argv)
{
    int             i, niters;
    FILE *ifp;
    
    /* malloc_debug(3); */
    if (argc < 2)
	fail("usage : psipred mtx-file weight-file1 ... weight-filen");
    ifp = fopen(argv[1], "r");
    if (!ifp)
	exit(1);
    seqlen = getmtx(ifp);
    fclose(ifp);
    
    init();
    
    predict(argc,argv);
    
    return 0;
}
