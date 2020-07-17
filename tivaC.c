#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"
#include "PORTF.h"
#include <stdbool.h> 

const  char line_x [] = {
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};
const  char line_y [] = {
	
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};


char X_O = 'X';
int pos = 1;
int count;
bool pl = true;
bool winner = false;
bool start = true;
bool tie = false;
int array [9] = {0,0,0,0,0,0,0,0,0};
char player[9] = {'K','L','M','N','S','A','R','V','U'};
#define red 0x02
#define blue 0x04
#define green 0x08
#define sw GPIO_PORTF_DATA_R&0x11
#define sw1 ((sw)==0x01)
#define sw2 ((sw)==0x10)
#define both (sw) == (sw1&sw2)
#define led GPIO_PORTF_DATA_R

void delay (unsigned long count); 
void interface();
void Nokia5110_DrawFullImageX (const char *ptr);
void Nokia5110_DrawFullImageY (const char *ptr);	
void draw (int,char player);
void crusor();
void move();
void who_win(void);
void win (void);
void draw_up (void);
void instruct (void);

void init(){
	int array [9] = {1,2,3,4,5,6,7,8,9};
	SYSCTL_RCGCGPIO_R |= 0x20;
	GPIO_PORTF_AFSEL_R = 0;
	GPIO_PORTF_PCTL_R = 0;
	GPIO_PORTF_AMSEL_R = 0;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_DEN_R = 0X1F;
	GPIO_PORTF_PUR_R = 0X11;
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0X1F;	
	
	Nokia5110_Init();
	TExaS_Init(SSI0_Real_Nokia5110_Scope);
	PortF_Init();
}


int main(){
	
	init ();
  
	while (1){
		interface ();
		move();
		if (winner){
			 win();
		}
		if (tie){
				draw_up();
		}
	}
}

void instruct(){
	Nokia5110_Clear();
	Nokia5110_SetCursor(0,0);
	Nokia5110_OutString(" Controls");
	Nokia5110_SetCursor(0,1);
	Nokia5110_OutString("Sw1 Mov For-ward");
	Nokia5110_SetCursor(0,3);
	Nokia5110_OutString("Sw2 Mov Back");
	Nokia5110_SetCursor(0,4);
	Nokia5110_OutString("Both Select");
	delay (50);
	start = false;
}

void move (){
	if (sw1){
			if(pos == 9){	
				return;                             
			}
			led = red;
			pos++;
			interface ();
		}
		else if (sw2){
			if(pos == 1){	
				return;                             
			}
			led = blue;
			pos--;
			interface ();
		}
		
		else if (both){
			int ind = array [pos];
			if (ind == 1){
				return;
			}
			if (pl){
				X_O = 'X';
			}
			else{
				X_O = 'O';
			}
			pl = !(pl);
			array[pos] = 1;
			player[pos] = X_O ;
			count ++;
			
			led = green;
			interface();
			who_win();
			if (count == 9 && !winner)
			{ 
				tie = true;
				draw_up();
			}
		}
		else {
			led = 0;
		}
}

void who_win(){
	bool win1 = (player[1]==player[2]) && (player[1]==player[3] );
	bool win2 = (player[1]==player[4]) && (player[1]==player[7] );
	bool win3 = (player[1]==player[5]) && (player[1]==player[9] );
	bool win4 = (player[2]==player[5]) && (player[2]==player[8] );
	bool win5 = (player[3]==player[5]) && (player[3]==player[7] );
	bool win6 = (player[3]==player[6]) && (player[3]==player[9] );
	bool win7 = (player[4]==player[5]) && (player[4]==player[6] );
	bool win8 = (player[7]==player[8]) && (player[7]==player[9] );
	if (win1 || win2 || win3 || win4 || win5 || win6 || win7 || win8 ){
		win();
		winner = true;
}
}
	

void draw (int pos_ind,char player){
  
{  switch (pos_ind)
		
    {case 1:{
      Nokia5110_SetCursor(2,1);
      Nokia5110_OutChar(player);
      break;
		}
    case 2:{
      Nokia5110_SetCursor(6,1);
      Nokia5110_OutChar(player);
      break;}
    case 3:{
      Nokia5110_SetCursor(9,1);
      Nokia5110_OutChar(player);
      break;}
    case 4:{
      Nokia5110_SetCursor(2,3);
      Nokia5110_OutChar(player);
      break;}
    case 5:{
      Nokia5110_SetCursor(6,3);
      Nokia5110_OutChar(player);
      break;}
    case 6:{
      Nokia5110_SetCursor(9,3);
      Nokia5110_OutChar(player);
      break;}
    case 7:{
      Nokia5110_SetCursor(2,5);
      Nokia5110_OutChar(player);
      break;}
    case 8:{
      Nokia5110_SetCursor(6,5);
      Nokia5110_OutChar(player);
      break;}  
    case 9:{
      Nokia5110_SetCursor(9,5);
      Nokia5110_OutChar(player);
      break;}

}}
}

void crusor (){
{  
  switch (pos)
  
    {case 1:{
      Nokia5110_SetCursor(2,1);
      Nokia5110_OutString("^");
      break;}
    case 2:{
      Nokia5110_SetCursor(6,1);
      Nokia5110_OutString("^");
      break;}
    case 3:{
      Nokia5110_SetCursor(9,1);
      Nokia5110_OutString("^");
      break;}
    case 4:{
      Nokia5110_SetCursor(2,3);
      Nokia5110_OutString("^");
      break;}
    case 5:{
      Nokia5110_SetCursor(6,3);
      Nokia5110_OutString("^");
      break;}
    case 6:{
      Nokia5110_SetCursor(9,3);
      Nokia5110_OutString("^");
      break;}
    case 7:{
      Nokia5110_SetCursor(2,5);
      Nokia5110_OutString("^");
      break;}
    case 8:{
      Nokia5110_SetCursor(6,5);
      Nokia5110_OutString("^");
      break;}  
    case 9:{
      Nokia5110_SetCursor(9,5);
      Nokia5110_OutString("^");
      break;}
		default:{
			Nokia5110_SetCursor(9,5);
      Nokia5110_OutString("^");}

}}
}




void interface(){
	int i;
	if (start){
		instruct();
	}
	if (winner || tie)
		return;
	Nokia5110_Clear();
	crusor();
	Nokia5110_DrawFullImageX (line_x);
	Nokia5110_DrawFullImageY (line_y);
	for (i=0;i<10;i++){
		if (array[i] == 1){
			draw(i,player[i]);
		}
	}
	
}

void delay(unsigned long count){
	
	unsigned long volatile time;
  while(count>0){
    time = 72724;
    while(time){
	  	time--;
    }
    count--;
  }
}


void win(){
	delay (10);
	Nokia5110_Clear();
	Nokia5110_SetCursor(0,2);
	Nokia5110_OutString("   winner        ");
	Nokia5110_OutChar(X_O);
}

void draw_up (){
	delay (10);
	Nokia5110_Clear();
	Nokia5110_SetCursor(0,2);
	Nokia5110_OutString("   draw_up      ");
	Nokia5110_OutString("-_-");
}
