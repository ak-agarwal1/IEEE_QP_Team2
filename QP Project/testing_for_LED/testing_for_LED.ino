int r = D8;
int g = D7;
int b = D6;

void setup() {
  pinMode(r, OUTPUT);
  digitalWrite(r, LOW);
  pinMode(g, OUTPUT);
  digitalWrite(g, LOW);
  pinMode(b, OUTPUT);
  digitalWrite(b, LOW);
}

void loop() {
  
  digitalWrite(r, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(g, HIGH);
}
