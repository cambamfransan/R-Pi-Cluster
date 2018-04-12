# R-Pi-Cluster
Distributed self healing super computer

To Build, you must install rapidjson, QT5, and google protobufs
Everytime you push up to github, jenkins will pull it down and test it.

Sample Programs:
https://github.com/cambamfransan/RPiCalc

Step 1:
Navigate to the directory where the cluster will run.
Verify that git is installed by running git –-version. If it is not installed, it will say git is not installed. To install git, run sudo apt-get install git
To download the cluster, run git clone https://github.com/cambamfransan/R-Pi-Cluster

![alt text](./images/gitVersion.png)

Step 2:
After running git clone, a new folder called R-Pi-Cluster will be in the current directory. 
Run ./R-Pi-Cluster/Scripts/Install.sh
This will install the dependencies for the server and build the program.

![alt text](./images/script.pdf)
