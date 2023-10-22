//
//
//
namespace X
{
	 

	/*!
	* Xynapse logging class, initialized within the engine.
	* This class is engine-internal.
	* @note Internal Xynapse Engine Class.
	*/
	class CLogManager
	{

	public:

		/*!
		* Class constructor
		*/
		CLogManager();
		/*!
		* Class deconstructor
		*/
		~CLogManager();
		/*!
		* Write data into the log file.
		* @note Accepts multi arguments
		* @param fmt String to log
		* @return void
		*/
		void write(const char *fmt,...);
 
	private:
		/*!
		* Private File Pointer
		*/
		FILE *fp;
		/*!
		* Keeps the name of the file
		*/		
		char f_fileName[256];
		/*!
		* Keeps the timestamp of the class initialization
		* For futher timestamp logging possibilities.
		*/
		int t_Stamp;
		
		

	};
	 
}