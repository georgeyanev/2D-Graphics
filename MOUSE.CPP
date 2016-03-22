// Module for working with the mouse
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include "graphpro.h"
#define C 101
static struct REGPACK registers; //ax, bx, cx, dx, bp, si, di, ds, es, flags
//static union REGS commonregs; //ax(al, ah), bx(bl, bh), cx(cl, ch), dx(dl, dh)
extern char msg[81];
static int cursorx, cursory;

void IsMouse(void)  // checking for Mouse driver
 {
  registers.r_ax = 0x00;
  intr(0x33, &registers);
  if (registers.r_ax == 0x0000)
   {
    printf("\n\a Mouse driver not loaded. Exiting...");
    exit(1);
   }
 }

void GetMouseButtons(char* LeftYN, char* RightYN, int* mousex, int* mousey)
 {
  registers.r_ax = 0x03;
  intr(0x33, &registers);

  if ( (registers.r_bx & 1) == 1)
   *LeftYN = 1;
  else
   *LeftYN = 0;

  if ( (registers.r_bx & 2) == 2)
   *RightYN = 1;
  else
   *RightYN = 0;

  *mousex = registers.r_cx;
  *mousey = registers.r_dx;
 } // End of function GetMouseButtons

void HideMouse(void)
 { // hiding the mouse pointer by drawing again with XOR_PUT mode ...
  setwritemode(XOR_PUT);
  BLine(cursorx, cursory - 2, cursorx, cursory - 10, C);
  BLine(cursorx, cursory + 2, cursorx, cursory + 10, C);
  BLine(cursorx - 2, cursory, cursorx - 10, cursory, C);
  BLine(cursorx + 2, cursory, cursorx + 10, cursory, C);
  // disabling the mouse event handler ...
  registers.r_ax = 0x0c;
  registers.r_cx = 0x00;
  registers.r_es = FP_SEG(mouse_event_handler);
  registers.r_dx = FP_OFF(mouse_event_handler);
  intr(0x33, &registers);
 }

void ShowMouse(void)
 {
  char fikt1, fikt2;
  int mousex, mousey;

  GetMouseButtons(&fikt1, &fikt2, &mousex, &mousey);
  cursorx = mousex;
  cursory = mousey;
  // and now draw to the new cursor place ...
  setwritemode(XOR_PUT);
  BLine(cursorx, cursory - 2, cursorx, cursory - 10, C);
  BLine(cursorx, cursory + 2, cursorx, cursory + 10, C);
  BLine(cursorx - 2, cursory, cursorx - 10, cursory, C);
  BLine(cursorx + 2, cursory, cursorx + 10, cursory, C);
  // istalling again the mouse event handler ...
  registers.r_ax = 0x0c;
  registers.r_cx = 0x01;
  registers.r_es = FP_SEG(mouse_event_handler);
  registers.r_dx = FP_OFF(mouse_event_handler);
  intr(0x33, &registers);
 }

void InitMouse(int& mouse_maxX, int& mouse_maxY)
 // functions to initialize the mouse pointer
 {//reseting the mouse ...
  registers.r_ax = 0x00;
  intr(0x33, &registers);
  // determining the minX maxX coordinates of the mouse ...
  registers.r_ax = 0x07;
  registers.r_cx = 0x00;   // min X coordinate
  registers.r_dx = mouse_maxX;   // max X coordinate
  intr(0x33, &registers);
  // determining the minY maxY coordinates of the mouse ...
  registers.r_ax = 0x08;
  registers.r_cx = 0x00;   // min Y coordinate
  registers.r_dx = mouse_maxY;   // max Y coordinate
  intr(0x33, &registers);
  // set the start coordinates ...
  registers.r_ax = 0x04;
  registers.r_cx = cursorx = mouse_maxX/2;
  registers.r_dx = cursory = mouse_maxY/2;
  intr(0x33, &registers);
  // drawing the first cursor ...
  setwritemode(XOR_PUT);
  BLine(cursorx, cursory - 2, cursorx, cursory - 10, C);
  BLine(cursorx, cursory + 2, cursorx, cursory + 10, C);
  BLine(cursorx - 2, cursory, cursorx - 10, cursory, C);
  BLine(cursorx + 2, cursory, cursorx + 10, cursory, C);

  // installing event handler ...
  registers.r_ax = 0x0c;
  registers.r_cx = 0x01;
  registers.r_es = FP_SEG(mouse_event_handler);
  registers.r_dx = FP_OFF(mouse_event_handler);
  intr(0x33, &registers);
 }

void SetMouseBound(int mouse_minX, int mouse_minY, int mouse_maxX, int mouse_maxY)
 { // this function sets the current mouse boundaries
  // determining the minX maxX coordinates of the mouse ...
  registers.r_ax = 0x07;
  registers.r_cx = mouse_minX;   // min X coordinate
  registers.r_dx = mouse_maxX;   // max X coordinate
  intr(0x33, &registers);
  // determining the minY maxY coordinates of the mouse ...
  registers.r_ax = 0x08;
  registers.r_cx = mouse_minY;   // min Y coordinate
  registers.r_dx = mouse_maxY;   // max Y coordinate
  intr(0x33, &registers);
 }



void mouse_event_handler(void) // used only to draw the mouse cursor
 {
  //in these registers are the coordinates of the Mouse pointer
  register int mousex = _CX, mousey = _DX;

  setwritemode(XOR_PUT);
  //first we draw in the old cursor place
  BLine(cursorx, cursory - 2, cursorx, cursory - 10, C);
  BLine(cursorx, cursory + 2, cursorx, cursory + 10, C);
  BLine(cursorx - 2, cursory, cursorx - 10, cursory, C);
  BLine(cursorx + 2, cursory, cursorx + 10, cursory, C);
  cursorx = mousex;
  cursory = mousey;
  //and now to the new cursor place
  BLine(cursorx, cursory - 2, cursorx, cursory - 10, C);
  BLine(cursorx, cursory + 2, cursorx, cursory + 10, C);
  BLine(cursorx - 2, cursory, cursorx - 10, cursory, C);
  BLine(cursorx + 2, cursory, cursorx + 10, cursory, C);
  draw_uvxy();
 }


