/* seq2mtx - convert single sequence to pseudo IMPALA mtx file */

/* Copyright (C) 2000 D.T. Jones */

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
const short aamat[23][23] =
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

/* Report an error and exit. */
static void fail(const char *message)
{
    fprintf(stderr, "\n*** %s\n\n", message);
    exit(EXIT_FAILURE);
}

/* Convert an ASCII amino-acid letter to a numeric code (0-22). */
static int aanum(int ch)
{
    static const int aacvs[] =
    {
        999, 0, 20, 4, 3, 6, 13, 7, 8, 9, 22, 11, 10, 12, 2,
        22, 14, 5, 1, 15, 16, 22, 19, 17, 22, 18, 21
    };

    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        return aacvs[ch & 31];
    return 22;
}

/* Read the first sequence in one of the supported input formats. */
static int getseq(char *dbname, char *dseq, FILE *lfil)
{
    enum { UNKNOWN, EMBL, FASTA, OWL, GCG };
    int fformat, badln, i, j = 0, split;
    size_t len;
    char buf[MAXSEQLEN];

    dbname[0] = '\0';
    dseq[0] = '\0';

    if (!fgets(buf, sizeof(buf), lfil))
        return -1;

    if (strstr(buf, "of:") != NULL && strstr(buf, "check:") != NULL)
        fformat = GCG;
    else if (strncmp(buf, "ID   ", 5) == 0)
        fformat = EMBL;
    else if (buf[0] == '>' &&
             (buf[1] == '>' || (strlen(buf) >= 4 && buf[3] == ';')))
        fformat = OWL;
    else if (buf[0] == '>')
        fformat = FASTA;
    else
    {
        fprintf(stderr,
                "WARNING: Attempting to interpret input file "
                "with unknown format\n");
        fformat = UNKNOWN;
    }

    switch (fformat)
    {
    case GCG:
        if (sscanf(strstr(buf, "of:") + 3, "%70s", dbname) != 1)
            return -1;
        while (strstr(buf, "..") == NULL)
            if (!fgets(buf, sizeof(buf), lfil))
                return -1;
        if (!fgets(buf, sizeof(buf), lfil))
            return -1;
        break;

    case EMBL:
        snprintf(dbname, 71, "%.70s", buf + 5);
        while (buf[0] != ' ')
            if (!fgets(buf, sizeof(buf), lfil))
                return -1;
        break;

    case OWL:
        if (!fgets(buf, sizeof(buf), lfil))
            return -1;
        snprintf(dbname, 71, "%.70s", buf);
        if (!fgets(buf, sizeof(buf), lfil))
            return -1;
        break;

    case FASTA:
        snprintf(dbname, 71, "%.70s", buf + 1);
        if (!fgets(buf, sizeof(buf), lfil))
            return -1;
        break;

    default:
        /* Retain the original heuristic for unrecognised formats. */
        do
        {
            badln = (strpbrk(buf, "JjOoUu<>#$%&@") != NULL);
            if (badln && !fgets(buf, sizeof(buf), lfil))
                return -1;
        }
        while (badln);
        strcpy(dbname, "<NO NAME>");
        break;
    }

    len = strlen(dbname);
    while (len > 0 && (dbname[len - 1] == '\n' || dbname[len - 1] == '\r'))
        dbname[--len] = '\0';

    for (;;)
    {
        /* A new FASTA/PIR/OWL header must never become sequence data. */
        if ((fformat == FASTA || fformat == OWL) && buf[0] == '>')
        {
            fprintf(stderr,
                    "WARNING: Additional sequence record ignored; "
                    "using the first sequence only.\n");
            break;
        }

        if (strncmp(buf, "//", 2) == 0)
            break;

        for (i = 0; buf[i] != '\0' && j < MAXSEQLEN - 1; i++)
        {
            /* ctype functions require EOF or an unsigned-char value. */
            split = toupper((unsigned char)buf[i]);
            if (split == '@' || (fformat == OWL && split == '*'))
            {
                dseq[j] = '\0';
                return j;
            }
            if (split >= 'A' && split <= 'Z')
                dseq[j++] = (char)split;
        }

        if (!fgets(buf, sizeof(buf), lfil))
            break;
    }

    if (j == MAXSEQLEN - 1)
        fprintf(stderr, "\nWARNING: sequence %s reaches the %d-residue "
                "buffer limit; input rejected!\n", dbname, MAXSEQLEN - 1);

    dseq[j] = '\0';
    return j;
}

int main(int argc, char **argv)
{
    int i, j, seqlen;
    char desc[71], seq[MAXSEQLEN];
    const char *ncbicodes = "XAXCDEFGHIKLMNPQRSTVWXYXXX";
    FILE *ifp;

    if (argc != 2)
        fail("Usage: seq2mtx seq-file");

    ifp = fopen(argv[1], "r");
    if (!ifp)
        fail("Unable to open sequence file!");

    seqlen = getseq(desc, seq, ifp);
    if (ferror(ifp))
        fail("Error reading sequence file!");
    if (fclose(ifp) != 0)
        fail("Error closing sequence file!");

    /* Keep the original conservative sequence-length limit. */
    if (seqlen < 5 || seqlen >= MAXSEQLEN - 1)
        fail("Sequence length error!");

    printf("%d\n%s\n", seqlen, seq);
    printf("0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n");

    for (i = 0; i < seqlen; i++)
    {
        for (j = 0; j < 26; j++)
        {
            if (ncbicodes[j] != 'X')
                printf("%d  ", aamat[aanum(seq[i])][aanum(ncbicodes[j])] * 100);
            else
                printf("-32768  ");
        }
        putchar('\n');
    }

    if (fflush(stdout) == EOF || ferror(stdout))
        fail("Error writing matrix output!");

    return EXIT_SUCCESS;
}
