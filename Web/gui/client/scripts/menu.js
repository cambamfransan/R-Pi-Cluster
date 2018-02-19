// ------------------------------------------------------------------
//
// This is the menu handler used to load system, menus, jobs, and units.
//
// ------------------------------------------------------------------
var menuSystem = {
	jobs: true, 
	units: false, 
	help: false 
};

function showJobs() {
	// Show table of jobs.
}

function showUnits() {
	// Show table of units.
}

Menu.system = (function(screens) {
	'use strict';
	
	//------------------------------------------------------------------
	//
	// This function is used to change to a new active screen.
	//
	//------------------------------------------------------------------
	function showScreen(id) {
		var screen = 0,
			active = null;
		// Remove the active state from all screens. There should only be one...
		active = document.getElementsByClassName('active');
		for (screen = 0; screen < active.length; screen++) {
			active[screen].classList.remove('active');
		}
		// Tell the screen to start actively running.
		screens[id].run();
		// Then, set the new screen to be active.
		document.getElementById(id).classList.add('active');
	}
	//------------------------------------------------------------------
	//
	// This function performs the one-time system initialization.
	//
	//------------------------------------------------------------------
	function initialize() {
		document.getElementById('add-button').addEventListener('mouseover', function() { 
			hover('add', true);
		});
		document.getElementById('add-button').addEventListener('mouseout', function() { 
			hover('add', false);
		});
		document.getElementById('add-button').addEventListener('click', function() { 
			// Display pop-up window for adding a job or unit.
		});
		document.getElementById('remove-button').addEventListener('mouseover', function() { 
			hover('remove', true);
		});
		document.getElementById('remove-button').addEventListener('mouseout', function() { 
			hover('remove', false);
		});
		document.getElementById('remove-button').addEventListener('click', function() { 
			// Display pop-up window for removing a job or unit.
		});
		document.getElementById('search-button').addEventListener('click', function() { 
			// Search for specified job or unit.
		});
		document.getElementById('jobs-button').addEventListener('click', function() { 
			// Display the screen for "Jobs".
		});
		document.getElementById('units-button').addEventListener('click', function() { 
			// Display the screen for "Units".
		});
		document.getElementById('help-button').addEventListener('click', function() { 
			// Display the screen for "Help".
		});
		document.getElementById('about-button').addEventListener('click', function() { 
			// Display pop-up window for "About".
		});
		document.getElementById('playAll-image').addEventListener('mouseover', function() { 
			hover('play', true);
		});
		document.getElementById('playAll-image').addEventListener('mouseout', function() { 
			hover('play', false);
		});
		document.getElementById('playAll-image').addEventListener('click', function() { 
			// Run all jobs.
		});
		document.getElementById('pauseAll-image').addEventListener('mouseover', function() { 
			hover('pause', true);
		});
		document.getElementById('pauseAll-image').addEventListener('mouseout', function() { 
			hover('pause', false);
		});
		document.getElementById('pauseAll-image').addEventListener('click', function() { 
			// Halt all jobs.
		});
		document.getElementById('stopAll-image').addEventListener('mouseover', function() { 
			hover('stop', true);
		});
		document.getElementById('stopAll-image').addEventListener('mouseout', function() { 
			hover('stop', false);
		});
		document.getElementById('stopAll-image').addEventListener('click', function() { 
			// Terminate all jobs.
		});
		document.getElementById('on-button').addEventListener('click', function() { 
			// Turn on the cluster system.
		});
		document.getElementById('off-button').addEventListener('click', function() { 
			// Turn off the cluster system.
		});
		document.getElementById('restart-button').addEventListener('click', function() { 
			// Restart the cluster system.
		});
		var screen = null;
		// Go through each of the screens and tell them to initialize.
		for (screen in screens) {
			if (screens.hasOwnProperty(screen)) {
				screens[screen].initialize();
			}
		}
		showScreen('jobs-screen');
	}

	function hover(icon, over) {
		if (icon == 'add') {
			if (over)
				document.getElementById('add-image').src = menuImages.add.image.mouseover.src;
			else
				document.getElementById('add-image').src = menuImages.add.image.mouseout.src;
		}
		else if (icon == 'remove') {
			if (over)
				document.getElementById('remove-image').src = menuImages.remove.image.mouseover.src;
			else
				document.getElementById('remove-image').src = menuImages.remove.image.mouseout.src;
		}
		else if (icon == 'play') {
			if (over)
				document.getElementById('playAll-image').src = menuImages.play.all.image.mouseover.src;
			else
				document.getElementById('playAll-image').src = menuImages.play.all.image.mouseout.src;
		}
		else if (icon == 'pause') {
			if (over)
				document.getElementById('pauseAll-image').src = menuImages.pause.all.image.mouseover.src;
			else
				document.getElementById('pauseAll-image').src = menuImages.pause.all.image.mouseout.src;
		}
		else if (icon == 'stop') {
			if (over)
				document.getElementById('stopAll-image').src = menuImages.stop.all.image.mouseover.src;
			else
				document.getElementById('stopAll-image').src = menuImages.stop.all.image.mouseout.src;
		}
	}

	return {
		initialize : initialize,
		showScreen : showScreen,
		hover : hover 
	};
}(Menu.screens));

Menu.screens['jobs-screen'] = (function(system) {
	'use strict';
	
	function initialize() {
		// none
	}

	function run() {
		// none
	}

	return {
		initialize : initialize,
		run : run 
	};
}(Menu.system));

Menu.screens['units-screen'] = (function(system) {
	'use strict';
	
	function initialize() {
		// none
	}

	function run() {
		// none
	}

	return {
		initialize : initialize,
		run : run 
	};
}(Menu.system));

Menu.screens['help-screen'] = (function(system) {
	'use strict';
	
	function initialize() {
		// none
	}
	
	function run() {
		// none
	}
	
	return {
		initialize : initialize,
		run : run
	};
}(Menu.system));