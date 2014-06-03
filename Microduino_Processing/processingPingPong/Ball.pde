
//class for pong balls
class Ball {

  float bAngle = 0; //added to keep track of ball's current angle
  float bRadius; //add this line to create a vector for the ball

  int bSize;
  float xpos, ypos;
  float xspeed, yspeed;
  float xdir, ydir;

  Ball(int ibSize, float ixpos, float iypos, float ixspeed, float iyspeed, float ixdir, float iydir) {

    bSize = ibSize;
    xpos = ixpos;
    ypos = iypos;
    xspeed = ixspeed;
    yspeed = iyspeed;
    xdir = ixdir;
    ydir = iydir;
  }


  void update() {

    xpos = xpos + (xspeed * xdir);
    ypos = ypos + (yspeed * ydir);

    /* commented out for better hit test
     //if hits paddle, change direction
     if ((xpos > rightPaddle.xPos - bSize && xpos < width - bSize && 
     ypos < rpp + rightPaddle.pHeight/2 - bSize && 
     ypos > rpp - rightPaddle.pHeight/2 - bSize) || 
     (xpos < leftPaddle.xPos + bSize && xpos < 0 + bSize &&
     ypos < lpp + leftPaddle.pHeight/2 - bSize &&
     ypos > lpp - leftPaddle.pHeight/2 - bSize) )
     
     {
     xdir *= -1;  //change ball direction
     }
     */

    //added for new hit test

    calcRadius(); //called to find out current ball vector value

    //here is the right paddle hit test
    rPadHitTest();

    //here is the left paddle hit test
    lPadHitTest();

    if (xpos > width-bSize + 1) { //player one scored
      playerOne++;
      if (playerOne < 7) {
        //println(playerOne);
        reset();
      }
      else if (playerOne == 7) {
        oneWins = true;
        xpos = width/2;
        ypos = height/2;
        xdir = 0;
        ydir = 0;
      }
    }


    if (xpos < 0) { //player two scored

      playerTwo++;

      if (playerTwo < 7) {
        //println(playerTwo);
        reset();
      }
      else if (playerTwo == 7) {
        twoWins = true;
        xpos = width/2;
        ypos = height/2;
        xdir = 0;
        ydir = 0;
      }
    }



    if (ypos > height-bSize || ypos < 0) { //ball hits top or bottom
      ydir *= -1;
    }
  }

  void display() {
    ellipse(xpos+bSize/2, ypos+bSize/2, bSize, bSize);
  }

  void reset() {
    xpos = width/2;
    ypos = height/2;
    ydir = random(-1, 1);
    float dir = random(-1, 1);
    if (dir > 0) {
      xdir = 1;
    }
    else if (dir <= 0) {
      xdir = -1;
    }
    pong.xspeed = 8; //reset ball speed
    pong.yspeed = 2; //reset ball speed
    timePlayed = 0;  //reset time played counter
    bRadius = 8.25;  //reset ball vector
  }

  void keyPressed() {

    if (keyCode=='R') {
      newGame();
    }

    if (keyCode=='1') {
      bSize++;
    }
    if (keyCode=='2') {
      bSize--;
    }
    if (keyCode=='3') {
      leftPaddle.pHeight++;
    }    
    if (keyCode=='4') {
      leftPaddle.pHeight--;
    }
    if (keyCode=='5') {
      rightPaddle.pHeight++;
    }    
    if (keyCode=='6') {
      rightPaddle.pHeight--;
    }
  }


  void newGame() {

    playerOne = 0;
    playerTwo = 0;
    oneWins = false;
    twoWins = false;
    reset();
  }

  //function headers start here

  void calcRadius() { //added to calculate radius/vector of ball trajectory
    bRadius = sqrt(pow(abs(xspeed), 2) + pow(abs(yspeed), 2)); //added to figure out ball vector
  }

  void rPadHitTest() { //added to hit test for right paddle
    if (xpos > rightPaddle.xPos - rightPaddle.pWidth/2 + bSize/2  //check to see if it's hitting paddle side to side
    && ypos < rpp + rightPaddle.pHeight/2 + bSize/2           //check to see if it's hitting paddle up and down
    && ypos > rpp - rightPaddle.pHeight/2 - bSize/2)          //check to see if it's hitting paddle up and down
    {
      rPadAngle();                                            //change the angle of the ball depending where it hits the paddle
      xdir *= -1;                                             //turn the ball around so it bounces
    }
  }
  

  void lPadHitTest() {
    if (xpos < leftPaddle.xPos - leftPaddle.pWidth/2 - bSize/2  //check to see if it's hitting paddle side to side
    && ypos < lpp + leftPaddle.pHeight/2 + bSize/2           //check to see if it's hitting paddle up and down
    && ypos > lpp - leftPaddle.pHeight/2 - bSize/2)          //check to see if it's hitting paddle up and down
    {
      lPadAngle();                                            //change the angle of the ball depending where it hits the paddle
      xdir *= -1;                                             //turn the ball around so it bounces
    }
  }

  void rPadAngle() {       //added to change the ball's angle when it hits the right paddle
    if (ypos < rpp - 10) {  //checks if y position is above the middle section of the right paddle
      println(bAngle);
      bAngle = bAngle - (map(abs(rpp - 10 - ypos), 0, 50, 0, 15));  //subtracts from angle of ball trajectory
      xspeed = cos (radians(bAngle)) * bRadius; //change ball's x speed
      yspeed = sin (radians(bAngle)) * bRadius; //change ball's y spped
      println(bAngle);
    }
    if (ypos < rpp + 10) {  //checks if y position is below the middle section of the right paddle
      println(bAngle);
      bAngle = bAngle + (map(abs(rpp + 10 - ypos), 0, 50, 0, 15));  //adds to angle of ball trajectory
      xspeed = cos (radians(bAngle)) * bRadius; //change ball's x speed
      yspeed = sin (radians(bAngle)) * bRadius; //change ball's y spped
      println(bAngle);
    }
    xpos = rightPaddle.xPos - rightPaddle.pWidth/2 - bSize/2; //corrects for sticking in right paddle
  }

  void lPadAngle() {
    if (ypos < lpp - 10) {  //checks if y position is above the middle section of the right paddle
      println(bAngle);
      bAngle = bAngle + (map(abs(lpp - 10 - ypos), 0, 50, 0, 15));  //adds to angle of ball trajectory
      xspeed = cos (radians(bAngle)) * bRadius; //change ball's x speed
      yspeed = sin (radians(bAngle)) * bRadius; //change ball's y spped
      println(bAngle);
    }
    if (ypos < lpp + 10) {  //checks if y position is below the middle section of the right paddle
      println(bAngle);
      bAngle = bAngle - (map(abs(lpp + 10 - ypos), 0, 50, 0, 15));  //subtracts from angle of ball trajectory
      xspeed = cos (radians(bAngle)) * bRadius; //change ball's x speed
      yspeed = sin (radians(bAngle)) * bRadius; //change ball's y spped
      println(bAngle);
    }
    xpos = leftPaddle.xPos + leftPaddle.pWidth/2 + bSize/2; //corrects for sticking in right paddle
  }
}

