void setup(void)
{
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
}

void loop(void)
{
  delay(3000);
  
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);  
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(4, LOW);
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(7, LOW);
}
