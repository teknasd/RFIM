
g++ --std=c++14 -Ofast -lm -funroll-loops -msse4 -Wall LatticeStuff.cpp clustering.cpp RFIM_2D_main.cpp PushRelabel.cpp -o app

./app




#if [! -d log];then
#mkdir log
#fi


