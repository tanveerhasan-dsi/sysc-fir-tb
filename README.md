# sysc-fir-tb

a sample SystemC Training Sources using Forte's training video 

Using linux SystemC docker.  Will try MacOS later.  

Thanks to Zim.  

Simple and quick installation with docker

To make this simple installation happen, one must have docker. By the command below we will pull the docker image for SystemC.

docker pull learnwithexamples/systemc


How to run!
Run this command to create a container with our systemc image and get inside the container.

docker run -it --name systemc learnwithexamples/systemc /bin/bash



Once we exit, we can rerun the same container with

docker start -i systemc




