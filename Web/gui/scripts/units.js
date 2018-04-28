// ------------------------------------------------------------------
//
// This is the unit handler used to track units.
//
// ------------------------------------------------------------------
System.units = (function() {
	'use strict';
	var imageID; var inputID;

	function saveListener(e) {
		if (e.which === 13) // ENTER key
			document.getElementById(imageID).click();
		else if (e.target.id != imageID && e.target.id != inputID)
			document.getElementById(imageID).click();
	}

	function Units() {
		var that = {};
		var IDs = {
			active: [], 
			reuse: [] 
		};
		that.index = function() {
			// Assign an index value to the unit.
			var unitIndex;
			if (IDs.reuse.length == 0) {
				if (IDs.active.length == 0)
					unitIndex = unitsBody.rows.length.toString();
				else {
					if (IDs.active.length >= 2) {
						IDs.active.sort(function(a,b){return a-b});
						for (var index = 0; index < IDs.active.length-1; index++) {
							if (IDs.active[index+1] - IDs.active[index] >= 2)
								unitIndex = (IDs.active[index] + 1).toString();
						}
					}
					else
						unitIndex = (IDs.active[0] + 1).toString();
				}
				if (unitIndex == null)
					unitIndex = (IDs.active[IDs.active.length-1] + 1).toString();
				IDs.active.push(Number(unitIndex));
			}
			else {
				IDs.reuse.sort(function(a,b){return a-b});
				unitIndex = IDs.reuse[0].toString();
				IDs.reuse.splice(0,1);
				IDs.active.push(Number(unitIndex));
			}
			return unitIndex;
		}
		that.add = function() {
			// Add a unit to the interface table.
			var unitIndex = that.index();
			var unitRow = unitsBody.insertRow(0);
			unitRow.id = 'unitRow' + unitIndex;
			// Create table cell for unit checkbox.
			var unitCheckbox  = unitRow.insertCell(0);
			var unitCheckboxID = 'unitCheckbox' + unitIndex;
			unitCheckbox.innerHTML = '<input id="' + unitCheckboxID + '" type="checkbox">';
			document.getElementById(unitCheckboxID).addEventListener('click', function () {
				if (!document.getElementById(unitCheckboxID).checked)
					document.getElementById('checkAllUnits').checked = false;
			});
			// Create table cell for unit name.
			var unitName  = unitRow.insertCell(1);
			var unitNameValue = document.getElementById('unit-name').value;
			unitName.id = 'unitNameCell'+ unitIndex;
			var unitNameID = 'unitName' + unitIndex;
			var unitNameIconID = 'unitNameIcon' + unitIndex;
			var unitNameTextID = 'unitNameText' + unitIndex;
			var unitNameSave = false;
			unitName.innerHTML = '<span id="' + unitNameID + '">' + unitNameValue + '</span><input id="' + unitNameTextID + '" type="search"><img id="' + unitNameIconID + '" class="icon" src="gui/images/edit_mouseout.svg" title="Edit Name">';
			document.getElementById(unitNameID).style.cssFloat = 'left';
			document.getElementById(unitNameID).style.marginLeft = '3px';
			document.getElementById(unitNameID).style.marginTop = '2px';
			document.getElementById(unitNameID).style.maxWidth = '140px';
			document.getElementById(unitNameID).style.overflow = 'hidden';
			document.getElementById(unitNameID).style.textOverflow = 'ellipsis';
			document.getElementById(unitNameID).style.whiteSpace = 'nowrap';
			document.getElementById(unitNameTextID).style.cssFloat = 'left';
			document.getElementById(unitNameTextID).style.marginLeft = '3px';
			document.getElementById(unitNameTextID).style.display = 'none';
			document.getElementById(unitNameTextID).style.width = '135px';
			document.getElementById(unitNameTextID).style.textAlign = 'left';
			document.getElementById(unitNameTextID).maxLength = '20';
			document.getElementById(unitNameTextID).pattern = '[a-zA-Z0-9]+';
			document.getElementById(unitNameIconID).style.marginRight = '3px';
			document.getElementById(unitNameIconID).style.cssFloat = 'right';
			document.getElementById(unitNameIconID).style.visibility = 'hidden';
			document.getElementById(unitName.id).addEventListener('mouseover', function () {
				if (!unitNameSave)
					document.getElementById(unitNameIconID).style.visibility = 'visible';
			});
			document.getElementById(unitName.id).addEventListener('mouseout', function () {
				if (!unitNameSave)
					document.getElementById(unitNameIconID).style.visibility = 'hidden';
			});
			document.getElementById(unitNameIconID).addEventListener('mouseover', function () {
				if (!unitNameSave)
					document.getElementById(unitNameIconID).src = menuImages.edit.image.mouseover.src;
				else
					document.getElementById(unitNameIconID).src = menuImages.save.image.mouseover.src;
				document.getElementById(unitNameIconID).style.cursor = 'pointer';
			});
			document.getElementById(unitNameIconID).addEventListener('mouseout', function () {
				if (!unitNameSave)
					document.getElementById(unitNameIconID).src = menuImages.edit.image.mouseout.src;
				else
					document.getElementById(unitNameIconID).src = menuImages.save.image.mouseout.src;
			});
			document.getElementById(unitNameIconID).addEventListener('click', function () {
				if (unitCoresSave)
					document.getElementById(imageID).click();
				if (!unitNameSave) {
					imageID = unitNameIconID; inputID = unitNameTextID;
					document.addEventListener('keydown', saveListener);
					document.addEventListener('click', saveListener);
					unitNameSave = true;
					document.getElementById(unitNameTextID).value = unitNameValue;
					document.getElementById(unitNameID).style.display = 'none';
					document.getElementById(unitNameTextID).style.display = 'initial';
					document.getElementById(unitNameTextID).focus();
					document.getElementById(unitNameIconID).style.visibility = 'visible';
					document.getElementById(unitNameIconID).src = menuImages.save.image.mouseout.src;
					document.getElementById(unitNameIconID).title = 'Save Name';
				}
				else {
					imageID = null; inputID = null;
					document.removeEventListener('keydown', saveListener);
					document.removeEventListener('click', saveListener);
					unitNameSave = false;
					unitNameValue = document.getElementById(unitNameTextID).value;
					document.getElementById(unitNameID).innerText = unitNameValue;
					document.getElementById(unitNameID).style.display = 'initial';
					document.getElementById(unitNameTextID).style.display = 'none';
					document.getElementById(unitNameIconID).style.visibility = 'hidden';
					document.getElementById(unitNameIconID).src = menuImages.edit.image.mouseout.src;
					document.getElementById(unitNameIconID).title = 'Edit Name';
				}
			});
			document.getElementById(unitNameTextID).addEventListener('input', function () {
				if (!document.getElementById(unitNameTextID).validity.valid) {
					var value = document.getElementById(unitNameTextID).value;
					value = value.slice(0,-1);
					document.getElementById(unitNameTextID).value = value;
				}
			});
			// Create table cell for unit IP address.
			var unitIP  = unitRow.insertCell(2);
			var unitIPValue = document.getElementById('unit-IP1').value + '.' + document.getElementById('unit-IP2').value + '.' + document.getElementById('unit-IP3').value + '.' + document.getElementById('unit-IP4').value;
			var unitIPID = 'unitIP' + unitIndex;
			unitIP.innerHTML = '<span id="' + unitIPID + '">' + unitIPValue + '</span>';
			// Create table cell for unit MAC address.
			var unitMAC  = unitRow.insertCell(3);
				// ! Get the MAC address of the unit.
				var unitMACValue = '<span class="error">XX:XX:XX:XX:XX:XX</span>';
			var unitMACID = 'unitMAC' + unitIndex;
			unitMAC.innerHTML = '<span id="' + unitMACID + '">' + unitMACValue + '</span>';
			// Create table cell for unit status.
			var unitStatus  = unitRow.insertCell(4);
			var unitStatusID = 'unitStatus' + unitIndex;
			unitStatus.innerHTML = '<span id="' + unitStatusID + '"><span class="error">ERROR!</span></span>';
			// Create table cell for unit CPU load.
			var unitCPU  = unitRow.insertCell(5);
			var unitCPUID = 'unitCPU' + unitIndex;
			unitCPU.innerHTML = '<div class="load"><div class="bar"><div id="' + unitCPUID + '" class="percent"><span class="error">ERROR!</span></div></div></div>';
			// Create table cell for unit RAM load.
			var unitRAM  = unitRow.insertCell(6);
			var unitRAMID = 'unitRAM' + unitIndex;
			unitRAM.innerHTML = '<div class="load"><div class="bar"><div id="' + unitRAMID + '" class="percent"><span class="error">ERROR!</span></div></div></div>';
			// Create table cell for unit jobs.
			var unitJobs  = unitRow.insertCell(7);
			var unitJobsID = 'unitJobs' + unitIndex;
			unitJobs.innerHTML = '<button id="' + unitJobsID + '" class="small"><span>View</span></button>';
			document.getElementById(unitJobsID).addEventListener('click', function () {
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
				document.getElementById('view-type').innerText = 'Unit';
				document.getElementById('view-name').innerText = unitNameValue;
				var viewRow; var viewCell; var cellsPerRow = 2;
				for (var index = 0; index < jobsBody.rows.length; index++) {
					var viewPosition = index % cellsPerRow;
					if (viewPosition == 0)
						viewRow = viewBody.insertRow(viewBody.rows.length);
					viewCell = viewRow.insertCell(viewPosition);
					viewCell.innerText = jobsBody.rows[index].cells[1].innerText;
				}
				drawCenter(false);
				document.getElementById('view-popup').style.visibility = 'visible';
			});
			// Create table cell for unit cores limit.
			var unitCores  = unitRow.insertCell(8);
			var unitCoresValue = Number(document.getElementById('unit-cores').value);
			unitCores.id = 'unitCoresCell'+ unitIndex;
			var unitCoresID = 'unitCores' + unitIndex;
			var unitCoresIconID = 'unitCoresIcon' + unitIndex;
			var unitCoresNumberID = 'unitCoresNumber' + unitIndex;
			var unitCoresSave = false;
			unitCores.innerHTML = '<span id="' + unitCoresID + '">' + unitCoresValue + '</span><input id="' + unitCoresNumberID + '" type="number"><img id="' + unitCoresIconID + '" class="icon" src="gui/images/push_mouseout.svg" title="Edit Cores Limit">';
			document.getElementById(unitCores.id).style.position = 'relative';
			document.getElementById(unitCoresID).style.display = 'inline';
			document.getElementById(unitCoresNumberID).style.cssFloat = 'left';
			document.getElementById(unitCoresNumberID).style.marginLeft = '3px';
			document.getElementById(unitCoresNumberID).style.display = 'none';
			document.getElementById(unitCoresNumberID).style.width = '38px';
			document.getElementById(unitCoresNumberID).style.textAlign = 'center';
			document.getElementById(unitCoresNumberID).min = '1';
			document.getElementById(unitCoresNumberID).max = '4';
			document.getElementById(unitCoresNumberID).pattern = '[1-4]+';
			document.getElementById(unitCoresIconID).style.position = 'absolute';
			document.getElementById(unitCoresIconID).style.right = '4px';
			document.getElementById(unitCoresIconID).style.visibility = 'hidden';
			document.getElementById(unitCores.id).addEventListener('mouseover', function () {
				if (!unitCoresSave)
					document.getElementById(unitCoresIconID).style.visibility = 'visible';
			});
			document.getElementById(unitCores.id).addEventListener('mouseout', function () {
				if (!unitCoresSave)
					document.getElementById(unitCoresIconID).style.visibility = 'hidden';
			});
			document.getElementById(unitCoresIconID).addEventListener('mouseover', function () {
				if (!unitCoresSave)
					document.getElementById(unitCoresIconID).src = menuImages.push.image.mouseover.src;
				else
					document.getElementById(unitCoresIconID).src = menuImages.save.image.mouseover.src;
				document.getElementById(unitCoresIconID).style.cursor = 'pointer';
			});
			document.getElementById(unitCoresIconID).addEventListener('mouseout', function () {
				if (!unitCoresSave)
					document.getElementById(unitCoresIconID).src = menuImages.push.image.mouseout.src;
				else
					document.getElementById(unitCoresIconID).src = menuImages.save.image.mouseout.src;
			});
			document.getElementById(unitCoresIconID).addEventListener('click', function () {
				if (unitNameSave)
					document.getElementById(imageID).click();
				if (!unitCoresSave) {
					imageID = unitCoresIconID; inputID = unitCoresNumberID;
					document.addEventListener('keydown', saveListener);
					document.addEventListener('click', saveListener);
					unitCoresSave = true;
					document.getElementById(unitCoresNumberID).value = unitCoresValue;
					document.getElementById(unitCoresID).style.display = 'none';
					document.getElementById(unitCoresNumberID).style.display = 'initial';
					document.getElementById(unitCoresNumberID).focus();
					document.getElementById(unitCoresIconID).style.visibility = 'visible';
					document.getElementById(unitCoresIconID).src = menuImages.save.image.mouseout.src;
					document.getElementById(unitCoresIconID).title = 'Save Cores Limit';
				}
				else {
					imageID = null; inputID = null;
					document.removeEventListener('keydown', saveListener);
					document.removeEventListener('click', saveListener);
					unitCoresSave = false;
					unitCoresValue = document.getElementById(unitCoresNumberID).value;
					document.getElementById(unitCoresID).innerText = unitCoresValue;
					document.getElementById(unitCoresID).style.display = 'initial';
					document.getElementById(unitCoresNumberID).style.display = 'none';
					document.getElementById(unitCoresIconID).style.visibility = 'hidden';
					document.getElementById(unitCoresIconID).src = menuImages.push.image.mouseout.src;
					document.getElementById(unitCoresIconID).title = 'Edit Cores Limit';
				}
			});
			document.getElementById(unitCoresNumberID).addEventListener('input', function () {
				if (!document.getElementById(unitCoresNumberID).validity.valid) {
					var value = document.getElementById(unitCoresNumberID).value;
					value = value.slice(0,-1);
					document.getElementById(unitCoresNumberID).value = value;
				}
			});
		};
		that.remove = function() {
			// Remove a unit(s) from the interface table.
			document.removeEventListener('keydown', saveListener);
			document.removeEventListener('click', saveListener);
			for (var index = 0; index < unitsBody.rows.length; index++) {
				var row = unitsBody.rows[index];
				var checkbox = row.cells[0].getElementsByTagName('input')[0];
				if (checkbox.type == 'checkbox' && checkbox.checked) {
					var unitNumber = row.id.split('unitRow');
					unitNumber = Number(unitNumber[1]);
					for (var item = 0; item < IDs.active.length; item++) {
						if (unitNumber == IDs.active[item]) {
							IDs.active.splice(item,1);
							item = IDs.active.length;
						}
					}
					IDs.reuse.push(unitNumber);
					unitsBody.deleteRow(index);
					// ! Remove checked unit from the cluster.
					index--;
				}
			}
		};
		that.all = function() {
			// Check or uncheck all units in the table.
			for (var index = 0; index < unitsBody.rows.length; index++) {
				var row = unitsBody.rows[index];
				var checkbox = row.cells[0].getElementsByTagName('input')[0];
				if (document.getElementById('checkAllUnits').checked) {
					if (checkbox.type == 'checkbox')
						checkbox.checked = true;
				}
				else {
					if (checkbox.type == 'checkbox')
						checkbox.checked = false;
				}
			}
		};
		return that;
	}

	return {
		Units : Units 
	};
}());

loadSystem();