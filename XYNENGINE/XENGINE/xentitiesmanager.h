namespace X
{


class CEntity
{

public:

	CEntity();
	~CEntity(); 
	CEntity &CEntity::operator=(const CEntity &other);	 
	void Reset();
	

	std::string classname;
	std::string model_name;
	std::string entity_name;
	std::string targetname;
	std::string target;
	std::string lookat;
	CVector3 origin;
	CVector3 color;
	CVector3 angle;
	int fov;
	float radius;
	float weight;
	bool castshadows;
	bool bindable;
	bool switchable;
	bool bCharacter;
	float time;



};






class CEntitiesManager
{

public:
	CEntitiesManager();
	~CEntitiesManager();


	bool parseEntities(char *pEntities);
	void seedEntities();

	CEntity *getEntityClass(const std::string &rClassName);				//Get entity by entity classname
	CEntity *getEntityName(const std::string &rEntityName);				//Get entity by entity entity_name
	CEntity *getEntityTargetName(const std::string &rTargetName);		//Get entity by entity targetname
	CEntity *getEntityTarget(const std::string &rTarget);				//Get entity by entity target

	bool XS_getEntityClass(const std::string &name, CEntity &ret);
	bool XS_getEntityName(const std::string &name, CEntity &ret);
	bool XS_getEntityTargetName(const std::string &name, CEntity &ret);
	bool XS_getEntityTarget(const std::string &name, CEntity &ret);


	
	void AddRef();
	void Release();

private:
	std::vector<CEntity*> pMapEntities;
	int m_refCount;
};


}//ns