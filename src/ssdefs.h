#define MAXSEQLEN 10000

enum
{
    FALSE, TRUE
};

#define SQR(x) ((x)*(x))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))

/* logistic 'squashing' function (output range +/- 1.0) */
#define logistic(x) (1.0F / (1.0F + expf(-(x))))

/* Rectifier function */
#define rectifier(x) ((x) < 0.0F ? 0.0F : (x))
