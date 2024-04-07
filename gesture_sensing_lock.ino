#include <Wire.h>
#include "paj7620.h"
#include <LiquidCrystal.h>
#define GES_REACTION_TIME		500				// You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME			800				// When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
#define GES_QUIT_TIME			1000
int Contrast = 60;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char code[4];

int keycount = 0 ;
int wrongcount = 0 ;

byte heart[8] = 

              {

                0b00000,

                0b01010,

                0b11111,

                0b11111,

                0b11111,

                0b01110,

                0b00100,

                0b00000

              };


byte smile[8] = 

              {

                0b00000,

                0b00000,

                0b01010,

                0b00000,

                0b10001,

                0b01110,

                0b00000,

                0b00000

              };

void setup()
{
  analogWrite(6, Contrast);
  lcd.begin(16, 2);

	uint8_t error = 0;

	Serial.begin(9600);
	Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

	error = paj7620Init();			// initialize Paj7620 registers
	if (error) 
	{
		Serial.print("INIT ERROR,CODE:");
		Serial.println(error);
	}
	else
	{
		Serial.println("INIT OK");
	}
  lcd.setCursor(0, 0);
  lcd.print("Enter gesture");

	Serial.println("Please input your gestures:\n");

   lcd.createChar(1, heart);

  lcd.createChar(2, smile);
    lcd.setCursor(0, 1);
  lcd.write(1);

}

void loop()
{
  char psd[4] = {'1','2','3','4'};
	uint8_t data = 0, data1 = 0, error;
	error = paj7620ReadReg(0x43, 1, &data);				// Read Bank_0_Reg_0x43/0x44 for gesture result.
	if (!error) 
	{
		switch (data) 									// When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
		{
			case GES_RIGHT_FLAG:
				delay(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Forward");
            code[keycount] = '0';
            keycount++;
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
            lcd.clear();
            code[keycount] = '1';
            keycount++;
            
            lcd.setCursor(0, 0);
            lcd.print("Backward");

					delay(GES_QUIT_TIME);
				}
				else
				{ 
            lcd.clear();
          keycount = 0;
           lcd.setCursor(0, 0);
            lcd.print("Reset");
            for(int i = 0 ; i < 4 ; i++){
              code[i] = '9';
            }

					Serial.println("Right");
				}          
				break;
			case GES_LEFT_FLAG: 
				delay(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{ 
            lcd.clear();
            code[keycount] = '0';
            keycount++;
          
          lcd.setCursor(0, 0);
          lcd.print("Forward");

					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
            lcd.clear();
            code[keycount] = '1';
            keycount++;

            lcd.setCursor(0, 0);
            lcd.print("Backward");

					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
            lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("Show Result");

					Serial.println("Left");
          if(code[0]=='5'&& code[1]=='6'&& code[2]=='2' && code[3]=='4'&&wrongcount<2){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Unlocked");
            keycount = 0 ;
            for(int i = 0 ; i < 4 ; i++){
              code[i] = '9';
            }
            lcd.write(2);

					Serial.println("Unclocked");
          }
          else{
            if(wrongcount>1){
              lcd.setCursor(0,0);
              lcd.print("Completely locked");
              keycount = 0 ;
              for(int i = 0 ; i < 4 ; i++){
              code[i] = '9';
            }
            }
            else if(wrongcount>0){
              lcd.setCursor(0,0);
              lcd.print("Security alert");
              wrongcount++;
              keycount = 0 ;
              for(int i = 0 ; i < 4 ; i++){
              code[i] = '9';
            }
            }
            
            else{
            lcd.setCursor(0, 0);
            lcd.print("Wrong,locked");
            wrongcount++;
					Serial.println("Wrong locked");
          keycount = 0;
          for(int i = 0 ; i < 4 ; i++){
              code[i] = '9';
            }
          }
				}
        }          
				break;
			case GES_UP_FLAG:
				delay(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{
            lcd.clear();
            code[keycount] = '0';
            keycount++;

            lcd.setCursor(0, 0);
            lcd.print("Forward");

					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
            lcd.clear();
            code[keycount] = '1';
            keycount++;

            lcd.setCursor(0, 0);
            lcd.print("Backward");

					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
            lcd.clear();
            code[keycount] = '5';
            keycount++;

            lcd.setCursor(0, 0);
            lcd.print("Up");

					Serial.println("Up");
				}          
				break;
			case GES_DOWN_FLAG:
				delay(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{
            lcd.clear();
            code[keycount] = '0';
            keycount++;
            

            lcd.setCursor(0, 0);
            lcd.print("Forward");

					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
            lcd.clear();
            code[keycount] = '1';
            keycount++;
         
            lcd.setCursor(0, 0);
            lcd.print("Backward");

					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
            lcd.clear();
            code[keycount] = '6';
            keycount++;

            lcd.setCursor(0, 0);
            lcd.print("Down");

					Serial.println("Down");
				}          
				break;
			case GES_FORWARD_FLAG:
              lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Forward");
            code[keycount] = '0';
            keycount++;

				Serial.println("Forward");
				delay(GES_QUIT_TIME);
				break;
			case GES_BACKWARD_FLAG:	
            lcd.clear();
            code[keycount] = '1';
            keycount++;

          lcd.setCursor(0, 0);
          lcd.print("Backward");
	  
				Serial.println("Backward");
				delay(GES_QUIT_TIME);
				break;
			case GES_CLOCKWISE_FLAG:
            lcd.clear();
            code[keycount] = '2';
            keycount++;

        lcd.setCursor(0, 0);
        lcd.print("Clockwise");

				Serial.println("Clockwise");
				break;
			case GES_COUNT_CLOCKWISE_FLAG:
            lcd.clear();
            code[keycount] = '3';
            keycount++;

        lcd.setCursor(0, 0);
        lcd.print("anti-clockwise");

				Serial.println("anti-clockwise");
				break;  
			default:
				paj7620ReadReg(0x44, 1, &data1);
				if (data1 == GES_WAVE_FLAG) 
				{
            lcd.clear();
            code[keycount] = '4';
            keycount++;

            lcd.setCursor(0, 0);
            lcd.print("wave");

					Serial.println("wave");
				}
				break;
		}
	}
	delay(100);
}
