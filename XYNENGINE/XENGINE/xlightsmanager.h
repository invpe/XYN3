namespace X
{


#define LIGHTS_MAX_DYNAMIC	4		/*This is the max we calculate in shaders per object*/


	/*X Light types*/
	enum LightType
	{
		XLIGHT_NONE=0,
		XLIGHT_OMNI,
		XLIGHT_SPOT,
		XLIGHT_TYPES
	};


	/*Light data structure*/
	struct LightData
	{

	 
	 
	  /*Diffuse term*/   	   
	  CVector4 diffuse;

	  /*Ambient term*/
   	  CVector4 ambient;

	  /*camera space position*/
	  float position_cs[3];


	  /*A'ka 'radius'*/
	  float radius;
	  

   	  float constantAttenuation;
   	  float linearAttenuation;
   	  float quadraticAttenuation;

	  /*Light type*/
	  LightType lightType;

	  /*This is used for sorting algorithm*/
      float distance;	  

	  /*Flags if light needs a pass to render to shadowmap*/
	  bool castShadows;

	  /*Internal ID*/
	  int id;

	  /*Bound flag - we can bind this light to 'any' XYNMODEL/BONE - with this*/  
	  std::string			boundBoneName;
	  std::string			boundActorName;

	  /*Shadowmapping required matrices*/	  
	  CMatrix4x4 m_LightModel;
	  CMatrix4x4 m_LightProjection;
	  CMatrix4x4 m_LightView[6];
	  CMatrix4x4 m_LightModelViewProjection[6];
	  
	  /*This represents actual light position*/
	  CMatrix4x4 m_Model;					
	
		LightData(CVector3 _position, CVector3 _ambient, CVector3 _diffuse, LightType Type, float _radius,bool _castShadows);
		~LightData();
 	
		bool bindTo(std::string s_ActorName,std::string s_BoneName);
		void Unbind();
	
		//// transform world space light position to camera space
		void Process(const float m_View[16]);
	  
		CVector3 getPosition();

	};




	class CLightsManager
	{

	private:
		

		  
		int m_refCount;


	public:
		std::vector<LightData*> Lights;

		CLightsManager();
		~CLightsManager();

		int createLight(const CVector3 &position,const CVector3 &color_ambient,const CVector3 &color_diffuse,LightType l_Type,float radius,bool cast_shadows);
		bool deleteLight(int l_id);
		int calculateLighting(const CVector3 &rPosition,X::CCamera *Camera,X::CFrustum	*Frustum);
		 void setLightRadius(int lightNumber,float radius);
		  void setLightPosition(int lightNumber,const CVector3 &newPosition);
		   void setCastShadows(int lightNumber,bool bCastShadows);
 		int getLightsCount();
		LightData *getLight(int l_id);
		
		void passLightsToShader(X::Shader *Shader);
		void renderLightSources();

		void initializeMaps();
		void processLights();





  
		int XS_createLight(const CVector3 &position,const CVector3 &color_ambient,const CVector3 &color_diffuse,LightType l_Type,float radius,bool cast_shadows);
		bool XS_bindLightToActorBone(int lightId,const std::string &actorName,const std::string &boneName);
		bool XS_deleteLight(int id);
		

		/*Cube shadowmap*/	  
	    GLuint tDepthCubeMap[LIGHTS_MAX_DYNAMIC];
		void AddRef();
		void Release();
	};
	

}


/*
namespace X
{
#define MAX_LIGHTS 1000
 
 typedef struct
 {
   	  float pos[4];   
   	  float diffuse[4];
   	  float ambient[4];   

   	  float constantAttenuation;
   	  float linearAttenuation;
   	  float quadraticAttenuation;
      float brightness;

 } lightdata;



 
 class lighting
 {
 private:
	 lightdata lightP[MAX_LIGHTS];
	 int num_lights;
 public:
 	  lighting();
 	  ~lighting();
 
	   void updateLight(int id,CVector3 position);
 	  char createlight(float x, float y, float z,
 		           float Dr, float Dg, float Db,
 		           float Ar, float Ag, float Ab,
 		           float cA, float lA, float qA);
 	  int dolighting(CVector3 position,X::Camera *Camera,X::CFrustum	*Frustum);
 };
  
}
*/