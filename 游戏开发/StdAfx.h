// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__80F53ED8_9FE0_4FAD_B808_F803C23E3BF1__INCLUDED_)
#define AFX_STDAFX_H__80F53ED8_9FE0_4FAD_B808_F803C23E3BF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>	
#include <math.h>
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "SkinPPWTL.h" 
//#pragma comment(lib, "SkinPPWTL.lib")


#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>										// Header File For The GLu32 Library
#include <gl\glaux.h>
#include <gl\glut.h>
//#include <gl\glext.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glut32.lib")	

//#include "StructDef.h"

#define WM_MYMSG1		WM_USER+100  //导入三维模型


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__80F53ED8_9FE0_4FAD_B808_F803C23E3BF1__INCLUDED_)
