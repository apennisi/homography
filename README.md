# Homography
A simple tool for computing the homography between two images

##Requirements

Homography requires the following packeges to build:

* OpenCV

## How to build

Homography works under Linux and Mac Os environments. We recommend a so-called out of source build 
which can be achieved by the following command sequence:

* mkdir build
* cd build
* cmake ../
* make -j\<number-of-cores+1\>

##How to use

Once the build phase has been successfully, you can use the main provided in the 
repository by launching the file **homography** located in the directory _bin_.
The correct command is: _./homography_ _image_ _mappedImage_ .
