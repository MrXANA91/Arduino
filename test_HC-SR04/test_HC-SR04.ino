int echo = 9;
int trig = A4;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

unsigned long time_var;

void setup() {
  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);

  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete && inputString.equals("trig\n")) {
    digitalWrite(trig,HIGH);
    delayMicroseconds(15);
    digitalWrite(trig,LOW);

    while(digitalRead(echo) == LOW);
    time_var = micros();
    while(digitalRead(echo) == HIGH);
    time_var = micros() - time_var;

    Serial.println(time_var);
    
    inputString = "";
    delay(60);
  }
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
