//BASED ON OTHER DEVELOPERS CODE(https://kkmspb.ru/development/arduino/russia-thermo-printer.php, https://www.youtube.com/watch?v=-rm_HQQMKOQ)
#define PIN_MOTOR1 2
#define PIN_MOTOR2 3
#define PIN_MOTOR3 4
#define PIN_MOTOR4 5

#define PIN_PRN_LATCH 6
#define PIN_PRN_CLK 7
#define PIN_PRN_STROBE 8
#define PIN_PRN_DATA 9

#define PRN_LOAD_DELAY_US 10
#define PRN_BURN_DELAY_US_MIN 1000
#define PRN_BURN_DELAY_US_MAX 5000

bool rotation_table[4][4] = {
  {true, false, false, true},
  {false, false, true, true},
  {false, true, true, false},
  {true, true, false, false}
};

String input;

void setup() {
  pinMode(PIN_MOTOR1, OUTPUT);
  pinMode(PIN_MOTOR2, OUTPUT);
  pinMode(PIN_MOTOR3, OUTPUT);
  pinMode(PIN_MOTOR4, OUTPUT);
  pinMode(PIN_PRN_LATCH, OUTPUT);
  pinMode(PIN_PRN_CLK, OUTPUT);
  pinMode(PIN_PRN_STROBE, OUTPUT);
  pinMode(PIN_PRN_DATA, OUTPUT);
  digitalWrite(PIN_MOTOR1, false);
  digitalWrite(PIN_MOTOR2, false);
  digitalWrite(PIN_MOTOR3, false);
  digitalWrite(PIN_MOTOR4, false);
  digitalWrite(PIN_PRN_LATCH, true);
  digitalWrite(PIN_PRN_CLK, false);
  digitalWrite(PIN_PRN_STROBE, true);
  digitalWrite(PIN_PRN_DATA, false);
  Serial.begin(115200);
  Serial.println("Init");
}

void rotateStepMotor() {
  for(int i = 0; i < 4; i++) {
    digitalWrite(PIN_MOTOR1, rotation_table[i][0]);
    digitalWrite(PIN_MOTOR2, rotation_table[i][1]);
    digitalWrite(PIN_MOTOR3, rotation_table[i][2]);
    digitalWrite(PIN_MOTOR4, rotation_table[i][3]);
    delay(5);
  }
  digitalWrite(PIN_MOTOR1, false);
  digitalWrite(PIN_MOTOR2, false);
  digitalWrite(PIN_MOTOR3, false);
  digitalWrite(PIN_MOTOR4, false);
}

void printOneLine(int line[128]) {
  Serial.println("Printing started");
  //Serial.print("Line: ");for(int i = 0; i < 128; i++) {Serial.print(line[i]);}; Serial.println();
  bool active[16];
  for(int j = 1; j < 16; j++) {
    bool x = false;
    for(int i = 0; i < 128; i++) {
      if(line[i] == j) {
        x = true;
        break;
      }
    }
    //if(x) { Serial.print("A J active: ");Serial.println(j); }
    active[j] = x;
  }
    //Serial.print("A ");Serial.print(j);Serial.println();
  for(int j = 1; j < 16; j++) {
    if(active[j]) {
      //Serial.print("B J active: ");Serial.println(j);
      int pointsCount = 0;
      bool temp[128];
      for(int i = 0; i < 128; i++) {
        bool firePoint = (line[i] == j);
        temp[i] = firePoint;
        if(firePoint) {
          pointsCount++;
        }
        if(pointsCount >= 32 || i>=127) { //only 32 points allowed at one burn
          //Serial.print("Burning: ");Serial.print(pointsCount);Serial.print(" points, index ");Serial.println(i);
          for(int k = 128; k >= 0; k--) {
            digitalWrite(PIN_PRN_DATA, temp[k]);
            temp[k] = false;
            delayMicroseconds(PRN_LOAD_DELAY_US);
            digitalWrite(PIN_PRN_CLK, true);
            delayMicroseconds(PRN_LOAD_DELAY_US);
            digitalWrite(PIN_PRN_CLK, false);
            delayMicroseconds(PRN_LOAD_DELAY_US);
          }
          delayMicroseconds(PRN_LOAD_DELAY_US);
          digitalWrite(PIN_PRN_LATCH, false);
          delayMicroseconds(PRN_LOAD_DELAY_US);
          digitalWrite(PIN_PRN_LATCH, true);
          delayMicroseconds(PRN_LOAD_DELAY_US);
          
          digitalWrite(PIN_PRN_STROBE, false);
          //int delayUs = map(j, 1, 15, PRN_BURN_DELAY_US_MIN, PRN_BURN_DELAY_US_MAX); //NOT WORKING BY SOME REASON, LOOP JUST HANGS FOREVER
          int delayUs = (j - 1) * (PRN_BURN_DELAY_US_MAX - PRN_BURN_DELAY_US_MIN) / (15 - 1) + PRN_BURN_DELAY_US_MIN;
          delayMicroseconds(delayUs);
          digitalWrite(PIN_PRN_STROBE, true);

          digitalWrite(PIN_PRN_DATA, false);
          pointsCount = 0;
        }
      }
    }
  }
  rotateStepMotor();
  Serial.println("Printing finished");
}

void loop() {
  while (Serial.available() <= 0) {}
  input = Serial.readStringUntil('\n');
  if(input.length() != 128) {
    Serial.print("Error! Line length should be 128 chars, containing values from 0 to f (");Serial.print(input);Serial.println(")");
    return;
  }
  int line[128];
  for(int i = 0; i < 128; i++) {
    switch(input[i]) {
      case '0':
        line[i] = 0;
        break;
      case '1':
        line[i] = 1;
        break;
      case '2':
        line[i] = 2;
        break;
      case '3':
        line[i] = 3;
        break;
      case '4':
        line[i] = 4;
        break;
      case '5':
        line[i] = 5;
        break;
      case '6':
        line[i] = 6;
        break;
      case '7':
        line[i] = 7;
        break;
      case '8':
        line[i] = 8;
        break;
      case '9':
        line[i] = 9;
        break;
      case 'a':
        line[i] = 10;
        break;
      case 'b':
        line[i] = 11;
        break;
      case 'c':
        line[i] = 12;
        break;
      case 'd':
        line[i] = 13;
        break;
      case 'e':
        line[i] = 14;
        break;
      case 'f':
        line[i] = 15;
        break;
      default:
        Serial.println("Error! Line length should be 128 chars, values from 0 to f");
        return;
    }
  }
  printOneLine(line);
}
