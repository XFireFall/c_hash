set output 'hash_comp.png'
set ylabel "Count of words"
set xlabel "Hash value"
set grid ytics
set style data histograms
set boxwidth 1 absolute
set style fill solid 1
set yrange [0:100]
set xrange [-1:9999]
set terminal png font "Verdana,50" size 10000, 1000
plot 'hash_comp.txt' u 2 ti "hash 1", '' u 3 ti "hash 2", '' u 4 ti "hash 3", '' u 5 ti "hash 4", '' u 6 ti "hash 5"
set terminal png font "Verdana,10" size 1000, 800
set output 'hash_1.png'
plot '' u 2 ti "hash 1" lt rgb 'blue'
set output 'hash_2.png'
plot '' u 3 ti "hash 2" lt rgb 'blue'
set output 'hash_3.png'
plot '' u 4 ti "hash 3" lt rgb 'blue'
set output 'hash_4.png'
plot '' u 5 ti "hash 4" lt rgb 'blue'
set output 'hash_5.png'
plot '' u 6 ti "hash 5" lt rgb 'blue'
