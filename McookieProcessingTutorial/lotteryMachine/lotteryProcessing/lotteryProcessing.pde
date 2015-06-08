
import processing.serial.*;
import java.util.ArrayList;
import java.util.List;
import processing.core.PApplet;
import processing.core.PFont;
import processing.core.PImage;
 
  Serial port;    
  PFont f;
  int persongNum = 60;
  int value = 0;
  List<Integer> NumArray = new ArrayList<Integer>();
  List<Integer> oldNumArray = new ArrayList<Integer>();
  boolean running = false;
  int currentNum = 0;
  boolean rectOver = false;
  
  int c;
  char cc;
  String currentInfo="0";
 
  int width = 0;
  int height = 0;
 
  int rectX = 0;
  int rectY = 0;
 
  int rectWidth = 0;
  int rectHigth = 0;
 
  int numberLineHight = 0;
 
  PImage img;
 
  public void setup() {
 
    // Position of square button
    width = (displayWidth - 100);
    height = (displayHeight - 100);
 
    rectX = (width - 200);
    rectY = (height - 100);
 
    rectWidth = 90;
    rectHigth = 45;
 
    numberLineHight = 50;
 
    size(width, height);
    //background(255);
    background(255, 204, 0);
    // f = createFont("Arial", 16, true);
    f = createFont("Aharoni-Bold-16.vlw", 16, true);
    textFont(f, 200);
    //fill(0); // STEP 5 Specify font color
    fill(255,0,0);
    text(currentNum, width / 2, height / 2); // STEP 6 Display Text
 
 
    //初始化人数
    textFont(f, 40);
    text(60, 120, numberLineHight);
    NumArray.add(6);
    NumArray.add(0);
 
    //img = loadImage("sjaw.jpg");
    println(Serial.list());
    port = new Serial(this, Serial.list()[0], 9600); 
 
  }
 
  public void draw() {
    
      while(port.available()>0) {
      //if(port.available()>0) {
        //currentInfo=port.readStringUntil('\n');
        c = port.read();
        cc=char(c);
        println(cc);
      }
      
      if(cc=='1') {
        running=true;
        //println("true");
      }else if(cc=='0') {
        running=false;
        //println("false");
      }

    
    // 更新是否mouseOver
    update(mouseX, mouseY);
    // 如果正在选，显示循环随机数
    if (running) {
      //background(255);
      background(255, 204, 0);
      textFont(f, 200); // STEP 4 Specify font to be used
      //fill(0);
      fill(255,0,0);
      int num = (int) (Math.random() * persongNum) + 1;// 产生1~persongNum的整数随机数
 
      while(oldNumArray.contains(num)) {
         num = (int) (Math.random() * persongNum) + 1;
      }
 
      text(num, width / 2, height / 2); // STEP 6 Display Text
 
      currentNum=num;
    }
 
    fill(0);
    // 显示控制按钮
    rect(rectX, rectY, rectWidth, rectHigth);
    // 更换鼠标状态
    if (rectOver) {
      cursor(HAND);
    } else {
      cursor(ARROW);
    }
 
    // 显示到场人数
    fill(255,204,0);
    textFont(f, 40); // STEP 4 Specify font to be used
    text("Lottery", 2, numberLineHight);
    if (NumArray.size() > 0) {
      StringBuffer stringNums = new StringBuffer();
      for (Integer Num : NumArray) {
        stringNums.append(Num);
      }
      text(stringNums.toString(), 120, numberLineHight);
      persongNum = new Integer(stringNums.toString());
    }
 
    fill(255,0,0);
    textFont(f, 40); // STEP 4 Specify font to be used
    text("Lottery Activity", 2, height-100);
 
    if(img!=null) {
      image(img, width-200, 0);
    }
 
  }
 
  public void mousePressed() {
    if (rectOver) {
      if (running == false) {
        running = true;
      } else {
        running = false;
        oldNumArray.add(currentNum);   
      }
    }
  }
 
  void update(int x, int y) {
    if (overRect(rectX, rectY, rectWidth, rectHigth)) {
      rectOver = true;
    } else {
      rectOver = false;
    }
  }
 
  boolean overRect(int x, int y, int width, int height) {
    if (mouseX >= x && mouseX <= (x + width) && mouseY >= y
        && mouseY <= (y + height)) {
      return true;
    } else {
      return false;
    }
  }
 
  public void keyPressed() {
    if ((key - 48) >= 0 && (key - 48) <= 9) {
      NumArray.add((int) (key - 48));
    } else if (keyCode == BACKSPACE) {
      if (NumArray.size() > 0) {
        NumArray.remove(NumArray.size() - 1);
      }
    }
  }
