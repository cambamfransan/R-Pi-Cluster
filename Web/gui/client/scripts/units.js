// ------------------------------------------------------------------
//
// This is the unit handler used to track units.
//
// ------------------------------------------------------------------
System.units = (function() {
	'use strict';

	var units = [];

	function Units() {
		var that = {};
		var unit = { 
			name: null, 
			ip: null, 
			status: null, 
			jobs: [], 
			cores: 0 
		};
		that.add = function() {
			// Add a unit to the table.
		};
		that.remove = function() {
			// Remove a unit from the table.
		};
		that.sort = function(category) {
			// Sort table by specified category.
		};
		return that;
	}

	return {
		Units : Units 
	};
}());