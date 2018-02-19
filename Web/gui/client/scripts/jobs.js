// ------------------------------------------------------------------
//
// This is the job handler used to track jobs.
//
// ------------------------------------------------------------------
System.jobs = (function() {
	'use strict';

	var jobs = [];

	function Jobs() {
		var that = {};
		var job = { 
			name: null, 
			priority: 0, 
			status: 'Running', 
			progress: 0, 
			control: 'Play', 
			units: [], 
			tasks: 0 
		};
		that.add = function() {
			// Add a job to the table.
		};
		that.remove = function() {
			// Remove a job from the table.
		};
		that.sort = function(category) {
			// Sort table by specified category.
		};
		return that;
	}

	return {
		Jobs : Jobs 
	};
}());