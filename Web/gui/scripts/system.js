//------------------------------------------------------------------
//
// This is the system handler that performs the one-time system initialization.
//
//------------------------------------------------------------------
var terminal = false; // if true, then print to browser console

System.main = (function(jobs, units) {
	'use strict';
	var that = {}; // connect system to menu
	var terminate = false;

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
	// Add, remove, or select a job(s) or a unit(s).
	//
	//------------------------------------------------------------------
	that.systemEvent = function(event) {
		if (event == 'add_job') Jobs_system.add();
		else if (event == 'remove_job') Jobs_system.remove();
		else if (event == 'all_jobs') Jobs_system.all();
		else if (event == 'add_unit') Units_system.add();
		else if (event == 'remove_unit') Units_system.remove();
		else if (event == 'all_units') Units_system.all();
	};
	//------------------------------------------------------------------
	//
	// Update the state of the system based on time.
	//
	//------------------------------------------------------------------
	function systemUpdate(elapsedTime) {
		Jobs_system.update();
		Units_system.update();
	}
	//------------------------------------------------------------------
	//
	// Change the run state of the root function.
	//
	//------------------------------------------------------------------
	that.systemRun = function(run) {
		if (run)
			terminate = false;
		else
			terminate = true;
	};
	//------------------------------------------------------------------
	//
	// The root function of the system that updates the state of jobs and units.
	//
	//------------------------------------------------------------------
	that.systemLoop = function(time) {
		var elapsedTime = time - lastTimeStamp;
		lastTimeStamp = time;
		systemUpdate(elapsedTime);
		if (!terminate)
			requestAnimationFrame(that.systemLoop);
	};

	return that; // connect system to menu
 
}(System.jobs, System.units));