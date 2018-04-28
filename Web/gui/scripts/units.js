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
		that.add = function(client) {
			// Add a unit to the interface table.
			var unitIndex = client.clientId;
			var unitRow = unitsBody.insertRow(0);
			unitRow.id = 'unitRow' + unitIndex;
			// Create table cell for unit checkbox.
			var unitCheckbox  = unitRow.insertCell(0);
			var unitCheckboxID = 'unitCheckbox' + unitIndex;
			unitCheckbox.innerHTML = '<input type="checkbox" id="' + unitCheckboxID + '" class="check-box">';
			document.getElementById(unitCheckboxID).addEventListener('click', function () {
				if (!document.getElementById(unitCheckboxID).checked)
					document.getElementById('checkAllUnits').checked = false;
			});
			// Create table cell for unit name.
			var unitName  = unitRow.insertCell(1);
			var unitNameValue = unitIndex;
			unitName.id = 'unitNameCell'+ unitIndex;
			var unitNameID = 'unitName' + unitIndex;
			var unitNameIconID = 'unitNameIcon' + unitIndex;
			var unitNameTextID = 'unitNameText' + unitIndex;
			var unitNameSave = false;
			unitName.innerHTML = '<dt id="' + unitNameID + '">' + unitNameValue + '</dt><input type="search" id="' + unitNameTextID + '"><img src="gui/images/edit_mouseout.svg" id="' + unitNameIconID + '" class="icon3">';
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
					document.getElementById(unitNameIconID).style.visibility = 'visible';
					document.getElementById(unitNameIconID).src = menuImages.save.image.mouseout.src;
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
					document.getElementById(unitNameIconID).style.visibility = 'visible';
					document.getElementById(unitNameIconID).src = menuImages.edit.image.mouseout.src;
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
			var unitIPValue = client.ipaddress;
			var unitIPID = 'unitIP' + unitIndex;
			unitIP.innerHTML = '<dt id="' + unitIPID + '">' + unitIPValue + '</dt>';
			// Create table cell for unit MAC address.
			var unitMAC  = unitRow.insertCell(3);
				// ! Get the MAC address of the unit.
				var unitMACValue = 'XX:XX:XX:XX:XX:XX';
			var unitMACID = 'unitMAC' + unitIndex;
			unitMAC.innerHTML = '<dt id="' + unitMACID + '">' + unitMACValue + '</dt>';
			// Create table cell for unit status.
			var unitStatus  = unitRow.insertCell(4);
			var unitStatusID = 'unitStatus' + unitIndex;
			unitStatus.innerHTML = '<dt id="' + unitStatusID + '">Online</dt>';
			// Create table cell for unit CPU load.
			var unitCPU  = unitRow.insertCell(5);
			var unitCPUID = 'unitCPU' + unitIndex;
			unitCPU.innerHTML = '<div class="main-progress"><div class="main-bar"><div id="' + unitCPUID + '" class="main-percentage">50%</div></div></div>';
			// Create table cell for unit RAM load.
			var unitRAM  = unitRow.insertCell(6);
			var unitRAMID = 'unitRAM' + unitIndex;
			unitRAM.innerHTML = '<div class="main-progress"><div class="main-bar"><div id="' + unitRAMID + '" class="main-percentage">50%</div></div></div>';
			// Create table cell for unit jobs.
			var unitJobs  = unitRow.insertCell(7);
			var unitJobsID = 'unitJobs' + unitIndex;
			unitJobs.innerHTML = '<button id="' + unitJobsID + '" class="small"><dt class="text4">View</dt></button>';
			document.getElementById(unitJobsID).addEventListener('click', function () {
				// ! Show popup of list of jobs that unit is executing.

				// ! Load list of jobs that unit is executing on cluster.
				// Show tooltip popup of list of jobs that unit is executing for.
			});
			// Create table cell for unit cores limit.
			var unitCores  = unitRow.insertCell(8);
			var unitCoresValue = Number(document.getElementById('unit-cores').value);
			unitCores.id = 'unitCoresCell'+ unitIndex;
			var unitCoresID = 'unitCores' + unitIndex;
			var unitCoresIconID = 'unitCoresIcon' + unitIndex;
			var unitCoresNumberID = 'unitCoresNumber' + unitIndex;
			var unitCoresSave = false;
			unitCores.innerHTML = '<dt id="' + unitCoresID + '">' + unitCoresValue + '</dt><input type="number" id="' + unitCoresNumberID + '"><img src="gui/images/push_mouseout.svg" id="' + unitCoresIconID + '" class="icon3">';
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
					document.getElementById(unitCoresIconID).style.visibility = 'visible';
					document.getElementById(unitCoresIconID).src = menuImages.save.image.mouseout.src;
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
					document.getElementById(unitCoresIconID).style.visibility = 'visible';
					document.getElementById(unitCoresIconID).src = menuImages.push.image.mouseout.src;
				}
			});
			document.getElementById(unitCoresNumberID).addEventListener('input', function () {
				if (!document.getElementById(unitCoresNumberID).validity.valid) {
					var value = document.getElementById(unitCoresNumberID).value;
					value = value.slice(0,-1);
					document.getElementById(unitCoresNumberID).value = value;
				}
			});
      Menu.system.empty('unit');
		};
    System.socket.on('newClient', function(data) {
      that.add(data);
    });
		that.remove = function(id) {
			// Remove a unit(s) from the interface table.
			document.removeEventListener('keydown', saveListener);
			document.removeEventListener('click', saveListener);
      var row = document.getElementById('unitRow' + id);
      row.parentNode.removeChild(row);
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
		that.update = function() {
			// Check for unit status and load updates.
				// Status = ( Online, Offline )
				// Load = 0% -> 100%
			for (var index = 0; index < unitsBody.rows.length; index++) {
				// ! Load status of unit in cluster.
				// Load last known status of unit in table.
				// If status of unit in cluster is different from it's status in the table...
					// Then change unit status in table to match unit status from cluster.
				// ! Load CPU/RAM activity load of unit from cluster.
				// Update unit load bar width and text label of unit in table.
				// If load of unit in cluster is 100%...
					// Do nothing.
			}
		};
    System.socket.emit('systemData', {MsgType: 'RequestCurrentClients'});

    System.socket.on('RequestCurrentClientsAck', function(data) {
      for(var key in data) {
        if(data.hasOwnProperty(key)){
          that.add(data[key]);
        }
      }
    });

    System.socket.on('lostClient', function (data) {
      that.remove(data.clientId);
    });

		return that;
	}

	return {
		Units : Units 
	};
}());

