/*
 * beatomagic.c
 *
 * Copyright 2016 belese <besme2@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
//nombre de temps
#define number_of_beat = 8
#define time_before_next_function = 3000

//*******Define pin number here*******//

//register pin number
int dataPin = 9;
int latchPin = 8;
int clockPin = 7;

//track input pin
int tracks=[] = {10,11,12,13};

//solenoid output pin, must have the same lenght than tracks array
int solenoid[] = {1,2,3,4}

//start/stop Button
int startPin = 5

//tempo potentionmetre
int tempoPin = A0
//**********************************//

//********Define constant solenoid value here*****//
//time a solenoid must be on in ms
int delayOn[] = {80,80,80,80}
int preTime[] = {0,0,0,0}


//************************************************//

//nombre de piste
int number_of_track;
boolean state[number_of_track];

#define PAUSE = 1
#define PLAY = 2
#define SETUP = 3
#define USB = 4
int status;


//buuton on/off info
boolean isPressed = false
  
//beat info
long nextbeat = 0;
int currentbeat = 0;
int totalbeat = number_of_beat;

//solenoid info
boolean solstatus[number_of_track];



void setup() {
  Serial.begin(9600);
  Serial.println("reset");

  pinMode(dataPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);

  number_of_track = sizeof(tracks)/sizeof(int)

  for (int i;i<number_of_track;i++){
      pinMode(tracks[i],INPUT);
  }

  for (int i;i<number_of_track;i++){
      pinMode(solenoid[i],OUTPUT);
      digitalWrite(solenoid[i],LOW);
  }
}

void loop (){
    for(int i=0;i<number_of_beat;i++){
        if digitalRead(startPin)
            delay(20)
            break;

        Serial.println("je joue le temps %d"%i+1);
        //lire l'etat des switch
        getSwitchState(i)
        //activer les solenoids correspondant pendant delayon ms
        solenoid(HIGH)
        delay(delayOn)
        solenoid(LOW)

        for(int j=0;j<readTempo()/10;j++){
           if digitalRead(startPin)
               break;
           delay(readTempo()/10)
        }
    }
}


//verifie l'etat du bouotton on/off et usb
//et active l'etat correspondant
//return True si l'etat a été modifier, False sinon

int checkState()
{
   if (serial)
   {
     //usb cable is plugged to arduino
     if (status != USB)
     {
       status = USB;
       return true;
     }
       
   }
   else
   {
     if (status == USB)
     {
       status = PAUSE
       return true
     }
   }
   
   if (digitalRead(startPin))
   {
      //button is pressed
      if (not isPressed)
      {       
          isPressed = True;
          pressTime = millis();        
      }
   }
   else
   {
      //button is released
      if (isPressed)
      {
          //button was pressed and is released
          //check wich status will be next
          isPressed = false;
          if ((pressTime + time_before_next_function) > millis)
          {
              //long Press
              switch (status) 
              {
   				 case PAUSE:
      				//pass in setup mode
                    status = SETUP;
                    return True;      			
    			 case PLAY:
      				//record the state of switch here 
      				break;
                 case SETUP:
                    //save config here and return in pause mode                    
                    status = PAUSE
                    break;
  			   }
               
          }
          else
          {
              //short press
                 case PAUSE:
      				//pass in play mode
                    status = PLAY
                    return True      				
    			 case PLAY:
      				//pass in pause mode
                    status = PAUSE
                    return True      				
                 case SETUP:
                    //cancel setup here and return in pause mode                    
                    status = PAUSE
                    break;
                     
          }
          
      }          
   }
   return false      
}

void checkBeat()
{
  //check the beat
  if (nextbeat => millis())
  {
    //we are on next bit
    nextbeat = millis() + getTempo();
    currentbeat++;
    if (currentbeat == totalbeat)
      currentbeat = 0;
  }
}


void checkSolenoid(int id)
{
  //verifie si le solenoid doit etre activé ou stopper
  if (solstatus[id] == false)
  {
      if (nextbeat - preTime[id] >= millis())
      {
           if (checkSwitch(id))
           {
                //we have to run this solenoid
                solstatus[id] = true;
                digitalWrite(solenoid[id],HIGH);
           }
      }
  }
  else
  {
      if (nextbeat - preTime[id] + delayOn[id] >= millis())
      {
           //we have to stop this solenoid
           solstatus[id] = false;
           digitalWrite(solenoid[id],LOW);           
      }
  }  
}



void checkTempo(){
}



//Utils
void setShiftRegister(int value){
  //mettre le latch pin  a low pour pouvoir envoyer des valeurs en sÃ©rie au shift register
    digitalWrite(latchPin, LOW);
    //Activer la colonne correspondant aux temp actuel
    shiftOut(dataPin, clockPin,MSBFIRST,value);
    //rendre cela effectif sur le shift register en mettant le latchpin a High
    digitalWrite(latchPin, HIGH);
}

boolean getSwitchState(int pos){
  return digitalRead(tracks[pos])   
}


void solenoid(boolean state){
  for (int i=0;j<number_of_track,i++){
     if(state[i])
        digitalWrite(solenoid[j],state);
  }
}

int readTempo (){
  //PotentiomÃ¨tre>tempo
  //chaque chose en son temps
  int tempo =100;
  //tempo = analogRead(tempoPin);
  //tempo = map(tempo, 0, 1023, 0, 255);
  //delay(10);
  return tempo;
}