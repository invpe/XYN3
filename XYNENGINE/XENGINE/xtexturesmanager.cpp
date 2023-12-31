
#include "xmain.h"
 
namespace X
{  
  



STextureData* CTexturesManager::LoadDDS( const char *filename )
{
    STextureData *pDDSImageData;
    DDSURFACEDESC2 ddsd;
    char filecode[4];
    FILE *pFile;
    int factor;
    int bufferSize;

    // Open the file
    pFile = fopen( filename, "rb" );

    if( pFile == NULL )
    {
        char str[255];
         return NULL;
    }

    // Verify the file is a true .dds file
    fread( filecode, 1, 4, pFile );

    if( strncmp( filecode, "DDS ", 4 ) != 0 )
    {
        char str[255];
         fclose( pFile );
        return NULL;
    }

    // Get the surface descriptor
    fread( &ddsd, sizeof(ddsd), 1, pFile );

    pDDSImageData = (STextureData*) malloc(sizeof(STextureData));

    memset( pDDSImageData, 0, sizeof(STextureData) );

    //
    // This .dds loader supports the loading of compressed formats DXT1, DXT3 
    // and DXT5.
    //
    switch( ddsd.ddpfPixelFormat.dwFourCC )
    {
        case FOURCC_DXT1:
            // DXT1's compression ratio is 8:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            factor = 2;
            break;

        case FOURCC_DXT3:
            // DXT3's compression ratio is 4:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            factor = 4;
            break;

        case FOURCC_DXT5:
            // DXT5's compression ratio is 4:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            factor = 4;
         break;



        default:
            char str[255];
             return NULL;
    }

    //
    // How big will the buffer need to be to load all of the pixel data 
    // including mip-maps?
    //

    if( ddsd.dwLinearSize == 0 )
    {
 		 return NULL;
    }

    if( ddsd.dwMipMapCount > 1 )
        bufferSize = ddsd.dwLinearSize * factor;
    else
        bufferSize = ddsd.dwLinearSize;

    pDDSImageData->data = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));

    fread( pDDSImageData->data, 1, bufferSize, pFile );

    // Close the file
    fclose( pFile );

    pDDSImageData->sizeX      = ddsd.dwWidth;
    pDDSImageData->sizeY     = ddsd.dwHeight;
    pDDSImageData->numMipMaps = ddsd.dwMipMapCount;

    if( ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT1 )
        pDDSImageData->channels = 3;
    else
        pDDSImageData->channels = 4;
     return pDDSImageData;
}

 
 
 
STextureData *CTexturesManager::LoadJPG(const char *strFileName)
{
	struct jpeg_decompress_struct cinfo;
	STextureData *pImageData = NULL;
	FILE *pFile;
		
	// Open a file pointer to the jpeg file and check if it was found and opened 
	if((pFile = fopen(strFileName, "rb")) == NULL) 
	{
		// Display an error message saying the file was not found, then return NULL
		Game->Log()->write("<%s:%i %s> %s FNF ",__FILE__,__LINE__,__FUNCTION__,strFileName);
		return NULL;
	}
	
	// Create an error handler
	jpeg_error_mgr jerr;

	// Have our compression info object point to the error handler address
	cinfo.err = jpeg_std_error(&jerr);
	
	// Initialize the decompression object
	jpeg_create_decompress(&cinfo);
	
	// Specify the data source (Our file pointer)	
	jpeg_stdio_src(&cinfo, pFile);
	
	// Allocate the structure that will hold our eventual jpeg data (must free it!)
	pImageData = (STextureData*)malloc(sizeof(STextureData));

	// Decode the jpeg file and fill in the image data structure to pass back
	// Start decoding JPG data

	// Read in the header of the jpeg file
	jpeg_read_header(&cinfo, TRUE);
	
	// Start to decompress the jpeg file with our compression info
	jpeg_start_decompress(&cinfo);

	// Get the image dimensions and channels to read in the pixel data
	
	pImageData->channels = cinfo.num_components;
	pImageData->sizeX    = cinfo.image_width;
	pImageData->sizeY    = cinfo.image_height;

	// Get the row span in bytes for each row
	int rowSpan = cinfo.image_width * cinfo.num_components;
	
	// Allocate memory for the pixel buffer
	pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*rowSpan*pImageData->sizeY));
			
	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[pImageData->sizeY];

	for (int i = 0; i < pImageData->sizeY; i++)
		rowPtr[i] = &(pImageData->data[i * rowSpan]);

	// Now comes the juice of our work, here we extract all the pixel data
	int rowsRead = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		// Read in the current row of pixels and increase the rowsRead count
		rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	}
	
	// Delete the temporary row pointers
	delete [] rowPtr;

	// Finish decompressing the data
	jpeg_finish_decompress(&cinfo);
	
	// This releases all the stored memory for reading and decoding the jpeg
	jpeg_destroy_decompress(&cinfo);
	
	// Close the file pointer that opened the file
	fclose(pFile);




	// Return the jpeg data (remember, you must free this data after you are done)
	return pImageData;
}

 
STextureData *CTexturesManager::LoadTGA(const char *strFileName)
{
	STextureData *pImageData = NULL;			// This stores our important image data
	WORD width = 0, height = 0;			// The dimensions of the image
	byte length = 0;					// The length in bytes to the pixels
	byte imageType = 0;					// The image type (RLE, RGB, Alpha...)
	byte bits = 0;						// The bits per pixel for the image (16, 24, 32)
	FILE *pFile = NULL;					// The file pointer
	int channels = 0;					// The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0;						// The stride (channels * width)
	int i = 0;							// A counter

	// Open a file pointer to the targa file and check if it was found and opened 
	if((pFile = fopen(strFileName, "rb")) == NULL) 
	{
		// Display an error message saying the file was not found, then return NULL

		return NULL;
	}
		
	// Allocate the structure that will hold our eventual image data (must free it!)
	pImageData = (STextureData*)malloc(sizeof(STextureData));

	// Read in the length in bytes from the header to the pixel data
	fread(&length, sizeof(byte), 1, pFile);
	
	// Jump over one byte
	fseek(pFile,1,SEEK_CUR); 

	// Read in the imageType (RLE, RGB, etc...)
	fread(&imageType, sizeof(byte), 1, pFile);
	
	// Skip past general information we don't care about
	fseek(pFile, 9, SEEK_CUR); 

	// Read the width, height and bits per pixel (16, 24 or 32)
	fread(&width,  sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits,   sizeof(byte), 1, pFile);
	
	// Now we move the file pointer to the pixel data
	fseek(pFile, length + 1, SEEK_CUR); 

	// Check if the image is RLE compressed or not
	if(imageType != XTEXTURES_TGA_RGB)
	{
		// Check if the image is a 24 or 32-bit image
		if(bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = bits / 8;
			stride = channels * width;
			pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));

			// Load in all the pixel data line by line
			for(int y = 0; y < height; y++)
			{
				// Store a pointer to the current line of pixels
				unsigned char *pLine = &(pImageData->data[stride * y]);

				// Read in the current line of pixels
				fread(pLine, stride, 1, pFile);
			
				// Go through all of the pixels and swap the B and R values since TGA
				// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
				for(i = 0; i < stride; i += channels)
				{
					int temp     = pLine[i];
					pLine[i]     = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			int r=0, g=0, b=0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 3;
			stride = channels * width;
			pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));

			// Load in all the pixel data pixel by pixel
			for(int i = 0; i < width*height; i++)
			{
				// Read in the current pixel
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				
				// Convert the 16-bit pixel into an RGB
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;
				
				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				pImageData->data[i * 3 + 0] = r;
				pImageData->data[i * 3 + 1] = g;
				pImageData->data[i * 3 + 2] = b;
			}
		}	
		// Else return a NULL for a bad or unsupported pixel format
		else
			return NULL;
	}
	// Else, it must be Run-Length Encoded (RLE)
	else if(imageType != XTEXTURES_TGA_RLE)
	{
		// Create some variables to hold the rleID, current colors read, channels, & stride.
		byte rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));
		byte *pColors = ((byte*)malloc(sizeof(byte)*channels));

		// Load in all the pixel data
		while(i < width*height)
		{
			// Read in the current color count + 1
			fread(&rleID, sizeof(byte), 1, pFile);
			
			// Check if we don't have an encoded string of colors
			if(rleID < 128)
			{
				// Increase the count by 1
				rleID++;

				// Go through and read all the unique colors found
				while(rleID)
				{
					// Read in the current color
					fread(pColors, sizeof(byte) * channels, 1, pFile);

					// Store the current pixel in our image array
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			// Else, let's read in a string of the same character
			else
			{
				// Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

				// Read in the current color, which is the same for a while
				fread(pColors, sizeof(byte) * channels, 1, pFile);

				// Go and read as many pixels as are the same
				while(rleID)
				{
					// Assign the current pixel to the current index in our pixel array
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
				
			}
				
		}
	}

	// Close the file pointer that opened the file
	fclose(pFile);

	// Fill in our STextureData structure to pass back
	pImageData->channels = channels;
	pImageData->sizeX    = width;
	pImageData->sizeY    = height;

	// Return the TGA data (remember, you must free this data after you are done)
	return pImageData;
}


 
bool CTexturesManager::loadTexture(const std::string &s_FileName)
{
	Game->Log()->write("<%s:%i %s> \"%s\"",__FILE__,__LINE__,__FUNCTION__,s_FileName.c_str());
	if(s_FileName.empty()==true)return false;

	XTexture *t_NewTexture=new XTexture;


		//Convert the path to XYNAPSE ENGINE Materials path
		//Last available token would be the correct filename to pass to MaterialsManager
		char s_TextureName[256];	 
		char s_TempTextureName[256];

		sprintf(s_TempTextureName,"%s",s_FileName.c_str());
	 	char *token=NULL;
		token=strtok( s_TempTextureName ,"/");
		while(token != NULL)
		{
		 	sprintf(s_TextureName,"%s",token);
			token=strtok(NULL,"/");
		}


	//Do not duplicate the textures
	if(getTexture(s_TextureName)==NULL)
	{
	/****************************/
	/** TARGA (TGA) SUPPORT    **/
	/****************************/
	if(strstr(s_FileName.c_str(),".tga")||strstr(s_FileName.c_str(),".TGA"))
	{
		STextureData *pImage = NULL;
		pImage = LoadTGA(s_FileName.c_str());
		if(pImage == NULL)								
		return false;
		 
		glGenTextures(1, &t_NewTexture->u_Texture);
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, t_NewTexture->u_Texture);
		 
		int textureType;
		if(pImage->channels == 4)		
			textureType = GL_RGBA;
		else
			textureType = GL_RGB;
		 
		gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX, pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data);
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		 
		t_NewTexture->s_TextureName=s_TextureName;
		t_NewTexture->i_Channels=pImage->channels;
		t_NewTexture->v_TextureSize.x=pImage->sizeX;
		t_NewTexture->v_TextureSize.y=pImage->sizeY;

		if (pImage)										
		{
			if (pImage->data)							
			{
				free(pImage->data);						
			}

			free(pImage);								
		}
	}
	 

 	/****************************/
	/** JPG (JPG  ) SUPPORT    **/
	/****************************/
	if(strstr(s_FileName.c_str(),".jpg")||strstr(s_FileName.c_str(),".JPG"))
	{
 		STextureData *pImage = NULL;
		pImage = LoadJPG(s_FileName.c_str());

		if(pImage == NULL)								
		return false;
		
		glGenTextures(1, &t_NewTexture->u_Texture);
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, t_NewTexture->u_Texture);

		int textureType;
		if(pImage->channels == 4)
			textureType = GL_RGBA;
		else 
			textureType = GL_RGB;
		
		gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX,pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data);
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		t_NewTexture->s_TextureName=s_TextureName;
		t_NewTexture->i_Channels=pImage->channels;
		t_NewTexture->v_TextureSize.x=pImage->sizeX;
		t_NewTexture->v_TextureSize.y=pImage->sizeY;

		if (pImage)										
		{
			if (pImage->data)							
			{
				free(pImage->data);						
			}

			free(pImage);								
		}

	}
	
 	/****************************/
	/** DDS (DDS  ) SUPPORT    **/
	/****************************/
	if(strstr(s_FileName.c_str(),".dds")||strstr(s_FileName.c_str(),".DDS"))
	{

				GLuint g_compressedTextureID;
				STextureData *pDDSImageData = LoadDDS( s_FileName.c_str() );
				if( pDDSImageData != NULL )
				{
					int nHeight     = pDDSImageData->sizeY;	
					int nWidth      = pDDSImageData->sizeX;
					int nNumMipMaps = pDDSImageData->numMipMaps;
					int nBlockSize;

					t_NewTexture->s_TextureName=s_TextureName;
					t_NewTexture->i_Channels=pDDSImageData->channels;
					t_NewTexture->v_TextureSize.x=nWidth;
					t_NewTexture->v_TextureSize.y=nHeight;

					if( pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )     
						nBlockSize = 8;
					else
						nBlockSize = 16;

					glGenTextures( 1, &t_NewTexture->u_Texture );
					glBindTexture( GL_TEXTURE_2D, t_NewTexture->u_Texture );

					//DDS WITH ALPHA
					if(pDDSImageData->channels==4)
					{
						//GL_CLAMP_TO_EDGE
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					}
					else
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					}

					//Trilinear filtering
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

					// select modulate to mix texture with color for shading
					glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
 
					int nSize;
					int nOffset = 0;
					// Load the mip-map levels
					for( int i = 0; i < nNumMipMaps; ++i )
					{
						if( nWidth  == 0 ) nWidth  = 1;
						if( nHeight == 0 ) nHeight = 1;
						nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;
						glCompressedTexImage2DARB( GL_TEXTURE_2D,
												   i,
												   pDDSImageData->format,
												   nWidth,
												   nHeight,
												   0,
												   nSize,
												   pDDSImageData->data + nOffset );
						nOffset += nSize;
						 // Half the image size for the next mip-map level...
						nWidth  = (nWidth  / 2);
						nHeight = (nHeight / 2);
					}
				}
				if( pDDSImageData != NULL )
				{
					if( pDDSImageData->data != NULL )
						free( pDDSImageData->data );

					free( pDDSImageData );
				}
	} 



		
		xTextures.push_back(t_NewTexture);
		Game->Log()->write("<%s:%i %s> Added new texture \"%s\", textures count: %i",__FILE__,__LINE__,__FUNCTION__,t_NewTexture->s_TextureName.c_str(),xTextures.size());
	}
	  
	return true;
}



XTexture *CTexturesManager::getTexture(const std::string &s_TextureName)
{
	int iter;
	for(iter=0;iter<xTextures.size();iter++)
	{		
		if(xTextures[iter]->s_TextureName==s_TextureName)
		{
				return xTextures[iter];
		}
	}
	return NULL;
}



 
GLuint CTexturesManager::createTextureParams(GLint sizex, GLint sizey,GLint magfilter,GLint minfilter)
{
	GLuint textura;
	glGenTextures( 1, &textura );
	glBindTexture( GL_TEXTURE_2D, textura );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,sizex, sizey, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter );		
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glGenerateMipmap(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D, 0);
	
 		
	return textura;
}
 
GLuint CTexturesManager::EmptyTexture(int width,int height)											// Create An Empty Texture
{
	GLuint txtnumber;											// Texture ID
	unsigned int* data;											// Stored Data

	// Create Storage Space For Texture Data (128x128x4)
	data = (unsigned int*)new GLuint[((width * height))];
	ZeroMemory(data,((width * height)));	// Clear Storage Memory

	glGenTextures(1, &txtnumber);								// Create 1 Texture
	glBindTexture(GL_TEXTURE_2D, txtnumber);					// Bind The Texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	delete [] data;												// Release data

	return txtnumber;											// Return The Texture ID
}
 

void CTexturesManager::removeTexture(const std::string &s_TextureName)
{
	Game->Log()->write("<%s:%i %s> \"%s\"",__FILE__,__LINE__,__FUNCTION__,s_TextureName.c_str());
	int iter;
	for(iter=0;iter<xTextures.size();iter++)
	{		
		if(xTextures[iter]->s_TextureName==s_TextureName)
		{
				
				glDeleteTextures(1, &xTextures[iter]->u_Texture);
				xTextures.erase(xTextures.begin()+iter);
				Game->Log()->write("<%s:%i %s> \"%s\" Removed, TexturesManaged: %i",__FILE__,__LINE__,__FUNCTION__,s_TextureName.c_str(),xTextures.size());
		}
	}

}


}
 