int dig_IN = 2;
int an_IN = A0;

void setup() {
  pinMode(dig_IN,INPUT);
  pinMode(an_IN,INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val_digIN = digitalRead(dig_IN);
  int val_anIN = analogRead(an_IN);
  Serial.print("Valeur D2 : ");
  Serial.print(val_digIN);
  Serial.print(" ; Valeur A0 : ");
  Serial.println(val_anIN);
}
