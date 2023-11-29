# isolalert

***ISOALERT*** is a Project created to analyze the behavior of the population, calculate the necessary distance between people to avoid contagion (COVID-19) and statistical processing of the number of people and vehicles in a given region.

# Overview / Usage
Using computer vision and deep learning techniques, the system analyzes the behavior of the population, calculating the necessary distance between people to avoid contagion. And with pedestrian and vehicle location algorithms, it is possible to obtain an estimate of the number of people traveling in a given region.

Social distance are behaviors that limit social interaction to reduce the spread of certain diseases. The adoption of these measures is important to prevent an increase in the number of people infected. Thus, the population is able to reduce the number of hospitalizations and avoid overloading the health system

Based on the problems of the COVID-19 pandemic, the ISOLALERT project was created. Its objective is to monitor the flow of people, with this monitoring we can calculate the distance between the identified objects. Then, it is possible to measure the efficiency of social distance in a given location (public and private). By measuring the social distance, the State and the Government can make decisions by regions based on the behavior of the population with the rules imposed to contain the contamination.

See below the example running in real time, on Avenida Paulista in the city of São Paulo / Brazil:

https://cam.assuntonerd.com.br/

- Vehicle counter: Analyzes the number of vehicles present in the processed video
- Pedestrian counter: Analyzes the number of pedestrians present in the live video
- Mask detector: Analyzes quantities of users who obey the rules for using the mask
- Behavior detector: Analyzes numbers of users who do not obey the rules of behavior (hugs, handshake, mask manipulation)
- Distance meter: Analyzes quantities of users who do not obey social distance rules

## Building the ISOALERT

### Download Intel® oneAPI Base Toolkit

The firt thing you need is to download and install the Intel® oneAPI Base Toolkit.

You can register, download and information about install it from the following link (Linux version): https://software.intel.com/content/www/us/en/develop/tools/oneapi/download.html#basekit 

### Clone the project

Clone the repository at desired location:

``` bash
# git clone https://github.com/cabelo/isolalert
```

### Download the required files

Download the required files. You will have to download the pre-trained weight file and another. Or just run these commands:

``` bash
$ wget https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov3.cfg

$ wget https://pjreddie.com/media/files/yolov3.weights

$ wget https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names

```

### Build the project

Now execute 

``` bash
$ /usr/bin/c++ -I/opt/intel/inteloneapi/vpl/latest/include  -o isolalert.cpp.o  -c isolalert.cpp
$ /usr/bin/c++ isolalert.cpp.o -lcurl  /opt/intel/inteloneapi/vpl/latest/lib/libopencv_world.so -o isolalert 

```

### The final result

Bellow an example running in Avenue Paulista in public Brazil camera.

contact : Alessandro de Oliveira Faria (A.K.A.CABELO) cabelo@opensuse.org

![](img/isolalert.jpg)
