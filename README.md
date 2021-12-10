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
To set an option you just have to follow the pattern ```-option=value```.
Here is the list of the parameters available:

- the _size_ of the sample (default is 48)
- the _number_ of samples (default is 10000)
- the random _seed_ used by the algorithm rngs (default is 0)
- the Euler Angles that define the rotation of the mesh (default is 0, 0, 0)

Here is an example:
````
 ./glover -size=32 -number=5000 -seed=1234 -alpha=0.8
 ```
