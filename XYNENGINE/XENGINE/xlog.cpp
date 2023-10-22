//
#include "xmain.h"
  
namespace X
{ 
 

	CLogManager::CLogManager()
	{
		 t_Stamp=GetTickCount();
		 sprintf(f_fileName,"data/logs/xengine_%i.txt",GetTickCount());
		
		  write("<%s:%i %s> Opening logfile \"%s\". ",__FILE__,__LINE__,__FUNCTION__,f_fileName);
		  
	}

	CLogManager::~CLogManager()
	{ 
		 	 write("<%s:%i %s> Closing logfile \"%s\". ",__FILE__,__LINE__,__FUNCTION__,f_fileName);
		 
	}
 

	void CLogManager::write(const char *fmt, ...) 
	{ 
		 
		char text[60000];			
		va_list		ap;					
		if (fmt == NULL)	return;		

		va_start(ap, fmt);				
		vsprintf(text, fmt, ap);		
		va_end(ap);						
	 
		fp=fopen(f_fileName,"a");
		 if(fp)
		 {
			fprintf(fp,"[%i] %s\n",GetTickCount(),text);
			fclose(fp);
		 }
	}
		


}