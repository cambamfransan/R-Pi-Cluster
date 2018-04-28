//------------------------------------------------------------------
//
// This is the system handler that performs the one-time system initialization.
//
//------------------------------------------------------------------
System.main = (function(jobs, units) {
	'use strict';
	var that = {}, // Connect System to Menu
	//------------------------------------------------------------------
	// Execute the one-time system initialization of jobs and units.
	//------------------------------------------------------------------
	Jobs_system = jobs.Jobs({}),
	Units_system = units.Units({});
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
	return that; // Connect System to Menu
 
}(System.jobs, System.units));

Menu.system.initialize();
