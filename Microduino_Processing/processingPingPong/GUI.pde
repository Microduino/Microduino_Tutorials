
void showGUI() {

  stroke(150);
  line(width/2, 0, width/2, height);

  textSize(16);
  text("Player 1: " + playerOne, 100, 20); // player 1 score display
  text("Player 2: " + playerTwo, 600, 20);  // play 2 score display

  if (oneWins == true) { //if player one wins
    textSize(24);
    image(pEnd, 0, 0, width, height); //display End Game Screen
    text("Player 1 WINS!!!", width/2 - 95, height/2 - 50);
    //text("Press 'R' for New Game", width/2 - 135, height/2 - 20);
  }

  if (twoWins == true) { //if player two wins
    textSize(24);
    image(pEnd, 0, 0, width, height); //display End Game Screen
    text("Player 2 WINS!!!", width/2 - 95, height/2 - 50);
    //text("Press 'R' for New Game", width/2 - 135, height/2 - 20);
  }
}

