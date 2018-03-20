socket.on('gameScores', function(data) {
    //console.log('Received Scores from Server');
    if (data !== null) {
        //console.log('Scores is Empty');
        if (JSON.parse(localStorage.reload)) {
            //console.log('Browser Window Reloaded');
            localStorage.reload = false;
        }
        else {
            //console.log('Set Scores to Default');
            localStorage.gameScores = JSON.stringify(data);
            showScores();
        }
        localStorage.emptyScores = false;
    }
    else {
        //console.log('Scores is Not Empty');
        localStorage.emptyScores = true;
        localStorage.reload = false;
    }
});
socket.on('defaultScores', function(data) {
    //console.log('Received Reset from Server');
    if (data == null) {
        localStorage.emptyScores = true;
        Game.main.gameScores('load');
    }
});