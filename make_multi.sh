
#obj_file=(${1//./ })

if  g++ --std=c++14 -Wall RFIM_2D_main_multi_core.cpp LatticeStuff.cpp  Clustering.cpp PushRelabel.cpp  -o main ; then 
    echo '*************'
    ./main
    
else 
    exit
fi

#rm build/$obj_file
