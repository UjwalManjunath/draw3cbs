#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#define TRUE 1
#define FALSE 0

void startCallback ( Widget w, XtPointer clientData, XtPointer callData );
void quitCallback ( Widget w, XtPointer clientData, XtPointer callData );
void yesCallback ( Widget w, XtPointer clientData, XtPointer callData );
void noCallback ( Widget w, XtPointer clientData, XtPointer callData );
void redCallback ( Widget w, XtPointer clientData, XtPointer callData );
void greenCallback ( Widget w, XtPointer clientData, XtPointer callData );
void blueCallback ( Widget w, XtPointer clientData, XtPointer callData );
void brownCallback ( Widget w, XtPointer clientData, XtPointer callData );
void orangeCallback ( Widget w, XtPointer clientData, XtPointer callData );
void HandleBoardEvents ( Widget w, XtPointer clientData, XEvent *event, Boolean *flag );
void clearCallback (Widget w, XtPointer clientData, XtPointer callData);

Widget start,quit,yes,no,red,blue,green,brown,orange,clear,start;
Colormap cmap;
XColor Rcolor,Gcolor,Bcolor,Brcolor,Ocolor,ignore;
char *redcolor = "red";
char *greencolor = "green";
char *bluecolor = "blue";
char *browncolor = "brown";
char *orangecolor = "orange";

Arg wargs[1];
Display *display;
int screen;

XGCValues gcval;
GC draw;
Window window;
int FirstPt = TRUE,Clearflag=TRUE;
int  pointx1, pointy1,pointx2,pointy2,midx1,midx2,midy1,midy2;
int radius = 3;
float Dist,midpointx,midpointy,d1=100;
Widget shell, canvas, panel, commands, options;

void main ( int argc, char **argv )
{
	XtAppContext app;

	if ( putenv("XENVIRONMENT=Draw3cbs.color") < 0)
		printf("can't set XENVIRONMENT\n");

	shell = XtAppInitialize ( &app, "Draw3cbs", NULL, 0,&argc, argv, NULL, NULL, 0 );
	/*
	* The overall window layout is handled by an XmForm widget.
	*/
	panel = XtCreateManagedWidget ( "panel", xmFormWidgetClass, shell, NULL, 0 );
	/*
	* An XmRowColumn widget holds the buttons along the top
	* of the window.
	*/

	commands =  XtVaCreateManagedWidget ( "commands", xmRowColumnWidgetClass, panel,
	    					XmNnumColumns, 3,
	    					XmNorientation, XmHORIZONTAL,
	    					XmNtopAttachment, XmATTACH_FORM,
	    					XmNrightAttachment, XmATTACH_FORM,
							XmNleftAttachment, XmATTACH_FORM,
	    					XmNbottomAttachment, XmATTACH_NONE,
	    					NULL 
					    );
	/*
	* Another XmRowColumn widget contains a column of buttons
	* along the left side of the window.
	*/

	options =  XtVaCreateManagedWidget ( "options", xmRowColumnWidgetClass,panel,
	    					XmNnumColumns, 1,
	    					XmNorientation, XmVERTICAL,
	    					XmNtopAttachment, XmATTACH_WIDGET,
	    					XmNtopWidget, commands,
	    					XmNrightAttachment, XmATTACH_FORM,
	    					XmNleftAttachment, XmATTACH_NONE,
	    					XmNbottomAttachment,XmATTACH_FORM,
	    					NULL
					   );
	/*
	* The middle window, in which the application can display
	* text or graphics is an XmDrawingArea widget.
	*/

	canvas =  XtVaCreateManagedWidget ( "canvas", xmDrawingAreaWidgetClass, panel,
	    					XmNtopAttachment, XmATTACH_WIDGET,
	    					XmNtopWidget, commands,
	    					XmNleftAttachment, XmATTACH_FORM,
	    					XmNrightWidget, options,
	    					XmNrightAttachment, XmATTACH_WIDGET,
	    					XmNbottomAttachment,XmATTACH_FORM,
	    					XmNwidth,400,
	    					XmNheight,400,NULL
					  );

	/*
	* The buttons in the commands and options panels are
	* created as XmPushButton widgets.
	*/
	start = XtCreateManagedWidget ( "Start", xmPushButtonWidgetClass,commands, NULL, 0 );
	quit = XtCreateManagedWidget ( "Quit", xmPushButtonWidgetClass,commands, NULL, 0 );
	no = XtCreateManagedWidget ( "no", xmPushButtonWidgetClass,commands, NULL, 0 );
	yes = XtCreateManagedWidget ( "yes", xmPushButtonWidgetClass,commands, NULL, 0 );
	clear = XtVaCreateManagedWidget ("Clear", xmPushButtonWidgetClass, commands, NULL, 0);
	red =XtCreateManagedWidget ( "   ", xmPushButtonWidgetClass, options, NULL, 0 );
	green=XtCreateManagedWidget ( "   ", xmPushButtonWidgetClass,options, NULL, 0 );
	blue=XtCreateManagedWidget ( "   ", xmPushButtonWidgetClass,options, NULL, 0 );
	brown=XtCreateManagedWidget ( "   ", xmPushButtonWidgetClass,options, NULL, 0 );
	orange=XtCreateManagedWidget ( "   ", xmPushButtonWidgetClass,options, NULL, 0 );
	
	/* change the label and background color of a pushbutton */
	XtSetArg(wargs[0], XmNlabelString,XmStringCreateLocalized("      START     "));
	XtSetValues(start, wargs, 1);
	XtSetArg(wargs[0], XmNlabelString,XmStringCreateLocalized("      QUIT     "));
	XtSetValues(quit, wargs, 1);
	XtSetArg(wargs[0], XmNlabelString,XmStringCreateLocalized("      CLEAR     "));
	XtSetValues(clear, wargs, 1);
	XtAddCallback ( start, XmNactivateCallback,startCallback, NULL );
	XtAddCallback ( quit, XmNactivateCallback,quitCallback, NULL );
	XtAddCallback ( yes, XmNactivateCallback,yesCallback, NULL );
	XtAddCallback ( no, XmNactivateCallback,noCallback, NULL );
	XtAddCallback (clear, XmNactivateCallback, clearCallback, NULL );
	XtAddCallback ( red, XmNactivateCallback, redCallback, NULL );
	XtAddCallback ( green, XmNactivateCallback, greenCallback, NULL );
	XtAddCallback ( blue, XmNactivateCallback, blueCallback, NULL );
	XtAddCallback ( brown, XmNactivateCallback, brownCallback, NULL );
	XtAddCallback ( orange, XmNactivateCallback, orangeCallback, NULL );

	display= XtDisplay(shell);
	screen = DefaultScreen(display);
	cmap = DefaultColormap (display, screen);

	XAllocNamedColor(display, cmap, redcolor, &Rcolor, &ignore);
	XtSetArg(wargs[0], XmNbackground, Rcolor.pixel);
	XtSetValues(red, wargs, 1);

	XAllocNamedColor(display, cmap, greencolor, &Gcolor, &ignore);
	XtSetArg(wargs[0], XmNbackground, Gcolor.pixel);
	XtSetValues(green, wargs, 1);

	XAllocNamedColor(display, cmap, bluecolor, &Bcolor, &ignore);
	XtSetArg(wargs[0], XmNbackground, Bcolor.pixel);
	XtSetValues(blue, wargs, 1);

	XAllocNamedColor(display, cmap, browncolor, &Brcolor, &ignore);
	XtSetArg(wargs[0], XmNbackground, Brcolor.pixel);
	XtSetValues(brown, wargs, 1);
	
	XAllocNamedColor(display, cmap, orangecolor, &Ocolor, &ignore);
	XtSetArg(wargs[0], XmNbackground, Ocolor.pixel);
	XtSetValues(orange, wargs, 1);
	
	/* draw lines on the canvas */

	XtVaGetValues ( canvas,XmNforeground, &gcval.foreground,XmNbackground, &gcval.background,NULL );
	draw = XtGetGC ( canvas, GCForeground | GCBackground , &gcval );
	XtAddEventHandler(canvas,ButtonPressMask,FALSE,HandleBoardEvents, NULL);
	
	XtRealizeWidget ( shell );
	XtUnmapWidget(no);
	XtUnmapWidget(yes);
	XtUnmapWidget(quit);
	XtUnmapWidget(clear);
	XtUnmapWidget(options);
	XtVaSetValues( canvas, XmNsensitive, False, NULL);
	XtAppMainLoop ( app );
}

void startCallback ( Widget w, XtPointer clientData, XtPointer callData )
{
	XtUnmapWidget(start);
	XtMapWidget(quit);
	XtMapWidget(clear);
	XtMapWidget(options);
	XtVaSetValues( canvas, XmNsensitive, True, NULL);
}

void clearCallback (Widget w, XtPointer clientData, XtPointer callData) 
{ 
	//XClearArea (XtDisplay(canvas), XtWindow(canvas), 0, 0, 0, 0, 0);
	//XClearWindow(display,XtWindow(canvas));
	Colormap cmap;
	XColor color, ignore;
	char *colorname = "Black";

	XtSetArg(wargs[0], XmNlabelString, XmStringCreateLocalized("Are you sure?"));
	XtSetValues(clear, wargs, 1);
	XtVaSetValues( clear, XmNsensitive, False, NULL);
	XtVaSetValues( canvas, XmNsensitive, False, NULL);
	XtUnmapWidget(quit);
	XtMapWidget(no);
	XtMapWidget(yes);
	cmap = DefaultColormap (display, screen);
	XAllocNamedColor(display, cmap, colorname, &color, &ignore);
	XtSetArg(wargs[0], XmNbackground, color.pixel);
	XtSetValues(clear, wargs, 1);
	Clearflag=TRUE;
}

void redCallback ( Widget w, XtPointer clientData, XtPointer callData)
{
	gcval.foreground = Rcolor.pixel;
	draw = XtGetGC ( canvas, GCForeground | GCBackground , &gcval );
}

void greenCallback ( Widget w, XtPointer clientData, XtPointer callData)
{
	gcval.foreground = Gcolor.pixel;
	draw = XtGetGC ( canvas, GCForeground | GCBackground , &gcval );
}

void blueCallback ( Widget w, XtPointer clientData, XtPointer callData)
{
	gcval.foreground = Bcolor.pixel;
	draw = XtGetGC ( canvas, GCForeground | GCBackground , &gcval );
}

void brownCallback ( Widget w, XtPointer clientData, XtPointer callData)
{
	gcval.foreground = Brcolor.pixel;
	draw = XtGetGC ( canvas, GCForeground | GCBackground , &gcval );
}

void orangeCallback ( Widget w, XtPointer clientData, XtPointer callData)
{
	gcval.foreground = Ocolor.pixel;
	draw = XtGetGC ( canvas, GCForeground | GCBackground , &gcval );
}

void quitCallback ( Widget w, XtPointer clientData, XtPointer callData)
{
	Colormap cmap;
	XColor color, ignore;
	char *colorname = "Black";

	XtSetArg(wargs[0], XmNlabelString, XmStringCreateLocalized("Are you sure?"));
	XtSetValues(quit, wargs, 1);
	XtVaSetValues( quit, XmNsensitive, False, NULL);
	XtVaSetValues( canvas, XmNsensitive, False, NULL);
	XtUnmapWidget(clear);
	XtMapWidget(no);
	XtMapWidget(yes);
	cmap = DefaultColormap (display, screen);
	XAllocNamedColor(display, cmap, colorname, &color, &ignore);
	XtSetArg(wargs[0], XmNbackground, color.pixel);
	XtSetValues(quit, wargs, 1);
	Clearflag=FALSE;
}

void yesCallback ( Widget w, XtPointer clientData, XtPointer callData)
{
	if(Clearflag)
	{
		XClearWindow(display,XtWindow(canvas));
		XtSetArg(wargs[0], XmNlabelString,XmStringCreateLocalized("     CLEAR     "));
		XtSetValues(clear, wargs, 1);
		XtVaSetValues( clear, XmNsensitive, True, NULL);
		XtVaSetValues( canvas, XmNsensitive, True, NULL);
		XtMapWidget(quit);
		XtMapWidget(clear);
		XtUnmapWidget(no);
		XtUnmapWidget(yes);
	}
	else
		exit(0);
}

void noCallback ( Widget w, XtPointer clientData, XtPointer callData)
{
	if(Clearflag)
	{
		XtSetArg(wargs[0], XmNlabelString,XmStringCreateLocalized("     CLEAR     "));
		XtSetValues(clear, wargs, 1);
		XtVaSetValues( clear, XmNsensitive, True, NULL);
	}
	else
	{
		XtSetArg(wargs[0], XmNlabelString,XmStringCreateLocalized("      QUIT     "));
		XtSetValues(quit, wargs, 1);
		XtVaSetValues( quit, XmNsensitive, True, NULL);		
	}
	XtVaSetValues( canvas, XmNsensitive, True, NULL);
	XtMapWidget(quit);
	XtMapWidget(clear);
	XtUnmapWidget(no);
	XtUnmapWidget(yes);
}

void HandleBoardEvents ( Widget w, XtPointer clientData, XEvent *event, Boolean *flag )
{
	if (FirstPt) 
	{
        	FirstPt=FALSE;
        	//printf("first point \n");
        	pointx1 = event->xbutton.x;
        	pointy1 = event->xbutton.y;
        	midx1 = event->xbutton.x -  radius;
        	midy1 = event->xbutton.y - radius;
        	XFillArc (XtDisplay(w),XtWindow(w),draw, midx1, midy1,2*radius,2*radius,0,360*64);
	}
	else 
	{
        	FirstPt=TRUE;
			//printf("second point \n ");
        	pointx2 = event->xbutton.x;
        	pointy2 =  event->xbutton.y;
        	midx2 = event->xbutton.x-radius;
        	midy2= event->xbutton.y-radius;
        	Dist = sqrt((pointx2-pointx1)*(pointx2-pointx1)+(pointy2-pointy1)*(pointy2-pointy1));
        	midpointx = (pointx1+pointx2)/2;
        	midpointy = (pointy1+pointy2)/2;
		XFillArc (XtDisplay(w),XtWindow(w),draw, midx2,midy2,2*radius,2*radius,0,360*64);
 		if( d1 > Dist)
		{
			XDrawLine (XtDisplay(w),XtWindow(w),draw,pointx1,pointy1,pointx2, pointy2);
		}
		else 
		if(Dist< 2*d1)
		{
			XFillArc(XtDisplay(w),XtWindow(w),draw,midpointx-radius,midpointy-radius,2*radius,2*radius,0,360*64);
			XDrawArc( XtDisplay(w),XtWindow(w),draw,midpointx-Dist/2,midpointy-Dist/2,Dist,Dist,0,360*64);
		}
		else
		{
			XDrawLine (XtDisplay(w),XtWindow(w),draw,pointx2,pointy2, pointx2, pointy1);
			XFillArc (XtDisplay(w),XtWindow(w),draw, pointx2-radius, pointy1-radius,2*radius,2*radius,0,360*64);
			XDrawLine (XtDisplay(w),XtWindow(w),draw,pointx2,pointy1, pointx1, pointy1);
			XDrawLine (XtDisplay(w),XtWindow(w),draw,pointx1,pointy1, pointx1, pointy2);
			XFillArc (XtDisplay(w),XtWindow(w),draw, pointx1-radius, pointy2-radius,2*radius,2*radius,0,360*64);
			XDrawLine (XtDisplay(w),XtWindow(w),draw,pointx1,pointy2, pointx2, pointy2);
		}
	}
}
