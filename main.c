 /*
************************************************************************
 ECE 362 - Mini-Project C Source File - Spring 2015
***********************************************************************
	 	   			 		  			 		  		
 Team ID: < 09 >

 Project Name: < ? >

 Team Members:

   - Team/Doc Leader: < ? >      Signature: ______________________
   
   - Software Leader: < ? >      Signature: ______________________

   - Interface Leader: < ? >     Signature: ______________________

   - Peripheral Leader: < ? >    Signature: ______________________


 Academic Honesty Statement:  In signing above, we hereby certify that we 
 are the individuals who created this HC(S)12 source file and that we have
 not copied the work of any other student (past or present) while completing 
 it. We understand that if we fail to honor this agreement, we will receive 
 a grade of ZERO and be subject to possible disciplinary action.

***********************************************************************

 The objective of this Mini-Project is to .... < ? >


***********************************************************************

 List of project-specific success criteria (functionality that will be
 demonstrated):

 1.

 2.

 3.

 4.

 5.

***********************************************************************

  Date code started: < April 18 2015 >

  Update history (add an entry every time a significant change is made):

  Date: < April 20 2015 >  Name: < Eshamogbo Ojuba >   Update: < 1 >

  Date: < April 22 2015 >  Name: < Eshamogbo Ojuba >   Update: < 2  >

  Date: < April 25 2015 >  Name: < Eshamogbo Ojuba >   Update: < 3 >
  
  Date: < April 27 2015 >  Name: < Eshamogbo Ojuba >   Update: < 4 >
  
  Date: < April 28 2015 >  Name: < Eshamogbo Ojuba >   Update: < 5 >
  
  Date: < April 29 2015 >  Name: < Eshamogbo Ojuba >   Update: < 6 >

***********************************************************************
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>


/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);
void bcdinc(void);
void timedisp(void);
void secdisp(char s);
void hrsdisp(char h);
void mindisp(char m);
void decpntdisp(char d);
void leddisp(char c);
char rand(void);
void gamedisp(char no, char pos);
//char readhigh(void);
//void sethigh(char score);
//void Flash_Init(unsigned long oscclk);
//signed char Flash_Write_Word(unsigned int *far far_address,unsigned int data);


/* Variable declarations */ 	   			 		  			 		       
char modepb = 0;  // pushbutton flag
char statepb = 0;  // pushbutton flag
char incpb = 0; //  pushbutton flag
char okpb = 0;  // pushbutton flag for game only
char mprevpb = 0;  // previous pushbutton state
char sprevpb = 0;  // previous pushbutton state
char iprevpb = 0;  // previous pushbutton state
char okprevpb = 0; // previous pushbutton state


//char runstp	= 0xff;	// clock run/stop flag
char onesec 	= 0;	// one second flag
char halfsec = 0;
char alarm = -1; // alarm on or off
char mode = 3;   // variable that shall hold the modes: alarm set, game mode or time set
char state = 0; // mins or hrs?

char stat = 0;
char stat1 = 1;  // this are used to allow the led displays
char stat2 = 1;  // to be turned on only once
char stat3 = 1;
char stat4 = 1;

char count = 0; // count of 100ms

unsigned char secs = 0x15;
unsigned char mins = 0x59;
unsigned char hrs = 0x11;

unsigned char counter = 0;
unsigned char statflag = 0;

unsigned char almin = 0;
unsigned char alhr = 0;

char mflg = 2;    // flag to show hours has changed
char hflg = 0;    // flag to show minute has changed
char modeflag = 0; // flag to show that mode has changed
char alarmflag = 0;

/* Game variables */
char randps = 0;  // position where random no shall appear (hrs, mins, or secs?)
char level = 5;   // level of player (no of numbers to display to player)
char numarray[100]; // where randomly generated numbers shall be stored
char posarray[100]; // where position of number shall be stored
char k = 0;        // index to be used to access posarray
unsigned char a;         // variable for random number generation algorithm
char flag = 1;  // this flag indicates when the gamer has entered in wrong number
char hrinput = 0;     // variable that gamer would input
char mininput = 0;    // variable that gamer would input
char score;       // user score
char j;
char correct;
unsigned char seq = 0;  // sequence of corrected input from user

// test variable x
int xa = 0;
  	   			 		  			 		       

/* Special ASCII characters */
#define CR 0x0D		// ASCII return 
#define LF 0x0A		// ASCII new line 

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10		// RS pin mask (PTT[4])
#define RW 0x20		// R/W pin mask (PTT[5])
#define LCDCLK 0x40	// LCD EN/CLK pin mask (PTT[6])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction
#define LINE1 0x80	// LCD line 1 cursor position
#define LINE2 0xC0	// LCD line 2 cursor position

/* DISPLAY DEFINITIONS BIT MASKS */
#define SSSECS 0x01      // PT0 for seconds
#define SSMINS 0x01      // PM0 for mins
#define SSHRS 0x02       // PM1 for hrs

#define SSLEDS 0x01      // AN0 for RGB LEDs
#define SSDEC 0x02      // AN1 for decimal points and colons

#define SECDP 9
#define MINDP 7
#define HRDP 5

/* LED DISPLAY COLOR DEFINITIONS BIT MASKS */
#define BLUE 0xb4  
#define RED 0xd8  
#define GREEN 0x6C 
#define OFF 0xfc
#define YELLOW 0x48 
#define WHITE 0
#define PINK 0x90
#define CYAN 0x24 



/* MODE DEFINITIONS BIT MASKS */
#define TIME 0  // time display mode
#define SETALARM 1 // alarm clock set mode
#define SETTIME 2 // set time
#define GAME 3  // game playing mode


/* STATE DEFINITIONS BIT MASKS */
#define MINS 0
#define HRS 1

	 	   		
/*	 	   		
***********************************************************************
 Initializations
***********************************************************************
*/

void  initializations(void) {

/* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
  PLLCTL = PLLCTL | 0x40; //; turn on PLL
  SYNR = 0x02;            //; set PLL multiplier
  REFDV = 0;              //; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; //; engage PLL

/* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40   ; //COP off; RTI and COP stopped in BDM-mode

/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
  SCIBDH =  0x00; //set baud rate to 9600
  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; //$9C = 156
  SCICR2 =  0x0C; //initialize SCI for program-driven operation
  DDRB   =  0x10; //set PB4 for output mode
  PORTB  =  0x10; //assert DTR pin on COM port

/* Initialize peripherals */

/* 
   Initialize TIM Ch 7 (TC7) for periodic interrupts every 100 ms  
    - Enable timer subsystem                         
    - Set channel 7 for output compare
    - Set appropriate pre-scale factor and enable counter reset after OC7
    - Set up channel 7 to generate 10 ms interrupt rate
    - Initially disable TIM Ch 7 interrupts	 	   			 		  			 		  		
*/
   			 		  			 		  		
  TIE = 0x80;          // enable interrupt on timer 7
  TIOS = 0x80;
  TSCR1 = 0x80;
  TSCR2 = 0x0F;  // 187 500Hz is result of prescaler of 128, with TCNT reset when OC7 happens
  TCTL1 = 0;            // disconnet port pin from timer
  TCTL2 = 0;
  TC7 = 18750;          // interrupt every 100 ms


   // interrupt rate = 65.536*2ms

   CRGINT = 0x80;    // disable real-time interrupt
   RTICTL = 0x7d;  // set prescalar
    
 

/*  Initialize SPI for baud rate of 6 Mbs, MSB first
  (note that R/S, R/W', and LCD clk are on different PTT pins) */

  SPICR1 = 0x50;  // power on SPI in master mode
  SPICR2 = 0;
  SPIBR = 0b0001000;


/* Initialize digital I/O port pins */

  DDRAD = 0x07;      // pins 0 and 1 are SS pins for LEDs and DEC points
  ATDDIEN = 0xF0; // 7 AND 6 are digitial inputs

/* Initialize Slave Select Pins - PORT M */
   DDRM = 0x03;    // use PM0 and PM1 to select hrs and mins
   DDRT = 0x03;    // use PT0 to select secs
   
/* Initialize PWM for buzzer - on PT1 */
   PWMPER1 = 250;
   PWMPOL = 0x02;   // high polariy
   PWMDTY1 = 125;  // 50% duty cycle
   PWMPRCLK = 0x0f; // prescale by 128
   MODRR = 0x02;
   PWME = 0;	      
	 
	 hrsdisp(hrs);
   mindisp(mins);
   secdisp(secs);     
}

	 		  			 		  		
/*	 		  			 		  		
***********************************************************************
Main
***********************************************************************
*/
void main(void) {
  	DisableInterrupts
	initializations();
	EnableInterrupts	  			 		  		
  leddisp(CYAN);
 // xa = PTAD_PTAD4;
  
/*  asm {
    movw $FFF0, xa ;  
  } */

 for(;;) {
   
     
     if (onesec != 0) {
      onesec = 0; 
      bcdinc();
      
        // display the corresponding LEDS
        if (mode == TIME || mode == SETTIME) {

        timedisp();
          
          
          if (hrs > 0x05 && hrs < 0x21) {            
            PTAD_PTAD2 = 0;    // power on nixie
            
            counter = (counter + 1) % 30;
          
            if (counter == 0) {
              stat = ~stat;
              statflag = 1;
            }
          
            if (statflag != 0 && stat == 0) {
              statflag = 0;
              leddisp(CYAN);
            } 
          
            else if (statflag != 0 && stat != 0) {
              statflag = 0;
              leddisp(PINK);
            }
            
              stat1 = 0;
              stat2 = 1;
          } 
          
          /*
          else if (stat2 != 0 && hrs > 0x11 && hrs < 0x16) {
            leddisp(CYAN);
            stat2 = 0;
            stat3 = 1;
          } 
          
          else if (stat3 != 0 && hrs > 0x15 && hrs < 0x21) {
            leddisp(WHITE);
            stat3 = 0;
            stat4 = 1;
          } */
          
          else if (stat4 != 0 && hrs > 0x21 || hrs < 0x05) {
            leddisp(OFF);
            stat4 = 0;
            stat1 = 1;
          } 
         
          
          
          

        } 
   
        // check if alarm clock is activated
        if (alarm != 0) {        
           if (mins == almin && hrs == alhr) {
              // send signal to alarm buzzer
              PWME = 0x02;
           } 
           
           else {
            PWME = 0;
           }
        } 
      
        
     } // end of onesec
     
     /****** MODES ******/
     
     /* GAME MODE */
  if (mode == GAME) {
  
  // Initialize variables for the game   
     char i;
     a = ((count+secs+mins) * 4) % 11;
     hrsdisp(0);
     secdisp(0);
     mindisp(0);
     
     
     leddisp(BLUE);
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
     
     leddisp(OFF);
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
     
     leddisp(BLUE);
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
     
     leddisp(OFF);
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
     
     leddisp(BLUE);
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
     
     leddisp(OFF);
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
     
     leddisp(BLUE);
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
     
     leddisp(OFF);
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
      
  
     // generate the random numbers and positions and store them in corresponding array
     i = 0;
     while (mode == GAME && i < 100) {
        numarray[i] = rand();
        
        if (onesec != 0) {
          onesec = 0; 
          bcdinc();
       } 
       i++;
     }
     
     // wait one second 
     while (onesec == 0) {}
     onesec = 0; 
     bcdinc();
   
   i = 0;
  
	while (mode == GAME && i < level) {
	
		hrsdisp(numarray[i]);
		
		leddisp(GREEN);
		while (onesec == 0) {}
		onesec = 0; 
		bcdinc();
		leddisp(OFF);
		while (onesec == 0) {}
		onesec = 0; 
		bcdinc();
		i++;	
    } 

	/* Collect input from the user*/
	j = 0;	
	while (mode == GAME && flag != 0 && j < level) {
	
		if (onesec != 0) {
			onesec = 0;
			bcdinc();
		}
	
		if (incpb != 0) {
			incpb = 0;
			hrinput = (hrinput + 1) % 10;
			hrsdisp(hrinput);
		}
		
		if (okpb != 0) {
			okpb = 0;
			if (hrinput == numarray[j]) {
				leddisp(GREEN);
				seq++;
				j++;
			}
			
			else {
				j = 0;
				leddisp(RED);
				
				// quit loop
				flag = 0; 
			}
		}
	}
  
  
 //quit: 
  // wait five seconds
   
   if (seq == level) {
     level = level + 1;
   }
   
   //leddisp(WHITE);
   secdisp(seq);
   mindisp(0);
   hrsdisp(0);
   while (mode == GAME && onesec == 0) {}
   onesec = 0; 
   bcdinc();
   while (mode == GAME && onesec == 0) {}
   onesec = 0; 
   bcdinc();
   while (mode == GAME && onesec == 0) {}
   onesec = 0; 
   bcdinc();
   while (mode == GAME && onesec == 0) {}
   onesec = 0; 
   bcdinc();
   while (mode == GAME && onesec == 0) {}
   onesec = 0; 
   bcdinc();
//   hrsdisp(hrs);
//   mindisp(mins);
//  secdisp(secs);
  
          
} // end of game mode
    
   /* SET the TIME MODE */
   else if (mode == SETTIME) {    
        if (incpb != 0 && state == HRS) {
          incpb = 0;
    
          asm {
             clc
             ldaa   hrs
             adca   #$01         ; increment minutes by one
             daa             ; decimal adjust
             staa   hrs     ; put it back
             cmpa   #$24     ; is it 24 reset
             bne    exit3
             movb   0,hrs
          exit3:
          }
      
          hrsdisp(hrs);
        }
    
        if (incpb != 0 && state == MINS) {
          incpb = 0;
    
          asm {
             clc
             ldaa   mins
             adca   #$01         ; increment minutes by one
             daa             ; decimal adjust
             staa   mins     ; put it back 
             cmpa   #$60     ; is it sixty reset
             bne    exit4
             movb   0,mins
          exit4:
          }
      
          mindisp(mins);
        }
        
      // display hours decimal point
      }
         
      else if (mode == SETALARM) {
      
        hrsdisp(alhr);
        mindisp(almin);
        secdisp(0);
      
        if (incpb != 0 && state == HRS) {
        incpb = 0;
    
        asm {
           clc
           ldaa   alhr
           adca   #$01         ; increment minutes by one
           daa             ; decimal adjust
           staa   alhr     ; put it back
           cmpa   #$24     ; is it sixty reset
           bne    exit
           movb   0,alhr
        exit:
        }
      }
    
      if (incpb != 0 && state == MINS) {
        incpb = 0;
    
        asm {
           clc
           ldaa   almin
           adca   #$01         ; increment minutes by one
           daa             ; decimal adjust
           staa   almin     ; put it back 
           cmpa   #$60     ; is it sixty reset
           bne    exit2
           movb   0,almin
        exit2:
        }
      }
      
      // display mins decimal point
      
      if (mode == TIME || mode == SETTIME) {
        hrsdisp(hrs);
        mindisp(mins);
        secdisp(secs);
      
      }
      
    } // end of setalarm mode
    
    // powering on decimal points
    if (modeflag != 0) {
      modeflag = 0;
    
      if (mode == SETALARM) {
        decpntdisp(MINDP);
      } 
      
      else if (mode == SETTIME) {
        decpntdisp(HRDP);
      } 
      
    }
    
    
    // powering on alarm decimal point
    if (alarmflag != 0) {
      if (alarm == 0) {
        decpntdisp(SECDP);
      }
    }
        
     

     
  } /* loop forever */
   
}   /* do not leave main */

void bcdinc(void) {
   asm {
   
      ldaa   secs
      adca   #$01       ; increment seconds by one
      daa             ; decimal adjust a
      staa   secs     ; put it back in seconds
      cmpa   #$60     ; is it sixty
      bne    exit     ; don't touch hrs nor mins if secs is not sixty
          
      clc
      movb   #0,secs  ; reset seconds if it hits 60
      
      ldaa   mins
      adca   #$01         ; increment minutes by one
      daa             ; decimal adjust
      staa   mins     ; put it back
      movb   #$01, mflg   ; set minute increased flag 
      cmpa   #$60     ; is it sixty reset 
      bne    exit      ; don't touch hours if mins is not sixty
          
      clc
      movb   #0, mins  ; reset minutes
      
      ldaa   hrs 
      adca   #$01        ; increment hours by one 
      daa
      staa   hrs
      movb   #$01, hflg  ; set hours increased flag
      cmpa   #$24      ; is it 24
      bne    exit
      
      clc
      movb   #0,hrs

      
exit:            
   }
}

void timedisp(void) {
   secdisp(secs);
   
    if (mflg == 1) {
      mflg = 0;
      mindisp(mins);
    }
    
    if (hflg == 1) {
      hflg = 0;
      hrsdisp(hrs);
    }
    
    
}

void secdisp(char s) {
    
    char i;


    PTT &= ~SSSECS;
    while (!SPISR_SPTEF) { }  // wait for flag to be empty
    
    SPIDR = s;
    for (i=0; i<10; i++) {}
    PTT |= SSSECS;

}

void mindisp(char m) {

    char i;

    PTM &= ~SSMINS;
    while (!SPISR_SPTEF) { }  // wait for flag to be empty
    
    SPIDR = m;
    for (i=0; i<10; i++) {}
    PTM |= SSMINS;

}

void hrsdisp(char h) {

    char i;
    
    PTM &= ~SSHRS;
    
    while (!SPISR_SPTEF) { }  // wait for flag to be empty
    SPIDR = h;
    for (i=0; i<10; i++) {}
    PTM |= SSHRS;  

}

void decpntdisp(char d) {
   PTT = d;
}

void leddisp(char c) {

    char i;
    
    PTAD &= ~SSLEDS;
    
    while (!SPISR_SPTEF) { }  // wait for flag to be empty
    SPIDR = c;
    for (i=0; i<10; i++) {}
    PTAD |= SSLEDS;  
}

char rand(void) {
   
   a = (a * 40 + 3) % 17;
   return ((a % 9) + 1);
}

void gamedisp(char no, char pos) {
  
  if (pos == 0) {
    hrsdisp(no);
    mindisp(0);
  } 
  
  else {
    mindisp(no);
    hrsdisp(0);
  }
}

/*
char readhigh (void) {
}

void sethigh(char score) {

}
*/


/*
***********************************************************************
IRQ service routine:

shall be used to turn on/off display in the night
 
***********************************************************************
*/

/*
interrupt 6 IRQ_ISR(void) {
  if (hrs > 21 || hrs < 05) {
    PTAD_PTAD2 = ~PTAD_PTAD2;  // toggle
  } 
  else {
    PTAD_PTAD2 = 0; // keep it on
  }
}*/

/*
***********************************************************************                       
 RTI interrupt service routine: RTI_ISR
************************************************************************
*/

interrupt 7 void RTI_ISR(void)
{ 
  	// clear RTI interrupt flagt 
  	CRGFLG = CRGFLG | 0x80;
  	
  	// mode changing goes on here
  	if(PTAD_PTAD5 == 1) {
  	  if (mprevpb == 0) {
  	    mode = (mode + 1) % 4; 
  	    modeflag = 1;
  	    mprevpb = 1;
  	  } 
  	} 
  	
  	else {
  	  mprevpb = 0;
  	}
  	
  	// change of select: min, hours,etc
  	if (PTAD_PTAD6 == 1) {
  	  if (sprevpb == 0) {
  	    state = (state + 1) % 2;
  	    sprevpb = 1;
  	  }
  	} 
  	
  	else {
  	  sprevpb = 0;
  	}

    // increment pushbutton and put alarm on pushbutton
  	if(PTAD_PTAD7== 1) {
  	  if (iprevpb == 0) {
  	
  	    if (mode == SETTIME || mode == SETALARM || mode == GAME) {
  	      
  	      incpb = 1;
  	    } 
  	    
  	    else if (mode == TIME) {
  	      alarm = ~alarm;
  	      alarmflag = 1;   // to indicate alarm has been put on or off
  	    } 
  	    
  	    iprevpb = 1;
  	  }
  	}
  	
  	 else {
  	  
  	  iprevpb = 0;
    	}
  
   // ok pusbutton only for game mode only	
   if(PTAD_PTAD4== 0) {
  	  if (okprevpb == 1) {
  	    
  	  okpb = 1;
  	  }
  	  
  	okprevpb = 0;
  	} else {
  	  
  	  okprevpb = 1;
  	}
  	
 
}
/*
***********************************************************************                       
  TIM interrupt service routine	  		
***********************************************************************
*/

interrupt 15 void TIM_ISR(void)
{
  	// clear TIM CH 7 interrupt flag 
 	TFLG1 = TFLG1 | 0x80; 
 	count = (count + 1);
 	
 		if (count == 10) {
 		count = 0;
 	  onesec = 1;
 	} 
 	
// 	randps = ~randps;
}

/*
***********************************************************************                       
  SCI interrupt service routine		 		  		
***********************************************************************
*/

interrupt 20 void SCI_ISR(void)
{

}

/*
***********************************************************************
 Character I/O Library Routines for 9S12C32 
***********************************************************************
 Name:         inchar
 Description:  inputs ASCII character from SCI serial port and returns it
 Example:      char ch1 = inchar();
***********************************************************************
*/

char inchar(void) {
  /* receives character from the terminal channel */
        while (!(SCISR1 & 0x20)); /* wait for input */
    return SCIDRL;
}

/*
***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
 Example:      outchar('x');
***********************************************************************
*/

void outchar(char x) {
  /* sends a character to the terminal channel */
    while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
    SCIDRL = x;
}