#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "graphpro.h"

extern unsigned char frcolor, bkcolor;
extern ld U1, V1, U2, V2;   // user window
extern int X1, Y1, X2, Y2;   // monitor display
extern int resmaxx, resmaxy, pos, userfont;
static TLine *p_Line; // it can't be used in other files
static TEllipse *p_Ellipse; // it can't be used in other files
static TPolygon *p_Polygon; // it can't be used in other files

boolean TBase::marked = false;


// member functions of the class TBase ...

boolean TBase::check_in(const int& mousex, const int& mousey) // checks if the mouse is in the limit rectangle
 {
  int x1, y1, x2, y2;
  usertodevice(limitx1, limity1, x1, y1);
  usertodevice(limitx2, limity2, x2, y2);
  if (x1 <= mousex && y1 <= mousey && x2 >= mousex && y2 >= mousey)
   {
    return(true);
   }
  return(false);
 }

void TBase::select_object(void)
 {
  int step;
  int x1, y1, x2, y2;
  step = floor(2.0*(resmaxx + 1)/640 + 0.5);
  usertodevice(limitx1, limity1, x1, y1);
  usertodevice(limitx2, limity2, x2, y2);
  selected = true;
  marked = true;
  setwritemode(XOR_PUT);
  Block1(x1 - step, y1 - step, x1 + step, y1 + step, 15);
  Block1((x2 + x1)/2 - step, y1 - step, (x2 + x1)/2 + step, y1 + step, 15);
  Block1(x2 - step, y1 - step, x2 + step, y1 + step, 15);
  Block1(x1 - step, (y2 + y1)/2 - step, x1 + step, (y2 + y1)/2 + step, 15);
  Block1(x2 - step, (y2 + y1)/2 - step, x2 + step, (y2 + y1)/2 + step, 15);
  Block1(x1 - step, y2 - step, x1 + step, y2 + step, 15);
  Block1((x2 + x1)/2 - step, y2 - step, (x2 + x1)/2 + step, y2 + step, 15);
  Block1(x2 - step, y2 - step, x2 + step, y2 + step, 15);
 } // End of function select_object

void TBase::unselect_object(void)
 {
  int step;
  step = floor(2.0*(resmaxx + 1)/640 + 0.5);
  int x1, y1, x2, y2;
  usertodevice(limitx1, limity1, x1, y1);
  usertodevice(limitx2, limity2, x2, y2);
  selected = false;
  marked = false;
  setwritemode(XOR_PUT);
  Block1(x2 - step, y2 - step, x2 + step, y2 + step, 15);
  Block1((x2 + x1)/2 - step, y2 - step, (x2 + x1)/2 + step, y2 + step, 15);
  Block1(x1 - step, y2 - step, x1 + step, y2 + step, 15);
  Block1(x2 - step, (y2 + y1)/2 - step, x2 + step, (y2 + y1)/2 + step, 15);
  Block1(x1 - step, (y2 + y1)/2 - step, x1 + step, (y2 + y1)/2 + step, 15);
  Block1(x2 - step, y1 - step, x2 + step, y1 + step, 15);
  Block1((x2 + x1)/2 - step, y1 - step, (x2 + x1)/2 + step, y1 + step, 15);
  Block1(x1 - step, y1 - step, x1 + step, y1 + step, 15);
 }

boolean TBase::check_in_center(const int& mousex, const int& mousey)
 {
  int step;
  step = floor(3.0*(resmaxx + 1)/640 + 0.5);
  int x, y;
  usertodevice(centerx, centery, x, y);
  if (x - step <= mousex && y - step <= mousey && x + step >= mousex && y + step >= mousey)
   return(true);
  else
   return(false);
 }

boolean TBase::check_for_rotation(const int& mousex, const int& mousey)
 {
  int step;
  step = floor(3.0*(resmaxx + 1)/640 + 0.5);
  int x1, y1, x2, y2;
  usertodevice(limitx1, limity1, x1, y1);
  usertodevice(limitx2, limity2, x2, y2);
  if (x1 - step <= mousex && y1 - step <= mousey && x1 + step >= mousex && y1 + step >= mousey
      ||
      x1 - step <= mousex && y2 - step <= mousey && x1 + step >= mousex && y2 + step >= mousey)
   return(true);
  return(false);
 }

boolean TBase::check_for_dilatation(const int& mousex, const int& mousey, Tdirection& direction)
 {
  int step;
  step = floor(3.0*(resmaxx + 1)/640 + 0.5);
  int x1, y1, x2, y2;
  usertodevice(limitx1, limity1, x1, y1);
  usertodevice(limitx2, limity2, x2, y2);
  if (x1 - step <= mousex && (y2 + y1)/2 - step <= mousey && x1 + step >= mousex && (y2 + y1)/2 + step >= mousey)
   {
    direction = west;
    return(true);
   }
  if (x2 - step <= mousex && (y2 + y1)/2 - step <= mousey && x2 + step >= mousex && (y2 + y1)/2 + step >= mousey)
   {
    direction = east;
    return(true);
   }
  if ((x2 + x1)/2 - step <= mousex && y1 - step <= mousey && (x2 + x1)/2 + step >= mousex && y1 + step >= mousey)
   {
    direction = north;
    return(true);
   }
  if ((x2 + x1)/2 - step <= mousex && y2 - step <= mousey && (x2 + x1)/2 + step >= mousex && y2 + step >= mousey)
   {
    direction = south;
    return(true);
   }
  return(false);
 }

void TBase::select_for_rot(void)   //  select for rotation
 {
  int step;
  step = floor(2.0*(resmaxx + 1)/640 + 0.5);
  int x1, y1, x2, y2, x, y;
  usertodevice(limitx1, limity1, x1, y1);
  usertodevice(limitx2, limity2, x2, y2);
  usertodevice(centerx, centery, x, y);
  setwritemode(XOR_PUT);
  setlinestyle(DOTTED_LINE, 0, NORM_WIDTH); // 0 means not user defined
  setcolor(15);
  line(x1, y1, x2, y1);
  line(x2, y1, x2, y2);
  line(x2, y2, x1, y2);
  line(x1, y2, x1, y1);
  Block1(x - step, y - step, x + step, y + step, 15);
 }

void TBase::unselect_from_rot(void)
 {
  int step;
  step = floor(2.0*(resmaxx + 1)/640 + 0.5);
  int x1, y1, x2, y2, x, y;
  usertodevice(limitx1, limity1, x1, y1);
  usertodevice(limitx2, limity2, x2, y2);
  usertodevice(centerx, centery, x, y);
  setwritemode(XOR_PUT);
  setlinestyle(DOTTED_LINE, 0, NORM_WIDTH); // 0 means not user defined
  setcolor(15);
  Block1(x - step, y - step, x + step, y + step, 15);
  line(x1, y2, x1, y1);
  line(x2, y2, x1, y2);
  line(x2, y1, x2, y2);
  line(x1, y1, x2, y1);
 }

void TBase::move_center(void) // non virtual
 {
  char left, right;
  int mousex, mousey, oldx, oldy;
  int step;
  step = floor(2.0*(resmaxx + 1)/640 + 0.5);
  int x, y;
  usertodevice(centerx, centery, x, y);
  GetMouseButtons(&left, &right, &mousex, &mousey);
  oldx = mousex; oldy = mousey;
  SetMouseBound(0, 0, pos - 2, resmaxy);
  setwritemode(XOR_PUT);
  while (left)
   {
    GetMouseButtons(&left, &right, &mousex, &mousey);
    if (oldx != mousex || oldy != mousey)
     {
      // deleting the old block
      Block1(x - step, y - step, x + step, y + step, 15);
      x += mousex - oldx;
      y += mousey - oldy;
      // drawing the new block
      Block1(x - step, y - step, x + step, y + step, 15);
      draw_uvxy();
      oldx = mousex; oldy = mousey;
     } // end if
   } // End While
  devicetouser(centerx, centery, x, y);
  SetMouseBound(0, 0, resmaxx, resmaxy);
 }


// member functions of the class TLine ...

TLine::TLine(const int& x1, const int& y1) // first constructor
 {
  char LeftYN, RightYN;
  int mousex, mousey, x2, y2;
  x2 = x1; y2 = y1;
  HideMouse();
  color = frcolor;
  fillcolor = 0;
  GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
  if (LeftYN == 1) BLine(x1, y1, x2, y2, color);
  draw_uvxy();
  setwritemode(XOR_PUT);
  SetMouseBound(0, 0, pos - 2, resmaxy);
  while (LeftYN == 1)
   {
    GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
    if (x2 != mousex || y2 != mousey)
     {
      BLine(x1, y1, x2, y2, color); // delete the old line
      BLine(x1, y1, mousex, mousey, color); // draw the new line
      draw_uvxy();
      x2 = mousex, y2 = mousey;
     } //End if
   } // end while (LeftYN == 1)
  setwritemode(COPY_PUT);
  BLine(x1, y1, x2, y2, color); // draw the new line
  devicetouser(point1[0], point1[1], x1, y1);
  point1[2] = 1; // homogenious coordinates
  devicetouser(point2[0], point2[1], x2, y2);
  point2[2] = 1; // homogenios coordinates
  limitx1 = min(point1[0], point2[0]);
  limity1 = max(point1[1], point2[1]);
  limitx2 = max(point1[0], point2[0]);
  limity2 = min(point1[1], point2[1]);
  centerx = (limitx1 + limitx2)/2;
  centery = (limity1 + limity2)/2;
  selected = false;
  marked = false;
  filled = false;
  SetMouseBound(0, 0, resmaxx, resmaxy);
  ShowMouse();
 } // End of the first constructor

TLine::TLine(FILE* loadfile) // second constructor
 {
  int intbuffer; ld ldbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  color = intbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  fillcolor = intbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  filled = (boolean) intbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  point1[0] = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  point1[1] = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  point2[0] = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  point2[1] = ldbuffer;
  point1[2] = point2[2] = 1;
  limitx1 = min(point1[0], point2[0]);
  limity1 = max(point1[1], point2[1]);
  limitx2 = max(point1[0], point2[0]);
  limity2 = min(point1[1], point2[1]);
  centerx = (limitx1 + limitx2)/2;
  centery = (limity1 + limity2)/2;
  selected = false;
  marked = false;
  fgetc(loadfile);
  fgetc(loadfile);
  if (!feof(loadfile)) fseek(loadfile, -1L, SEEK_CUR);
 }

void TLine::draw_object(void)
 {
  userline(point1[0], point1[1], point2[0], point2[1], color);
  draw_uvxy();
 }

void TLine::translate_object(vector p)
 {
  point1[0] += p[0];
  point1[1] += p[1];
  point2[0] += p[0];
  point2[1] += p[1];
  point1[2] = 1; // homogenious coordinates
  point2[2] = 1; // homogenious coordinates
  limitx1 += p[0];
  limity1 += p[1];
  limitx2 += p[0];
  limity2 += p[1];
  centerx += p[0];
  centery += p[1];
 }

void TLine::rotate_object(matrix m, const ld& usercenterx, const ld& usercentery, const ld& angle)
 {
  vector dummy1, dummy2;
  dummy1[0] = point1[0] - usercenterx;
  dummy1[1] = point1[1] - usercentery;
  dummy1[2] = 1; // homogenious coordinates
  dummy2[0] = point2[0] - usercenterx;
  dummy2[1] = point2[1] - usercentery;
  dummy2[2] = 1;
  mul_matrix_vector(dummy1, m);
  mul_matrix_vector(dummy2, m);
  point1[0] = dummy1[0] + usercenterx;
  point1[1] = dummy1[1] + usercentery;
  point2[0] = dummy2[0] + usercenterx;
  point2[1] = dummy2[1] + usercentery;
  point1[2] = angle;
  point1[2] = 1; // homogenious coordinates
  point2[2] = 1; // homogenious coordinates
  limitx1 = min(point1[0], point2[0]);
  limity1 = max(point1[1], point2[1]);
  limitx2 = max(point1[0], point2[0]);
  limity2 = min(point1[1], point2[1]);
 }

void TLine::dilatate_object(vector p, const Tdirection& direction)
 {
  switch (direction)
   {
    case east :
     if (point1[0] - point2[0] < 0)
      {
       point2[0] += p[0];
       limitx2 += p[0];
      }
     else
      if (point1[0] - point2[0] > 0)
       {
	point1[0] += p[0];
	limitx2 += p[0];
       }
      else break;
    break;

    case west :
     if (point1[0] - point2[0] < 0)
      {
       point1[0] += p[0];
       limitx1 += p[0];
      }
     else
      if (point1[0] - point2[0] > 0)
       {
	point2[0] += p[0];
	limitx1 += p[0];
       }
      else break;
    break;

    case north :
     if (point1[1] - point2[1] > 0)
      {
       point1[1] += p[1];
       limity1 += p[1];
      }
     else
      if (point1[1] - point2[1] < 0)
       {
	point2[1] += p[1];
	limity1 += p[1];
       }
      else break;
    break;
    case south :
     if (point1[1] - point2[1] > 0)
      {
       point2[1] += p[1];
       limity2 += p[1];
      }
     else
      if (point1[1] - point2[1] < 0)
       {
	point1[1] += p[1];
	limity2 += p[1];
       }
      else break;
   } // End switch
  centerx = (limitx1 + limitx2)/2;
  centery = (limity1 + limity2)/2;
 }

void TLine::fill_object(void)
 {
  filled = true;
 }

void TLine::save_object(FILE* savefile)
 {
  fseek(savefile, 0L, SEEK_END);
  fprintf(savefile, "LINE ");
  fprintf(savefile, "%u ", color);
  fprintf(savefile, "%u ", fillcolor);
  fprintf(savefile, "%d ", filled);
  fprintf(savefile, "%.17Le ", point1[0]);
  fprintf(savefile, "%.17Le ", point1[1]);
  fprintf(savefile, "%.17Le ", point2[0]);
  fprintf(savefile, "%.17Le\n", point2[1]);
 }

kind TLine::kind_of_object(void)
 {
  return (LINE);
 }


//member functions of the class TPolygon

TPolygon::TPolygon(const int& x1, const int& y1, const kind& type) // first constructor
 {
  char LeftYN, RightYN;
  int mousex = x1, mousey = y1;
  int oldx = mousex, oldy = mousey;
  HideMouse();
  color = frcolor;
  fillcolor = 0;
  SetMouseBound(0, 0, pos - 2, resmaxy);
  setwritemode(XOR_PUT);
  switch (type)
   {
    int recx1, recy1, recx2, recy2;
    case RECTANGLE :
      GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
      if (mousex - x1 >= 0 && mousey - y1 >= 0) // down right
       {
	EnterRectangle1(x1, y1, mousex, mousey, color);
	recx1 = x1; recy1 = y1; recx2 = mousex; recy2 = mousey;
       }
      if (mousex - x1 >= 0 && mousey - y1 < 0) // up right
       {
	EnterRectangle1(x1, mousey, mousex, y1, color);
	recx1 = x1; recy1 = mousey; recx2 = mousex; recy2 = y1;
       }
      if (mousex - x1 < 0 && mousey - y1 < 0) // up left
       {
	EnterRectangle1(mousex, mousey, x1, y1, color);
	recx1 = mousex; recy1 = mousey; recx2 = x1; recy2 = y1;
       }
      if (mousex - x1 < 0 && mousey - y1 >= 0) // down left
       {
	EnterRectangle1(mousex, y1, x1, mousey, color);
	recx1 = mousex; recy1 = y1; recx2 = x1; recy2 = mousey;
       }
      while (LeftYN)
       {
	GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
	if (oldx != mousex || oldy != mousey)
	 {
	  if (mousex - x1 >= 0 && mousey - y1 >= 0) // down right
	   {
	    EnterRectangle1(x1, y1, oldx, oldy, color); // delete
	    EnterRectangle1(x1, y1, mousex, mousey, color); // draw
	    recx1 = x1; recy1 = y1; recx2 = mousex; recy2 = mousey;
	   }
	  if (mousex - x1 >= 0 && mousey - y1 < 0) // up right
	   {
	    EnterRectangle1(x1, oldy, oldx, y1, color); // delete
	    EnterRectangle1(x1, mousey, mousex, y1, color); // draw
	    recx1 = x1; recy1 = mousey; recx2 = mousex; recy2 = y1;
	   }
	  if (mousex - x1 < 0 && mousey - y1 < 0) // up left
	   {
	    EnterRectangle1(oldx, oldy, x1, y1, color); // delete
	    EnterRectangle1(mousex, mousey, x1, y1, color); // draw
	    recx1 = mousex; recy1 = mousey; recx2 = x1; recy2 = y1;
	   }
	  if (mousex - x1 < 0 && mousey - y1 >= 0) // down left
	   {
	    EnterRectangle1(oldx, y1, x1, oldy, color); // delete
	    EnterRectangle1(mousex, y1, x1, mousey, color); // draw
	    recx1 = mousex; recy1 = y1; recx2 = x1; recy2 = mousey;
	   }
	  draw_uvxy();
	  oldx = mousex; oldy = mousey;
	 } //End if (oldx != mousex || oldy != mousey)
       } // end while (left)
      setwritemode(COPY_PUT);
      EnterRectangle1(recx1, recy1, recx2, recy2, color);
      TPoints *p_currentpoint;
      ld userrecx1, userrecy1, userrecx2, userrecy2;
      devicetouser(userrecx1, userrecy1, recx1, recy1);
      devicetouser(userrecx2, userrecy2, recx2, recy2);
      p_pointlist = new TPoints;
      p_currentpoint = p_pointlist;
      p_currentpoint -> point[0] = userrecx1;
      p_currentpoint -> point[1] = userrecy1;
      p_currentpoint -> point[2] = 1;
      p_currentpoint -> p_next = new TPoints;
      p_currentpoint = p_currentpoint -> p_next;
      p_currentpoint -> point[0] = userrecx2;
      p_currentpoint -> point[1] = userrecy1;
      p_currentpoint -> point[2] = 1;
      p_currentpoint -> p_next = new TPoints;
      p_currentpoint = p_currentpoint -> p_next;
      p_currentpoint -> point[0] = userrecx2;
      p_currentpoint -> point[1] = userrecy2;
      p_currentpoint -> point[2] = 1;
      p_currentpoint -> p_next = new TPoints;
      p_currentpoint = p_currentpoint -> p_next;
      p_currentpoint -> point[0] = userrecx1;
      p_currentpoint -> point[1] = userrecy2;
      p_currentpoint -> point[2] = 1;
      p_currentpoint -> p_next = NULL;
      MakeRebraList();
      break; // break case RECTANGLE
    case SQUARE :
      int length;
      GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
      if (mousex - x1 >= 0 && mousey - y1 >= 0) // down right
       {
	length = max(mousex - x1, mousey - y1);
	EnterRectangle1(x1, y1, x1 + length, y1 + length, color);
	recx1 = x1; recy1 = y1; recx2 = x1 + length; recy2 = y1 + length;
       }
      if (mousex - x1 >= 0 && mousey - y1 < 0) // up right
       {
	length = max(mousex - x1, y1 - mousey);
	EnterRectangle1(x1, y1 - length, x1 + length, y1, color);
	recx1 = x1; recy1 = y1 - length; recx2 = x1 + length; recy2 = y1;
       }
      if (mousex - x1 < 0 && mousey - y1 < 0) // up left
       {
	length = max(x1 - mousex, y1 - mousey);
	EnterRectangle1(x1 - length, y1 - length, x1, y1, color);
	recx1 = x1 - length; recy1 = y1 - length; recx2 = x1; recy2 = y1;
       }
      if (mousex - x1 < 0 && mousey - y1 >= 0) // down left
       {
	length = max(x1 - mousex, mousey - y1);
	EnterRectangle1(x1 - length, y1, x1, y1 + length, color);
	recx1 = x1 - length; recy1 = y1; recx2 = x1; recy2 = y1 + length;
       }
      while (RightYN)
       {
	GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
	if (oldx != mousex || oldy != mousey)
	 {
	  if (mousex - x1 >= 0 && mousey - y1 >= 0) // down right
	   {
	    EnterRectangle1(recx1, recy1, recx2, recy2, color); // delete
	    length = max(mousex - x1, mousey - y1);
	    EnterRectangle1(x1, y1, x1 + length, y1 + length, color); // draw
	    recx1 = x1; recy1 = y1; recx2 = x1 + length; recy2 = y1 + length;
	   }
	  if (mousex - x1 >= 0 && mousey - y1 < 0) // up right
	   {
	    EnterRectangle1(recx1, recy1, recx2, recy2, color); // delete
	    length = max(mousex - x1, y1 - mousey);
	    EnterRectangle1(x1, y1 - length, x1 + length, y1, color);
	    recx1 = x1; recy1 = y1 - length; recx2 = x1 + length; recy2 = y1;
	   }
	  if (mousex - x1 < 0 && mousey - y1 < 0) // up left
	   {
	    EnterRectangle1(recx1, recy1, recx2, recy2, color); // delete
	    length = max(x1 - mousex, y1 - mousey);
	    EnterRectangle1(x1 - length, y1 - length, x1, y1, color);
	    recx1 = x1 - length; recy1 = y1 - length; recx2 = x1; recy2 = y1;
	   }
	  if (mousex - x1 < 0 && mousey - y1 >= 0) // down left
	   {
	    EnterRectangle1(recx1, recy1, recx2, recy2, color); // delete
	    length = max(x1 - mousex, mousey - y1);
	    EnterRectangle1(x1 - length, y1, x1, y1 + length, color);
	    recx1 = x1 - length; recy1 = y1; recx2 = x1; recy2 = y1 + length;
	   }
	  draw_uvxy();
	  oldx = mousex; oldy = mousey;
	 } //End if (oldx != mousex || oldy != mousey)
       } // end while (left)
      setwritemode(COPY_PUT);
      EnterRectangle1(recx1, recy1, recx2, recy2, color);
      devicetouser(userrecx1, userrecy1, recx1, recy1);
      devicetouser(userrecx2, userrecy2, recx2, recy2);
      p_pointlist = new TPoints;
      p_currentpoint = p_pointlist;
      p_currentpoint -> point[0] = userrecx1;
      p_currentpoint -> point[1] = userrecy1;
      p_currentpoint -> point[2] = 1;
      p_currentpoint -> p_next = new TPoints;
      p_currentpoint = p_currentpoint -> p_next;
      p_currentpoint -> point[0] = userrecx2;
      p_currentpoint -> point[1] = userrecy1;
      p_currentpoint -> point[2] = 1;
      p_currentpoint -> p_next = new TPoints;
      p_currentpoint = p_currentpoint -> p_next;
      p_currentpoint -> point[0] = userrecx2;
      p_currentpoint -> point[1] = userrecy2;
      p_currentpoint -> point[2] = 1;
      p_currentpoint -> p_next = new TPoints;
      p_currentpoint = p_currentpoint -> p_next;
      p_currentpoint -> point[0] = userrecx1;
      p_currentpoint -> point[1] = userrecy2;
      p_currentpoint -> point[2] = 1;
      p_currentpoint -> p_next = NULL;
      MakeRebraList();
    break; // break case SQUARE

    case POLY :
      TPoints* p_previouspoint = NULL;
      p_pointlist = new TPoints;
      p_currentpoint = p_pointlist;
      ld userpointx, userpointy;
      int prevx1 = x1, prevy1 = y1, lines = 0;
      devicetouser(userpointx, userpointy, x1, y1);
      p_currentpoint -> point[0] = userpointx;
      p_currentpoint -> point[1] = userpointy;
      p_currentpoint -> p_next = new TPoints;
      p_currentpoint = p_currentpoint -> p_next;
      GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
      while (1)
       { // At least three times have to pass by here ...
	while (LeftYN) // whaiting to be released
	 GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
	oldx = mousex; oldy = mousey;
	setwritemode(XOR_PUT);
	BLine(prevx1, prevy1, oldx, oldy, color);
	while (LeftYN == 0)
	 {
	  GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
	  if (RightYN && lines >= 2) break; // break while (!LeftYN)
	  if (oldx != mousex || oldy != mousey)
	   {
	    BLine(prevx1, prevy1, oldx, oldy, color); // delete the old line
	    BLine(prevx1, prevy1, mousex, mousey, color); // draw the new line
	    draw_uvxy();
	    oldx = mousex, oldy = mousey;
	   } //End if
	 } // end while (LeftYN == 0)
	if (RightYN && lines >= 2)
	 {
	  BLine(prevx1, prevy1, oldx, oldy, color); // delete the last
	  setwritemode(COPY_PUT);
	  BLine(prevx1, prevy1, x1, y1, color); // draw the last
	  p_previouspoint -> p_next = NULL;
	  break; // break the endless loop
	 }
	lines++;
	setwritemode(COPY_PUT);
	BLine(prevx1, prevy1, oldx, oldy, color); // draw the new line
	devicetouser(userpointx, userpointy, oldx, oldy);
	p_currentpoint -> point[0] = userpointx;
	p_currentpoint -> point[1] = userpointy;
	p_currentpoint -> p_next = new TPoints;
	p_previouspoint = p_currentpoint;
	p_currentpoint = p_currentpoint -> p_next;
	prevx1 = oldx, prevy1 = oldy;
       } // End Endless loop :-)
      MakeRebraList();
      break; // break case POLY
   } // End switch(type)

  limitx1 = minpoint(0, p_pointlist); // 0 means we compare the X-s
  limity1 = maxpoint(1, p_pointlist); // 1 means we compare the Y-s
  limitx2 = maxpoint(0, p_pointlist);
  limity2 = minpoint(1, p_pointlist);
  centerx = (limitx1 + limitx2)/2;
  centery = (limity1 + limity2)/2;
  selected = false;
  marked = false;
  filled = false;
  SetMouseBound(0, 0, resmaxx, resmaxy);
  ShowMouse();
 } // End of first constructor of TPolygon

TPolygon::TPolygon(FILE* loadfile) // second constructor
 {
  int intbuffer; ld ldbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  color = intbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  fillcolor = intbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  filled = (boolean) intbuffer;
  p_pointlist = new TPoints;
  signed char ch = ' ';
  TPoints *p_currentpoint = p_pointlist, *p_previouspoint;
  while (ch == ' ')
   {
    fscanf(loadfile, "%Le", &ldbuffer);
    p_currentpoint -> point[0] = ldbuffer;
    fscanf(loadfile, "%Le", &ldbuffer);
    p_currentpoint -> point[1] = ldbuffer;
    p_currentpoint -> point[2] = 1;
    p_currentpoint -> p_next = new TPoints;
    p_previouspoint = p_currentpoint;
    p_currentpoint = p_currentpoint -> p_next;
    ch = fgetc(loadfile);
   }
  delete p_previouspoint -> p_next;
  p_previouspoint -> p_next = NULL;
  MakeRebraList();
  limitx1 = minpoint(0, p_pointlist); // 0 means we compare the X-s
  limity1 = maxpoint(1, p_pointlist); // 1 means we compare the Y-s
  limitx2 = maxpoint(0, p_pointlist);
  limity2 = minpoint(1, p_pointlist);
  centerx = (limitx1 + limitx2)/2;
  centery = (limity1 + limity2)/2;
  selected = false;
  marked = false;
  fgetc(loadfile);
  if (!feof(loadfile)) fseek(loadfile, -1L, SEEK_CUR);
 }

TPolygon::~TPolygon()
 {
  TPoints** pp_pointlist = &p_pointlist;
  while (*pp_pointlist) // the points
   {
    while ( (*pp_pointlist) -> p_next)
     pp_pointlist = &( (*pp_pointlist) -> p_next );

    delete *pp_pointlist;
    *pp_pointlist = NULL;
    pp_pointlist = &p_pointlist;
   }
  TRebra** pp_rebralist = &p_rebralist;
  while (*pp_rebralist) // rebrata
   {
    while ( (*pp_rebralist) -> p_next)
     pp_rebralist = &( (*pp_rebralist) -> p_next );

    delete *pp_rebralist;
    *pp_rebralist = NULL;
    pp_rebralist = &p_rebralist;
   }
 }

void TPolygon::MakeRebraList(void)
 {
  TRebra* p_currentrebro;
  TPoints* p_currentpoint;

  p_rebralist = new TRebra;
  p_currentrebro = p_rebralist;
  p_currentpoint = p_pointlist;
  while (p_currentpoint -> p_next)
   {
    p_currentrebro -> p_point1 = p_currentpoint;
    p_currentrebro -> p_point2 = p_currentpoint -> p_next;
    p_currentpoint = p_currentpoint -> p_next;
    p_currentrebro -> p_next = new TRebra;
    p_currentrebro = p_currentrebro -> p_next;
   }
  // now the fourth rebro ...
  p_currentrebro -> p_point1 = p_currentpoint;
  p_currentrebro -> p_point2 = p_pointlist;
  p_currentrebro -> p_next = NULL;
 }

ld TPolygon::minpoint(const int& index, TPoints* p_pointlist)
 {
  ld result = p_pointlist -> point[index];
  TPoints *p_currentpoint = p_pointlist;
  while (p_currentpoint != NULL)
   {
    if (result > p_currentpoint -> point[index])
     result = p_currentpoint -> point[index];
    p_currentpoint = p_currentpoint -> p_next;
   }
  return(result);
 } // End of function minpoint

ld TPolygon::maxpoint(const int& index, TPoints* p_pointlist)
 {
  ld result = p_pointlist -> point[index];
  TPoints *p_currentpoint = p_pointlist;
  while (p_currentpoint != NULL)
   {
    if (result < p_currentpoint -> point[index])
     result = p_currentpoint -> point[index];
    p_currentpoint = p_currentpoint -> p_next;
   }
  return(result);
 } // End of function maxpoint

void TPolygon::draw_object(void)
 {
  userpolygon(p_rebralist, color);
  draw_uvxy();
 }

void TPolygon::translate_object(vector p)
 {
  TPoints* p_currentpoint = p_pointlist;
  while (p_currentpoint)
   {
    p_currentpoint -> point[0]  += p[0];
    p_currentpoint -> point[1]  += p[1];
    p_currentpoint = p_currentpoint -> p_next;
   }
  limitx1 += p[0];
  limity1 += p[1];
  limitx2 += p[0];
  limity2 += p[1];
  centerx += p[0];
  centery += p[1];
 }

void TPolygon::rotate_object(matrix m, const ld& usercenterx, const ld& usercentery, const ld& angle)
 {
  TPoints* p_currentpoint = p_pointlist;
  vector dummy;
  dummy[2] = angle; // for the warning
  while (p_currentpoint)
   {
    dummy[0] = p_currentpoint -> point[0] - usercenterx;
    dummy[1] = p_currentpoint -> point[1] - usercentery;
    dummy[2] = 1; // hommogenious coordinates
    mul_matrix_vector(dummy, m);
    p_currentpoint -> point[0] = dummy[0] + usercenterx;
    p_currentpoint -> point[1] = dummy[1] + usercentery;
    p_currentpoint -> point[2] = 1;
    p_currentpoint = p_currentpoint -> p_next;
   }
  limitx1 = minpoint(0, p_pointlist); // 0 means we compare the X-s
  limity1 = maxpoint(1, p_pointlist); // 1 means we compare the Y-s
  limitx2 = maxpoint(0, p_pointlist);
  limity2 = minpoint(1, p_pointlist);
 }

void TPolygon::dilatate_object(vector p, const Tdirection& direction)
 {
  TPoints* p_currentpoint = p_pointlist;
  switch (direction)
   {
    case east :
     while (p_currentpoint)
      {
       p_currentpoint -> point[0] += (p_currentpoint -> point[0] - limitx1)*p[0]/(limitx2 - limitx1);
       p_currentpoint = p_currentpoint -> p_next;
      }
     limitx2 += p[0];
     break; // break case east
    case west :
     while (p_currentpoint)
      {
       p_currentpoint -> point[0] += (limitx2 - p_currentpoint -> point[0])*p[0]/(limitx2 - limitx1);
       p_currentpoint = p_currentpoint -> p_next;
      }
     limitx1 += p[0];
    break; // break case west
    case north :
     while (p_currentpoint)
      {
       p_currentpoint -> point[1] += (p_currentpoint -> point[1] - limity2)*p[1]/(limity1 - limity2);
       p_currentpoint = p_currentpoint -> p_next;
      }
     limity1 += p[1];
    break; // break case north
    case south :
     while (p_currentpoint)
      {
       p_currentpoint -> point[1] += (limity1 - p_currentpoint -> point[1])*p[1]/(limity1 - limity2);
       p_currentpoint = p_currentpoint -> p_next;
      }
     limity2 += p[1];
   } // End Switch
  centerx = (limitx1 + limitx2)/2;
  centery = (limity1 + limity2)/2;
 }

void TPolygon::fill_object(void)
 { // ScanLineFill algorithm
  Edgepointer ps_TP[769], p_CAP = NULL; // TP = (T)tablica na (P)rebrata
  Edgepointer current, previous;
  int x1, x2, y, ymin, i;
  filled = true;
  int backup = getcolor();
  setcolor(fillcolor);
  setlinestyle(SOLID_LINE, 0, NORM_WIDTH); // 0 means not user defined
  for (i = 0; i < 769; i++) // initialize TP
   ps_TP[i] = NULL;
  CreateTP(ps_TP, p_rebralist, &ymin); y = ymin;
  while ( p_CAP || ps_TP[y] )
   {
    AppendlistsCAP(&p_CAP, ps_TP[y]);
    SortEdges(&p_CAP);
    current = p_CAP;
    while (current)
     {
      x1 = ceil(current -> X);
      current = current -> next;
      x2 = floor(current -> X);
      if (current -> X == x2) x2--;
      if (x1 <= x2)
       line(x1, y, x2, y);
      current = current -> next;
     } // End while current
    y++; current = p_CAP; previous = NULL;
    while (current)
     {
      if (current -> Ymax == y) // exclude
       if (previous)
	{
	 previous -> next = current -> next;
	 delete current;
	 current = previous -> next;
	 continue;
	}
       else
	{
	 p_CAP = current -> next;
	 delete current;
	 current = p_CAP;
	 continue;
	}
      else
       current -> X += current -> IncX;

      previous = current;
      current = current -> next;
     } // End while current
   } // End while (p_CAP || (ps_TP + y))

  for (i = 0; i < 769; i++)
   if (ps_TP[i])
    DeleteList(&ps_TP[i]);
  setcolor(backup);
 } // End of function fill_object

void TPolygon::save_object(FILE* savefile)
 {
  TPoints* p_currentpoint = p_pointlist;
  fseek(savefile, 0L, SEEK_END);
  fprintf(savefile, "POLY ");
  fprintf(savefile, "%u ", color);
  fprintf(savefile, "%u ", fillcolor);
  fprintf(savefile, "%d ", filled);
  while (p_currentpoint)
   {
    fprintf(savefile, "%.17Le ",p_currentpoint -> point[0]);
    fprintf(savefile, "%.17Le ",p_currentpoint -> point[1]);
    p_currentpoint = p_currentpoint -> p_next;
   }
  fseek(savefile, -1L, SEEK_CUR);
  fprintf(savefile, "\n");
 }

kind TPolygon::kind_of_object(void)
 {
  return (POLY);
 }


// member functons of the class TEllipse ...

TEllipse::TEllipse(const int& x1, const int& y1, const kind& type) // first constructor
 {
  char LeftYN, RightYN;
  int mousex, mousey, oldx, oldy;
  int xcenter, ycenter;
  HideMouse();
  color = frcolor;
  fillcolor = 0;
  if (type == CIRCLE)
   {
    int radius;
    GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
    if (mousex - x1 >= 0 && mousey - y1 >= 0) // down right
     {
      radius = max(mousex - x1, mousey - y1)/2;
      xcenter = x1 + radius;
      ycenter = y1 + radius;
     }
    if (mousex - x1 >= 0 && mousey - y1 < 0) // up right
     {
      radius = max(mousex - x1, y1 - mousey)/2;
      xcenter = x1 + radius;
      ycenter = y1 - radius;
     }
    if (mousex - x1 < 0 && mousey - y1 < 0) // up left
     {
      radius = max(x1 - mousex, y1 - mousey)/2;
      xcenter = x1 - radius;
      ycenter = y1 - radius;
     }
    if (mousex - x1 < 0 && mousey - y1 >= 0) // down left
     {
      radius = max(x1 - mousex, mousey - y1)/2;
      xcenter = x1 - radius;
      ycenter = y1 + radius;
     }
    setwritemode(XOR_PUT);
    if (RightYN == 1) MyEllipse(xcenter, ycenter, radius, radius, 0.0l, color);
    SetMouseBound(0, 0, pos - 2, resmaxy);
    oldx = mousex, oldy = mousey;
    while (RightYN == 1)
     {
      GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
      if (oldx != mousex || oldy != mousey)
       {
	MyEllipse(xcenter, ycenter, radius, radius, 0.0l, color); // delete
	if (mousex - x1 >= 0 && mousey - y1 >= 0)
	 {
	  radius = max(mousex - x1, mousey - y1)/2;
	  xcenter = x1 + radius;
	  ycenter = y1 + radius;
	 }
	if (mousex - x1 >= 0 && mousey - y1 < 0)
	 {
	  radius = max(mousex - x1, y1 - mousey)/2;
	  xcenter = x1 + radius;
	  ycenter = y1 - radius;
	 }
	if (mousex - x1 < 0 && mousey - y1 < 0)
	 {
	  radius = max(x1 - mousex, y1 - mousey)/2;
	  xcenter = x1 - radius;
	  ycenter = y1 - radius;
	 }
	if (mousex - x1 < 0 && mousey - y1 >= 0)
	 {
	  radius = max(x1 - mousex, mousey - y1)/2;
	  xcenter = x1 - radius;
	  ycenter = y1 + radius;
	 }
	MyEllipse(xcenter, ycenter, radius, radius, 0.0l, color); // draw
	draw_uvxy();
	oldx = mousex; oldy = mousey;
       } //End if (x2 != mousex || y2 != mousey)
     } // end while (RightYN == 1)

    setwritemode(COPY_PUT);
    MyEllipse(xcenter, ycenter, radius, radius, 0.0l, color);
    devicetouser(center[0], center[1], xcenter, ycenter);
    center[2] = 1; // hommogen coordinates!
    ld zenitx, zenity;
    devicetouser(zenitx, zenity, xcenter + radius, ycenter);
    a = zenitx - center[0];
    b = a;
    angle = 0.0l;
    limitx1 = center[0] - a;
    limity1 = center[1] + b;
    limitx2 = center[0] + a;
    limity2 = center[1] - b;
    centerx = center[0]; // center of rotation
    centery = center[1];
    selected = false;
    marked = false;
    filled = false;
    SetMouseBound(0, 0, resmaxx, resmaxy);
    ShowMouse();
   } // End if CIRCLE
  else // type = ELLIPSE
   if (type == ELLIPSE)
    {
     int radiusa, radiusb;
     GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
     if (mousex - x1 >= 0 && mousey - y1 >= 0) // down right
      {
       radiusa = (mousex - x1)/2;
       radiusb = (mousey - y1)/2;
       xcenter = x1 + radiusa;
       ycenter = y1 + radiusb;
      }
     if (mousex - x1 >= 0 && mousey - y1 < 0) // up right
      {
       radiusa = (mousex - x1)/2;
       radiusb = (y1 - mousey)/2;
       xcenter = x1 + radiusa;
       ycenter = y1 - radiusb;
      }
     if (mousex - x1 < 0 && mousey - y1 < 0) // up left
      {
       radiusa = (x1 - mousex)/2;
       radiusb = (y1 - mousey)/2;
       xcenter = x1 - radiusa;
       ycenter = y1 - radiusb;
      }
     if (mousex - x1 < 0 && mousey - y1 >= 0) // down left
      {
       radiusa = (x1 - mousex)/2;
       radiusb = (mousey - y1)/2;
       xcenter = x1 - radiusa;
       ycenter = y1 + radiusb;
      }
     setwritemode(XOR_PUT);
     if (LeftYN == 1) MyEllipse(xcenter, ycenter, radiusa, radiusb, 0.0l, color);
     SetMouseBound(0, 0, pos - 2, resmaxy);
     oldx = mousex, oldy = mousey;
     while (LeftYN == 1)
      {
       GetMouseButtons(&LeftYN, &RightYN, &mousex, &mousey);
       if (oldx != mousex || oldy != mousey)
	{
	MyEllipse(xcenter, ycenter, radiusa, radiusb, 0.0l, color); // delete
	if (mousex - x1 >= 0 && mousey - y1 >= 0) // down right
	 {
	  radiusa = (mousex - x1)/2;
	  radiusb = (mousey - y1)/2;
	  xcenter = x1 + radiusa;
	  ycenter = y1 + radiusb;
	 }
	if (mousex - x1 >= 0 && mousey - y1 < 0) // up right
	 {
	  radiusa = (mousex - x1)/2;
	  radiusb = (y1 - mousey)/2;
	  xcenter = x1 + radiusa;
	  ycenter = y1 - radiusb;
	 }
	if (mousex - x1 < 0 && mousey - y1 < 0) // up left
	 {
	  radiusa = (x1 - mousex)/2;
	  radiusb = (y1 - mousey)/2;
	  xcenter = x1 - radiusa;
	  ycenter = y1 - radiusb;
	 }
	if (mousex - x1 < 0 && mousey - y1 >= 0) // down left
	 {
	  radiusa = (x1 - mousex)/2;
	  radiusb = (mousey - y1)/2;
	  xcenter = x1 - radiusa;
	  ycenter = y1 + radiusb;
	 }
	MyEllipse(xcenter, ycenter, radiusa, radiusb, 0.0l, color); // draw
	draw_uvxy();
	oldx = mousex; oldy = mousey;
	} //End if (x2 != mousex || y2 != mousey)
      } // end while (RightYN == 1)
     setwritemode(COPY_PUT);
     MyEllipse(xcenter, ycenter, radiusa, radiusb, 0.0l, color);
     devicetouser(center[0], center[1], xcenter, ycenter);
     center[2] = 1; // hommogen coordinates!
     ld zenitx, zenity;
     devicetouser(zenitx, zenity, xcenter + radiusa, ycenter);
     a = zenitx - center[0];
     devicetouser(zenitx, zenity, xcenter, ycenter + radiusb);
     b = center[1] - zenity;
     angle = 0.0l;
     limitx1 = center[0] - a;
     limity1 = center[1] + b;
     limitx2 = center[0] + a;
     limity2 = center[1] - b;
     centerx = center[0]; // center of rotation
     centery = center[1];
     selected = false;
     marked = false;
     filled = false;
     SetMouseBound(0, 0, resmaxx, resmaxy);
     ShowMouse();
    } // End if Ellipse
 } // End of the first constructor

TEllipse::TEllipse(FILE* loadfile)
 {
  int intbuffer; ld ldbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  color = intbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  fillcolor = intbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  filled = (boolean) intbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  limitx1 = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  limity1 = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  limitx2 = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  limity2 = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  center[0] = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  center[1] = ldbuffer;
  center[2] = 1;
  fscanf(loadfile, "%Le", &ldbuffer);
  b = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  angle = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  a = ldbuffer;
  centerx = center[0]; // center of rotation
  centery = center[1];
  selected = false;
  marked = false;
  fgetc(loadfile);
  fgetc(loadfile);
  if (!feof(loadfile)) fseek(loadfile, -1L, SEEK_CUR);
 }

void TEllipse::draw_object(void)
 {
  userellipse(center[0], center[1], a, b, angle, color);
  draw_uvxy();
 }

void TEllipse::translate_object(vector p)
 {
  center[0] += p[0];
  center[1] += p[1];
  center[2] = 1; // homogenious coordinates
  limitx1 += p[0];
  limity1 += p[1];
  limitx2 += p[0];
  limity2 += p[1];
  centerx += p[0];
  centery += p[1];
 }

void TEllipse::rotate_object(matrix m, const ld& usercenterx, const ld& usercentery, const ld& anglepar)
 {
  int x1, y1;
  vector dummy1;
  dummy1[0] = center[0] - usercenterx;
  dummy1[1] = center[1] - usercentery;
  dummy1[2] = 1; // homogenious coordinates
  mul_matrix_vector(dummy1, m);
  center[0] = dummy1[0] + usercenterx;
  center[1] = dummy1[1] + usercentery;
  angle += anglepar;
  if (angle >= M_PI)
   angle -= M_PI;

  ld ax, ay, bx, by;
  int axint, ayint, bxint, byint;
  long int a1, b1; // integer!!!!
  usertodevice(center[0], center[1], x1, y1);
  ax = a*cosl(angle) + center[0];
  ay = a*sinl(angle) + center[1];
  usertodevice(ax, ay, axint, ayint); // the coordinates on the int coor system
  a1 = (long int) floorl( sqrtl( ((ld)axint - x1)*((ld)axint - x1) + ((ld)ayint - y1)*((ld)ayint - y1) ) + 0.5);
  bx = b*cos(angle - M_PI_2) + center[0];
  by = b*sin(angle - M_PI_2) + center[1];
  usertodevice(bx, by, bxint, byint);
  b1 = (long int) floorl( sqrtl( ((ld)byint - y1)*((ld)byint - y1) + ((ld)bxint - x1)*((ld)bxint - x1) ) + 0.5);
  ld A, B, C, D; // middle values coefitients
  A = (ld) a1*a1*cosl(angle)*cosl(angle) + b1*b1*sinl(angle)*sinl(angle);
  B = ( (ld) a1*a1 - b1*b1)*sinl(angle + angle);
  C = (ld) b1*b1*cosl(angle)*cosl(angle) + a1*a1*sinl(angle)*sinl(angle);
  D = -a1*(ld) a1*b1*b1;
  int intlimitx1, intlimitx2, intlimity1, intlimity2;
  intlimitx1 = (int) ceill( -sqrtl( (4*A*D)/(B*B - 4*A*C) ) ) + x1;
  intlimitx2 = (int) floorl( sqrtl( (4*A*D)/(B*B - 4*A*C) ) ) + x1;
  intlimity1 = (int) ceill( -sqrtl( (4*C*D)/(B*B - 4*A*C) ) ) + y1;
  intlimity2 = (int) floorl( sqrtl( (4*C*D)/(B*B - 4*A*C) ) ) + y1;
  devicetouser(limitx1, limity1, intlimitx1, intlimity1);
  devicetouser(limitx2, limity2, intlimitx2, intlimity2);
 }

void TEllipse::dilatate_object(vector p, const Tdirection& direction)
 {
  ld ax, bx, ay, by, axnew, bxnew, aynew, bynew;
  ax = a*cosl(angle);
  bx = b*cosl(angle - M_PI_2);
  ay = a*sinl(angle);
  by = b*sinl(angle - M_PI_2);
  switch (direction)
   {
    case east :
     axnew = ((limitx2 - limitx1 + p[0])*ax)/(limitx2 - limitx1);
     bxnew = ((limitx2 - limitx1 + p[0])*bx)/(limitx2 - limitx1);
     angle = atan2l(ay, axnew);
     a = sqrtl(ay*ay + axnew*axnew);
     b = sqrtl(by*by + bxnew*bxnew);
     center[0] += p[0]/2;
     limitx2 += p[0];
     break;
    case west :
     axnew = ((limitx2 - limitx1 - p[0])*ax)/(limitx2 - limitx1);
     bxnew = ((limitx2 - limitx1 - p[0])*bx)/(limitx2 - limitx1);
     angle = atan2l(ay, axnew);
     a = sqrtl(ay*ay + axnew*axnew);
     b = sqrtl(by*by + bxnew*bxnew);
     center[0] += p[0]/2;
     limitx1 += p[0];
     break;
    case north :
     aynew = ((limity2 - limity1 - p[1])*ay)/(limity2 - limity1);
     bynew = ((limity2 - limity1 - p[1])*by)/(limity2 - limity1);
     angle = atan2l(aynew, ax);
     a = sqrtl(aynew*aynew + ax*ax);
     b = sqrtl(bynew*bynew + bx*bx);
     center[1] += p[1]/2;
     limity1 += p[1];
     break;
    case south :
     aynew = ((limity2 - limity1 + p[1])*ay)/(limity2 - limity1);
     bynew = ((limity2 - limity1 + p[1])*by)/(limity2 - limity1);
     angle = atan2l(aynew, ax);
     a = sqrtl(aynew*aynew + ax*ax);
     b = sqrtl(bynew*bynew + bx*bx);
     center[1] += p[1]/2;
     limity2 += p[1];
     break;
   } // End Switch
  centerx = (limitx1 + limitx2)/2;
  centery = (limity1 + limity2)/2;
 }

void TEllipse::fill_object(void)
 {
  int x1, y1;
  ld ax, ay, bx, by;
  int axint, ayint, bxint, byint;
  long int lengtha, lengthb; // integer!!!!
  int backup = getcolor();
  setcolor(fillcolor);
  setlinestyle(SOLID_LINE, 0, NORM_WIDTH); // 0 means not user defined
  filled = true;
  usertodevice(center[0], center[1], x1, y1);
  ax = a*cosl(angle) + center[0];
  ay = a*sinl(angle) + center[1];
  usertodevice(ax, ay, axint, ayint); // the coordinates on the int coor system
  lengtha = (long int) floorl( sqrtl( ((ld)axint-x1)*((ld)axint-x1) + ((ld)ayint-y1)*((ld)ayint-y1) ) + 0.5);
  bx = b*cosl(angle - M_PI_2) + center[0];
  by = b*sinl(angle - M_PI_2) + center[1];
  usertodevice(bx, by, bxint, byint);
  lengthb = (long int) floorl( sqrtl( ((ld)byint-y1)*((ld)byint-y1) + ((ld)bxint-x1)*((ld)bxint-x1) ) + 0.5);
  long int ellx = (long int) x1, elly = (long int) y1;

  ld A, B, C, D, anglehere; // middle values coefitients
  long int U1, U2,Vup, Vdown; // the roots of the square equation
  long int U;  // counter
  if ( lengtha == 0 || lengthb == 0 ) return;
  if (lengtha == lengthb)
   anglehere = 0.0;
  else
   anglehere = angle;
  A = (ld) lengtha*lengtha*cosl(anglehere)*cosl(anglehere) + lengthb*lengthb*sinl(anglehere)*sinl(anglehere);
  B = ( (ld) lengtha*lengtha - lengthb*lengthb)*sinl(anglehere + anglehere);
  C = (ld) lengthb*lengthb*cosl(anglehere)*cosl(anglehere) + lengtha*lengtha*sinl(anglehere)*sinl(anglehere);
  D = -lengtha*(ld) lengtha*lengthb*lengthb;
  U1 = (long int) ceill( -sqrtl( (4*A*D)/(B*B - 4*A*C) ) );
  U2 = (long int) floorl( sqrtl( (4*A*D)/(B*B - 4*A*C) ) );
  for (U = U1 + 1; U < U2; U++)
   {
    Vup = (long int) floorl( ( -U*B - sqrtl(U*U*B*B - 4*A*(U*U*C + D)) )/(2*A) + 0.5);
    Vdown = (long int) floorl( ( -U*B + sqrtl(U*U*B*B - 4*A*(U*U*C + D) ) )/(2*A) + 0.5);
    line( (int) U + (int) ellx, (int) Vdown + (int) elly - 1, (int) U + (int) ellx, (int) Vup + (int) elly);
   }
  setcolor(backup);
 } // End of function fill_object

void TEllipse::save_object(FILE* savefile)
 {
  fseek(savefile, 0L, SEEK_END);
  fprintf(savefile, "ELLIPSE ");
  fprintf(savefile, "%u ", color);
  fprintf(savefile, "%u ", fillcolor);
  fprintf(savefile, "%d ", filled);
  fprintf(savefile, "%.17Le ", limitx1);
  fprintf(savefile, "%.17Le ", limity1);
  fprintf(savefile, "%.17Le ", limitx2);
  fprintf(savefile, "%.17Le ", limity2);
  fprintf(savefile, "%.17Le ", center[0]);
  fprintf(savefile, "%.17Le ", center[1]);
  fprintf(savefile, "%.17Le ", b); // second radius
  fprintf(savefile, "%.17Le ", angle);
  fprintf(savefile, "%.17Le", a); // first radius
  fprintf(savefile, "\n");
 }

kind TEllipse::kind_of_object(void)
 {
  return (ELLIPSE);
 }


// member functons of the class TList ...

TList::TList()
 {
  int i;
  for (i = 0; i < N + 1; i++)
   ps_Base[i] = NULL;
 }

TList::~TList()
 {
  int end = Findnull();
  for (int i = 0; i < end; i++)
   delete ps_Base[i];
 }

void TList::Putlinein(const int& x, const int& y) // Puts an object - Line in List
 {
  unsigned int i;
  i = Findnull();
  if (i == 100) printf("\a"); // attention !!!!
  p_Line = new TLine(x, y);
  ps_Base[i] = p_Line;
 } // End of function Putlinein

void TList::Putrectanglein(const int& mousex, const int& mousey)
 {
  unsigned int i;
  i = Findnull();
  if (i == 100) printf("\a"); // attention !!!!
  p_Polygon = new TPolygon(mousex, mousey, RECTANGLE);
  ps_Base[i] = p_Polygon;
 }

void TList::Putsquarein(const int& mousex, const int& mousey)
 {
  unsigned int i;
  i = Findnull();
  if (i == 100) printf("\a"); // attention !!!!
  p_Polygon = new TPolygon(mousex, mousey, SQUARE);
  ps_Base[i] = p_Polygon;
 }
void TList::Putpolyin(const int& mousex, const int& mousey)
 {
  unsigned int i;
  i = Findnull();
  if (i == 100) printf("\a"); // attention !!!!
  p_Polygon = new TPolygon(mousex, mousey, POLY);
  ps_Base[i] = p_Polygon;
 }

void TList::Putcirclein(const int& mousex, const int& mousey)
 {
  unsigned int i;
  i = Findnull();
  if (i == 100) printf("\a"); // attention !!!!
  p_Ellipse = new TEllipse(mousex, mousey, CIRCLE);
  ps_Base[i] = p_Ellipse;
 } // End of function Putcirclein

void TList::Putellipsein(const int& mousex, const int& mousey)
 {
  unsigned int i;
  i = Findnull();
  if (i == 100) printf("\a"); // attention !!!!
  p_Ellipse = new TEllipse(mousex, mousey, ELLIPSE);
  ps_Base[i] = p_Ellipse;
 } // End of function Putellipsein

unsigned int TList::Findnull(void)
 {
  int i = 0;
  while (ps_Base[i++] != NULL);
  return(--i);
 }

int TList::Ismousein(const int& mousex, const int& mousey)
 {
  int i;
  for (i = Findnull() - 1; i >= 0; i--)
   if (ps_Base[i] -> check_in(mousex, mousey))
    return(i);
  return(-1);
 }

void TList::Shift(const int& current)
 {
  int i = current;
  while (ps_Base[i + 1] != NULL)
   {
    ps_Base[i] = ps_Base[i + 1];
    i++;
   }
  ps_Base[i] = NULL;
 }

void TList::Draweverything(void)
 {
  int i = 0;
  Block(0, 0, resmaxx, resmaxy, bkcolor);
  while (ps_Base[i+1] != NULL)
   {
    if (ps_Base[i] -> filled)
     {
      setwritemode(COPY_PUT);
      ps_Base[i] -> fill_object();
     }
    setwritemode(COPY_PUT);
    ps_Base[i++] -> draw_object();
   }
 }

void TList::Unselectall(void)
 {
  int i = 0;
  if (ps_Base[0] -> marked)
   {
    while (ps_Base[i] != NULL)
     {
      if (ps_Base[i] -> selected)
       {
	ps_Base[i] -> unselect_object();
	setwritemode(COPY_PUT);
	if (ps_Base[i] -> filled)
	 {
	  setwritemode(COPY_PUT);
	  ps_Base[i] -> fill_object();
	 }
	ps_Base[i] -> draw_object();
	return;
       } // if
      i++;
     } // End While
    printf("\a"); // attention
   } // End if
  return;
 }

void TList::Moveobject(const int& current)
 {
  char left, right;
  int mousex, mousey, oldx, oldy;
  ld usermousex, usermousey, useroldx, useroldy;
  vector p;
  draw_uvxy();
  GetMouseButtons(&left, &right, &mousex, &mousey);
  oldx = mousex; oldy = mousey;
  SetMouseBound(0, 0, pos - 2, resmaxy);
  while (left)
   {
    GetMouseButtons(&left, &right, &mousex, &mousey);
    if (oldx != mousex || oldy != mousey)
     {
      devicetouser(usermousex, usermousey, mousex, mousey);
      devicetouser(useroldx, useroldy, oldx, oldy);
      p[0] = usermousex - useroldx;
      p[1] = usermousey - useroldy;
      p[2] = 1.0;
      setwritemode(XOR_PUT);
      ps_Base[current] -> draw_object(); // deleting the old object
      ps_Base[current] -> translate_object(p);
      ps_Base[current] -> draw_object(); // drawing the new object
      draw_uvxy();
      oldx = mousex; oldy = mousey;
     } // end if
   } // End While
  SetMouseBound(0, 0, resmaxx, resmaxy);
 }


void TList::Rotateobject(const int& current)
 {
  char left, right;
  int mousex, mousey, oldx, oldy;
  ld usermousex, usermousey, useroldx, useroldy, usercenterx, usercentery;
  double angle = 0.0;
  matrix m;
  GetMouseButtons(&left, &right, &mousex, &mousey);
  oldx = mousex; oldy = mousey;
  SetMouseBound(0, 0, pos - 2, resmaxy);
  usercenterx = ps_Base[current] -> centerx;
  usercentery = ps_Base[current] -> centery;
  while (left)
   {
    GetMouseButtons(&left, &right, &mousex, &mousey);
    if (oldx != mousex || oldy != mousey)
     {
      devicetouser(usermousex, usermousey, mousex, mousey);
      devicetouser(useroldx, useroldy, oldx, oldy);
      ld therex = useroldx - usercenterx, therey = useroldy - usercentery;
      // check for old pointer in first cuadrant or third cuadrant ...
      if (therey > 0 && therex <= therey && therex >= -therey
	  || therey < 0 && therex <= -therey && therex >= therey) // we are in 1 or 3!!!!
       angle = -atan( (usermousex - usercenterx)/(usermousey - usercentery) )
		+ atan(therex/therey);
      else // we are not in the 1 or 3 cuadrant, checking for 2 or 4 ...
       if (therex < 0 && therey <= -therex && therey >= therex // we are in 2
	   || therex > 0 && therey <= therex && therey >= -therex) // in 4
	angle = atan( (usermousey - usercentery) / ( usermousex - usercenterx) )
		- atan(therey/therex);
       else /* we are in position therex = 0 && therey = 0 => angle = 0*/
	angle = 0.0;

      m[0][0] = cos(angle);
      m[0][1] = sin(angle);
      m[0][2] = 0;
      m[1][0] = -sin(angle);
      m[1][1] = cos(angle);
      m[1][2] = 0;
      m[2][0] = 0;
      m[2][1] = 0;
      m[2][2] = 1;
      setwritemode(XOR_PUT);
      ps_Base[current] -> draw_object(); // deleting the old object
      ps_Base[current] -> rotate_object(m, usercenterx, usercentery, angle);
      ps_Base[current] -> draw_object(); // drawing the new object
      draw_uvxy();
      oldx = mousex; oldy = mousey;
     } // end if
   } // End While
  SetMouseBound(0, 0,resmaxx, resmaxy);
 }

void TList::Dilatateobject(const int& current, const Tdirection& direction)
 {
  char left, right;
  int mousex, mousey, oldx, oldy;
  ld usermousex, usermousey, useroldx, useroldy;
  vector p;
  draw_uvxy();
  GetMouseButtons(&left, &right, &mousex, &mousey);
  oldx = mousex; oldy = mousey;
  int intlimitx1, intlimity1, intlimitx2, intlimity2;
  usertodevice(ps_Base[current] -> limitx1, ps_Base[current] -> limity1, intlimitx1, intlimity1);
  usertodevice(ps_Base[current] -> limitx2, ps_Base[current] -> limity2, intlimitx2, intlimity2);
  switch (direction)
   {
    case east : SetMouseBound(intlimitx1 + 2, 0, pos - 2, resmaxy); break;
    case west : SetMouseBound(0, 0, intlimitx2 - 2, resmaxy); break;
    case north : SetMouseBound(0, 0, resmaxx, intlimity2 - 2); break;
    case south : SetMouseBound(0, intlimity1 + 2, resmaxx, resmaxy); break;
   }
  while (left)
   {
    GetMouseButtons(&left, &right, &mousex, &mousey);
    if (oldx != mousex || oldy != mousey)
     {
      devicetouser(usermousex, usermousey, mousex, mousey);
      devicetouser(useroldx, useroldy, oldx, oldy);
      p[0] = usermousex - useroldx;
      p[1] = usermousey - useroldy;
      p[2] = 1.0;
      setwritemode(XOR_PUT);
      ps_Base[current] -> draw_object(); // deleting the old object
      ps_Base[current] -> dilatate_object(p, direction);
      ps_Base[current] -> draw_object(); // drawing the new object
      draw_uvxy();
      oldx = mousex; oldy = mousey;
     } // end if
   } // End While
  SetMouseBound(0, 0, resmaxx, resmaxy);
 }

void TList::Fillselected(const int& current)
 {
  ps_Base[current] -> fillcolor = frcolor;
  ps_Base[current] -> unselect_object();
  setwritemode(COPY_PUT);
  ps_Base[current] -> fill_object();
  setwritemode(COPY_PUT);
  ps_Base[current] -> draw_object();
 } // End function Fillselected

int TList::Findselected(void)
 {
  int i = 0;
  while (ps_Base[i] != NULL)
   if (ps_Base[i++] -> selected)
    return(--i);
  return(-1);
 }

void TList::Saveall(FILE* savefile)
 {
  fseek(savefile, 0L, SEEK_SET);
  fprintf(savefile, "COLORS ");
  fprintf(savefile, "%u ", frcolor);
  fprintf(savefile, "%u\n", bkcolor);
  fprintf(savefile, "WINDOW ");
  fprintf(savefile, "%.17Le ", U1);
  fprintf(savefile, "%.17Le ", V1);
  fprintf(savefile, "%.17Le\n", U2);
  int end = Findnull();
  for (int i = 0; i < end; i++)
   ps_Base[i] -> save_object(savefile);
 }

void TList::Loadall(FILE* loadfile)
 {
  int intbuffer; ld ldbuffer; char stringbuffer[20];
  ld u1, v1, u2;
  int end = Findnull();
  for (int i = 0; i < end; i++)
   {
    delete ps_Base[i];
    ps_Base[i] = NULL;
   }
  fseek(loadfile, 0L, SEEK_SET);
  fscanf(loadfile,"%s", stringbuffer); // COLORS
  fscanf(loadfile, "%d", &intbuffer);
  frcolor = (unsigned char) intbuffer;
  fscanf(loadfile, "%d", &intbuffer);
  bkcolor = (unsigned char) intbuffer;
  fscanf(loadfile, "%s", stringbuffer); // WINDOW
  fscanf(loadfile, "%Le", &ldbuffer);
  u1 = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  v1 = ldbuffer;
  fscanf(loadfile, "%Le", &ldbuffer);
  u2 = ldbuffer;
  Window(u1, v1, u2);
  fgetc(loadfile);
  fgetc(loadfile);
  if (!feof(loadfile)) fseek(loadfile, -1L, SEEK_CUR);
  i = 0;
  kind type;
  while (!feof(loadfile))
   {
    type = SQUARE;
    fscanf(loadfile, "%s", stringbuffer);
    if (!_fstrcmp(stringbuffer, "LINE")) type = LINE;
    if (!_fstrcmp(stringbuffer, "POLY")) type = POLY;
    if (!_fstrcmp(stringbuffer, "ELLIPSE")) type = ELLIPSE;
    switch (type)
     {
      case LINE :
       p_Line = new TLine(loadfile);
       ps_Base[i] = p_Line;
       break; // case LINE
      case POLY :
       p_Polygon = new TPolygon(loadfile);
       ps_Base[i] = p_Polygon;
       break; // case POLY
      case ELLIPSE :
       p_Ellipse = new TEllipse(loadfile);
       ps_Base[i] = p_Ellipse;
       break; // case ELLIPSE
      default : printf("\a\a");
     } // End switch
    i++;
   } // End while
 }
