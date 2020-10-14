/*/*	Author: Cuong Thi 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"
#include "../header/io.h"
#include "../header/keypad.h"
#include "../header/bit.h"
#include <stdlib.h>

#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum dstate {start2,write,mmenu,lose,yeet} dstate;
enum state {start,shift,reset}state,state1,state2,state3;
enum rstate {start1,wait,press}rstate,rstate2;
static task task1,task1_2,task2,task3,task4,task4_5,task5;
task *tasks[] = {&task1,&task1_2,&task2,&task3,&task4,&task4_5,&task5};
size_t sizetask;
unsigned char temp,temp2;

unsigned menu,pause,lost,ms,fire,shoot,que;etime;lose_time;score_time,gcd,type1,type2,fp,score,yeet_time,del,numplay,pmark,L,Y,scroll_time,tf;
unsigned cnt1,fire2,etime2,shoot2,type3,posb2,s3,dtime;
unsigned long diff;
size_t rem;
char msg[] = "LCD Racer!";
char msg1[] = "GAME OVER!";
char msg2[] = "Score:";
char selec[]="One     Two";
char str[10];
unsigned sz_selec;
int cnt,cnt2,index,saved,endcnt,pos1,pos2;posb,posc,s1,s2,scroll,yeet_index;
int bullet(state2) {
	if (pause ==0) {
	
		switch (state2) {
			case start:state2=shift;shoot =0;posb = cnt;break;
			case shift:
				if(shoot ==1){
					/*
					PROJECTILE LOGIC!!!!!!!!!!
					*/
					if ( (posb == posb2 || posb == posb2+1)&&type3 %2 == 1){s3 = 1;state2 = reset;break;}
					if ( (posb == pos1 || posb == pos1+1)&&type1 %2 ==1 ){s1 = 1;state2 = reset;break;}
					if ( (posb == pos1 || posb == pos1+1)&&type1 %2 ==0 ){state2 = reset;break;}
					if ( (posb == pos2 || posb == pos2+1)&&type2 %2==1 ){s2 = 1;state2 = reset;break;}
					if ( (posb == pos2 || posb == pos2+1)&&type2 %2==0 ){state2 = reset;break;}
					if (posb == 16 || posb == 32) {state2 = reset;break;}
				}
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

int bullet2(state3) {
	if (pause ==0 && numplay ==2) {
	
		switch (state3) {
			case start:state3=shift;type3=1;shoot2 =0;posb2 = cnt1-1;break;
			case shift:
				if(shoot2 ==1){
					/*
					PROJECTILE LOGIC!!!!!!!!!!
					*/
					if ( (posb2 == pos1 || posb2 == pos1-1|| posb2 == pos1+1)){s1 = 1;}
					if ( (posb2 == pos2 || posb2 == pos2-1 || posb2==pos2+1)){s2 = 1;}
					if (posb2 == 1 || posb2 == 17) {state3 = reset;break;}
				}
				state3=shift;break;
			case reset:
				state3=shift;break;
		}
		switch (state3) {
			case start:break;
			case shift:
				if(shoot2==1){posb2--;}else{posb2=cnt1-1;}
				break;
			case reset:posb2 =cnt1-1;shoot2=0;s3=0;break;
		}
		return state3;
	}
		posb2 =-2;
		return state3;
	
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
			case shift:pos1--;score+=10;if(pos1 == cnt1){etime2=0;};break;
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
			case shift:pos2--;if(pos2 == cnt1){etime2=0;}break;
			case reset:pos2 =32; type2=rand()%20;s2=0;break;
		}
	}
		return state1;
	
}
int display(dstate){
	
	switch(dstate) {
		case start2:dstate=mmenu,menu=1;pause=1;break;
		case mmenu:if (menu ==0){s1=0;s2=0;s3=0;cnt=1;score =0;pos1=10;pos2=32;task2.period = 300;task3.period=300;pause =0;
					
					fp=0;dstate = write;break;}
			break;
		case lose: 
				if(lose_time>=3000){fp=0;etime=0;tf=0;dstate= mmenu;numplay=1;menu=1;pause=1;
					task2.period =300;L=0;task3.period=300;endcnt=0;cnt2 = 16;posc=17;index=0;saved=0;LCD_ClearScreen();				
					break;
				}
				dstate=lose;
				break;		
		case write:if((numplay==2&&s3==0 && (cnt==posb2||cnt==posb2+1))||(s1==0 && cnt == pos1) || (s2==0 && cnt == pos2) || (pmark == 1&&((s1==0&&(cnt==pos1||cnt==pos1+1)) || (s2==0&&(cnt == pos2 || cnt==pos2+1))))){pause =1;
				yeet_time=0;Y=1;cnt1=13;dstate=yeet;yeet_index=0;

				break;
				}
			if (menu ==1){dstate = mmenu;tf=0;numplay=1;cnt1=13;endcnt=0;cnt2 = 16;index=0;saved=0;posc=17;LCD_ClearScreen();LCD_Cursor(0);fp = 0;
					task2.period = 300;task3.period=300;					
					pause=1;break;
			} 
			if (dtime >=5000) {if(task3.period > 50&&task2.period>50){dtime=0;task2.period -=gcd;task3.period-=gcd;}}
			dstate=write;break;
		case yeet: if (yeet_time >= 3000) {Y=0;task2.period = 300;task3.period=300;fp=0;etime=0;lose_time=0;L=1;dstate=lose;break;}
				dstate = yeet;break;
		}
	switch(dstate) {
		case start2:break;
		case yeet: 	if (cnt <= 16) {LCD_Cursor(cnt+16);
				LCD_WriteData(' ');
				}
				else {LCD_Cursor(cnt-16);
				LCD_WriteData(' ');
				}
				if (cnt > 17 || (cnt > 1&&cnt<=15)){
					LCD_Cursor(cnt-1);
					LCD_WriteData(' ');
				}
				if (cnt < 16 || (cnt>=17&&cnt < 31)){
					LCD_Cursor(cnt+1);
					LCD_WriteData(' ');
				}
				if(yeet_time % 150 ==0 && yeet_time<=3000){
					LCD_Cursor(cnt);
					LCD_WriteData(0xeb);//*********
					LCD_Cursor(0);				
				}
				else if (yeet_time % 150 !=0 && yeet_time<3000) { 
					LCD_Cursor(cnt);
					LCD_WriteData(0xa1);//*********
					LCD_Cursor(0);
				}
								
				break;
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
							LCD_Cursor(p++);			
							LCD_WriteData(str[i]);
										
					}
					
					fp =1;
					LCD_Cursor(0);
			}
					
					break;
		case mmenu: 
			if (fp==0) {LCD_ClearScreen();LCD_Cursor(0);fp=1;}	
			if (tf ==0) {
				int b = 1;
				for (unsigned i = 0; i < rem;i++) {
						LCD_Cursor(b++);
						LCD_WriteData(msg[i]);
					}
					
				LCD_Cursor(--b);
				LCD_WriteData(' ');
				b =18;
				for (unsigned i = 0;i < sz_selec;i++) {
					LCD_Cursor(b++);
					LCD_WriteData(selec[i]);
					if (i==sz_selec-1){LCD_Cursor(--b);LCD_WriteData(' ');}
				}
				tf=1;
			}
					LCD_Cursor(del);
					LCD_WriteData(' ');
					LCD_Cursor(posc);
					LCD_WriteData(0x7e);LCD_Cursor(0);
				break;
				
		case write:
			if (pause ==0 ) {
				if (etime >=3000){PORTD = SetBit(PORTD,0,1);}
				else {PORTD = SetBit(PORTD,0,0);}
				if (etime2 >=3000 && numplay==2){PORTD = SetBit(PORTD,1,1);}
				else {PORTD = SetBit(PORTD,1,0);}
				ms =0;
				LCD_ClearScreen();
				if(numplay == 2) {
					LCD_Cursor(9);
					LCD_WriteData(0xff);
					LCD_Cursor(25);
					LCD_WriteData(0xff);
					LCD_Cursor(cnt1);
					LCD_WriteData(0xfc);	
					if (shoot2==1 && s3 == 0) {
						LCD_Cursor(posb2);			
						LCD_WriteData('*');
					}		
				}
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
			if ((temp&0x04)==0x04&&pause==0){
				if(numplay ==1 ){
					if(  cnt<15 || (cnt >=17 && cnt < 31) ){cnt++;}
				}
				else if (numplay == 2) {
					if(  (cnt!=8 && cnt <=16)  || (cnt!=24 && cnt>=17) ){cnt++;}
				}
				pmark = 1;
				rstate=press;break;}
			if ((temp&0x04)==0x04&&pause==1&&menu==1){
				del=posc;posc += 8;numplay=2;if(posc>=25){posc=25;}
				rstate=press;break;}
			if ((temp&0x08)==0x08&&pause==0){
					if( (cnt>1 && cnt <=16) || (cnt >17 && cnt<=32) ){cnt--;}
				pmark = 1;
				rstate=press;break;}
			if ((temp&0x08)==0x08&&pause==1&&menu==1){
				del=posc;posc -= 8;numplay=1;if(posc<=17){posc=17;}
				rstate=press;break;
			}
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
				cnt =1;pos1=9;pos2=32;
			rstate=press;break;
			}
			rstate=wait;break;
		case press:if (temp == 0x00){pmark=0;rstate=wait;break;}
			rstate=press;break;
	}
	switch (rstate){
		case start:break;
		case wait:break;
		case press:if(fire == 1 &&  etime >= 3000) {shoot = 1;etime=0;}fire=0;break;
	}
	return rstate;

}

int button2(rstate2) {
	switch (rstate2){
		case start1:rstate2=wait;cnt1 = 13;break;
		case wait:
			if (numplay == 2) {
				if ((temp2&0x01)==0x01&&pause==0){if(cnt1 >= 17){cnt1-=16;}rstate2=press;break;}
				if ((temp2&0x02)==0x02&&pause==0){if(cnt1 <= 16){cnt1+=16;}rstate2=press;break;}
				if ((temp2&0x04)==0x04&&pause==0){
						if(  (cnt1>9 && cnt1 <16)  || (cnt1>25 && cnt1 <32) ){cnt1++;}
					rstate2=press;break;}
				if ((temp2&0x08)==0x08&&pause==0){
						if(  (cnt1>10&&cnt1<=16) || (cnt1>26&&cnt1<=32)  ){cnt1--;}
					rstate2=press;break;}
				if ((temp2&0x10)==0x10&&pause==0){
					fire2=(fire2 ==0)?1:0;rstate2=press;break;
					}
				rstate2=wait;break;
			}

			rstate2=wait;break;
		case press:if (temp2 == 0x00){rstate2=wait;break;}
			rstate2=press;break;
	}
	switch (rstate2){
		case start:break;
		case wait:break;
		case press:if(fire2 == 1 &&  etime2 >= 3000) {shoot2 = 1;etime2=0;}fire2=0;break;
	}
	return rstate2;
}

void clock() {
		if (pause == 0) {
			if (dtime <=5000) {
				dtime+=gcd;			
			}
			else {dtime =5000;}
			if (etime<=3000){
				etime +=gcd;
			}
			else {etime=3000;}
			if (etime2<=3000){
				etime2 +=gcd;
			}
			else {etime2=3000;}
			/*if (score_time<=1000){
				if (score_time % 100 == 0) {score +=5;}
				score_time += task3.period;
			}
			else {score_time=0;}*/
			
		}
		else {yeet_time+=gcd;/*if (menu == 1) {scroll_time+=gcd;if(scroll_time>100){scroll_time=0;}};*/if (L==1 && lose_time<=3000){lose_time+=gcd;}}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xff;
	DDRA = 0x00;
	DDRC = 0X00;
	DDRD = 0XFF;
	PORTA = 0xFF;
	PORTD =0x00;
	PORTB=0X00;
	PORTC=0Xff;
	LCD_init();
	//LCD_DisplayString(1,"Congratulations!");
	scroll_time=0;
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
	posc=17;
	del =17;
	yeet_time =0;
	yeet_index=0;
	numplay = 1;
	L= 0;
	Y=0;
tf=0;
	pmark=0;
	scroll_time =0;
	type1=0;type2=1;menu=1;
	cnt1=13;fire2=0;etime2=0;shoot2=0;
	type3=1;posb2=cnt1-1,s3=0;
    /* Insert your solution below */
    	rem = sizeof(msg)/sizeof(msg[0]);
	sz_selec= sizeof(selec)/sizeof(selec[0]);
	//task declaration
	
	
	sizetask = sizeof(tasks)/sizeof(*tasks);
	//task initialize
	task1.state =start1;
	task1.period =50;
	task1.elapsedTime =task1.period;
	task1.TickFct = &button;

	task1_2.state =start1;
	task1_2.period =50;
	task1_2.elapsedTime =task1_2.period;
	task1_2.TickFct = &button2;

	task2.state =start;
	task2.period =300;
	task2.elapsedTime =task2.period;
	task2.TickFct = &obs1;
	
	task3.state =start;
	task3.period =300;
	task3.elapsedTime =task3.period;
	task3.TickFct = &obs2;
		
	task4.state =start;
	task4.period =50;
	task4.elapsedTime =task4.period;
	task4.TickFct = &bullet;
	
	task4_5.state =start;
	task4_5.period =50;
	task4_5.elapsedTime =task4_5.period;
	task4_5.TickFct = &bullet2;
	
	task5.state =start2;
	task5.period =10;
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
		temp2=~PINC;
		temp =~PINA;
		clock();
		if (temp2 != 0x00) {PORTC = 0X80;}
		for (unsigned i =0; i < 7;i++) {
			if (tasks[i]->elapsedTime >= tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime =0;
			}
			else {
				tasks[i]->elapsedTime +=gcd;
			}
					
		}
		
		PORTC=0XFF;
		while(!TimerFlag){}
		TimerFlag =0;
    }
    return 1;
}
