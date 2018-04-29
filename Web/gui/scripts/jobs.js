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
			jobCheckbox.innerHTML = '<input id="' + jobCheckboxID + '" type="checkbox">';
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
			jobName.innerHTML = '<span id="' + jobNameID + '">' + jobNameValue + '</span><input id="' + jobNameTextID + '" type="search"><img id="' + jobNameIconID + '" class="icon" src="gui/images/edit_mouseout.svg" title="Edit Name">';
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
					document.getElementById(jobNameTextID).focus();
					document.getElementById(jobNameIconID).style.visibility = 'visible';
					document.getElementById(jobNameIconID).src = menuImages.save.image.mouseout.src;
					document.getElementById(jobNameIconID).title = 'Save Name';
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
					document.getElementById(jobNameIconID).style.visibility = 'hidden';
					document.getElementById(jobNameIconID).src = menuImages.edit.image.mouseout.src;
					document.getElementById(jobNameIconID).title = 'Edit Name';
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
			var jobSourceValue,
          jobSourceType;
			jobSourceValue = job.remote;
			if(jobSourceValue.startsWith("http"))
				jobSourceType = 'URL';
			else 
				jobSourceType = 'File';
			var jobSourceID = 'jobSource' + jobIndex;
			jobSource.innerHTML = '<span id="' + jobSourceID + '" title="' + jobSourceValue + '">' + jobSourceType + '</span>';
			// Create table cell for job priority.
			var jobPriority  = jobRow.insertCell(3);
			var jobPriorityValue = job.priority;
			// var jobPriorityValue = Number(document.getElementById('job-priority').value);
			jobPriority.id = 'jobPriorityCell'+ jobIndex;
			var jobPriorityID = 'jobPriority' + jobIndex;
			var jobPriorityIconID = 'jobPriorityIcon' + jobIndex;
			var jobPriorityNumberID = 'jobPriorityNumber' + jobIndex;
			var jobPrioritySave = false;
			jobPriority.innerHTML = '<span id="' + jobPriorityID + '">' + jobPriorityValue + '</span><input id="' + jobPriorityNumberID + '" type="number"><img id="' + jobPriorityIconID + '" class="icon" src="gui/images/push_mouseout.svg" title="Edit Priority">';
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
					document.getElementById(jobPriorityNumberID).focus();
					document.getElementById(jobPriorityIconID).style.visibility = 'visible';
					document.getElementById(jobPriorityIconID).src = menuImages.save.image.mouseout.src;
					document.getElementById(jobPriorityIconID).style.right = '3px';
					document.getElementById(jobPriorityIconID).title = 'Save Priority';
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
					document.getElementById(jobPriorityIconID).style.visibility = 'hidden';
					document.getElementById(jobPriorityIconID).src = menuImages.push.image.mouseout.src;
					document.getElementById(jobPriorityIconID).style.right = '0px';
					document.getElementById(jobPriorityIconID).title = 'Edit Priority';
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
			jobProgress.innerHTML = '<div class="progress"><div class="bar" id="' + jobProgressIDBar + '"><div id="' + jobProgressID + '" class="percent">0%</div></div></div>';
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
			jobControls.innerHTML = '<img id="' + jobPlayID + '" src="gui/images/play_mouseover.svg" title="Start Job"><img id="' + jobPauseID + '" src="gui/images/pause_mouseout.svg" title="Pause Job"><img id="' + jobStopID + '" src="gui/images/stop_mouseout.svg" title="Stop Job">';
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
					// ! Pause Job
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
			jobResults.innerHTML = '<span id="' + jobResultsID + '">None</span>';
			document.getElementById(jobStatusID).addEventListener('change', function () {
				if (document.getElementById(jobStatusID).innerText == 'Done') {
					document.getElementById(jobResultsID).remove();
					jobResults.innerHTML = '<button id="' + jobResultsID + '" class="small"><span>Ready</span></button>';
				}
			});
			// Create table cell for job units.
			var jobUnits  = jobRow.insertCell(8);
			var jobUnitsID = 'jobUnits' + jobIndex;
			jobUnits.innerHTML = '<button id="' + jobUnitsID + '" class="small"><span>View</span></button>';
			document.getElementById(jobUnitsID).addEventListener('click', function () {
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
				document.getElementById('view-type').innerText = 'Job';
				document.getElementById('view-name').innerText = jobNameValue;
				var viewRow; var viewCell; var cellsPerRow = 2;
				for (var index = 0; index < unitsBody.rows.length; index++) {
					var viewPosition = index % cellsPerRow;
					if (viewPosition == 0)
						viewRow = viewBody.insertRow(viewBody.rows.length);
					viewCell = viewRow.insertCell(viewPosition);
					viewCell.innerText = unitsBody.rows[index].cells[1].innerText;
				}
				drawCenter(false);
				document.getElementById('view-popup').style.visibility = 'visible';
			});
			// Create table cell for job tpb.
			var jobTPB = jobRow.insertCell(9);
			var jobTPBValue = job.taskPerBundle;
			// var jobTPBValue = Number(document.getElementById('job-tpb').value);
			var jobTPBID = 'jobTPB' + jobIndex;
			jobTPB.innerHTML = '<input id="' + jobTPBID + '" type="range">';
			document.getElementById(jobTPBID).min = '1';
			document.getElementById(jobTPBID).max = '9';
			document.getElementById(jobTPBID).value = jobTPBValue;
			document.getElementById(jobTPBID).addEventListener('input', function () {
				// ! Update tasks per bundle of job in the cluster.
			});
			Menu.system.emptyTable('job');			
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
		//updateBar("jobProgressBar" + data[i].JobId, "progress");
		document.getElementById('jobProgress' + data[i].JobId).innerText = data[i].progress + '%';

		document.getElementById('jobProgressBar' + data[i].JobId).style.width = data[i].progress + '%';
        if(data[i].progress == 100) 
        {
          // Change control images
          document.getElementById('jobStatus'+data[i].JobId).innerText = 'Done';
          // Enable download button
          document.getElementById('jobResults'+data[i].JobId).innerText = 'Ready';
        }
      }
    }

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

loadSystem();
