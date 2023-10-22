//
namespace X
{

	struct BufferData
	{
 
 
		CVector2 sizeRBO;
		GLuint	 h_RBO;
		GLuint	 h_FBO;
		
		std::string	s_RBO;
		std::string	s_FBO;

		bool valid;
		

 		
	};






	class CBuffersManager
	{

	private:
		std::vector<BufferData> Buffers;





	public:
		CBuffersManager();
		~CBuffersManager();

 

		//Create FBO or RBO or both with specific data
		int Create(const std::string &s_FBOName,const std::string &s_RBOName,CVector2 vRBOsize,GLuint renderbufferComponent);

		//Bind FBO/RBO
		bool Bind(const std::string &s_FBOName,const std::string &s_RBOName,int eventualBufferID);

		//Attach renderbuffer to currently bound FBO
		void AttachRenderbuffer(const std::string &s_RBOName,GLuint attachment);
		
		//Attach texture to currently bound FBO
		void AttachTexture(const std::string &s_FBOName,GLuint attachment, GLuint texturebound,GLuint texturetype);
		void Unbind(const std::string &s_FBOObjectName,const std::string &s_RBOObjectName);


		int BuffersCount();

		// Lookup by FBO name, RBO name or FBO & RBO names
		int Lookup(const std::string &s_FBOObjectName,const std::string &s_RBOObjectName);
		
		CVector2 GetRBOSize(const std::string &s_RBOName);
 
	
		bool CheckFrameBufferStatus();

		/*
		*	gets
		*/

		BufferData *get(int id);

 

		/*
		*	sets
		*/



	};

}