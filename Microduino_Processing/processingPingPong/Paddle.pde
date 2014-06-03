//class for paddles

class Paddle {
 
 float pWidth;
 float pHeight;
 float yPos;
 float xPos;
 
 Paddle(float ipWidth, float ipHeight, float ixPos, float iyPos) {
  
  pWidth = ipWidth;
  pHeight = ipHeight;
  xPos = ixPos;
  yPos = iyPos; 
 }
 
 void display(float yPos) {
  
  noStroke();
  fill(150); 
  rect(xPos, yPos, pWidth, pHeight);
   
 }
  
}
