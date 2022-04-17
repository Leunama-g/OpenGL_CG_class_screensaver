#include<iostream>

#include <windows.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int state = -1;
int trictr = 0;
float rotate = 0;

int colorstate = 0;
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

Color centerPieceColor[8];
float coloroffset = 0.0f;

const int noOfTriangles = 8;
Coordinate origins[noOfTriangles];
Coordinate centerPiece[noOfTriangles];


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

void display() {
    //used to store the rotation offset
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

        angle = 90 + (45 * i);

       
            
        if (angle == 360)
            angle -= 360;

        if (seq == 1) {
            if (trictr == i) {
                angle += rotate;
                //std::cout << "i = " << i << ", rotate = " << rotate << ", state = " << state << std::endl;
            }
            glTranslatef(origins[i].x, origins[i].y, 0.0f);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            drawTriangle(bottom, head);
            
        }
        else
        {   
            
            glTranslatef(origins[i].x, origins[i].y, 0.0f);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            if (((colorstate + ((trictr - 1) * 20)) / 20) % 8 == i) {
                drawTriangle(highlight, highlight);
                //std::cout << "i = " << i << ", colorstate = " << ((colorstate + ((trictr - 1) * 20)) / 20) % 8 << std::endl;
            }
            else
            {
                drawTriangle(bottom, head);
            }
        }

        
        glLoadIdentity();
    }

    Color drawnC;
    glBegin(GL_POLYGON);

    for (int i = 0; i < noOfTriangles; i++) {
        drawnC.r = centerPieceColor[i].r + coloroffset;
        if (drawnC.r > 1.0f)
            drawnC.r -= 1.0f;
        drawnC.g = centerPieceColor[i].g + coloroffset;
        if (drawnC.g > 1.0f)
            drawnC.g -= 1.0f;
        drawnC.b = centerPieceColor[i].b + coloroffset;
        if (drawnC.b > 1.0f)
            drawnC.b -= 1.0f;
        glColor3f(drawnC.r, drawnC.g, drawnC.b);
        glVertex2f(centerPiece[i].x, centerPiece[i].y);
    }

    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {

    //call glutdisplayfunc again
    glutPostRedisplay();

    //update the screen 72 times per second
    glutTimerFunc(1000 / 72, timer, 0);

    if (colorstate < 180 && seq == 0)
        colorstate++;

    else if (colorstate == 180 && seq == 0) {
        colorstate = 0;
        seq = 1;
    }

    if (state < 90 && seq == 1) {
        state++;
    }
        
    else if (state == 90 && seq == 1) {
        state = 0;
        rotate = 0;
        seq = 0;
        if (trictr < 8)
            trictr++;
        else
            trictr = 0;
    }        

    if (rotate < 360 && seq == 1)
        rotate += 4;
    else if (rotate == 360 && seq == 1)
        rotate = 4;
    
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
