// ------------------------------------------------------------------
//
// This is the load handler that loads all system media assets.
//
// ------------------------------------------------------------------
// number of media files loaded
var filesLoaded = { current: 0, limit: 16, done: false };

function systemReady() {
    if (!filesLoaded.done) {
        if (filesLoaded.current >= filesLoaded.limit) {
            filesLoaded.done = true;
            document.getElementById('load').style.visibility = 'hidden';
            Menu.system.initialize();
        }
        else {
            var percentage = (filesLoaded.current / filesLoaded.limit) * 100;
            document.getElementById('percentage').innerHTML = Math.round(percentage);
        }
    }
}

var Menu = { screens : {} };

var menuImages = {
	play: {
        one: {
            image: { mouseout: null, mouseover: null }, 
            ready: { mouseout: false, mouseover: false } 
        }, 
        all: {
            image: { mouseout: null, mouseover: null }, 
            ready: { mouseout: false, mouseover: false } 
        } 
    }, 
    pause: {
        one: {
            image: { mouseout: null, mouseover: null }, 
            ready: { mouseout: false, mouseover: false } 
        }, 
        all: {
            image: { mouseout: null, mouseover: null }, 
            ready: { mouseout: false, mouseover: false } 
        } 
    }, 
    stop: {
        one: {
            image: { mouseout: null, mouseover: null }, 
            ready: { mouseout: false, mouseover: false } 
        }, 
        all: {
            image: { mouseout: null, mouseover: null }, 
            ready: { mouseout: false, mouseover: false } 
        } 
    }, 
    add: {
        image: { mouseout: null, mouseover: null }, 
		ready: { mouseout: false, mouseover: false } 
    }, 
    remove: {
        image: { mouseout: null, mouseover: null }, 
		ready: { mouseout: false, mouseover: false } 
    } 
};

function filesLoad() {
    menuImages.add.image.mouseout = new Image();
    menuImages.add.image.mouseout.src = 'client/media/images/add_mouseout.svg';
    menuImages.add.image.mouseout.addEventListener('load', function() {
        menuImages.add.ready.mouseout = true;
        document.getElementById('add-image').src = menuImages.add.image.mouseout.src;
        filesLoaded.current++; systemReady();
        menuImages.add.image.mouseout.removeEventListener('load', null);
    });
    menuImages.add.image.mouseover = new Image();
    menuImages.add.image.mouseover.src = 'client/media/images/add_mouseover.svg';
    menuImages.add.image.mouseover.addEventListener('load', function() {
        menuImages.add.ready.mouseover = true;
        filesLoaded.current++; systemReady();
        menuImages.add.image.mouseover.removeEventListener('load', null);
    });
    menuImages.remove.image.mouseout = new Image();
    menuImages.remove.image.mouseout.src = 'client/media/images/remove_mouseout.svg';
    menuImages.remove.image.mouseout.addEventListener('load', function() {
        menuImages.remove.ready.mouseout = true;
        document.getElementById('remove-image').src = menuImages.remove.image.mouseout.src;
        filesLoaded.current++; systemReady();
        menuImages.remove.image.mouseout.removeEventListener('load', null);
    });
    menuImages.remove.image.mouseover = new Image();
    menuImages.remove.image.mouseover.src = 'client/media/images/remove_mouseover.svg';
    menuImages.remove.image.mouseover.addEventListener('load', function() {
        menuImages.remove.ready.mouseover = true;
        filesLoaded.current++; systemReady();
        menuImages.remove.image.mouseover.removeEventListener('load', null);
    });
    menuImages.play.all.image.mouseout = new Image();
    menuImages.play.all.image.mouseout.src = 'client/media/images/play_mouseout.svg';
    menuImages.play.one.image.mouseout = menuImages.play.all.image.mouseout;
    menuImages.play.one.image.mouseout.src = menuImages.play.all.image.mouseout.src;
    menuImages.play.all.image.mouseout.addEventListener('load', function() {
        menuImages.play.all.ready.mouseout = true;
        document.getElementById('playAll-image').src = menuImages.play.all.image.mouseout.src;
        filesLoaded.current++; systemReady();
        menuImages.play.all.image.mouseout.removeEventListener('load', null);
    });
    menuImages.play.one.image.mouseout.addEventListener('load', function() {
        menuImages.play.one.ready.mouseout = true;
        document.getElementById('play-image').src = menuImages.play.one.image.mouseout.src;
        filesLoaded.current++; systemReady();
        menuImages.play.one.image.mouseout.removeEventListener('load', null);
    });
    menuImages.play.all.image.mouseover = new Image();
    menuImages.play.all.image.mouseover.src = 'client/media/images/play_mouseover.svg';
    menuImages.play.one.image.mouseover = menuImages.play.all.image.mouseover;
    menuImages.play.one.image.mouseover.src = menuImages.play.all.image.mouseover.src;
    menuImages.play.all.image.mouseover.addEventListener('load', function() {
        menuImages.play.all.ready.mouseover = true;
        document.getElementById('playAll-image').src = menuImages.play.all.image.mouseover.src;
        filesLoaded.current++; systemReady();
        menuImages.play.all.image.mouseover.removeEventListener('load', null);
    });
    menuImages.play.one.image.mouseover.addEventListener('load', function() {
        menuImages.play.one.ready.mouseover = true;
        document.getElementById('play-image').src = menuImages.play.one.image.mouseover.src;
        filesLoaded.current++; systemReady();
        menuImages.play.one.image.mouseover.removeEventListener('load', null);
    });
    menuImages.pause.all.image.mouseout = new Image();
    menuImages.pause.all.image.mouseout.src = 'client/media/images/pause_mouseout.svg';
    menuImages.pause.one.image.mouseout = menuImages.pause.all.image.mouseout;
    menuImages.pause.one.image.mouseout.src = menuImages.pause.all.image.mouseout.src;
    menuImages.pause.all.image.mouseout.addEventListener('load', function() {
        menuImages.pause.all.ready.mouseout = true;
        document.getElementById('pauseAll-image').src = menuImages.pause.all.image.mouseout.src;
        filesLoaded.current++; systemReady();
        menuImages.pause.all.image.mouseout.removeEventListener('load', null);
    });
    menuImages.pause.one.image.mouseout.addEventListener('load', function() {
        menuImages.pause.one.ready.mouseout = true;
        document.getElementById('pause-image').src = menuImages.pause.one.image.mouseout.src;
        filesLoaded.current++; systemReady();
        menuImages.pause.one.image.mouseout.removeEventListener('load', null);
    });
    menuImages.pause.all.image.mouseover = new Image();
    menuImages.pause.all.image.mouseover.src = 'client/media/images/pause_mouseover.svg';
    menuImages.pause.one.image.mouseover = menuImages.pause.all.image.mouseover;
    menuImages.pause.one.image.mouseover.src = menuImages.pause.all.image.mouseover.src;
    menuImages.pause.all.image.mouseover.addEventListener('load', function() {
        menuImages.pause.all.ready.mouseover = true;
        document.getElementById('pauseAll-image').src = menuImages.pause.all.image.mouseover.src;
        filesLoaded.current++; systemReady();
        menuImages.pause.all.image.mouseover.removeEventListener('load', null);
    });
    menuImages.pause.one.image.mouseover.addEventListener('load', function() {
        menuImages.pause.one.ready.mouseover = true;
        document.getElementById('pause-image').src = menuImages.pause.one.image.mouseover.src;
        filesLoaded.current++; systemReady();
        menuImages.pause.one.image.mouseover.removeEventListener('load', null);
    });
    menuImages.stop.all.image.mouseout = new Image();
    menuImages.stop.all.image.mouseout.src = 'client/media/images/stop_mouseout.svg';
    menuImages.stop.one.image.mouseout = menuImages.stop.all.image.mouseout;
    menuImages.stop.one.image.mouseout.src = menuImages.stop.all.image.mouseout.src;
    menuImages.stop.all.image.mouseout.addEventListener('load', function() {
        menuImages.stop.all.ready.mouseout = true;
        document.getElementById('stopAll-image').src = menuImages.stop.all.image.mouseout.src;
        filesLoaded.current++; systemReady();
        menuImages.stop.all.image.mouseout.removeEventListener('load', null);
    });
    menuImages.stop.one.image.mouseout.addEventListener('load', function() {
        menuImages.stop.one.ready.mouseout = true;
        document.getElementById('stop-image').src = menuImages.stop.one.image.mouseout.src;
        filesLoaded.current++; systemReady();
        menuImages.stop.one.image.mouseout.removeEventListener('load', null);
    });
    menuImages.stop.all.image.mouseover = new Image();
    menuImages.stop.all.image.mouseover.src = 'client/media/images/stop_mouseover.svg';
    menuImages.stop.one.image.mouseover = menuImages.stop.all.image.mouseover;
    menuImages.stop.one.image.mouseover.src = menuImages.stop.all.image.mouseover.src;
    menuImages.stop.all.image.mouseover.addEventListener('load', function() {
        menuImages.stop.all.ready.mouseover = true;
        document.getElementById('stopAll-image').src = menuImages.stop.all.image.mouseover.src;
        filesLoaded.current++; systemReady();
        menuImages.stop.all.image.mouseover.removeEventListener('load', null);
    });
    menuImages.stop.one.image.mouseover.addEventListener('load', function() {
        menuImages.stop.one.ready.mouseover = true;
        document.getElementById('stop-image').src = menuImages.stop.one.image.mouseover.src;
        filesLoaded.current++; systemReady();
        menuImages.stop.one.image.mouseover.removeEventListener('load', null);
    });
    System.main.systemLoad();
}