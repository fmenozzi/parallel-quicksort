n=( 10 100 1000 10000 100000 1000000 10000000 100000000 )

for i in "${n[@]}"
do
    touch seq-times-n-$i.txt
    touch par-times-n-$i.txt

	./seq $i >> seq-times-n-$i.txt
	./par $i >> par-times-n-$i.txt
done
