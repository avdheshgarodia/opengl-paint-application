#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <math.h>
#include "lodepng.h"

using namespace std;

void keyboard(unsigned char key, int x, int y);
void render(void);
void reshape_window(GLsizei w, GLsizei h);
void setupTexture();



int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;


enum MENU_TYPE
{
        FLIPV,
        FLIPH,
		INVERT,
		ADJBRIT	
			
};


// Menu handling function declaration
void menu(int);


struct Pixel{

	uint8_t r;
	uint8_t g;	
	uint8_t b;		
	uint8_t a;	
}__attribute__((packed));


vector<Pixel> pixelarray;


int main(int argc, char** argv)
{
	 srand (time(NULL));
	 	 
	 pixelarray.resize(SCREEN_WIDTH*SCREEN_HEIGHT);

	//Lets do some drawing  =)
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);

	//set title of window
	glutCreateWindow("Paint Thing");
	

	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(render);
	glutReshapeFunc(reshape_window);

	//set the background color to WHITE
	glClearColor(1.0,1.0,1.0,1.0);
	
	
	
	glutCreateMenu(menu);

	        // Add menu items
	        glutAddMenuEntry("Flip Vertical", FLIPV);
	        glutAddMenuEntry("Flip Horizontal", FLIPH);
			glutAddMenuEntry("Invert Colors", INVERT);
			glutAddMenuEntry("Adjust Brightness", ADJBRIT);



	        // Associate a mouse button with menu
	        glutAttachMenu(GLUT_RIGHT_BUTTON);
	

	
	glutMainLoop();
	
}



vector<Pixel> flipv(vector<Pixel> &i, int w, int h){
	
	
	vector<Pixel> image =i;
	
	
	for (size_t x = 0; x < w; ++x)
	{
		for (size_t y = 0; y < h/2; ++y)
		{
			
			uint8_t r =image[(y*w)+x].r;
			uint8_t g =image[(y*w)+x].g;	
			uint8_t b =image[(y*w)+x].b;
			uint8_t a =image[(y*w)+x].a;
			
			int newy = (h - y - 1);
			
			
			image[(y*w)+x].r = image[(newy*w)+x].r;
			image[(y*w)+x].g = image[(newy*w)+x].g;	
			image[(y*w)+x].b = image[(newy*w)+x].b;
			image[(y*w)+x].a = image[(newy*w)+x].a;

			
			image[(newy*w)+x].r=r;
			image[(newy*w)+x].g=g;	
			image[(newy*w)+x].b=b;
			image[(newy*w)+x].a=a;
		}
    }
	
	return image;
}


vector<Pixel> fliph(vector<Pixel> &i, int w, int h){
	
	
	vector<Pixel> image =i;
	
	
	for (size_t x = 0; x < w/2; ++x)
	{
		for (size_t y = 0; y < h; ++y)
		{
			
			uint8_t r =image[(y*w)+x].r;
			uint8_t g =image[(y*w)+x].g;	
			uint8_t b =image[(y*w)+x].b;
			uint8_t a =image[(y*w)+x].a;
			
			int newx = (w - x - 1);
			
			
			image[(y*w)+x].r = image[(y*w)+newx].r;
			image[(y*w)+x].g = image[(y*w)+newx].g;	
			image[(y*w)+x].b = image[(y*w)+newx].b;
			image[(y*w)+x].a = image[(y*w)+newx].a;

			
			image[(y*w)+newx].r=r;
			image[(y*w)+newx].g=g;	
			image[(y*w)+newx].b=b;
			image[(y*w)+newx].a=a;
		}
    }
	
	return image;
}

vector<Pixel> invertcolors(vector<Pixel> &i, int w, int h)
{
	
	vector<Pixel> image =i;
	
	for (size_t x = 0; x < w; ++x)
	{
		for (size_t y = 0; y < h; ++y)
		{
			image[(y*w)+x].r =255 - image[(y*w)+x].r;
			image[(y*w)+x].g =255 -  image[(y*w)+x].g;	
			image[(y*w)+x].b =255 -  image[(y*w)+x].b;
			image[(y*w)+x].a =255 -  image[(y*w)+x].a;
		}       
	} 
	
	return image;  
}
vector<Pixel> adjust(vector<Pixel> &i, int w, int h, int rt,int gt,int bt)
{
	
	vector<Pixel> image =i;
	
	for (size_t x = 0; x < w; ++x)
	{
		for (size_t y = 0; y < h; ++y)
		{
			image[(y*w)+x].r =255 - image[(y*w)+x].r;
			image[(y*w)+x].g =255 -  image[(y*w)+x].g;	
			image[(y*w)+x].b =255 -  image[(y*w)+x].b;
			image[(y*w)+x].a =255 -  image[(y*w)+x].a;
			
			
			uint8_t r =image[(y*w)+x].r;
			uint8_t g =image[(y*w)+x].g;	
			uint8_t b =image[(y*w)+x].b;

			r+=rt;
			g+=gt;
			b+=bt;
			
			if(r <0) r = 0;
			if(g <0) g = 0;
			if(b <0) b = 0;
			if(r > 255) r = 255;
			if(g > 255) g = 255;
			if(b > 255) b = 255;
			
			
			image[(y*w)+x].r=r;
			image[(y*w)+x].g=g;	
			image[(y*w)+x].b=b;
			
			
		}       
	} 
	
	return image;  
}




void menu(int item)
{
        switch (item)
        {
        case FLIPV:
		{
			pixelarray = flipv(pixelarray,SCREEN_WIDTH,SCREEN_HEIGHT);
		}
			break;	
        case FLIPH:
		{
			pixelarray = fliph(pixelarray,SCREEN_WIDTH,SCREEN_HEIGHT);
		}
		break;
		case INVERT:
		{
			pixelarray = invertcolors(pixelarray,SCREEN_WIDTH,SCREEN_HEIGHT);
		}
		case ADJBRIT:
		{
			int rt,gt,bt;
			cin<<rt;
			cin<<gt;
			cin<<bt;
			
			cout<<rt<<endl;
			cout<<gt<<endl;
			cout<<bt<<endl;
		
			
			
			pixelarray = adjust(pixelarray,SCREEN_WIDTH,SCREEN_HEIGHT,rt,gt,bt);
		}
		break;
        default:
                {       /* Nothing */       }
                break;
        }

        glutPostRedisplay();

        return;
}


void reshape_window(int width, int height)
{
	/*
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);        
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
	*/
	
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	
	pixelarray.resize(SCREEN_WIDTH*SCREEN_HEIGHT);
	
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GLdouble (width), 0, GLdouble (height) );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


	

}

void paint(int x, int y, double value){
	if(x>=0 && x<SCREEN_WIDTH){
		if(y>=0 && y<SCREEN_HEIGHT){
		
			pixelarray[x+(y*SCREEN_WIDTH)].r=value*60;
			pixelarray[x+(y*SCREEN_WIDTH)].g=value*100;
			pixelarray[x+(y*SCREEN_WIDTH)].b=value*170;
			

		
		}
	}		
}

void keyboard(unsigned char key, int x, int y){
	
	//if key is 27 esacpe is pressed 	
	if(key==27){
		//if escape is pressed exit the program
		exit(0);
	}else if(key=='c'){
		pixelarray.clear();
		pixelarray.resize(SCREEN_WIDTH*SCREEN_HEIGHT);
	}
	else if(key=='q'){
		
		int xc = x;
		int yc = SCREEN_HEIGHT - y;
		
		
		
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){	
				paint(xc+i, yc-j, (rand() / double(RAND_MAX)) );
				paint(xc-i, yc+j,  (rand() / double(RAND_MAX)));
				paint(xc+i, yc+j,  (rand() / double(RAND_MAX)) );
				paint(xc-i, yc-j,  (rand() / double(RAND_MAX)) );
			}
		}
		
	}else if(key=='g'){
		
		int xc = x;
		int yc = SCREEN_HEIGHT - y;
		
		pixelarray[xc+(yc*SCREEN_WIDTH)].b=255;
	}else if(key=='w'){
		cout<<"Clearing the screen"<<endl;
		
		for(int i=0;i<pixelarray.size();i++){
			pixelarray[i].r=255;
			pixelarray[i].g=255;
			pixelarray[i].b=255;
			pixelarray[i].a=255;
		}
			
	}else if(key=='l'){
		
		
	    vector<unsigned char> image; //the raw pixels
	    unsigned width, height;
		//char* filename;
		//cin<<filename;
		
		
	    //decode
	    unsigned error = lodepng::decode(image, width, height, "in.png");
	

	    //if there's an error, display it
	    if(error) cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;

	    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
		reshape_window(width,height);
		
		for(int i=0;i<width*height*4;i++){
			
			switch(i%4){
				
				case 0:pixelarray[(i/4)].r=(uint8_t)image[i];
					break;	
				case 1:pixelarray[(i/4)].g=(uint8_t)image[i];
					break;	
				case 2:pixelarray[(i/4)].b=(uint8_t)image[i];
					break;	
				case 3:pixelarray[(i/4)].a=(uint8_t)image[i];
			}
				
		}
		
		
		for(int x=0;x<width;x++){
			for(int y=0;y<height;y++){
				for(int c=0;c<4;c++){
					
					
				}
				
			}
		}
		
	}else if(key=='s'){
		
		
	
		//char* filename;
		//cin<<filename;
	  //Encode the image
		
		vector<Pixel> pixelarrayflipped = flipv(pixelarray,SCREEN_WIDTH,SCREEN_HEIGHT);
		
		vector<unsigned char> image;
		
		image.resize(SCREEN_WIDTH*SCREEN_HEIGHT*4);
		
		for(int i=0;i<SCREEN_WIDTH*SCREEN_HEIGHT*4;i++){
			
			switch(i%4){
				
				case 0:image[i]=(unsigned char)pixelarrayflipped[i/4].r;
					break;	
				case 1:image[i]=(unsigned char)pixelarrayflipped[i/4].g;
					break;	
				case 2:image[i]=(unsigned char)pixelarrayflipped[i/4].b;
					break;	
				case 3:image[i]=(unsigned char)pixelarrayflipped[i/4].a;
			}
				
		}
		cout<<"Writing to file"<<endl;
	    unsigned error = lodepng::encode("out.png",image, SCREEN_WIDTH, SCREEN_HEIGHT);

	    //if there's an error, display it
	    if(error) cout << "encoder error " << error << ": "<< lodepng_error_text(error) << endl;
		
	}
			
}


void render(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	//updateQuads();
	
	glDrawPixels((GLsizei)SCREEN_WIDTH,(GLsizei)SCREEN_HEIGHT,
	 	GL_RGBA,
	 	GL_UNSIGNED_BYTE,
	 	&pixelarray[0]);
    glutSwapBuffers();
}




