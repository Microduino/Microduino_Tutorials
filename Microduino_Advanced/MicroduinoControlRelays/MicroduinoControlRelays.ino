/*
 * 用继电器控制电机
 */
 
const int buttonPin = 2;                 // 按钮(pushbutton)
const int relayPin = 13;                 // 继电器(Relay)
int relayState = 0;                      // 继电器状态
 
void setup()
{
  Serial.begin(9600);                     // 开启 Serial port, 波特率 9600 bps
   
  pinMode(buttonPin, INPUT);             // 把 buttonPin 设置成 INPUT
  pinMode(relayPin, OUTPUT);             // 把 relayPin 设置成 OUTPUT    
}
 
void switchRelay()
{
  if (relayState == 0)                        
    relayState = 1;                      // 把继电器状态改为 ON
  else
    relayState = 0;                      // 把继电器状态改为 OFF     
       
  digitalWrite(relayPin, relayState);    // 切换开关
 
  Serial.print("Relay status: ");        // 串口输出继电器状态
  Serial.println(relayState);
}
 
void loop()
{
  int buttonState;
   
  // 读取按钮状态
  buttonState = digitalRead(buttonPin);
 
  // 检查按钮是否按下(pressed)
  // 有的話，buttonState 會是 HIGH
  if (buttonState == HIGH) {      
     switchRelay();                      // 切换继电器开关
     delay(500);                         // 延迟0.5秒
  }
}
