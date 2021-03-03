/*
 *		This Code Was Created By Lionel Brits & Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */
//#include <afx.h>
#include <windows.h>		// Header File For Windows
#include <math.h>			// Math Library Header File
#include <stdio.h>			// Header File For Standard Input/Output
#include <conio.h>

/*#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
*/
#include "SkyBox.h"

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application
GLuint		base;


bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	blend;				// Blending ON/OFF
bool	bp;					// B Pressed?
bool	fp;					// F Pressed?
bool    ct = true;
bool	gameover = false;
bool    enemyattack[6]={false, false, false, false,false,false};
#define SOLID 1
#define WIRE 2


const float piover180 = 0.0174532925f;
float heading = 0;
float xpos;
float zpos;
float ypos = 0.0f;
float ybase = 0.0f;
float cg = 0.0f;
int enemynum = 6;
int time1[6] = {30,30,30,30,30,30};
int time2[6] = {100,100,100,100,100,100};
int mytime = 30;
bool myattackflag = false;
bool attackflag[6] = {false,false,false,false,false,false};
bool tietuisme;
bool zuiji = false;
float vspeed = 0.1f;


//鼠标位置
	int xmouse = 0;
	int ymouse = 0;
	int xmouseOld = 0;
	int ymouseOld = 0;
bool attacking = false;

bool firstcome = true;
GLfloat	yrot;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat	z=0.0f;				// Depth Into The Screen
GLfloat toward = 0;
GLfloat toward1 = 0;
GLfloat toward2 = 0;


double xrote = 0.0f;
double yrote = 0.0f;
double zrote = 0.0f;

GLuint	filter;				// Which Filter To Use
GLuint	texture[7];			// Storage For 7 Textures

GLuint	tietu[10];

CSkyBox      m_cSkyBox;//cy
/*
typedef struct													// Create A Structure
{
	GLubyte	*imageData;											// Image Data (Up To 32 Bits)
	GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
	GLuint	width;												// Image Width
	GLuint	height;												// Image Height
	GLuint	texID;												// Texture ID Used To Select A Texture
} TextureImage;													// Structure Name
TextureImage textures[10];
bool LoadTGA(TextureImage *texture, char *filename)				// Loads A TGA File Into Memory
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;											// Temporary Variable
	GLuint		type=GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if(	file==NULL ||											// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
			return FALSE;										// Return False
		else													// Otherwise
		{
			fclose(file);										// If Anything Failed, Close The File
			return FALSE;										// Return False
		}
	}

	texture->width  = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(	texture->width	<=0	||									// Is The Width Less Than Or Equal To Zero
		texture->height	<=0	||									// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))						// Is The TGA 24 or 32 Bit?
	{
		fclose(file);											// If Anything Failed, Close The File
		return FALSE;											// Return False
	}

	texture->bpp	= header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;							// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

	if(	texture->imageData==NULL ||								// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)							// Was Image Data Loaded
			free(texture->imageData);							// If So, Release The Image Data

		fclose(file);											// Close The File
		return FALSE;											// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];								// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);												// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);						// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	if (texture[0].bpp==24)										// Was The TGA 24 Bits
	{
		type=GL_RGB;											// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;												// Texture Building Went Ok, Return True
}
GLvoid BuildFont(GLvoid)										// Build Our Font Display List
{
	base=glGenLists(95);										// Creating 95 Display Lists
	glBindTexture(GL_TEXTURE_2D, textures[9].texID);			// Bind Our Font Texture
	for (int loop=0; loop<95; loop++)							// Loop Through All 95 Lists
	{
		float cx=float(loop%16)/16.0f;							// X Position Of Current Character
		float cy=float(loop/16)/8.0f;							// Y Position Of Current Character

		glNewList(base+loop,GL_COMPILE);						// Start Building A List
			glBegin(GL_QUADS);									// Use A Quad For Each Character
				glTexCoord2f(cx,         1.0f-cy-0.120f); glVertex2i(0,0);	// Texture / Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f, 1.0f-cy-0.120f); glVertex2i(16,0);	// Texutre / Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f, 1.0f-cy);		  glVertex2i(16,16);// Texture / Vertex Coord (Top Right)
				glTexCoord2f(cx,         1.0f-cy);		  glVertex2i(0,16);	// Texture / Vertex Coord (Top Left)
			glEnd();											// Done Building Our Quad (Character)
			glTranslated(10,0,0);								// Move To The Right Of The Character
		glEndList();											// Done Building The Display List
	}															// Loop Until All 256 Are Built
}


//显示文本信息
GLvoid glPrint(GLint x, GLint y, const char *string, ...)		// Where The Printing Happens
{
	char		text[256];										// Holds Our String
	va_list		ap;												// Pointer To List Of Arguments

	if (string == NULL)											// If There's No Text
		return;													// Do Nothing

	va_start(ap, string);										// Parses The String For Variables
	    vsprintf(text, string, ap);								// And Converts Symbols To Actual Numbers
	va_end(ap);													// Results Are Stored In Text

	glBindTexture(GL_TEXTURE_2D, textures[9].texID);			// Select Our Font Texture
	glPushMatrix();												// Store The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	glTranslated(x,y,0);										// Position The Text (0,0 - Bottom Left)
	glListBase(base-32);										// Choose The Font Set
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);			// Draws The Display List Text
	glPopMatrix();												// Restore The Old Projection Matrix
}
*/

class ModelPeople{
public:
	ModelPeople(){}
	ModelPeople(int isenemy, float x,float z){
		IsEnemy = isenemy;
		Dir = 1;
		Towards = 0;
		X = x;
		Z = z;
		HP = 100;
		alive = true;
	}
	float getposX(){
		return X;
	}
	float getposZ(){
		return Z;
	}

	float X, Z;
	int Dir;
	int Towards;
	int IsEnemy;
	int HP;
	bool alive;
	

};
ModelPeople me(0,0,0);
ModelPeople enemy[6] = {
	ModelPeople(1,3,0),
	ModelPeople(1,5,3),
	ModelPeople(1,-55,-1),
	ModelPeople(1,-50,2),
	ModelPeople(1,0,24),
	ModelPeople(1,2,26)

};


typedef struct tagVERTEX
{
	float x, y, z;
	float u, v;
} VERTEX;

typedef struct tagTRIANGLE
{
	VERTEX vertex[4];
} TRIANGLE;

typedef struct tagSECTOR
{
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;

SECTOR sector1;				// Our Model Goes Here:

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

void readstr(FILE *f,char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}


void SetupWorld()
{
	float x, y, z, u, v;
	int numtriangles;
	FILE *filein;
	char oneline[255];
	filein = fopen("data/world.txt", "rt");				// File To Load World Data From

	readstr(filein,oneline);
	sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);

	sector1.triangle = new TRIANGLE[numtriangles];
	sector1.numtriangles = numtriangles;
	for (int loop = 0; loop < numtriangles; loop++)
	{
		for (int vert = 0; vert < 4; vert++)
		{
			readstr(filein,oneline);
			sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			sector1.triangle[loop].vertex[vert].x = x;
			sector1.triangle[loop].vertex[vert].y = y;
			sector1.triangle[loop].vertex[vert].z = z;
			sector1.triangle[loop].vertex[vert].u = u;
			sector1.triangle[loop].vertex[vert].v = v;
		}
	}
	fclose(filein);
	return;
}

AUX_RGBImageRec *LoadBMP(char *Filename)                // Loads A Bitmap Image
{
        FILE *File=NULL;                                // File Handle

        if (!Filename)                                  // Make Sure A Filename Was Given
        {
                return NULL;                            // If Not Return NULL
        }

        File=fopen(Filename,"r");                       // Check To See If The File Exists

        if (File)                                       // Does The File Exist?
        {
                fclose(File);                           // Close The Handle
                return auxDIBImageLoad(Filename);       // Load The Bitmap And Return A Pointer
        }
        return NULL;                                    // If Load Failed Return NULL
}





int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
        int Status=FALSE;                               // Status Indicator

        AUX_RGBImageRec *TextureImage[10];               // Create Storage Space For The Texture

        memset(TextureImage,0,sizeof(void *)*1);        // Set The Pointer To NULL
			

		char *ph[] = {
			"Data/地板.BMP",
			"Data/天花板1.BMP",
			"Data/QIANG.bmp",
			"Data/XIANGZI.bmp",
			"Data/A.bmp",
			"Data/B.bmp",
			"Data/C.bmp",
			"Data/D.bmp",
			"Data/迷彩1.bmp",
			"Data/迷彩2.bmp"
		};
        // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
		for(int pi = 0; pi < 10; pi++){
			
			if (TextureImage[pi]=LoadBMP(ph[pi]))
			{
				
                Status=TRUE;                            // Set The Status To TRUE
                glGenTextures(1, &tietu[pi]);          // Create Three Textures


			
				// Create MipMapped Texture
				glBindTexture(GL_TEXTURE_2D, tietu[pi]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[pi]->sizeX, TextureImage[pi]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[pi]->data);
			}
			if (TextureImage[pi])                            // If Texture Exists
			{
                if (TextureImage[pi]->data)              // If Texture Image Exists
                {
                        free(TextureImage[pi]->data);    // Free The Texture Image Memory
                }

                free(TextureImage[pi]);                  // Free The Image Structure
			}
		}
			
        return Status;                                  // Return The Status
		
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine
	{
		
		return FALSE;									// If Texture Didn't Load Return FALSE
	}
	/*	
	if ((!LoadTGA(&textures[0],"Data/BlueFace.tga")) ||			// Load The BlueFace Texture
		(!LoadTGA(&textures[1],"Data/Bucket.tga")) ||			// Load The Bucket Texture
		(!LoadTGA(&textures[2],"Data/Target.tga")) ||			// Load The Target Texture
		(!LoadTGA(&textures[3],"Data/Coke.tga")) ||				// Load The Coke Texture
		(!LoadTGA(&textures[4],"Data/Vase.tga")) ||				// Load The Vase Texture
		(!LoadTGA(&textures[5],"Data/Explode.tga")) ||			// Load The Explosion Texture
		(!LoadTGA(&textures[6],"Data/Ground.tga")) ||			// Load The Ground Texture
		(!LoadTGA(&textures[7],"Data/Sky.tga")) ||				// Load The Sky Texture
		(!LoadTGA(&textures[8],"Data/Crosshair.tga")) ||		// Load The Crosshair Texture
		(!LoadTGA(&textures[9],"Data/Font.tga")))				// Load The Crosshair Texture
	{
		return FALSE;											// If Loading Failed, Return False
	}
	BuildFont();
	*/
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	SetupWorld();
		
	m_cSkyBox.InitSkyBox(25*4,25*4,40*2,0,0,-40*0.8);//cy

	return TRUE;										// Initialization Went OK
}

//画球
void drawBall(double R, double x, double y,double z, int MODE) {
	glPushMatrix();
	glTranslated(x,y,z);
	if (MODE == SOLID) {
		glutSolidSphere(R,20,20);
	} else if (MODE ==WIRE) {
		glutWireSphere(R,20,20);
	}
	glPopMatrix();
}
//画半球
void drawHalfBall(double R, double x, double y,double z, int MODE) {
	glPushMatrix();
	glTranslated(x,y,z);
	GLdouble eqn[4]={0.0, 1.0, 0.0, 0.0};
	glClipPlane(GL_CLIP_PLANE0,eqn);
	glEnable(GL_CLIP_PLANE0);
	if (MODE == SOLID) {
		glutSolidSphere(R,20,20);
	} else if (MODE ==WIRE) {
		glutWireSphere(R,20,20);
	}
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
}



void drawSkewed(double l, double w, double h, double x, double y, double z, int MODE) {
	int tt = 8;
	if(tietuisme)
		tt = 9;
	glPushMatrix();
	glScaled(l, w, h);
	glTranslated(x, y, z);
	if (MODE == SOLID) {
		glEnable ( GL_TEXTURE_GEN_S );  //启用纹理坐标生成
		glEnable ( GL_TEXTURE_GEN_T ); 
		glBindTexture(GL_TEXTURE_2D, tietu[tt]);
		glutSolidCube(1);
		//glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT);		//防止颜色干扰贴图
		glBegin(GL_QUADS);
			// 前面
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x-l/2, y+w/2,  z+h/2);	//纹理和四边形的左上方
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x-l/2, y-w/2,  z+h/2);	//纹理和四边形的左下角
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+l/2, y-w/2,  z+h/2);	//纹理和四边形的右下角
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+l/2, y+w/2,  z+h/2);	//纹理和四边形的右上角
		
			// 后面
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x+l/2, y+w/2,  z-h/2);	//纹理和四边形的左上方
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x+l/2, y-w/2,  z-h/2);	//纹理和四边形的左下角
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x-l/2, y-w/2,  z-h/2);	//纹理和四边形的右下角
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x-l/2, y+w/2,  z-h/2);	//纹理和四边形的右上角
		
			// 上面
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x-l/2, y+w/2,  z-h/2);	//纹理和四边形的左上方
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x-l/2, y+w/2,  z+h/2);	//纹理和四边形的左下角
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+l/2, y+w/2,  z+h/2);	//纹理和四边形的右下角
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+l/2, y+w/2,  z-h/2);	//纹理和四边形的右上角
		
			// 下面
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x-l/2, y-w/2,  z+h/2);	//纹理和四边形的左上方
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x-l/2, y-w/2,  z-h/2);	//纹理和四边形的左下角
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+l/2, y-w/2,  z-h/2);	//纹理和四边形的右下角
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+l/2, y-w/2,  z+h/2);	//纹理和四边形的右上角
	
			// 右面
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x+l/2, y+w/2,  z+h/2);	//纹理和四边形的左上方
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x+l/2, y-w/2,  z+h/2);	//纹理和四边形的左下角
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+l/2, y-w/2,  z-h/2);	//纹理和四边形的右下角
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+l/2, y+w/2,  z-h/2);	//纹理和四边形的右上角
		
			// 左面
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x-l/2, y+w/2,  z-h/2);	//纹理和四边形的左上方
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x-l/2, y-w/2,  z-h/2);	//纹理和四边形的左下角
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x-l/2, y-w/2,  z+h/2);	//纹理和四边形的右下角
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x-l/2, y+w/2,  z+h/2);	//纹理和四边形的右上角
		
		glEnd();
		glDisable ( GL_TEXTURE_GEN_S );  //启用纹理坐标生成
		glDisable ( GL_TEXTURE_GEN_T );
	} else if (MODE ==WIRE) {
		glutWireCube(1);
	}

	glPopMatrix();
}

//画人
void drawpeople(bool isattack) {
		glPushMatrix();
	//定义白色

	glColor3f(1.0, 1.0, 1.0); 
	

	
	if(isattack){
		//胳膊
		drawSkewed(0.1, 0.3, 0.1, 3.5, -1.2, 0, SOLID);
		drawSkewed(0.1, 0.2, 0.1, -3.5, -1.6, 0, SOLID);
		drawSkewed(0.1, 0.095, 0.3, -3.5, -4.0, 0.33, SOLID);
		//手
		drawBall(0.08, 0.35, -0.5, 0, SOLID);
		drawBall(0.08, -0.35, -0.38, 0.3, SOLID);
	}else{
		//胳膊
		drawSkewed(0.1, 0.3, 0.1, 3.5, -1.2, 0, SOLID);
		drawSkewed(0.1, 0.3, 0.1, -3.5, -1.2, 0, SOLID);
		//手
		drawBall(0.08, 0.35, -0.5, 0, SOLID);
		drawBall(0.08, -0.35, -0.5, 0, SOLID);
	}
	//头
	drawBall(0.12, 0, 0, 0, SOLID);
	//身体
	drawSkewed(0.5, 0.44, 0.4, 0, -0.75, 0, SOLID);
	//肩膀
	drawHalfBall(0.1, 0.35, -0.2, 0, SOLID);
	drawHalfBall(0.1, -0.35, -0.2, 0, SOLID);
	//腿
	drawSkewed(0.12, 0.3, 0.2, 1, -2.4, 0, SOLID);
	drawSkewed(0.12, 0.3, 0.2, -1, -2.4, 0, SOLID);
	//脚
	drawSkewed(0.15, 0.1, 0.3, 0.9, -9.2, 0, SOLID);
	drawSkewed(0.15, 0.1, 0.3, -0.9, -9.2, 0, SOLID);

	glPopMatrix();

}

bool check_be_attacked(int i){
	
	if(attackflag[i] && (time1[i] != 0 || time2[i] != 0)){
		if(time1[i] != 0){
			time1[i] --;
			time2[i] --;
			return false;
		}
		enemyattack[i] = false;	
		time2[i] --;
		return false;
	
	}
	time1[i] = 30;
	time2[i] = 100;
		float xmin, xmax, zmin, zmax;
		if(sin(enemy[i].Towards)){
			xmin = min(enemy[i].X, enemy[i].X + 2 * sin(enemy[i].Towards));
			xmax = max(enemy[i].X, enemy[i].X + 2 * sin(enemy[i].Towards));
			zmin = enemy[i].Z - 0.5;
			zmax = enemy[i].Z + 0.5;
		}else{
			zmin = min(enemy[i].Z, enemy[i].Z + 2 * cos(enemy[i].Towards));
			zmax = max(enemy[i].Z, enemy[i].Z + 2 * cos(enemy[i].Towards));
			xmin = enemy[i].X - 0.5;
			xmax = enemy[i].X + 0.5;
		}
	
		if(me.X > xmin && me.X < xmax && me.Z > zmin && me.Z < zmax){
			PlaySound("data/enemyattack.wav",NULL,SND_ASYNC);
			enemyattack[i] = true;
			attackflag[i] = true;
			return true;
		}
		attackflag[i] = false;
		
		return false;
		
	
	
}
bool check_attack(int i){
	if(myattackflag && mytime != 0){
		mytime --;
		return false;
	}
	myattackflag = false;
	mytime = 30;
	float minx = min(me.X + 3 * cos(-heading + 30),me.X + 3 * cos(-heading - 30));
	float maxx = max(me.X + 3 * cos(-heading + 30),me.X + 3 * cos(-heading - 30));
	float minz = min(me.Z + 3 * sin(-heading + 30),me.Z + 3 * sin(-heading - 30));
	float maxz = max(me.Z + 3 * sin(-heading + 30),me.Z + 3 * sin(-heading - 30));
	if(enemy[i].X <= maxx && enemy[i].X >= minx &&enemy[i].Z <= maxz && enemy[i].Z >= minz ){
		myattackflag = true;
		return true;
	}
		
	return false;
}



int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	AllocConsole();
	if(firstcome){
		CString hun;
		hun.Format("W,A,S,D移动，空格跳跃，shift加速，J攻击，靠近敌人2个单位会被攻击！");
		AfxMessageBox(hun);
		firstcome = false;
	
	}
	
	if(!me.alive && !gameover){
		CString hun;
		hun.Format("你死了！");
		AfxMessageBox(hun);
		gameover = true;
	}
	if(enemynum <= 0 && !gameover){
		CString hun;
		hun.Format("你赢了！");
		AfxMessageBox(hun);
		gameover = true;
	}
	if(me.alive&&enemynum > 0){
	//glPrint(250,10,"HP: %i",me.HP);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	
	
	/*
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT);		//防止颜色干扰贴图
    glPointSize( 10 );//设置点的大小为10
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POINTS);
	glColor3f(0.0,1.0,0.0); //设置颜色	
	glVertex3f(0.0f, 0.0f, -0.5f);					
	glEnd();
	glPopAttrib();
	*/
	
	int i;
	for(i = 0; i < 1; i ++){
		if(check_be_attacked(i) && enemy[i].alive){
			//me.HP -= 10;
			zuiji = true;
			_cprintf("MY_HP=%d\n", me.HP);
		}
			
		if(me.HP <= 0){
			PlaySound("data/enemydead.wav",NULL,SND_ASYNC);
			me.alive = false;
		}
			
	}
	if(me.HP > 0){
		
		
		glRotatef(180 ,0,1,0);
		glTranslatef(0, -1.25, 4);
		glColor3f(0.0, 1.0, 0.0); 
		tietuisme = true;
		drawpeople(attacking);
		glTranslatef(0, 1.25, 0);
		glRotatef(180 ,0,-1,0);

	}


	//FreeConsole();
	
	glEnable(GL_TEXTURE_2D);
	
	GLfloat x_m, y_m, z_m, u_m, v_m;
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;

	ypos += cg;
	
	yrot = heading;
	xmouse = xmouseOld;
	GLfloat ytrans = -ypos-walkbias-0.25f;
	GLfloat sceneroty = 360.0f - yrot;
	toward = yrot;
	while(toward < 0)
		toward += 360;
	while(toward > 360)
		toward -= 360;
	toward = 360 - toward;
	toward1 = toward + 30;
	toward2 = toward - 30;

/*	_cprintf("X=%f\n", me.X);
	_cprintf("Z=%f\n", me.Z);
	_cprintf("toward=%f\n", toward);*/
	int numtriangles;
	
	


	//lookupdown += (ymouse - ymouseOld) * 0.1f;
	glRotatef(lookupdown ,1.0f,0,0);
	glRotatef(sceneroty ,0,1.0f,0);

	glTranslatef(xtrans - 50, ytrans - 2, ztrans - 60);
	me.X = -(xtrans - 50);
	me.Z = -(ztrans - 60 + 2);

	int num = 0;
	for(num; num < 1; num ++){
		if(enemy[num].alive){
		glTranslatef(enemy[num].X, 1, enemy[num].Z+2);
		glRotatef(enemy[num].Towards ,0,1.0f,0);
		tietuisme = false;
		drawpeople(enemyattack[num]);
		glRotatef(enemy[num].Towards ,0,-1.0f,0);
		glTranslatef(-enemy[num].X, -1, -enemy[num].Z-2);
		}
	}
	
	if(zuiji){
		float dx = me.X - enemy[0].X;
		float dz = me.Z - enemy[0].Z;
		float dd = sqrt(dx * dx + dz * dz);
		if(dd > 15.0)
			zuiji = false;
		enemy[0].X += 0.05 * (dx / dd);
		enemy[0].Z += 0.05 * (dz / dd);
	}
	
	//enemy1
	if(!zuiji)
	switch(enemy[0].Dir){
	case 1:
		enemy[0].Towards = 90;
		enemy[0].X += 0.1;
		
		if(enemy[0].X > 50)
			enemy[0].Dir = 2; 
		break;
	case 2:
		enemy[0].Towards = 0;
		enemy[0].Z += 0.1;
		if(enemy[0].Z > 22)
			enemy[0].Dir = 3;
		break;
	case 3:
		enemy[0].Towards = -90;
		enemy[0].X -= 0.1;
		if(enemy[0].X < 0)
			enemy[0].Dir = 4;

		break;
	case 4:
		enemy[0].Towards = -180;
		enemy[0].Z -= 0.1;
		if(enemy[0].Z < 0)
			enemy[0].Dir = 1;
		break;

	}
	/*
//enemy2

	
	switch(enemy[1].Dir){
	case 1:
		enemy[1].Towards = 90;
		enemy[1].X += 0.15;
		
		if(enemy[1].X > 50)
			enemy[1].Dir = 2; 
		break;
	case 2:
		enemy[1].Towards = 0;
		enemy[1].Z += 0.15;
		if(enemy[1].Z > 50)
			enemy[1].Dir = 3;
		break;
	case 3:
		enemy[1].Towards = -90;
		enemy[1].X -= 0.1;
		if(enemy[1].X < -10)
			enemy[1].Dir = 4;

		break;
	case 4:
		enemy[1].Towards = -180;
		enemy[1].Z -= 0.1;
		if(enemy[1].Z < 0)
			enemy[1].Dir = 1;
		break;

	}

//enemy3
	

	
	switch(enemy[2].Dir){
	case 2:
		enemy[2].Towards = 90;
		enemy[2].X += 0.07;
		
		if(enemy[2].X > 5)
			enemy[2].Dir = 3; 
		break;
	case 1:
		enemy[2].Towards = 0;
		enemy[2].Z += 0.07;
		if(enemy[2].Z > 22)
			enemy[2].Dir = 2;
		break;
	case 3:
		enemy[2].Towards = 0;
		enemy[2].Z += 0.1;
		if(enemy[2].Z > 55)
			enemy[2].Dir = 4;

		break;
	case 4:
		enemy[2].Towards = -90;
		enemy[2].X -= 0.15;
		if(enemy[2].X < -55)
			enemy[2].Dir = 5;
		break;
	case 5:
		enemy[2].Towards = -180;
		enemy[2].Z -= 0.1;
		if(enemy[2].Z < 0)
			enemy[2].Dir = 1;
		break;
	}
		

	//enemy4
	

	
	switch(enemy[3].Dir){
	case 1:
		enemy[3].Towards = 90;
		enemy[3].X += 0.2;
		
		if(enemy[3].X > 5)
			enemy[3].Dir = 2; 
		break;
	case 2:
		enemy[3].Towards = 0;
		enemy[3].Z += 0.05;
		if(enemy[3].Z > 20)
			enemy[3].Dir = 3;
		break;
	case 3:
		enemy[3].Towards = -90;
		enemy[3].X -= 0.1;
		if(enemy[3].X < -55)
			enemy[3].Dir = 4;

		break;
	case 4:
		enemy[3].Towards = 0;
		enemy[3].Z += 0.15;
		if(enemy[3].Z > 50)
			enemy[3].Dir = 5;
		break;
	case 5:
		enemy[3].Towards = 90;
		enemy[3].X += 0.05;
		if(enemy[3].X > 8)
			enemy[3].Dir = 6;
		break;
	case 6:
		enemy[3].Towards = -180;
		enemy[3].Z -= 0.1;
		if(enemy[3].X < 23)
			enemy[3].Dir = 7;
		break;
	case 7:
		enemy[3].Towards = -90;
		enemy[3].X -= 0.2;
		if(enemy[3].X < -43)
			enemy[3].Dir = 8;
		break;
	case 8:
		enemy[3].Towards = -180;
		enemy[3].Z -= 0.1;
		if(enemy[3].Z < 2)
			enemy[3].Dir = 1;
		break;

	}

	//enemy5

	
	switch(enemy[4].Dir){
	case 1:
		enemy[4].Towards = 90;
		enemy[4].X += 0.1;
		
		if(enemy[4].X > 50)
			enemy[4].Dir = 2; 
		break;
	case 2:
		enemy[4].Towards = -90;
		enemy[4].X -= 0.13;
		if(enemy[4].X < -55)
			enemy[4].Dir = 1;
		break;

	}


	//enemy6
	

	
	switch(enemy[5].Dir){
	case 1:
		enemy[5].Towards = 0;
		enemy[5].Z += 0.08;
		
		if(enemy[5].Z > 68)
			enemy[5].Dir = 2; 
		break;
	case 2:
		enemy[5].Towards = -180;
		enemy[5].Z -= 0.16;
		if(enemy[5].Z < 2)
			enemy[5].Dir = 1;
		break;

	}
	*/

		
		



		numtriangles = sector1.numtriangles;
		
		// Process Each Triangle
		for (int loop_m = 0; loop_m < numtriangles; loop_m++)
		{
			int wi;
			if(loop_m == 0)
				wi = 0;
			else if(loop_m == 1)
				wi = 1;
			else if(loop_m > 5 && loop_m < 10)
				wi = 4;
			else if(loop_m > 9 && loop_m < 14)
				wi = 5;
			else if(loop_m > 13 && loop_m < 18)
				wi = 6;
			else if(loop_m > 17 && loop_m < 22)
				wi = 7;
			else if(loop_m >= 22)
				wi = 3;
			else wi = 2;
			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT);
			glBindTexture(GL_TEXTURE_2D, tietu[wi]);
			glBegin(GL_QUADS);

			x_m = sector1.triangle[loop_m].vertex[0].x;
			y_m = sector1.triangle[loop_m].vertex[0].y;
			z_m = sector1.triangle[loop_m].vertex[0].z;
			u_m = sector1.triangle[loop_m].vertex[0].u;
			v_m = sector1.triangle[loop_m].vertex[0].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
			
			x_m = sector1.triangle[loop_m].vertex[1].x;
			y_m = sector1.triangle[loop_m].vertex[1].y;
			z_m = sector1.triangle[loop_m].vertex[1].z;
			u_m = sector1.triangle[loop_m].vertex[1].u;
			v_m = sector1.triangle[loop_m].vertex[1].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
			
			x_m = sector1.triangle[loop_m].vertex[2].x;
			y_m = sector1.triangle[loop_m].vertex[2].y;
			z_m = sector1.triangle[loop_m].vertex[2].z;
			u_m = sector1.triangle[loop_m].vertex[2].u;
			v_m = sector1.triangle[loop_m].vertex[2].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);

			x_m = sector1.triangle[loop_m].vertex[3].x;
			y_m = sector1.triangle[loop_m].vertex[3].y;
			z_m = sector1.triangle[loop_m].vertex[3].z;
			u_m = sector1.triangle[loop_m].vertex[3].u;
			v_m = sector1.triangle[loop_m].vertex[3].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
			glPopAttrib();
			
			glEnd();
			//glPrint(240,450,"NeHe Productions");
		}

		
	}


	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}
void mousemove(int lparam){
	xmouseOld = xmouse;
	ymouseOld = ymouse;
	xmouse=LOWORD(lparam);
	ymouse=HIWORD(lparam);


}
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		case WM_MOUSEMOVE:
		{	
			mousemove(lParam);
			return 0;
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
void checkpos(){
					if(xpos < -109)
						xpos = -108.9;
					if(xpos > 9)
						xpos = 8.9;
					if(zpos < -59)
						zpos = -58.9;
					if(zpos > 19)
						zpos = 18.9;
					if(xpos <= -89.5){
						if((zpos >= -55 && zpos <= -40) || (zpos >= -30 && zpos <= -10)){
							if(xpos > -89.8)
								xpos = -89.8;
						
						
						}
						
					}
					if(zpos >= -10.5){
						if((xpos >= -90 && xpos <= -70) || (xpos >= -30 && xpos <= -10)){
							if(zpos < -9.8)
								zpos = -9.8;
						
						}
					
					}
					if(zpos <= -54.5){
						if((xpos >= -90 && xpos <= -80)){
							if(zpos > -55.2)
								zpos = -55.2;
						
						}
					
					}
					if(zpos <= -52.5){
						if((xpos >= -40 && xpos <= -20)){
							if(zpos > -53.2)
								zpos = -53.2;
						
						}
					
					}
					if(xpos >=-20.5){
						if(zpos > -53 && zpos < -40)
							if(xpos < -19.8)
								xpos = -19.8;
					
					}
					if(xpos >= -10.5){
						if(zpos > -30 && zpos < -10)
							if(xpos < -9.7)
								xpos = -9.7;
					}
					if(zpos >= -40.5){
						if((xpos > -90 && xpos < -80) || (xpos > -40 && xpos < -20))
							if(zpos < -39.7)
								zpos = -39.7;
					}
					if(zpos <= -29.5){
						if((xpos > -90 && xpos < -70) || (xpos > -30 && xpos < -10))
							if(zpos > -30.3)
								zpos = -30.3;
					
					}
					if(xpos >= -80.5){
						if(zpos > -55 && zpos < -40)
							if(xpos < -79.7)
								xpos = -79.7;
					}
					if(xpos >= -70.5){
						if(zpos > -30 && zpos < -10)
							if(xpos < -69.7)
								xpos = -69.7;
					}
					if(xpos <= -39.5){
						if(zpos > -53 && zpos < -40)
							if(xpos > -40.3)
								xpos = -40.3;
					
					}
					if(xpos <= -29.5){
						if(zpos > -30 && zpos < -10)
							if(xpos > -30.3)
								xpos = -30.3;
					
					}
					if(zpos <= -31.5 && ybase != 2.0){
						if(xpos > -12 && xpos < -10)
							if(zpos > -32.2)
								zpos = -32.2;
					}
					if(xpos <= -11.5 && ybase != 2.0){
						if(zpos > -32 && zpos < -29)
							if(xpos > -12.2)
								xpos = -12.2;
					}
					if(xpos >= -10.5 && ybase != 2.0){
						if(zpos > -32 && zpos < -30)
							if(xpos < -9.8)
								xpos = -9.8;
					}

}



int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop
	
	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("究极简化版CS（拳击模式）",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	while(!done)									// Loop That Runs While done=FALSE
	{   
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
				
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			/*	if (keys['B'] && !bp)
				{
					bp=TRUE;
					blend=!blend;
					if (!blend)
					{
						glDisable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);
					}
					else
					{
						glEnable(GL_BLEND);
						glDisable(GL_DEPTH_TEST);
					}
				}
				if (!keys['B'])
				{
					bp=FALSE;
				}

				if (keys['F'] && !fp)
				{
					fp=TRUE;
					filter+=1;
					if (filter>2)
					{
						filter=0;
					}
				}
				if (!keys['F'])
				{
					fp=FALSE;
				}*/
				if(gameover)
					done = true;
				
				if(keys['J']){
					
					PlaySound("data/myattack.wav",NULL,SND_ASYNC);
					attacking = true;
					int i;
					for(i = 0; i < 6; i ++)
						if(check_attack(i) && enemy[i].alive){
							enemy[i].HP -= 30;
							if(enemy[i].HP <= 0){
								enemy[i].HP = 0;
								enemy[i].alive = false;
								enemynum --;
							}
							_cprintf("enemy[%d]_HP=%d\n", i,enemy[i].HP);	
						}
				
				}
				if(!keys['J']){
					attacking = false;
					
				
				}
				if(keys[VK_SPACE]){
					if(xpos >= -12 && xpos <= -10 && zpos >= -32 && zpos <= -30)
						ybase = 2.0;
					

					if(ypos <= ybase)
						ypos = ybase;
					if(ypos == ybase){
						
						cg = 0.1f;
					}
					if(ypos >= ybase + 1.9f)
						cg = -0.1f;
		
				
				}
				if(!keys[VK_SPACE]){
					if(!(xpos >= -12 && xpos <= -10 && zpos >= -32 && zpos <= -30))
						ybase = 0.0;
					if(ypos >= ybase + 2.0f){
					
						cg = -0.1f;
					}
					if(ypos <= ybase){
						cg = 0.0f;
					}
					
						
				
				}
				if(keys[VK_CONTROL]){
					
					if(ct){
						ypos = ybase-1.8f;
						ct = false;
					}
						
				
				}
				if(!keys[VK_CONTROL]){
					
					if(!ct){
						ypos = ybase;
						ct = true;
					}
					
				
				}

				if(keys[VK_SHIFT]){
					vspeed = 0.2f;
				}
				if(!keys[VK_SHIFT]){
					vspeed = 0.1f;
				}

				if (keys['W'])
				{
						checkpos();
						xpos -= (float)sin(heading*piover180) * vspeed;
						zpos -= (float)cos(heading*piover180) * vspeed;
					
					
							if (walkbiasangle >= 359.0f)
							{
								walkbiasangle = 0.0f;
							}
							else
							{
								walkbiasangle+= 8.0;
							}
							walkbias = (float)sin(walkbiasangle * piover180)/10.0f;
							
				
				}

				//鼠标
				//glutMouseFunc(mouseCB);
				//glutMotionFunc(mouseMotionCB);
				if (keys['S']){
					
					
						checkpos();

						xpos += (float)sin(heading*piover180) * vspeed;
						zpos += (float)cos(heading*piover180) * vspeed;
				


							if (walkbiasangle <= 1.0f)
							{
								walkbiasangle = 359.0f;
							}
							else
							{
								walkbiasangle-= 8.0;
							}
							walkbias = (float)sin(walkbiasangle * piover180)/10.0f;
						
					
							
					
					
				}
				if (keys['D'])
				{
					heading -= 1.0f;
					//yrot = heading;
					
				}

				if (keys['A'])
				{
					heading += 1.0f;	
					//yrot = heading;
				}

				if (keys[VK_PRIOR])
				{
					if(lookupdown < -85.0f)
						lookupdown = -85.0f;
					if(lookupdown >= -85.0f)
						lookupdown-= 1.0f;
				}

				if (keys[VK_NEXT])
				{	
					if(lookupdown > 85.0f)
						lookupdown = 85.0f;
					if(lookupdown <= 85.0f)
						lookupdown+= 1.0f;
				}

				if (keys[VK_F1])						// Is F1 Being Pressed?
				{
					keys[VK_F1]=FALSE;					// If So Make Key FALSE
					KillGLWindow();						// Kill Our Current Window
					fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
					// Recreate Our OpenGL Window
					if (!CreateGLWindow("究极简化版CS（拳击模式）",640,480,16,fullscreen))
					{
						return 0;						// Quit If Window Was Not Created
					}
				}
				
			}
		}
	}

	// Shutdown
	KillGLWindow();										// Kill The Window
	return (msg.wParam);								// Exit The Program
}
