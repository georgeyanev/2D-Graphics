#include <stdio.h>
#include <graphics.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "graphpro.h"

extern ld U1, V1, U2, V2;   // user window
extern int resmaxx, resmaxy, userfont, fontsize, pos, y1, y2, y3;
extern int yy1, yy2, yy3;
extern TMenu* p_Menu;
extern TList List;
extern unsigned char frcolor, bkcolor;

boolean TButton::marked = false;  // static member variable

TButton::TButton(int tx1, int ty1, int tx2, int ty2, unsigned char tnumber, char* tcaption)
 {
   x1 = tx1; y1 = ty1; x2 = tx2; y2 = ty2;
   pressed = false;
   number = tnumber;
   _fstrcpy(caption, tcaption);
 }

void TButton::Press()
 {
  if (!pressed)
   {
    setwritemode(COPY_PUT);
    pressed = true;
    marked = true;
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH); // 0 means not user's pattern
    settextstyle(userfont, HORIZ_DIR, fontsize); // 1 means 8x8 not stroked
    setcolor(0); // set it to BLACK
    rectangle(x1, y1, x2, y2);
    BLine (x1 + 1, y1 + 1, x2 - 1, y1 + 1, 8); // 8 - DARKGRAY
    BLine (x1 + 1, y1 + 1, x1 + 1, y2 - 1, 8);
    Block (x1 + 2, y1 + 2, x2 - 1, y2 - 1, 7); // 7 - LIGHTGRAY
    setcolor(0);
    moveto( (resmaxx + pos)/2 - 1, (y1 + y2)/2.0 + 0.5);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
//    setusercharsize(1, 1, 1, 1); // Borland's bool shit bugs
    outtext(caption);
   }
 } // End of member function Press()

void TButton::Unpress()
 {
  setwritemode(COPY_PUT);
  pressed = false;
  marked = false;
  setlinestyle(SOLID_LINE, 0, NORM_WIDTH); // 0 means not user's pattern
  settextstyle(userfont, HORIZ_DIR, fontsize); // 1 means 8x8 not stroked
  setcolor(0); // set it to BLACK
  rectangle(x1, y1, x2, y2);
  BLine(x1 + 1, y1 + 1, x2 - 1, y1 + 1, 15); // 15 - WHITE
  BLine(x1 + 1, y1 + 1, x1 + 1, y2 - 1, 15);
  Block (x1 + 2, y1 + 2, x2 - 2, y2 - 2, 7);
  BLine (x2 - 1, y1 + 1, x2 - 1, y2 - 1, 8);
  BLine (x1 + 1, y2 - 1, x2 - 1, y2 - 1, 8);
  setcolor(0);
  moveto( (resmaxx + pos)/2, (y1 + y2)/2.0 - 2.0*(resmaxx + 1)/640);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
//  setusercharsize(1, 1, 1, 1); // Borland's bool shit bugs
  outtext(caption);
 } // End of member function Unpress

void TButton::Draw()
 {
  if (pressed)
   {
    Unpress();
    Press();
   }
  else
   Unpress();
 } // End of all class TButton!!!!

void Block (int x1, int y1, int x2, int y2, unsigned char color)
 {
  setfillstyle(SOLID_FILL, color);
  bar (x1, y1, x2, y2);
 }

void Block1 (const int& x1, const int& y1, const int& x2, const int& y2, const unsigned char& color)
 {
  int i;
  for (i = y1; i <= y2; i++) // Block1 affects the setwritemode function
   BLine(x1, i, x2, i, color);
 }

// Now the menu class ...

TMenu::TMenu()
 {
  int i; // i always is a simple counter
  Y_ais[0] = (int) floor( 36*(resmaxy + 1)/480.0 + 0.5) - 1;
  for (i = 1; i < 13; i++) // create the Y - coordinates ...
   Y_ais[i] = Y_ais[i-1] + (int) floor( 37*(resmaxy + 1)/480.0 + 0.5);
  // now creating the buttons
  yy1 = (Y_ais[11] + Y_ais[10])/2;
  yy2 = Y_ais[11];
  yy3 = (Y_ais[12] + Y_ais[11])/2;
  p_buttons[0] = new TButton(pos + 1, Y_ais[0] + 2, resmaxx - 1, Y_ais[1] - 1, 0, "Line");
  p_buttons[1] = new TButton(pos + 1, Y_ais[1] + 2, resmaxx - 1, Y_ais[2] - 1, 1, "Rectangle/Sq");
  p_buttons[2] = new TButton(pos + 1, Y_ais[2] + 2, resmaxx - 1, Y_ais[3] - 1, 2, "Polygon");
  p_buttons[3] = new TButton(pos + 1, Y_ais[3] + 2, resmaxx - 1, Y_ais[4] - 1, 3, "Elipse/Circle");
  p_buttons[4] = new TButton(pos + 1, Y_ais[4] + 2, resmaxx - 1, Y_ais[5] - 1, 4, "Set colors");
  p_buttons[5] = new TButton(pos + 1, Y_ais[5] + 2, resmaxx - 1, Y_ais[6] - 1, 5, "Fill selected");
  p_buttons[6] = new TButton(pos + 1, Y_ais[6] + 2, resmaxx - 1, Y_ais[7] - 1, 6, "Window");
  p_buttons[7] = new TButton(pos + 1, Y_ais[7] + 2, resmaxx - 1, Y_ais[8] - 1, 7, "Load");
  p_buttons[8] = new TButton(pos + 1, Y_ais[8] + 2, resmaxx - 1, Y_ais[9] - 1, 8, "Save");
  p_buttons[9] = new TButton(pos + 1, Y_ais[9] + 2, resmaxx - 1, Y_ais[10] - 1, 9, "Exit");
 }

TMenu::~TMenu() // destructor, we delete all of the buttons
 {
  int i;
  for (i = 0; i < 10; i++)
   delete p_buttons[i];
 }

void TMenu::Draw(void)
 {
  int i, mousex, mousey;
  char buf[13];
  setwritemode(COPY_PUT);
  BLine(pos-1, 0, pos-1, resmaxy, 0);
  setfillstyle(SOLID_FILL, 15); // 15 - WHITE
  bar(pos, 0, resmaxx, resmaxy);
  setcolor(0);
  moveto( (resmaxx + pos)/2 - 1, Y_ais[0]/2);
  settextstyle(userfont, HORIZ_DIR, (int) floor( 4.0*(resmaxx+1)/640) + 0.5);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtext("(C)1998 Georgi Yanev");
  // now print the coordinates x y u and v
  char f1, f2; // fictive
  ld u, v;
  GetMouseButtons(&f1, &f2, &mousex, &mousey);
  moveto(pos + 2, yy1);
  settextjustify(LEFT_TEXT, CENTER_TEXT);
  devicetouser(u, v, mousex, mousey);
  sprintf(buf, "u=%-10Lf", u);
  outtext(buf);   // user coordinates
  moveto( pos + 2, yy2);
  settextjustify(LEFT_TEXT, CENTER_TEXT);
  sprintf(buf, "v=%-10Lf", v);
  outtext(buf);   // user coordinates
  moveto(pos + 2, yy3);
  settextjustify(LEFT_TEXT, CENTER_TEXT);
  sprintf(buf, "x=%d", mousex);
  outtext(buf);   // mouse coordinates
  moveto( (resmaxx + pos)/2 - 1, yy3);
  settextjustify(LEFT_TEXT, CENTER_TEXT);
  sprintf(buf, "y=%d", mousey);
  outtext(buf);   // mouse coordinates

  for (i = 0; i < 10; i++)  // drawing the buttons ...
   p_buttons[i] -> Draw();
 }

signed char TMenu::Getpressedbutton() // returns -1 if no button is pressed
 {
  int i;
  if (p_buttons[0] -> marked) // if we have a pressed button ...
   for (i = 0; i < 4; i++) // we have four buttons to check
    if (p_buttons[i] -> pressed)
     return(p_buttons[i] -> number); // or return i
  return(-1);
 }

void TMenu::Release(void)
 {
  signed char i;
  i = Getpressedbutton();
  if (i == -1) return;
  p_buttons[i] -> Unpress();
 }

void TMenu::Handler(int mousey)
 {
  int fik1, fik2;
  char left = 1, right = 1;
  if (mousey > Y_ais[0] + 1 && mousey < Y_ais[1]) // we are in Button "Line"
   if (p_buttons[0] -> pressed)
    {
     while (left) // waiting for the button to be released
      GetMouseButtons(&left, &right, &fik1, &fik2);
     HideMouse();
     p_buttons[0] -> Unpress();
     ShowMouse();
     return;
    }
   else
    {
     HideMouse();
     List.Unselectall();
     Release(); // first we release the previous pressed button
     p_buttons[0] -> Press();
     ShowMouse();
     while (left) // waiting for the button to be released
      GetMouseButtons(&left, &right, &fik1, &fik2);
     return;
    }

  if (mousey > Y_ais[1] + 1 && mousey < Y_ais[2]) // we are in Button "Rectangle/Sq"
   if (p_buttons[1] -> pressed)
    {
     while (left) // waiting for the button to be released
      GetMouseButtons(&left, &right, &fik1, &fik2);
     HideMouse();
     p_buttons[1] -> Unpress();
     ShowMouse();
     return;
    }
   else
    {
     HideMouse();
     List.Unselectall();
     Release(); // first we release the previous pressed button
     p_buttons[1] -> Press();
     ShowMouse();
     while (left) // waiting for the button to be released
      GetMouseButtons(&left, &right, &fik1, &fik2);
     return;
    }

  if (mousey > Y_ais[2] + 1 && mousey < Y_ais[3]) // we are in Button "Polygon"
   if (p_buttons[2] -> pressed)
    {
     while (left) // waiting for the button to be released
      GetMouseButtons(&left, &right, &fik1, &fik2);
     HideMouse();
     p_buttons[2] -> Unpress();
     ShowMouse();
     return;
    }
   else
    {
     HideMouse();
     List.Unselectall();
     Release(); // first we release the previous pressed button
     p_buttons[2] -> Press();
     ShowMouse();
     while (left) // waiting for the button to be released
      GetMouseButtons(&left, &right, &fik1, &fik2);
     return;
    }
  if (mousey > Y_ais[3] + 1 && mousey < Y_ais[4]) // we are in Button "Ellipse/Circle"
   if (p_buttons[3] -> pressed)
    {
     while (left) // waiting for the button to be released
      GetMouseButtons(&left, &right, &fik1, &fik2);
     HideMouse();
     p_buttons[3] -> Unpress();
     ShowMouse();
     return;
    }
   else
    {
     HideMouse();
     List.Unselectall();
     Release(); // first we release the previous pressed button
     p_buttons[3] -> Press();
     ShowMouse();
     while (left) // waiting for the button to be released
      GetMouseButtons(&left, &right, &fik1, &fik2);
     return;
    }

  if (mousey > Y_ais[4] + 1 && mousey < Y_ais[5]) // we are in Button "Set colors"
   {
// HAVE TO RECUPERATE MARKED IF ANY OF THE BUTTONS 0..3 IS CURRENTLY PRESSED
    HideMouse();
    List.Unselectall();
    Release(); // first we release the previous pressed button
    p_buttons[4] -> Press();
    ShowMouse();
    while (left) // waiting for the button to be released
     GetMouseButtons(&left, &right, &fik1, &fik2);
    HideMouse();
    ShowColors();
    ShowMouse();
    GetColors();
    HideMouse();
    clearviewport();
    List.Draweverything();
    int i = List.Findnull();
    setwritemode(COPY_PUT);
    if (i > 0)
     {
      if (List.ps_Base[i - 1] -> filled)
       List.ps_Base[i - 1] -> fill_object();
      List.ps_Base[i - 1] -> draw_object();
     } // End if i > 0
    Draw();
    p_buttons[4] -> Unpress();
    ShowMouse();
    return;
   }

  if (mousey > Y_ais[5] + 1 && mousey < Y_ais[6]) // we are in Button "Fill selected"
   {
    int current;
    HideMouse();
    Release();
    p_buttons[5] -> Press();
    while (left) // waiting for the button to be released
     GetMouseButtons(&left, &right, &fik1, &fik2);
    current = List.Findselected();
    if (current >= 0)
     List.Fillselected(current);
    p_buttons[5] -> Unpress();
    ShowMouse();
    return;
   }
  if (mousey > Y_ais[6] + 1 && mousey < Y_ais[7]) // we are in Button "Window"
   {
    HideMouse();
    List.Unselectall();
    Release();
    p_buttons[6] -> Press();
    ShowMouse();
    while (left) // waiting for the button to be released
     GetMouseButtons(&left, &right, &fik1, &fik2);
    Zoom();
    HideMouse();
    p_buttons[6] -> Unpress();
    ShowMouse();
    return;
   }
  if (mousey > Y_ais[7] + 1 && mousey < Y_ais[8]) // we are in Button "Load"
   {
    HideMouse();
    List.Unselectall();
    Release();
    p_buttons[7] -> Press();
    while (left) // waiting for the button to be released
     GetMouseButtons(&left, &right, &fik1, &fik2);

    FILE *loadfile;
    char filename[9];
    OpenWindow(150, 170, 362, 230, "Load file");
    GetFileName(filename);
    if ( (loadfile = fopen(filename, "rt")) == NULL)
     {
      printf("\a"); // attention
      p_buttons[7] -> Unpress();
      ShowMouse();
      return;
     }

    List.Loadall(loadfile);
    fclose(loadfile);

    clearviewport();
    List.Draweverything();
    int i = List.Findnull();
    setwritemode(COPY_PUT);
    if (i > 0)
     {
      if (List.ps_Base[i - 1] -> filled)
       List.ps_Base[i - 1] -> fill_object();
      List.ps_Base[i - 1] -> draw_object();
     } // End if i > 0
    Draw();
    p_buttons[7] -> Unpress();
    ShowMouse();
    return;
   }
  if (mousey > Y_ais[8] + 1 && mousey < Y_ais[9]) // we are in Button "Save"
   {
    HideMouse();
    List.Unselectall();
    Release();
    p_buttons[8] -> Press();
    while (left) // waiting for the button to be released
     GetMouseButtons(&left, &right, &fik1, &fik2);

    FILE *savefile;
    char filename[9];
    OpenWindow(150, 170, 362, 230, "Save file");
    GetFileName(filename);
    if ( (savefile = fopen(filename, "wt")) == NULL)
     {
      printf("\a"); // attention
      HideMouse();
      p_buttons[8] -> Unpress();
      ShowMouse();
      return;
     }
    List.Saveall(savefile);
    fclose(savefile);

    clearviewport();
    List.Draweverything();
    int i = List.Findnull();
    setwritemode(COPY_PUT);
    if (i > 0)
     {
      if (List.ps_Base[i - 1] -> filled)
       List.ps_Base[i - 1] -> fill_object();
      List.ps_Base[i - 1] -> draw_object();
     } // End if i > 0
    Draw();
    p_buttons[8] -> Unpress();
    ShowMouse();
    return;
   }
  if (mousey > Y_ais[9] + 1 && mousey < Y_ais[10]) // we are in Button "Exit"
   {
     HideMouse();
     p_buttons[9] -> Press();
     ShowMouse();
      while (left) // waiting for the button to be released
       GetMouseButtons(&left, &right, &fik1, &fik2);
    // clean up
    HideMouse();
    delete p_Menu; // deleting all of the buttons to free up memory...
    int end = List.Findnull();
    for (int i = 0; i < end; i++)
     delete List.ps_Base[i];
    closegraph();
    printf("\nYako a? :-)");
    exit(0);
   }
  while (left) // waiting for the button to be released
   GetMouseButtons(&left, &right, &fik1, &fik2); // when we are not on a button

  return; // if no button is selected return
 }

void TMenu::ShowColors(void)
 {
  int i, j;
  for (i = 0; i < 16; i++)
   for (j = 0; j < 16; j++)
    Block(j*(resmaxx + 1)/16, i*(resmaxy+1)/16, (j + 1)*(resmaxx + 1)/16 - 1, (i + 1)*(resmaxy + 1)/16 - 1, i*16 + j);

  setcolor(26);
  setwritemode(COPY_PUT);
  moveto( (frcolor%16)*(resmaxx + 1)/16, (frcolor/16)*(resmaxy + 1)/16 );
  settextstyle(userfont, HORIZ_DIR, (int) floor( 7.0*(resmaxx+1)/640) + 0.5);
  settextjustify(LEFT_TEXT, TOP_TEXT);
  outtext("FR");
  moveto( (bkcolor%16)*(resmaxx + 1)/16, (bkcolor/16)*(resmaxy + 1)/16 );
  settextjustify(LEFT_TEXT, TOP_TEXT);
  outtext("BK");
 }

void TMenu::GetColors(void)
 {
  char mbleft, mbright;
  int mousex, mousey;
  while (1)
   {
    GetMouseButtons(&mbleft, &mbright, &mousex, &mousey);
    if (mbright)
     {
      while (mbright)
       GetMouseButtons(&mbleft, &mbright, &mousex, &mousey);
      return;
     }
    if (mbleft)
     {
      HideMouse();
      Block( (frcolor%16)*(resmaxx + 1)/16, (frcolor/16)*(resmaxy + 1)/16, (frcolor%16)*(resmaxx + 1)/16 + (resmaxx + 1)/16 - 1, (frcolor/16)*(resmaxy + 1)/16 + (resmaxy + 1)/16 - 1, frcolor);
      setcolor(26);
      setwritemode(COPY_PUT);
      moveto( (mousex/((resmaxx + 1)/16))*((resmaxx + 1)/16), (mousey/((resmaxy + 1)/16))*((resmaxy + 1)/16) );
      settextstyle(userfont, HORIZ_DIR, (int) floor( 7.0*(resmaxx+1)/640) + 0.5);
      settextjustify(LEFT_TEXT, TOP_TEXT);
      outtext("FR");
      frcolor = (unsigned char) (mousey/((resmaxy + 1)/16)*16 + mousex/((resmaxx + 1)/16));
      ShowMouse();
      while (mbleft)
       GetMouseButtons(&mbleft, &mbright, &mousex, &mousey);
      while (1)
       {
	GetMouseButtons(&mbleft, &mbright, &mousex, &mousey);
	if (mbright)
	 {
	  while (mbright)
	   GetMouseButtons(&mbleft, &mbright, &mousex, &mousey);
	  return;
	 }
	if (mbleft)
	 {
	  HideMouse();
	  Block( (bkcolor%16)*(resmaxx + 1)/16, (bkcolor/16)*(resmaxy + 1)/16, (bkcolor%16)*(resmaxx + 1)/16 + (resmaxx + 1)/16 - 1, (bkcolor/16)*(resmaxy + 1)/16 + (resmaxy + 1)/16 - 1, bkcolor);
	  setcolor(26);
	  setwritemode(COPY_PUT);
	  moveto( (mousex/((resmaxx + 1)/16))*((resmaxx + 1)/16), (mousey/((resmaxy + 1)/16))*((resmaxy + 1)/16) );
	  settextstyle(userfont, HORIZ_DIR, (int) floor( 7.0*(resmaxx+1)/640) + 0.5);
	  settextjustify(LEFT_TEXT, TOP_TEXT);
	  outtext("BK");
	  bkcolor = (unsigned char) (mousey/((resmaxy + 1)/16)*16 + mousex/((resmaxx + 1)/16));
	  ShowMouse();
	  while (mbleft)
	   GetMouseButtons(&mbleft, &mbright, &mousex, &mousey);
	  break; // breaking the internal while (1);
	 } // End if (mbleft)
       } // End internal while (1)
     } // End if (mbleft)
   } // End external While (1)
 } // End of function GetColors

void TMenu::Zoom(void)
 {
  char left = 0, right = 0;
  int mousex, mousey;
  int  x1, y1, x2, y2;
  ld userx1, usery1, userx2, usery2;
  while (1)
   {
    GetMouseButtons(&left, &right, &mousex, &mousey);
    if (left && mousex > pos && mousey > Y_ais[6] + 1 && mousey < Y_ais[7])
     {
      while(left)
       GetMouseButtons(&left, &right, &mousex, &mousey);
      return; // pressing again the button Window
     } // End if (left && mousex > pos && mousey > Y_ais[6] + 1 && mousey < Y_ais[7])
    if (left)
     {
      int mousex1 = mousex, mousey1 = mousey;
      HideMouse();
      GetMouseButtons(&left, &right, &mousex, &mousey);
      if (mousex - mousex1 >= 0 && mousey - mousey1 >= 0) // down right
       {
	x1 = mousex1;
	y1 = mousey1;
	x2 = mousex1 + max(mousex - mousex1, mousey - mousey1);
	devicetouser(userx1, usery1, x1, y1);
	devicetouser(userx2, usery1, x2, y1);
	usery2 = usery1 - ((userx2 - userx1)*(resmaxy + 1))/(pos - 1);
	usertodevice(userx2, usery2, x2, y2);
       }
      if (mousex - mousex1 >= 0 && mousey - mousey1 < 0) // up right
       {
	x1 = mousex1;
	y2 = mousey1;
	x2 = mousex1 + max(mousex - mousex1, mousey1 - mousey);
	devicetouser(userx2, usery2, x2, y2);
	devicetouser(userx1, usery2, x1, y2);
	usery1 = usery2 + ((userx2 - userx1)*(resmaxy + 1))/(pos - 1);
	usertodevice(userx1, usery1, x1, y1);
       }
      if (mousex - mousex1 < 0 && mousey - mousey1 < 0) // up left
       {
	x2 = mousex1;
	y2 = mousey1;
	x1 = mousex1 - max(mousex1 - mousex, mousey1 - mousey);
	devicetouser(userx2, usery2, x2, y2);
	devicetouser(userx1, usery2, x1, y2);
	usery1 = usery2 + ((userx2 - userx1)*(resmaxy + 1))/(pos - 1);
	usertodevice(userx1, usery1, x1, y1);
       }
      if (mousex - mousex1 < 0 && mousey - mousey1 >= 0) // down left
       {
	x2 = mousex1;
	y1 = mousey1;
	x1 = mousex1 - max(mousex1 - mousex, mousey - mousey1);
	devicetouser(userx1, usery1, x1, y1);
	devicetouser(userx2, usery1, x2, y1);
	usery2 = usery1 - ((userx2 - userx1)*(resmaxy + 1))/(pos - 1);
       }
      setwritemode(XOR_PUT);
      if (left) EnterRectangle(x1, y1, x2, y2);
      int oldx = mousex, oldy = mousey;
      boolean entered = false;
      while (left)
       {
	GetMouseButtons(&left, &right, &mousex, &mousey);
	if (oldx != mousex || oldy != mousey)
	 {
	  entered = true;
	  EnterRectangle(x1, y1, x2, y2); // delete
	  if (mousex - mousex1 >= 0 && mousey - mousey1 >= 0) // down right
	   {
	    x1 = mousex1;
	    y1 = mousey1;
	    x2 = mousex1 + max(mousex - mousex1, mousey - mousey1);
	    devicetouser(userx1, usery1, x1, y1);
	    devicetouser(userx2, usery1, x2, y1);
	    usery2 = usery1 - ((userx2 - userx1)*(resmaxy + 1))/(pos - 1);
	    usertodevice(userx2, usery2, x2, y2);
	   }
	  if (mousex - mousex1 >= 0 && mousey - mousey1 < 0) // up right
	   {
	    x1 = mousex1;
	    y2 = mousey1;
	    x2 = mousex1 + max(mousex - mousex1, mousey1 - mousey);
	    devicetouser(userx2, usery2, x2, y2);
	    devicetouser(userx1, usery2, x1, y2);
	    usery1 = usery2 + ((userx2 - userx1)*(resmaxy + 1))/(pos - 1);
	    usertodevice(userx1, usery1, x1, y1);
	   }
	  if (mousex - mousex1 < 0 && mousey - mousey1 < 0) // up left
	   {
	    x2 = mousex1;
	    y2 = mousey1;
	    x1 = mousex1 - max(mousex1 - mousex, mousey1 - mousey);
	    devicetouser(userx2, usery2, x2, y2);
	    devicetouser(userx1, usery2, x1, y2);
	    usery1 = usery2 + ((userx2 - userx1)*(resmaxy + 1))/(pos - 1);
	    usertodevice(userx1, usery1, x1, y1);
	   }
	  if (mousex - mousex1 < 0 && mousey - mousey1 >= 0) // down left
	   {
	    x2 = mousex1;
	    y1 = mousey1;
	    x1 = mousex1 - max(mousex1 - mousex, mousey - mousey1);
	    devicetouser(userx1, usery1, x1, y1);
	    devicetouser(userx2, usery1, x2, y1);
	    usery2 = usery1 - ((userx2 - userx1)*(resmaxy + 1))/(pos - 1);
	    usertodevice(userx2, usery2, x2, y2);
	   }
	  EnterRectangle(x1, y1, x2, y2); // draw
	  draw_uvxy();
	  oldx = mousex; oldy = mousey;
	 } //End if (oldx != mousex || oldy != mousey)
       } // end while (left)
      if (entered)
       {
	clearviewport();
	Window(userx1, usery1, userx2);
	List.Draweverything();
	int i = List.Findnull();
	if (i > 0)
	 {
	  if (List.ps_Base[i - 1] -> filled)
	   {
	    setwritemode(COPY_PUT);
	    List.ps_Base[i - 1] -> fill_object();
	   }
	  setwritemode(COPY_PUT);
	  List.ps_Base[i - 1] -> draw_object();
	 } // End if i > 0
	Draw();
       } // end if we have seen the dashed rectangle
      ShowMouse();
     } // End if (left)
    else
// now we want to zoom out ...
     if (right)
      {
       HideMouse();
       while (right)  // waiting to the mouse button to be released
	GetMouseButtons(&left, &right, &mousex, &mousey);
       userx1 = U1 - (U2 - U1)/2;
       usery1 = V1 - (V2 - V1)/2;
       userx2 = U2 + (U2 - U1)/2;
       clearviewport();
       Window(userx1, usery1, userx2);
       List.Draweverything();
       int i = List.Findnull();
       if (i > 0)
	{
	 if (List.ps_Base[i - 1] -> filled)
	  {
	   setwritemode(COPY_PUT);
	   List.ps_Base[i - 1] -> fill_object();
	  }
	 setwritemode(COPY_PUT);
	 List.ps_Base[i - 1] -> draw_object();
	} // End if i > 0
       Draw();
       ShowMouse();
      } // End if right
   } // End while (1)
 } // End of function Zoom
