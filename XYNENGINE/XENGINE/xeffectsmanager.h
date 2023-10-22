#pragma once
namespace X
{

	/*
	*	This describes the XEffect
	*/
	struct XEffect
	{
		std::string s_EffectName;
		std::string	s_FragFile;
		std::string s_VertFile;
		GLuint		t_OutputTexture;
		float		f_InternalTimer;
	};
	

	/*
	*	XEffects Management class
	*/
	class CEffectsManager
	{
		private:
			std::vector<XEffect*> xEffects; 
			std::vector<XEffect*> xEffectsQueued;
			int m_refCount;
		
		public:
			CEffectsManager();
			~CEffectsManager();
			bool loadEffect(const std::string &s_EffectName,const std::string &s_optFragmentShader, const std::string &s_optVertexShader);
			bool loadEffect(const std::string &s_EffectName);
		 	void renderEffects();
			bool addEffect(const std::string &s_EffectName);
			void removeEffect(const std::string &s_EffectName);
			void removeQueuedEffects();
			int getEffectsQueued();
			bool isQueued(const std::string &s_EffectName);
			void Tick();
		//XS
		void AddRef();
		void Release();

	};
}