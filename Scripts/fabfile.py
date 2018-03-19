from fabric.api import run, env

def startClient(serverNode="127.0.0.1", port="0", path=""):
    run('git clone https://github.com/cambamfransan/R-Pi-Cluster.git') 
    run('sudo ./R-Pi-Cluster/Scripts/Install.sh')
#	sudo("python test.py  2>/dev/null >/dev/null &")
    run("pwd")
	run("cd _bld/bin")
	run("export DISPLAY=:0.0")
	run("tmux")
    run("./Client " + serverNode + " " + port + " " + path + " &")
	run("tmux detach")

