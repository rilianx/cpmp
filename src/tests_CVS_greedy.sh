tot=0; count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-3/data3-3-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1)); else echo ./feg 5 ../Instancias/CVS/3-3/data3-3-$i.dat 0; fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-4/data3-4-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1)); else echo ./feg 5 ../Instancias/CVS/3-4/data3-4-$i.dat 0;  fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-5/data3-5-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-6/data3-6-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-7/data3-7-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 5 ../Instancias/CVS/3-8/data3-8-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 6 ../Instancias/CVS/4-4/data4-4-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1)); else echo ./feg 6 ../Instancias/CVS/4-4/data4-4-$i.dat 0;  fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 6 ../Instancias/CVS/4-5/data4-5-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 6 ../Instancias/CVS/4-6/data4-6-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 6 ../Instancias/CVS/4-7/data4-7-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-4/data5-4-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));  else echo ./feg 7 ../Instancias/CVS/5-4/data5-4-$i.dat 0; fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-5/data5-5-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-6/data5-6-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-7/data5-7-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-8/data5-8-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-9/data5-9-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 7 ../Instancias/CVS/5-10/data5-10-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 8 ../Instancias/CVS/6-6/data6-6-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1)); else echo ./feg 8 ../Instancias/CVS/6-6/data6-6-$i.dat 0;  fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 8 ../Instancias/CVS/6-10/data6-10-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1));    fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 12 ../Instancias/CVS/10-6/data10-6-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1)); else echo ./feg 12 ../Instancias/CVS/10-6/data10-6-$i.dat 0;  fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
tot=0; count=0; for i in {1..40}; do res=$(./feg 12 ../Instancias/CVS/10-10/data10-10-$i.dat 1.2 0.6 0 --FEG | head -1 | cut -d$'\t' -f1); if [[ $res -ne -1 ]]; then tot=$(($tot+$res)); count=$((count+1)); else echo ./feg 12 ../Instancias/CVS/10-12/data10-12-$i.dat 0;   fi; done; echo $((100*$tot / $count)) | sed -e 's/..$/.&/;t' -e 's/.$/.0&/'
