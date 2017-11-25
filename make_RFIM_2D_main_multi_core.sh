
#obj_file=(${1//./ })

if  g++ --std=c++14 -Wall RFIM_2D_main_multi_core.cpp LatticeStuff.cpp  Clustering.cpp PushRelabel.cpp -lpthread -o main ; then 
    echo '****** RUNNING MULTI CORE RFIM SIMULATION *******'
    ./main
    
else 
    echo '  XXXXX  SOMETHING WENT WRONG XXXXX '
    echo '  XXXXX  most probably your career decision '
    exit
fi

#rm build/$obj_file
