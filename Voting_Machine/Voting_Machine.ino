  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include <Keypad.h>
  #include <Adafruit_Fingerprint.h>
  #include <Servo.h>
  #include <EEPROM.h>
  
  LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
  SoftwareSerial mySerial(11, 12); //yellow wire,orange wire
  Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
  Servo myservo;
  
  int red = 22;
  int yellow = 24;
  int buzzerPin1 = 25;
  int buzzerPin2 = 27;
  const int votingDone = 0;
  volatile boolean seenresult = false;
  uint8_t id = 1;
  boolean success = true;
  volatile int votearray[10];
  const byte ROWS = 4; //Keypad has four rows
  const byte COLS = 4; //Keypad has four columns
  char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };
  
  byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
  byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  char pass[10] = {'1', '2', '3', '4'};
  const int passlen = 4;
  //This is the default password of the machine...Edit the pass and passlen to change the password
  char passread[10];
  /////////////////////////////////////////////////////////
  //Edit this to increase count of voters
  volatile boolean voted[30];
  /////////////////////////////////////////////////////////
  char key;
  int reset = A1;
  int result = A0;
  int cpanel = A2;
  int panel = 47;
  int systemreset = 48;
  int blindvote = 0;
  
  void setup() {
    pinMode(buzzerPin1, OUTPUT);
    pinMode(buzzerPin2, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(yellow, OUTPUT);
    pinMode(result, INPUT);
    pinMode(reset, INPUT);
    pinMode(panel, INPUT);
    pinMode(systemreset, OUTPUT);
    pinMode(30, OUTPUT);
    pinMode(31, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(34, OUTPUT);
    pinMode(35, OUTPUT);
    pinMode(36, OUTPUT);
    pinMode(37, OUTPUT);
    pinMode(38, OUTPUT);
    pinMode(29, OUTPUT);
    myservo.attach(50);
    myservo.write(135);
  
    Serial.begin(9600);
    finger.begin(57600);
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Welcome to AEVM ");
    delay(2000);
    if (finger.verifyPassword()) {
      Serial.println("Found fingerprint sensor!");
    } else {
      Serial.println("Did not find fingerprint sensor :(");
      while (1) {
        delay(1);
      }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    if (finger.verifyPassword()) {
  
    } else {
      lcd.print("FPS Not Found");
      while (1) {
        delay(1);
      }
    }
    lcd.print("Testing AEVM");
    for (int it = 30; it < 39; it++) {
      digitalWrite(it, HIGH);
      delay(1000);
      digitalWrite(it, LOW);
    }
    digitalWrite(29, HIGH);
    delay(1000);
    digitalWrite(29, LOW);
    digitalWrite(yellow, HIGH);
    delay(1000);
    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
    delay(1000);
    digitalWrite(red, LOW);
    digitalWrite(buzzerPin1, HIGH);
    digitalWrite(buzzerPin2, HIGH);
    delay(1000);
    digitalWrite(buzzerPin1, LOW);
    digitalWrite(buzzerPin2, LOW);
    delay(1000);
  
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password");
    while (!checkpass()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password OK");
    delay(2000);
  
    if (EEPROM.read(0) == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Result Found");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press A to open control panel");
      delay(100);
      while (1) {
        key = keypad.getKey();
        lcd.scrollDisplayLeft();
        delay(400);
        if (key) {
          if (key == 'A') {
            openPanel();
          }
        }
      }
    }
  
    option();
  }
  
  
  void loop() {
  
  
  
  }
  void enroll() {
    int i = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press A to enroll next.B to quit             ");
    delay(100);
  
  
    while (i != 1) {
      key = keypad.getKey();
      lcd.scrollDisplayLeft();
      delay(400);
      if (key) {
        if (key == 'A') {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enroll id:");
          lcd.print(id);
          delay(3000);
          getFingerprintEnroll();
          delay(1000);
          if (success) {
            id++;
          }
          i = 1;
          enroll();
  
        }
        else if (key == 'B') {
          option();
        }
      }
    }
  }
  
  void vote() {
    int i = 0, j = 0, k = 0, n, fid;
    char key, lastKey = 'A';
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter No. of");
    lcd.setCursor(0, 1);
    lcd.print("Candidates");
    while (i != 1) {
      key = keypad.getKey();
      if (key) {
  
        n = int(key) - 47;
  
        if (n > 2) {
          i++;
        }
        else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Invalid No");
        }
  
      }
    }
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Voting Started");
    delay(1000);
    i = 0;
  test: while (i != 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scan Finger");
      digitalWrite(yellow, HIGH);
      digitalWrite(red, LOW);
      while (k != 1) {
        key = keypad.getKey();
        fid = getFingerprintIDez();
        if (fid == -1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Try Again");
        }
        else if (fid != -2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          if (voted[fid] == true) {
            lcd.print("Kallan");
            for (int it = 1; it <= 15; ++it) {
              digitalWrite(buzzerPin1, HIGH);
              digitalWrite(buzzerPin2, HIGH);
              delay(300);
              digitalWrite(buzzerPin1, LOW);
              digitalWrite(buzzerPin2, LOW);
              delay(300);
            }
  
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Scan Finger");
            continue;
          }
          lcd.print("Found ID:");
          lcd.print(fid);
          k = 1;
        }
        if (key) {
          if ((lastKey == 'C') && (key == 'D')) {
            lcd.clear();
            lcd.setCursor(0, 0);
            digitalWrite(yellow, LOW);
            digitalWrite(red, LOW);
            lcd.print("Enter Password");
            if (checkpass()) {
              showpercentage(n);
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Scan Finger");
            }
  
          }
          if ((lastKey == 'C') && (key == 'B')) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Enter Password");
            if (checkpass()) {
              blindvote += 1;
              Serial.println(blindvote);
              blind(n);
              goto test;
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Scan Finger");
            }
  
          }
  
          lastKey = key;
        }
  
      }
      k = 0;
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cast Your Vote");
      digitalWrite(yellow, LOW);
      digitalWrite(red, HIGH);
      j = 0;
      while (j != 1) {
        key = keypad.getKey();
        if (key) {
          if ((int(key) - 48) < n) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("You Voted For:");
            lcd.setCursor(0, 1);
            lcd.print(key);
            votearray[int(key) - 48]++;
            digitalWrite(29 + (int(key) - 48), HIGH);
            digitalWrite(buzzerPin1, HIGH);
            digitalWrite(buzzerPin2, HIGH);
            delay(2000);
            digitalWrite(buzzerPin1, LOW);
            digitalWrite(buzzerPin2, LOW);
            digitalWrite(29 + (int(key) - 48), LOW);
            voted[fid] = true;
            delay(1000);
            digitalWrite(yellow, HIGH);
            digitalWrite(red, LOW);
            j++;
          }
          else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Invalid Vote");
            lcd.setCursor(0, 1);
            lcd.print("Vote Again");
            delay(500);
          }
        }
      }
    }
  }
  
  void blind(int n) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cast Your Vote");
    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
    int j = 0;
    while (j != 1) {
      char key = keypad.getKey();
      if (key) {
        if ((int(key) - 48) < n) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("You Voted For:");
          lcd.setCursor(0, 1);
          lcd.print(key);
          votearray[int(key) - 48]++;
          digitalWrite(29 + (int(key) - 48), HIGH);
          digitalWrite(buzzerPin1, HIGH);
          digitalWrite(buzzerPin2, HIGH);
          delay(2000);
          digitalWrite(buzzerPin1, LOW);
          digitalWrite(buzzerPin2, LOW);
          digitalWrite(29 + (int(key) - 48), LOW);
          delay(1000);
          digitalWrite(yellow, HIGH);
          digitalWrite(red, LOW);
          j++;
        }
        else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Invalid Vote");
          lcd.setCursor(0, 1);
          lcd.print("Vote Again");
          delay(500);
        }
      }
    }
  }
  
  void option() {
    int opt = 0, i = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("1.Enroll");
    lcd.setCursor(0, 1);
    lcd.print("2.Start Voting");
    while (i != 1) {
      char key = keypad.getKey();
      switch (key) {
        case '1': enroll();
          i++;
          break;
        case '2': vote();
          i++;
          break;
        default: i = i;
      }
    }
  }
  boolean checkpass() {
    int i = 0, j = 0;
    while (i < passlen) {
      char key = keypad.getKey();
      if (key) {
        lcd.setCursor(i, 1);
        lcd.print("*");
        passread[i] = key;
        i++;
        if (i == passlen) {
          for (j = 0; j < passlen; j++) {
            if (pass[j] != passread[j]) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Invalid Password");
              delay(1000);
              return false;
            }
          }
          return true;
        }
      }
    }
  }
  
  uint8_t getFingerprintEnroll() {
  
    int p = -1;
    Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place Finger");
    delay(1000);
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          Serial.println("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          Serial.println(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Imaging error");
          break;
        default:
          Serial.println("Unknown error");
          break;
      }
    }
  
    // OK success!
  
    p = finger.image2Tz(1);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    }
  
    Serial.println("Remove finger");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Remove Finger");
    delay(1000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
    }
    Serial.print("ID "); Serial.println(id);
    p = -1;
    Serial.println("Place same finger again");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place the");
    lcd.setCursor(0, 1);
    lcd.print("SAME FINGER");
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          Serial.println("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          Serial.print(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Imaging error");
          break;
        default:
          Serial.println("Unknown error");
          break;
      }
    }
  
    // OK success!
  
    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    }
  
    // OK converted!
    Serial.print("Creating model for #");  Serial.println(id);
  
    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      Serial.println("Prints matched!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SUCCESS");
      success = true;
      delay(2000);
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fingers did Not");
      lcd.setCursor(0, 1);
      lcd.print("Match Try Again");
      success = false;
      delay(1000);
      Serial.println("Fingerprints did not match");
      return p;
    } else {
      Serial.println("Unknown error");
      return p;
    }
  
    Serial.print("ID "); Serial.println(id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
      Serial.println("Stored!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return p;
    } else if (p == FINGERPRINT_BADLOCATION) {
      Serial.println("Could not store in that location");
      return p;
    } else if (p == FINGERPRINT_FLASHERR) {
      Serial.println("Error writing to flash");
      return p;
    } else {
      Serial.println("Unknown error");
      return p;
    }
  }
  
  int getFingerprintIDez() {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -2;
  
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -2;
  
    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK) {
      return -1;
    }
  
    // found a match!
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    return finger.fingerID;
  }
  
  
  void showpercentage(int n) {
    int sum = 0, total = 0;
    float avg;
    lcd.clear();
    lcd.setCursor(0, 0);
    for (int i = 0; i < n; ++i) {
      sum += votearray[i];
    }
    finger.getTemplateCount();
    total = finger.templateCount;
    total = total + blindvote;
    avg = (float(sum) / total) * 100;
  
    lcd.print(" Polling: ");
    lcd.print(avg);
    lcd.print("%");
  
    lcd.print("  Reg Voters: ");
    lcd.print(total);
    lcd.print(" ");
  
    lcd.setCursor(0, 1);
    lcd.print(" Blind: ");
    lcd.print(blindvote);
    lcd.print("  ");
  
    EEPROM.write(0, 1);
    EEPROM.write(1, n);
    for (int i = 0; i <= n; ++i) {
      EEPROM.write(i + 2, votearray[i]);
    }
  
  
    while (1) {
      lcd.scrollDisplayLeft();
      delay(300);
    }
  }
  
  
  void openPanel() {
    lcd.clear();
    lcd.setCursor(0, 0);
    myservo.write(45);
    delay(15);
    lcd.print("Panel Open");
    delay(1000);
    int i = 0;
    delay(500);
    while (1) {
      Serial.println(analogRead(result));
      if (analogRead(result) > 1020 && i != 0) {
        Serial.println("result");
        showresult();
  
      }
  
      if (analogRead(reset) > 1020 && i != 0) {
        Serial.println("result");
        resetsystem();
      }
      i++;
      delay(500);
    }
  
  }
  
  
  void showresult() {
    lcd.clear();
    lcd.setCursor(0, 0);
    long int timer = millis();
    seenresult = true;
    for (int i = 2; i <= EEPROM.read(1) + 1; ++i) {
  
      if (i == 8) {
        lcd.setCursor(0, 1);
      }
      if (i == 2) {
        lcd.print("NOTA");
        lcd.print(">>");
        lcd.print(EEPROM.read(i));
        lcd.print("  ");
      }
      else {
        lcd.print(i - 2);
        lcd.print(">>");
        lcd.print(EEPROM.read(i));
        lcd.print("  ");
      }
    }
    while (millis() - timer < 15000) {
      lcd.scrollDisplayLeft();
      delay(300);
    }
  
  }
  
  
  void resetsystem() {
    if (seenresult == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Result not seen");
      delay(1000);
      return;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    while (1) {
  
      if (digitalRead(panel) == LOW) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Close the Panel");
        delay(1000);
      }
      else {
  
        lcd.print("Resetting.....");
        EEPROM.write(0, 0);
        finger.emptyDatabase();
        delay(2000);
        myservo.write(135);
        lcd.clear();
        digitalWrite(systemreset, HIGH);
      }
    }
  
  
  }
