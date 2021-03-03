// SkyBox.h: interface for the CSkyBox class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS_
#undef _WINDOWS_
#endif //_WINDOWS_
#include "stdafx.h"
#if !defined(AFX_SKYBOX_H__343D3EC5_8632_44AC_AA26_8F63641F3D68__INCLUDED_)
#define AFX_SKYBOX_H__343D3EC5_8632_44AC_AA26_8F63641F3D68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkyBox  
{
public:
	CSkyBox();
	virtual ~CSkyBox();
	bool InitSkyBox(int XLenght=100, int YLenght=100,int ZLenght=100,
		      int CenterX=0,int CenterY=0,int CenterZ=0);
	void DrawSkyBox();

	GLuint CSkyBox::CreateTexture(CString filename );


	int m_XLength,m_YLength,m_ZLength;
	int m_CenterX,m_CenterY,m_CenterZ;

	unsigned int texSky[6];
};

#endif // !defined(AFX_SKYBOX_H__343D3EC5_8632_44AC_AA26_8F63641F3D68__INCLUDED_)
