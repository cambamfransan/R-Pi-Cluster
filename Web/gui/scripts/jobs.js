// ------------------------------------------------------------------
//
// This is the job handler used to track jobs.
//
// ------------------------------------------------------------------
System.jobs = (function() {
	'use strict';
	var imageID; var inputID;

	function saveListener(e) {
		if (e.which === 13) // ENTER key
			document.getElementById(imageID).click();
		else if (e.target.id != imageID && e.target.id != inputID)
			document.getElementById(imageID).click();
	}

	function Jobs() {
		var that = {};
		var IDs = {
			active: [], 
			reuse: [] 
		};
		that.add = function(job) {
			// Add a job to the interface table.
			var jobIndex = job.JobId;
			var jobRow = jobsBody.insertRow(0);
			jobRow.id = 'jobRow' + jobIndex;
			// Create table cell for job checkbox.
			var jobCheckbox  = jobRow.insertCell(0);
			var jobCheckboxID = 'jobCheckbox' + jobIndex;
			jobCheckbox.innerHTML = '<input type="checkbox" id="' + jobCheckboxID + '" class="check-box">';
			document.getElementById(jobCheckboxID).addEventListener('click', function () {
				if (!document.getElementById(jobCheckboxID).checked)
					document.getElementById('checkAllJobs').checked = false;
			});
			// Create table cell for job name.
			var jobName  = jobRow.insertCell(1);
			// var jobNameValue = document.getElementById('job-name').value;
			var jobNameValue = job.name;
			jobName.id = 'jobNameCell'+ jobIndex;
			var jobNameID = 'jobName' + jobIndex;
			var jobNameIconID = 'jobNameIcon' + jobIndex;
			var jobNameTextID = 'jobNameText' + jobIndex;
			var jobNameSave = false;
			jobName.innerHTML = '<dt id="' + jobNameID + '">' + jobNameValue + '</dt><input type="search" id="' + jobNameTextID + '"><img src="gui/images/edit_mouseout.svg" id="' + jobNameIconID + '" class="icon3">';
			document.getElementById(jobNameID).style.cssFloat = 'left';
			document.getElementById(jobNameID).style.marginLeft = '3px';
			document.getElementById(jobNameID).style.marginTop = '2px';
			document.getElementById(jobNameID).style.maxWidth = '140px';
			document.getElementById(jobNameID).style.overflow = 'hidden';
			document.getElementById(jobNameID).style.textOverflow = 'ellipsis';
			document.getElementById(jobNameID).style.whiteSpace = 'nowrap';
			document.getElementById(jobNameTextID).style.cssFloat = 'left';
			document.getElementById(jobNameTextID).style.marginLeft = '3px';
			document.getElementById(jobNameTextID).style.display = 'none';
			document.getElementById(jobNameTextID).style.width = '135px';
			document.getElementById(jobNameTextID).style.textAlign = 'left';
			document.getElementById(jobNameTextID).maxLength = '20';
			document.getElementById(jobNameTextID).pattern = '[a-zA-Z0-9]+';
			document.getElementById(jobNameIconID).style.marginRight = '3px';
			document.getElementById(jobNameIconID).style.cssFloat = 'right';
			document.getElementById(jobNameIconID).style.visibility = 'hidden';
			document.getElementById(jobName.id).addEventListener('mouseover', function () {
				if (!jobNameSave)
					document.getElementById(jobNameIconID).style.visibility = 'visible';
			});
			document.getElementById(jobName.id).addEventListener('mouseout', function () {
				if (!jobNameSave)
					document.getElementById(jobNameIconID).style.visibility = 'hidden';
			});
			document.getElementById(jobNameIconID).addEventListener('mouseover', function () {
				if (!jobNameSave)
					document.getElementById(jobNameIconID).src = menuImages.edit.image.mouseover.src;
				else
					document.getElementById(jobNameIconID).src = menuImages.save.image.mouseover.src;
				document.getElementById(jobNameIconID).style.cursor = 'pointer';
			});
			document.getElementById(jobNameIconID).addEventListener('mouseout', function () {
				if (!jobNameSave)
					document.getElementById(jobNameIconID).src = menuImages.edit.image.mouseout.src;
				else
					document.getElementById(jobNameIconID).src = menuImages.save.image.mouseout.src;
			});
			document.getElementById(jobNameIconID).addEventListener('click', function () {
				if (jobPrioritySave)
					document.getElementById(imageID).click();
				if (!jobNameSave) {
					imageID = jobNameIconID; inputID = jobNameTextID;
					document.addEventListener('keydown', saveListener);
					document.addEventListener('click', saveListener);
					jobNameSave = true;
					document.getElementById(jobNameTextID).value = jobNameValue;
					document.getElementById(jobNameID).style.display = 'none';
					document.getElementById(jobNameTextID).style.display = 'initial';
					document.getElementById(jobNameIconID).style.visibility = 'visible';
					document.getElementById(jobNameIconID).src = menuImages.save.image.mouseout.src;
				}
				else {
					imageID = null; inputID = null;
					document.removeEventListener('keydown', saveListener);
					document.removeEventListener('click', saveListener);
					jobNameSave = false;
					jobNameValue = document.getElementById(jobNameTextID).value;
					document.getElementById(jobNameID).innerText = jobNameValue;
					document.getElementById(jobNameID).style.display = 'initial';
					document.getElementById(jobNameTextID).style.display = 'none';
					document.getElementById(jobNameIconID).style.visibility = 'visible';
					document.getElementById(jobNameIconID).src = menuImages.edit.image.mouseout.src;
				}
			});
			document.getElementById(jobNameTextID).addEventListener('input', function () {
				if (!document.getElementById(jobNameTextID).validity.valid) {
					var value = document.getElementById(jobNameTextID).value;
					value = value.slice(0,-1);
					document.getElementById(jobNameTextID).value = value;
				}
			});
			// Create table cell for job source.
			var jobSource  = jobRow.insertCell(2);
			var jobSourceValue;
			jobSourceValue = job.remote;
			// if (document.getElementById('remote-source').checked) {
			// 	jobSourceValue = document.getElementById('job-remote').value;
			// 	if (document.getElementById('job-protocol').value == 'http')
			// 		jobSourceValue = 'http://' + jobSourceValue;
			// 	else if (document.getElementById('job-protocol').value == 'https')
			// 		jobSourceValue = 'https://' + jobSourceValue;
			// }
			// else if (document.getElementById('local-source').checked)
			// 	jobSourceValue = document.getElementById('job-local').innerText;
			var jobSourceID = 'jobSource' + jobIndex;
			jobSource.innerHTML = '<dt id="' + jobSourceID + '" class="tooltip">' + jobSourceValue + '<span class="tooltip-text">' + jobSourceValue + '</span></dt>';
			document.getElementById(jobSourceID).style.marginLeft = '3px';
			document.getElementById(jobSourceID).style.maxWidth = '55px';
			document.getElementById(jobSourceID).style.verticalAlign = 'middle';
			document.getElementById(jobSourceID).style.textOverflow = 'ellipsis';
			document.getElementById(jobSourceID).style.whiteSpace = 'nowrap';
			document.getElementById(jobSourceID).style.overflow = 'hidden';
			document.getElementById(jobSourceID).addEventListener('mouseover', function () {
				document.getElementById(jobSourceID).style.overflow = 'visible';
			});
			document.getElementById(jobSourceID).addEventListener('mouseout', function () {
				document.getElementById(jobSourceID).style.overflow = 'hidden';
			});
			// Create table cell for job priority.
			var jobPriority  = jobRow.insertCell(3);
			var jobPriorityValue = job.priority;
			// var jobPriorityValue = Number(document.getElementById('job-priority').value);
			jobPriority.id = 'jobPriorityCell'+ jobIndex;
			var jobPriorityID = 'jobPriority' + jobIndex;
			var jobPriorityIconID = 'jobPriorityIcon' + jobIndex;
			var jobPriorityNumberID = 'jobPriorityNumber' + jobIndex;
			var jobPrioritySave = false;
			jobPriority.innerHTML = '<dt id="' + jobPriorityID + '">' + jobPriorityValue + '</dt><input type="number" id="' + jobPriorityNumberID + '"><img src="gui/images/push_mouseout.svg" id="' + jobPriorityIconID + '" class="icon3">';
			document.getElementById(jobPriority.id).style.position = 'relative';
			document.getElementById(jobPriorityID).style.display = 'inline';
			document.getElementById(jobPriorityNumberID).style.cssFloat = 'left';
			document.getElementById(jobPriorityNumberID).style.marginLeft = '3px';
			document.getElementById(jobPriorityNumberID).style.display = 'none';
			document.getElementById(jobPriorityNumberID).style.width = '38px';
			document.getElementById(jobPriorityNumberID).style.textAlign = 'center';
			document.getElementById(jobPriorityNumberID).min = '1';
			document.getElementById(jobPriorityNumberID).max = '99';
			document.getElementById(jobPriorityNumberID).pattern = '[0-9]+';
			document.getElementById(jobPriorityIconID).style.position = 'absolute';
			document.getElementById(jobPriorityIconID).style.right = '0px';
			document.getElementById(jobPriorityIconID).style.visibility = 'hidden';
			document.getElementById(jobPriority.id).addEventListener('mouseover', function () {
				if (!jobPrioritySave)
					document.getElementById(jobPriorityIconID).style.visibility = 'visible';
			});
			document.getElementById(jobPriority.id).addEventListener('mouseout', function () {
				if (!jobPrioritySave)
					document.getElementById(jobPriorityIconID).style.visibility = 'hidden';
			});
			document.getElementById(jobPriorityIconID).addEventListener('mouseover', function () {
				if (!jobPrioritySave)
					document.getElementById(jobPriorityIconID).src = menuImages.push.image.mouseover.src;
				else
					document.getElementById(jobPriorityIconID).src = menuImages.save.image.mouseover.src;
				document.getElementById(jobPriorityIconID).style.cursor = 'pointer';
			});
			document.getElementById(jobPriorityIconID).addEventListener('mouseout', function () {
				if (!jobPrioritySave)
					document.getElementById(jobPriorityIconID).src = menuImages.push.image.mouseout.src;
				else
					document.getElementById(jobPriorityIconID).src = menuImages.save.image.mouseout.src;
			});
			document.getElementById(jobPriorityIconID).addEventListener('click', function () {
				if (jobNameSave)
					document.getElementById(imageID).click();
				if (!jobPrioritySave) {
					imageID = jobPriorityIconID; inputID = jobPriorityNumberID;
					document.addEventListener('keydown', saveListener);
					document.addEventListener('click', saveListener);
					jobPrioritySave = true;
					document.getElementById(jobPriorityNumberID).value = jobPriorityValue;
					document.getElementById(jobPriorityID).style.display = 'none';
					document.getElementById(jobPriorityNumberID).style.display = 'initial';
					document.getElementById(jobPriorityIconID).style.visibility = 'visible';
					document.getElementById(jobPriorityIconID).src = menuImages.save.image.mouseout.src;
					document.getElementById(jobPriorityIconID).style.right = '3px';
				}
				else {
					imageID = null; inputID = null;
					document.removeEventListener('keydown', saveListener);
					document.removeEventListener('click', saveListener);
					jobPrioritySave = false;
					jobPriorityValue = document.getElementById(jobPriorityNumberID).value;
					document.getElementById(jobPriorityID).innerText = jobPriorityValue;
					document.getElementById(jobPriorityID).style.display = 'initial';
					document.getElementById(jobPriorityNumberID).style.display = 'none';
					document.getElementById(jobPriorityIconID).style.visibility = 'visible';
					document.getElementById(jobPriorityIconID).src = menuImages.push.image.mouseout.src;
					document.getElementById(jobPriorityIconID).style.right = '0px';
				}
			});
			document.getElementById(jobPriorityNumberID).addEventListener('input', function () {
				if (!document.getElementById(jobPriorityNumberID).validity.valid) {
					var value = document.getElementById(jobPriorityNumberID).value;
					value = value.slice(0,-1);
					document.getElementById(jobPriorityNumberID).value = value;
				}
			});
			document.getElementById(jobPriorityNumberID).addEventListener('change', function () {
				var value = Number(document.getElementById(jobPriorityNumberID).value);
				var min = Number(document.getElementById(jobPriorityNumberID).min);
				var max = Number(document.getElementById(jobPriorityNumberID).max);
				if (value < min)
					document.getElementById(jobPriorityNumberID).value = min;
				else if (value > max)
					document.getElementById(jobPriorityNumberID).value = max;
			});
			// Create table cell for job status.
			var jobStatus  = jobRow.insertCell(4);
			var jobStatusID = 'jobStatus' + jobIndex;
			jobStatus.innerHTML = '<dt id="' + jobStatusID + '">Active</dt>';
			// Create table cell for job progress.
			var jobProgress  = jobRow.insertCell(5);
			var jobProgressID = 'jobProgress' + jobIndex;
      var jobProgressIDBar = 'jobProgressBar' + jobIndex;
			// will this work?
			jobProgress.innerHTML = '<div class="main-progress"><div class="main-bar" id="' + jobProgressIDBar + '"><div id="' + jobProgressID + '" class="main-percentage">0%</div></div></div>';
			// Create table cell for job controls.
			var jobControls  = jobRow.insertCell(6);
			var jobControl = {
				play: true,
				pause: false,
				stop: false
			};
			var jobPlayID = 'jobPlay' + jobIndex;
			var jobPauseID = 'jobPause' + jobIndex;
			var jobStopID = 'jobStop' + jobIndex;
			jobControls.innerHTML = '<img src="gui/images/play_mouseover.svg" id="' + jobPlayID + '" class="icon2" title="Run Job"><img src="gui/images/pause_mouseout.svg" id="' + jobPauseID + '" class="icon2" title="Halt Job"><img src="gui/images/stop_mouseout.svg" id="' + jobStopID + '" class="icon2" title="Clear Job">';
			document.getElementById(jobPlayID).addEventListener('mouseover', function () {
				if (!jobControl.play)
					document.getElementById(jobPlayID).src = menuImages.play.image.mouseover.src;
			});
			document.getElementById(jobPlayID).addEventListener('mouseout', function () {
				if (!jobControl.play)
					document.getElementById(jobPlayID).src = menuImages.play.image.mouseout.src;
			});
			document.getElementById(jobPlayID).addEventListener('click', function () {
				if (!jobControl.play) {
					jobControl.play = true; jobControl.pause = false; jobControl.stop = false;
					document.getElementById(jobPlayID).src = menuImages.play.image.mouseover.src;
					document.getElementById(jobPauseID).src = menuImages.pause.image.mouseout.src;
					document.getElementById(jobStopID).src = menuImages.stop.image.mouseout.src;
					// ! Run Job
				}
			});
			document.getElementById(jobPauseID).addEventListener('mouseover', function () {
				if (!jobControl.pause)
					document.getElementById(jobPauseID).src = menuImages.pause.image.mouseover.src;
			});
			document.getElementById(jobPauseID).addEventListener('mouseout', function () {
				if (!jobControl.pause)
					document.getElementById(jobPauseID).src = menuImages.pause.image.mouseout.src;
			});
			document.getElementById(jobPauseID).addEventListener('click', function () {
				if (!jobControl.pause) {
					jobControl.play = false; jobControl.pause = true; jobControl.stop = false;
					document.getElementById(jobPlayID).src = menuImages.play.image.mouseout.src;
					document.getElementById(jobPauseID).src = menuImages.pause.image.mouseover.src;
					document.getElementById(jobStopID).src = menuImages.stop.image.mouseout.src;
					// ! Halt Job
				}
			});
			document.getElementById(jobStopID).addEventListener('mouseover', function () {
				if (!jobControl.stop)
					document.getElementById(jobStopID).src = menuImages.stop.image.mouseover.src;
			});
			document.getElementById(jobStopID).addEventListener('mouseout', function () {
				if (!jobControl.stop)
					document.getElementById(jobStopID).src = menuImages.stop.image.mouseout.src;
			});
			document.getElementById(jobStopID).addEventListener('click', function () {
				if (!jobControl.stop) {
					jobControl.play = false; jobControl.pause = false; jobControl.stop = true;
					document.getElementById(jobPlayID).src = menuImages.play.image.mouseout.src;
					document.getElementById(jobPauseID).src = menuImages.pause.image.mouseout.src;
					document.getElementById(jobStopID).src = menuImages.stop.image.mouseover.src;
					// ! Stop Job
				}
			});
			// Create table cell for job results.
			var jobResults  = jobRow.insertCell(7);
			var jobResultsID = 'jobResults' + jobIndex;
			jobResults.innerHTML = '<dt id="' + jobResultsID + '">None</dt>';
			// Create table cell for job units.
			var jobUnits  = jobRow.insertCell(8);
			var jobUnitsID = 'jobUnits' + jobIndex;
			jobUnits.innerHTML = '<button id="' + jobUnitsID + '" class="small"><dt class="text4">View</dt></button>';
			document.getElementById(jobUnitsID).addEventListener('click', function () {
				// ! Load list of units that job is running on from cluster.
				// Show tooltip popup of list of units that job is running on.
			});
			// Create table cell for job tpb.
			var jobTPB = jobRow.insertCell(9);
			var jobTPBValue = job.taskPerBundle;
			// var jobTPBValue = Number(document.getElementById('job-tpb').value);
			var jobTPBID = 'jobTPB' + jobIndex;
			jobTPB.innerHTML = '<input type="range" id="' + jobTPBID + '" class="tpb-field">';
			document.getElementById(jobTPBID).min = '1';
			document.getElementById(jobTPBID).max = '9';
			document.getElementById(jobTPBID).value = jobTPBValue;
			document.getElementById(jobTPBID).addEventListener('input', function () {
				// ! Update tasks per bundle of job in the cluster.
			});
			Menu.system.empty('job');			
		};

		System.socket.on('AddJobAck', function(data) {
			that.add(data);
		});

		that.remove = function() {
			// Remove a job(s) from the interface table.
			document.removeEventListener('keydown', saveListener);
			document.removeEventListener('click', saveListener);
			for (var index = 0; index < jobsBody.rows.length; index++) {
				var row = jobsBody.rows[index];
				var checkbox = row.cells[0].getElementsByTagName('input')[0];
				if (checkbox.type == 'checkbox' && checkbox.checked) {
					var jobNumber = row.id.split('jobRow');
					jobNumber = Number(jobNumber[1]);
					for (var item = 0; item < IDs.active.length; item++) {
						if (jobNumber == IDs.active[item]) {
							IDs.active.splice(item,1);
							item = IDs.active.length;
						}
					}
					IDs.reuse.push(jobNumber);
					jobsBody.deleteRow(index);
					// ! Remove checked job from the cluster.
					index--;
				}
			}
		};
		that.all = function() {
			// Check or uncheck all jobs in the table.
			for (var index = 0; index < jobsBody.rows.length; index++) {
				var row = jobsBody.rows[index];
				var checkbox = row.cells[0].getElementsByTagName('input')[0];
				if (document.getElementById('checkAllJobs').checked) {
					if (checkbox.type == 'checkbox')
						checkbox.checked = true;
				}
				else {
					if (checkbox.type == 'checkbox')
						checkbox.checked = false;
				}
			}
		};
    // map: ids: progress
    that.updateFromServer = function(data) {
      for(let i = 0; i < data.length; i++){
          document.getElementById('jobProgress' + data[i].JobId).innerText = data[i].progress + '%';
          document.getElementById('jobProgressBar' + data[i].JobId).style.width = data[i].progress + '%';
      }
    }
		that.update = function() {
			// Check for job status and progress updates.
				// Status = ( Idle, Active, Halted, Done )
				// Progress = 0% -> 100%
			for (var index = 0; index < jobsBody.rows.length; index++) {
				// ! Load status of job in cluster.
				// Load last known status of job in table.
				// If status of job in cluster is different from it's status in the table...
					// Then change job status in table to match job status from cluster.
				// ! Load progress percentage completed of job from cluster.
				// Update job progress bar width and text label of job in table.
				// If progress of job in cluster is 100%...
					// Then change job status in table to 'Done'...
					// And display 'Ready' button in job results column in table.
						// Attach click event listerner to 'Ready' button.
			}
		};

		System.socket.on('RequestCurrentJobsAck', function(data) {
      for(var key in data) {
        if(data.hasOwnProperty(key)){
			    that.add(data[key]);
        }
      }
		});
    System.socket.emit('systemData', {MsgType:'RequestCurrentJobs'});

    System.socket.on('jobsProgress', function(data) {
      that.updateFromServer(data);
    });

		return that;
	}

	return {
		Jobs : Jobs 
	};
}(System));
