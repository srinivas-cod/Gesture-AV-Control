#define TRIG_PIN 8
#define ECHO_PIN 7

long duration;
int distance;

// Gesture tracking
int lastState = 2;        // 0 = far, 1 = near, 2 = out-of-range
unsigned long holdStart = 0;
const int HOLD_TIME = 1000;  // 1 second hold for PLAY/PAUSE
unsigned long lastActionTime = 0;
const int COOLDOWN = 800;    // 0.8 sec cooldown between gestures

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read distance
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // in cm

  // Ignore invalid readings
  if (distance <= 2 || distance > 400) {
    return;
  }

  // Determine current state
  int currentState; // 0 = far, 1 = near, 2 = out-of-range
  if (distance < 20) currentState = 1;           // near
  else if (distance >= 20 && distance < 60) currentState = 0; // far
  else currentState = 2;                         // out-of-range

  unsigned long now = millis();

  // Detect hold gesture for PLAY/PAUSE
  if (currentState == 1) { // hand near
    if (holdStart == 0) holdStart = now; // start hold timer
    if (now - holdStart > HOLD_TIME && now - lastActionTime > COOLDOWN) {
      Serial.println("PLAY_PAUSE");
      lastActionTime = now;
    }
  } else {
    holdStart = 0; // reset hold timer
  }

  // Detect in/out gestures (NEXT or PREV)
  if (currentState != lastState && now - lastActionTime > COOLDOWN) {  
    if (currentState == 1 && lastState == 0) {
      Serial.println("NEXT"); // hand moved near
      lastActionTime = now;
    } 
    else if (currentState == 0 && lastState == 1) {
      Serial.println("PREV"); // hand moved away
      lastActionTime = now;
    }
  }

  lastState = currentState;
  delay(50); // small delay for stability
}
