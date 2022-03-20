#include <Adafruit_Fingerprint.h>
//SoftwareSerial mySerial(2, 3);
#define mySerial Serial3
#define espSerial Serial1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//#define RL 13
uint8_t id;
uint8_t cmd;
uint8_t id_send;
bool checked; //Finger Check
String iDStr; //Id Input
const byte rows = 4; //số hàng
const byte columns = 4; //số cột
const int maxChar=11;

String esp_R="";


int holdDelay = 700; //Thời gian trễ để xem là nhấn 1 nút nhằm tránh nhiễu
int n = 3; // 
int state = 0; //nếu state =0 ko nhấn,state =1 nhấn thời gian nhỏ , state = 2 nhấn giữ lâu
char key = 0;
 
//Định nghĩa các giá trị trả về
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

  
byte rowPins[rows] = {25,29,33,37}; //Cách nối chân với Arduino
byte columnPins[columns] = {41,45,49,53}; 
 
//cài đặt thư viện keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);

void setup() {
  lcd.init();
  lcd.init();
  lcd.backlight();
  //lcd.setCursor(4, 0);
  //pinMode(RL, OUTPUT);
  Serial.begin(9600);
  espSerial.begin(115200);
  while (!Serial);  //
  delay(100);
  Serial.println("\n\nFinger Detection Test");
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  

}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
} 

void errorPrint() {
    lcd.setCursor(0, 1);
    lcd.print("That bai");
    delay(2000);
    lcd.clear();
  }
void okPrint() {
    lcd.setCursor(0, 1);
    lcd.print("Thanh cong");
    delay(2000);
    lcd.clear();
  }


void loop()
{
  char temp = keypad.getKey();
  if ((int)keypad.getState() ==  PRESSED) {
    if (temp != 0) {
      key = temp;
    }
  }
  if ((int)keypad.getState() ==  HOLD) {
    state++;
    state = constrain(state, 1, n-1);
    delay(holdDelay);
  }

  delay(100);
  ///------KeyPad
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nhap A:Dang nhap");
  lcd.setCursor(1, 1);
  lcd.print("Nhap B:Them ID");
  delay(50);
  // 1 trong 2 chế độ
  switch (key) {
  case 'A':
    checked=false;  
    while ((!checked))  {
    key = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dang nhap");
    //lcd.setCursor(1, 1);
    //lcd.print("Nhan 3 de thoat")
    id_send= getFingerprintIDez();
      if (id_send==-1) 
      {
        errorPrint();
        break;
      };
    }
    break;
    
  case 'B': 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Van tay ID:");
    //--keypad input
    char num = keypad.getKey();
    iDStr='\0';
    while (key!='#') {
      while (num == NO_KEY) num = keypad.getKey();
      key=num;
      num=NO_KEY;
      int iDValid=(int)key;
      //Serial.print((int)key);
      if ((iDValid<58) && (iDValid>47))
      { 
        iDStr=iDStr+key;
        lcd.setCursor(12, 0);
        lcd.print(iDStr);
      }
      else if(iDValid==35) ;
      else if ((iDValid==42)&&(iDStr.length()>0)) 
      {  
        iDStr.remove(iDStr.length()-1,1);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Van tay ID:");
        lcd.setCursor(12, 0);
        lcd.print(iDStr);
      }
      else 
      {
        lcd.setCursor(1, 1);
        lcd.print("Khong hop le");
      }
      delay(50);
      while ((int)keypad.getState() ==  HOLD) ;
      }
    //-----
    Serial.println(iDStr);
    id=iDStr.toInt();
    if ((id<1) || (id>127)) errorPrint();
    else
    {
    Serial.print("Enrolling ID #");
    Serial.println(id);    
    getFingerprintEnroll();

    }

    break;
  }
  delay(50);
}


uint8_t getFingerprintEnroll() {
  
  //Scan lần 1
  int p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan lan 1");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  if (p==FINGERPRINT_OK) okPrint();
  else {
    errorPrint();
    return p;
  }

  p = finger.image2Tz(1);
  if (p!=FINGERPRINT_OK) {
    errorPrint();
    return p;
  }
  delay(2000);
  //------------------
  //Scan lan 2
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan lan 2");
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  if (p==FINGERPRINT_OK) okPrint();
  else {
    errorPrint();
    return p;
  }

  p = finger.image2Tz(2);
  if (p!=FINGERPRINT_OK) {
    errorPrint();
    return p;
  }
  //------------------
  // So sanh
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("So sanh");
  p = finger.createModel();
  if (p == FINGERPRINT_OK)  okPrint();
  else {
    errorPrint();
    return p;
  }
  //------------------
  //Luutru
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luu tru");
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) 
  {
  okPrint();
  }
  else {
    errorPrint();
    return p;
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p==FINGERPRINT_NOFINGER) return -2;
  else
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
 
  // found a match!
  //digitalWrite(RL, HIGH);
  //digitalWrite(RL, LOW);    

  okPrint();    
  Serial.println(finger.fingerID);
  espSerial.print(finger.fingerID);
  espSerial.print('e');
  espSerial.flush();
  //Respond check
  lcd.setCursor(0, 0);
  lcd.print("Ghi nhan");
  esp_R="";
  while (espSerial.available()<=0);
  while (esp_R.length()<=0)
  {
  esp_R=espSerial.readStringUntil('f');
  esp_R.replace(" ","");
  }
  if (esp_R == "200") {
    okPrint();
    Serial.println("here");
  }
  else {
    errorPrint();
    Serial.print(esp_R);
    Serial.println("not here");
  }
  espSerial.flush();

   
  //----
  checked=true;
  return finger.fingerID;
}
