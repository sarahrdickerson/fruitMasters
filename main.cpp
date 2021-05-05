// main.cpp
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 1/16/2021 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* 
 Copyright 2021 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 47k  resistor DAC bit 0 on PB0 (least significant bit)
// 24k  resistor DAC bit 1 on PB1
// 12k  resistor DAC bit 2 on PB2
// 6k   resistor DAC bit 3 on PB3 
// 3k   resistor DAC bit 4 on PB4 
// 1.5k resistor DAC bit 5 on PB5 (most significant bit)

// VCC   3.3V power to OLED
// GND   ground
// SCL   PD0 I2C clock (add 1.5k resistor from SCL to 3.3V)
// SDA   PD1 I2C data

//************WARNING***********
// The LaunchPad has PB7 connected to PD1, PB6 connected to PD0
// Option 1) do not use PB7 and PB6
// Option 2) remove 0-ohm resistors R9 R10 on LaunchPad
//******************************

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "TExaS.h"
#include "SSD1306.h"
#include "Random.h"
#include "Switch.h"
#include "SlidePot.h"
#include "Images.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Timer2.h"
#include "sprite.h"
#include "fruit.h"
#include "fruitImage.h"
#include "Sound.h"
#include "bomb.h"
#include "player.h"
#include "arrow.h"
#include "print.h"
//********************************************************************************
// debuging profile, pick up to 7 unused bits and send to Logic Analyzer
#define PA54                  (*((volatile uint32_t *)0x400040C0)) // bits 5-4
#define PF321                 (*((volatile uint32_t *)0x40025038)) // bits 3-1
// use for debugging profile
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PA5       (*((volatile uint32_t *)0x40004080)) 
#define PA4       (*((volatile uint32_t *)0x40004040)) 

const char InstructionsEnglish[] = "    INSTRUCTIONS:  1\xAF";
const char InstructionsSpanish[] = "   INSTRUCCIONES: 1\xAF";
const char Dir1English[] = "-Try to shoot as \n many fruits as\n possible.";
const char Dir1Spanish[] = "-Intenta disparar la\n mayor cantidad de\n frutas posible.";
const char Dir2English[] = "-The game is over if\n you shoot the bomb\n or miss 3 fruits.";
const char Dir2Spanish[] = "-El juego se termina\n si disparas a la\n bomba o fallas\n disparar a 3 frutas.";
const char ControlsEnglish[] = "   GAME CONTROLS:  1\xAF";
const char ControlsSpanish[] = "  MANDOS DE JUEGO: 1\xAF";
const char ShootEnglish[] = "-Press 1 to shoot";
const char ShootSpanish[] = "-Presiona 1 para\n disparar";
const char PauseEnglish[] = "-Press 2 to pause";
const char PauseSpanish[] = "-Presiona 2 para\n pausar";
const char MoveSlidepotEnglish[] = "-Move slidepot to\n move player";
const char MoveSlidepotSpanish[] = "-Mueva el deslizador\n para mover jugador";
const char GameOverEnglish[] = "      GAME OVER!";
const char GameOverSpanish[] = "  \xAD" "JUEGO TERMINADO!";
const char ScoreEnglish[] = "       SCORE: ";
const char ScoreSpanish[] = "      PUNTAJE: ";
const char NewHighScoreEnglish[] = "NEW HIGH SCORE!: ";
const char NewHighScoreSpanish[] = "\xAD" "NUEVO R\x90" "CORD!: ";
const char HighScoreEnglish[] = "HIGH SCORE: ";
const char HighScoreSpanish[] = "   R\x90" "CORD: ";
const char PlayAgainEnglish[] = "PRESS 3 TO PLAY AGAIN";
const char PlayAgainSpanish[] = "PRESIONA 3 PARA JUGAR\n       DE NUEVO";
const char InGameScoreEnglish[] = "Score:";
const char InGameScoreSpanish[] = "Puntaje:";
const char InGameLivesEnglish[] = "Lives:";
const char InGameLivesSpanish[] = "Vidas:";
	
const char* Phrases[14][2] = 
{ {InstructionsEnglish, InstructionsSpanish},
	{Dir1English, Dir1Spanish},
	{Dir2English, Dir2Spanish},
	{ControlsEnglish, ControlsSpanish},
	{ShootEnglish, ShootSpanish},
	{PauseEnglish, PauseSpanish},
	{MoveSlidepotEnglish, MoveSlidepotSpanish},
	{GameOverEnglish, GameOverSpanish},
	{ScoreEnglish, ScoreSpanish},
	{NewHighScoreEnglish, NewHighScoreSpanish},
	{HighScoreEnglish, HighScoreSpanish},
	{PlayAgainEnglish, PlayAgainSpanish},
	{InGameScoreEnglish, InGameScoreSpanish},
	{InGameLivesEnglish, InGameLivesSpanish}
};

// TExaSdisplay logic analyzer shows 7 bits 0,PA5,PA4,PF3,PF2,PF1,0 
void LogicAnalyzerTask(void){
  UART0_DR_R = 0x80|PF321|PA54; // sends at 10kHz
}
void ScopeTask(void){  // called 10k/sec
  UART0_DR_R = (ADC1_SSFIFO3_R>>4); // send ADC to TExaSdisplay
}
void Profile_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x21;      // activate port A,F
  while((SYSCTL_PRGPIO_R&0x20) != 0x20){};
  GPIO_PORTF_DIR_R |=  0x0E;   // output on PF3,2,1 
  GPIO_PORTF_DEN_R |=  0x0E;   // enable digital I/O on PF3,2,1
  GPIO_PORTA_DIR_R |=  0x30;   // output on PA4 PA5
  GPIO_PORTA_DEN_R |=  0x30;   // enable on PA4 PA5  
}
//********************************************************************************
 

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

void Init(void);
void Draw(void);
void Move(void);



void SysTick_Init(){
  //*** students write this ******
		NVIC_ST_CTRL_R = 0;
		NVIC_ST_CURRENT_R = 0;
		NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)| 0x20000000;
		
		NVIC_ST_CTRL_R = 7; //enable interrupts
		NVIC_ST_RELOAD_R = 4000000-1; //set period
}

int flag = 0x01; //flag = 1: not paused, flag = 0: need to pause
int highScore = 0;
int NeedtoDraw = 0;
int numLives = 3;
int lang;

arrow_t arrows[18];
fruit_t fruits[20];
int totalPoints;
player_t player;
bomb_t Bomb;
int main(void){//uint32_t time=0;
  DisableInterrupts();
  // pick one of the following three lines, all three set to 80 MHz
  //PLL_Init();                   // 1) call to have no TExaS debugging
  TExaS_Init(&LogicAnalyzerTask); // 2) call to activate logic analyzer
  //TExaS_Init(&ScopeTask);       // or 3) call to activate analog scope PD2
  SSD1306_Init(SSD1306_SWITCHCAPVCC);
  SSD1306_OutClear();   
  Random_Init(34);
  Profile_Init(); // PB5,PB4,PF3,PF2,PF1 
  SSD1306_ClearBuffer();
	SysTick_Init();
	ADC_Init(4);
	Switch_Init();
	Sound_Init();
  SSD1306_DrawBMP(2, 62, fruit_master, 0, SSD1306_WHITE);
	SSD1306_OutBuffer();
	Delay100ms(30);
	
	SSD1306_OutClear();
	SSD1306_SetCursor(3,0);
	SSD1306_OutString((char*)"SELECT LANGUAGE");
	SSD1306_SetCursor(1, 2);
	SSD1306_OutString((char*)"SELECCIONE UN IDIOMA");	
	SSD1306_SetCursor(0,4);
	SSD1306_OutString((char*)"- PRESS 1 FOR ENGLISH");
	SSD1306_SetCursor(0,6);
	SSD1306_OutString((char*)"- PRESIONA 2 PARA\n  ESPA\xA5""OL");
	
		int val = Switch_In();
		static int lastMain = 0;
		while(!(lastMain == 0 && ((val&0x01) == 0x01 ||(val&0x02) == 0x02))){
			val = Switch_In();
		}
		lang = val - 1; //spanish = 1; english = 0
		
	SSD1306_OutClear();
	SSD1306_SetCursor(0,0);
	
		/*
	if(lang == 1){
		SSD1306_OutString((char*)"INSTRUCCIONES:");	
	}
	else{
		SSD1306_OutString((char*)"INSTRUCTIONS:");	
	}
		*/
	SSD1306_OutString((char*)Phrases[0][lang]);
	
	SSD1306_SetCursor(0,1);
	//SSD1306_OutString((char*)"Try to shoot as many fruits as possible.");
	SSD1306_OutString((char*)Phrases[1][lang]);
	SSD1306_SetCursor(0,4);
	//SSD1306_OutString((char*)"The game is over if you shoot the bomb or miss 3 fruits.");
	SSD1306_OutString((char*)Phrases[2][lang]);
	SSD1306_SetCursor(0,7);
	Delay100ms(10);	
	
	static int lastPlay = 0;
	while(!(lastPlay == 0 && (Switch_In()&0x01) == 0x01)){
	}
	
	SSD1306_OutClear();
	SSD1306_SetCursor(0,0);
	/*
	if(lang == 0){
		SSD1306_OutString((char*)"GAME CONTROLS:");
	}
	else {
		SSD1306_OutString((char*)"MANDOS DE JUEGO:");
	}
	*/
	SSD1306_OutString((char*)Phrases[3][lang]);
		
	SSD1306_SetCursor(0,2);
	//SSD1306_OutString((char*)"-Press 1 to shoot");
	SSD1306_OutString((char*)Phrases[4][lang]);
	SSD1306_SetCursor(0,4);
	//SSD1306_OutString((char*)"-Press 2 to pause");
	SSD1306_OutString((char*)Phrases[5][lang]);
	SSD1306_SetCursor(0,6);
	//SSD1306_OutString((char*)"-Move slidepot to move player");
	SSD1306_OutString((char*)Phrases[6][lang]);
	Delay100ms(10);
	
	static int lastOneFr = 0;
	while(!(lastOneFr == 0 && (Switch_In()&0x01) == 0x01)){
	}
	
	while(1){
		EnableInterrupts();
		Init();
		while(player.get_status() == alive){
			if(flag == 0){		
				SSD1306_SetCursor(19,0);
				SSD1306_OutString((char*)"\xDD" "\xDD");
			} else{
				SSD1306_SetCursor(19,0);
				SSD1306_OutString((char*)"  ");
			}			
			if(NeedtoDraw){
				Draw();
			}
			//while(flag == 0){}
				
		}
		
		SSD1306_OutClear();
		SSD1306_ClearBuffer();
		//DisableInterrupts();
		SSD1306_SetCursor(0,0);
		//SSD1306_OutString((char*)"GAME OVER!");		
		SSD1306_OutString((char*)Phrases[7][lang]);
		SSD1306_SetCursor(0,2);
		//SSD1306_OutString((char*)"SCORE:");
		SSD1306_OutString((char*)Phrases[8][lang]);
		LCD_OutDec(player.getPoints());
		
		if(player.getPoints() > highScore){
			SSD1306_SetCursor(2,4);
			highScore = player.getPoints();
			//SSD1306_OutString((char*)"NEW HIGH SCORE!:");
			SSD1306_OutString((char*)Phrases[9][lang]);
			
		} else{
			SSD1306_SetCursor(4,4);
			//SSD1306_OutString((char*)"HIGH SCORE:");
			SSD1306_OutString((char*)Phrases[10][lang]);
		}
			LCD_OutDec(highScore);
		
		SSD1306_SetCursor(0,6);
		//SSD1306_OutString((char*)"PRESS 3 TO PLAY AGAIN");
		SSD1306_OutString((char*)Phrases[11][lang]);
		
		Delay100ms(5);
		DisableInterrupts();

		static int last = 0;
		while(!(last == 0 && (Switch_In()&0x04) == 0x04)){
		}
	}
}

// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}

int time;
void Init(){
	time = 0;
	//initialize player 
	Bomb.Init((Random() % 110)+5, 80, 1, -1, dead, 0);
	player.Init(60,60,0,0,alive,1);
	
	for(int i = 0; i < 18; i++){
		arrows[i].Init(0,0,0,0,dead,0);
	}
	//***********initialize all 20 fruits*******************
	
	//5 of them are "alive" right now
	for(int i = 0; i < 2; i++){
		int randNum = (Random() + Random() + Random())% 5;
		if(i%2 == 0)
			fruits[i].Init(((Random()%110)+5), 76, 1, -1, alive, randNum);
		else	
			fruits[i].Init(((Random()%110)+5), 76, -1, -1, alive, randNum);
	}
	
	for(int i = 2; i < 20; i++){
		int randNum = (Random() + Random() + Random())% 5;
		if(i%2 == 0)
			fruits[i].Init(((Random()%110)+5), 76, -1, -1, dead, randNum);
		else
			fruits[i].Init(((Random()%110)+5), 76, 1, -1, dead, randNum);
		
	}
	
	//*****************************************************
	
}


int k = 0;

void Move(){
	//*********move player**************
	if(player.get_status() == alive){
		player.Move();
		NeedtoDraw = 1;
	}
	//**************move fruits*******************

			
	//in the array, if the fruit is "alive" move the fruit
	for(int i = 0; i < 20; i++){
//		if(i == 19){
//			i = 0;
//		}
		if(fruits[i].get_status() == alive){
			NeedtoDraw = 1;
			fruits[i].Move();	
			if(fruits[i].get_error()){
				player.setNumLives(player.getNumLives() - 1);
				if(i == 19){
					fruits[0].set_status(alive);
				} else{
					fruits[(i+1)].set_status(alive);
				}
			}
			if(fruits[i].get_y() < 30){
				if(i == 19){
					fruits[0].set_status(alive);
				} else{
					fruits[(i+1)].set_status(alive);
				}
			}
			if(player.getNumLives() == 0){
				player.set_status(dead);
		}
			
	}
}

	//-------------arrow--------------
	for(int i = 0; i < 18; i++){
		if(arrows[i].get_status() == alive){
			NeedtoDraw = 1;
			arrows[i].Move();
		}
	}
	
	//-------------bomb-----------------
	if(Bomb.get_status() == alive){
		Bomb.Move();
	}
}

void Draw(void){
	SSD1306_ClearBuffer();
	for(int i = 0; i < 20; i++){
			if(fruits[i].get_status() == alive){
					fruits[i].Draw();
			}
		};
		if(player.get_status() == alive){
			player.Draw();
		}
		
		for(int i = 0; i < 18; i++){
			if(arrows[i].get_status() == alive){
					arrows[i].Draw();
			}
		}	
		Bomb.Draw();
		
		SSD1306_OutBuffer();
		
		if(lang == 0){
			SSD1306_SetCursor(11, 0);
			//SSD1306_OutString((char *)"Score:");
			SSD1306_OutString((char*)Phrases[12][lang]);
		}
		else{
			SSD1306_SetCursor(9, 0);
			//SSD1306_OutString((char *)"Puntaci\xA2" "n:");
			SSD1306_OutString((char*)Phrases[12][lang]);
		}
		
		LCD_OutDec(player.getPoints());
		SSD1306_SetCursor(0, 0);
		
		if(lang == 0){
			//SSD1306_OutString((char *)"Lives:");
			SSD1306_OutString((char*)Phrases[13][lang]);
		} else {
			//SSD1306_OutString((char *)"Vidas:");
			SSD1306_OutString((char*)Phrases[13][lang]);
		}
		
		LCD_OutDec(player.getNumLives());
		NeedtoDraw = 0;
	}
	
	

	
	
void Fire(int vx1, int vy1){
	int i = 0;
	while(arrows[i].get_status() == alive){
		i++;
		if(i == 18)
			return;
	}
	//change to arrows[i].Init(player.get_x() + 7, player.get_y() - 4, vx1, vy1, alive, 1)
	//vv do all that in arrow.Init()
	arrows[i].set_x(player.get_x() + 7);
	arrows[i].set_y(player.get_y() - 4);
	arrows[i].set_vx(vx1);
	arrows[i].set_vy(vy1);
	arrows[i].set_status(alive);
	arrows[i].set_image(arrow);
	
}


uint32_t point;

void Collide() {
   for(int i = 0; i < 18; i++){
		 if(arrows[i].get_status() == alive) {
       int x1 = arrows[i].get_x()+ 4;
       int y1 = arrows[i].get_y() - 12;
       if(Bomb.get_status() == alive){
           if(Bomb.Collision(x1, y1)){
						 player.set_status(dead);
					 }
       }      
       for(int k = 0; k < 20; k++){
           if (fruits[k].get_status() == alive){
               if(fruits[k].Collision(x1, y1)){
								 arrows[i].set_status(dead);
								 player.setPoints(player.getPoints() + 1);
								 if(k == 19){
									 fruits[0].set_status(alive);
								 } else{
									fruits[(k+1)].set_status(alive);
								 }
							 }
           }

       }
		 }
   }
}


int num;
int press = 0;
int pressed = 0;
int systick = 0;
int check2 = 0;
void SysTick_Handler(){
	systick++;
	static int lastTwo = 0; //for switch #2
	int downTwo = Switch_In() & 0x02;
	if (lastTwo == 0 && downTwo == 0x02){
			pressed = 1;
		
	}
	if(pressed == 1 && (Switch_In() & 0x02) == 0){
		pressed = 0;
		flag^=0x01;
		check2++;
	}

	if(flag != 0){//if (flag != 0)	
	static int lastOne = 0;
	int down = Switch_In() & 0x01;
	if (lastOne == 0 && down == 0x01){
		Fire(0,-2);
		Shoot_Sound();
		press++;
	}
	num = Random();
	if(Bomb.get_status() == dead){
		if(num % 2 == 0){
			Bomb.set_status(alive);
			NeedtoDraw = 1;
			
		}
	}
	Move();
	Collide();
	lastOne = down;
}
}

