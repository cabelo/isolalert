# isolalert

***ISOALRT*** is a Project created to analyze the behavior of the population, calculate the necessary distance between people to avoid contagion (COVID-19) and statistical processing of the number of people and vehicles in a given region.

## Building the ISOALERT

The firt thing you need is to download and install the Intel® oneAPI Base Toolki.

You can register, download and information about install it from the following link (Linux version): https://software.intel.com/content/www/us/en/develop/tools/oneapi/download.html#basekit 

``` bash
/usr/bin/c++ -I/opt/intel/inteloneapi/vpl/latest/include  -o isolalert.cpp.o  -c isolalert.cpp
/usr/bin/c++ isolalert.cpp.o -lcurl  /opt/intel/inteloneapi/vpl/latest/lib/libopencv_world.so -o isolalert 

```
