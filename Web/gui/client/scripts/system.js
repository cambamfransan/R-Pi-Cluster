//------------------------------------------------------------------
//
// This is the system handler that performs the one-time system initialization.
//
//------------------------------------------------------------------
var terminal = false; // if true, then print to browser console

System.main = (function(jobs, units) {
	'use strict';
	var that = {}; // connect system to menu
	var run = false; // if true, then system is running

	var lastTimeStamp = performance.now(),
		//------------------------------------------------------------------
		// Execute the one-time system initialization of jobs and units.
		//------------------------------------------------------------------
		Jobs_system = jobs.Jobs({
			//
		}),
		Units_system = units.Units({
			//
		});
	//------------------------------------------------------------------
	//
	// Load the system.
	//
	//------------------------------------------------------------------
	that.systemLoad = function() {
		if (terminal) console.log('Loading system...');
		//
	};
	//------------------------------------------------------------------
	//
	// Clear the system.
	//
	//------------------------------------------------------------------
	function systemClear() {
		//
	}
	//------------------------------------------------------------------
	//
	// Add or remove a job or a unit.
	//
	//------------------------------------------------------------------
	that.systemEvents = function(event) {
		if (event == 'add_job') Jobs_system.add();
		if (event == 'remove_job') Jobs_system.remove();
		if (event == 'add_unit') Units_system.add();
		if (event == 'remove_unit') Units_system.remove();
	};
	//------------------------------------------------------------------
	//
	// Update the state of the system based on time.
	//
	//------------------------------------------------------------------
	function systemUpdate(elapsedTime) {
		//
	}
	//------------------------------------------------------------------
	//
	// The root function of the system that updates the state of jobs and units.
	//
	//------------------------------------------------------------------
	that.systemLoop = function(time) {
		var elapsedTime = time - lastTimeStamp;
		lastTimeStamp = time;
		systemUpdate(elapsedTime);
	};

	return that; // connect system to menu
 
}(System.jobs, System.units));