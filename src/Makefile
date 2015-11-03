######################################################################
#	Makefile for PSIpred3                                        #
######################################################################

#### CC		Command to execute C compiler
#### CFLAGS	Flags to pass to C compiler.

INCDIR		= .
CC		= cc

CFLAGS		= -O
LIBS		= -lm

all:		psipred psipass2 chkparse seq2mtx pfilt

install:
		/bin/cp psipred psipass2 chkparse seq2mtx pfilt ../bin

clean:
		/bin/rm -f psipred psipass2 chkparse seq2mtx pfilt

psipred:	sspred_avpred.c ssdefs.h sspred_net.h
		$(CC) $(CFLAGS) sspred_avpred.c $(LIBS) -o psipred

psipass2:	sspred_hmulti.c ssdefs.h sspred_net2.h
		$(CC) $(CFLAGS) sspred_hmulti.c $(LIBS) -o psipass2

chkparse:	chkparse.c
		$(CC) $(CFLAGS) chkparse.c $(LIBS) -o chkparse

seq2mtx:	seq2mtx.c
		$(CC) $(CFLAGS) seq2mtx.c $(LIBS) -o seq2mtx

pfilt:		pfilt.c
		$(CC) $(CFLAGS) pfilt.c $(LIBS) -o pfilt
