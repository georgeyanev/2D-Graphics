#2D Graphics

A course project of mine back from the year 2000 ;)

Any further improvements are welcome


C++ sources and 16-bit exe file for 2D vector graphics manipulation.

2D Graphics is a program for drawing and manipulating geometric
figures under DOS. Source code is included.

You can draw, move, resize, rotate, zoom and stretch lines,
ellipses, circles, rectangles and any kind of polygons. You can fill
the ellipses and the polygons, choosing colors of your taste.
The center of rotation of the elements is user defined.
You can save your graphic to a file, which can later be loaded.

The program can be run only on 32bit version of winXP, Vista, 8, 8.1 or newer since the 64bit versions don't have 16 bit subsystem. 

It is intended for educational purposes as there are implemented many drawing algorithms.

##Gide:
Use the "Line" button to draw a line. When the button is pressed down
you can draw as many lines as you want, by pressing the left mouse button and dragging
the line. For manipulation of existing lines
you must release the button by clicking on it again.
This is valid for plygons and ellipses also.
Now the line is available for moving, rotating or stretching.
First you need to select the line by pressing the left mouse button
in the rounding rectangle (in which the line resides).
Eight little squares will appear around  the selected line. You can
move it by clicking on the rectangle defined by these squares and
dragging it anywhere.
For rotation you must click on the upper left little square. Then the line
is rounded by a dashed line and there is a square in the center of the 
rounding rectangle determining the center of rotation. You can drag the center
of rotation anywhere. To rotate the line simply press the left mouse button
on the rounding rectangle and drag it around the center of rotation.
You can delete the line by clicking the right mouse button when is is selected.

When you press the "Rectangle/Sq" button you can draw squares and rectangles.
By using the left mouse button you make a rectangle and with the right
mouse button you make a square.
The same is with the button "Ellipce/Circle" button for ellipses and circles.

With the "Polygon" button you draw poligons line by line dragging them with
the left mouse button. To finish just click the right mouse button.

When an item is selected it can be filled by pressing the "Fill Selected" button.

The button "window" is for zoom in and zoom out. When the button is pressed
the area you surround with the mouse will be zoomed to fill the whole screen.
This is a zoom in procedure. You can zoom out by simply pressing the right
mouse button.

With "Set colors" button you choose foreground and background colors.
"Save" and "Load" are respectively for saving and loading drawings to and from
disk files.

The project is built with Borland C++ 3.1. It uses SVGA256.BGI driver
for the grapic mode and handles mouse through INT 33
