from fabric.api import run, env

def startClient(serverNode="127.0.0.1", port=0):
    run('git clone https://github.com/cambamfransan/R-Pi-Cluster.git') 
    run('sudo ./R-Pi-Cluster/Scripts/Install.sh')
#	sudo("python test.py  2>/dev/null >/dev/null &")
    run("./_bld/bin/Client.exe " + serverNode + " " + port + " &")

