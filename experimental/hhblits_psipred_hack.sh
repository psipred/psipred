
#!/bin/tcsh

# This is a simple script which will carry out all of the basic steps
# required to make a PSIPRED V4 prediction. Note that it assumes that the
# following programs are in the appropriate directories:
# blastpgp - PSIBLAST executable (from NCBI toolkit)
# makemat - IMPALA utility (from NCBI toolkit)
# psipred - PSIPRED V4 program
# psipass2 - PSIPRED V4 program

# NOTE: Script modified to be more cluster friendly (DTJ April 2008)

# The name of the HHBLITS data bank
set dbname = /ssd1/hhblits/uniclust30_2017_10

# Where the NCBI programs have been installed
set ncbidir = /usr/local/bin

# Where the PSIPRED V4 programs have been installed
set execdir = /usr/local/bin

# Where the PSIPRED V4 data files have been installed
set datadir = /usr/local/share/psipred

set basename = $1:r
set rootname = $basename:t

# Generate a "unique" temporary filename root
set hostid = `hostid`
set tmproot = psitmp$$$hostid

setenv HHLIB /usr/local/lib/hhsuite/hh

\cp -f $1 $tmproot.fasta

echo "Running hhblits with sequence" $1 "..."

$execdir/hhblits -d $dbname -i $tmproot.fasta -oa3m $tmproot.a3m -e 0.001 -n 3 -cpu 20 -diff inf -cov 10 -Z 100000 -B 100000 -maxfilt 100000 -maxmem 5 >& $tmproot.hhblits

$HHLIB/scripts/reformat.pl a3m psi $tmproot.a3m $tmproot.psi

echo "Running PSI-BLAST with sequence" $1 "..."

$ncbidir/formatdb -i $tmproot.a3m -t $tmproot.a3m

$ncbidir/blastpgp -a 12 -b 0 -j 2 -h 0.01 -d $tmproot.a3m -i $tmproot.fasta -B $tmproot.psi -C $tmproot.chk >& $tmproot.blast

if ($status != 0) then
    cat $tmproot.blast
    echo "FATAL: Error whilst running blastpgp - script terminated!"
    exit 1
endif

echo "Predicting secondary structure..."

echo $tmproot.chk > $tmproot.pn
echo $tmproot.fasta > $tmproot.sn

$ncbidir/makemat -P $tmproot

if ($status != 0) then
    echo "FATAL: Error whilst running makemat - script terminated!"
    exit 1
endif

echo Pass1 ...

$execdir/psipred $tmproot.mtx $datadir/weights.dat $datadir/weights.dat2 $datadir/weights.dat3 > $rootname.ss

if ($status != 0) then
    echo "FATAL: Error whilst running psipred - script terminated!"
    exit 1
endif

echo Pass2 ...

$execdir/psipass2 $datadir/weights_p2.dat 1 1.0 1.0 $rootname.ss2 $rootname.ss > $rootname.horiz

if ($status != 0) then
    echo "FATAL: Error whilst running psipass2 - script terminated!"
    exit 1
endif

echo Solvation pass ...

$execdir/solvpred $tmproot.mtx $datadir/weights_solv.dat > $rootname.solv

if ($status != 0) then
    echo "FATAL: Error whilst running solvpred - script terminated!"
    exit 1
endif

# Remove temporary files

echo Cleaning up ...
\rm -f $tmproot.* error.log

echo "Final output files:" $rootname.ss2 $rootname.horiz $rootname.solv
echo "Finished."
