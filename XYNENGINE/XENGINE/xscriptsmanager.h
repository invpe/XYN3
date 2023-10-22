//angelscript wrapper

namespace X
{
	class CScriptsManager
	{

	public:
		CScriptsManager();
		~CScriptsManager();

		//
		bool Init();
		bool LoadScript(const std::string &xs_ScriptName,const std::string &xs_ModuleName);
		int Run(const std::string &funcName);

		//GET
		asIScriptEngine *Get();
		asIScriptContext *ctx;

	private:
		asIScriptEngine			*eScripting;
		std::string				xs_ModuleName;
		

	};
}