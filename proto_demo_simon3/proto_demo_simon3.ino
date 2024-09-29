#define CODE_MIN_LENGTH 5
#define CODE_MAX_LENGTH 100

#define NUM_BTN 3
#define BTN0 12
#define BTN1 6
#define BTN2 0
#define LED0 A0
#define LED1 A1
#define LED2 A2

typedef enum {
  WAITING,
  FAILED,
  PASSED
} success_t ;

unsigned int leds[NUM_BTN] = { LED0, LED1, LED2 };

void ledAnimationFailure() {
  digitalWrite(LED0, HIGH);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  delay(2000);
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void ledAnimationSuccess() {
  for (unsigned int i=0; i < 5; i++) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay(50);
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    delay(50);
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(50);
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay(50);
  }
}

void ledAnimationShowCode(unsigned int code[CODE_MAX_LENGTH], unsigned int length) {
  for (unsigned int i=0; i < length; i++) {
    digitalWrite(leds[code[i]], HIGH);
    delay(1000);
    digitalWrite(leds[code[i]], LOW);
    delay(200);
  }
}

void setup() {
  pinMode(BTN0, INPUT_PULLUP);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  //Serial.begin(115200);
}

void loop() {
  static success_t result = FAILED; // boot state => restart from failure
  static unsigned int code[CODE_MAX_LENGTH];
  static unsigned int length = 0;
  static unsigned int press_index = 0;
  static bool pressed = false;

  bool btns[NUM_BTN] = { !digitalRead(BTN0), !digitalRead(BTN1), !digitalRead(BTN2) };
  unsigned int btns_sum1 = 0;
  for (unsigned int i=0; i < NUM_BTN; i++) {
    if (btns[i]) {
      btns_sum1 += i+1;
    }
  }

  if (result == FAILED) {
    ledAnimationFailure();
    delay(1000);
    // generate a full new code
    randomSeed(analogRead(A5)); // https://www.arduino.cc/reference/en/language/functions/random-numbers/random/
    for (unsigned int i=0; i < CODE_MAX_LENGTH; i++) {
      code[i] = random(NUM_BTN); // generates a number in range [0 ; NUM_BTN-1]
    }
    // reset status
    length = CODE_MIN_LENGTH;
    result = WAITING;
    press_index = 0;
    // show the code to be reproduced
    ledAnimationShowCode(code, length);
  }
  else if (result == PASSED) {
    ledAnimationSuccess();
    delay(1000);
    if (length < CODE_MAX_LENGTH) {
      // go to next step
      length++;
      result = WAITING;
      press_index = 0;
      // show the code to be reproduced
      ledAnimationShowCode(code, length);
    }
    else {
      // maximum level reached!
      ledAnimationSuccess();
      ledAnimationSuccess();
      ledAnimationSuccess();
      // restart from scratch
      result = FAILED;
    }
  }
  else { // waiting for user input
    // lit the LEDs corresponding to the current button(s) press
    for (unsigned int i=0; i < NUM_BTN; i++) {
      digitalWrite(leds[i], btns[i]);
    }
    // then check if the button(s) press matches the code
    if (btns_sum1) {
      if (!pressed) {
        pressed = true;
        // compare button press to stored code @ press_index
        //Serial.println(btns_sum1-1 + String(" ?= ") + code[press_index]);
        if (btns_sum1-1 == code[press_index]) { // good input!
          if (++press_index == length) {
            result = PASSED;
          }
          // else: wait for the user to complete his/her input
        }
        else {
          result = FAILED;
        }
      }
    }
    else if (pressed && btns_sum1 == 0) {
      pressed = false;
    }
  }
}
