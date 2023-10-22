#pragma once	
namespace XModel
{  
#define XYN_VERSION						3.0			// The exporter version
#define XYN_MAX_ANIMATIONS				100
#define XYN_MAX_MODEL_TEXTURES			10		 
#define XYN_MAX_FILE_NAME				25	
#define XYN_MAX_USERPROPERTIES_TEXT		256
#define XYN_MAX_BONES					50  
#define MAX_BONE_NAME					64

//VBO vertices struktura
struct XYN_vbo_vert
{
GLfloat x,y,z;
GLfloat tx,ty;
GLfloat nx,ny,nz;
GLfloat tangent[4];
};
 

/*
*	Bone naming XYN III
*   per object tBoneData structue, holding bone names and actual translation/rotation matrices
*/
struct tBoneData
{
	char boneName[MAX_BONE_NAME];
	CVector3 v_Origin;							//When no animation, and bones are in the model, this is their actual position



	CQuaternion q_ActualRotation;				//When animated, this is bone rotation 
	CVector3 v_ActualTranslation;				//When animated, this is bone translation vector
	CVector3 v_ActualPosition;					//Actual calculated bone position
	int objectID;								//bone to pObject reference id

	CMatrix4x4 getPositionMatrix()
	{
		CMatrix4x4 _Matrix;				
		_Matrix.Translate(v_ActualPosition.x,v_ActualPosition.y,v_ActualPosition.z);		
		return _Matrix;
	}

	 

	CMatrix4x4 getTranslationMatrix()
	{
		CMatrix4x4 _Matrix;
		_Matrix.Translate(v_ActualTranslation.x,v_ActualTranslation.y,v_ActualTranslation.z);		
		return _Matrix;
	}


	CMatrix4x4 getRotationMatrix()
	{

		 
		CMatrix4x4 _Matrix; 
		q_ActualRotation.CreateMatrix(_Matrix);
	 		return _Matrix;
	}


	CMatrix4x4 getMVMatrix()
	{
		CMatrix4x4 _Matrix;
		_Matrix=getTranslationMatrix()*getRotationMatrix();
		return _Matrix;

	}
	
 
};

//Taken from MAX to match the texture types when exporting.
enum XTextureType
{
	 XTEXTURE_AMBIENT=0,
	 XTEXTURE_DIFFUSE,						/*colormap*/
	 XTEXTURE_SPECULAR,
	 XTEXTURE_SHININESS,
	 XTEXTURE_SHININESS_STRENGTH,
	 XTEXTURE_SELFILLUMINATION,
	 XTEXTURE_OPACITY,
	 XTEXTURE_FILTER,
	 XTEXTURE_BUMP,							/*normalmap*/
	 XTEXTURE_REFLECTION,
	 XTEXTURE_REFRACTION,
	 XTEXTURE_DISPLACEMENT,					/*heightmap*/
	 XTEXTURE_COUNT
};


struct tWeightInfo
{
	int numBlendLinks;			// The number of weight influences
	CVector2	*pWeightInfo;	// The list of weights for the current weight influence
};

// This holds a translation and quaternion rotation for our animation
struct tBoneMatrix
{
	
	CVector3 vTranslation;		// The translation vector
	CVector4 qRotation;			// The quaternion rotation vector
};

  
struct animationInfo
{
	std::string sequenceName;
	int start;
	int end;
	int looping;
};

// This stores the number of bones and the translation and rotation list
struct tBoneAnimationFrames
{
	int numBones;							// The number of bones		
	tBoneMatrix *pBoneMatrices;				// The list of translations and rotations
};

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////



// This is our face structure
struct tFace
{
	int vertIndex[3];			// indices for the verts that make up this triangle
	int coordIndex[3];			// indices for the tex coords to texture this face
	int textureID;
	CVector3 normal;
};


// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them.
struct xMaterialInfo
{
	char  strName[255];			// The texture name
	//char  strFile[255];			// The texture file name (If this is set it's a texture map)
	/*BYTE  color[3];				// The color of the object (R, G, B)
	int   texureId;				// The texture ID
	float uTile;				// u tiling of texture  
	float vTile;				// v tiling of texture	
	float uOffset;			    // u offset of texture
	float vOffset;				// v offset of texture*/
	int	type;					// COLOR/NORMAL/HEIGHTMAP
	
} ;


/*
*	XYN:: Single object within MODEL
*/
struct x3DObject 
{
	int  numOfMaterials;								// The number of material per object
	int uvchanges;										//liczy ile bylo zmian uv na jednym vertexie
	unsigned int	m_nVBOVertices;						// Vertex VBO 
	unsigned int	m_nVBOIndices;						// Index 
	GLuint vao;


	XYN_vbo_vert *pVertexy;

	int  numOfVerts;			// The number of verts in the model
	int  numOfFaces;			// The number of faces in the model
	int  numTexVertex;			// The number of texture coordinates
	std::vector<int>  materialID;// The texture ID to use, which is the index into our texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object	
	char strName[255];			// The name of the object
	 
	CVector3  *pVerts;			// The object's vertices
	CVector3  *pNormals;		// The object's normals
	CVector2  *pTexVerts;		// The texture's UV coordinates
	tFace *pFaces;				// The faces information of the object
	
	unsigned int numIndices;
	unsigned short *m_pIndices;
	bool bAnimated;						// This tells us if there is animation or not
	char name[25];						// nazwa modelu z maxa
	int numBones;						// This stores the number of bones for this mesh
	int numFrames;						// The number of frames of animation
	CVector3 size;						// Size of the object itself
	CVector3 pivot;						// pozycja 0,0,0 obiektu
	vector<tWeightInfo> vWeightInfo;	// The list of weight influence data
	vector<tBoneAnimationFrames>   vBoneInfo;		// The bone lists
	std::vector<tBoneData> bonesData;	// The names of the bones and corresponding ID

	
};


/*
*	XYN:: Whole model structure
*/
struct t3DModel 
{
	int numOfObjects;					// The number of objects in the model
	vector<xMaterialInfo*> pMaterials;	// The list of material information (Textures and colors)
	vector<x3DObject> pObject;			// The object list for our model
 	 
	int curranim;
	int animSpeed;						// The frame rate for our animation
	int currentFrame;					// The current frame of the current animation 
	int nextFrame;						// The next frame of animation to interpolate too
	float t;							// The ratio of 0.0f to 1.0f between each key frame
	float lastTime;						// This stores the last time that was stored
	animationInfo animations[XYN_MAX_ANIMATIONS];
	bool animated;
	CVector3 modelSize;
	 
};
 


// This is used when converting all our data from the loader to a model structure.
// It stores the sizes of each of the face-data lists.
struct tMeshInfo
{
	int numVertices;
	int numTexCoords;
	int numIndices;
	int numTexIndices;
	int numMaterials;

};

 



		class XYNLoader
		{

		public:

			// This inits the data members
			XYNLoader();					
			bool ImportXYN(t3DModel *pModel, const std::string &rFullPath);
			


		private:

			// XYN2 animations loader
			bool LoadAnimations(t3DModel *pModel,char *filename);
			
			
			void ComputeNormals(x3DObject *pObject);

			// This reads in the data from the XYN file and stores it in the member variables.
			bool ReadXYNData(t3DModel *pModel);


			// This converts the member variables to our pModel structure
			void ConvertDataStructures(t3DModel *pModel, tMeshInfo meshInfo);

			// This frees memory and closes the file
			void CleanUp();
			
			// Member Variables		

			// The file pointer
			FILE *m_FilePointer;

			CVector3	*m_pVertices;		// The list of vertices
			int			*m_pIndices;		// The list of indices into the vertex list
			CVector3	*m_pTexCoords;		// The texture coordinates
			int			*m_pTexIndices;		// The texture coordinate indices
		};


// This is our model class that we use to load and draw and free the model
class XYN
{

public:
	
	// These are our init and deinit() C++ functions (Constructor/Deconstructor)
	XYN();
	~XYN();
	
 
	// Set the animation by providing the name of it
	int setAnimation(const std::string &animationName);
	int getAnimation(const std::string &animationName);

	// This loads the model from a path and file name
	bool LoadModel(const std::string &strPath, const std::string  &strModel);

	// This loads the textures associated with our model.  
	// The path is where the textures are stored.
	bool LoadMaterials(const std::string &strPath);
	 
	// This frees the model's data
	void DestroyModel();

	// This updates the models current frame of animation, and calls SetCurrentTime().
	void UpdateModel();
	void UpdateModelPhysics();

	// Get methods
	tBoneData *getBone(const char *name);
	t3DModel *getModel();	

 	UINT m_ColorTexture[XYN_MAX_MODEL_TEXTURES];
	UINT m_NormalTexture[XYN_MAX_MODEL_TEXTURES];

	UINT t_Texture[XTEXTURE_COUNT];
	
	void renderModel(const bool &shadowPass);	
	void renderObject(const int &objectNumber, const bool &b_shadowPass);

private:

	
	
	tBoneData *findBone(const char *name);

				  
	void SetCurrentTime();


	void drawModelSimple();
	void drawModelFull();
	void drawObjectSimple(const int &objectNumber);
	void drawObjectFull(const int &objectNumber);

	t3DModel m_Model;			
	vector<string> strTextures;


};
 
}//namespace