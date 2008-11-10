set size 1.5
set output "transient_10_500000_100000.eps"
set xlabel "Osservazioni"
set ylabel "Tempo di risposta (sec)"
plot "transient_10_500000_100000" notitle with lines lt 1 linewidth 1
