// SkyBox.cpp: implementation of the CSkyBox class.
//
//////////////////////////////////////////////////////////////////////


#include "SkyBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkyBox::CSkyBox()
{
}
CSkyBox::~CSkyBox()
{
}
bool CSkyBox::InitSkyBox(int XLenght, int YLenght,int ZLenght,
				 int CenterX,int CenterY,int CenterZ)
{
	m_XLength=XLenght;
	m_YLength=YLenght;
	m_ZLength=ZLenght;
	m_CenterX=CenterX;
	m_CenterY=CenterY;
	m_CenterZ=CenterZ;

	//Textures\Sky

	texSky[0] = CreateTexture("Textures\\Sky\\skyb10.bmp");
	texSky[1] = CreateTexture("Textures\\Sky\\skyb11.bmp");
	texSky[2] = CreateTexture("Textures\\Sky\\skyb12.bmp");
	texSky[3] = CreateTexture("Textures\\Sky\\skyb13.bmp");
	texSky[4] = CreateTexture("Textures\\Sky\\skyb14.bmp");
	texSky[5] = CreateTexture("Textures\\Sky\\skyb5.bmp");

	return true;
}
void CSkyBox::DrawSkyBox()
{
	int lx,ly,lz,cx,cy,cz;
	lx=m_XLength;
	ly=m_YLength;
	lz=m_ZLength;
	cx=m_CenterX;
	cy=m_CenterY;
	cz=m_CenterZ;

	float btmy=0.004f;
	float btmx=0.004f;
		
	float topy=0.996f;
	float topx=0.996f;
	////////front
 	glBindTexture(GL_TEXTURE_2D, texSky[0]);
   	glEnable(GL_TEXTURE_2D); 
  		glBegin(GL_QUADS);
	            glTexCoord2f(btmx,btmy);
			    glVertex3i(-lx+cx, -ly+cy,   cz);

    	        glTexCoord2f(topx,btmy);
			    glVertex3i(lx+cx, -ly+cy,   cz);

	            glTexCoord2f(topx,topy);
			    glVertex3i(lx+cx, -ly+cy,   lz+cz);

	            glTexCoord2f(btmx,topy);
			    glVertex3i(-lx+cx, -ly+cy,    lz+cz);
            glEnd();
	glDisable(GL_TEXTURE_2D); 
	///     ////////////////sky1
	/////////////////right
	glBindTexture(GL_TEXTURE_2D, texSky[1]);
   	glEnable(GL_TEXTURE_2D); 
  
		glBegin(GL_QUADS);
	            glTexCoord2f(btmx,btmy);
			    glVertex3i(lx+cx, -ly+cy,    cz);

    	        glTexCoord2f(topx,btmy);
			    glVertex3i(lx+cx, ly+cy,    cz);

	            glTexCoord2f(topx,topy);
			    glVertex3i(lx+cx, ly+cy,    lz+cz);

	            glTexCoord2f(btmx,topy);
			    glVertex3i(lx+cx, -ly+cy,    lz+cz);
            glEnd();
	glDisable(GL_TEXTURE_2D); 
	///     ////////////////sky2
	//////////////back
	glBindTexture(GL_TEXTURE_2D, texSky[2]);
   	glEnable(GL_TEXTURE_2D); 
 
		glBegin(GL_QUADS);
	            glTexCoord2f(btmx,btmy);
			    glVertex3i(lx+cx, ly+cy,    cz);

    	        glTexCoord2f(topx,btmy);
			    glVertex3i(-lx+cx, ly+cy,    cz);

	            glTexCoord2f(topx,topy);
			    glVertex3i(-lx+cx, ly+cy,    lz+cz);

	            glTexCoord2f(btmx,topy);
			    glVertex3i(lx+cx, ly+cy,    lz+cz);
            glEnd();
	glDisable(GL_TEXTURE_2D); 
	///     ////////////////sky3
	////////////////left
	glBindTexture(GL_TEXTURE_2D, texSky[3]);
   	glEnable(GL_TEXTURE_2D); 
	       glBegin(GL_QUADS);
	            glTexCoord2f(btmx,btmy);
			    glVertex3i(-lx+cx, ly+cy,    cz);

    	        glTexCoord2f(topx,btmy);
			    glVertex3i(-lx+cx, -ly+cy,    cz);

	            glTexCoord2f(topx,topy);
			    glVertex3i(-lx+cx,  -ly+cy,   lz+cz);

	            glTexCoord2f(btmx,topy);
			    glVertex3i(-lx+cx, ly+cy,    lz+cz);
            glEnd();
	glDisable(GL_TEXTURE_2D); 
	///     ////////////////sky4
	/////////////up
	glBindTexture(GL_TEXTURE_2D, texSky[4]);
   	glEnable(GL_TEXTURE_2D); 

		glBegin(GL_QUADS);
	            glTexCoord2f(btmx,btmy);
			    glVertex3i(-lx+cx, -ly+cy,    lz+cz);

    	        glTexCoord2f(topx,btmy);
			    glVertex3i(lx+cx,-ly+cy,    lz+cz);

	            glTexCoord2f(topx,topy);
			    glVertex3i(lx+cx, ly+cy,    lz+cz);

	            glTexCoord2f(btmx,topy);
			    glVertex3i(-lx+cx, ly+cy,    lz+cz);
            glEnd();
	glDisable(GL_TEXTURE_2D);
  //glEnable(GL_FOG); 
	/////////////////////sky5 bottom
	glBindTexture(GL_TEXTURE_2D, texSky[5]);
   	glEnable(GL_TEXTURE_2D); 

		glBegin(GL_QUADS);
	            glTexCoord2f(0,0);
			    glVertex3i(-lx+cx, -ly+cy,  cz);

    	        glTexCoord2f(1,0);
			    glVertex3i(lx+cx,-ly+cy,    cz);

	            glTexCoord2f(1,1);
			    glVertex3i(lx+cx, ly+cy,    cz);

	            glTexCoord2f(0,1);
			    glVertex3i(-lx+cx, ly+cy,    cz);
            glEnd();
	glDisable(GL_TEXTURE_2D);
}



GLuint CSkyBox::CreateTexture(CString filename )
{

     GLuint      texture;     
	
	_AUX_RGBImageRec *Image;				

    if(Image = auxDIBImageLoadA( (const char*) filename ))
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

   
  		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Image->sizeX,
                              Image->sizeY, GL_RGB,
                              GL_UNSIGNED_BYTE, Image->data);
	}


  
   
  	if(Image)
	{										
		if (Image->data)
			delete Image->data;			
		delete Image;
	}
	return texture;


}



