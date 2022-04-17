#include<iostream>

#include <windows.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//used to keep track of how long a triangle rotates
//90 frames
int state = -1;

//used to keep track of the triangle that is supposed to be rotated and highlighted in the que
int trictr = 0;

//used as a degree offset to rotate the triangles
//incremented 4 degrees per frame
float rotate = 0;


//used to keep track of for how long a triangle is to be highlighted 
//20 frames
int colorstate = 0;

//when seq == 1 rotate one triangle out of the eight 360 degrees
//when seq == 0 highlight the rotated triangle using the highlight color variable
int seq = 1;


struct Coordinate {
    float x = 0.0f;
    float y = 0.0f;
};

struct Color {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

//used to animate the color change of the polygon
//off sets each vertex of the polygon
//incremented by o.o1 every frame
float coloroffset = 0.0f;


const int noOfTriangles = 8;

//holds the coordinats of the triangle center 
//Draw triangle function draws the triangles on the center of the screen then the coordinates in origins are used to translate the center to
//change the position of the triangle
Coordinate origins[noOfTriangles];

//holds the coordinates of the center polygon(octagon)
Coordinate centerPiece[noOfTriangles];

//holds the color values for the colors of each vertex of the octagon
Color centerPieceColor[8];

void getNewOrigin(float r, int num_points, Coordinate* origins);
void drawTriangle(Color bottom, Color head);


// produces set of points that equidistant from the center and also equi distant from themselves
// PARAM:
//      r is the radius
//      num_points is the number of points to be generated 
//      origins is an array of new origin point for translation
void getNewOrigin(float r, int num_points, Coordinate* origins)
{
    float theta, x, y;
    for (int ii = 0; ii < num_points; ii++)
    {
        theta = 2.0f * 3.1415926f * float(ii) / float(num_points);//get the current angle

        x = r * cosf(theta);//calculate the x component
        y = r * sinf(theta);//calculate the y component

        origins[ii].x = x;
        origins[ii].y = y;
    }
}


//draw triangle on the center of the screen
void drawTriangle(Color bottom, Color head) {

    glBegin(GL_TRIANGLES);

    glColor3f(head.r, head.g, head.b);
    glVertex2f(0.0, 0.5);
    glColor3f(bottom.r, bottom.g, bottom.b);
    glVertex2f(0.56, -0.5);
    glColor3f(bottom.r, bottom.g, bottom.b);
    glVertex2f(-0.56, -0.5);

    glEnd();
}



void init(void)
{
    glClearColor(0, 0, 0, 0);

    //get the set of points to translate the triangles
    getNewOrigin(5, noOfTriangles, origins);

    //get of points for the center polygon
    getNewOrigin(2.5, noOfTriangles, centerPiece);
}

//draws every frame
void display() {
    //used to store the rotation offset
    //value of angle is used to rotate each triangle so that it points to the center of the screen
    int angle;

    //head: color the first point of the triangle
    //bottom: color the other two points of the triangle
    //highlight: color for the highlighted triangle
    Color head, bottom, highlight;

    head.r = 0.0f;
    head.g = 0.4f;
    head.b = 0.8f;

    bottom.r = 0.0f;
    bottom.g = 0.4f;
    bottom.b = 0.4f;

    highlight.r = 0.9f; 
    highlight.g = 0.1f;
    highlight.b = 0.1f;

    

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    for (int i = 0; i < noOfTriangles; i++) {

        //value of angle is used to rotate each triangle so that it points to the center of the screen
        angle = 90 + (45 * i);
    
        if (angle == 360)
            angle -= 360;

        if (seq == 1) {

            //used to rotate one triangle at a time
            if (trictr == i) {
                angle += rotate;
                //std::cout << "i = " << i << ", rotate = " << rotate << ", state = " << state << std::endl;
            }

            //translate the origin to the supposed triangle positions
            glTranslatef(origins[i].x, origins[i].y, 0.0f);

            //rotate the triangle with the value of angle
            glRotatef(angle, 0.0f, 0.0f, 1.0f);

            //draw triangle
            drawTriangle(bottom, head);
            
        }
        else
        {   
            //translate the origin to the supposed triangle positions
            glTranslatef(origins[i].x, origins[i].y, 0.0f);

            //rotate so that it points to the center
            glRotatef(angle, 0.0f, 0.0f, 1.0f);

            //used to highlight every triangle sequentially
            //each triangle is highlighted for 20 frames
            if (((colorstate + ((trictr - 1) * 20)) / 20) % 8 == i) {
                drawTriangle(highlight, highlight);
                //std::cout << "i = " << i << ", colorstate = " << ((colorstate + ((trictr - 1) * 20)) / 20) % 8 << std::endl;
            }
            else
            {
                //the other 7 triangles that arent highlighted are drawn
                drawTriangle(bottom, head);
            }
        }

        //makes the changes that gltranslatef anf glrotatef are undone
        glLoadIdentity();
    }

    Color drawnC;
    //draws the octagon
    glBegin(GL_POLYGON);

    for (int i = 0; i < noOfTriangles; i++) {
        //coloroffset is used to animate the color change
        //incremented 0.01 each frame
        drawnC.r = centerPieceColor[i].r + coloroffset;

        //acceptablle values for glcolor3f are between 0-1
        //to make sure an error does not occur 
        if (drawnC.r > 1.0f)
            drawnC.r -= 1.0f;
        drawnC.g = centerPieceColor[i].g + coloroffset;
        if (drawnC.g > 1.0f)
            drawnC.g -= 1.0f;
        drawnC.b = centerPieceColor[i].b + coloroffset;
        if (drawnC.b > 1.0f)
            drawnC.b -= 1.0f;

        //changes the color so that each vertex is different
        glColor3f(drawnC.r, drawnC.g, drawnC.b);

        //draws the vertex of the polygon
        glVertex2f(centerPiece[i].x, centerPiece[i].y);
    }

    glEnd();

    glutSwapBuffers();
}

//called everytime the width and hight of the window changes
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
}

//used to redisplay the modified frame buffer at 72hz
void timer(int) {

    //call glutdisplayfunc again
    glutPostRedisplay();

    //update the screen 72 times per second
    glutTimerFunc(1000 / 72, timer, 0);


    //controls the red highlight state
    //happens every 20 frame
    //20 * 8 = 160 so all the triangles are highlighted 
    //added another 20 so that the one that was highlighted firstis highlighted at the end
    if (colorstate < 180 && seq == 0)
        colorstate++;

    else if (colorstate == 180 && seq == 0) {
        colorstate = 0;
        seq = 1;
    }

    //used to keep track of how long a triangle rotates
    //90 frames
    if (state < 90 && seq == 1) {
        state++;
    }
        
    else if (state == 90 && seq == 1) {
        state = 0;

        //reset the rotation offset so that it starts from 0 for the next triangle
        rotate = 0;

        // change the seq
        // highlight sequence instead of rotation sequence 
        seq = 0;

        //every 90 frame the target triangle is changed 
        //the target triangle gets rotated or is highlighted first in the highlight sequence 
        if (trictr < 8)
            trictr++;
        else
            trictr = 0;
    }    


    //used as a degree offset to rotate the triangles
    //incremented 4 degrees per frame
    if (rotate < 360 && seq == 1)
        rotate += 4;
    else if (rotate == 360 && seq == 1)
        rotate = 4;
    

    //used to animate the color change of the polygon
    //off sets each vertex of the polygon
    //incremented by o.o1 every frame
    if (coloroffset < 1.0f) {
        coloroffset += 0.01f;
    }
    else
    {
        coloroffset = 0.0f;
    }

}


int main(int argc, char** argv)
{
    //center piece color setup
    centerPieceColor[3].r = 0.2f;
    centerPieceColor[3].g = 0.8f;
    centerPieceColor[3].b = 0.0f;

    centerPieceColor[7].r = 1.0f;
    centerPieceColor[7].g = 0.9f;
    centerPieceColor[7].b = 0.2f;

    centerPieceColor[6].r = 1.0f;
    centerPieceColor[6].g = 0.5f;
    centerPieceColor[6].b = 0.1f;

    centerPieceColor[5].r = 0.9f;
    centerPieceColor[5].g = 0.1f;
    centerPieceColor[5].b = 0.1f;

    centerPieceColor[4].r = 0.6f;
    centerPieceColor[4].g = 0.0f;
    centerPieceColor[4].b = 0.6f;

    centerPieceColor[1].r = 0.0f;
    centerPieceColor[1].g = 0.4f;
    centerPieceColor[1].b = 0.8f;

    centerPieceColor[2].r = 0.0f;
    centerPieceColor[2].g = 0.7f;
    centerPieceColor[2].b = 0.7f;

    centerPieceColor[0].r = 0.0f;
    centerPieceColor[0].g = 0.4f;
    centerPieceColor[0].b = 0.4f;

    //glut setup
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(250, 0);
    glutInitWindowSize(780, 780);

    glutCreateWindow("Screensaver");

    std::cout << glGetString(GL_VERSION) << std::endl;

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    return 0;
}
