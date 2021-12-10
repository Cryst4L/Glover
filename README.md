## Glover

Glover is synthetic Hand Pose dataset generator written in C99. It aims to be regarded as a modern alternative to the MNIST. 
The main difference with the MNIST is that shapes are computer generated, which ensures zero ambiguity between the samples.

<p align="center">
  <img src="https://github.com/Cryst4L/Glover/blob/main/hands.png"/>
</p>

### How to generate data

The first thing to do is to build the project with CMake. To do so place youtself in the root folder and type the following:

```sh
mkdir build 
cd build
cmake ..
make
```
Then to generate the data with the default parameters, you just need to run the executable:

```sh
./Glover
```
### How to custom the dataset 

You can change several configuration parameters before generating the dataset.
To set an option you just have to follow the pattern ```sh -option=value```
