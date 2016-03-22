#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "graphpro.h"
/* function prototypes */
void AskResolution(int*);
void checkerrors(void);

ld U1, V1, U2, V2;   // user window
int X1, Y1, X2, Y2;   // monitor display
int yy1, yy2, yy3; // Y_coordinates of the black displayed numbers
int resmaxx, resmaxy, userfont;
int fontsize, pos; // position of the black line between Menu&Screen
unsigned char frcolor = 15, bkcolor = 0;
TMenu* p_Menu; // defining a pointer to the object Menu
TList List; // here goes the constructor TList::TList

char msg[81];

int main(void)
{
 int gdriver, gmode;
 IsMouse(); // check for Mouse driver
 AskResolution(&gmode);
 // install a user written device driver
 gdriver = installuserdriver("svga256", NULL);
 // check for any installation errors
 checkerrors();
 // initialize graphics and local variables
 initgraph(&gdriver, &gmode, "");
 // check for any initialization errors
 checkerrors();
 cleardevice();
 resmaxx = getmaxx(); resmaxy = getmaxy();
 sprintf(msg, "Resolution Mode is %dx%d with %d colors", resmaxx+1, resmaxy+1, getmaxcolor()+1);
 fontsize = (int) floor(5*(resmaxx + 1)/640.0 + 0.5);
 pos = 13*(resmaxx + 1)/16 - 1;
 Device(0, 0, pos - 2, resmaxy); // This is our viewport :-)
 Window(0.0, 10.0, 20.0); // the fourth is calculated automatically
 setviewport(0, 0, resmaxx, resmaxy, 1); // 1 - means CLIP_ON but it doesn't work :-)
 setcolor(4);
 userfont = installuserfont("litt.chr");
 settextstyle(userfont, HORIZ_DIR, 6);
 outtextxy(0, 0, msg);

 p_Menu = new TMenu;
 p_Menu -> Draw();
 InitMouse(resmaxx, resmaxy);
 viewporthandler();

 // unreachable code:
 return 0;
} //End main

// check for and report any graphics errors
void checkerrors(void)
 {
  int errorcode;
  // read result of last graphics operation
  errorcode = graphresult();
  if (errorcode != grOk)
   {
    printf("\nGraphics error: %s\n", grapherrormsg(errorcode));
    printf("Press any key to halt:");
    getch();
    exit(2);
   }
 } // End CheckErrors

void AskResolution(int* grmode)
 {
  char mode;
  clrscr();
  printf("What Resolution Mode ? \n\n1 - 640x480\n2 - 800x600\n3 - 1024x768\n");
  do
    mode = getch();
  while (mode < '1' || mode > '3');
  switch (mode)
   {
    case '1' : *grmode = 2; break;
    case '2' : *grmode = 3; break;
    case '3' : *grmode = 4; break;
    default : printf("\n\a Bug!"); exit(1);
   }
 } // End of function AskResolution()
