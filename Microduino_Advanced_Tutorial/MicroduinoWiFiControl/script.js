function stopRobot(){

	$.get( "update_state.php", { 
		speedLeft: "0", 
		speedRight: "0", 
		directionLeft: "1", 
		directionRight: "1"} );
}

function fullForward(){

	$.get( "update_state.php", { 
		speedLeft: "100", 
		speedRight: "100", 
		directionLeft: "1", 
		directionRight: "1"} );
}

function turnLeft(){

	$.get( "update_state.php", { speedLeft: "100", 
		speedRight: "100", 
		directionLeft: "0", 
		directionRight: "1"} );
}

function turnRight(){

	$.get( "update_state.php", { speedLeft: "100", 
		speedRight: "100", 
		directionLeft: "1", 
		directionRight: "0"} );
}

function sleep(milliseconds) {
  var start = new Date().getTime();
  for (var i = 0; i < 1e7; i++) {
    if ((new Date().getTime() - start) > milliseconds){
      break;
    }
  }
}

