#define red RED_LED 
#define green GREEN_LED 
#define blue BLUE_LED

void setup()
{
  // put your setup code here, to run once:
      pinMode(red,OUTPUT); 
      pinMode(blue,OUTPUT); 
      pinMode(green,OUTPUT);  
}

void loop()
{
  // put your main code here, to run repeatedly: 
      digitalWrite(red,HIGH); 
      delay(100); 
      digitalWrite(red,LOW); 
      delay(100); 
       
                 digitalWrite(green,HIGH); 
                 delay(100); 
                 digitalWrite(green,LOW); 
                 delay(100); 
                 
                         digitalWrite(blue,HIGH); 
                         delay(100); 
                         digitalWrite(blue,LOW);  
                         delay(100); 
}
