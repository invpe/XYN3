namespace X
{


	//Because we can assign few textures to a material
	//This holds the GL_Bind number and two texture names
	//One is a full path to the texture file
	//The second is a 'texturename' that we use to pass
	//to the textures manager when we lookup a texture in it.
	struct tMaterialTexture
	{
		std::string s_FullTexturePath;
		std::string s_TextureName;
		
		int i_ActiveTextureBindNumber;

		tMaterialTexture()
		{
			i_ActiveTextureBindNumber=XTEXTURE6;
		}


	};



	//This is our material structure
	//Filled in while parsing .xmat
	struct tMaterial
	{

		//Unique material name
		std::string	sMaterialName;
		std::string sMaterialShader;
		
		//Shader data
		bool	 bCalculateLighting;		//material requires per-pixel lighting
		bool	 bCalculateShadows;		//material requires shadows to be applied
	
		//Standard lighting data
		CVector4 vAmbient;
		CVector4 vDiffuse;
		CVector4 vSpecular;
		float	 fShininess;
		
		//A list of textures for this material
		std::vector<tMaterialTexture*> Textures;
		 

		//Default constructor
		tMaterial()
		{
			vAmbient=CVector4(1,1,1,1);
			vDiffuse=CVector4(1,1,1,1);
			vSpecular=CVector4(1,1,1,1);
			fShininess=100;		
			sMaterialName.clear();	
			sMaterialShader.clear();
			Textures.clear();
			bCalculateLighting=false;
			bCalculateShadows=false;			
		}


	};




	class CMaterialsManager
	{

		public:
			CMaterialsManager();
			~CMaterialsManager();
			bool loadMaterial(const std::string &sMaterialName);
			tMaterial *getMaterial(const std::string &sMaterialName);
			bool bindMaterial(tMaterial *material,const CMatrix4x4 &m_Model);
			void removeMaterial(const std::string &sMaterialName);
			void unbindMaterial();
			void reloadMaterials();
		private:
			std::vector<tMaterial*> pMaterials; 
			X::Shader *pActiveShader;

	};

	

};