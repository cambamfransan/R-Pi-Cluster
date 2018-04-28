// ------------------------------------------------------------------
//
// This is the menu handler used to load system, menus, jobs, and units.
//
// ------------------------------------------------------------------
System.socket = io();
System.socket.on('disconnect', function(){
  window.alert("Lost connection to the server");
});

var Menu = { screens : {} };
var idle = {
	jobs: true,
	units: true
};

var menuSystem = {
	jobs: true, 
	units: false, 
	help: false 
};

var menuColors = {
	mouseout: '#FFFFFF', // white
	mouseover: '#3FA9F5', // blue
	click: '#FF931E' // orange
}

var menuImages = {
	add: {
        image: { mouseout: null, mouseover: null }, 
	}, 
	remove: {
        image: { mouseout: null, mouseover: null }, 
	}, 
	close: {
	    image: { mouseout: null, mouseover: null },
	}, 
	edit: {
	    image: { mouseout: null, mouseover: null },
	}, 
	save: {
	    image: { mouseout: null, mouseover: null },
	}, 
	push: {
	    image: { mouseout: null, mouseover: null },
	}, 
	play: {
        image: { mouseout: null, mouseover: null },
	}, 
	pause: {
        image: { mouseout: null, mouseover: null },
	}, 
	stop: {
        image: { mouseout: null, mouseover: null },
    }
};

var jobsTable = document.getElementById('jobs-table');
var jobsBody = jobsTable.getElementsByTagName('tbody')[0];

var unitsTable = document.getElementById('units-table');
var unitsBody = unitsTable.getElementsByTagName('tbody')[0];

var viewTable = document.getElementById('view-table');
var viewBody = viewTable.getElementsByTagName('tbody')[0];

var jobsSearch = null;
var unitsSearch = null;

function addListener(e) {
	if (e.which === 13) { // ENTER key
		if (menuSystem.jobs)
			document.getElementById('add-job-button').click();
		else if (menuSystem.units)
			document.getElementById('add-unit-button').click();
	}
}
var sortColor = '#C51A4A';

var sortJobs = {
	name: true,
	priority: false,
	status: false,
	progress: false
};

var sortUnits = {
	name: true,
	status: false,
	cpu: false,
	ram: false
};

var controlAll = {
	play: false, 
	pause: false, 
	stop: false 
};

var errorHTML = '<dt>ERROR!</dt>Missing or Invalid Input';

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
		// Then, set the new screen to be active.
		document.getElementById(id).classList.add('active');
	}
	//------------------------------------------------------------------
	//
	// This function performs the one-time system initialization.
	//
	//------------------------------------------------------------------
	function initialize() {
        // Load all images of the menu interface.
		menuImages.add.image.mouseout = new Image();
		menuImages.add.image.mouseout.src = 'gui/images/add_mouseout.svg';
		menuImages.add.image.mouseover = new Image();
		menuImages.add.image.mouseover.src = 'gui/images/add_mouseover.svg';
		menuImages.remove.image.mouseout = new Image();
		menuImages.remove.image.mouseout.src = 'gui/images/remove_mouseout.svg';
		menuImages.remove.image.mouseover = new Image();
		menuImages.remove.image.mouseover.src = 'gui/images/remove_mouseover.svg';
		menuImages.close.image.mouseout = new Image();
		menuImages.close.image.mouseout.src = 'gui/images/close_mouseout.svg';
		menuImages.close.image.mouseover = new Image();
		menuImages.close.image.mouseover.src = 'gui/images/close_mouseover.svg';
		menuImages.edit.image.mouseout = new Image();
		menuImages.edit.image.mouseout.src = 'gui/images/edit_mouseout.svg';
		menuImages.edit.image.mouseover = new Image();
		menuImages.edit.image.mouseover.src = 'gui/images/edit_mouseover.svg';
		menuImages.save.image.mouseout = new Image();
		menuImages.save.image.mouseout.src = 'gui/images/save_mouseout.svg';
		menuImages.save.image.mouseover = new Image();
		menuImages.save.image.mouseover.src = 'gui/images/save_mouseover.svg';
		menuImages.push.image.mouseout = new Image();
		menuImages.push.image.mouseout.src = 'gui/images/push_mouseout.svg';
		menuImages.push.image.mouseover = new Image();
		menuImages.push.image.mouseover.src = 'gui/images/push_mouseover.svg';
		menuImages.play.image.mouseout = new Image();
		menuImages.play.image.mouseout.src = 'gui/images/play_mouseout.svg';
		menuImages.play.image.mouseover = new Image();
		menuImages.play.image.mouseover.src = 'gui/images/play_mouseover.svg';
		menuImages.pause.image.mouseout = new Image();
		menuImages.pause.image.mouseout.src = 'gui/images/pause_mouseout.svg';
		menuImages.pause.image.mouseover = new Image();
		menuImages.pause.image.mouseover.src = 'gui/images/pause_mouseover.svg';
		menuImages.stop.image.mouseout = new Image();
		menuImages.stop.image.mouseout.src = 'gui/images/stop_mouseout.svg';
		menuImages.stop.image.mouseover = new Image();
		menuImages.stop.image.mouseover.src = 'gui/images/stop_mouseover.svg';
		// Draws all pop-up windows to the center of the menu body.
		drawCenter(true);
        // Add event listeners to all interface buttons.
		document.getElementById('add-button').addEventListener('mouseover', function() { 
			// Add button icon and border color turns green.
			document.getElementById('add-image').src = menuImages.add.image.mouseover.src;
			document.getElementById('add-button').style.borderColor = '#7AC943';
		});
		document.getElementById('add-button').addEventListener('mouseout', function() { 
			// Add button icon and border color returns to default.
			document.getElementById('add-image').src = menuImages.add.image.mouseout.src;
			document.getElementById('add-button').style.borderColor = '';
		});
		document.getElementById('add-button').addEventListener('click', function() { 
			// Display pop-up window for adding a job or unit.
			if (!menuSystem.help) {
				document.getElementById('system').style.visibility = 'hidden';
				document.getElementById('popup-screen').style.visibility = 'visible';
				document.getElementById('hide-screen').style.visibility = 'visible';
				document.getElementById('add-button').style.visibility = 'hidden';
				document.getElementById('remove-button').style.visibility = 'hidden';
				document.getElementById('search-image').style.visibility = 'hidden';
				document.getElementById('search-input').style.visibility = 'hidden';
				document.getElementById('empty-table').style.visibility = 'hidden';
				if (menuSystem.jobs) {
					document.getElementById('add-job').style.visibility = 'visible';
					document.getElementById('checkAllJobs').checked = false;
					document.getElementById('job-remote').focus();
				}
				else if (menuSystem.units) {
					document.getElementById('add-unit').style.visibility = 'visible';
					document.getElementById('checkAllUnits').checked = false;
					document.getElementById('unit-name').focus();
				}
				document.addEventListener('keydown', addInputListener);
			}
		});
		document.getElementById('remove-button').addEventListener('mouseover', function() { 
			// Remove button icon and border color turns red.
			document.getElementById('remove-image').src = menuImages.remove.image.mouseover.src;
			document.getElementById('remove-button').style.borderColor = '#FF1D25';
		});
		document.getElementById('remove-button').addEventListener('mouseout', function() { 
			// Remove button icon and border color returns to default.
			document.getElementById('remove-image').src = menuImages.remove.image.mouseout.src;
			document.getElementById('remove-button').style.borderColor = '';
		});
		document.getElementById('remove-button').addEventListener('click', function() { 
			// Removes all checked rows from the job or unit table.
			if (!menuSystem.help) {
				if (menuSystem.jobs) {
					System.main.systemEvent('remove_job');
					emptyTable('job');
					document.getElementById('checkAllJobs').checked = false;
					if (jobsBody.rows.length > 1) {
						if (sortJobs.name)
							sortTable(1,'text');
						else if (sortJobs.priority)
							sortTable(3,'number');
						else if (sortJobs.status)
							sortTable(4,'text');
						else if (sortJobs.progress)
							sortTable(5,'number');
					}
				}	
				else if (menuSystem.units) {
					System.main.systemEvent('remove_unit');
					emptyTable('unit');
					document.getElementById('checkAllUnits').checked = false;
					if (unitsBody.rows.length > 1) {
						if (sortUnits.name)
							sortTable(1,'text');
						else if (sortUnits.status)
							sortTable(4,'text');
						else if (sortUnits.cpu)
							sortTable(5,'number');
						else if (sortUnits.ram)
							sortTable(6,'number');
					}
				}
			}
		});
    document.getElementById('server-IP-text').innerText = 'IP:PORT MISSING!';
    System.socket.on('RequestIpAck', function(data) {
      document.getElementById('server-IP-text').innerText = data.ipAddress + ":" + data.port;
    });
    System.socket.emit('systemData', {MsgType: 'RequestIp'});
		document.getElementById('search-input').addEventListener('input', function () {
			// Search table for specified text.
			if (!menuSystem.help) {
				if ((menuSystem.jobs && jobsBody.rows.length != 0) || (menuSystem.units && unitsBody.rows.length != 0)) {
					var input = document.getElementById('search-input');
					var filter = input.value.toUpperCase();
					var table;
					if (menuSystem.jobs)
						table = jobsTable;
					else if (menuSystem.units)
						table = unitsTable;
					var tr = table.getElementsByTagName('tr');
					for (var index = 0; index < tr.length; index++) {
						var td = tr[index].getElementsByTagName('td')[1];
						if (td) {
							if (td.innerText.toUpperCase().indexOf(filter) > -1)
								tr[index].style.display = '';
							else
								tr[index].style.display = 'none';
						}
					}
				}
			}
		});
		document.getElementById('jobs-button').addEventListener('mouseover', function () {
			// Jobs button border color turns blue.
		    activeMenu('jobs','mouseover');
		});
		document.getElementById('jobs-button').addEventListener('mouseout', function () {
			// Jobs button border color returns to default.
		    activeMenu('jobs','mouseout');
		});
		document.getElementById('jobs-button').addEventListener('click', function () {
			// Display the "Jobs" menu screen.
			document.getElementById('add-button').style.visibility = 'visible';
			document.getElementById('remove-button').style.visibility = 'visible';
			document.getElementById('search-image').style.visibility = 'visible';
			document.getElementById('search-input').style.visibility = 'visible';
			document.getElementById('checkAllJobs').checked = false;
			if (menuSystem.jobs)
				jobsSearch = document.getElementById('search-input').value;
			else if (menuSystem.units)
				unitsSearch = document.getElementById('search-input').value;
		    activeMenu('jobs','click');
			emptyTable('job');
			document.getElementById('search-input').value = jobsSearch;
			showScreen('jobs-screen');
		});
		document.getElementById('units-button').addEventListener('mouseover', function () {
			// Units button border color turns blue.
		    activeMenu('units','mouseover');
		});
		document.getElementById('units-button').addEventListener('mouseout', function () {
			// Units button border color returns to default.
		    activeMenu('units','mouseout');
		});
		document.getElementById('units-button').addEventListener('click', function () {
			// Display the "Units" menu screen.
			document.getElementById('add-button').style.visibility = 'visible';
			document.getElementById('remove-button').style.visibility = 'visible';
			document.getElementById('search-image').style.visibility = 'visible';
			document.getElementById('search-input').style.visibility = 'visible';
			document.getElementById('checkAllUnits').checked = false;
			if (menuSystem.jobs)
				jobsSearch = document.getElementById('search-input').value;
			else if (menuSystem.units)
				unitsSearch = document.getElementById('search-input').value;
		    activeMenu('units','click');
			emptyTable('unit');
			document.getElementById('search-input').value = unitsSearch;
			showScreen('units-screen');
		});
		document.getElementById('help-button').addEventListener('mouseover', function () {
			// Help button border color turns blue.
		    activeMenu('help','mouseover');
		});
		document.getElementById('help-button').addEventListener('mouseout', function () {
			// Help button border color returns to default.
		    activeMenu('help','mouseout');
		});
		document.getElementById('help-button').addEventListener('click', function () {
		    // Display the "Help" menu screen.
			document.getElementById('add-button').style.visibility = 'hidden';
			document.getElementById('remove-button').style.visibility = 'hidden';
			document.getElementById('search-image').style.visibility = 'hidden';
			document.getElementById('search-input').style.visibility = 'hidden';
			document.getElementById('empty-table').style.visibility = 'hidden';
			if (menuSystem.jobs)
				jobsSearch = document.getElementById('search-input').value;
			else if (menuSystem.units)
				unitsSearch = document.getElementById('search-input').value;
			activeMenu('help','click');
			showScreen('help-screen');
		});
		document.getElementById('about-button').addEventListener('mouseover', function () {
			// About button border color turns blue.
		    activeMenu('about','mouseover');
		});
		document.getElementById('about-button').addEventListener('mouseout', function () {
			// About button border color returns to default.
		    activeMenu('about','mouseout');
		});
        document.getElementById('about-button').addEventListener('click', function() { 
			// Display pop-up window for "About".
			document.getElementById('system').style.visibility = 'hidden';
			document.getElementById('popup-screen').style.visibility = 'visible';
			document.getElementById('hide-screen').style.visibility = 'visible';
			if (!menuSystem.help) {
				document.getElementById('add-button').style.visibility = 'hidden';
				document.getElementById('remove-button').style.visibility = 'hidden';
				document.getElementById('search-image').style.visibility = 'hidden';
				document.getElementById('search-input').style.visibility = 'hidden';
				document.getElementById('empty-table').style.visibility = 'hidden';
			}
			document.getElementById('about-popup').style.visibility = 'visible';
		});
		document.getElementById('close-about').addEventListener('mouseover', function () {
			// Close pop-up window button for "About" turns red.
			document.getElementById('close-about').src = menuImages.close.image.mouseover.src;
		});
		document.getElementById('close-about').addEventListener('mouseout', function () {
			// Close pop-up window button for "About" returns to default.
			document.getElementById('close-about').src = menuImages.close.image.mouseout.src;
		});
		document.getElementById('close-about').addEventListener('click', function () {
		    // Close the pop-up window for "About".
			document.getElementById('about-popup').style.visibility = 'hidden';
			document.getElementById('hide-screen').style.visibility = 'hidden';
		    document.getElementById('popup-screen').style.visibility = 'hidden';
			document.getElementById('system').style.visibility = 'visible';
			if (!menuSystem.help) {
				document.getElementById('add-button').style.visibility = 'visible';
				document.getElementById('remove-button').style.visibility = 'visible';
				document.getElementById('search-image').style.visibility = 'visible';
				document.getElementById('search-input').style.visibility = 'visible';
				if (menuSystem.jobs)
					emptyTable('job');
				else if (menuSystem.units)
					emptyTable('unit');
			}
		});
		document.getElementById('close-view').addEventListener('mouseover', function () {
			// Close pop-up window button for job/unit view turns red.
			document.getElementById('close-view').src = menuImages.close.image.mouseover.src;
		});
		document.getElementById('close-view').addEventListener('mouseout', function () {
			// Close pop-up window button for job/unit view returns to default..
		    document.getElementById('close-view').src = menuImages.close.image.mouseout.src;
		});
		document.getElementById('close-view').addEventListener('click', function () {
			// Close the view pop-up window.
			viewBody.innerHTML = '';
			document.getElementById('view-type').innerText = '';
			document.getElementById('view-name').innerText = '';
			document.getElementById('view-popup').style.visibility = 'hidden';
			document.getElementById('hide-screen').style.visibility = 'hidden';
		    document.getElementById('popup-screen').style.visibility = 'hidden';
			document.getElementById('system').style.visibility = 'visible';
			if (!menuSystem.help) {
				document.getElementById('add-button').style.visibility = 'visible';
				document.getElementById('remove-button').style.visibility = 'visible';
				document.getElementById('search-image').style.visibility = 'visible';
				document.getElementById('search-input').style.visibility = 'visible';
				if (menuSystem.jobs)
					emptyTable('job');
				else if (menuSystem.units)
					emptyTable('unit');
			}
		});
		document.getElementById('playAll-button').addEventListener('mouseover', function() { 
			// Play All button icon turns green.
			activeControl('play','mouseover');
		});
		document.getElementById('playAll-button').addEventListener('mouseout', function() { 
			// Play All button icon returns to default.
			activeControl('play','mouseout');
		});
		document.getElementById('playAll-button').addEventListener('click', function() { 
			// Start all jobs.
			activeControl('play','click');
			for (var index = 0; index < jobsBody.rows.length; index++) {
				var row = jobsBody.rows[index];
				var image = row.cells[6].getElementsByTagName('img')[0];
				document.getElementById(image.id).click();
			}
		});
		document.getElementById('pauseAll-button').addEventListener('mouseover', function() { 
			// Pause All button icon turns blue.
			activeControl('pause','mouseover');
		});
		document.getElementById('pauseAll-button').addEventListener('mouseout', function() { 
			// Pause All button icon returns to default.
			activeControl('pause','mouseout');
		});
		document.getElementById('pauseAll-button').addEventListener('click', function() { 
			// Pause all jobs.
			activeControl('pause','click');
			for (var index = 0; index < jobsBody.rows.length; index++) {
				var row = jobsBody.rows[index];
				var image = row.cells[6].getElementsByTagName('img')[1];
				document.getElementById(image.id).click();
			}
		});
		document.getElementById('stopAll-button').addEventListener('mouseover', function() { 
			// Stop All button icon turns red.
			activeControl('stop','mouseover');
		});
		document.getElementById('stopAll-button').addEventListener('mouseout', function() { 
			// Stop All button icon returns to default.
			activeControl('stop','mouseout');
		});
		document.getElementById('stopAll-button').addEventListener('click', function() { 
			// Stop all jobs.
			activeControl('stop','click');
			for (var index = 0; index < jobsBody.rows.length; index++) {
				var row = jobsBody.rows[index];
				var image = row.cells[6].getElementsByTagName('img')[2];
				document.getElementById(image.id).click();
			}
		});
		document.getElementById('main').addEventListener('mouseover', function() { 
			// Clear state of job controls.
			activeControl('clear',null);
		});
		document.getElementById('close-offline').addEventListener('mouseover', function () {
			// Close pop-up window button for offline pop-up turns red.
			document.getElementById('close-offline').src = menuImages.close.image.mouseover.src;
		});
		document.getElementById('close-offline').addEventListener('mouseout', function () {
			// Close pop-up window button for offline pop-up returns to default.
			document.getElementById('close-offline').src = menuImages.close.image.mouseout.src;
		});
		document.getElementById('close-offline').addEventListener('click', function () {
			// Close the pop-up window for offline unit.
			document.getElementById('offline-popup').style.visibility = 'hidden';
		});
		var screen = null;
		// Go through each of the screens and tell them to initialize.
		for (screen in screens) {
			if (screens.hasOwnProperty(screen)) {
				screens[screen].initialize();
			}
		}
		// Go to the default screen.
		document.getElementById('checkAllJobs').checked = false;
		activeMenu('jobs','click');
		emptyTable('job');
		showScreen('jobs-screen');
	}

	// Set to active the selected menu and disable non-active menus.
	function activeMenu(menu,event) {
	    if (menu == 'jobs') {
	        if (event == 'mouseover') {
	            if (!menuSystem.jobs)
	                document.getElementById('jobs-button').style.borderColor = menuColors.mouseover;
	        }
	        else if (event == 'mouseout') {
	            if (!menuSystem.jobs)
	                document.getElementById('jobs-button').style.borderColor = menuColors.mouseout;
	        }
	        else if (event == 'click') {
	            menuSystem.jobs = true; menuSystem.units = false; menuSystem.help = false;
	            document.getElementById('jobs-button').style.borderColor = menuColors.click;
	            document.getElementById('units-button').style.borderColor = menuColors.mouseout;
	            document.getElementById('help-button').style.borderColor = menuColors.mouseout;
	        }
	    }
	    else if (menu == 'units') {
	        if (event == 'mouseover') {
	            if (!menuSystem.units)
	                document.getElementById('units-button').style.borderColor = menuColors.mouseover;
	        }
	        else if (event == 'mouseout') {
	            if (!menuSystem.units)
	                document.getElementById('units-button').style.borderColor =  menuColors.mouseout;
	        }
	        else if (event == 'click') {
	            menuSystem.jobs = false; menuSystem.units = true; menuSystem.help = false;
	            document.getElementById('jobs-button').style.borderColor =  menuColors.mouseout;
	            document.getElementById('units-button').style.borderColor = menuColors.click;
	            document.getElementById('help-button').style.borderColor =  menuColors.mouseout;
	        }
	    }
	    else if (menu == 'help') {
	        if (event == 'mouseover') {
	            if (!menuSystem.help)
	                document.getElementById('help-button').style.borderColor = menuColors.mouseover;
	        }
	        else if (event == 'mouseout') {
	            if (!menuSystem.help)
	                document.getElementById('help-button').style.borderColor =  menuColors.mouseout;
	        }
	        else if (event == 'click') {
	            menuSystem.jobs = false; menuSystem.units = false; menuSystem.help = true;
	            document.getElementById('jobs-button').style.borderColor =  menuColors.mouseout;
	            document.getElementById('units-button').style.borderColor = menuColors.mouseout;
	            document.getElementById('help-button').style.borderColor =  menuColors.click;
	        }
	    }
	    else if (menu == 'about') {
	        if (event == 'mouseover') {
                document.getElementById('about-button').style.borderColor = menuColors.mouseover;
	        }
	        else if (event == 'mouseout') {
                document.getElementById('about-button').style.borderColor =  menuColors.mouseout;
	        }
	    }
	}

	// Set to active the selected control and disable non-active controls.
	function activeControl(control,event) {
		if (control == 'clear') {
			controlAll.play = false; controlAll.pause = false; controlAll.stop = false;
			document.getElementById('playAll-button').src = menuImages.play.image.mouseout.src;
			document.getElementById('pauseAll-button').src = menuImages.pause.image.mouseout.src;
			document.getElementById('stopAll-button').src = menuImages.stop.image.mouseout.src;
		}
		else if (control == 'play') {
	        if (event == 'mouseover') {
	            if (!controlAll.play)
					document.getElementById('playAll-button').src = menuImages.play.image.mouseover.src;
	        }
	        else if (event == 'mouseout') {
	            if (!controlAll.play)
					document.getElementById('playAll-button').src = menuImages.play.image.mouseout.src;
	        }
	        else if (event == 'click') {
				controlAll.play = true; controlAll.pause = false; controlAll.stop = false;
				document.getElementById('playAll-button').src = menuImages.play.image.mouseover.src;
				document.getElementById('pauseAll-button').src = menuImages.pause.image.mouseout.src;
				document.getElementById('stopAll-button').src = menuImages.stop.image.mouseout.src;
	        }
		}
		else if (control == 'pause') {
	        if (event == 'mouseover') {
	            if (!controlAll.pause)
					document.getElementById('pauseAll-button').src = menuImages.pause.image.mouseover.src;
	        }
	        else if (event == 'mouseout') {
	            if (!controlAll.pause)
					document.getElementById('pauseAll-button').src = menuImages.pause.image.mouseout.src;
	        }
	        else if (event == 'click') {
				controlAll.play = false; controlAll.pause = true; controlAll.stop = false;
				document.getElementById('playAll-button').src = menuImages.play.image.mouseout.src;
				document.getElementById('pauseAll-button').src = menuImages.pause.image.mouseover.src;
				document.getElementById('stopAll-button').src = menuImages.stop.image.mouseout.src;
	        }
		}
		else if (control == 'stop') {
	        if (event == 'mouseover') {
	            if (!controlAll.stop)
					document.getElementById('stopAll-button').src = menuImages.stop.image.mouseover.src;
	        }
	        else if (event == 'mouseout') {
	            if (!controlAll.stop)
					document.getElementById('stopAll-button').src = menuImages.stop.image.mouseout.src;
	        }
	        else if (event == 'click') {
				controlAll.play = false; controlAll.pause = false; controlAll.stop = true;
				document.getElementById('playAll-button').src = menuImages.play.image.mouseout.src;
				document.getElementById('pauseAll-button').src = menuImages.pause.image.mouseout.src;
				document.getElementById('stopAll-button').src = menuImages.stop.image.mouseover.src;
	        }
	    }
	}

	// Display a notice if job or unit table is empty.
	function emptyTable(item) {
		if (item == 'job') {
			if (jobsBody.rows.length == 0) {
				document.getElementById('empty-table').style.visibility = 'visible';
				document.getElementById('empty-text').innerText = 'Jobs';
				document.getElementById('search-image').style.visibility = 'hidden';
				document.getElementById('search-input').style.visibility = 'hidden';
			}
			else {
				document.getElementById('empty-table').style.visibility = 'hidden';
				document.getElementById('empty-text').innerText = '';
				document.getElementById('search-image').style.visibility = 'visible';
				document.getElementById('search-input').style.visibility = 'visible';
			}
		}
		else if (item == 'unit') {
			if (unitsBody.rows.length == 0) {
				document.getElementById('empty-table').style.visibility = 'visible';
				document.getElementById('empty-text').innerText = 'Units';
				document.getElementById('search-image').style.visibility = 'hidden';
				document.getElementById('search-input').style.visibility = 'hidden';
			}
			else {
				document.getElementById('empty-table').style.visibility = 'hidden';
				document.getElementById('empty-text').innerText = '';
				document.getElementById('search-image').style.visibility = 'visible';
				document.getElementById('search-input').style.visibility = 'visible';
			}
		}
	}

	return {
		showScreen : showScreen,
		initialize: initialize,
		activeMenu : activeMenu,
		activeControl : activeControl,
		emptyTable : emptyTable
	};
}(Menu.screens));

Menu.screens['jobs-screen'] = (function(system) {
	'use strict';
	
	function initialize() {
		document.getElementById('remote-source').checked = true;
		setSource();
		document.getElementById('remote-source').addEventListener('click', function () {
		    // Set source of new job being added to remote; i.e. GitHub URL.
		    document.getElementById('remote-source').checked = true;
		    setSource();
		});
		document.getElementById('local-source').addEventListener('click', function () {
		    // Set source of new job being added to local; i.e. Local File.
		    document.getElementById('local-source').checked = true;
		    setSource();
		});
		document.getElementById('job-name').addEventListener('input', function () {
			// Check that characters typed into job name field are valid.
			// Go to job priority field if maximum number of characters has been typed.
			if (checkInput('job-name')) {
				var current = document.getElementById('job-name');
				var next = document.getElementById('job-priority');
				var previous = null;
				autoFocusNextInput(current,next,previous,true);
			}
		});
		document.getElementById('job-priority').addEventListener('input', function () {
			// Check that characters typed into job priority field are valid.
			// Go to job name field if zero number of characters has been typed.
			document.getElementById('job-priority').type = 'text';
			if (checkInput('job-priority')) {
				var current = document.getElementById('job-priority');
				var next = document.getElementById('job-tpb');
				var previous = document.getElementById('job-name');
				autoFocusNextInput(current,next,previous,true);
			}
			document.getElementById('job-priority').type = 'number';
		});
		document.getElementById('job-priority').addEventListener('change', function () {
			// Check that number entered into job priority stays within limits.
			var value = Number(document.getElementById('job-priority').value);
			var min = Number(document.getElementById('job-priority').min);
			var max = Number(document.getElementById('job-priority').max);
			if (value < min)
				document.getElementById('job-priority').value = min;
			else if (value > max)
				document.getElementById('job-priority').value = max;
		});
		updateBars();
		document.getElementById('job-tpb').addEventListener('input', function () {
		    // Set tasks per bundle for optimal performance or optimal updates.
		    updateBars();
		});
		document.getElementById('close-job').addEventListener('mouseover', function () {
			// Close pop-up window button for "Add Job" turns red.
			document.getElementById('close-job').src = menuImages.close.image.mouseover.src;
		});
		document.getElementById('close-job').addEventListener('mouseout', function () {
			// Close pop-up window button for "Add Job" return to default.
		    document.getElementById('close-job').src = menuImages.close.image.mouseout.src;
		});
		document.getElementById('close-job').addEventListener('click', function () {
		    // Close the pop-up window for "Add Job".
			document.getElementById('add-job').style.visibility = 'hidden';
			Menu.system.emptyTable('job');
			document.getElementById('checkAllJobs').checked = false;
			document.getElementById('hide-screen').style.visibility = 'hidden';
		    document.getElementById('popup-screen').style.visibility = 'hidden';
			document.getElementById('system').style.visibility = 'visible';
			document.getElementById('add-button').style.visibility = 'visible';
			document.getElementById('remove-button').style.visibility = 'visible';
			document.getElementById('search-image').style.visibility = 'visible';
			document.getElementById('search-input').style.visibility = 'visible';
			document.removeEventListener('keydown', addInputListener);
			// Return "Add Job" pop-up window to default.
			document.getElementById('remote-source').checked = true;
			setSource();
			document.getElementById('job-name').value = '';
			document.getElementById('job-priority').value = '';
			document.getElementById('job-tpb').value = 5;
			updateBars();
		});
		document.getElementById('add-job-button').addEventListener('click', function () {
			// Add new job to the cluster.
			if (submitJob()) {
				var toSend = {MsgType: 'AddJob'};
				if (document.getElementById('remote-source').checked) {
					var jobSourceValue;
				 	if (document.getElementById('job-protocol').value == 'http')
				 		jobSourceValue = 'http://';
				 	else if (document.getElementById('job-protocol').value == 'https')
				 		jobSourceValue = 'https://';
					toSend.remote = jobSourceValue + document.getElementById('job-remote').value;
				}
				else if (document.getElementById('local-source').checked) {
					toSend.file = document.getElementById('job-local').innerText;
				}
				toSend.name = document.getElementById('job-name').value;
				toSend.priority = document.getElementById('job-priority').value;
				toSend.taskPerBundle = document.getElementById('job-tpb').value;
				System.socket.emit('systemData', toSend);
				//console.log(toSend);

				//System.main.systemEvent('add_job');
				document.getElementById('close-job').click();
				if (jobsBody.rows.length > 1) {
					if (sortJobs.name)
						sortTable(1,'text');
					else if (sortJobs.priority)
						sortTable(3,'number');
					else if (sortJobs.status)
						sortTable(4,'text');
					else if (sortJobs.progress)
						sortTable(5,'number');
				}
			}
			else {
				document.addEventListener('click', addInputListener, { capture: true, once: true });
				document.getElementById('add-job-error').innerHTML = errorHTML;
				document.getElementById('add-job-error').style.visibility = 'visible';
			}
		});
		document.getElementById('add-job-button').addEventListener("keydown", function(e) {
			e.stopImmediatePropagation();
		},false);
		document.getElementById('checkAllJobs').addEventListener('click', function () {
			// Check all jobs in the table.
			System.main.systemEvent('all_jobs');
		});
		document.getElementById('sort-jobName').addEventListener('mouseover', function () {
			// "Name" column header cell changes to bright color.
			activeSort('name','mouseover');
		});
		document.getElementById('sort-jobName').addEventListener('mouseout', function () {
			// "Name" column header cell returns to default color.
			activeSort('name','mouseout');
		});
		document.getElementById('sort-jobName').addEventListener('click', function () {
			// Sort jobs table by "Name".
			activeSort('name','click');
			sortTable(1,'text');
		});
		document.getElementById('sort-jobPriority').addEventListener('mouseover', function () {
			// "Priority" column header cell changes to bright color.
			activeSort('priority','mouseover');
		});
		document.getElementById('sort-jobPriority').addEventListener('mouseout', function () {
			// "Priority" column header cell returns to default color.
			activeSort('priority','mouseout');
		});
		document.getElementById('sort-jobPriority').addEventListener('click', function () {
			// Sort jobs table by "Priority".
			activeSort('priority','click');
			sortTable(3,'number');
		});
		document.getElementById('sort-jobStatus').addEventListener('mouseover', function () {
			// "Status" column header cell changes to bright color.
			activeSort('status','mouseover');
		});
		document.getElementById('sort-jobStatus').addEventListener('mouseout', function () {
			// "Status" column header cell returns to default color.
			activeSort('status','mouseout');
		});
		document.getElementById('sort-jobStatus').addEventListener('click', function () {
			// Sort jobs table by "Status".
			activeSort('status','click');
			sortTable(4,'text');
		});
		document.getElementById('sort-jobProgress').addEventListener('mouseover', function () {
			// "Progress" column header cell changes to bright color.
			activeSort('progress','mouseover');
		});
		document.getElementById('sort-jobProgress').addEventListener('mouseout', function () {
			// "Progress" column header cell returns to default color.
			activeSort('progress','mouseout');
		});
		document.getElementById('sort-jobProgress').addEventListener('click', function () {
			// Sort jobs table by "Progress".
			activeSort('progress','click');
			sortTable(5,'number');
		});
		activeSort('name','click');
	}

	// Set the job source to either remote or local.
	function setSource() {
	    if (document.getElementById('remote-source').checked) {
			document.getElementById("job-source").innerHTML = '<select id="job-protocol"><option value="http">http://</option><option value="https">https://</option></select><input id="job-remote" type="url" placeholder="github.com/">';
		}
	    else if (document.getElementById('local-source').checked) {
			document.getElementById("job-source").innerHTML = '<table><tr><td id="job-local">No File Selected</td><td><button id="browse-button" class="small">Browse</button><input id="browse-file" type="file"></td></tr></table>';
			document.getElementById('browse-button').addEventListener('click', function () {
				document.getElementById('browse-file').click();
			});
			document.getElementById('browse-file').addEventListener('change', function () {
				if (document.getElementById('browse-file').value == '')
					document.getElementById('job-local').innerText = 'No File Selected';
				else {
					var filePath = document.getElementById('browse-file').value;
					filePath = filePath.replace(/\\/g, ",,");
					var fileName = filePath.split(",,");
					fileName = fileName[fileName.length-1];
					document.getElementById('job-local').innerText = fileName;
				}
			});
		}
	}

	// Adjust height of speed and update bars to user input.
	function updateBars() {
		var value = document.getElementById('job-tpb').value;
		var speedHeight = value*10; var updateHeight = 100-value*10;
		speedHeight = speedHeight.toString(); updateHeight = updateHeight.toString();
		document.getElementById("speed-bar").style.height = speedHeight + '%';
		document.getElementById("update-bar").style.height = updateHeight + '%';
	}

	// Verify that "Add Job" form is filled out completely.
	function submitJob() {
		var submit = true;
		if (document.getElementById('remote-source').checked) {
			if (document.getElementById('job-remote').value == '') 
				submit = false;
		}
		else if (document.getElementById('local-source').checked) {
			if (document.getElementById('job-local').innerText == 'No File Selected')
				submit = false;
		}
		if (document.getElementById('job-name').value == '')
			submit = false;
		if (document.getElementById('job-priority').value == '')
			submit = false;
		return submit;
	}

	// Set to active the selected column header and disable non-active column headers.
	function activeSort(column,event) {
		if (column == 'name') {
	        if (event == 'mouseover') {
	            if (!sortJobs.name)
					document.getElementById('sort-jobName').style.backgroundColor = sortColor;
	        }
	        else if (event == 'mouseout') {
	            if (!sortJobs.name)
				document.getElementById('sort-jobName').style.backgroundColor = '';
	        }
	        else if (event == 'click') {
				sortJobs.name = true; sortJobs.priority = false; sortJobs.status = false; sortJobs.progress = false;
				document.getElementById('sort-jobName').style.backgroundColor = sortColor;
				document.getElementById('sort-jobPriority').style.backgroundColor = '';
				document.getElementById('sort-jobStatus').style.backgroundColor = '';
				document.getElementById('sort-jobProgress').style.backgroundColor = '';
	        }
		}
		else if (column == 'priority') {
			if (event == 'mouseover') {
				if (!sortJobs.priority)
					document.getElementById('sort-jobPriority').style.backgroundColor = sortColor;
			}
			else if (event == 'mouseout') {
				if (!sortJobs.priority)
				document.getElementById('sort-jobPriority').style.backgroundColor = '';
			}
			else if (event == 'click') {
				sortJobs.name = false; sortJobs.priority = true; sortJobs.status = false; sortJobs.progress = false;
				document.getElementById('sort-jobName').style.backgroundColor = '';
				document.getElementById('sort-jobPriority').style.backgroundColor = sortColor;
				document.getElementById('sort-jobStatus').style.backgroundColor = '';
				document.getElementById('sort-jobProgress').style.backgroundColor = '';
			}
		}
		else if (column == 'status') {
			if (event == 'mouseover') {
				if (!sortJobs.status)
					document.getElementById('sort-jobStatus').style.backgroundColor = sortColor;
			}
			else if (event == 'mouseout') {
				if (!sortJobs.status)
				document.getElementById('sort-jobStatus').style.backgroundColor = '';
			}
			else if (event == 'click') {
				sortJobs.name = false; sortJobs.priority = false; sortJobs.status = true; sortJobs.progress = false;
				document.getElementById('sort-jobName').style.backgroundColor = '';
				document.getElementById('sort-jobPriority').style.backgroundColor = '';
				document.getElementById('sort-jobStatus').style.backgroundColor = sortColor;
				document.getElementById('sort-jobProgress').style.backgroundColor = '';
			}
		}
		else if (column == 'progress') {
			if (event == 'mouseover') {
				if (!sortJobs.progress)
					document.getElementById('sort-jobProgress').style.backgroundColor = sortColor;
			}
			else if (event == 'mouseout') {
				if (!sortJobs.progress)
				document.getElementById('sort-jobProgress').style.backgroundColor = '';
			}
			else if (event == 'click') {
				sortJobs.name = false; sortJobs.priority = false; sortJobs.status = false; sortJobs.progress = true;
				document.getElementById('sort-jobName').style.backgroundColor = '';
				document.getElementById('sort-jobPriority').style.backgroundColor = '';
				document.getElementById('sort-jobStatus').style.backgroundColor = '';
				document.getElementById('sort-jobProgress').style.backgroundColor = sortColor;
			}
		}
	}

	return {
		initialize : initialize,
		setSource : setSource,
		updateBars : updateBars,
		submitJob : submitJob,
		activeSort : activeSort
	};
}(Menu.system));

Menu.screens['units-screen'] = (function(system) {
	'use strict';
	
	function initialize() {
		document.getElementById('unit-name').addEventListener('input', function () {
			// Check that characters typed into unit name field are valid.
			// Go to unit ip address field if maximum number of characters has been typed.
			if (checkInput('unit-name')) {
				var current = document.getElementById('unit-name');
				var next = document.getElementById('unit-IP1');
				var previous = null;
				autoFocusNextInput(current,next,previous,true);
			}
		});
		for (let index = 1; index <= 4; index++) {
			document.getElementById('unit-IP' + index.toString()).addEventListener('input', function () {
				// Check that characters typed into IP address fields are valid.
				// Go to next unit ip address field if maximum number of characters has been typed.
				if(checkInput('unit-IP' + index.toString())) {
					if (index == 1) {
						var current = document.getElementById('unit-IP1');
						var next = document.getElementById('unit-IP2');
						var previous = document.getElementById('unit-name');
					}
					else if (index == 2) {
						var current = document.getElementById('unit-IP2');
						var next = document.getElementById('unit-IP3');
						var previous = document.getElementById('unit-IP1');
					}
					else if (index == 3) {
						var current = document.getElementById('unit-IP3');
						var next = document.getElementById('unit-IP4');
						var previous = document.getElementById('unit-IP2');
					}
					else if (index == 4) {
						var current = document.getElementById('unit-IP4');
						var next = document.getElementById('unit-username');
						var previous = document.getElementById('unit-IP3');
					}
					autoFocusNextInput(current,next,previous,true);
				}
			});
		}
		document.getElementById('unit-username').addEventListener('input', function () {
			// Check that characters typed into unit username field are valid.
			// Go to unit password field if maximum number of characters has been typed.
			if (checkInput('unit-username')) {
				var current = document.getElementById('unit-username');
				var next = document.getElementById('unit-password');
				var previous = document.getElementById('unit-IP4');
				autoFocusNextInput(current,next,previous,false);
			}
		});
		document.getElementById('unit-password').addEventListener('input', function () {
			// Go to unit cores limit field if maximum number of characters has been typed.
			var current = document.getElementById('unit-password');
			var next = document.getElementById('unit-cores');
			var previous = document.getElementById('unit-username');
			autoFocusNextInput(current,next,previous,false);
		});
		document.getElementById('unit-cores').addEventListener('input', function () {
			// Check that characters typed into unit cores field are valid.
			document.getElementById('unit-cores').type = 'text';
			checkInput('unit-cores');
			document.getElementById('unit-cores').type = 'number';
		});
		document.getElementById('close-unit').addEventListener('mouseover', function () {
			// Close pop-up window button for "Add Unit" turns red.
		    document.getElementById('close-unit').src = menuImages.close.image.mouseover.src;
		});
		document.getElementById('close-unit').addEventListener('mouseout', function () {
			// Close pop-up window button for "Add Unit" return to default.
		    document.getElementById('close-unit').src = menuImages.close.image.mouseout.src;
		});
		document.getElementById('close-unit').addEventListener('click', function () {
		    // Close the pop-up window for "Add Unit".
			document.getElementById('add-unit').style.visibility = 'hidden';
			Menu.system.emptyTable('unit');
			document.getElementById('checkAllUnits').checked = false;
			document.getElementById('hide-screen').style.visibility = 'hidden';
		    document.getElementById('popup-screen').style.visibility = 'hidden';
			document.getElementById('system').style.visibility = 'visible';
			document.getElementById('add-button').style.visibility = 'visible';
			document.getElementById('remove-button').style.visibility = 'visible';
			document.getElementById('search-image').style.visibility = 'visible';
			document.getElementById('search-input').style.visibility = 'visible';
			document.removeEventListener('keydown', addInputListener);
			// Return "Add Unit" pop-up window to default.
			document.getElementById('unit-name').value = '';
			document.getElementById('unit-IP1').value = '';
			document.getElementById('unit-IP2').value = '';
			document.getElementById('unit-IP3').value = '';
			document.getElementById('unit-IP4').value = '';
			document.getElementById('unit-username').value = '';
			document.getElementById('unit-password').value = '';
			document.getElementById('unit-cores').value = '4';
		});
		document.getElementById('add-unit-button').addEventListener('click', function () {
			// Add new unit to the cluster.
			if (submitUnit()) {
				document.getElementById('close-unit').click();
				if (unitsBody.rows.length > 1) {
					if (sortUnits.name)
						sortTable(1,'text');
					else if (sortUnits.status)
						sortTable(4,'text');
					else if (sortUnits.cpu)
						sortTable(5,'number');
					else if (sortUnits.ram)
						sortTable(6,'number');
				}
			}
			else {
				document.addEventListener('click', addInputListener, { capture: true, once: true });
				document.getElementById('add-unit-error').innerHTML = errorHTML;
				document.getElementById('add-unit-error').style.visibility = 'visible';
			}
		});
		document.getElementById('add-unit-button').addEventListener("keydown", function(e) {
			e.stopImmediatePropagation();
		},false);
		document.getElementById('checkAllUnits').addEventListener('click', function () {
			// Check all units in the table.
			System.main.systemEvent('all_units');
		});
		document.getElementById('sort-unitName').addEventListener('mouseover', function () {
			// "Name" column header cell changes to bright color.
			activeSort('name','mouseover');
		});
		document.getElementById('sort-unitName').addEventListener('mouseout', function () {
			// "Name" column header cell returns to default color.
			activeSort('name','mouseout');
		});
		document.getElementById('sort-unitName').addEventListener('click', function () {
			// Sort units table by "Name".
			activeSort('name','click');
			sortTable(1,'text');
		});
		document.getElementById('sort-unitStatus').addEventListener('mouseover', function () {
			// "Status" column header cell changes to bright color.
			activeSort('status','mouseover');
		});
		document.getElementById('sort-unitStatus').addEventListener('mouseout', function () {
			// "Status" column header cell returns to default color.
			activeSort('status','mouseout');
		});
		document.getElementById('sort-unitStatus').addEventListener('click', function () {
			// Sort units table by "Status".
			activeSort('status','click');
			sortTable(4,'text');
		});
		document.getElementById('sort-unitCPU').addEventListener('mouseover', function () {
			// "CPU" column header cell changes to bright color.
			activeSort('cpu','mouseover');
		});
		document.getElementById('sort-unitCPU').addEventListener('mouseout', function () {
			// "CPU" column header cell returns to default color.
			activeSort('cpu','mouseout');
		});
		document.getElementById('sort-unitCPU').addEventListener('click', function () {
			// Sort units table by "CPU".
			activeSort('cpu','click');
			sortTable(5,'number');
		});
		document.getElementById('sort-unitRAM').addEventListener('mouseover', function () {
			// "RAM" column header cell changes to bright color.
			activeSort('ram','mouseover');
		});
		document.getElementById('sort-unitRAM').addEventListener('mouseout', function () {
			// "RAM" column header cell returns to default color.
			activeSort('ram','mouseout');
		});
		document.getElementById('sort-unitRAM').addEventListener('click', function () {
			// Sort units table by "RAM".
			activeSort('ram','click');
			sortTable(6,'number');
		});
		activeSort('name','click');
	}

	// Verify that "Add Unit" form is filled out completely.
	function submitUnit() {
		var submit = true;
		if (document.getElementById('unit-name').value == '')
			submit = false;
		if (document.getElementById('unit-IP1').value == '')
			submit = false;
		if (document.getElementById('unit-IP2').value == '')
			submit = false;
		if (document.getElementById('unit-IP3').value == '')
			submit = false;
		if (document.getElementById('unit-IP4').value == '')
			submit = false;
		if (document.getElementById('unit-username').value == '')
			submit = false;
		if (document.getElementById('unit-password').value == '')
			submit = false;
		if (document.getElementById('unit-cores').value == '')
			submit = false;
		return submit;
	}

	// Set to active the selected column header and disable non-active column headers.
	function activeSort(column,event) {
		if (column == 'name') {
	        if (event == 'mouseover') {
	            if (!sortUnits.name)
					document.getElementById('sort-unitName').style.backgroundColor = sortColor;
	        }
	        else if (event == 'mouseout') {
	            if (!sortUnits.name)
					document.getElementById('sort-unitName').style.backgroundColor = '';
	        }
	        else if (event == 'click') {
				sortUnits.name = true; sortUnits.status = false; sortUnits.cpu = false; sortUnits.ram = false;
				document.getElementById('sort-unitName').style.backgroundColor = sortColor;
				document.getElementById('sort-unitStatus').style.backgroundColor = '';
				document.getElementById('sort-unitCPU').style.backgroundColor = '';
				document.getElementById('sort-unitRAM').style.backgroundColor = '';
	        }
		}
		else if (column == 'status') {
			if (event == 'mouseover') {
				if (!sortUnits.status)
					document.getElementById('sort-unitStatus').style.backgroundColor = sortColor;
			}
			else if (event == 'mouseout') {
				if (!sortUnits.status)
					document.getElementById('sort-unitStatus').style.backgroundColor = '';
			}
			else if (event == 'click') {
				sortUnits.name = false; sortUnits.status = true; sortUnits.cpu = false; sortUnits.ram = false;
				document.getElementById('sort-unitName').style.backgroundColor = '';
				document.getElementById('sort-unitStatus').style.backgroundColor = sortColor;
				document.getElementById('sort-unitCPU').style.backgroundColor = '';
				document.getElementById('sort-unitRAM').style.backgroundColor = '';
			}
		}
		else if (column == 'cpu') {
			if (event == 'mouseover') {
				if (!sortUnits.cpu)
					document.getElementById('sort-unitCPU').style.backgroundColor = sortColor;
			}
			else if (event == 'mouseout') {
				if (!sortUnits.cpu)
					document.getElementById('sort-unitCPU').style.backgroundColor = '';
			}
			else if (event == 'click') {
				sortUnits.name = false; sortUnits.status = false; sortUnits.cpu = true; sortUnits.ram = false;
				document.getElementById('sort-unitName').style.backgroundColor = '';
				document.getElementById('sort-unitStatus').style.backgroundColor = '';
				document.getElementById('sort-unitCPU').style.backgroundColor = sortColor;
				document.getElementById('sort-unitRAM').style.backgroundColor = '';
			}
		}
		else if (column == 'ram') {
			if (event == 'mouseover') {
				if (!sortUnits.ram)
					document.getElementById('sort-unitRAM').style.backgroundColor = sortColor;
			}
			else if (event == 'mouseout') {
				if (!sortUnits.ram)
					document.getElementById('sort-unitRAM').style.backgroundColor = '';
			}
			else if (event == 'click') {
				sortUnits.name = false; sortUnits.status = false; sortUnits.cpu = false; sortUnits.ram = true;
				document.getElementById('sort-unitName').style.backgroundColor = '';
				document.getElementById('sort-unitStatus').style.backgroundColor = '';
				document.getElementById('sort-unitCPU').style.backgroundColor = '';
				document.getElementById('sort-unitRAM').style.backgroundColor = sortColor;
			}
		}
	}

	return {
		initialize : initialize,
		submitUnit : submitUnit,
		activeSort : activeSort
	};
}(Menu.system));

Menu.screens['help-screen'] = (function(system) {
	'use strict';
	
	function initialize() {
		//
	}
	
	return {
		initialize : initialize
	};
}(Menu.system));

loadSystem();
