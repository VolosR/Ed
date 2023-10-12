#define TOUCH_MODULES_CST_SELF
#include "midleFont.h"
#include "smallFont.h"
#include "tinyFont.h"
#include "bigFont.h"
#include <PN5180.h>
#include <PN5180ISO14443.h>
#include <PN5180ISO15693.h>
#include "TouchLib.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Wire.h"
#include "pins_config.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

//#define WAKEPIN 3

#define PIN_IIC_SCL                  17
#define PIN_IIC_SDA                  18
#define PIN_TOUCH_INT                16
#define PIN_TOUCH_RES                21
#define TOUCH_MODULES_CST_SELF

#define PN5180_NSS  2 
#define PN5180_BUSY 12 
#define PN5180_RST  1

#define LCD_MODULE_CMD_1

#warning Please confirm that you have purchased a display screen with a touch chip, otherwise the touch routine cannot be implemented.
#if defined(TOUCH_MODULES_CST_MUTUAL)
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS328_SLAVE_ADDRESS, PIN_TOUCH_RES);
#elif defined(TOUCH_MODULES_CST_SELF)
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);
#endif

#define TOUCH_GET_FORM_INT 0

bool flags_sleep = false;
#if TOUCH_GET_FORM_INT
bool get_int = false;
#endif

//colors 
unsigned short grays[12];
#define red 0xC104
#define blue 0x1373
#define yellow 0x9381

PN5180ISO14443 nfc14443(PN5180_NSS, PN5180_BUSY, PN5180_RST); 
PN5180ISO15693 nfc15693(PN5180_NSS, PN5180_BUSY, PN5180_RST);
String card="";
String id="";
String down2="";
int n,m,deb=0;
int sl=320;
int animation=0;


//                     0     1    2    3    4    5    6    7 
String bloodTypes[8]={"A+","0+","B+","AB+","A-","0-","B-","AB-"};
int canRecieve[8][8]={{0,1,4,5,9,9,9,9},          // A+ can recieve
                      {1,5,9,9,9,9,9,9},          // 0+ can r
                      {1,2,5,6,9,9,9,9},          // B+
                      {0,1,2,3,4,5,6,7},          //AB+
                      {4,5,9,9,9,9,9,9},          //A-
                      {5,9,9,9,9,9,9,9},          //0-
                      {5,6,9,9,9,9,9,9},          //B-
                      {4,5,6,7,9,9,9,9}};         //AB-  


//..........enployes....................
//String usersID[5]={"2d9781215c16e0","23d6781215c16e0","","",""};
String usersID[5]={"e04150b8f6e0f1","e04150b8f6e0d1","e041539ab3d0","e04150b8f6e0a1","e04150b8f6e2fa"};

String usersName[5]={"EDWARD","ROSE","DAVE","NECO","MARC"};
int employee;

//............................patiences

int patient;
//String patientsID[5]={"b3d6781215c16e0","a0d7781215c16e0","","",""};
String patientsID[5]={"e04150b8f71e88","e04150b8f720cb","e04150b8f71e5a","e04150b8f7209b","e04150b8f71e2a"};
String patientsName[5]={"JULIE SMITH","JOHN ROBERT","ROBERT WILLIAMS","SAMUEL BRADY","JERRY JONES"};
int patientsBT[5]={3,0,6,4,5};
String petientsDOB[5]={"09/22/57","05/21/97","11/21/41","09/22/80","06/06/41"};
//String PatientREQ[3]={};

//.........................bags of blood
//String bagsID[9]={"46d5781215c16e0","c3d6781215c16e0","90d7781215c16e0","e04150b8f71f2e","e04150b8f71cea","e04150b8f86312","e04150b8f86597","e04150b8f86342","e04150b8f865c5"};
String bagsID[9]={"e04150b8f86672","e04150b8f6e121","e04150b8f6e13f","e04150b8f71f2e","e04150b8f71cea","e04150b8f86312","e04150b8f86597","e04150b8f86342","e04150b8f865c5"};
int    bagsBT[9]={5,1,3,7,2,6,0,0,4};
//   //                  0     1    2    3    4    5    6    7 
//String bloodTypes[8]={"A+","0+","B+","AB+","A-","0-","B-","AB-"};
int blood;



int fase=0;
int work=3;
int xx,yy=0;
String action[2]={"LOAD","UNLOAD"};
uint8_t uid[10];

String topText[7]={"PLEASE SCAN EMPLOYEE BADGE ","","PATIENT FOUND","PLEASE SCAN BLOOD BAG!","","BYE!!","ERROR"};
String downText[7]={"TO BEGIN PROCESS               ","Please Scan Patient Info!   ","","","","PLEASE CLOSE THE CASE!!",""};
String canRec="";

void draw()
{
  

  sprite.fillSprite(TFT_BLACK);
  
  sprite.fillRect(6,32,308,2,0x134B+5);
  sprite.fillRect(6,62,270,1,grays[10]);
  
  sprite.setTextDatum(0);
  sprite.setTextColor(grays[5],TFT_BLACK),
  sprite.drawString(String(fase),310,0);
  sprite.setTextColor(grays[1],TFT_BLACK),
  sprite.loadFont(midleFont);
  sprite.drawString(topText[fase],6,4);
  //sprite.drawString("DARK IS "+String(digitalRead(3)),200,20,2);
  sprite.unloadFont();
  sprite.loadFont(smallFont);
  if(fase!=2){
    sprite.setTextColor(grays[6],TFT_BLACK);
    sprite.drawString(downText[fase],6,40,2);
  }
  sprite.unloadFont();
  sprite.setTextColor(grays[8],TFT_BLACK),
  sprite.drawString(card,6,160);
  
    if(fase)
  
   if(fase==2 || fase==3 || fase==4)
   {
   sprite.loadFont(tinyFont);
   sprite.setTextColor(grays[7],TFT_BLACK),
   sprite.drawString("CAN RECIEVE:",6,68,2);
  sprite.setTextColor(red,TFT_BLACK),
  sprite.drawString(canRec,90,68,2);
  sprite.unloadFont(); 
   }
  if(fase==2){
  sprite.fillSmoothRoundRect(10,100,140,50,3,blue,TFT_BLACK);
  sprite.fillSmoothRoundRect(170,100,140,50,3,blue,TFT_BLACK);
  sprite.loadFont(smallFont);
  sprite.setTextColor(grays[6],TFT_BLACK),
  sprite.drawString("DAY OF BIRTH:",6,40,2);
  sprite.drawString("BLOOD TYPE:",194,40,2);


  sprite.setTextColor(TFT_ORANGE,TFT_BLACK),
  sprite.drawString(downText[fase],108,40,2);
  sprite.setTextColor(red,TFT_BLACK),
  sprite.drawString(down2,288,40,2);

  sprite.unloadFont();
  sprite.setTextDatum(4);
  sprite.loadFont(midleFont);
  sprite.setTextColor(TFT_BLACK,blue);
  sprite.drawString("LOAD",75,130,4);
  sprite.drawString("UNLOAD",240,130,4);
  sprite.unloadFont();
  
  }

 
  if(fase==4){
  
  sprite.fillSmoothRoundRect(10,100,140,50,3,blue,TFT_BLACK);
  sprite.fillSmoothRoundRect(170,100,140,50,3,blue,TFT_BLACK);
  sprite.setTextDatum(4);
  sprite.loadFont(midleFont);
  sprite.setTextColor(TFT_BLACK,blue);
  sprite.drawString("YES",75,130,4);
  sprite.drawString("NO",240,130,4);
  sprite.unloadFont();
  
  }

 
  if(animation<15)
  if(fase==0 || fase==3 || fase==1)
  {
  sprite.setTextDatum(4);
  sprite.loadFont(bigFont);
  sprite.setTextColor(grays[9],TFT_BLACK);
  sprite.drawString("SCANING!",160,125);
  sprite.unloadFont();
  }

  
  sprite.pushSprite(0,0);
  if(fase==6) {delay(3000); fase=3;}
  
}

void setup() {
  pinMode(3,INPUT);

   pinMode(15,OUTPUT);
   digitalWrite(15,1);

   pinMode(38,OUTPUT);
   digitalWrite(38,1);



  pinMode(PIN_TOUCH_RES, OUTPUT);
  digitalWrite(PIN_TOUCH_RES, LOW);
  delay(500);
  digitalWrite(PIN_TOUCH_RES, HIGH);
  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
  delay(100);
  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_ORANGE);

    ledcSetup(0, 2000, 8);
    ledcAttachPin(PIN_LCD_BL, 0);
    ledcWrite(0, 60);
  
  sprite.createSprite(320, 170);
  sprite.setSwapBytes(1);

  Serial.begin(115200);
  Serial.println(F("PN5180 Hard-Reset..."));

  //        sck  mi  mo ss
  SPI.begin(11, 10, 3, 2);
  nfc14443.begin();

  Serial.println(F("----------------------------------"));
  nfc14443.reset();

  Serial.println(F("----------------------------------"));
  Serial.println(F("Reading product version..."));
  uint8_t productVersion[2];
  nfc14443.readEEprom(PRODUCT_VERSION, productVersion, sizeof(productVersion));
  Serial.print(F("Product version="));
  Serial.print(productVersion[1]);
  Serial.print(".");
  Serial.println(productVersion[0]);

  if (0xff == productVersion[1]) { // if product version 255, the initialization failed
    Serial.println(F("Initialization failed!?"));
    Serial.println(F("Press reset to restart..."));
    Serial.flush();
    exit(-1); // halt
  }
  
  Serial.println(F("----------------------------------"));
  Serial.println(F("Reading firmware version..."));
  uint8_t firmwareVersion[2];
  nfc14443.readEEprom(FIRMWARE_VERSION, firmwareVersion, sizeof(firmwareVersion));
  Serial.print(F("Firmware version="));
  Serial.print(firmwareVersion[1]);
  Serial.print(".");
  Serial.println(firmwareVersion[0]);

  Serial.println(F("----------------------------------"));
  Serial.println(F("Reading EEPROM version..."));
  uint8_t eepromVersion[2];
  nfc14443.readEEprom(EEPROM_VERSION, eepromVersion, sizeof(eepromVersion));
  Serial.print(F("EEPROM version="));
  Serial.print(eepromVersion[1]);
  Serial.print(".");
  Serial.println(eepromVersion[0]);

  Serial.println(F("----------------------------------"));
  Serial.println(F("Enable RF field..."));
  nfc14443.setupRF();

  delay(30);
  //esp_sleep_enable_ext0_wakeup((gpio_num_t)WAKEPIN,0);

   // generate 12 levels of gray
     int co=210;
     for(int i=0;i<12;i++)
     { grays[i]=tft.color565(co, co, co);
     co=co-20;}

  nfc15693.begin();
  nfc15693.reset();
  nfc15693.setupRF();

  restart();
}


void loop() {
 card="";


   if(fase==5)
   if (touch.read()) { 
      TP_Point t = touch.getPoint(0);
      xx=t.y;  yy=170-t.x;
      if(xx>5 &&  xx<100 && yy>5 && yy<70)
      restart();
   }
 
  if(fase==2)
   if (touch.read()) {
    if(deb==0){
      deb=1;
      TP_Point t = touch.getPoint(0);
      xx=t.y;  yy=170-t.x;
      fase=3;
      if(xx>10 && xx<150 && yy>80)
      work=0;
      if(xx>170 && xx<310 && yy>80)
      work=1;
   
    fase=3;
   
    downText[fase]=action[work]+" // "+patientsName[patient]+"  BT: "+bloodTypes[patientsBT[patient]];
    } else {
    deb=0;}
    }

    if(fase==4)
    if (touch.read()) {
    if(deb==0){
      deb=1;
      TP_Point t = touch.getPoint(0);
      xx=t.y;  yy=170-t.x;
      
      if(xx>10 && xx<150 && yy>100)
      fase=3;
      if(xx>170 && xx<310 && yy>100)
      fase=5;
    //downText[fase]="FOR "+patientsName[patient]+"  BT: "+bloodTypes[patientsBT[patient]];
    } else {
    deb=0;}
  }
 
 
  // try to read ISO15693 inventory
  ISO15693ErrorCode rc = nfc15693.getInventory(uid);
  
  if (rc == ISO15693_EC_OK) {
    id="";
    for (int i=0; i<8; i++) {
    id=id+String(uid[i],HEX); }
    
    if(fase==0)
    for(int i=0;i<5;i++)
    if(id==usersID[i]){
    fase=1;
    employee=i;
   
    topText[fase]="HELLO "+usersName[i];
    } else{card="UNKNOWN CARD DETECTED! "+id;}

    if(fase==1)
    for(int i=0;i<5;i++)
    if(id==patientsID[i]){
    fase=2;
    patient=i;
    
    topText[fase]="PATIENT: "+patientsName[patient];
    downText[fase]=petientsDOB[patient];
    down2=bloodTypes[patientsBT[patient]];
    canRec="";
    for(int j=0;j<8;j++)
    if(canRecieve[patientsBT[patient]][j]<8)
    canRec=canRec+bloodTypes[canRecieve[patientsBT[patient]][j]]+", ";
    } else{card="CARD DETECTED! "+id;}

    if(fase==3)
    for(int i=0;i<9;i++)
    if(id==bagsID[i]){
    fase=4;
    blood=i;
    card="";
    topText[fase]="SUCCESSFUL: "+ bloodTypes[bagsBT[blood]];

    bool match=0;
    for(int i=0;i<8;i++)
    {if(bagsBT[blood]==canRecieve[patientsBT[patient]][i])
    match=1;
    }
    if(match==1)
    {
      downText[fase]="DO YOU WANT TO "+action[work]+" ANOTHER BAG??";
      
    }else {fase=6; downText[fase]= "Patient can't recieve "+ bloodTypes[bagsBT[blood]];+" blood";
    }
    
    } else{card="CARD DETECTED! "+id;}
    
  }


//
//   if(digitalRead(3)==1)
//  {
//   fase=4;
//  }
 
 animation++;
 if(animation==30) animation=0;
 
 draw();
}

void restart()
{
  fase=0;
  downText[0]="TO BEGIN PROCESS   ";  
  id="";
  canRec="";
  employee=-1;
  patient=-1;
  work=-1;
  blood=-1;
  down2="";
}
