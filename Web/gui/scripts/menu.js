// ------------------------------------------------------------------
//
// This is the menu handler used to load system, menus, jobs, and units.
//
// ------------------------------------------------------------------
var System = {};
System.socket = io();

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
var unitsTable = document.getElementById('units-table');

var jobsBody = jobsTable.getElementsByTagName('tbody')[0];
var unitsBody = unitsTable.getElementsByTagName('tbody')[0];

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
        // Load all images of interface.
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
        // Add event listeners to all interface buttons.
		document.getElementById('add-button').addEventListener('mouseover', function() { 
			hover('add', true);
		});
		document.getElementById('add-button').addEventListener('mouseout', function() { 
			hover('add', false);
		});
		document.getElementById('add-button').addEventListener('click', function() { 
			// Display pop-up window for adding a job or unit.
			if (!menuSystem.help) {
				document.getElementById('system').style.visibility = 'hidden';
				document.getElementById('hide-screen').style.zIndex = 1;
				document.getElementById('hide-screen').style.visibility = 'visible';
				document.getElementById('popup-screen').style.zIndex = 2;
				document.getElementById('popup-screen').style.visibility = 'visible';
				document.getElementById('add-button').style.visibility = 'hidden';
				document.getElementById('remove-button').style.visibility = 'hidden';
				document.getElementById('search-image').style.visibility = 'hidden';
				document.getElementById('search-text').style.visibility = 'hidden';
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
				document.addEventListener('keydown', addListener);
			}
		});
		document.getElementById('remove-button').addEventListener('mouseover', function() { 
			hover('remove', true);
		});
		document.getElementById('remove-button').addEventListener('mouseout', function() { 
			hover('remove', false);
		});
		document.getElementById('remove-button').addEventListener('click', function() { 
			if (!menuSystem.help) {
				if (menuSystem.jobs) {
					System.main.systemEvent('remove_job');
					empty('job');
					document.getElementById('checkAllJobs').checked = false;
				}	
				else if (menuSystem.units) {
					System.main.systemEvent('remove_unit');
					empty('unit');
					document.getElementById('checkAllUnits').checked = false;
				}
			}
		});
    document.getElementById('server-IP-text').innerText = 'IP:PORT MISSING!';
    System.socket.on('RequestIpAck', function(data) {
      document.getElementById('server-IP-text').innerText = data.ipAddress + ":" + data.port;
    });
    System.socket.emit('systemData', {MsgType: 'RequestIp'});
		document.getElementById('search-text').addEventListener('input', function () {
			// Search table for specified text.
			if (!menuSystem.help) {
				if ((menuSystem.jobs && jobsBody.rows.length != 0) || (menuSystem.units && unitsBody.rows.length != 0)) {
					var input = document.getElementById("search-text");
					var filter = input.value.toUpperCase();
					var table;
					if (menuSystem.jobs)
						table = jobsTable;
					else if (menuSystem.units)
						table = unitsTable;
					var tr = table.getElementsByTagName("tr");

					for (var index = 0; index < tr.length; index++) {
						var td = tr[index].getElementsByTagName("td")[1];
						if (td) {
							if (td.innerText.toUpperCase().indexOf(filter) > -1)
								tr[index].style.display = "";
							else
								tr[index].style.display = "none";
						}
					}
				}
			}
		});
		document.getElementById('jobs-button').addEventListener('mouseover', function () {
		    active('jobs', 'mouseover');
		});
		document.getElementById('jobs-button').addEventListener('mouseout', function () {
		    active('jobs', 'mouseout');
		});
		document.getElementById('jobs-button').addEventListener('click', function () {
			// Display the "Jobs" menu screen.
			document.getElementById('add-button').style.visibility = 'visible';
			document.getElementById('remove-button').style.visibility = 'visible';
			document.getElementById('search-image').style.visibility = 'visible';
			document.getElementById('search-text').style.visibility = 'visible';
			document.getElementById('checkAllJobs').checked = false;
			if (menuSystem.jobs)
				jobsSearch = document.getElementById('search-text').value;
			else if (menuSystem.units)
				unitsSearch = document.getElementById('search-text').value;
		    active('jobs', 'click');
			empty('job');
			document.getElementById('search-text').value = jobsSearch;
			showScreen('jobs-screen');
		});
		document.getElementById('units-button').addEventListener('mouseover', function () {
		    active('units', 'mouseover');
		});
		document.getElementById('units-button').addEventListener('mouseout', function () {
		    active('units', 'mouseout');
		});
		document.getElementById('units-button').addEventListener('click', function () {
			// Display the "Units" menu screen.
			document.getElementById('add-button').style.visibility = 'visible';
			document.getElementById('remove-button').style.visibility = 'visible';
			document.getElementById('search-image').style.visibility = 'visible';
			document.getElementById('search-text').style.visibility = 'visible';
			document.getElementById('checkAllUnits').checked = false;
			if (menuSystem.jobs)
				jobsSearch = document.getElementById('search-text').value;
			else if (menuSystem.units)
				unitsSearch = document.getElementById('search-text').value;
		    active('units', 'click');
			empty('unit');
			document.getElementById('search-text').value = unitsSearch;
			showScreen('units-screen');
		});
		document.getElementById('help-button').addEventListener('mouseover', function () {
		    active('help', 'mouseover');
		});
		document.getElementById('help-button').addEventListener('mouseout', function () {
		    active('help', 'mouseout');
		});
		document.getElementById('help-button').addEventListener('click', function () {
		    // Display the "Help" menu screen.
			document.getElementById('add-button').style.visibility = 'hidden';
			document.getElementById('remove-button').style.visibility = 'hidden';
			document.getElementById('search-image').style.visibility = 'hidden';
			document.getElementById('search-text').style.visibility = 'hidden';
			document.getElementById('empty-table').style.visibility = 'hidden';
			document.getElementById('empty-table-text').innerText = '';
			if (menuSystem.jobs)
				jobsSearch = document.getElementById('search-text').value;
			else if (menuSystem.units)
				unitsSearch = document.getElementById('search-text').value;
			active('help', 'click');
			showScreen('help-screen');
		});
		document.getElementById('about-button').addEventListener('mouseover', function () {
		    active('about', 'mouseover');
		});
		document.getElementById('about-button').addEventListener('mouseout', function () {
		    active('about', 'mouseout');
		});
        document.getElementById('about-button').addEventListener('click', function() { 
			// Display pop-up window for "About".
		});
		document.getElementById('playAll-button').addEventListener('mouseover', function() { 
			hover('play', true);
		});
		document.getElementById('playAll-button').addEventListener('mouseout', function() { 
			hover('play', false);
		});
		document.getElementById('playAll-button').addEventListener('click', function() { 
			// Run all jobs.
		});
		document.getElementById('pauseAll-button').addEventListener('mouseover', function() { 
			hover('pause', true);
		});
		document.getElementById('pauseAll-button').addEventListener('mouseout', function() { 
			hover('pause', false);
		});
		document.getElementById('pauseAll-button').addEventListener('click', function() { 
			// Halt all jobs.
		});
		document.getElementById('stopAll-button').addEventListener('mouseover', function() { 
			hover('stop', true);
		});
		document.getElementById('stopAll-button').addEventListener('mouseout', function() { 
			hover('stop', false);
		});
		document.getElementById('stopAll-button').addEventListener('click', function() { 
			// Clear all jobs.
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
		active('jobs', 'click');
		empty('job');
		showScreen('jobs-screen');
	}

	function active(menu, event) {
	    if (menu == 'jobs') {
	        if (event == 'mouseover') {
	            if (!menuSystem.jobs)
	                document.getElementById('jobs-button').style.borderColor = '#3FA9F5';
	        }
	        else if (event == 'mouseout') {
	            if (!menuSystem.jobs)
	                document.getElementById('jobs-button').style.borderColor = '#FFFFFF';
	        }
	        else if (event == 'click') {
	            menuSystem.jobs = true; menuSystem.units = false; menuSystem.help = false;
	            document.getElementById('jobs-button').style.borderColor = '#FF931E';
	            document.getElementById('units-button').style.borderColor = '#FFFFFF';
	            document.getElementById('help-button').style.borderColor = '#FFFFFF';
	        }
	    }
	    else if (menu == 'units') {
	        if (event == 'mouseover') {
	            if (!menuSystem.units)
	                document.getElementById('units-button').style.borderColor = '#3FA9F5';
	        }
	        else if (event == 'mouseout') {
	            if (!menuSystem.units)
	                document.getElementById('units-button').style.borderColor = '#FFFFFF';
	        }
	        else if (event == 'click') {
	            menuSystem.jobs = false; menuSystem.units = true; menuSystem.help = false;
	            document.getElementById('jobs-button').style.borderColor = '#FFFFFF';
	            document.getElementById('units-button').style.borderColor = '#FF931E';
	            document.getElementById('help-button').style.borderColor = '#FFFFFF';
	        }
	    }
	    else if (menu == 'help') {
	        if (event == 'mouseover') {
	            if (!menuSystem.help)
	                document.getElementById('help-button').style.borderColor = '#3FA9F5';
	        }
	        else if (event == 'mouseout') {
	            if (!menuSystem.help)
	                document.getElementById('help-button').style.borderColor = '#FFFFFF';
	        }
	        else if (event == 'click') {
	            menuSystem.jobs = false; menuSystem.units = false; menuSystem.help = true;
	            document.getElementById('jobs-button').style.borderColor = '#FFFFFF';
	            document.getElementById('units-button').style.borderColor = '#FFFFFF';
	            document.getElementById('help-button').style.borderColor = '#FF931E';
	        }
	    }
	    else if (menu == 'about') {
	        if (event == 'mouseover') {
                document.getElementById('about-button').style.borderColor = '#3FA9F5';
	        }
	        else if (event == 'mouseout') {
                document.getElementById('about-button').style.borderColor = '#FFFFFF';
	        }
	    }
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
				document.getElementById('playAll-button').src = menuImages.play.image.mouseover.src;
			else
				document.getElementById('playAll-button').src = menuImages.play.image.mouseout.src;
		}
		else if (icon == 'pause') {
			if (over)
				document.getElementById('pauseAll-button').src = menuImages.pause.image.mouseover.src;
			else
				document.getElementById('pauseAll-button').src = menuImages.pause.image.mouseout.src;
		}
		else if (icon == 'stop') {
			if (over)
				document.getElementById('stopAll-button').src = menuImages.stop.image.mouseover.src;
			else
				document.getElementById('stopAll-button').src = menuImages.stop.image.mouseout.src;
		}
	}

	function empty(item) {
		if (item == 'job') {
			if (jobsBody.rows.length == 0) {
				if (!idle.jobs) {
					idle.jobs = true;
					System.main.systemRun(false);
				}
				document.getElementById('empty-table').style.visibility = 'visible';
				document.getElementById('empty-table-text').innerText = 'Jobs';
			}
			else {
				if (idle.jobs) {
					idle.jobs = false;
					if (idle.units) {
						System.main.systemRun(true);
						System.main.systemLoop(performance.now());
					}
				}
				document.getElementById('empty-table').style.visibility = 'hidden';
				document.getElementById('empty-table-text').innerText = '';
			}
		}
		else if (item == 'unit') {
			if (unitsBody.rows.length == 0) {
				if (!idle.units) {
					idle.units = true;
					System.main.systemRun(false);
				}
				document.getElementById('empty-table').style.visibility = 'visible';
				document.getElementById('empty-table-text').innerText = 'Units';
			}
			else {
				if (idle.units) {
					idle.units = false;
					if (idle.jobs) {
						System.main.systemRun(true);
						System.main.systemLoop(performance.now());
					}
				}
				document.getElementById('empty-table').style.visibility = 'hidden';
				document.getElementById('empty-table-text').innerText = '';
			}
		}
	}

	function run() {
		// none
	}

	return {
		showScreen : showScreen,
		initialize: initialize,
        active : active,
		hover : hover,
		empty : empty,
		run : run
	};
}(Menu.screens));

Menu.screens['jobs-screen'] = (function(system) {
	'use strict';
	
	function initialize() {
		document.getElementById('remote-source').checked = true;
		source();
		document.getElementById('remote-source').addEventListener('click', function () {
		    // Set source of new job being added to remote.
		    document.getElementById('remote-source').checked = true;
		    source();
		});
		document.getElementById('local-source').addEventListener('click', function () {
		    // Set source of new job being added to local.
		    document.getElementById('local-source').checked = true;
		    source();
		});
		document.getElementById('job-name').addEventListener('input', function () {
			// Check that characters typed into job name field are valid.
			if (!document.getElementById('job-name').validity.valid) {
				var value = document.getElementById('job-name').value;
				value = value.slice(0,-1);
				document.getElementById('job-name').value = value;
			}
		});
		document.getElementById('job-priority').addEventListener('input', function () {
			// Check that characters typed into job priority field are valid.
			if (!document.getElementById('job-priority').validity.valid) {
				var value = document.getElementById('job-priority').value;
				value = value.slice(0,-1);
				document.getElementById('job-priority').value = value;
			}
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
		range();
		document.getElementById('job-tpb').addEventListener('input', function () {
		    // Set tasks per bundle for optimal performance or optimal updates.
		    range();
		});
		document.getElementById('close-job').addEventListener('mouseover', function () {
		    hover('close', true);
		});
		document.getElementById('close-job').addEventListener('mouseout', function () {
		    hover('close', false);
		});
		document.getElementById('close-job').addEventListener('click', function () {
		    // Close the job popup window.
			document.getElementById('add-job').style.visibility = 'hidden';
			Menu.system.empty('job');
			document.getElementById('checkAllJobs').checked = false;
		    document.getElementById('popup-screen').style.visibility = 'hidden';
		    document.getElementById('popup-screen').style.zIndex = -2;
		    document.getElementById('hide-screen').style.visibility = 'hidden';
			document.getElementById('hide-screen').style.zIndex = -1;
			document.getElementById('system').style.visibility = 'visible';
			document.getElementById('add-button').style.visibility = 'visible';
			document.getElementById('remove-button').style.visibility = 'visible';
			document.getElementById('search-image').style.visibility = 'visible';
			document.getElementById('search-text').style.visibility = 'visible';
			document.removeEventListener('keydown', addListener);
			// Return job popup form to default.
			document.getElementById('remote-source').checked = true;
			source();
			document.getElementById('job-name').value = '';
			document.getElementById('job-priority').value = '';
			document.getElementById('job-tpb').value = 5;
			range();
		});
		document.getElementById('checkAllJobs').addEventListener('click', function () {
			System.main.systemEvent('all_jobs');
		});
		document.getElementById('add-job-button').addEventListener('click', function () {
			// Add new job to the cluster.
			if (submit()) {
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
			}
		});
	}

	function hover(icon, over) {
		if (icon == 'close') {
		    if (over)
				document.getElementById('close-job').src = menuImages.close.image.mouseover.src;
			else
				document.getElementById('close-job').src = menuImages.close.image.mouseout.src;
		}
	}

	function source() {
	    if (document.getElementById('remote-source').checked) {
			document.getElementById("source-cell").innerHTML = '<select id="job-protocol"><option value="http">http://</option><option value="https">https://</option></select><input type="url" id="job-remote" placeholder="github.com/">';
		}
	    else if (document.getElementById('local-source').checked) {
			document.getElementById("source-cell").innerHTML = '<table style="width:100%;"><tr><td id="job-local">No File Selected</td><td style="width:1%;"><button id="browse-button" class="small">Browse</button><input type="file" id="browse-file" style="display:none;"></td></tr></table>';
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

	function range() {
		var value = document.getElementById('job-tpb').value;
		var speedHeight = value*10; var updateHeight = 100-value*10;
		speedHeight = speedHeight.toString(); updateHeight = updateHeight.toString();
		document.getElementById("speed-bar").style.height = speedHeight + '%';
		document.getElementById("update-bar").style.height = updateHeight + '%';
	}

	function submit() {
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

	function run() {
		// none
	}

	return {
		initialize : initialize,
		hover : hover,
		source : source,
		range : range,
		submit : submit,
		run : run
	};
}(Menu.system));

Menu.screens['units-screen'] = (function(system) {
	'use strict';
	
	function initialize() {
		document.getElementById('unit-name').addEventListener('input', function () {
			// Check that characters typed into unit name field are valid.
			if (!document.getElementById('unit-name').validity.valid) {
				var value = document.getElementById('unit-name').value;
				value = value.slice(0,-1);
				document.getElementById('unit-name').value = value;
			}
		});
		document.getElementById('unit-IP1').addEventListener('input', function () {
			// Check that characters typed into unit IP1 field are valid.
			if (!document.getElementById('unit-IP1').validity.valid) {
				var value = document.getElementById('unit-IP1').value;
				value = value.slice(0,-1);
				document.getElementById('unit-IP1').value = value;
			}
		});
		document.getElementById('unit-IP2').addEventListener('input', function () {
			// Check that characters typed into unit IP2 field are valid.
			if (!document.getElementById('unit-IP2').validity.valid) {
				var value = document.getElementById('unit-IP2').value;
				value = value.slice(0,-1);
				document.getElementById('unit-IP2').value = value;
			}
		});
		document.getElementById('unit-IP3').addEventListener('input', function () {
			// Check that characters typed into unit IP3 field are valid.
			if (!document.getElementById('unit-IP3').validity.valid) {
				var value = document.getElementById('unit-IP3').value;
				value = value.slice(0,-1);
				document.getElementById('unit-IP3').value = value;
			}
		});
		document.getElementById('unit-IP4').addEventListener('input', function () {
			// Check that characters typed into unit IP4 field are valid.
			if (!document.getElementById('unit-IP4').validity.valid) {
				var value = document.getElementById('unit-IP4').value;
				value = value.slice(0,-1);
				document.getElementById('unit-IP4').value = value;
			}
		});
		document.getElementById('unit-cores').addEventListener('input', function () {
			// Check that characters typed into unit cores field are valid.
			if (!document.getElementById('unit-cores').validity.valid) {
				var value = document.getElementById('unit-cores').value;
				value = value.slice(0,-1);
				document.getElementById('unit-cores').value = value;
			}
		});
		document.getElementById('close-unit').addEventListener('mouseover', function () {
		    hover('close', true);
		});
		document.getElementById('close-unit').addEventListener('mouseout', function () {
		    hover('close', false);
		});
		document.getElementById('close-unit').addEventListener('click', function () {
		    // Close the unit popup window.
			document.getElementById('add-unit').style.visibility = 'hidden';
			Menu.system.empty('unit');
			document.getElementById('checkAllUnits').checked = false;
		    document.getElementById('popup-screen').style.visibility = 'hidden';
		    document.getElementById('popup-screen').style.zIndex = -2;
		    document.getElementById('hide-screen').style.visibility = 'hidden';
			document.getElementById('hide-screen').style.zIndex = -1;
			document.getElementById('system').style.visibility = 'visible';
			document.getElementById('add-button').style.visibility = 'visible';
			document.getElementById('remove-button').style.visibility = 'visible';
			document.getElementById('search-image').style.visibility = 'visible';
			document.getElementById('search-text').style.visibility = 'visible';
			// Return unit popup form to default.
			document.getElementById('unit-name').value = '';
			document.getElementById('unit-IP1').value = '';
			document.getElementById('unit-IP2').value = '';
			document.getElementById('unit-IP3').value = '';
			document.getElementById('unit-IP4').value = '';
			document.getElementById('unit-username').value = '';
			document.getElementById('unit-password').value = '';
			document.getElementById('unit-cores').value = '4';
		});
		document.getElementById('checkAllUnits').addEventListener('click', function () {
			System.main.systemEvent('all_units');
		});
		document.getElementById('add-unit-button').addEventListener('click', function () {
			// Add new unit to the cluster.
			if (submit()) {
				System.main.systemEvent('add_unit');
				document.getElementById('close-unit').click();
			}
		});
	}

	function hover(icon, over) {
		if (icon == 'close') {
		    if (over)
				document.getElementById('close-unit').src = menuImages.close.image.mouseover.src;
			else
				document.getElementById('close-unit').src = menuImages.close.image.mouseout.src;
		}
	}

	function submit() {
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

	function run() {
		// none
	}

	return {
		initialize : initialize,
		hover : hover,
		submit : submit,
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

