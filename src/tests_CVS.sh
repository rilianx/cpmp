nb=$1
k=$2
type=$3
seed=$4
a=$5
r=$6

count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-3/data3-3-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});  echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-4/data3-4-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});  echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-5/data3-5-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-6/data3-6-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-7/data3-7-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-8/data3-8-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 6 ../Instancias/CVS/4-4/data4-4-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});   echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 6 ../Instancias/CVS/4-5/data4-5-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 6 ../Instancias/CVS/4-6/data4-6-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 6 ../Instancias/CVS/4-7/data4-7-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-4/data5-4-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});   echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-5/data5-5-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-6/data5-6-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-7/data5-7-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-8/data5-8-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-9/data5-9-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-10/data5-10-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});    echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 8 ../Instancias/CVS/6-6/data6-6-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});   echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 8 ../Instancias/CVS/6-10/data6-10-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});     echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 12 ../Instancias/CVS/10-6/data10-6-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});   echo ${res}; done;
count=0; for i in {1..40}; do res=$(./feg 12 ../Instancias/CVS/10-10/data10-10-$i.dat ${a} ${k} ${nb} --${type} ${seed} | head -1 | cut -d$'\t' -f${r});   echo ${res}; done;
