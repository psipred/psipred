/* seq2mtx - convert single sequence to pseudo IMPALA mtx file */

/* Copyright (C) 2000 D.T. Jones */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXSEQLEN 65536

#define FALSE 0
#define TRUE 1

#define SQR(x) ((x)*(x))
#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))

const char *rescodes = "ARNDCQEGHILKMFPSTWYVBZX";

/*  BLOSUM 62 */
const short           aamat[23][23] =
{
    {4, -1, -2, -2, 0, -1, -1, 0, -2, -1, -1, -1, -1, -2, -1, 1, 0, -3, -2, 0, -2, -1, 0},
    {-1, 5, 0, -2, -3, 1, 0, -2, 0, -3, -2, 2, -1, -3, -2, -1, -1, -3, -2, -3, -1, 0, -1},
    {-2, 0, 6, 1, -3, 0, 0, 0, 1, -3, -3, 0, -2, -3, -2, 1, 0, -4, -2, -3, 3, 0, -1},
    {-2, -2, 1, 6, -3, 0, 2, -1, -1, -3, -4, -1, -3, -3, -1, 0, -1, -4,
     -3, -3, 4, 1, -1},
    {0, -3, -3, -3,10, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2,
     -2, -1, -3, -3, -2},
    {-1, 1, 0, 0, -3, 5, 2, -2, 0, -3, -2, 1, 0, -3, -1, 0, -1, -2,
     -1, -2, 0, 3, -1},
    {-1, 0, 0, 2, -4, 2, 5, -2, 0, -3, -3, 1, -2, -3, -1, 0, -1, -3,
     -2, -2, 1, 4, -1},
    {0, -2, 0, -1, -3, -2, -2, 6, -2, -4, -4, -2, -3, -3, -2, 0, -2, -2,
     -3, -3, -1, -2, -1},
    {-2, 0, 1, -1, -3, 0, 0, -2, 8, -3, -3, -1, -2, -1, -2, -1, -2, -2,
     2, -3, 0, 0, -1},
    {-1, -3, -3, -3, -1, -3, -3, -4, -3, 4, 2, -3, 1, 0, -3, -2, -1, -3,
     -1, 3, -3, -3, -1},
    {-1, -2, -3, -4, -1, -2, -3, -4, -3, 2, 4, -2, 2, 0, -3, -2, -1, -2,
     -1, 1, -4, -3, -1},
    {-1, 2, 0, -1, -3, 1, 1, -2, -1, -3, -2, 5, -1, -3, -1, 0, -1, -3,
     -2, -2, 0, 1, -1},
    {-1, -1, -2, -3, -1, 0, -2, -3, -2, 1, 2, -1, 5, 0, -2, -1, -1, -1,
     -1, 1, -3, -1, -1},
    {-2, -3, -3, -3, -2, -3, -3, -3, -1, 0, 0, -3, 0, 6, -4, -2, -2, 1,
     3, -1, -3, -3, -1},
    {-1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4, 7, -1, -1, -4,
     -3, -2, -2, -1, -2},
    {1, -1, 1, 0, -1, 0, 0, 0, -1, -2, -2, 0, -1, -2, -1, 4, 1, -3,
     -2, -2, 0, 0, 0},
    {0, -1, 0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1, 1, 5, -2,
     -2, 0, -1, -1, 0},
    {-3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1, 1, -4, -3, -2, 11,
     2, -3, -4, -3, -2},
    {-2, -2, -2, -3, -2, -1, -2, -3, 2, -1, -1, -2, -1, 3, -3, -2, -2, 2,
     7, -1, -3, -2, -1},
    {0, -3, -3, -3, -1, -2, -2, -3, -3, 3, 1, -2, 1, -1, -2, -2, 0, -3,
     -1, 4, -3, -2, -1},
    {-2, -1, 3, 4, -3, 0, 1, -1, 0, -3, -4, 0, -3, -3, -2, 0, -1, -4,
     -3, -3, 4, 1, -1},
    {-1, 0, 0, 1, -3, 3, 4, -2, 0, -3, -3, 1, -1, -3, -1, 0, -1, -3,
     -2, -2, 1, 4, -1},
    {0, -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, 0, 0, -2,
     -1, -1, -1, -1, 4}
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
    static int aacvs[] =
    {
	999, 0, 20, 4, 3, 6, 13, 7, 8, 9, 22, 11, 10, 12, 2,
	22, 14, 5, 1, 15, 16, 22, 19, 17, 22, 18, 21
    };

    return (isalpha(ch) ? aacvs[ch & 31] : 22);
}

/* This routine will read in one sequence from a database file. The
   sequence can be in any of the supported formats. Returns length
   of sequence.
*/
int
  getseq(char *dbname, char *dseq, FILE * lfil)
{
    int i, j, len;
    short badln, fformat;
    enum
    {
	UNKNOWN, EMBL, FASTA, OWL, GCG
    };
    char buf[MAXSEQLEN], split;
    int offset;

    offset = j = 0;

    if (!fgets(buf, MAXSEQLEN, lfil))
	return (-1);
    if (strstr(buf, "of:") != NULL && strstr(buf, "check:") != NULL)
	fformat = GCG;
    else if (strncmp(buf, "ID   ", 5) == 0)
	fformat = EMBL;
    else if (buf[0] == '>' && (buf[1] == '>' || buf[3] == ';'))
	fformat = OWL;
    else if (buf[0] == '>')
	fformat = FASTA;
    else
    {
	fprintf(stderr, "WARNING: Attempting to interpret input file with unknown format");
	fformat = UNKNOWN;
    }

    switch (fformat)
    { 
    case GCG:
	sscanf(strstr(buf, "of:")+3, "%s", dbname);
	while (strstr(buf, "..") == NULL)
	    fgets(buf, MAXSEQLEN, lfil);
	fgets(buf, MAXSEQLEN, lfil);
	break;
	
    case EMBL:
	strncpy(dbname, buf + 5, 70);
	while (buf[0] != ' ')
	    fgets(buf, MAXSEQLEN, lfil);
	break;
	
    case OWL:
	fgets(buf, MAXSEQLEN, lfil);
	strncpy(dbname, buf, 70);
	fgets(buf, MAXSEQLEN, lfil);
	break;
	
    case FASTA:
	strncpy(dbname, buf + 1, 70);
	fgets(buf, MAXSEQLEN, lfil);
	break;
	
    default:
	/* Try to find a line which looks like a protein sequence */
	do
	{
	    badln = (strpbrk(buf, "JjOoUu<>#$%&@") != NULL);
	    if (badln && !fgets(buf, MAXSEQLEN, lfil))
		return (-1);
	}
	while (badln);
	strcpy(dbname, "<NO NAME>");
	break;
    }

    if (dbname[(len = strlen(dbname)) - 1] == '\n')
	dbname[--len] = '\0';
    if (len >= 70)
	dbname[70] = '\0';

    for (;;)
    {
	if (!strncmp(buf, "//", 2))
	    break;
	len = strlen(buf);
	for (i = offset; i < len && j < MAXSEQLEN; i++)
	{
	    split = islower(buf[i]) ? toupper(buf[i]) : buf[i];
	    if (split == '@' || (fformat == OWL && split == '*'))
	    {
		dseq[j] = '\0';
		while (fgets(buf, MAXSEQLEN, lfil));
		return (j);
	    }
	    if (isalpha(split))
		dseq[j++] = split;
	    else if (buf[i] == '\n')
		break;
	}
	if (!fgets(buf, MAXSEQLEN, lfil))
	    break;
    }

    if (j == MAXSEQLEN)
	printf("\nWARNING: sequence %s over %d long; truncated!\n",
	       dbname, MAXSEQLEN);

    dseq[j] = '\0';
    return (j);
}

int main(int argc, char **argv)
{
    int i, j, seqlen=0;
    char desc[65536], seq[MAXSEQLEN], buf[65536], *p;
    char *ncbicodes = "XAXCDEFGHIKLMNPQRSTVWXYXXX";
    FILE *ifp;

    if (argc != 2)
	fail("Usage: seq2psi seq-file");

    ifp = fopen(argv[1], "r");
    if (!ifp)
	fail("Unable to open sequence file!");

    seqlen = getseq(desc, seq, ifp);

    if (seqlen < 5 || seqlen >= MAXSEQLEN)
	fail("Sequence length error!");

    printf("%d\n", seqlen);

    for (i=0; i<seqlen; i++)
      putchar(seq[i]);

    printf("\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n");

    for (i=0; i<seqlen; i++)
    {
	for (j=0; j<26; j++)
	  if (ncbicodes[j] != 'X')
	    printf("%d  ", aamat[aanum(seq[i])][aanum(ncbicodes[j])]*100);
	  else
	    printf("-32768  ");
	putchar('\n');
    }

    return 0;
}
