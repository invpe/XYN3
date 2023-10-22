#ifndef OPENGLSTATEMANAGER_INCLUDED
#define OPENGLSTATEMANAGER_INCLUDED

namespace X
{  

#define INVALID_STATE_VALUE -0xFF
#define MAX_TEXTURINGUNITS 8

/*!
* Render State enumerators
* @note Render State only
*/
enum ERenderState
{
	LIGHT0=0,
	LIGHT1,
	LIGHT2,
	LIGHT3,
	LIGHT4,
	LIGHT5,
	LIGHT6,
	LIGHT7,
	BFCULLING,
	CULLSIDE,
	FACEORIENTATION,
	SHADEMODE,
	POLYGONMODE,
	BLENDING,
	FOG,
	ZBUFFER,
	ZBUFFERWRITE,
	ZFUNC,
	STENCILBUFFER,
	READBUFFER,
	DRAWBUFFER,
	ALPHATEST,
	LIGHTING,
	SRCBLEND,
	DSTBLEND,
	MULTISAMPLING,
	ALPHA_TO_COVERAGE,
	TEXTURING,
	NORMALIZE,
	DRAWFBOBUFFER,
	CLIPPLANE,
	MATERIAL_SHININESS,
	MATERIAL_SPECULAR,
	LAST_RENDERSTATE
};
/*!
* Hint Type enumerators
*/
enum EHintTypes
{
	NICEST,
	FASTEST

};
/*!
* Textures enumerators
*/
enum ETextureNumbers
{
	XTEXTURE0=GL_TEXTURE0,
	XTEXTURE1=GL_TEXTURE1,
	XTEXTURE2=GL_TEXTURE2,
	XTEXTURE3=GL_TEXTURE3,
	XTEXTURE4=GL_TEXTURE4,
	XTEXTURE5=GL_TEXTURE5,
	XTEXTURE6=GL_TEXTURE6,
	XTEXTURE7=GL_TEXTURE7,
	XTEXTURE8=GL_TEXTURE8,
	XTEXTURE9=GL_TEXTURE9

};

/*!
* Enabled/Disabled boolean renderstate value.
*/ 
enum EBoolState
{
	DISABLED = 0,
	ENABLED  = 1
};
/*!
* Possible states of CULLSIDE.
*/
enum ECullSide
{
	CULLSIDE_FRONT = 0,
	CULLSIDE_BACK,
	CULLSIDE_FRONTANDBACK
};
/*!
* Possible states of FACEORIENTATION
*/
enum EFaceOrientation
{
	FACEORIENTATION_CW = 0,
	FACEORIENTATION_CCW
};
/*!
* Possible states of SHADEMODE
*/
enum EShadeMode
{
	SHADEMODE_FLAT = 0,
	SHADEMODE_GOURAUD
};
/*!
* Possible states of POLYGONMODE
*/
enum EPolygonMode
{
	POLYGONMODE_POINT = 0,
	POLYGONMODE_LINE,
	POLYGONMODE_FILL
};
/*!
* Possible states of SRCBLEND / DSTBLEND
*/
enum EBlendFunc
{
	BLEND_ZERO = 0,
	BLEND_ONE,
	BLEND_SRCCOLOR,
	BLEND_INVSRCCOLOR,
	BLEND_DSTCOLOR,
	BLEND_INVDSTCOLOR,
	BLEND_SRCALPHA,
	BLEND_INVSRCALPHA,
	BLEND_DSTALPHA,
	BLEND_INVDSTALPHA,
	BLEND_SRCALPHASATURATE
};
/*!
* Possible states of ALPHAFUNC / ZFUNC
*/
enum ECmpFunc
{
	CMP_NEVER = 0,
	CMP_LESS,
	CMP_EQUAL,
	CMP_LESSEQUAL,
	CMP_GREATER,
	CMP_NOTEQUAL,
	CMP_GREATEREQUAL,
	CMP_ALWAYS
};
/*!
* Possible states of GENU / GENV
*/
enum ETexGenFunc
{
	TEXGEN_LOCALLINEAR = 0,
	TEXGEN_VIEWLINEAR,
	TEXGEN_SPHEREMAP,
	TEXGEN_NORMALMAP,     // GL_ARB_texture_cube_map
	TEXGEN_REFLECTIONMAP, // GL_ARB_texture_cube_map
};
/*!
* Possible states of READBUFFER / DRAWBUFFER.
*/
enum EColorBufferType
{
	COLORBUFFER_NONE = 0,
	COLORBUFFER_FRONT,
	COLORBUFFER_BACK,
	COLORBUFFER_FRONTANDBACK
};

/*!
* States manager class\n
* Avoids reduntant call in state changes
*/
class CStateManager
{
public:
	/*!
	* Constructor
	*/
	CStateManager();
	/*!
	* Destructor
	*/
	~CStateManager();
	/*!
	* Synchronizes ( resets StateManager's states )
	* @note Called internally when engine initializes.
	*/
	void glSynchronize();
	/*!
	* Sets new Render state, avoiding redundant state changes.
	* @param State to set
	* @param State value to set
	* @return true if new state is not redundant, false if a new state is redundant
	*/
	bool Set(ERenderState eState, int nValue);
	
private:
	/*!
	* Keeps actual RENDERSTATE values
	*/
	int						_pnRenderStates[LAST_RENDERSTATE];
	/*!
	* Keeps actual GL_BLEND Src value
	*/
	GLenum					_glSrcBlend;
	/*!
	* Keeps actual GL_BLEND Dst value
	*/
	GLenum					_glDstBlend;
	/*!
	* Initialization flag, True after glSynchronize() is done
	*/
	mutable bool m_Initialized;

 
}; 



}


#endif
