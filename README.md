# isolalert
Monitor the behavior and statistical efficiency of the isolation for COVID-19.


Build with oneAPI: 

/usr/bin/c++ -I/opt/intel/inteloneapi/vpl/latest/include  -o isolalert.cpp.o  -c isolalert.cpp
/usr/bin/c++ isolalert.cpp.o -lcurl  /opt/intel/inteloneapi/vpl/latest/lib/libopencv_world.so -o isolalert 
