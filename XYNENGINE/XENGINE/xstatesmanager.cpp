#include "xmain.h"
 

 
namespace X
{  
  

CStateManager::CStateManager()
{
	 
}

CStateManager::~CStateManager()
{
	 
}


bool CStateManager::Set(ERenderState eState, int nValue)
{

	//Redundant call?
	if(_pnRenderStates[eState]==nValue)
	{		
		return true;
	}


	

	//We are clear to change the state, this is not a redundant call :)
	switch(eState)
	{

	case BFCULLING: 
			nValue == DISABLED ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
	break;


	case CULLSIDE:

		switch(nValue)
		{
		case CULLSIDE_FRONT:        glCullFace(GL_FRONT);          break;
		case CULLSIDE_BACK:         glCullFace(GL_BACK);           break;
		case CULLSIDE_FRONTANDBACK: glCullFace(GL_FRONT_AND_BACK); break;
		}

		break;

	case FACEORIENTATION:
		
		switch(nValue)
		{
		case FACEORIENTATION_CCW: glFrontFace(GL_CCW); break;
		case FACEORIENTATION_CW:  glFrontFace(GL_CW);  break;
		}

		break;

	case SHADEMODE:

		switch(nValue)
		{
		case SHADEMODE_FLAT:    glShadeModel(GL_FLAT);   break;
		case SHADEMODE_GOURAUD: glShadeModel(GL_SMOOTH); break;
		}

		break;

	case POLYGONMODE:

		switch(nValue)
		{
		case POLYGONMODE_POINT: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
		case POLYGONMODE_LINE:  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  break;
		case POLYGONMODE_FILL:  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  break;
		}

		break;

	case BLENDING:      nValue == DISABLED ? glDisable(GL_BLEND)       : glEnable(GL_BLEND);      break;
	case FOG:           nValue == DISABLED ? glDisable(GL_FOG)         : glEnable(GL_FOG);        break;
	case ZBUFFER:
		{
			 

		nValue == DISABLED ? glDisable(GL_DEPTH_TEST)  : glEnable(GL_DEPTH_TEST); 
		}
	break;
	case ZBUFFERWRITE:  nValue == DISABLED ? glDepthMask(0)            : glDepthMask(1);          break;

	case ZFUNC:

		switch(nValue)
		{
		case CMP_NEVER:        glDepthFunc(GL_NEVER);    break;
		case CMP_LESS:         glDepthFunc(GL_LESS);     break;
		case CMP_EQUAL:        glDepthFunc(GL_EQUAL);    break;
		case CMP_LESSEQUAL:    glDepthFunc(GL_LEQUAL);   break;
		case CMP_GREATER:      glDepthFunc(GL_GREATER);  break;
		case CMP_NOTEQUAL:     glDepthFunc(GL_NOTEQUAL); break;
		case CMP_GREATEREQUAL: glDepthFunc(GL_GEQUAL);   break;
		case CMP_ALWAYS:       glDepthFunc(GL_ALWAYS);   break;
		}

		break;

	case STENCILBUFFER: nValue == DISABLED ? glDisable(GL_STENCIL_TEST) : glEnable(GL_STENCIL_TEST);

		break;

	case READBUFFER:

		switch(nValue)
		{
		case COLORBUFFER_NONE:         glReadBuffer(GL_NONE);           break;
		case COLORBUFFER_FRONT:        glReadBuffer(GL_FRONT);          break;
		case COLORBUFFER_BACK:         glReadBuffer(GL_BACK);           break;
		case COLORBUFFER_FRONTANDBACK: glReadBuffer(GL_FRONT_AND_BACK); break;
		}

		break;

	case DRAWFBOBUFFER:

		
		glDrawBuffer(nValue);


	break;
	case DRAWBUFFER:

		switch(nValue)
		{
		case COLORBUFFER_NONE:         glDrawBuffer(GL_NONE);           break;
		case COLORBUFFER_FRONT:        glDrawBuffer(GL_FRONT);          break;
		case COLORBUFFER_BACK:         glDrawBuffer(GL_BACK);           break;
		case COLORBUFFER_FRONTANDBACK: glDrawBuffer(GL_FRONT_AND_BACK); break;
		}

		break;

	case ALPHATEST: 
		nValue == DISABLED ? glDisable(GL_ALPHA_TEST) : glEnable(GL_ALPHA_TEST);

		break;

	case LIGHTING: 
		{
		nValue == DISABLED ? glDisable(GL_LIGHTING) : glEnable(GL_LIGHTING);
		
		}
	break;

	case LIGHT0:
	case LIGHT1:
	case LIGHT2:
	case LIGHT3:
	case LIGHT4:
	case LIGHT5:
	case LIGHT6:
	case LIGHT7:

		nValue == DISABLED ? glDisable(GL_LIGHT0 + (eState - LIGHT0)) : glEnable(GL_LIGHT0 + (eState - LIGHT0));

		break;

	case SRCBLEND:

		switch(nValue)
		{
		case BLEND_ZERO:             _glSrcBlend = GL_ZERO;					break;
		case BLEND_ONE:              _glSrcBlend = GL_ONE;					break;
		case BLEND_SRCCOLOR:         _glSrcBlend = GL_SRC_COLOR;			break;
		case BLEND_INVSRCCOLOR:      _glSrcBlend = GL_ONE_MINUS_SRC_COLOR;	break;
		case BLEND_DSTCOLOR:         _glSrcBlend = GL_DST_COLOR;			break;
		case BLEND_INVDSTCOLOR:      _glSrcBlend = GL_ONE_MINUS_DST_COLOR;	break;
		case BLEND_SRCALPHA:         _glSrcBlend = GL_SRC_ALPHA;			break;
		case BLEND_INVSRCALPHA:      _glSrcBlend = GL_ONE_MINUS_SRC_ALPHA;	break;
		case BLEND_DSTALPHA:         _glSrcBlend = GL_DST_ALPHA;			break;
		case BLEND_INVDSTALPHA:      _glSrcBlend = GL_ONE_MINUS_DST_ALPHA;	break;
		case BLEND_SRCALPHASATURATE: _glSrcBlend = GL_SRC_ALPHA_SATURATE;	break;
 		}		
		glBlendFunc(_glSrcBlend, _glDstBlend);

	 
	break;

	case DSTBLEND:

		switch(nValue)
		{
		case BLEND_ZERO:             _glDstBlend = GL_ZERO;					break;
		case BLEND_ONE:              _glDstBlend = GL_ONE;					break;
		case BLEND_SRCCOLOR:         _glDstBlend = GL_SRC_COLOR;			break;
		case BLEND_INVSRCCOLOR:      _glDstBlend = GL_ONE_MINUS_SRC_COLOR;	break;
		case BLEND_DSTCOLOR:         _glDstBlend = GL_DST_COLOR;			break;
		case BLEND_INVDSTCOLOR:      _glDstBlend = GL_ONE_MINUS_DST_COLOR;	break;
		case BLEND_SRCALPHA:         _glDstBlend = GL_SRC_ALPHA;			break;
		case BLEND_INVSRCALPHA:      _glDstBlend = GL_ONE_MINUS_SRC_ALPHA;	break;
		case BLEND_DSTALPHA:         _glDstBlend = GL_DST_ALPHA;			break;
		case BLEND_INVDSTALPHA:      _glDstBlend = GL_ONE_MINUS_DST_ALPHA;	break;
		case BLEND_SRCALPHASATURATE: _glDstBlend = GL_SRC_ALPHA_SATURATE;	break;
	 	}
		glBlendFunc(_glSrcBlend, _glDstBlend);
	 
	break;

	case MULTISAMPLING:
		nValue == DISABLED ? glDisable(GL_MULTISAMPLE_ARB) : glEnable(GL_MULTISAMPLE_ARB);
	break;

	case ALPHA_TO_COVERAGE:
		nValue == DISABLED ? glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB) : glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
	break;

	case TEXTURING:
		{
		nValue == DISABLED ? glDisable(GL_TEXTURE_2D) : glEnable(GL_TEXTURE_2D);
 
		}
	break;
	case NORMALIZE:
		nValue == DISABLED ? glDisable(GL_NORMALIZE) : glEnable(GL_NORMALIZE);
	break;

	case CLIPPLANE:
		nValue == DISABLED ? glDisable(GL_CLIP_PLANE0) : glEnable(GL_CLIP_PLANE0);
	break;


	 

	default:
	break;

	}

	_pnRenderStates[eState]=nValue;
	return true;
}
 

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Synchronize this instance with the current OpenGL state.

void CStateManager::glSynchronize()
{ 

     
	// Renderstates:
	int nStateCount;
	for(nStateCount = 0; nStateCount < LAST_RENDERSTATE; nStateCount++)
	{
		_pnRenderStates[nStateCount] = INVALID_STATE_VALUE;		 
	}
	_glSrcBlend=BLEND_SRCALPHA; 
	_glDstBlend=BLEND_ZERO;

 
}

 





}
// EOF: CStateManager.cpp
