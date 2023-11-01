/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  NOTE: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6A0x26sZY"
#define BLYNK_TEMPLATE_NAME "Percobaan"
#define BLYNK_AUTH_TOKEN "h0KvaLQEnC8Az6xf6qmUPgAo3OVdYEWl"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "avisalt2";
char pass[] = "TMsoftware";

Servo servoGripper;
Servo servoJointR;
Servo servoJointL;
Servo servoJointC;

boolean stateRecord = false;
boolean statePlay = false;

int gripper_pos, jointR_pos, jointL_pos, jointC_pos;
int g_gripper_pos, x_jointR_pos, y_jointL_pos, z_jointC_pos;

int pos = 90;

int saved_data[900];
int array_index = 0;

int action_pos;
int action_servo;

void readData()
{
  gripper_pos = servoGripper.read();
  jointR_pos  = servoJointR.read();
  jointL_pos  = servoJointL.read();
  jointC_pos  = servoJointC.read();
}

void recordData()
{ 
  readData();

   g_gripper_pos = gripper_pos;
   x_jointR_pos = jointR_pos;
   y_jointL_pos = jointL_pos;
   z_jointC_pos = jointC_pos;

  readData();

   if(g_gripper_pos == gripper_pos)
   {
     servoGripper.write(gripper_pos);
     saved_data[array_index] = gripper_pos + 0; //Save the new position to the array.
     array_index++;       
   }

   if(x_jointR_pos == jointR_pos)
   {
     servoJointR.write(jointR_pos);
     saved_data[array_index] = jointR_pos + 1000; //1000 is added for 1st servo motor as differentiator
     array_index++;      
   }

   if(y_jointL_pos == jointL_pos)
   {
     servoJointL.write(jointL_pos);
     saved_data[array_index] = jointL_pos + 2000; 
     array_index++;       
   }

   if(z_jointC_pos == jointC_pos)
   {
     servoJointC.write(jointC_pos);
     saved_data[array_index] = jointC_pos + 3000; 
     array_index++;       
   }
   
   //Print the value for debugging 
   Serial.print(gripper_pos);  
   Serial.print("  "); 
   Serial.print(jointR_pos); 
   Serial.print("  "); 
   Serial.print(jointL_pos); 
   Serial.print("  "); 
   Serial.println(jointC_pos);
   Serial.print ("Index = "); 
   Serial.println (array_index); 
   delay(100);
}

void playData()
{
  for (int Play_action=0; Play_action<array_index; Play_action++) //Navigate through every saved element in the array 
  {
    action_servo = saved_data[Play_action] / 1000; //The fist character of the array element is split for knowing the servo number
    action_pos = saved_data[Play_action] % 1000; //The last three characters of the array element is split to know the servo postion 

    switch(action_servo)
    {
      case 0: 
        servoGripper.write(action_pos);
      break;

      case 1:
        servoJointR.write(action_pos);
      break;
      
      case 2:
        servoJointL.write(action_pos);
      break;

      case 3:
        servoJointC.write(action_pos);
      break;
    }
    /*
    //Print the value for debugging 
    Serial.print("Play_action = ");
    Serial.print( Play_action);  
    Serial.print("  "); 
    Serial.print(" servoNum = ");
    Serial.print( action_servo); 
    Serial.print("  "); 
    Serial.print(" servoPos = "); 
    Serial.print( action_pos);
    Serial.print (" Index = "); 
    Serial.println ( array_index); 
    delay(50);
    */
  }
  
}

//dataStream V0 sd V3 Slider integer 0 sd 180 
BLYNK_WRITE(V0)
{
  int gripper = param.asInt();
  servoGripper.write(gripper);
}

BLYNK_WRITE(V1)
{
  servoJointR.write(param.asInt());
}

BLYNK_WRITE(V2)
{
  servoJointL.write(param.asInt());
}

BLYNK_WRITE(V3)
{
  servoJointC.write(param.asInt());
}


BLYNK_WRITE(V4)
{
  int pinValue = param.asInt();
  if(pinValue==1)
  {
    stateRecord=true;
  }
  else
  {
    stateRecord=false;
  }
}

BLYNK_WRITE(V5)
{
 if(param.asInt()==1)
 {
  statePlay=true; 
 }
 else
 {
  statePlay=false;
 }

}

BLYNK_WRITE(V6)
{
  if(param.asInt()==1)
  {
    
    servoGripper.write(pos);
    servoJointR.write(pos);
    servoJointL.write(pos);
    servoJointC.write(pos);
    
    Blynk.virtualWrite(V0, pos);
    Blynk.virtualWrite(V1, pos);
    Blynk.virtualWrite(V2, pos);
    Blynk.virtualWrite(V3, pos);
    
    array_index=0;
  }
  
   /*  
   //Print the value for debugging 
   Serial.print(gripper_pos);  
   Serial.print("  "); 
   Serial.print(jointR_pos); 
   Serial.print("  "); 
   Serial.print(jointL_pos); 
   Serial.print("  "); 
   Serial.println(jointC_pos);
   Serial.print ("Index = "); 
   Serial.println (array_index); 
   delay(100);
   */

}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
 
  servoGripper.attach(5);  //D1
  servoJointR.attach(4);   //D2
  servoJointL.attach(2);   //D4
  servoJointC.attach(14);  //D5
   
  Blynk.virtualWrite(V0, pos);
  Blynk.virtualWrite(V1, pos);
  Blynk.virtualWrite(V2, pos);
  Blynk.virtualWrite(V3, pos);
  
}

void loop()
{
  
  Blynk.run();
  if(stateRecord==true)
  {
    recordData();
  }
  if(statePlay==true)
  {
    playData();
  }
}
