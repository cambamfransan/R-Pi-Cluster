# R-Pi-Cluster
Distributed self healing super computer

Sample Programs:
https://github.com/cambamfransan/RPiCalc

# Install Instructions

Step 1:
Navigate to the directory where the cluster will run from.
Verify that git is installed by running **git –-version**. If it is not installed, it will say "git is not installed". To install git, run **sudo apt-get install git**

To download the cluster, run **git clone https://github.com/cambamfransan/R-Pi-Cluster**

![alt text](./images/gitVersion.png)

Step 2:
A new folder called R-Pi-Cluster will be in the current directory. 
Run **./R-Pi-Cluster/Scripts/Install.sh**
This will install the dependencies for the Server and build the program.

![alt text](./images/script.png)

Step 3:
There will be a new directory called _bld. Go into that directory and start tmux by entering **tmux**.
It will appear as a new terminal.
Tmux was installed during the install script.
If accessing the raspberry pi via ssh session, run **export DISPLAY=:0**. This will make the Server use the xserver on the raspberry pi
instead of the native machine's xserver.

![alt text](./images/startServer.png)

Step 4:
Navigate to ./_bld/bin.
Run **./Server \<database\> &** where \<database\> is where all of the Server’s database files will be stored. 
The database should have at least 32 GBs of storage. If many jobs will be run, additional storage may be required.
Exit out of tmux by typing **tmux detach**. 

In a browser, connect to \<IP Address\>:8080 to access the Web Interface. Where \<IP Address\> is the ip address of the server.

# To add a Client

To start a Client, follow steps one through four on another raspberry pi except run 
**./Client \<Server IP\> \<Server Port\> \<database\>** instead of **./Server \<database\>**. 
Replace \<Server IP\> with the ip address of the Server and \<Server Port\> with the port the server is running on.
Those values will be found in the top right corner of the Web Interface.
Replace\<database\> with the path where all the files will be stored on the Client.


