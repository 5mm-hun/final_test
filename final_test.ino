#include <Servo.h>
#include <Stepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);

const int stepsPerRevolution = 1024; 
Stepper myStepper(stepsPerRevolution,11,9,10,8); 

int TRIG_PIN =2;
int ECHO_PIN= 3;
int LED_PIN =4;
int DFlame= A0;
Servo myservo;

int servo =6;

bool smoke= false;

int num=0;
int pos=0;
int i;

void setup() { 
  myStepper.setSpeed(5);
  myservo.attach(servo);
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(LED_PIN, OUTPUT);

  pinMode(DFlame,INPUT);
  
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("start operation");
  Serial.begin(9600);
  
}
 
void loop() 
{
  int distance = 0;  
  int val;
  int type;
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW); 
  distance = pulseIn(ECHO_PIN, HIGH)/58.2;
  val = analogRead(DFlame);

 
  for(;distance<=25;distance = pulseIn(ECHO_PIN, HIGH)/58.2){
    Serial.println(distance);
    digitalWrite(LED_PIN, HIGH);
    lcd.clear();
    lcd.print("can't use");
    
    val = analogRead(DFlame);
    for(;val<999;val=analogRead(DFlame)){
      digitalWrite(13, HIGH); 
      digitalWrite(12, HIGH);
      lcd.clear();
      lcd.print("smoking");
      smoke=true;
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10); 
      digitalWrite(TRIG_PIN, LOW); 
      distance = pulseIn(ECHO_PIN, HIGH)/58.2;
      if(distance>25)
        break;
      
    }
      
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10); 
    digitalWrite(TRIG_PIN, LOW);
  }
  
  
  digitalWrite(LED_PIN, LOW);
  if(smoke){
    lcd.clear();
    lcd.print("wait some minutes");
    delay(500);
    stepmotor();
    servomotor();
    smoke=false;
  }
  
      
    digitalWrite(13, LOW); 
    digitalWrite(12, LOW);
    lcd.clear();
    lcd.print("servicable");

  
  
  
}
void stepmotor(){
  myStepper.step(stepsPerRevolution);
  delay(500);
  myStepper.step(stepsPerRevolution);
  delay(500);
}
void servomotor(){
 for (pos = 85; pos >= 0; pos -= 1){ 
   myservo.write(pos);              
   delay(5);
 }
 for (pos = 0; pos <= 85; pos += 1){ 
   myservo.write(pos);
   delay(5);  
 }
}
