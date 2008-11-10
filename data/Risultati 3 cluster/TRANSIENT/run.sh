#!/bin/bash
#gnuplot script

XLB="Osservazioni"
YLB="Tempo di risposta (sec)"

if [ "$1" = "" ]; then
	echo "Usage: run.sh <source_data>"
	exit -1
fi

echo $1
echo "set size 1.5" > aux.gnu
echo "set output \"$1.eps\"" >> aux.gnu
#echo "set title \"Test\"" >> aux.gnu
echo "set xlabel \"$XLB\"" >> aux.gnu
echo "set ylabel \"$YLB\"" >> aux.gnu
echo "plot \"$1\" notitle with lines lt 1 linewidth 1" >> aux.gnu
gnuplot script.input
wait
epstopdf $1.eps
wait
rm $1.eps
