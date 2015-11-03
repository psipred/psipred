/* chkparse - generate PSIPRED compatible mtx file from BLAST+ checkpoint file */

/* V0.3 */

/* Copyright (C) 2010 D.T. Jones */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXSEQLEN 65536

#define EPSILON 1e-6

#define FALSE 0
#define TRUE 1

#define SQR(x) ((x)*(x))
#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))

const char *ncbicodes = "*A*CDEFGHIKLMNPQRSTVWXY*****";

/*  BLOSUM 62 */
const short           aamat[23][23] =
{
    {4, -1, -2, -2, 0, -1, -1, 0, -2, -1, -1, -1, -1, -2, -1, 1, 0, -3, -2, 0, -2, -1, 0},
    {-1, 5, 0, -2, -3, 1, 0, -2, 0, -3, -2, 2, -1, -3, -2, -1, -1, -3, -2, -3, -1, 0, -1},
    {-2, 0, 6, 1, -3, 0, 0, 0, 1, -3, -3, 0, -2, -3, -2, 1, 0, -4, -2, -3, 3, 0, -1},
    {-2, -2, 1, 6, -3, 0, 2, -1, -1, -3, -4, -1, -3, -3, -1, 0, -1, -4, -3, -3, 4, 1, -1},
    {0, -3, -3, -3,10, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1, -3, -3, -2},
    {-1, 1, 0, 0, -3, 5, 2, -2, 0, -3, -2, 1, 0, -3, -1, 0, -1, -2, -1, -2, 0, 3, -1},
    {-1, 0, 0, 2, -4, 2, 5, -2, 0, -3, -3, 1, -2, -3, -1, 0, -1, -3, -2, -2, 1, 4, -1},
    {0, -2, 0, -1, -3, -2, -2, 6, -2, -4, -4, -2, -3, -3, -2, 0, -2, -2, -3, -3, -1, -2, -1},
    {-2, 0, 1, -1, -3, 0, 0, -2, 8, -3, -3, -1, -2, -1, -2, -1, -2, -2, 2, -3, 0, 0, -1},
    {-1, -3, -3, -3, -1, -3, -3, -4, -3, 4, 2, -3, 1, 0, -3, -2, -1, -3, -1, 3, -3, -3, -1},
    {-1, -2, -3, -4, -1, -2, -3, -4, -3, 2, 4, -2, 2, 0, -3, -2, -1, -2, -1, 1, -4, -3, -1},
    {-1, 2, 0, -1, -3, 1, 1, -2, -1, -3, -2, 5, -1, -3, -1, 0, -1, -3, -2, -2, 0, 1, -1},
    {-1, -1, -2, -3, -1, 0, -2, -3, -2, 1, 2, -1, 5, 0, -2, -1, -1, -1, -1, 1, -3, -1, -1},
    {-2, -3, -3, -3, -2, -3, -3, -3, -1, 0, 0, -3, 0, 6, -4, -2, -2, 1, 3, -1, -3, -3, -1},
    {-1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4, 7, -1, -1, -4, -3, -2, -2, -1, -2},
    {1, -1, 1, 0, -1, 0, 0, 0, -1, -2, -2, 0, -1, -2, -1, 4, 1, -3, -2, -2, 0, 0, 0},
    {0, -1, 0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1, 1, 5, -2, -2, 0, -1, -1, 0},
    {-3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1, 1, -4, -3, -2, 11, 2, -3, -4, -3, -2},
    {-2, -2, -2, -3, -2, -1, -2, -3, 2, -1, -1, -2, -1, 3, -3, -2, -2, 2, 7, -1, -3, -2, -1},
    {0, -3, -3, -3, -1, -2, -2, -3, -3, 3, 1, -2, 1, -1, -2, -2, 0, -3, -1, 4, -3, -2, -1},
    {-2, -1, 3, 4, -3, 0, 1, -1, 0, -3, -4, 0, -3, -3, -2, 0, -1, -4, -3, -3, 4, 1, -1},
    {-1, 0, 0, 1, -3, 3, 4, -2, 0, -3, -3, 1, -1, -3, -1, 0, -1, -3, -2, -2, 1, 4, -1},
    {0, -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, 0, 0, -2, -1, -1, -1, -1, 4}
};


/* Standard BLAST+ a.a. frequencies */
float aafreq[26] =
{
    0.00000, 0.07805, 0.00000, 0.01925, 0.05364, 0.06295, 0.03856, 0.07377, 0.02199, 0.05142, 0.05744, 0.09019,
    0.02243, 0.04487, 0.05203, 0.04264,	0.05129, 0.07120, 0.05841, 0.06441, 0.01330, 0.00000, 0.03216, 0.00000,
    0.00000, 0.00000
};

/* PSSM arrays */
float fratio[MAXSEQLEN][28], pssm[MAXSEQLEN][28];


/* Dump a rude message to standard error and exit */
void
  fail(char *errstr)
{
    fprintf(stderr, "\n*** %s\n\n", errstr);
    exit(-1);
}

/* Convert AA letter to numeric code (0-22 in 3-letter code order) */
int aanum(int ch)
{
    static const int aacvs[] =
    {
	999, 0, 20, 4, 3, 6, 13, 7, 8, 9, 22, 11, 10, 12, 2,
	22, 14, 5, 1, 15, 16, 22, 19, 17, 22, 18, 21
    };

    return (isalpha(ch) ? aacvs[ch & 31] : 22);
}

/* Scan ahead for file tokens */
void findtoken(char *buf, char *token, FILE *ifp)
{
    for (;;)
    {
	if (fscanf(ifp, "%s", buf) != 1)
	    fail("Cannot find token in checkpoint file!");
	if (!token[0] || !strcmp(buf, token))
	    break;
    }
}

/* Read hex sequence string */
int readhex(char *seq, FILE *ifp)
{
    int ch, aa, nres=0;
    
    while ((ch = fgetc(ifp)) != EOF)
	if (ch == '\'')
	    break;
    if (ch == EOF)
	fail("Bad sequence record in checkpoint file!");
	
    for (;;)
    {
	ch = fgetc(ifp);
	if (ch == '\'')
	    break;
	if (isspace(ch))
	    continue;
	if (!isxdigit(ch))
	    fail("Bad sequence record in checkpoint file!");
	if (ch >= 'A')
	    aa = 16 * (10 + ch - 'A');
	else
	    aa = 16 * (ch - '0');
	ch = fgetc(ifp);
	if (!isxdigit(ch))
	    fail("Bad sequence record in checkpoint file!");
	if (ch >= 'A')
	    aa += 10 + ch - 'A';
	else
	    aa += ch - '0';

	if (nres > MAXSEQLEN)
	    break;
	
	seq[nres++] = aa;
    }

    return nres;
}

/* This routine will extract PSSM data from a BLAST+ checkpoint file */
int getpssm(char *dseq, FILE *ifp)
{
    int i, j, len;
    float pssmrow[28], val, base, power;
    char buf[4096];
    
    findtoken(buf, "", ifp);
    if (strcmp(buf, "PssmWithParameters"))
	fail("Unknown checkpoint file format!");
    findtoken(buf, "numColumns", ifp);
    if (fscanf(ifp, "%d", &len) != 1)
	fail("Unknown checkpoint file format!");
    findtoken(buf, "ncbistdaa", ifp);
    if (len != readhex(dseq, ifp))
	fail("Mismatching sequence length in checkpoint file!");
    findtoken(buf, "freqRatios", ifp);
    findtoken(buf, "", ifp);

    for (i=0; i<len; i++)
	for (j=0; j<28; j++)
	{
	    findtoken(buf, "", ifp);
	    findtoken(buf, "", ifp);
	    if (sscanf(buf, "%f", &val) != 1)
		fail("Unknown checkpoint file format!");
	    findtoken(buf, "", ifp);
	    if (sscanf(buf, "%f", &base) != 1)
		fail("Unknown checkpoint file format!");
	    findtoken(buf, "", ifp);
	    if (sscanf(buf, "%f", &power) != 1)
		fail("Unknown checkpoint file format!");
	    findtoken(buf, "", ifp);

	    fratio[i][j] = val * pow(base, power);
	}

    findtoken(buf, "scores", ifp);
    findtoken(buf, "", ifp);
    for (i=0; i<len; i++)
	for (j=0; j<28; j++)
	{
	    findtoken(buf, "", ifp);
	    if (sscanf(buf, "%f", &val) != 1)
		fail("Unknown checkpoint file format!");
	    pssm[i][j] = val;
	}

    return len;
}


int roundint(double x)
{
    x += (x >= 0.0 ? 0.5 : -0.5);

    return (int)x;
}


int main(int argc, char **argv)
{
    int i, j, seqlen=0, nf;
    char seq[MAXSEQLEN];
    double scale, x, y, sxx, sxy;
    FILE *ifp;

    if (argc != 2)
	fail("Usage: chkparse chk-file");

    ifp = fopen(argv[1], "r");
    if (!ifp)
	fail("Unable to open checkpoint file!");

    seqlen = getpssm(seq, ifp);

    if (seqlen < 5 || seqlen >= MAXSEQLEN)
	fail("Sequence length error!");

    printf("%d\n", seqlen);

    for (i=0; i<seqlen; i++)
      putchar(ncbicodes[seq[i]]);

    printf("\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n");

    /* Estimate original scaling factor by weighted least squares regression */
    for (sxx=sxy=i=0; i<seqlen; i++)
	for (j=0; j<26; j++)
	    if (fratio[i][j] > EPSILON && aafreq[j] > EPSILON)
	    {
		x = log(fratio[i][j] / aafreq[j]);
		y = pssm[i][j];
		sxx += (y*y) * x * x; /* Weight by y^2 */
		sxy += (y*y) * x * y;
	    }

    scale = 100.0 * sxy / sxx;

    for (i=0; i<seqlen; i++)
    {
	for (j=0; j<28; j++)
	    if (ncbicodes[j] != '*')
	    {
		if (fratio[i][j] > EPSILON)
		    printf("%d  ", roundint(scale * log(fratio[i][j] / aafreq[j])));
		else
		    printf("%d  ", 100*aamat[aanum(ncbicodes[seq[i]])][aanum(ncbicodes[j])]);
	    }
	    else
		printf("-32768  ");
	putchar('\n');
    }
    
    return 0;
}
