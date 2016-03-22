#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include "graphpro.h"

extern ld U1, V1, U2, V2;   // user window
extern int X1, Y1, X2, Y2;   // monitor display
extern int yy1, yy2, yy3;
extern int resmaxx, resmaxy, pos, userfont;
extern TMenu* p_Menu;
extern TList List;
void BLine(int x1, int y1, int x2, int y2, int value)
 { // Bresenham line
  int x, y, dx, dy, incX, incY;
  int d, incUP, incDAWN, reverse, n;
  dx = abs(x2 - x1);
  dy = abs(y2 - y1);
  if ( (reverse = (dx < dy)) != 0 )
   {
    int buffer;
    buffer = x1; x1 = y1; y1 = buffer;
    buffer = x2; x2 = y2; y2 = buffer;
    buffer = dx; dx = dy; dy = buffer;
   }
  incUP = -dx - dx + dy + dy; //if choosen S
  incDAWN = dy + dy;
  incX = (x1 <= x2) ? 1 : -1;
  incY = (y1 <= y2) ? 1 : -1;
  d = -dx + dy + dy;
  x = x1; y = y1; n = dx; //+ 1;
  while (n--)
   {
    if (reverse)
     putpixel(y, x, value);
    else
     putpixel(x, y, value);
    x += incX;
    if (d > 0)
     { d += incUP; y += incY; }
    else
     d += incDAWN;
   } // end while
 } // End of function BLine

void MyEllipse(const long int& X1, const long int& Y1, const long int& a, const long int& b, const ld& angle, const unsigned char& color)
 {
  ld A, B, C, D, anglehere; // middle values coefitients
  long int U1, U2,V, Vold, Vstart; // the roots of the square equivalence
  long int U;  // counter
  if ( a == 0 || b == 0 ) return;
  if (a == b)
   anglehere = 0.0;
  else
   anglehere = angle;
  A = (ld) a*a*cosl(anglehere)*cosl(anglehere) + b*b*sinl(anglehere)*sinl(anglehere);
  B = ( (ld) a*a - b*b)*sinl(anglehere + anglehere);
  C = (ld) b*b*cosl(anglehere)*cosl(anglehere) + a*a*sinl(anglehere)*sinl(anglehere);
  D = -a*(ld) a*b*b;
  U1 = (long int) ceill( -sqrtl( (4*A*D)/(B*B - 4*A*C) ) );
  U2 = (long int) floorl( sqrtl( (4*A*D)/(B*B - 4*A*C) ) );
  Vstart = (long int) floorl( ( -U1*B - sqrtl(U1*U1*B*B - 4*A*(U1*U1*C + D)) )/(2*A) + 0.5);
  Vold = Vstart;
  // first above (with the sign '-')
  for (U = U1 + 1; U <= U2; U++)
   {
    V = (long int) floorl( ( -U*B - sqrtl(U*U*B*B - 4*A*(U*U*C + D)) )/(2*A) + 0.5);
    BLine( (int) U - 1 + (int) X1, (int) Vold + (int) Y1, (int) U + (int) X1, (int) V + (int) Y1, color);
    Vold = V;
   }
  // and now below (with the sign '+')
  for (U = U2 - 1; U >= U1; U--)
   {
    V = (long int) floorl( ( -U*B + sqrtl(U*U*B*B - 4*A*(U*U*C + D) ) )/(2*A) + 0.5);
    BLine( (int) U + 1 + (int) X1, (int) Vold + (int) Y1, (int) U + (int) X1, (int) V + (int) Y1, color);
    Vold = V;
   }
  if (Vold != Vstart)
   BLine( (int) U1 + (int) X1, (int) Vold + (int) Y1 - 1, (int) U1 + (int) X1, (int) Vstart + (int) Y1, color);
 } // end of function MyEllipse

void EnterRectangle(const int& x1, const int& y1, const int& x2, const int& y2)
 {
  setlinestyle(DASHED_LINE, 0, NORM_WIDTH); // 0 means not user defined
  setcolor(15);
  line(x1, y1, x2, y1);
  line(x2, y1, x2, y2);
  line(x2, y2, x1, y2);
  line(x1, y2, x1, y1);
 }

void EnterRectangle1(const int& x1, const int& y1, const int& x2, const int& y2, const unsigned char& color)
 {
  BLine(x1, y1, x2, y1, color);
  BLine(x2, y1, x2, y2, color);
  BLine(x2, y2, x1, y2, color);
  BLine(x1, y2, x1, y1, color);
 }

void userline(const ld& x1, const ld& y1, const ld& x2, const ld& y2, const unsigned char& color)
 {
  int xi1, yi1, xi2, yi2;
  usertodevice(x1, y1, xi1, yi1);
  usertodevice(x2, y2, xi2, yi2);
  BLine(xi1, yi1, xi2, yi2, color);
 }

void userellipse(const ld& centerx, const ld& centery, const ld& a, const ld& b, const ld& angle, const unsigned char& color)
 {
  int x1, y1;
  ld ax, ay, bx, by;
  int axint, ayint, bxint, byint;
  long int lengtha, lengthb; // integer!!!!
  usertodevice(centerx, centery, x1, y1);
  ax = a*cosl(angle) + centerx;
  ay = a*sinl(angle) + centery;
  usertodevice(ax, ay, axint, ayint); // the coordinates on the int coor system
  lengtha = (long int) floorl(sqrtl( ((ld)axint - x1)*((ld)axint - x1) + ((ld)ayint - y1)*((ld)ayint - y1) ) + 0.5);
  bx = b*cosl(angle - M_PI_2) + centerx;
  by = b*sinl(angle - M_PI_2) + centery;
  usertodevice(bx, by, bxint, byint);
  lengthb = (long int) floorl( sqrtl( ((ld)byint - y1)*((ld)byint - y1) + ((ld)bxint - x1)*((ld)bxint - x1) ) + 0.5);
  long int ellx = (long int) x1, elly = (long int) y1;
  MyEllipse( ellx, elly, lengtha, lengthb, angle, color);
 }

void userpolygon(TRebra* p_rebralist, const unsigned char& color)
 {
  TRebra* p_currentrebro;
  p_currentrebro = p_rebralist;
  while (p_currentrebro)
   {
    userline(p_currentrebro -> p_point1 -> point[0],
	     p_currentrebro -> p_point1 -> point[1],
	     p_currentrebro -> p_point2 -> point[0],
	     p_currentrebro -> p_point2 -> point[1],
	     color);
    p_currentrebro = p_currentrebro -> p_next;
   }
 }

void Window(ld x1, ld y1, ld x2)
 { // we do not have C1 & C2 and such stuff ...
  U1 = x1; V1 = y1; U2 = x2;
  V2 = V1 - (U2 - U1)*(resmaxy + 1)/(pos - 1); // automatic calculation
 }

void Device(int x1, int y1, int x2, int y2)
 {
  X1 = x1; Y1 = y1; X2 = x2; Y2 = y2;
 }

void usertodevice(ld u, ld v, int& x, int& y)
 {
  x = X1 + (int) floorl( (u - U1)*(X2 - X1)/(U2 - U1) + 0.5 ); // rounding
  y = Y1 + (int) floorl( (v - V1)*(Y2 - Y1)/(V2 - V1) + 0.5 ); // rounding
 }

void devicetouser(ld& u, ld& v, int x, int y)
 {
  u = U1 + (U2 - U1)*(x - X1)/(X2 - X1);
  v = V1 + (V2 - V1)*(y - Y1)/(Y2 - Y1);
 }

void draw_uvxy(void) // shows current user and viewport(mouse) coordinates ...
 {
  int mousex, mousey, x, y;
  char f1, f2; // fictive
  char buf[17];
  unsigned char color;
  ld u, v;
  settextstyle(userfont, HORIZ_DIR, (int) floor( 4*(resmaxx+1)/640) + 0.5); // third - the size of the font
  setfillstyle(SOLID_FILL, 15); // 15 - WHITE
  // now print the coordinates x y u and v
  setwritemode(COPY_PUT);
  GetMouseButtons(&f1, &f2, &mousex, &mousey);
  devicetouser(u, v, mousex, mousey);
//  usertodevice(u, v, x, y);
  x = mousex;  y = mousey;
  settextjustify(LEFT_TEXT, CENTER_TEXT);
  color = getcolor();
  setcolor(0);

  bar(pos + 2 + textwidth("u="), yy1 - textheight("_")/3, pos + 2 + textwidth("u=") + textwidth("00000000000000000"), yy1 - textheight("_")/3 + textheight("0000000000"));
  moveto(pos + 2 + textwidth("u=") + 1, yy1);
  sprintf(buf, "%-10Lf", u);
  outtext(buf);   // user coordinates

  bar(pos + 2 + textwidth("v="), yy2 - textheight("_")/3, pos + 2 + textwidth("v=") + textwidth("00000000000000000"), yy2 - textheight("_")/3 + textheight("0000000000"));
  moveto(pos + 2 + textwidth("v=") + 1, yy2);
  sprintf(buf, "%-10Lf", v);
  outtext(buf);   // user coordinates

  bar(pos + 2 + textwidth("x="), yy3 - textheight("_")/3, pos + 2 + textwidth("x=") + textwidth("0000"), yy3 - textheight("_")/3 + textheight("0000"));
  moveto(pos + 2 + textwidth("x=") + 1, yy3);
  sprintf(buf, "%d", x);
  outtext(buf);   // mouse coordinates

  bar((resmaxx + pos)/2 - 1 + textwidth("y="), yy3 - textheight("_")/3, (resmaxx + pos)/2 - 1 + textwidth("y=") + textwidth("0000"), yy3 - textheight("_")/3 + textheight("0000"));
  moveto((resmaxx + pos)/2 -1 + textwidth("y=") + 1, yy3);
  sprintf(buf, "%d", y);
  outtext(buf);   // mouse coordinates
  setwritemode(XOR_PUT);
  setcolor(color);
 }

int max(const int& x, const int& y)
 {
  return ( (x > y) ? x : y);
 }

int min(const int& x, const int& y)
 {
  return ( (x < y) ? x : y);
 }

ld max(const ld& x, const ld& y)
 {
  return ( (x > y) ? x : y);
 }

ld min(const ld& x, const ld& y)
 {
  return ( (x < y) ? x : y);
 }

void mul_matrix_vector(vector p, matrix m)
 {
  vector dummy = { 0.0, 0.0, 0.0 };
  int i, j;
  for (i = 0; i < 3; i++)
   for (j = 0; j < 3; j++)
    dummy[i] += p[j]*m[j][i];
  p[0] = dummy[0];
  p[1] = dummy[1];
  p[2] = dummy[2];
 }

void CreateTP(Edgepointer *TP, TRebra *p_rebralist, int *p_ymin)
 {
  int pointx1, pointy1, pointx2, pointy2;
  TRebra* p_currentrebro = p_rebralist;
  Edgepointer p_Edge;
  while (p_currentrebro)
   {
    usertodevice(p_currentrebro -> p_point1 -> point[0],
		 p_currentrebro -> p_point1 -> point[1],
		 pointx1,
		 pointy1);
    usertodevice(p_currentrebro -> p_point2 -> point[0],
		 p_currentrebro -> p_point2 -> point[1],
		 pointx2,
		 pointy2);
    if (pointy1 == pointy2)
     {
      p_currentrebro = p_currentrebro -> p_next;
      continue;
     }
    if (min(pointy1, pointy2) < 0)
     if (max(pointy1, pointy2) <= 0)
      {
       p_currentrebro = p_currentrebro -> p_next;
       continue;
      }
     else
      if (pointy1 < pointy2)
       {
	int bufferx1, buffery1;
	bufferx1 = (int) (((double) pointx1*pointy2 - (double) pointy1*pointx2)/(pointy2 - pointy1));
	buffery1 = 0;
	pointx1 = bufferx1;
	pointy1 = buffery1;
       }
      else // pointy2 < pointy1
       {
	int bufferx2, buffery2;
	bufferx2 = (int) (((double) pointx1*pointy2 - (double) pointy1*pointx2)/(pointy2 - pointy1));
	buffery2 = 0;
	pointx2 = bufferx2;
	pointy2 = buffery2;
       }

    if (max(pointy1, pointy2) > 767)
     if (min(pointy1, pointy2) >= 767)
      {
       p_currentrebro = p_currentrebro -> p_next;
       continue;
      }
     else
      if (pointy1 < pointy2)
       {
	int bufferx2, buffery2;
	bufferx2 = (int) (( (double)767*(pointx1 - pointx2) - (double)pointx1*pointy2 + (double)pointy1*pointx2)/(pointy1 - pointy2));
	buffery2 = 767;
	pointx2 = bufferx2;
	pointy2 = buffery2;
       }
      else // pointy2 < pointy1
       {
	int bufferx1, buffery1;
	bufferx1 = (int) (( (double)767*(pointx1 - pointx2) - (double)pointx1*pointy2 + (double)pointy1*pointx2)/(pointy1 - pointy2));
	buffery1 = 767;
	pointx1 = bufferx1;
	pointy1 = buffery1;
       }
    p_Edge = new Edge;
    if (!p_Edge) printf("\a"); // attention  !
    p_Edge -> next = NULL;
    p_Edge -> Ymax = max(pointy1, pointy2);
    if (pointy1 < pointy2)
     {
      p_Edge -> X = (double) pointx1;
      p_Edge -> IncX = (pointx2 - pointx1)/(double)(pointy2 - pointy1);
      Appendlists(&TP[pointy1], p_Edge);
     }
    else // pointy1 > pointy2
     {
      p_Edge -> X = (double) pointx2;
      p_Edge -> IncX = (pointx1 - pointx2)/(double)(pointy1 - pointy2);
      Appendlists(&TP[pointy2], p_Edge);
     }
    p_currentrebro = p_currentrebro -> p_next;
   } // End while (p_currentrebro)
  int i = 0;
  while ((!TP[i++]) && i < 769);
  *p_ymin = i - 1;
 } // End of function CreateTP

void Appendlists(Edgepointer* pp_first, Edgepointer p_second)
 {
  while (*pp_first)
   pp_first = &( (*pp_first) -> next );
  *pp_first = p_second;
 }

void AppendlistsCAP(Edgepointer* pp_CAP, Edgepointer p_second)
 {
  Edgepointer p_Edge, p_current = p_second;
  if (p_second == NULL) return;

  while (p_current)
   {
    p_Edge = new Edge;
    p_Edge -> next = NULL;
    p_Edge -> Ymax = p_current -> Ymax;
    p_Edge -> X = p_current -> X;
    p_Edge -> IncX = p_current -> IncX;
    Appendlists(pp_CAP, p_Edge);
    p_current = p_current -> next;
   }
 } // End of function AppendlistsCAP

void DeleteList(Edgepointer* pp_Edgelist)
 {
  if (*pp_Edgelist == NULL)
   return;
  Edgepointer p_last = *pp_Edgelist;
  while (1)
   {
    p_last = *pp_Edgelist;
    if (p_last -> next == NULL)
     {
      delete p_last;
      *pp_Edgelist = NULL;
      return;
     }
    while (p_last -> next -> next)
     p_last = p_last -> next;

    delete p_last -> next;
    p_last -> next = NULL;
   }
 }



void SortEdges(Edgepointer* pp_Edge) // only for the ScanLineFill algorithm
 {
  Edgepointer helper1, helper2, *pp_first;
  boolean b;
  do
   {
    b = false;
    pp_first = pp_Edge;
    while ( (*pp_first) -> next )
     {
      if ( (*pp_first) -> X > (*pp_first) -> next -> X )
       {
	b = true;
	helper1 = (*pp_first) -> next;
	helper2 = *pp_first;
	helper2 -> next = helper1 -> next;
	helper1 -> next = helper2;
	*pp_first = helper1;
       } // End if
      pp_first = &( (*pp_first) -> next );
     } // End while ...
   } // End do ... while
  while (b);
 } // End of function SortEdges


void OpenWindow(const int& A1, const int& B1, const int& A2, const int& B2, char* Name)
 {
  setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
  setcolor(0);
  rectangle (A1, B1, A2, B2);
  Block(A1+ 1, B1+ 1, A2- 1, B2- 1, 15);
  setcolor(1);
  rectangle(A1+ 1, B1+ 1, A2- 1, B2- 1);
  rectangle(A1+ 2, B1+ 2, A2- 2, B2- 2);
  Block(A1+ 4, B1+ 4, A2- 4, B1+ 20, 1);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  setcolor(15);
  moveto((A1 + A2)/2, B1 + 12);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtext(Name);
 }

boolean GetFileName(char* S)  // not mine function
 {
  unsigned char M, Ch;
  setcolor(0);
  moveto(200, 210);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtext("File name :");
  S[0] = '\0';
  M = 0;
Ev:
  Ch = getch();
  if ( Ch >= 'A' && Ch <= 'Z' ||  Ch >= '0' && Ch <= '9' || Ch >= 'a' && Ch <= 'z')
   {
    if (M == 8) goto Ev;
    char buf[2];
    S[M++] = Ch;
    S[M] = '\0';
    buf[0] = Ch;
    buf[1] = '\0';
    setcolor(0);
    moveto(252 + (M-1)*8, 210);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtext(buf);
    goto Ev;
   }
  if (Ch == 13)
   {
    if (S[0] == '\0') return(false);
    return(true);
   }
  goto Ev;
 }

void viewporthandler(void) // probably the most important function
 {
  int mousex, mousey;
  char leftmb, rightmb; // left mouse button, right mouse button

  while (1)
   {
    GetMouseButtons(&leftmb, &rightmb, &mousex, &mousey);

    if (leftmb && mousex > pos)
     {
      p_Menu -> Handler(mousey);
      continue;
     } // end of the part working with the menu
    /* tuka sega zavisi ako niamame menu imame select, vtori select etc. */
    if (leftmb && mousex < pos - 1)
     {
      int current;
      switch (p_Menu -> Getpressedbutton())
       {
	case 0 : List.Putlinein(mousex, mousey); break; // insert into the List an object - Line
	case 1 : List.Putrectanglein(mousex, mousey); break;
	case 2 : List.Putpolyin(mousex, mousey); break;
	case 3 : List.Putellipsein(mousex, mousey);break;
	default : printf("\a"); break; // attenton! bug
	// if any select, move, dilatate or select for rotation and then again
	case -1 : if ((current = List.Ismousein(mousex, mousey)) != -1)
		   {
		    if (List.ps_Base[current] -> selected)
		     {
		      Tdirection direction;
		      if (List.ps_Base[current] -> check_for_dilatation(mousex, mousey, direction))
		       { // dilatate
			HideMouse();
			List.ps_Base[current] -> unselect_object();
			List.Dilatateobject(current, direction);
			List.ps_Base[current] -> select_object();
			ShowMouse();
		       }

		      if (List.ps_Base[current] -> check_for_rotation(mousex, mousey))
		       { // rotate
			HideMouse();
			List.ps_Base[current] -> unselect_object();
			List.ps_Base[current] -> select_for_rot();
			ShowMouse();
			while (leftmb) // wait for release after selection
			 GetMouseButtons(&leftmb, &rightmb, &mousex, &mousey);

			do // the object is selected for rotation, let's rotate it...
			 { int mx, my;
			  GetMouseButtons(&leftmb, &rightmb, &mousex, &mousey);
			  // if the user wants let's move the center of rotation :-)
			  mx = mousex; my = mousey;
			  if (leftmb && List.ps_Base[current] -> check_in_center(mx, my))
			   {
			    HideMouse();
			    List.ps_Base[current] -> move_center();
			    ShowMouse();
			    GetMouseButtons(&leftmb, &rightmb, &mousex, &mousey);
			    while (leftmb == 0)
			     GetMouseButtons(&leftmb, &rightmb, &mousex, &mousey);
			   } // End moving the center, continue rotating ...

			  if (leftmb && List.ps_Base[current] -> check_in(mousex, mousey))
			   {
			    int step;
			    step = floor(2.0*(resmaxx + 1)/640 + 0.5);

			    HideMouse();
			    List.ps_Base[current] -> unselect_from_rot();
			    int intcenterx, intcentery;
			    usertodevice(List.ps_Base[current] -> centerx, List.ps_Base[current] -> centery, intcenterx, intcentery);
			    Block1(intcenterx - step, intcentery - step, intcenterx + step, intcentery + step, 15);
			    List.Rotateobject(current);
			    setwritemode(XOR_PUT);
			    Block1(intcenterx - step, intcentery - step, intcenterx + step, intcentery + step, 15);
			    List.ps_Base[current] -> select_for_rot();
			    ShowMouse();
			   }
			  else // unselecting from rotation
			   if (leftmb)
			    {
			     HideMouse();
			     List.ps_Base[current] -> unselect_from_rot();
			     if (List.ps_Base[current] -> filled)
			      {
			       setwritemode(COPY_PUT);
			       List.ps_Base[current] -> fill_object();
			      }
			     setwritemode(COPY_PUT);
			     List.ps_Base[current] -> draw_object();
			     p_Menu -> Draw();
			     ShowMouse();
			     break; // breaking the cycle While (1)
			    }
			 } // this is for the do - while
			while (1); // do - while
			// Have to remove it after finishing
			break; // breaking from the switch operator
		       } // End if the object is to be rotated
		      else
		       {
	       //	MOVING OBJECT...
			HideMouse();
			List.ps_Base[current] -> unselect_object();
			List.Moveobject(current);
			List.ps_Base[current] -> select_object();
		       // p_Menu -> Draw();
			ShowMouse();
		       }
		     } // End if the object is selected for once
		    else // the object is not selected let's select it ...
		     {
		      HideMouse();
		      List.Unselectall();
		      int temp;
		      temp = List.Findnull();
		      List.ps_Base[temp] = List.ps_Base[current];
		      List.Shift(current);
		      current = --temp;
		      clearviewport();
		      ShowMouse(); // always comes after clearviewport&cleardevice!
		      HideMouse();
		      List.Draweverything();
		      p_Menu -> Draw();
		      setwritemode(XOR_PUT);
		      List.ps_Base[current] -> draw_object();
		      List.ps_Base[current] -> select_object();
		      ShowMouse();
		     } // end if the object is not selected
		   } // End if we have a mouse in
		  else // we do not have a mouse in an object unselecting...
		   {
		    HideMouse();
		    List.Unselectall();
		    p_Menu -> Draw();
		    ShowMouse();
		   }
		  break; // we search in the list if the mouse pointer is on an object's rectangle
       } //End switch
      while (leftmb)
       GetMouseButtons(&leftmb, &rightmb, &mousex, &mousey);
      continue;
     } // End if (leftmb && mousex < pos - 1)

     //RIGHT BUTTON

    if (rightmb && mousex < pos - 1)
     {
      int current;
      switch (p_Menu -> Getpressedbutton())
       {
	case 0 : break; // insert into the List an object - Arc (this is not included in this version :-)
	case 1 : List.Putsquarein(mousex, mousey); break;
	case 2 : break;
	case 3 : List.Putcirclein(mousex, mousey); break;
	default : printf("\a"); break; // attention! bug
	case -1 : if ((current = List.Ismousein(mousex, mousey)) != -1)
		   if (List.ps_Base[current] -> selected)
		    {
		     List.ps_Base[current] -> unselect_object();
		     setwritemode(XOR_PUT);
		     List.ps_Base[current] -> draw_object(); // we delete the last object
		     delete List.ps_Base[current];
		     List.ps_Base[current] = NULL;
		    }
	break;
       }
      while (rightmb)
       GetMouseButtons(&leftmb, &rightmb, &mousex, &mousey);
      continue;
     }

   } // End while(1)
 } // End of function viewporthandler()