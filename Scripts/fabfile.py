from fabric.api import run, env

def startClient(serverNode="127.0.0.1", port):
    run('git clone https://github.com/cambamfransan/R-Pi-Cluster.git')
	run('cd R-Pi-Cluster/Scripts')
	run('chmod 7777 Install.sh')
	run('./Install.sh')
#	sudo("python test.py  2>/dev/null >/dev/null &")
	run('cd ../../_bld/bin')
	run('./Client.exe ' + serverNode + ' ' + port)
