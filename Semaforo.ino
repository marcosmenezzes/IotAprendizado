void setup() {
  pinMode(5, OUTPUT);   // Verde
  pinMode(4, OUTPUT);   // Amarelo
  pinMode(16, OUTPUT);  // Vermelho
}

void loop() {
  // Verde acende
  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(16, LOW);
  delay(3000);

  // Amarelo acende
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(16, LOW);
  delay(1000);

  // Vermelho acende
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  digitalWrite(16, HIGH);
  delay(3000);
}
