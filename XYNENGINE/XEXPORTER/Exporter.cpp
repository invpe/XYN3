 
#include "Exporter.h"
char logFile[1024];


 
void Exporter::Log(const char *fmt, ...)		
{
	FILE *fp;
    char		text[6000];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments
	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing
	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text
	


	fp=fopen(logFile,"a");
	fprintf(fp,"%s",text);
	fclose(fp);


}


 

Point3 ToD3DCoord(Point3 p)
{
   return Point3(p.x, p.z, p.y);
}
 
///////////////////////////////// Exporter \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This initializes all of our exporter data
/////
///////////////////////////////// Exporter \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

Exporter::Exporter()
{
	m_numMeshes = 0;
	m_lastTexture = 0;
	m_fp = NULL;
	bExportSelected = false;
	m_pInterface = NULL;
}


///////////////////////////////// ~Exporter \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This will deinitialize any needed data for our exporter class
/////
///////////////////////////////// ~Exporter \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

Exporter::~Exporter() 
{

}


///////////////////////////////// ExtCount \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns how many file extensions that we will use
/////
///////////////////////////////// ExtCount \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int Exporter::ExtCount()
{
	// We just have one file extension to support (.GTF)
	return 1;
}


///////////////////////////////// Ext \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the file extension we will use
/////
///////////////////////////////// Ext \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

const TCHAR *Exporter::Ext(int n)
{		
	// GTF is our own file type we created
	return _T("XYN");
}


///////////////////////////////// LongDesc \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a longer description of our exporter
/////
///////////////////////////////// LongDesc \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

const TCHAR *Exporter::LongDesc()
{
	// We just want to return a simple phrase about our exporter
	return _T("Xynapse3 3DSMAX9 Exporter");
}
	

///////////////////////////////// ShortDesc \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a short explanation of our exporter
/////
///////////////////////////////// ShortDesc \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

const TCHAR *Exporter::ShortDesc() 
{			
	// This is just a small explanation of our exporter
	return _T("XYNAPSE3 Exporter");
}


///////////////////////////////// AuthorName \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the author's name of the exporter
/////
///////////////////////////////// AuthorName \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

const TCHAR *Exporter::AuthorName()
{			
	// Since I wrote this exporter I figure I should take credit for it :)
	return _T("invpe Xynapse");
}


///////////////////////////////// CopyrightMessage \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the message of copyright for our exporter
/////
///////////////////////////////// CopyrightMessage \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

const TCHAR *Exporter::CopyrightMessage() 
{	
	// GT owns the rights to this exporter
	return _T("Copyright Xynapse.pl");
}


///////////////////////////////// OtherMessage1 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This apparently isn't used, but need in the exporter class
/////
///////////////////////////////// OtherMessage1 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

const TCHAR *Exporter::OtherMessage1() 
{		
	return _T("");
}


///////////////////////////////// OtherMessage2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This apparently isn't used, but need in the exporter class
/////
///////////////////////////////// OtherMessage2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

const TCHAR *Exporter::OtherMessage2() 
{		
	return _T("");
}


///////////////////////////////// Version \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns our version of the exporter
/////
///////////////////////////////// Version \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

unsigned int Exporter::Version()
{				
	// Apparently we want to return the version number in hundreds (1.00)
	return VERSION_3DSMAX;
}


///////////////////////////////// ShowAbout \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is used if we want to show an about message/box for our exporter
/////
///////////////////////////////// ShowAbout \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Exporter::ShowAbout(HWND hWnd)
{			
	MessageBox(hWnd,"codeshmastah 2011\n\n","Info",MB_OK);
}


///////////////////////////////// SupportsOptions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns true if we support certain options for our exporter
/////
///////////////////////////////// SupportsOptions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

BOOL Exporter::SupportsOptions(int ext, DWORD options)
{
	// We just say that we support all the options for now
	return TRUE;
}


///////////////////////////////// DoExport \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is our main starting function for our exporter
/////
///////////////////////////////// DoExport \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int	Exporter::DoExport(const TCHAR *szFile, ExpInterface *pEI, Interface *pInterface, BOOL bSuppressPrompts, DWORD options)
{
	sprintf(logFile,"%s.txt",szFile);
	DeleteFile(logFile);
 


	
	Log("+ Export started\n");

	// This tells us if the user is exporting the whole scene or not
	bExportSelected = (options & SCENE_EXPORT_SELECTED) ? TRUE : FALSE;

	// Here we store a pointer to the main interface in our exporter class
	m_pInterface = pInterface;
	
	// This tell us how many children the root node has for traversing
	int numChildren = pInterface->GetRootNode()->NumberOfChildren();

 
	// Now we open the file that we are exporting to as a binary file
	m_fp = fopen(szFile, "wb");

	if(!m_fp)
	{
		Log("! Cannot open file!\n");
		return -1;
	}

	 
	// The first thing written to the file is our KEY ("GT3D")
	char cKey[] = XYN_HEADERKEY;
	fwrite(&cKey, sizeof(char), 4, m_fp);
	Log("+ Wrote KEY\n");
	
	// The second thing written to the file is the current exporter version
	float exporterVersion = XYN_VERSION;
	fwrite(&exporterVersion, sizeof(float), 1, m_fp);
	Log("+ Wrote Version\n");


	Log("+ Children found %i\n",numChildren);
	// Go through each child node and count the meshes
	for(int i=0; i < numChildren; i++)
	{
		Log(" - Going children %i\n",i);
		// Check if the user cancelled the export yet or not
		if (pInterface->GetCancel())
			break;
			// Get the mesh count
			GetObjectCount(pInterface->GetRootNode()->GetChildNode(i));
			 
	}

	
	Log("+ Objects: %i\n",m_numMeshes);

	// We should have the number of meshes by now to write to our file
	fwrite(&m_numMeshes, sizeof(int), 1, m_fp);

	// Now we can go through each node and start exporting the data
	for(int i=0; i < numChildren; i++)
	{
		// Check if the user cancelled the export yet or not
		if (pInterface->GetCancel())
			break;

		Log(" + Children %i, processing node %s.\n",i,pInterface->GetRootNode()->GetChildNode(i)->GetName());
		// Process each child node and export the necessary data
		ProcessNode(pInterface->GetRootNode()->GetChildNode(i));
		
		 
	}

	Log("+ Exporter finished\n");
	// Finally, close the file and we are done!
	Log("+ Closing file\n");
	fclose(m_fp);

	// Everything thing went well so return true
	return true;
}



 






















///////////////////////////////// ProcessNode \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This recursively goes through each node gets us to the mesh data
/////
///////////////////////////////// ProcessNode \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool Exporter::ProcessNode(INode *pNode)
{
	// Check if we are exporting selected data only
	if(bExportSelected && pNode->Selected() == FALSE)
	{
		Log("\t+ Export selected mode\n");
		return false;
	}

	// Get the current object data
	ObjectState objState = pNode->EvalWorldState(0);

	// If there is an object and the .obj pointer isn't NULL
	if (objState.obj) 
	{
		// We do a switch on the CLASS_ID for the object
		switch(objState.obj->SuperClassID()) 
		{
			// If the object is a geometric object, export it!
			case GEOMOBJECT_CLASS_ID:			 
				ExportGeometry(pNode); 
			break;
		};
	}

	// If there are more children stored in this node, we need to
	// continue to recurse through them and export their data too.
	for(int i = 0; i < pNode->NumberOfChildren(); i++) 
	{
		Log("\t+ Children %i processing\n",i);
		// Go through and export the child nodes' data as well
		if(!ProcessNode(pNode->GetChildNode(i)))
			return false;
	}

	// We recursed all the children of the current node
	return true;
}


///////////////////////////////// IsMiscGeometry \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns false only if the object is actual geometry
/////
///////////////////////////////// IsMiscGeometry \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool Exporter::IsMiscGeometry(INode *pNode, ObjectState *pObjectState)
{
	// Skip this object if it's just a target
	if (!pObjectState->obj || (pObjectState->obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0)))
		return true;

	// Skip the bone information about the biped
	if(!strncmp(pNode->GetName(), "Bip", 3))
		return true;

	// Skip the bone information about the biped
	if(!strncmp(pNode->GetName(), "Bone", 4))
		return true;
	
	// Skip the bone information about the biped
	if(!strncmp(pNode->GetName(), "bip", 3))
		return true;

	// Skip unneeded tag information about our model
	if(!strncmp(pNode->GetName(), "tag_", 4))
		return true;

	// If we get here, the object is a normal mesh that we can export
	return false;
}


///////////////////////////////// GetObjectCount \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This recurses through the entire world and increase the mesh count
/////
///////////////////////////////// GetObjectCount \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool Exporter::GetObjectCount(INode* pNode)
{
	// Check if we are exporting selected data only
	if(bExportSelected && pNode->Selected() == FALSE)
		return false;

	// Get the current object data
	ObjectState objState = pNode->EvalWorldState(0);

	// If there is an object and the .obj pointer isn't NULL
	if (objState.obj) 
	{
		// We do a switch on the CLASS_ID for the object
		switch(objState.obj->SuperClassID()) 
		{
			// If the object is a geometric object
			case GEOMOBJECT_CLASS_ID:
				
				// First we need to test if the object isn't unneeded info
				if(IsMiscGeometry(pNode, &objState))
					break;

				// If we get here the object must have valid vertex data
				// so we can increase our number of meshes by 1.
				m_numMeshes++;
				break;
		};
	}

	// Recurse the rest of child nodes if needed to check for more meshes
	for(int i = 0; i < pNode->NumberOfChildren(); i++) 
	{
		if(!GetObjectCount(pNode->GetChildNode(i)))
			return false;
	}

	// If we get here we have gone through all of the child nodes
	return true;
}


///////////////////////////////// ExportGeometry \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This starts the exporting process for the current mesh
/////
///////////////////////////////// ExportGeometry \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Exporter::ExportGeometry(INode *pNode)
{
	Log("\t+ Exporting \"%s\"\n",pNode->GetName());
	// Grab the current state of the object depending on the set animation frame
	ObjectState objState = pNode->EvalWorldState(m_pInterface->GetTime());

	// Make sure this isn't a mesh without valid vertex data
	if(IsMiscGeometry(pNode, &objState))
	{
		Log("\t! Mesh without valid vertex data, boiling out\n");
		return;
	}
	Log("\t+ Valid mesh\n");
	
	// This boolean tells us if we converted a tri object that needs to be freed
	bool bDeleteTriObject = false;

	// This stores the pointer to our converted tri object
	TriObject *pTriObject = NULL;

	// Here we check to see if we can convert our mesh to a tri object
	if (objState.obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
	{ 
		// If the object isn't already a tri object this will create a new object
		pTriObject = (TriObject *)objState.obj->ConvertToType(0,Class_ID(TRIOBJ_CLASS_ID, 0));
		
		// If the tri object isn't the same pointer as the original object
		// we need to be sure to free the pointer after we are done exporting.
		if (objState.obj != pTriObject) 
			bDeleteTriObject = true;
	}

	// If there isn't a valid pointer to our mesh we need to return
	if (!pTriObject) 
		return;

	// Clear each of our lists before exporting
	m_vVertices.clear();
	m_vIndices.clear();
	m_vTexCoords.clear();
	m_vTexIndices.clear();
	m_vMaterials.clear();


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	
	// Just like always, make sure our lists are cleared before we handle each mesh
	m_vMaterials.clear();
	m_vBones.clear();
	m_vOrigVertices.clear();

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	

	// From our tri object we can access a pointer to a "Mesh" class that
	// holds the face information we will be exporting.
	Mesh *pMesh = &pTriObject->GetMesh();



 
	//Zapisz nazwe obiektu
	fwrite(pNode->GetName(), sizeof(char), MAX_FILE_NAME, m_fp);
  
	// Now we can extract the vertex and indices to our lists.  We just
	// need to pass in the current node and current mesh pointer.
	Log("\t+ Extracting face data\n");
	ExtractFaceData(pNode, pMesh);


	// Write the name of the object, this is required for engine to rende properly
	// Before we write the face data, let's save the material info first.
	Log("\t+ Extracting materials data\n");
	WriteMaterials(pNode);

	// Now that all the data is stored in our exporter class we'll save it
	Log("\t+ Writing face data\n");
	WriteFaceData();

	// If we had to create a new tri object we need to free it
	if (bDeleteTriObject)
	{
		delete pTriObject;
		pTriObject = NULL;
	}



	Log("+ Looking for PHYSIQUE\n");
 


	// Once all the face data is processed we can move onto the animation.
	// We first need to see if there is any skeletal animation in the mesh
	// before going on.  We do this by searching through the modifier stack
	// and checking for a PHYSIQUE ID.  If we found one then we have animation.

	// Use our function to get a pointer to a physique modifier
	Modifier* pPhysique = FindPhysiqueModifier(pNode);

	// Initially assume there is no animation
	bool bSkeletalAnim = false;

	// If we don't have a valid pointer then there isn't animation
	if(!pPhysique) 
	{
		Log("\t+ Physique not found, no animation.\n");
		
		// Write a FALSE for animation to the file and return from the function
		fwrite(&bSkeletalAnim, sizeof(bool), 1, m_fp);
		return;
	}
	else
	{
		// We have animation!  Let's write a TRUE to the file and continue
		Log("\t+ Got Physique\n");

		//Ensure there is animation:
		Interval animRange = m_pInterface->GetAnimRange();
		int start = animRange.Start() / GetTicksPerFrame();
		int end = animRange.End() / GetTicksPerFrame();

		bSkeletalAnim = true;
		fwrite(&bSkeletalAnim, sizeof(bool), 1, m_fp);

	}

	// First we need to grab all the bone and weight information
	Log("\t+ Writing bone data\n");
	WriteBoneData(pNode, pPhysique);

	// Now that we have the bone data, let's extract the animation frames
	// and matrix information for each frame.
	Log("\t+ Writing animation data\n");
	WriteAnimation();


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


}


///////////////////////////////// ExtractFaceData \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This goes through the mesh class and extracts all the face data
/////
///////////////////////////////// ExtractFaceData \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Exporter::ExtractFaceData(INode *pNode, Mesh *pMesh)
{
	// Here we get the transformation matrix for the current mesh 
	Matrix3 mTransform = pNode->GetObjTMAfterWSM(0);

	// This returns a boolean telling us if our polygon order is normal or not.
	bool bNormal = NormalPolygonOrder(mTransform);

	// This will store our index order for our triangles
	int indexOrder[3];

	// If the polygon order is not normal, reverse it, otherwise do the normal order
	if(!bNormal)
	{
		indexOrder[0] = 2;
		indexOrder[1] = 1;
		indexOrder[2] = 0;
	}
	else
	{
		indexOrder[0] = 0;
		indexOrder[1] = 1;
		indexOrder[2] = 2;
	}
	



	//PIERWSZE CO, ROBIMY LISTE MATERIALOW i KOORDYNATOW TEXTURY
	//TO W CHUJ WAZNE, BO POTEM NA PODSTAWIE COORDYNATOW TEXA DODAJEMY DANY VERTEX LUB NIE
	//DLA VBO NAJSMACZNIEJ JEST JESLI ILOSC VERTOW=ILOSCI TEXCOORDOW
	//ALE MOZE SIE ZDAZYC TAK JAK W PRZYPADKU DRZEWA,
	//ZE KORONA POSIADA 4 KOORDYNATY UV i XXX VERTEXOW
	//WTEDY ZEBYSMY NIE ZROBILI OVERFLOWA PRZY POROWNYWANIU (EXPORT VERTEXA)
	//UZNAJEMY ZE TEN VERTEX KTORY JEST > NIZ TEXCOORD.SIZE JEST UNIKATOWY I DODAJEMY GO
	Log("\t+ Faces: %i\n",pMesh->getNumFaces());
	for(int i = 0; i < pMesh->getNumFaces(); i++) 
	{

		 
	

		// For every face there is three triangles, so loop through each one
		for(int j = 0; j < 3; j++)
		{


 			// Assume the vertex is unique at first
			bool bUnique = true;		
			// Get the current size of the vertex list before we add a new vertice
			int lastIndex = (int)m_vVertices.size();
			// Get the current order of our polygon for the current vertice
			int index = indexOrder[j];	

			//TO TA CZESC KODU ODPOWIADA ZA NORMALE I POPRAWNE UVKI
			
			// If there are UV coordinates for this mesh
			 if(pMesh->numTVerts)
			 {
				// Create a Point3 variable to store the variable.  Apparently
				// the UV coordinates array is of type Point3.
				Point3 vTexCoord;

				// Initially set our boolean to true to assume it's unique
				bUnique = true;

				// Here we extract the material ID for this current face
				int materialID = pMesh->faces[i].getMatID();

				// Go through all of the materials and check if the ID is unique
				for(int c = 0; c < (int)m_vMaterials.size(); c++)
				{
					// Check if the material ID is the current material
					if(m_vMaterials[c] == materialID)
					{
						// The ID is already used so break
						bUnique = false;
						break;
					}
				}

				
				// If we have a unique ID, let's add it to our list
				if(bUnique)m_vMaterials.push_back(materialID);


 

				// Extract the current texture coordinate from the UV list
				// using the current face information.
				vTexCoord = pMesh->tVerts[pMesh->tvFace[i].t[index]];
				// Assume that this texture coordinate is unique
				bUnique = true;		
				// Grab the size of our UV coordinate list
				lastIndex = (int)m_vTexCoords.size();
		 		// Go through all the texture coordinates and see if this
				// one is unique or not. 
				for(int c = 0; c < (int)m_vTexCoords.size(); c++)
				{				
					// Is this texture coordinate already stored?
					if(m_vTexCoords[c].x == vTexCoord.x && m_vTexCoords[c].y == vTexCoord.y)
					{
						// Not a unique texture coordinate, so store c and break.
						bUnique = false;
						lastIndex = c;
						break;
					}
				}

				// Add the correct index for our texture coordinate
				m_vTexIndices.push_back(lastIndex);
				
				// If the UV coordinate is unique... 
				if(bUnique)
				{
					// Go through all of the materials and find the right 
					// material ID in our materials list.
					for(int m = 0; m < m_vMaterials.size(); m++)
					{
						// Check for a match
						if(m_vMaterials[m] == materialID)
						{
							// Assign the ID that is already in our list
							materialID = m;
						}
					}
					// Store our UV coords and the current face's material ID
					CVector3 vTexCoord2 = {vTexCoord.x, vTexCoord.y, materialID + m_lastTexture};
					m_vTexCoords.push_back(vTexCoord2);
				}


			} 
		}
		}

 

	 



 


	//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY
	//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY
	//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY//VERTEXY
	// Now we actually get to go through all the face data and extract data
	for(int i = 0; i < pMesh->getNumFaces(); i++) 
	{
		// For every face there is three triangles, so loop through each one
		for(int j = 0; j < 3; j++)
		{

		// Assume the vertex is unique at first
			bool bUnique = true;		
			// Get the current size of the vertex list before we add a new vertice
			int lastIndex = (int)m_vVertices.size();
			// Get the current order of our polygon for the current vertice
			int index = indexOrder[j];	


			Point3 vVertex;
			vVertex = mTransform * pMesh->verts[pMesh->faces[i].v[index]];
			 bUnique = true;		
			 lastIndex = (int)m_vVertices.size();
		 


		 	// Porownaj ten vertex z innymi
			for(int c = 0; c < (int)m_vVertices.size(); c++)
			{
				//JESLI POZYCJA VERTA JEST TAKA SAMA
				
				 
				if(m_vVertices[c].x == vVertex.x && m_vVertices[c].y == vVertex.z && m_vVertices[c].z == vVertex.y)
				{
			 			//jest juz wertex ktory ma taka sama pozycje x,y,z
						//teraz zobacz czy ten vertex ma te same UV
						//tak, ma te same UV, wiec olewamy go - zapisujemy tylko jego index
						Point3 vTexCoordnew;
						vTexCoordnew = pMesh->tVerts[pMesh->tvFace[i].t[index]]; 
							if(c<m_vTexCoords.size())
						{
							if(vTexCoordnew.x==m_vTexCoords[c].x && vTexCoordnew.y==m_vTexCoords[c].y)
							{
								lastIndex=c;
								bUnique=false;
								 
								break;
							}
						}
						else
						{
							Log("\t+ VertexCount > TexCoords, we assume this one is unique not to overflow\n");
							bUnique=true;
						}
						  
				}
				 
				 
			 
			} 
 

			// Add the current index to our unique vertices array to the indices list
			m_vIndices.push_back(lastIndex);
		 
			// Do a simple swap to change the y and z to be OpenGL friendly.
			// Max has Z pointing up, where OpenGL has Y pointing up.
			float temp = vVertex.y;
			vVertex.y = vVertex.z;
			vVertex.z = temp;

			// Create our own CVector3 variable to be added to our vertex list
			CVector3 vMyVertex = {vVertex.x, vVertex.y, vVertex.z};

			// If the vertex is unique, add it to the list
			if(bUnique)
			{
				m_vVertices.push_back(vMyVertex);
 				m_vOrigVertices.push_back(pMesh->faces[i].v[index]);
				 
			}








 





		}
	}
	 

}


///////////////////////////////// WriteFaceData \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This saves our face data for the current mesh to the file
/////
///////////////////////////////// WriteFaceData \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Exporter::WriteFaceData()
{
	 
	
	// Grab the number of vertices and write them to the file 
	int numVertices = (int)m_vVertices.size();
	Log("\t+ Vertices: %i\n",numVertices);

	fwrite(&numVertices, sizeof(int), 1, m_fp);
	
	// Go through all of our vertices and save them to the file
	for(int i = 0; i < (int)m_vVertices.size(); i++)
	{
		fwrite(&m_vVertices[i], sizeof(CVector3), 1, m_fp);
	}

	// Grab the number of indices and write them to the file
	int numIndices = (int)m_vIndices.size();
	Log("\t+ Indices: %i\n",numIndices);

	fwrite(&numIndices, sizeof(int), 1, m_fp);

	// Go through all of our indices and save them to the file
	for(int i = 0; i < (int)m_vIndices.size(); i++)
	{
		fwrite(&m_vIndices[i], sizeof(int), 1, m_fp);
	}

	// Grab the number of texture coordinates and write them to the file
	int numTexCoords = (int)m_vTexCoords.size();
	fwrite(&numTexCoords, sizeof(int), 1, m_fp);

	Log("\t+ Texcoords: %i\n",numTexCoords);
	// Go through all of our texture coordinates and save them to the file
	for(int i = 0; i < (int)m_vTexCoords.size(); i++)
	{
		fwrite(&m_vTexCoords[i], sizeof(CVector3), 1, m_fp);
	}

	// Grab the number of texture indices and write them to the file
	int numTexIndices = (int)m_vTexIndices.size();
	fwrite(&numTexIndices, sizeof(int), 1, m_fp);
	
	// Go through all of our texture indices and save them to the file
	for(int i = 0; i < (int)m_vTexIndices.size(); i++)
	{
		fwrite(&m_vTexIndices[i], sizeof(int), 1, m_fp);
	}
	 
 
}


///////////////////////////////// NormalPolygonOrder \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This tells us if our polygon order is normal
/////
///////////////////////////////// NormalPolygonOrder \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool Exporter::NormalPolygonOrder(Matrix3 &m)
{
	// Check to see if there was any mirroring on the model that needs adjusting.
	// This technique was found in the Max SDK Knowledgebase.
	return (DotProd( CrossProd(m.GetRow(0), m.GetRow(1) ), m.GetRow(2) ) < 0.0) ? 1:0;
}


///////////////////////////////// WriteMaterials \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This saves all the material names to our file
/////
///////////////////////////////// WriteMaterials \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Exporter::WriteMaterials(INode *pNode)
{
	// Grab the material class pointer from the node
	
	Mtl *pMaterial = pNode->GetMtl();
	

	// If we have valid materials
	if(pMaterial)
	{


		// If there are no sub-materials, just write one texture map
		 if(pMaterial->NumSubMtls() == 0)
		{
			

		 
	
			Log("\t+ Materials: 1/%s\n",pNode->GetMtl()->GetFullName());
 



			//Find out how many materials are applied to this slot
			int numMaterials=0;
			int matType=-1;
			for(int a=0;a<=ID_DP;a++)
			{
				Log("\t+ Checking %i ",a);
				string strMaterial = GetMaterialName(pMaterial,ID_AM+a);
				if(strMaterial.size()!=NULL)
				{
					Log(" FOUND\n");
					numMaterials++;
				}
				else Log(" NOT FOUND\n");
					
			}

			
			//Write down the number of materials in this slot
			Log("\t+ Materials in slot: %i\n",numMaterials);
			fwrite(&numMaterials, sizeof(int), 1, m_fp);




			//Iterate and export each one of them to XYNIII
			for(int a=0;a<=ID_DP;a++)
			{

				 
				string strMaterial = GetMaterialName(pMaterial,ID_AM+a);
				if(strMaterial.size()!=NULL)
				{
					Log("\t+ Exporting: \"%s\" Type: %i\n",strMaterial.c_str(),ID_AM+a);
					matType=ID_AM+a;

					char szMaterial[MAX_FILE_NAME] = {0};
					strncpy(szMaterial, strMaterial.c_str(), MAX_FILE_NAME-1);
					
					fwrite(szMaterial, sizeof(char), MAX_FILE_NAME, m_fp);
					fwrite(&matType,sizeof(int),1,m_fp);

					 
				}
				 

			}






			/*
			// Get the name of the texture-map applied to the mesh
			string strMaterial = GetMaterialName(pMaterial,ID_DI);
			string strMaterialBU = GetMaterialName(pMaterial,ID_BU);
			string strMaterialDIS = GetMaterialName(pMaterial,ID_DP);
			//fwrite(&numMaterials, sizeof(int), 1, m_fp);

			
			
			

			int numMaterials;
			int usingbumpmap;
			float shininess;
			//Nie mamy diffusea.
			if(!strMaterial.size())
			{
				numMaterials=0;
				fwrite(&numMaterials, sizeof(int), 1, m_fp);
				char szMaterial[MAX_FILE_NAME] = {0};
				strncpy(szMaterial, strMaterial.c_str(), MAX_FILE_NAME-1);
				fwrite(szMaterial, sizeof(char), MAX_FILE_NAME, m_fp);
				m_lastTexture+=numMaterials;
			}
			//Mamy diffuse
			else
			{

				//Mamy tez bumpmape
				if(strMaterialBU.size())
				{
				  
			 		numMaterials=2;
					usingbumpmap=1;
					fwrite(&usingbumpmap,sizeof(int),1,m_fp);			//1-using bumpmap

					fwrite(&numMaterials, sizeof(int), 1, m_fp);
					char szMaterial[MAX_FILE_NAME] = {0};
					strncpy(szMaterial, strMaterial.c_str(), MAX_FILE_NAME-1);
					fwrite(szMaterial, sizeof(char), MAX_FILE_NAME, m_fp);
					strncpy(szMaterial, strMaterialBU.c_str(), MAX_FILE_NAME-1);
					fwrite(szMaterial, sizeof(char), MAX_FILE_NAME, m_fp);
					m_lastTexture+=numMaterials;
				}
				else
				{
					shininess=0;
					numMaterials=1;
					usingbumpmap=0;	
					
			 		fwrite(&usingbumpmap,sizeof(int),1,m_fp);			//1-using bumpmap
					fwrite(&numMaterials, sizeof(int), 1, m_fp);
					char szMaterial[MAX_FILE_NAME] = {0};
					strncpy(szMaterial, strMaterial.c_str(), MAX_FILE_NAME-1);
					fwrite(szMaterial, sizeof(char), MAX_FILE_NAME, m_fp);	
					m_lastTexture+=numMaterials;
				}

			}

			 
  

  */

 


 
			
		}
		else
		{
			// Grab the number of materials we found earlier and save them
			int numMaterials = (int)m_vMaterials.size();
			fwrite(&numMaterials, sizeof(int), 1, m_fp);

			Log("\t+ NumSubMtls=0 Materials: %i\n",numMaterials);
			// Go through all of the materials and extract their names
			for(int i = 0; i < numMaterials; i++)
			{
				// Get the number of sub-materials for this mesh
				int subMaterials = pMaterial->NumSubMtls();

				// Use the recommended SDK method of obtaining the submaterial
				Mtl *subMaterial = pMaterial->GetSubMtl(m_vMaterials[i] % subMaterials);
				
				// Finally, we can get the sub-material name
				string strMaterial = GetMaterialName(subMaterial,ID_DI);

				// Move the material name over to a fixed buffer and save it
				char szMaterial[MAX_FILE_NAME] = {0};
				strncpy(szMaterial, strMaterial.c_str(), MAX_FILE_NAME-1);
				fwrite(szMaterial, sizeof(char), MAX_FILE_NAME, m_fp);

				// Increase the last texture ID
				m_lastTexture++;
			}
		}
		
	}
	else
	{
		// There are no materials for this mesh so we just save a 0
		int numMaterials = 0;
		fwrite(&numMaterials, sizeof(int), 1, m_fp);
	}
}


///////////////////////////////// GetMaterialName \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This takes a pointer to a material class and returns the texture name
/////
///////////////////////////////// GetMaterialName \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
 

string Exporter::GetMaterialName(Mtl *pMaterial, int ID)
{

//ID - ID_DI - diffus
//	 - ID_BU - bump map

	
	// Grab the diffuse texture pointer
	Texmap* pTexture = pMaterial->GetSubTexmap(ID);

	// Here we make sure there is a valid pointer to our texture, and if so
	// we can check if the class ID is a bitmap (or other types).
	if(pTexture && (pTexture ->ClassID() == Class_ID(BMTEX_CLASS_ID, 0)))
	{
		// Next, we extract the full path of the texture map by casting
		// the texture class to BitmapTex pointer, which has GetMapName().
		char *szFullPath = ((BitmapTex*)pTexture)->GetMapName();

		// We don't want the full path, just the file name so we just store
		// a char pointer to the start of the actual file name.
		char *szFile = strrchr(szFullPath, '\\');

		// If the pointer is invalid, just store the original name
		if(!szFile)
			szFile = szFullPath;
		// Otherwise, move the pointer up one so we don't have the '\' char in the name
		else
			++szFile;

		// Return the texture-map name
		return szFile;
	}
	
	// There is no valid name found, so return NULL.
	return "";
}


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// FindPhysiqueModifier \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This searches the modifier stack and returns a physique pointer
/////
///////////////////////////////// FindPhysiqueModifier \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

Modifier* Exporter::FindPhysiqueModifier(INode* node)
{
	// This function comes from the Max SDK and shows us how to find a
	// pointer to a physique modifier.  In Max there is a modifier stack, 
	// and if the mesh has a physique modifier applied to it, then there 
	// is animation.  We want to verify if there is a physique modifier, then
	// return a pointer to that modifier to extract bone data.
	
	// This gives us a pointer to the object that the node references
	Object* pObject = node->GetObjectRef();
	// If there isn't an object then return a NULL
	if(!pObject) 
		return NULL;

	// Next we check to see if our object can be a derived object.  We need 
	// to access a derived object so that we can get at the modifier stack.
	while(pObject && pObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// It's safe to cast our object into a derived object
		IDerivedObject* pObjectDerived = (IDerivedObject *)(pObject);				
			
		// Create an index into the modifier stack
		int stackIndex = 0;

		// Go through the stack and check the modifiers 
		while(stackIndex < pObjectDerived->NumModifiers())
		{
			// Here we get the modifier pointer to the current modifier
			Modifier* pModifier = pObjectDerived->GetModifier(stackIndex);

			// Now we check to see if this modifier is a physique modifier
			if(pModifier->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
 
				// We found it!  Now let's return it.
				return pModifier;
			}

			// Increase the current stack index to the next modifier
			stackIndex++;
		}

		// Get the original object pointer
		pObject = pObjectDerived->GetObjRef();
	}

	// I guess there is no physique modifier for this mesh
	return NULL;
}



///////////////////////////////// WriteBoneData \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This writes the bone and weight information to the file
/////
///////////////////////////////// WriteBoneData \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Exporter::WriteBoneData(INode *pNode, Modifier *pPhysique)
{
	// In this function we handle special or no weighting to the bone animation.
	// If there is no weighting for the model we just assign 1.0 as the weight.
	// First we need to get a couple pointers to Character Studio classes.
	// Character Studio was originally a character animation plugin that is now 
	// integrated with Max.  After we acquire the appropriate pointers, the next
	// step is to go through all of our unique vertices and extract the weight
	// and bone information.  You will notice that we use the m_vOrigVertices
	// array to index into the vertex interface for getting the bone/weight data.
	// This is because it needs an index that is the same index into the original
	// vertex array.  Max's vertex indices have no correlation to our unique vertex 
	// indices.

	// This line grabs a pointer to the interface attached to the physique plugin.
	// From this pointer we can then get a "context interface" for the current mesh.
	IPhysiqueExport *pPhysiqueExport = (IPhysiqueExport *)pPhysique->GetInterface(I_PHYINTERFACE);

	// From our previous pointer we can grab a context pointer for the current mesh
	// so we can start using it's functions to extract the weight and bone data.
	IPhyContextExport *pContext = (IPhyContextExport *)pPhysiqueExport->GetContextInterface(pNode);

	// With these functions we allow blending for the mesh and convert our
	// vertices to a "Rigid" type.  Rigid means that the vertices are linked to
	// the object node. 
	pContext->AllowBlending(TRUE);
	pContext->ConvertToRigid(TRUE);

	 
	// Now let's go through all of our unique vertices and extract the bone/weight data
	for(int i = 0; i < (int)m_vVertices.size(); i++)
	{

		 
		// By using our original vertices index we can get a vertex interface 
		// pointer.  This way our vertices match up with Max's vertices.
		IPhyVertexExport *pVertexData = pContext->GetVertexInterface(m_vOrigVertices[i]);

		// Here we check if the mesh has blended weights
		if (pVertexData->GetVertexType() & BLENDED_TYPE)
		{
			Log("Vert%i Blended\n",i);
			// This mesh has blended weights, so let's convert it to a new 
			// class to start extracting the bone/weight data.
			IPhyBlendedRigidVertex *pBlendData = (IPhyBlendedRigidVertex *)pVertexData;

			// Since we are extracting blended weight data, we need to
			// see how many weight influences this mesh has.
			int numBlendLinks = pBlendData->GetNumberNodes();

			// Write the number of weight influences to our file
			fwrite(&numBlendLinks, sizeof(int), 1, m_fp);

			// For each blend link we need to go through and write
			// the weight and bone information.
			 
			for(int n = 0; n < numBlendLinks; n++)
			{
				// Grab the current bone pointer
				INode *pBone = pBlendData->GetNode(n);

				// Grab the current weight
				float weight = pBlendData->GetWeight(n);
		
					
				// Just like how we extracted our vertices and such, we
				// only want to store unique bones.

				// Assume this bone is unique
				bool bUnique = true;		

				// Get the size of the current bone list
				int lastIndex = (int)m_vBones.size();

				// Go through all of the bones and check for a repeat
				for(int c = 0; c < (int)m_vBones.size(); c++)
				{
					// If this bone is already added to our list
					if(m_vBones[c] == pBone)
					{
						// This bone is not unique, so save the index and break
						bUnique = false;
						lastIndex = c;
						break;
					}
				}
 
				// If this bone is unique then add it to our list
				if(bUnique)
				{
	
					m_vBones.push_back(pBone);
					 

				}
				
				// Now we can store the bone index and the weight in a CVector2
				CVector2 vWeightInfo = {lastIndex, weight};
				
				// Write the weight info to the file
				fwrite(&vWeightInfo, sizeof(CVector2), 1, m_fp);
			
			} 
		}
		else
		{
			 Log("Vert%i notBlended\n",i);
			// If we get here there is no weight influences, so set the
			// blend links to 1.  Then we write that to the file.  Every
			// weight will just be 1.0 as a default.
			int numBlendLinks = 1;
			fwrite(&numBlendLinks, sizeof(int), 1, m_fp);

			// Convert our vertex data pointer to a IPhyRigidVertex pointer
			IPhyRigidVertex *pNoBlendData = (IPhyRigidVertex *)pVertexData;

			// Get a pointer to the current bone
			INode *pBone = pNoBlendData->GetNode();
			

		 	// Assume this bone is unique
			bool bUnique = true;		

			// Get the size of the current bone list
			int lastIndex = (int)m_vBones.size();

			// Go through all of the bones and check for a repeat
			for(int c = 0; c < (int)m_vBones.size(); c++)
			{
			 	// If this bone is already added to our list
				if(m_vBones[c] == pBone)
				{
					// This bone is not unique, so save the index and break
					bUnique = false;
					lastIndex = c;
					break;
				}
			}

			// If this bone is unique then add it to our list
			if(bUnique)
			{
				m_vBones.push_back(pBone); 

			 
			}




			// Now we can store the bone index and a default weight in a CVector2
			CVector2 vWeightInfo = {lastIndex, 1.0f};

			// Write the weight info to the file
			fwrite(&vWeightInfo, sizeof(CVector2), 1, m_fp);
			 
		}
		
 
		// Free our vertex interface before going on
		 pContext->ReleaseVertexInterface(pVertexData);
	}

		
 
	// Now that we have the unique bones' size we can save it to our file
	int numBones = (int)m_vBones.size();
 	fwrite(&numBones, sizeof(int), 1, m_fp);

	 // Save the bone names (XYNIII)
	for(int b = 0; b < (int)m_vBones.size(); b++)	
	{
		fwrite(m_vBones[b]->GetName(),MAX_BONE_NAME,sizeof(char),m_fp);

		//Save starting bone position
		CVector3 boneStartPosition;
		boneStartPosition.x=m_vBones[b]->GetObjectTM(0).GetTrans().x;
		boneStartPosition.y=m_vBones[b]->GetObjectTM(0).GetTrans().z;
		boneStartPosition.z=m_vBones[b]->GetObjectTM(0).GetTrans().y;

		fwrite(&boneStartPosition,sizeof(CVector3),1,m_fp);
		
		Log("\t + %s Default position <%f,%f,%f>\n",m_vBones[b]->GetName(),m_vBones[b]->GetObjectTM(0).GetTrans().x,m_vBones[b]->GetObjectTM(0).GetTrans().z,m_vBones[b]->GetObjectTM(0).GetTrans().y);
		
	}


	 // Free our context interface and physique pointer
	pPhysiqueExport->ReleaseContextInterface(pContext);
	pPhysique->ReleaseInterface(I_PHYINTERFACE, pPhysiqueExport);
 }


///////////////////////////////// WriteAnimation \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This exporters the matrix information for each frame of animation
/////
///////////////////////////////// WriteAnimation \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Exporter::WriteAnimation()
{
	
	// This is the last function we will add for this tutorial, and the
	// most important one at that.  We first export the frame rate
	// and number of frames of animation.  Finally, we need to go through
	// each frame of animation and export all of the translation and
	// rotation data for each bone.
	
	// First, let's get the interval for the animation
	Interval animRange = m_pInterface->GetAnimRange();

	// Now we can find the start and end frames for animation by dividing
	// the ticks per from by the start and end values in the animation interval.
	int start = animRange.Start() / GetTicksPerFrame();
	int end = animRange.End() / GetTicksPerFrame();

	Log("\t+ Start: %i, End: %i\n",start,end);
	// Let's save the frame rate of the animation to the file
	int frameRate = GetFrameRate();

	
	fwrite(&frameRate, sizeof(int), 1, m_fp);
	Log("\t+ Framerate: %i\n",frameRate);

	// Calculate and write the number of frames
	int numFrames = end - start;
	fwrite(&numFrames, sizeof(int), 1, m_fp);
	 
	Log("\t+ Bones: %i\n",m_vBones.size());

	// This next part can be a bit tricky.  The transform matrices that
	// we get from the bones are going to have world-space modifiers 
	// applied to them (GetObjTMAfterWSM()).  What we need to do is to 
	// obtain the "inverse" of the matrices.  By later multiplying the 
	// inverse of the matrice by the original it reverses any world-space 
	// modifiers and brings us back to object space.

	// To store the matrices' inverse we need to allocate memory to store them
	Matrix3* pMatrixInverses = new Matrix3[m_vBones.size()];

	// Now we want to go through all the bones and obtain the inverses
	// to each one of their transform matrices.
	for(int b = 0; b < (int)m_vBones.size(); b++)	
		pMatrixInverses[b] = Inverse(m_vBones[b]->GetObjTMAfterWSM(0));

	// Finally, we need to go through each frame and extract the animation data
	for(int i = start; i < end; i++)
	{
		// Let's go through each bone and save their transform data
		for(int b = 0; b < (int)m_vBones.size(); b++)
		{
			// First we want to multiply the matrix for the current bone by its 
			// inverse to return it back to object space.  We then pass that matrix
			// into our function RemoveScaling() to get rid of any scaling in our matrix.
			// Notice that we need to reference the current frame of animation by
			// multiplying the frame by GetTicksPerFrame(), not just "i".
			// The result is a transform matrix for the current bone and for the
			// current frame of animation.
			Matrix3 mTransform = RemoveScaling(pMatrixInverses[b] * m_vBones[b]->GetObjTMAfterWSM(i * GetTicksPerFrame()));
			
			// Since Max has the Z axis point up, we want to modify our matrix so that 
			// y and z are swapped.  This is done by creating a simple identity matrix
			// with y and z swapped, then multiplying it by our transform matrix.  Then
			// we swap the second row with the third row in the matrix.
			Matrix3 swapYZ(Point3(1,0,0), Point3(0,0,1), Point3(0,1,0), Point3(0,0,0));
			mTransform = mTransform * swapYZ;
			Matrix3 mSwappedTransform(mTransform.GetRow(0), mTransform.GetRow(2), mTransform.GetRow(1), mTransform.GetRow(3));

			// It's simple to get the translation from the matrix, just call GetTrans().
			// We store that translation in our own CVector3 structure.
			Point3 vTemp = mSwappedTransform.GetTrans();
			CVector3 vTranslation = {vTemp.x, vTemp.y, vTemp.z};
			
			// To get the rotation we pass in our new transform matrix to create 
			// a quaternion.  Just in case, we normalize it and then take the conjugate
			// to come up with the final rotation values.
			Quat q(mSwappedTransform); 
			q.Normalize(); 
			Quat qRotation = q.Conjugate();

			// Finally, we store the quaternion in our our CVector4 class and save
			// the translation and quaternion to our file.
			CVector4 vQuaternion = {qRotation.x, qRotation.y, qRotation.z, qRotation.w};
			fwrite(&vTranslation, sizeof(CVector3), 1, m_fp);
			fwrite(&vQuaternion, sizeof(CVector4), 1, m_fp);
	

			Log("\t+ Bone: %i  \"%s\" Frame: %i <%1.1f %1.1f %1.1f> <%1.1f %1.1f %1.1f>\n",b,m_vBones[b]->GetName(),i,vTranslation.x,vTranslation.y,vTranslation.z,vQuaternion.x,vQuaternion.y,vQuaternion.z,vQuaternion.w);
		}
	}

	// Delete the array of inverses
	delete [] pMatrixInverses;
}


///////////////////////////////// RemoveScaling \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This removes scaling from a matrix
/////
///////////////////////////////// RemoveScaling \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

Matrix3 Exporter::RemoveScaling(Matrix3& mTransform) 
{
	// This function is given in the Max SDK to remove any scaling in
	// a transform matrix that might skew the vertices.  What it does
	// is decompose the matrix into parts (i.e. the rotation, translation, etc.).
	// It then puts the parts back together into a new matrix without scaling.

	AffineParts parts;		// This will hold the parts of the matrix
	Matrix3 mNewMatrix;		// This will hold the final matrix w/o scaling

	// 1) Decompose the original matrix and get rotation and translation
	decomp_affine(mTransform, &parts); 

	// 2) Construct a 3x3 rotation from quaternion parts.q
	parts.q.MakeMatrix(mNewMatrix);

	// 3) Add the translation to the new matrix in row 3 from parts.t
	mNewMatrix.SetRow(3, parts.t);

	// Return the new matrix minus the scaling
	return(mNewMatrix);
}




/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Not so bad was it?  Exporting can be a bit overwhelming, but hopefully this
// tutorial series is able to give you all the basics to get your models in
// your applications/games.
//
// You can test this exporter by using the Loader Part3 tutorial.  You will be
// able to see the character animation using bones.
//
// Just a quick overview of what we did--we added a few functions to export the
// bone and weight information.  First we had to get the weight and bone info,
// then we had to go through each from of animation and save the translation
// and rotation information for every bone.
//
// This ends the basic tutorial series for exporting characters from 3DS Max.
//
//
// ฉ 2000-2006 GameTutorials