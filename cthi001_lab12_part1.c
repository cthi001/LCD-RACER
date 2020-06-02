/*/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"
#include "../header/io.h"
#include "../header/keypad.h"
#include <stdlib.h>

#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum dstate {start2,write,mmenu,lose} dstate;
enum state {start,shift,reset}state,state1,state2,state3;
enum rstate {start1,wait,press}rstate;
static task task1,task2,task3,task4,task5;
task *tasks[] = {&task1,&task2,&task3,&task4,&task5};
size_t sizetask;
unsigned char temp;

unsigned menu,pause,lost,ms,fire,shoot,que;etime;lose_time;score_time,gcd,type1,type2,fp,score;
unsigned long diff;
size_t rem;
char msg[] = "Welcome... To LCD Racer! ";
char msg1[] = "GAME OVER!";
char msg2[] = "Score:";
char str[7];
int cnt,cnt2,index,saved,endcnt,pos1,pos2;posb,s1,s2,scroll;
int bullet(state2) {
	if (pause ==0) {
	
		switch (state2) {
			case start:state2=shift;shoot =0;posb = cnt;break;
			case shift:
				if( shoot==1 && ((posb == 16 || posb == 32 || ((posb >= pos1 && posb <=16&&cnt <= pos1)||(posb>=pos2 && posb>=17 && cnt <=pos2)) && (s1==0||s2==0)))){
				shoot=0;state2=reset;
				if ((posb >= pos1 && (cnt < pos1 || cnt <pos1+17)) && posb <=16 && type1%2==1){s1 =1;}
				else if (posb >=pos2 && (cnt < pos2) && posb>=17&&type2%2==1) {s2=1;}
				break;}	
				state2=shift;break;
			case reset:
				state2=shift;break;
		}
		switch (state2) {
			case start:break;
			case shift:
				if(shoot==1){posb++;}else{posb=cnt+1;}
				break;
			case reset:posb =cnt+1;shoot=0;break;
		}
	}
		return state2;
	
}
int obs1(state) {
	if (pause ==0) {
		switch (state) {
			case start:state=shift;pos1 = 9;break;
			case shift:
				if(pos1 == 1){state=reset;break;}state=shift;break;
			case reset:
				state=shift;break;
		}
		switch (state) {
			case start:break;
			case shift:pos1--;break;
			case reset:pos1 =16;type1=rand()%20;s1=0;break;
		}
	}
		return state;
	
}
int obs2(state1) {
	if (pause ==0) {
		switch (state1) {
			case start:state1=shift;que=0;pos2 = 32;break;
			case shift:	
				if(pos2 == 17){state1=reset;break;}state1=shift;break;
			case reset:state1=shift;break;
		}
		switch (state1) {
			case start:break;
			case shift:pos2--;break;
			case reset:pos2 =32; type2=rand()%20;s2=0;break;
		}
	}
		return state1;
	
}
int display(dstate){
	
	switch(dstate) {
		case start2:dstate=mmenu;task5.period =150;pause=1;break;
		case mmenu:if (menu ==0){cnt=1;score =0;pos1=10;pos2=32;pause =0;task5.period=50;
					
					fp=0;dstate = write;break;}
			break;
		case lose: 
				if(lose_time>3000){fp=0;etime=0;dstate= mmenu;task5.period=150;menu=1;pause=1;
					endcnt=0;cnt2 = 16;index=0;saved=0;LCD_ClearScreen();LCD_Cursor(0);				
					break;
				}
				dstate=lose;
				break;		
		case write:if((s1==0 && cnt == pos1) || (s2==0 && cnt == pos2)){pause =1;
				fp=0;etime=0;lose_time=0;dstate=lose;
					
				break;
				}
			if (menu ==1){dstate = mmenu;task5.period=150;endcnt=0;cnt2 = 16;index=0;saved=0;LCD_ClearScreen();LCD_Cursor(0);fp = 0;
					pause=1;break;
			} 
			dstate=write;break;
		}
	switch(dstate) {
		case start2:break;
		case lose:
			if (fp==0)	{LCD_ClearScreen();
					int p = 5;
					for (int i =0; i <sizeof(msg1)/sizeof(msg1[0]);i++){
						LCD_Cursor(p);LCD_WriteData(msg1[i]);p++;				
					}
					LCD_Cursor(--p);
					LCD_WriteData(' ');
					p=17;
					for (int i =0; i <sizeof(msg2)/sizeof(msg2[0]);i++){
						LCD_Cursor(p);LCD_WriteData(msg2[i]);p++;			
					}
					LCD_Cursor(--p);
					LCD_WriteData(' ');
					itoa(score,str,10);
					p=24;
					for (int i =0; i <sizeof(str)/sizeof(str[0]);i++){
							LCD_Cursor(p);
							LCD_WriteData(str[i]);
							p++;
									
					}
					LCD_Cursor(--p);
					LCD_WriteData(' ');
					LCD_Cursor(--p);
					LCD_WriteData(' ');
					LCD_Cursor(--p);
					LCD_WriteData(' ');
					LCD_Cursor(--p);
					LCD_WriteData(' ');
					fp =1;
					LCD_Cursor(0);
			}
					lose_time+=gcd;break;
		case mmenu: 
			if (fp==0) {LCD_ClearScreen();LCD_Cursor(0);fp=1;}
			if (endcnt == 16){endcnt=0;cnt2 = 16;index=0;saved=0;LCD_ClearScreen();LCD_Cursor(0);fp = 0;}
				if (cnt2 >0) {
					index = 0;
				}
				else {
					index = saved;
					index +=1;
					saved = index;	
				}
			for (unsigned i = cnt2; i < 16;i++) {
					LCD_Cursor(i);
					if (index < rem-1){
						LCD_WriteData(msg[index]);
						LCD_Cursor(0);
						index++;
					}
					else {
						if (index > rem-1) {
							LCD_WriteData(msg[' ']);
							LCD_Cursor(0);
						}		
					}
					LCD_Cursor(0);
				}
				if (index >= rem-1) {endcnt++;}
				if (cnt2 > 0) {
				cnt2--;
				}
		

				break;
				
		case write:
			if (pause ==0 ) {
				ms =0;
				LCD_ClearScreen();
				if (shoot==1) {
					LCD_Cursor(posb);			
					LCD_WriteData('-');
				}			
				LCD_Cursor(pos1);
				if (s1 == 0 ) {
					if (type1%2 == 0) {			
						LCD_WriteData('#');
					}
					else{LCD_WriteData('[');}
				}
				LCD_Cursor(pos2);
				if (s2==0) {		
					if (type2%2 == 0) {			
						LCD_WriteData('#');
					}
					else{LCD_WriteData('[');}
				}
				LCD_Cursor(cnt);
				LCD_WriteData('=');
				LCD_Cursor(0);
			}
			
		
	}
	return dstate;
}
int button(rstate) {

	switch (rstate){
		case start1:rstate=wait;break;
		case wait:if ((temp&0x01)==0x01&&pause==0){if(cnt-16 > 0){cnt-=16;}rstate=press;break;}
			if ((temp&0x02)==0x02&&pause==0){if(cnt+16 < 32){cnt+=16;}rstate=press;break;}
			if ((temp&0x04)==0x04&&pause==0){if( ( cnt<=16 &&  (cnt+1) <=16 ) || (cnt<=32&& (cnt+1)<=32  && cnt+1 != 17) ){cnt++;}rstate=press;break;}
			if ((temp&0x08)==0x08&&pause==0){if( ( cnt>=1 &&  ((cnt-1) >=1 )&& cnt-1 != 16) || (cnt>=17&& (cnt-1)>=17) ){cnt--;}rstate=press;break;}
			if ((temp&0x20)==0x20&&pause==0){
				fire=(fire ==0)?1:0;rstate=press;
				}
			if ((temp&0x20)==0x20){
				rstate=press;
				if (menu == 1) {menu = 0;}				
					break;
			}
			if ((temp&0x10)==0x10){
				menu = (menu == 0)?1:0;
				cnt =1;pos1=7;pos2=32;
			rstate=press;break;
			}
			rstate=wait;break;
		case press:if (temp == 0x00){rstate=wait;break;}
			rstate=press;break;
	}
	switch (rstate){
		case start:break;
		case wait:break;
		case press:if(fire == 1 &&  etime >= 3000) {shoot = 1;etime=0;}fire=0;break;
	}
	return rstate;

}
void clock() {
		if (pause == 0) {
			if (etime<=3000){
				etime +=gcd;
			}
			else {etime=3000;}
			if (score_time<=1000){
				if (score_time % 100 == 0) {score +=5;}
				score_time +=gcd;
			}
			else {score_time=0;}
		}
		else {lose_time+=gcd;}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xff;
	DDRA = 0x00;
	DDRC = 0Xf0;
	DDRD = 0XFF;
	PORTA = 0xFF;
	PORTD =0X00;
	PORTB=0X00;
	PORTC=0X0f;
	LCD_init();
	//LCD_DisplayString(1,"Congratulations!");
	cnt =1;
	cnt2=16;
	endcnt=0;
	pause =0;
	shoot =0;
	fire =0;
	scroll =0;
	lose_time=0;
	etime =0;
	score =0;
	ms=0;
	diff =150;
	type1=0;type2=1;menu=1;
    /* Insert your solution below */
    	rem = sizeof(msg)/sizeof(msg[0]);
	//task declaration
	
	task *tasks[] = {&task1,&task2,&task3,&task4,&task5};
	sizetask = sizeof(tasks)/sizeof(*tasks);
	//task initialize
	task1.state =start1;
	task1.period =50;
	task1.elapsedTime =task1.period;
	task1.TickFct = &button;

	task2.state =start;
	task2.period =150;
	task2.elapsedTime =task2.period;
	task2.TickFct = &obs1;
	
	task3.state =start;
	task3.period =150;
	task3.elapsedTime =task3.period;
	task3.TickFct = &obs2;
		
	task4.state =start;
	task4.period =50;
	task4.elapsedTime =task4.period;
	task4.TickFct = &bullet;

	task5.state =start2;
	task5.period =50;
	task5.elapsedTime =task5.period;
	task5.TickFct = &display;
	//***************
	gcd =tasks[0]->period;
	for (unsigned i =0;i < 5; i++) {
		gcd = findGCD(gcd,tasks[i]->period);	
	}
	TimerSet(gcd);
	TimerOn();
	while (1) {
		temp =~PINA;
		clock();
		for (unsigned i =0; i < 5;i++) {
			if (tasks[i]->elapsedTime >= tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime =0;
			}
			else {
				tasks[i]->elapsedTime +=gcd;
			}
					
		}
		
		while(!TimerFlag){}
		TimerFlag =0;
    }
    return 1;
}
