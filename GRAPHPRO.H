#define N 100 // the number of objects the program will support
typedef long double ld;
typedef ld vector[3];
typedef vector matrix[3];
typedef struct P
 {
  vector point;
  struct P *p_next;
 } TPoints;
typedef struct R
 {
  TPoints *p_point1, *p_point2; // we use them only as points not as lists
  struct R *p_next;
 } TRebra;
typedef struct E // only for the ScanLineFill algorithm
 {
  E* next;
  int Ymax;
  double X, IncX;
 } Edge;
typedef Edge* Edgepointer;

enum boolean { false = 0, true = 1 };
enum kind { LINE = 0, SQUARE = 1, RECTANGLE = 2, POLY = 3, CIRCLE = 4, ELLIPSE = 5, ARC = 6 };
enum Tdirection { east = 0, west = 1, north = 2, south = 3 };

void IsMouse(void);
void mouse_event_handler(void);
void InitMouse(int&, int&);
void HideMouse(void);
void ShowMouse(void);
void BLine(int, int, int, int, int);
void GetMouseButtons(char*, char*, int*, int*);
void Window(ld, ld, ld);
void Block (int, int, int, int, unsigned char);
void Block1 (const int&, const int&, const int&, const int&, const unsigned char&);
void Device(int, int, int, int);
void usertodevice(ld, ld, int&, int&);
void devicetouser(ld&, ld&, int, int);
void userline(const ld&, const ld&, const ld&, const ld&, const unsigned char&);
void draw_uvxy(void);
void viewporthandler(void);
void SetMouseBound(int, int, int, int);
int max(const int&, const int&);
int min(const int&, const int&);
ld max(const ld&, const ld&);
ld min(const ld&, const ld&);
void mul_matrix_vector(vector, matrix);
void MyEllipse(const long int&, const long int&, const long int&, const long int&, const ld&, const unsigned char&);
void userellipse(const ld&, const ld&, const ld&, const ld&, const ld&, const unsigned char&);
void EnterRectangle(const int&, const int&, const int&, const int&);
void EnterRectangle1(const int&, const int&, const int&, const int&, const unsigned char&);
void SortEdges(Edgepointer*); // only for the ScanLineFill algorithm
void Appendlists(Edgepointer*, Edgepointer); // only for the ScanLineFill algorithm
void DeleteList(Edgepointer*); // only for the ScanLineFill algorithm
void AppendlistsCAP(Edgepointer*, Edgepointer); // only for the ScanLineFill algorithm
void OpenWindow (const int&, const int&, const int&, const int&, char*);
boolean GetFileName(char*);

class TButton
 {
  private:
   int x1, y1, x2, y2; // coordinates
   char caption[21];
  public:
   static boolean marked; // true if there is a pressed button
   boolean pressed;       // check if the button is pressed or not
   unsigned char number;  // 0 - line, 1 - rectangle, 2 - polygon etc.
   TButton(int, int, int, int, unsigned char, char*); // constructor
   void Press(void);
   void Unpress(void);
   void Draw(void);
 }; // End of class TButton

class TMenu
 {
  private:
   TButton* p_buttons[10];
   int Y_ais[13]; // Y - coordinates of the buttons
  public:
   TMenu(); // constructor
   ~TMenu(); // destructor
   void Draw(void); // draws the menu on the screen
   signed char Getpressedbutton(); // returns -1 if no button is pressed
   void Release(void); // release if we have any pressed buttons
   void Handler(int); // (mousex&) mousey there the button is pressed
   void ShowColors(void);
   void GetColors(void);
   void Zoom(void);
   // it's possible this class to grow here
 };

// 2D geometric objects...
class TBase // abstract class
 {
  friend class TList;
  friend class TMenu;
  protected:
   ld limitx1, limity1; // boundaries of the limit rectangle
   ld limitx2, limity2;
   unsigned char color;
   unsigned char fillcolor;
  public:
   ld centerx, centery; // center of rotation
   boolean selected;
   boolean filled;
   static boolean marked; // if there is a selected object
   void move_center(void);
   void unselect_from_rot(void);
   boolean check_in_center(const int&, const int&);
   boolean check_for_rotation(const int&, const int&); // mouse
   boolean check_for_dilatation(const int&, const int&, Tdirection&);
   void select_for_rot(void);
   void select_object(void); // non virtual
   void unselect_object(void); // non virtual
   boolean check_in(const int&, const int&);
   virtual void draw_object(void) = 0;
   virtual void translate_object(vector) = 0;
   virtual void rotate_object(matrix, const ld&, const ld&, const ld&) = 0;
   virtual void dilatate_object(vector, const Tdirection&) = 0;
   virtual void fill_object(void) = 0;
   virtual void save_object(FILE*) = 0;
   virtual kind kind_of_object(void) = 0;
 }; // End of class Base

class TLine : public TBase // Line
 {
  private:
   vector point1; // typical for the object - Line
   vector point2;
  public:
   TLine(const int&, const int&); // first constructor
   TLine(FILE*); // second constructor (when from file)
   virtual void draw_object(void);
   virtual void translate_object(vector);
   virtual void rotate_object(matrix, const ld&, const ld&, const ld&);
   virtual void dilatate_object(vector, const Tdirection&);
   virtual void fill_object(void);
   virtual void save_object(FILE*);
   virtual kind kind_of_object(void);
 }; // End of class TLine

class TPolygon : public TBase
 {
  private:
   TRebra *p_rebralist;
   TPoints *p_pointlist;
  public:
   TPolygon(const int&, const int&, const kind&); // first constructor
   TPolygon(FILE*); // second constructor (when from file)
   ~TPolygon(); // destructor
   ld minpoint(const int&, TPoints*); // 0 means we compare the X-s
   ld maxpoint(const int&, TPoints*); // 1 means we compare the Y-s
   void MakeRebraList(void);
   virtual void draw_object(void);
   virtual void translate_object(vector);
   virtual void rotate_object(matrix, const ld&, const ld&, const ld&);
   virtual void dilatate_object(vector, const Tdirection&);
   virtual void fill_object(void);
   virtual void save_object(FILE*);
   virtual kind kind_of_object(void);
 }; // End of class TPolygon;

void userpolygon(TRebra*, const unsigned char&);
void CreateTP(Edgepointer*, TRebra*, int*);

class TEllipse : public TBase
 {
  private:
   vector center; // the center
   ld a; // first radius
   ld b; // second radius
   ld angle; // between a and Ox
  public:
   TEllipse(const int&, const int&, const kind&); // first constructor
   TEllipse(FILE*); // second constructor (when from file)
   virtual void draw_object(void);
   virtual void translate_object(vector);
   virtual void rotate_object(matrix, const ld&, const ld&, const ld&);
   virtual void dilatate_object(vector, const Tdirection&);
   virtual void fill_object(void);
   virtual void save_object(FILE*);
   virtual kind kind_of_object(void);
 }; // End of class TEllipse


class TList
 {
  public:
   TBase* ps_Base[N + 1]; // an array of pointers to TBase
   TList(); // constructor
   ~TList(); // destructor
   int Ismousein(const int&, const int&);
   void Putlinein(const int&, const int&);
   void Putcirclein(const int&, const int&);
   void Putellipsein(const int&, const int&);
   void Putrectanglein(const int&, const int&); // mouse coordinates
   void Putpolyin(const int&, const int&);
   void Putsquarein(const int&, const int&);
   void Unselectall(void);
   unsigned int Findnull(void);
   int Findselected(void);
   void Shift(const int&);
   void Draweverything(void);
   void Moveobject(const int&); // number of object (current)
   void Rotateobject(const int&); // number of object (current)
   void Dilatateobject(const int&, const Tdirection&);
   void Fillselected(const int&);
   void Saveall(FILE*);
   void Loadall(FILE*);
 }; // End of class TList
