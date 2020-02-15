 /*
         Based on the example "Web client" from Arduino

         Circuit:
         * GSM shield attached to an Arduino
         * SIM card with a data plan

         created 8 Mar 2012
         by Tom Igoe
         - Modified 15 Nov 2014
         by Mateo Velez for Ubidots,Inc.

         http://arduino.cc/en/Tutorial/GSMExamplesWebClient

         */

        // libraries
        #include <GSM.h>

        const int waterSens = A0; //define water sensor
const int led = 9;//define led to pin 9
int waterVal; //define the water sensor value
int Buzzer = 8;
int tones[] = {261, 277, 293, 311, 329, 349, 369, 392, 415, 440, 466, 493, 523 , 554};


        // PIN Number
        #define PINNUMBER ""

        // APN data
        #define GPRS_APN       "claro.com.br" // replace with your GPRS APN
        #define GPRS_LOGIN     "claro"    // replace with your GPRS login
        #define GPRS_PASSWORD  "claro" // replace with your GPRS password

        // initialize the library instance
        GSMClient client;
        GPRS gprs;
        GSM gsmAccess(true);
        uint16_t reset = 0;


        String idvariable = "5c2ff714c03f974e37fc37af";   // replace with your Ubidots Variable ID
        String token = "A1E-u1caxq0fjhE6e7evKVgGechNmiio7i";  // replace with your Ubidots token

        void setup(){
          pinMode(led, OUTPUT); //set led as an output
          pinMode(waterSens, INPUT);//set water sensor as an input
          // initialize serial communications and wait for port to open:
          Serial.begin(9600);
          while (!Serial){
            ; // wait for serial port to connect. Needed for Leonardo only
          }
          Serial.println("Starting Arduino web client.");
          // connection state
          boolean notConnected = true;
         // After starting the modem with 
          // attach the shield to the GPRS network with the APN, login and password
          while (notConnected){
            if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &
                (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)){
              notConnected = false;
            }else{
              Serial.println("Not connected");
              delay(1000);
            }
          }       
        }

        void loop(){

        waterVal = analogRead(waterSens); //read the water sensor

       Serial.println(waterVal);
          
          if (waterVal >= 12){
          }
          else{
            reset++;
            if (reset == 10){
              asm volatile ("  jmp 0");    // reset the Arduino board if the data transmission fail
            }
          }       
          // if the server's disconnected, stop the client:
          if (!client.available() && !client.connected()){
            Serial.println();
            Serial.println("disconnecting.");
            client.stop();
            // do nothing forevermore:        
          }
        }
        boolean save_value(int value){
          Serial.println("connecting...");        
          int num=0;
          String var = "{\"value\":"+ String(value) + "}";
          num = var.length();
          // if you get a connection, report back via serial:
          if (client.connect("things.ubidots.com", 80)){
            Serial.println("connected");
            // Make a HTTP request:
            client.print("POST /api/v1.6/variables/"+idvariable+"/values HTTP/1.1\nContent-Type: application/json\nContent-Length: "+String(num)+"\nX-Auth-Token: "+token+"\nHost: things.ubidots.com\n\n");
            client.println(var);
            client.println();           
          }else{
            // if you didn't get a connection to the server:
            Serial.println("connection failed");
            return false;
          } 
            while (client.available())
          {         
            char c = client.read();
            Serial.print(c);
          }
        }
