#define MAXSEQLEN 10000

enum
{
    FALSE, TRUE
};

#define SQR(x) ((x)*(x))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))

#define REAL float

/* logistic 'squashing' function (output range +/- 1.0) */
#define logistic(x) ((REAL)1.0 / ((REAL)1.0 + (REAL)exp(-(x))))
