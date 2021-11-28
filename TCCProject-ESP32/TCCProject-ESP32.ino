//LIBRARIES
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <ArduinoJson.h>

//DEFINE DATABASE OBJECTS
#define API_KEY "INSERIR A API KEY DO SEU FIREBASE" //substituir pelo seu
#define DATABASE_URL "INSERIR A URL DO SEU REALTIME DATABASE" //substituir pelo seu
#define USER_EMAIL "inserir seu gmail da conta do utilizada no Firebase" //substituir pelo seu
#define USER_PASSWORD "inserir a senha do seu gmail" //substituir pelo seu

#define DATABASE_PATCH "/caminho1/caminho2" //inserir o caminho dos seus dados

FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
FirebaseData fbdo;
FirebaseData fbdoStream;

//-------------------------------WIFI
#define WIFI_SSID "Inserir nome do seu WIFI aqui" //inserir o nome do seu wifi
#define WIFI_PASSWORD "Inserir senha do seu WIFI aqui" //inserir a senha do seu wifi
//observação, o ESP32 não aceita frequencia 5.0Ghz, deve ser 2.4Ghz

//-------------------------------Control Objects
String Command = "";
String Direction = "";
unsigned int Distance = 0;
String Unity = "";
String firebaseData = "";

bool delayFlag = false;
bool taskCompleted = false;
bool firstReadDone = false;
unsigned int calculatedDelay = 0;
unsigned int i=0;

//--------------------------------IO's
const int greenLed = 18;
const int yellowLed = 19; 
const int rightWheelOn = 26;
const int rightWheelOff = 25;
const int leftWheelOn = 17;
const int leftWheelOff = 16;


//------------------------------------------------------------------
//--------------------------SETUP-----------------------------------
//------------------------------------------------------------------

void setup(){
  //SETUP IOs
  Serial.begin(115200);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(rightWheelOn, OUTPUT);
  pinMode(rightWheelOff, OUTPUT);
  pinMode(leftWheelOn, OUTPUT);
  pinMode(leftWheelOff, OUTPUT);

  digitalWrite(rightWheelOn,LOW);
  digitalWrite(rightWheelOff,LOW);
  
  digitalWrite(leftWheelOn,LOW);
  digitalWrite(leftWheelOff,LOW);

  //CONNECT TO WIFI
  Serial.printf("Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  digitalWrite(yellowLed, HIGH);
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //SETUP DATABASE
  FirebaseSetup();

}

//------------------------------------------------------------------
//--------------------------LOOP------------------------------------
//------------------------------------------------------------------

void loop(){
  //CONNECT TO THE DATABASE
  if (Firebase.ready() && !taskCompleted){
    taskCompleted = true;
    Serial.println("...Connected to Realtime Database...");

  }if (taskCompleted == true && firstReadDone == true){
    if (delayFlag == true){
      
      if (i<= calculatedDelay){
        i++;
        delay(1);
      }else{
        stopChair();   
      }
    }else{
      
    }
  }
}

//------------------------------------------------------------------
//--------------------------FIREBASE STREAM-------------------------
//------------------------------------------------------------------

void streamCallback(FirebaseStream data){
  //Print out all information
  Serial.println("\nSTREAM DATA----------------");
  Serial.println(data.streamPath());
  Serial.println(data.dataPath());
  Serial.println(data.dataType());

  if(data.dataPath() == "/command"){
    Command = data.to<String>();
  }else if (data.dataPath() == "/direction"){
    Direction = data.to<String>();
  }else if (data.dataPath() == "/distance"){
    Distance = data.to<int>();
  }else if(data.dataPath() == "/unity"){
    Unity = data.to<String>();
  }else{
    FirebaseJson *json = data.to<FirebaseJson *>();
    firebaseData = json->raw();
    Serial.println(firebaseData);
    decodeJson();
  }

  Serial.println("\nCommand: " + Command);
  Serial.println("Direction: " + Direction);
  Serial.print("Distance: ");
  Serial.println(Distance);
  Serial.println("Unity: " + Unity);
  Serial.println("------------------------------");

  //IF FIRST READ WHEN BOOTING, DON'T DO COMMAND
  if (firstReadDone == false){
    firstReadDone = true;
    if(Command != "stopped")
      stopChair ();
    digitalWrite(greenLed, HIGH);
  }else{
    //DO COMMAND IF NOT THE FIRST READ
    processCommand();
  }
  
}

void streamTimeoutCallback(bool timeout){
  if(timeout){
    //Stream timeout occurred
    Serial.println("Stream timeout, resume streaming...");
    digitalWrite(greenLed, LOW);
    ESP.restart();
  }
}


//------------------------------------------------------------------
//--------------------------PROCESS COMMAND-------------------------
//------------------------------------------------------------------

void processCommand (){
    if(Command == "stop"){
      stopChair();
      
    }else if (Command == "move"){
      moveChair();
      
    }else if (Command == "turn"){
      turnChair();
      
    }else if (Command == "reset"){
      digitalWrite(greenLed, LOW);
      ESP.restart();
    }
}

//------------------------------------------------------------------
//--------------------------MOVE FUNCTIONS--------------------------
//------------------------------------------------------------------

//STOP FUNCTION-----------------------------------------------
void stopChair(){
  Serial.println("\nSTOP COMMAND----------------------- \n\n");

  delayFlag = false;
  i=0;
  calculatedDelay=0;
  
  digitalWrite(rightWheelOn,LOW);
  digitalWrite(rightWheelOff,LOW);
  
  digitalWrite(leftWheelOn,LOW);
  digitalWrite(leftWheelOff,LOW);
  
  updateFirebase("stopped","",0,"");
}


//MOVE FUNCTION-----------------------------------------------
void moveChair(){
  Serial.println("\nMOVE COMMAND-----------------------\n\n");
  calculateDelay();
  
  if(Direction == "frente"){
    digitalWrite(rightWheelOn, HIGH);
    digitalWrite(rightWheelOff,LOW);
      
    digitalWrite(leftWheelOn,HIGH);
    digitalWrite(leftWheelOff,LOW);
  }

  else if(Direction == "tras" || Direction == "trás"){
    digitalWrite(rightWheelOn,LOW);
    digitalWrite(rightWheelOff,HIGH);
    
    digitalWrite(leftWheelOn,LOW);
    digitalWrite(leftWheelOff,HIGH);
  }

  delayFlag = true;
  
}


//TURN FUNCTION-----------------------------------------------
void turnChair(){
  Serial.println("\nTURN COMMAND-----------------------\n\n");
  calculateDelay();
  
  if(Direction == "direita"){
    digitalWrite(rightWheelOn, LOW);
    digitalWrite(rightWheelOff,HIGH);
      
    digitalWrite(leftWheelOn,HIGH);
    digitalWrite(leftWheelOff,LOW);
  }

  else if(Direction == "esquerda"){
    digitalWrite(rightWheelOn, HIGH);
    digitalWrite(rightWheelOff,LOW);
      
    digitalWrite(leftWheelOn,LOW);
    digitalWrite(leftWheelOff,HIGH);
  }

  delayFlag = true;
  
}



//------------------------------------------------------------------
//--------------------------DELAY FUNCTIONS-------------------------
//------------------------------------------------------------------

void calculateDelay(){
  
  if(Unity == "ms"){
    calculatedDelay = Distance;
    
  }else if(Unity == "s"){
    calculatedDelay = 1000 * Distance;
    
  }else if(Unity == "cm"){
    calculatedDelay = (100 * Distance)/4;
    //delay(100*Distance);
    
  }else if (Unity == "m"){
    Distance = Distance * 100;
    calculatedDelay = (100 * Distance)/4;
    
  }else if (Unity == "graus"){
    calculatedDelay = (50 * Distance)/13;
    
  }else {}
  
}


//------------------------------------------------------------------
//--------------------------MY DATABASE FUNCTIONS-------------------
//------------------------------------------------------------------

void updateFirebase(const char* CommandDB, const char* DirectionDB, int DistanceDB, const char* UnityDB){
  Serial.println("\nUPDATE DATA------------------------------");
  FirebaseJson updateData;

  updateData.add("command", CommandDB);
  updateData.add("direction", DirectionDB);
  updateData.add("distance", DistanceDB);
  updateData.add("unity", UnityDB);

  if (Firebase.RTDB.updateNodeSilent(&fbdo, DATABASE_PATCH, &updateData)) {
    Serial.println(fbdo.dataPath());
    Serial.println(fbdo.dataType());
    Serial.println(fbdo.jsonString()); 
  } else {
    Serial.println(fbdo.errorReason());
  }
  
  
  Serial.println("------------------------------");
}


//------------------------------------------------------------------
//--------------------------AUXILIARY AND LYBRARY FUNCTIONS---------
//------------------------------------------------------------------
void decodeJson(){
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, firebaseData);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    
    const char* CommandJson = doc["command"];
    const char* DirectionJson = doc["direction"];
    int DistanceJson = doc["distance"];
    const char* UnityJson = doc["unity"];

    Command = CommandJson;
    Direction = DirectionJson;
    Distance = DistanceJson;
    Unity = UnityJson;
}



//------------------------------------------------------------------
//---------------------------FIREBASE SETUP-------------------------
//------------------------------------------------------------------
void FirebaseSetup(){
  //---------------ASSIGNMENTS
  //--------------------------
  
  //Assign the api key (required)
  config.api_key = API_KEY;

  //Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  //Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  //Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h


  //BEGIN FIREBASE REALTIME DATABASE
  //--------------------------------
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);


  //----------------------------------------FIREBASE CONFIGS AND CALLBACKS
  //----------------------------------------------------------------------
  //Set the size of HTTP response buffers in the case where we want to work with large data.
  fbdo.setResponseSize(1024);

  //Set database read timeout to 30 seconds (max 15 minutes)
  Firebase.RTDB.setReadTimeout(&fbdo, 1000 * 30);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.RTDB.setwriteSizeLimit(&fbdo, "tiny");

  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);


  //--------------DATABASE CHANGES LISTENER / STREAM
  //------------------------------------------------
  Firebase.RTDB.setStreamCallback(&fbdoStream, streamCallback, streamTimeoutCallback);
  if (!Firebase.RTDB.beginStream(&fbdoStream, DATABASE_PATCH))
  {
    Serial.println(fbdoStream.errorReason());
  }
}
