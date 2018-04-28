/*
socket.on('systemData', function(data) {
    //console.log('Received Data from Server');
    if (data !== null) {
        //console.log('Data is Empty');
        if (JSON.parse(localStorage.reload)) {
            //console.log('Browser Window Reloaded');
            localStorage.reload = false;
        }
        else {
            //console.log('Set Data to Default');
            localStorage.systemData = JSON.stringify(data);
            showData();
        }
        localStorage.emptyData = false;
    }
    else {
        //console.log('Data is Not Empty');
        localStorage.emptyData = true;
        localStorage.reload = false;
    }
});
socket.on('defaultSystem', function(data) {
    //console.log('Received Reset from Server');
    if (data == null) {
        localStorage.emptyData = true;
        System.main.systemData('load');
    }
});
*/