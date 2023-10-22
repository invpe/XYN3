#pragma once 
#ifndef _TEXTURES_H
#define _TEXTURES_H 
// Required for DirectX's DDSURFACEDESC2 structure definition
#include <ddraw.h>			
// JpegLib decompressor
#include "systems/jpeglib/jpeglib.h"

namespace X
{   

/*!
* This tells us it's a normal RGB (really BGR) file
* @note TGA specific
*/
#define XTEXTURES_TGA_RGB		 2		
/*!
* This tells us it's a ALPHA file
* @note TGA specific
*/
#define XTEXTURES_TGA_A		 3		 
/*!
* TThis tells us that the targa is Run-Length Encoded (RLE)
* @note TGA specific
*/
#define XTEXTURES_TGA_RLE		10		 
 

/*!
* This structure holds the data required when a Texture is loaded into engine.
* @note When a texture file is parsed, this structure is filled in
*/
struct STextureData
{
	/*!
	* The channels in the image (3 = RGB : 4 = RGBA)
	*/
	int channels;	
	/*!
	* TThe width of the image in pixels
	*/
	int sizeX;				 
	/*!
	* The height of the image in pixels
	*/
	int sizeY;	
	/*!
	* DDS Specific format
	*/
    GLenum   format;
	/*!
	* The number of mipmaps this DDS has
	* @note Supported :
	* @note GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
	* @note GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
	* @note GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
	*
	*/
    int      numMipMaps;
	/*!
	* The image pixel data
	*/
	GLubyte *data;	 
};

 

/*!
* Engine texture definition structure.
* This describes the texture that has been loaded, and pushed onto Engine textures stack.
*/
struct XTexture
{
	/*!
	* This is the unique texture name
	*/
	std::string s_TextureName;
	/*!
	* This is a vector holding texture size
	*/
	CVector2 v_TextureSize;
	/*!
	* This is the GL pointer to the loaded texture
	*/
	GLuint   u_Texture;
	/*!
	* This is the number of channels for the texture
	*/
	int		 i_Channels;
	
};



/*!
* Responsible for loading, removing and reloading the textures in the engine.
* @note Remember that XEngine uses "Unique names" for textures too.\n
* When you load a texture, you pass in the texture file name, later on you
* can get a pointer to the texture structure by passing in the "Unique texture name"
*/
class CTexturesManager
{

public:
	/*!
	* This is to be removed
	*/
	GLuint createTextureParams(GLint sizex, GLint sizey,GLint magfilter,GLint minfilter);
	/*!
	* This is to be removed
	*/
	GLuint EmptyTexture(int width,int height); 
	/*!
	* Loads texture into the engine
	* @param s_FileName - the texture file name
	* @return bool - true if loaded, false if failed to load
	*/
	bool loadTexture(const std::string &s_FileName);
	/*!
	* Returns the texture if exists in the engine
	* @param Unique texture file name
	* @return XTexture structure
	* @note will return NULL if failed.
	*/
	XTexture *getTexture(const std::string &s_TextureName);
	/*!
	* Removes a texture from the Engine
	* @param s_TextureName - the unique texture name
	*/
	void removeTexture(const std::string &s_TextureName);


private:
	/*!
	* This holds all textures loaded through the manager to the engine
	*/
	std::vector<XTexture*> xTextures; 
	/*!
	* Loads DDS texture
	* @param texture file name
	* @return loaded texture DDS_IMAGE_DATA structure
	*/
	STextureData *LoadDDS( const char *filename ); 	
	/*!
	* Loads JPG texture
	* @param texture file name
	* @return loaded texture STextureData structure
	*/
	STextureData *LoadJPG(const char *strFileName);
	/*!
	* Loads TGA texture
	* @param texture file name
	* @return loaded texture STextureData structure
	*/
	STextureData *LoadTGA(const char *strFileName);
	 
};
 


}//namespace
#endif
