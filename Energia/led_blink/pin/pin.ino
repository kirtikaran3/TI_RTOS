int led=30; 
int ledb=40; 
int ledg=39;
void setup()
{
  // put your setup code here, to run once:
  pinMode(led,OUTPUT); 
  pinMode(ledb,OUTPUT); 
  pinMode(ledg,OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(led,HIGH); 
  delay(100); 
  digitalWrite(led,LOW); 
  delay(100); 
  
           digitalWrite(ledb,HIGH); 
           delay(100); 
           digitalWrite(ledb,LOW); 
           delay(100); 
           
  digitalWrite(ledg,HIGH); 
  delay(100); 
  digitalWrite(ledg,LOW); 
  delay(100);
}
