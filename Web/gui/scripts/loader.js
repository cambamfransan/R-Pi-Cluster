// ------------------------------------------------------------------
//
// These are system functions used throughout the menu, jobs, and units.
//
// ------------------------------------------------------------------
var System = {};
var Menu = { screens : {} };

// The current and next scripts to be loaded and executed.
var currentScript;
var nextScript;

// Loads and inserts source code for popups from an external file.
// Runs the menu script to begin sequence of system script loads.
function loadMenu() {
	var popup = document.getElementById("popup");
	document.getElementById("popup-screen").innerHTML = popup.contentDocument.body.innerHTML;
	currentScript = document.createElement('script');
	currentScript.src = 'gui/scripts/menu.js';
	nextScript = 'jobs';
	document.head.appendChild(currentScript);
}

// Loads and runs the correct sequence of system scripts.
function loadSystem() {
	if (nextScript == 'jobs') {
		currentScript = document.createElement('script');
		currentScript.src = 'gui/scripts/jobs.js';
		nextScript = 'units';
		document.head.appendChild(currentScript);
	}
	else if (nextScript == 'units') {
		currentScript = document.createElement('script');
		currentScript.src = 'gui/scripts/units.js';
		nextScript = 'system';
		document.head.appendChild(currentScript);
	}
	else if (nextScript == 'system') {
		currentScript = document.createElement('script');
		currentScript.src = 'gui/scripts/system.js';
		nextScript = null;
		document.head.appendChild(currentScript);
	}
}

// Loads and inserts source code for help menu from an external file.
function loadHelp() {
	document.getElementById("help-screen").innerHTML = help.contentDocument.body.innerHTML;
	var helpTable = help.getElementsByClassName('table')[0];
	var helpLinks = help.getElementsByClassName('link');
	for (var index = 0; index < helpLinks.length; index++) {
		var helpLinkID = helpLinks[index].id;
		document.getElementById(helpLinkID).addEventListener('mouseover', function () {
			document.getElementById('help').style.overflow = 'hidden';
			help.getElementsByClassName('links')[0].style.width = '98.25%';
			index = Number(this.id.slice(-1));
			for (var row = 0; row < helpTable.rows.length; row+=2) {
				if (row == 2*(index-1)) {
					helpTable.rows[row].style.display = '';
					helpTable.rows[row+1].style.display = '';
				}
				else {
					helpTable.rows[row].style.display = 'none';
					helpTable.rows[row+1].style.display = 'none';
				}
			}
		});
		document.getElementById(helpLinkID).addEventListener('mouseout', function () {
			document.getElementById('help').style.overflow = 'auto';
			help.getElementsByClassName('links')[0].style.width = '100%';
			for (var row = 0; row < helpTable.rows.length; row+=2) {
				helpTable.rows[row].style.display = '';
				helpTable.rows[row+1].style.display = '';
			}
		});
	}
}

// Listens for the press of the ENTER key and mouse click.
function addInputListener(e) {
	if (e.which === 13) { // ENTER key
		// Submit data in the add form of job/unit.
		if (menuSystem.jobs)
			document.getElementById('add-job-button').click();
		else if (menuSystem.units)
			document.getElementById('add-unit-button').click();
	}
    else { // Mouse click
		// Hide the error notice for submitting missing/invalid input.
		if (menuSystem.jobs)
			document.getElementById('add-job-error').style.visibility = 'hidden';
		else if (menuSystem.units)
			document.getElementById('add-unit-error').style.visibility = 'hidden';
	}
}

// Check input to verify that characters are valid.
function checkInput(id) {
	if (!document.getElementById(id).validity.valid) {
		var value = document.getElementById(id).value;
		value = value.slice(0,-1);
		document.getElementById(id).value = value;
		return false;
	}
	else
		return true;
}

// Automatically moves input element focus to another input when max/zero characters is typed.
function autoFocusNextInput(current,next,previous,limit) {
	if (limit)
		var maxLength = parseInt(current.attributes["maxlength"].value, 10);
	else
		var maxLength = current.value.length + 1;
	if (maxLength == null)
		maxLength = myLength + 1;
	var myLength = current.value.length;
    if (myLength >= maxLength) {
		var newNext = current;
        while (newNext = next) {
            if (newNext == null)
                break;
            if (newNext.tagName.toLowerCase() === "input") {
                newNext.focus();
                break;
            }
        }
	}
	else if (myLength === 0) {
		// Move to previous field if empty (user pressed backspace)
		if (previous) {
			var newPrevious = current;
			while (newPrevious = previous) {
				if (newPrevious == null)
					break;
				if (newPrevious.tagName.toLowerCase() === "input") {
					newPrevious.focus();
					break;
				}
			}
		}
	}
}

// Draws all pop-up windows to the center of region.
function drawCenter(all) {
	var regionLeft = document.getElementById('menu').offsetWidth;
	var regionTop = document.getElementById('corner').offsetHeight;
	var regionWidth = document.getElementById('frame').offsetWidth - regionLeft;
	var regionHeight = document.getElementById('frame').offsetHeight - regionTop;
	var popups = document.getElementById('hide-screen');
	popups = popups.getElementsByClassName('popup');
	if (all) { // Draw all pop-up windows to the center.
		for (var index = 0; index < popups.length; index++) {
			var id = popups[index].id.toString();
			var elementWidth = document.getElementById(id).offsetWidth;
			var elementHeight = document.getElementById(id).offsetHeight;
			var elementLeft = (regionLeft + (regionWidth/2)) - (elementWidth/2);
			var elementTop = (regionTop + (regionHeight/2)) - (elementHeight/2);
			document.getElementById(id).style.left = elementLeft.toString() + 'px';
			document.getElementById(id).style.top = elementTop.toString() + 'px';
		}
	}
	else { // Draw only the view pop-up window to the center.
		var id = 'view-popup';
		var elementWidth = document.getElementById(id).offsetWidth;
		var elementHeight = document.getElementById(id).offsetHeight;
		var elementLeft = (regionLeft + (regionWidth/2)) - (elementWidth/2);
		var elementTop = (regionTop + (regionHeight/2)) - (elementHeight/2);
		document.getElementById(id).style.left = elementLeft.toString() + 'px';
		document.getElementById(id).style.top = elementTop.toString() + 'px';
	}
}

// Sort table rows by the selected column.
function sortTable(columnIndex,columnType) {
	var table, rows, switching, rowIndex, rowX, rowY, shouldSwitch, direction, switchcount = 0;
	if (menuSystem.jobs)
		table = jobsTable;
	else if (menuSystem.units)
		table = unitsTable;
	switching = true;
	direction = "a2z";
	while (switching) {
	  	switching = false;
		rows = table.getElementsByTagName("tr");
		for (rowIndex = 1; rowIndex < (rows.length-1); rowIndex++) {
			shouldSwitch = false;
			rowX = rows[rowIndex].getElementsByTagName("td")[columnIndex];
			rowY = rows[rowIndex + 1].getElementsByTagName("td")[columnIndex];
			if (direction == "a2z") {
				if (columnType == 'text') {
					if (rowX.innerText.toLowerCase() > rowY.innerText.toLowerCase()) {
						shouldSwitch= true;
						break;
					}
				}
				else if (columnType == 'number') {
					if (parseInt(rowX.textContent) > parseInt(rowY.textContent)) {
						shouldSwitch= true;
						break;
					}
				}
			}
			else if (direction == "z2a") {
				if (columnType == 'text') {
					if (rowX.innerText.toLowerCase() < rowY.innerText.toLowerCase()) {
						shouldSwitch= true;
						break;
					}
				}
				else if (columnType == 'number') {
					if (parseInt(rowX.textContent) < parseInt(rowY.textContent)) {
						shouldSwitch= true;
						break;
					}
				}
			}
		}
		if (shouldSwitch) {
			rows[rowIndex].parentNode.insertBefore(rows[rowIndex + 1], rows[rowIndex]);
			switching = true;
			switchcount ++;
		}
		else {
			if (switchcount == 0 && direction == "a2z") {
				direction = "z2a";
				switching = true;
			}
		}
	}
}

// Update the color of the selected load/progress bar.
function updateBar(id,type) {
	var color1 = '#3F7123'; // green
	var color2 = '#D6A211'; // yellow
	var color3 = '#FF4C0F'; // orange
	var color4 = '#FF0F13'; // red
	if (type == 'load') {
		var percentage = parseInt(document.getElementById(id).style.width);
		if (percentage >= 0 && percentage < 25)
			document.getElementById(id).style.backgroundColor = color1;
		else if (percentage >= 25 && percentage < 50)
			document.getElementById(id).style.backgroundColor = color2;
		else if (percentage >= 50 && percentage < 75)
			document.getElementById(id).style.backgroundColor = color3;
		else if (percentage >= 75 && percentage <= 100)
			document.getElementById(id).style.backgroundColor = color4;
	}
	else if (type == 'progress') {
		var percentage = parseInt(document.getElementById(id).style.width);
		if (percentage >= 0 && percentage < 25)
			document.getElementById(id).style.backgroundColor = color4;
		else if (percentage >= 25 && percentage < 50)
			document.getElementById(id).style.backgroundColor = color3;
		else if (percentage >= 50 && percentage < 75)
			document.getElementById(id).style.backgroundColor = color2;
		else if (percentage >= 75 && percentage <= 100)
			document.getElementById(id).style.backgroundColor = color1;
	}	
}