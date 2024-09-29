void setup() {
  // put your setup code here, to run once:
  pinMode(0, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(A0, !digitalRead(12));
  digitalWrite(A1, !digitalRead(6));
  digitalWrite(A2, !digitalRead(0));
}
