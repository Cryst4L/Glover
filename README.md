## Glover

Glover is synthetic Hand Pose dataset generator written in C99. It aims to be regarded as a modern alternative to the MNIST. 
The main difference with the MNIST is that shapes are computer generated, which ensures zero ambiguity between the samples.

<p align="center">
  <img src="https://github.com/Cryst4L/Glover/blob/main/hands.png"/>
</p>

### How to generate data

The first thing to do is to build the project with CMake. To do so place yourself in the root folder and type the following:

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
# How to read the data

To read the data you wan to open the file in binary mode.

First thing to do is to read the header which consist of 4 bytes: the first one give the size of the samples, and the three others give the number of samples.

After those 4 bytes, the data is organised sample by sample as follow:

- first, the sample is recorded in row major order. The number of bytes is _size_x_size_.
- secondly the 10 degrees of freedoms that encode the Hand Pose are written one by one (each of the DOF is encoded in a single byte). 

This pattern repeat itself for every samples.

### How to custom the dataset 

You can change several configuration parameters before generating the dataset.
To set an option you just have to follow the pattern ```-option=value```.
Here is the list of the parameters available:

- the _size_ of the sample (default is 48)
- the _number_ of samples (default is 10000)
- the random _seed_ used by the algorithm rngs (default is 0)
- the Euler Angles that define the rotation of the mesh (default is 0, 0, 0)

Here is an example:
```
 ./Glover -size=32 -number=5000 -seed=1234 -alpha=0.8
 ```
By default the name of the generated dataset is _glover-dataset.bytes_. But you can change it by adding a destination name after the option:
```
./Glover -size=32 train-dataset.bytes
```
### License

This project is released under MIT license.

```
MIT License

Copyright (c) 2016-2021, Benjamin D. Halimi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

