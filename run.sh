n=( 10 100 1000 10000 100000 1000000 10000000 100000000 1000000000 )

touch seq-times.txt
touch par-times.txt

for i in "${n[@]}"
do
    ./seq $i >> seq-times.txt
    ./par $i >> par-times.txt
done
