#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include "xmain.h" 
			 
namespace XModel
{ 

 




XYN::XYN()
{
	
}

 
XYN::~XYN()
{
	DestroyModel();
}	

 
/*
*  !!!!! BUGGY !!!!!
*/
void XYN::DestroyModel()
{
	Game->Log()->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);
	
	// Go through all the objects in the model
	for(int i = 0; i < m_Model.numOfObjects; i++)
	{

		Game->Log()->write("<%s:%i %s> Removing object %i of %i",__FILE__,__LINE__,__FUNCTION__,i,m_Model.numOfObjects);
		
	 	// Free the faces and vertices
		Game->Log()->write("Removing pFaces");
		if(m_Model.pObject[i].pFaces){		delete [] m_Model.pObject[i].pFaces,m_Model.pObject[i].pFaces=NULL;}
		Game->Log()->write("Removing pVerts");
		if(m_Model.pObject[i].pVerts){		delete [] m_Model.pObject[i].pVerts,m_Model.pObject[i].pVerts=NULL;}
		Game->Log()->write("Removing pNormals");
		if(m_Model.pObject[i].pNormals){		delete [] m_Model.pObject[i].pNormals,m_Model.pObject[i].pNormals=NULL;}
		Game->Log()->write("Removing pTexVerts");
		if(m_Model.pObject[i].pTexVerts){	delete [] m_Model.pObject[i].pTexVerts,m_Model.pObject[i].pTexVerts=NULL;}
		Game->Log()->write("Removing pVertexy");
		if(m_Model.pObject[i].pVertexy){		delete [] m_Model.pObject[i].pVertexy,m_Model.pObject[i].pVertexy=NULL;}
		Game->Log()->write("Removing m_pIndices");
		if(m_Model.pObject[i].m_pIndices){	delete [] m_Model.pObject[i].m_pIndices,m_Model.pObject[i].m_pIndices=NULL;}

		//Remove VBO/IBO
		glDeleteBuffers(1,&m_Model.pObject[i].m_nVBOVertices);
		glDeleteBuffers(1,&m_Model.pObject[i].m_nVBOIndices);

		// Go through all of our weight lists and free the pointers
		for(int w = 0; w < (int)m_Model.pObject[i].vWeightInfo.size(); w++)
		{
			// Check if there is a valid pointer for this weight list
			if(m_Model.pObject[i].vWeightInfo[w].pWeightInfo)
			{
				// Delete the pointer and set it to NULL
				delete [] m_Model.pObject[i].vWeightInfo[w].pWeightInfo;
				m_Model.pObject[i].vWeightInfo[w].pWeightInfo = NULL;
			}
		}

		// Clear our STL list for the weight influences
		m_Model.pObject[i].vWeightInfo.clear();

		// Go through all of the bone data and free the lists
		for(int b = 0; b < (int)m_Model.pObject[i].vBoneInfo.size(); b++)
		{
			// Check if the matrice pointer is valid
			if(m_Model.pObject[i].vBoneInfo[b].pBoneMatrices)
			{
				// Delete and NULL the matrix list
				delete [] m_Model.pObject[i].vBoneInfo[b].pBoneMatrices;
				m_Model.pObject[i].vBoneInfo[b].pBoneMatrices = NULL;
			}
		}

		// Clear the bone list
		m_Model.pObject[i].vBoneInfo.clear();


 
		 
	}
	Game->Log()->write("<%s:%i %s> Finished",__FILE__,__LINE__,__FUNCTION__);
}

tBoneData *XYN::findBone(const char *name)
{
	//Traverse models and find the appropriate bone name
	//Remember: bone names are unique!
	for(int a=0;a< m_Model.numOfObjects; a++)
	{

		for(int b=0;b<m_Model.pObject[a].bonesData.size();b++)
		{
			if(strstr(m_Model.pObject[a].bonesData[b].boneName,name))
			{
				return &m_Model.pObject[a].bonesData[b];
			}
		}

	}

	return NULL;
}

tBoneData *XYN::getBone(const char *name)
{

	tBoneData *boneFound=findBone(name);
	if(boneFound!=NULL)
		return boneFound;
	else return NULL;

} 
///////////////////////////////// LOAD MODEL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This loads our model from the given path and file name
/////
///////////////////////////////// LOAD MODEL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool XYN::LoadModel(const std::string &strPath, const std::string &strModel)
{

	Game->Log()->write("<%s:%i %s> \"%s\\%s\" ",__FILE__,__LINE__,__FUNCTION__,strPath.c_str(),strModel.c_str());
 	// Create a loader class instance
	XYNLoader  loadXYN;

	// Make sure valid model name was passed in
	if(strModel.empty()==true || strPath.empty()==true) 
	{
		Game->Log()->write("<%s:%i %s> Error, path/model name failure.",__FILE__,__LINE__,__FUNCTION__);
 		return false;
	}
 
	
	if(!loadXYN.ImportXYN(&m_Model, strPath+"\\"+strModel))
	{		
 		return false;
	}
	
	
	
	// After all the data is loaded we can go through and load all the textures. 
	if(LoadMaterials(strPath.c_str())==false)return false;

	// The model was loaded correctly so return true
	return true;
}

 

bool XYN::LoadMaterials(const std::string &strPath)
{

	
	Game->Log()->write("<%s:%i %s> From \"%s\".",__FILE__,__LINE__,__FUNCTION__,strPath.c_str());
	// Grab a pointer to the model
	t3DModel *pModel = &m_Model;
	int normalmapct=0;
	strTextures.clear();
 	// Go through all the materials that are assigned to this model
	for(int i = 0; i < (int)pModel->pMaterials.size(); i++)
	{
		
		//Perform material loading		
		if(Game->MaterialsManager()->loadMaterial(std::string(pModel->pMaterials[i]->strName))==true)
		{
				 
		}
		else return false;

	}
	return true;
}


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

///////////////////////////////// SET CURRENT TIME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This sets time t for the interpolation between the current and next key frame
/////
///////////////////////////////// SET CURRENT TIME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void XYN::SetCurrentTime()
{
	 
	static float elapsedTime   = 0.0f;
	elapsedTime += Game->getDelta();
	float oneOverAnimSpeed = 1.0f / (float)m_Model.animSpeed;
	float t = elapsedTime / oneOverAnimSpeed;
	if (elapsedTime >= oneOverAnimSpeed )
	{
		// Set our current frame to the next key frame (which could be the start of the anim)
		m_Model.currentFrame = m_Model.nextFrame;
		m_Model.nextFrame = (m_Model.currentFrame + 1) % m_Model.pObject[0].numFrames;
		t = 0.0f;
		elapsedTime = 0.0f;
	}

	m_Model.t = t;

	  
}



void XYN::UpdateModelPhysics()
{
	/*
	t3DModel *pModel = &m_Model;
	
	//Na podstawie czasu i interpolacji updatenij buffery
 	for(int A=0;A<m_Model.numOfObjects;A++)
	{

		
		x3DObject *pObject = &m_Model.pObject[A];
		


 				glBindBuffer(GL_ARRAY_BUFFER, m_Model.pObject[A].m_nVBOVertices );

				for(int j = 0; j < pObject->numOfFaces; j++)
				{  
						for(int whichVertex = 0; whichVertex < 3; whichVertex++)
						{
							int vertIndex = pObject->pFaces[j].vertIndex[whichVertex];
				 			CVector3 vPos = pObject->pVerts[vertIndex];					
							
							CVector3 vNewPos(0, 0, 0);
							int blendIndex = vertIndex;
							for(int b=0; b < pObject->vWeightInfo[blendIndex].numBlendLinks; b++)
							{

								int boneIndex = (int)pObject->vWeightInfo[blendIndex].pWeightInfo[b].x;
								//if(boneIndex > pObject->numBones)return;

									btTransform trans;
									trans=pObject->bonesData[boneIndex].p_Body->getWorldTransform();
									
									CVector3 p_Vector=CVector3(trans.getOrigin().x(),trans.getOrigin().y(),trans.getOrigin().z());


								tBoneMatrix *pBoneMatrix1 = &(pObject->vBoneInfo[m_Model.currentFrame].pBoneMatrices[boneIndex]);
								CQuaternion	qPos(pBoneMatrix1->qRotation); 
								CVector3 vPosition = pBoneMatrix1->vTranslation;
								CVector3 vTempPos = vPos;
								vTempPos.x += vPosition.x +p_Vector.x;
								vTempPos.y += vPosition.y +p_Vector.y;
								vTempPos.z += vPosition.z +p_Vector.z;
 								float weight = pObject->vWeightInfo[blendIndex].pWeightInfo[b].y;

								vNewPos += vTempPos * weight;		

								 
							 

							}


							
								pObject->pVertexy[vertIndex].x=vNewPos.x;
								pObject->pVertexy[vertIndex].y=vNewPos.y;
								pObject->pVertexy[vertIndex].z=vNewPos.z;

						}
				}

			
	 	glBufferSubData(GL_ARRAY_BUFFER, NULL, sizeof(XYN_vbo_vert)*pObject->numOfVerts, &pObject->pVertexy[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);  			

	}*/
		 
}








void XYN::UpdateModel()
{
 
	//Process if model is animated only
	if(m_Model.animated==true)
	{
				static double elapsedTime   = 0.0f;

				int startFrame = m_Model.animations[m_Model.curranim].start;
				int endFrame   = m_Model.animations[m_Model.curranim].end;
								
				elapsedTime += Game->getDelta() / 1000.0f;	//in seconds

			 	float oneOverAnimSpeed = 1.0f / (float)  m_Model.animSpeed;
				float  t = elapsedTime / oneOverAnimSpeed;
			 
				// sum of delta is > than 1/animation speed, 
				// switch to next keyframe
				if (elapsedTime >= oneOverAnimSpeed )
				{
		 
					//If next frame is the endFrame
					//Check if we're up to be looping
					if(m_Model.nextFrame>=endFrame)
					{
						//If we are to loop, set current frame to animation startFrame
						//and next frame to startFrame+1
						if(m_Model.animations[m_Model.curranim].looping==1)
						{
							m_Model.currentFrame = startFrame;
							m_Model.nextFrame = startFrame + 1 ;
						}
						else
						{
							m_Model.currentFrame=endFrame;
							m_Model.nextFrame=endFrame;
						}


					}
					//Normal continous work,
					//We just increment the frames here.
					else
					{
			
						m_Model.currentFrame++;
						m_Model.nextFrame++;

					}



				 		t = 0.0f;
						elapsedTime = 0.0f;			
				}

				m_Model.t = t;
		 


				//Na podstawie czasu i interpolacji updatenij buffery
 				for(int A=0;A<m_Model.numOfObjects;A++)
				{

		
					x3DObject *pObject = &m_Model.pObject[A];
					if(!pObject->bAnimated)continue;


 							glBindBuffer(GL_ARRAY_BUFFER, m_Model.pObject[A].m_nVBOVertices );

							for(int j = 0; j < pObject->numOfFaces; j++)
							{  
									for(int whichVertex = 0; whichVertex < 3; whichVertex++)
									{
										int vertIndex = pObject->pFaces[j].vertIndex[whichVertex];
				 						CVector3 vPos = pObject->pVerts[vertIndex];					
							
										CVector3 vNewPos(0, 0, 0);
										int blendIndex = vertIndex;
										for(int b=0; b < pObject->vWeightInfo[blendIndex].numBlendLinks; b++)
										{

											int boneIndex = (int)pObject->vWeightInfo[blendIndex].pWeightInfo[b].x;
											//if(boneIndex > pObject->numBones)return;

											tBoneMatrix *pBoneMatrix1 = &(pObject->vBoneInfo[m_Model.currentFrame].pBoneMatrices[boneIndex]);
											tBoneMatrix *pBoneMatrix2 = &(pObject->vBoneInfo[m_Model.nextFrame].pBoneMatrices[boneIndex]);
											CQuaternion	qPos(pBoneMatrix1->qRotation); 
											CQuaternion	qPos2(pBoneMatrix2->qRotation); 
											CQuaternion qNew = qPos.Slerp(qPos, qPos2, m_Model.t);
											CVector3 vTempPos = qNew * vPos;
											CVector3 vPosition = pBoneMatrix1->vTranslation;
											CVector3 vNextPosition = pBoneMatrix2->vTranslation;
											vTempPos.x += vPosition.x + m_Model.t * (vNextPosition.x - vPosition.x);
											vTempPos.y += vPosition.y + m_Model.t * (vNextPosition.y - vPosition.y);
											vTempPos.z += vPosition.z + m_Model.t * (vNextPosition.z - vPosition.z);
 											float weight = pObject->vWeightInfo[blendIndex].pWeightInfo[b].y;
											vNewPos += vTempPos * weight;		

											
											//Update the vectors/quaternions
											pObject->bonesData[boneIndex].v_ActualTranslation=pObject->vBoneInfo[m_Model.currentFrame].pBoneMatrices[boneIndex].vTranslation;
											pObject->bonesData[boneIndex].q_ActualRotation=qPos2;
											pObject->bonesData[boneIndex].v_ActualPosition=vNewPos;


										}


											pObject->pVertexy[vertIndex].x=vNewPos.x;
											pObject->pVertexy[vertIndex].y=vNewPos.y;
											pObject->pVertexy[vertIndex].z=vNewPos.z;

									}
							}

	 				glBufferSubData(GL_ARRAY_BUFFER, NULL, sizeof(XYN_vbo_vert)*pObject->numOfVerts, &pObject->pVertexy[0]);
					glBindBuffer(GL_ARRAY_BUFFER, 0);  			
				 
				}

	}//Animated==1
 
}

int XYN::getAnimation(const std::string &animationName)
{
for(int a=0;a<XYN_MAX_ANIMATIONS;a++)
{
	if( m_Model.animations[a].sequenceName==animationName)
		return a;
}
return -1;
}

//Ustaw animacje po nazwie
int XYN::setAnimation(const std::string &animationName)
{

	for(int a=0;a<XYN_MAX_ANIMATIONS;a++)
	{
 
		if(m_Model.animations[a].sequenceName==animationName)
		{

	  		m_Model.curranim=a;				
			m_Model.currentFrame=m_Model.animations[a].start;
			m_Model.nextFrame=m_Model.currentFrame+1;
		  	return a;

		}
		
	}


return -1;
}




//VBOIBO render
void XYN::drawObjectSimple(const int &objectNumber)
{
	if(objectNumber<m_Model.numOfObjects)
	{
				
	
							glBindBuffer( GL_ARRAY_BUFFER,m_Model.pObject[objectNumber].m_nVBOVertices  );
							glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,m_Model.pObject[objectNumber].m_nVBOIndices);
				  			glEnableClientState(GL_VERTEX_ARRAY);
		 					glVertexPointer( 3, GL_FLOAT,sizeof(XYN_vbo_vert),(char*)0);
							glDrawElements(GL_TRIANGLES,  m_Model.pObject[objectNumber].numIndices,GL_UNSIGNED_SHORT,0);
							glDisableClientState(GL_VERTEX_ARRAY);		
							glBindBuffer( GL_ARRAY_BUFFER, 0);
							glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);


	}

}

//VAO render
void XYN::drawObjectFull(const int &objectNumber)
{
	if(objectNumber<m_Model.numOfObjects)
	{	
 
			 		glBindVertexArray(m_Model.pObject[objectNumber].vao);
				 	glDrawElements(GL_TRIANGLES,  m_Model.pObject[objectNumber].numIndices,GL_UNSIGNED_SHORT,0);					
					glBindVertexArray(0);
 

	}
}	


//VBOIBO render
void XYN::drawModelSimple()
{

		for(int A=0;A<m_Model.numOfObjects;A++)
		{
			

 							glBindBuffer( GL_ARRAY_BUFFER,m_Model.pObject[A].m_nVBOVertices  );
							glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,m_Model.pObject[A].m_nVBOIndices);
				  			glEnableClientState(GL_VERTEX_ARRAY);
		 					glVertexPointer( 3, GL_FLOAT,sizeof(XYN_vbo_vert),(char*)0);
							glDrawElements(GL_TRIANGLES,  m_Model.pObject[A].numIndices,GL_UNSIGNED_SHORT,0);
							glDisableClientState(GL_VERTEX_ARRAY);		
							glBindBuffer( GL_ARRAY_BUFFER, 0);
							glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
	
		}

}

//VAO render
void XYN::drawModelFull()
{
	 int A;
	  for(A=0;A<m_Model.numOfObjects;A++)
	  {


					glBindVertexArray(m_Model.pObject[A].vao);
					glDrawElements(GL_TRIANGLES,  m_Model.pObject[A].numIndices,GL_UNSIGNED_SHORT,0);					
					glBindVertexArray(0);

	}
		
}




void XYN::renderModel(const bool &shadowPass)	
{
	
	  if(shadowPass==true)drawModelSimple();
	  else drawModelFull();

}
 

void XYN::renderObject(const int &objectNumber, const bool &b_shadowPass)
{

	if(objectNumber<m_Model.numOfObjects)
	{
		if(b_shadowPass==true)drawObjectSimple(objectNumber);
		else drawObjectFull(objectNumber);
	}
}

t3DModel *XYN::getModel()
{
	return &m_Model;
}
 
  

 
 



XYNLoader::XYNLoader()
{
	// Set the pointers to null
	m_pVertices=NULL;
	m_pIndices=NULL;
	m_pTexCoords=NULL;
	m_pTexIndices=NULL;	
}

 
void XYNLoader::ComputeNormals(x3DObject *pObject)
{
		Game->Log()->write("<%s:%i %s> Computing for object   \"%s\"",__FILE__,__LINE__,__FUNCTION__, pObject->name);

	 
 
	CVector3 vVector1, vVector2, vNormal, vPoly[3];
 
	 
	
		// Here we allocate all the memory we need to calculate the normals
		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
	 	pObject->pNormals		= new CVector3 [pObject->numOfVerts];
		CVector3 *emptyNormals	= new CVector3 [pObject->numOfVerts];
		 
		// Go though all of the faces of this object
		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			// To cut down LARGE code, we extract the 3 points of this face
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];		 
			 

			// Now let's calculate the face normals (Get 2 vectors and find the cross product of those 2)
			vVector1 = vVector1.Vector(vPoly[0], vPoly[2]);		// Get the vector of the polygon (we just need 2 sides for the normal)
			vVector2 = vVector2.Vector(vPoly[2], vPoly[1]);		// Get a second vector of the polygon
			
			vNormal  = vNormal.Cross(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
			
			pTempNormals[i] = vNormal;							// Save the un-normalized normal for the vertex normals
			
			vNormal.Normalize();				// Normalize the cross product to give us the polygons normal
			
			pNormals[i] = vNormal;						// Assign the normal to the list of normals
			pObject->pFaces[i].normal=pTempNormals[i];
		 
		
		}

			 
		//////////////// Now Get The Vertex Normals /////////////////

		CVector3 vSum;// = {0.0, 0.0, 0.0};
		vSum=CVector3(0,0,0);
		CVector3 vZero = vSum;
		int shared=0;

		for (int i = 0; i < pObject->numOfVerts; i++)			// Go through all of the vertices
		{
 			for (int j = 0; j < pObject->numOfFaces; j++)	// Go through all of the triangles
			{												// Check if the vertex is shared by another face
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{					
					vSum+=pTempNormals[j];
					shared++;								// Increase the number of shared triangles
				}
			}    

	 
						//vSum/=shared;
						pObject->pNormals[i] = vSum*-shared;
						pObject->pNormals[i].Normalize();
						 
						emptyNormals[i]=vSum;		//tutaj trzymamy nieznormalizowane dane o normalu dla vertexa

			vSum = vZero;									// Reset the sum
			shared = 0;										// Reset the shared
		}
	 
		//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!
		//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!
		//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!//!!!!!!!!!!!!!!!!!
		//Naprawiamy bledne normale, tam gdzie verty sa na tej samej pozycji
		//i nie biora one udzialu w wyliczaniu fejsow, ktore je wykorzystuja ( bo doddalismy te verty w
		//exporterze dla vbo)
		 for(int i=0;i<pObject->numOfVerts;i++)
		{ 
			
			int ilosc_tych_samych_pozycji=0;
			GLint *sharedList=new GLint[pObject->numOfVerts];
			CVector3 preNormal = pObject->pNormals[i];

			//Szukamy vertow na tej samej pozycji
			//i sumujemy wszystkie ich wektory normali
			for(int b=0;b<pObject->numOfVerts;b++){
					if(pObject->pVerts[b].x==pObject->pVerts[i].x&&pObject->pVerts[b].y==pObject->pVerts[i].y&&pObject->pVerts[b].z==pObject->pVerts[i].z&&i!=b){
						    sharedList[ilosc_tych_samych_pozycji]=b;							
							preNormal+=pObject->pNormals[b];
							ilosc_tych_samych_pozycji++;
							
					}
			}

			//Wszystkim tym vertom nadajemy jedna usredniona normale
			if(ilosc_tych_samych_pozycji>0)
			{
		 			for(int c=0;c<ilosc_tych_samych_pozycji;c++)
					{
						preNormal/=GLfloat(ilosc_tych_samych_pozycji);
		 				pObject->pNormals[sharedList[c]]=preNormal;
						pObject->pNormals[sharedList[c]].Normalize();

					}
			}
			  	 
			Game->Log()->write("Normal %f %f %f\n",pObject->pNormals[i].x,pObject->pNormals[i].y,pObject->pNormals[i].z);
		} 
	// Free our memory and start over on the next object
		delete [] pTempNormals;
		delete [] pNormals;
	 

 	 
		 



	 Game->Log()->write("<%s:%i %s> Completed",__FILE__,__LINE__,__FUNCTION__);
	 
}


bool XYNLoader::ImportXYN(t3DModel *pModel,  const std::string &rFullPath)
{
	char strMessage[255] = {0};

	Game->Log()->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);
	// Open the XYN file in binary
	m_FilePointer = fopen(rFullPath.c_str(), "rb");

	// Make sure we have a valid file pointer (we found the file)
	if(!m_FilePointer) 
	{
		// Display an error message and don't load anything if no file was found
		Game->Log()->write("<%s:%i %s> Error, model not found.",__FILE__,__LINE__,__FUNCTION__);
		return false;
	}
	
	char fileID[4] = {0};
	float version = 0;

	// Read in the XYN key and exporter version
	fread(&fileID, sizeof(char), 4, m_FilePointer);
	fread(&version, sizeof(float), 1, m_FilePointer);

	// Get the 4 character ID
	char *ID = fileID;

	// Make sure the ID == GT3D and the version greater than or equal t0 1.0
	if((ID[0] != 'X' || ID[1] != 'N' || ID[2] != '3' || ID[3] != 'D') || version !=  XYN_VERSION)
	{
		// Display an error message for bad file format, then stop loading
		Game->Log()->write("<%s:%i %s> Error, header mismatch.",__FILE__,__LINE__,__FUNCTION__);
		return false;
	}

	// Read in the model and animation data
	pModel->animated=0;


	if(ReadXYNData(pModel)==false)
	{
		Game->Log()->write("<%s:%i %s> Discarding loader.",__FILE__,__LINE__,__FUNCTION__);
		return false;
	}
 
	if(pModel->animated==1)
	{
		Game->Log()->write("<%s:%i %s> Loading animation file.",__FILE__,__LINE__,__FUNCTION__);
		char fname[128];
		sprintf(fname,"%s_anim",rFullPath.c_str());		

		if(LoadAnimations(pModel,fname)==false)
		{
			Game->Log()->write("<%s:%i %s> Couldn't load animation file.",__FILE__,__LINE__,__FUNCTION__);
			pModel->animated=0;
		}		
	}
  



	// Clean up after everything
	CleanUp();


	// Return asuccess
	return true;
}

bool XYNLoader::LoadAnimations(t3DModel *pModel,char *filename){
	Game->Log()->write("<%s:%i %s> From \"%s\"",__FILE__,__LINE__,__FUNCTION__,filename);
	FILE *fp;
	fp=fopen(filename,"r");
	if(!fp)
	{
		Game->Log()->write("<%s:%i %s> Error, File not found",__FILE__,__LINE__,__FUNCTION__);
 		return false;
	}
 
	char linia[128];
	char nazwa[32];
	int start,end,fps,looping;
	int ctr=0;
	while(fgets(linia,sizeof(linia),fp) )
	{
		if(strlen(linia)>1 && !strstr(linia,"//"))
		{
					sscanf(linia,"%s %i %i %i",nazwa,&start,&end,&looping);
					pModel->animations[ctr].start=start;
					pModel->animations[ctr].end=end;
					pModel->animations[ctr].looping=looping;
					pModel->animations[ctr].sequenceName=nazwa;

					Game->Log()->write("<%s:%i %s> Loaded \"%s\"",__FILE__,__LINE__,__FUNCTION__,nazwa);
	 				ctr++;
		}
	}
	fclose(fp);
	return true;
}

///////////////////////////////// READ XYN DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in all of the model's data, except the animation frames
/////
///////////////////////////////// READ XYN DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool XYNLoader::ReadXYNData(t3DModel *pModel)
{
	
	Game->Log()->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);

	// Read in the number of objects for this scene
	int numObjects = 0;
	fread(&numObjects, sizeof(int), 1, m_FilePointer);

	Game->Log()->write("<%s:%i %s> Objects inside model: %i",__FILE__,__LINE__,__FUNCTION__,numObjects);
	
	for(int i = 0; i < numObjects; i++)
	{
		
		
		char objectname[XYN_MAX_FILE_NAME] = {0};

		//Nazwa obiektu
		fread(objectname,sizeof(char),XYN_MAX_FILE_NAME,m_FilePointer);
		Game->Log()->write("<%s:%i %s> Reading Object %i - \"%s\"",__FILE__,__LINE__,__FUNCTION__,i,objectname);
		
	 
	 
		// Create a structure to hold the size of the data lists
		tMeshInfo meshInfo = {0};
		// In our loading code we first want to load in all of the materials.
		// First we load the number of materials in the file.
		fread(&meshInfo.numMaterials, sizeof(int), 1, m_FilePointer);
		

		Game->Log()->write("<%s:%i %s> Mats: %i.",__FILE__,__LINE__,__FUNCTION__,meshInfo.numMaterials);

 
	 
		// Go through all of our materials and load them
		for(int m = 0; m < meshInfo.numMaterials; m++)
		{
			// Load in only MAX_FILE_NAME characters for the texture name
			char szMaterial[XYN_MAX_FILE_NAME] = {0};
			fread(szMaterial, sizeof(char), XYN_MAX_FILE_NAME, m_FilePointer);

			// Load the material type ( NOT NEEDED ANYMORE )
			int matType=-1;
			fread(&matType,sizeof(int),1,m_FilePointer);

			// Create a material structure instance and store the material info
			xMaterialInfo *newMaterial=new xMaterialInfo;

			// Remove the material extension (.jpg/.dds/...) to fit the Xynapse .xmat extension
			char sMaterialName[256];	 
	 		char *token=NULL;
			token=strtok( szMaterial ,".");
			strcpy(newMaterial->strName, token);

			// Add a new material
			pModel->pMaterials.push_back(newMaterial);
			Game->Log()->write("<%s:%i %s> Material %i added \"%s\"  Type: %i.",__FILE__,__LINE__,__FUNCTION__,m,szMaterial,matType);

		}
		
		// Read the number of vertices, allocate memory and read them from the file.
		fread(&meshInfo.numVertices, sizeof(int), 1, m_FilePointer);
		m_pVertices  = new CVector3 [meshInfo.numVertices];
		fread(m_pVertices, sizeof(CVector3), meshInfo.numVertices, m_FilePointer);

		// Read the number of indices, allocate memory and read them from the file.
		fread(&meshInfo.numIndices, sizeof(int), 1, m_FilePointer);
		m_pIndices = new int [meshInfo.numIndices];
		fread(m_pIndices, sizeof(int), meshInfo.numIndices, m_FilePointer);		

		// Read the number of UV coordinates, allocate memory and read them from the file.
		fread(&meshInfo.numTexCoords, sizeof(int), 1, m_FilePointer);
		m_pTexCoords = new CVector3 [meshInfo.numTexCoords];
		fread(m_pTexCoords, sizeof(CVector3), meshInfo.numTexCoords, m_FilePointer);

		// Read the number of UV indices, allocate memory and read them from the file.
		fread(&meshInfo.numTexIndices, sizeof(int), 1, m_FilePointer);
		m_pTexIndices = new int [meshInfo.numTexIndices];
		fread(m_pTexIndices, sizeof(int), meshInfo.numTexIndices, m_FilePointer);		

		Game->Log()->write("<%s:%i %s> Vertexes:  %i.",__FILE__,__LINE__,__FUNCTION__,meshInfo.numVertices);
		Game->Log()->write("<%s:%i %s> Indices:  %i.",__FILE__,__LINE__,__FUNCTION__,meshInfo.numIndices);
		Game->Log()->write("<%s:%i %s> TexCoords:  %i.",__FILE__,__LINE__,__FUNCTION__,meshInfo.numTexCoords);
		Game->Log()->write("<%s:%i %s> TexIndices:  %i.",__FILE__,__LINE__,__FUNCTION__,meshInfo.numTexIndices);

		if(!meshInfo.numVertices||!meshInfo.numIndices)
		{

			Game->Log()->write("<%s:%i %s> Error, model does not contain vertex information?.",__FILE__,__LINE__,__FUNCTION__);
			return false;
		}




		ConvertDataStructures(pModel, meshInfo);

		
 
		// and bone data.  First we grab a pointer to our current object.
		x3DObject *pObject = &pModel->pObject[i];
	 	sprintf(pObject->name,"%s",objectname);




	
		//Set material's indexes per object
		int matID;
		for(matID=pModel->pMaterials.size();	matID>pModel->pMaterials.size()-pObject->numOfMaterials;	matID--)
		{
				int matIndex=matID-1;
				pObject->materialID.push_back(matIndex);
		}
	






		// Grab the boolean that tells us if there is animation or not
		fread(&pObject->bAnimated, sizeof(bool), 1, m_FilePointer);
		Game->Log()->write("<%s:%i %s> Animated: %i",__FILE__,__LINE__,__FUNCTION__,pObject->bAnimated);
 
		//Flag the model only if there is any object within that is animated
		if(pObject->bAnimated==true)pModel->animated=pObject->bAnimated;

		//If there is no PHYSIQUE, it means we have no bones information and animation frames
		//Skip reading that data
		if(pObject->bAnimated==true)
		{
 
				
		 	    // We need to go through every vertice and load the corresponding
			    // weight-influence data.  	
				for(int w = 0; w < meshInfo.numVertices; w++)
				{
			
					// Create an instance of our weight info class to hold weights
					tWeightInfo weightInfo;

					// Read in the number of blend links for this vertex and add it to our list
					fread(&weightInfo.numBlendLinks, sizeof(int), 1, m_FilePointer);
					if(weightInfo.numBlendLinks>=0)
					{
					
						pObject->vWeightInfo.push_back(weightInfo);
			 			// Since we are working with pointers, just to be safe we wanted to add 
						// the weight info to our list first, and then grab a pointer to it.
						// We can then use that pointer to allocate memory for a list of all the
						// blend links (weight influences).  Then we will read in the links into
						// our pWeightInfo pointer that is allocated.
						// Grab a pointer to the last weight info in our list (that was just added).
						tWeightInfo *pWeightInfo = &(pObject->vWeightInfo[pObject->vWeightInfo.size() - 1]);
 						// Allocate memory for the weight links.  It's a CVector3 because we store
						// the bone index and the weight value (between 0 and 1).
						pWeightInfo->pWeightInfo = new CVector2 [pWeightInfo->numBlendLinks];
				 
						// Read in all the influences for the weight according to this vertex.
						fread(pWeightInfo->pWeightInfo, sizeof(CVector2), pWeightInfo->numBlendLinks, m_FilePointer);
					 
					}
				}
	
				// Read in the number of bones, animation frame rate and number of anim frames.
				fread(&pObject->numBones, sizeof(int), 1, m_FilePointer);
		
				// Read the names of the bones (XynIII)
				for(int B=0;B<pObject->numBones;B++)
				{
					char bName[MAX_BONE_NAME+10];
					fread(bName,sizeof(char),MAX_BONE_NAME,m_FilePointer);

					//Construct the boneName structure
					tBoneData _bone;
					sprintf(_bone.boneName,"%s",bName);
					_bone.objectID=i;						

					//Get the bone starting position (ORIGIN)
					fread(_bone.v_Origin,sizeof(CVector3),1,m_FilePointer);

					//We set this vector to origin in case there is physique but there is no animation
					//for bone getMatrix.. methods to return us the proper position
					//if no animation is made for a bone.
					_bone.v_ActualPosition=_bone.v_Origin;
					_bone.v_ActualTranslation=_bone.v_Origin;

					//Add bone to the object bones list.
					pObject->bonesData.push_back(_bone);
					Game->Log()->write("<%s:%i %s> Bone: \"%s\" origin position <%f %f %f>",__FILE__,__LINE__,__FUNCTION__,bName,_bone.v_Origin.x,_bone.v_Origin.y,_bone.v_Origin.z);
			 
				}
			 
		 		fread(&pModel->animSpeed, sizeof(int), 1, m_FilePointer);
				fread(&pObject->numFrames, sizeof(int), 1, m_FilePointer);
 	 		

				Game->Log()->write("<%s:%i %s> Bones: %i",__FILE__,__LINE__,__FUNCTION__,pObject->numBones);
				Game->Log()->write("<%s:%i %s> Speed: %i",__FILE__,__LINE__,__FUNCTION__,pModel->animSpeed);
				Game->Log()->write("<%s:%i %s> Frames: %i",__FILE__,__LINE__,__FUNCTION__,pObject->numFrames);
			

				// Now we will go through every frame of animation and get the matrix data
				for(int a = 0; a < pObject->numFrames; a++)
				{
					// Create a structure to hold the matrix information
					tBoneAnimationFrames boneInfo;

					// Store the number of bones so we know how big the list is, then
					// add our structure to the list of vBoneInfo.
					boneInfo.numBones = pObject->numBones;
					pObject->vBoneInfo.push_back(boneInfo);
				
					// Just like when we loaded our weight influences we want to grab
					// a pointer to the last item we just added to our list.  We then
					// allocate memory for the matrix information for each bone and
					// read in the data.
					tBoneAnimationFrames *pBoneInfo = &pObject->vBoneInfo[pObject->vBoneInfo.size() - 1];
					pBoneInfo->pBoneMatrices = new tBoneMatrix [pObject->numBones];
					fread(pBoneInfo->pBoneMatrices, sizeof(tBoneMatrix), pObject->numBones, m_FilePointer);
					Game->Log()->write("<%s:%i %s> Frame: %i trans=<%1.1f %1.1f %1.1f> rot=<%1.1f %1.1f %1.1f %1.1f>",__FILE__,__LINE__,__FUNCTION__,a,pBoneInfo->pBoneMatrices->vTranslation.x,pBoneInfo->pBoneMatrices->vTranslation.y,pBoneInfo->pBoneMatrices->vTranslation.z,pBoneInfo->pBoneMatrices->qRotation.x,pBoneInfo->pBoneMatrices->qRotation.y,pBoneInfo->pBoneMatrices->qRotation.z,pBoneInfo->pBoneMatrices->qRotation.w);
				}

 

		}//PHYSIQUE NOT FOUND


	pObject->numIndices=meshInfo.numIndices;
	pObject->m_pIndices=new unsigned short[meshInfo.numIndices];
	for(int a=0;a<meshInfo.numIndices;a++){pObject->m_pIndices[a]=m_pIndices[a];}


	//Now we compute the normals as we have vert array converted
	ComputeNormals(pObject);

  
 
	/*
	*	PRE VBO STEP
	*	We build here the VBO structure filling texcoords, tangents, normals and vertexes inside.
	*/
	pObject->uvchanges=0;
 	for(int zzz=0;zzz < pObject->numOfFaces; zzz++)
	{
 
		
		 
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					int vertIndex = pObject->pFaces[zzz].vertIndex[whichVertex];

				 		pObject->pVertexy[vertIndex].nx=pObject->pNormals[ vertIndex ].x;
				 		pObject->pVertexy[vertIndex].ny=pObject->pNormals[ vertIndex ].y;
						pObject->pVertexy[vertIndex].nz=pObject->pNormals[ vertIndex ].z; 

		  			int texIndex  = pObject->pFaces[zzz].coordIndex[whichVertex];

						//This is fucking bad
						if( ( pObject->pVertexy[vertIndex].tx>0 &&   pObject->pVertexy[vertIndex].ty>0 )&& ( pObject->pVertexy[vertIndex].tx!=pObject->pTexVerts[texIndex].x ||   pObject->pVertexy[vertIndex].ty!=pObject->pTexVerts[texIndex].y) )
						{
							 pObject->uvchanges++;
						}

					 	pObject->pVertexy[vertIndex].tx=pObject->pTexVerts[texIndex].x;
				 		pObject->pVertexy[vertIndex].ty=pObject->pTexVerts[texIndex].y;
				}
 
			 	//PRZELATUJEMY PRZEZ WSZYSTKIE FEJSY
				//DLA KAZDEGO FEJSA WYLICZAMY TANGENT DLA NORMALMAPY
				 float tangent[4] = {0.0f, 0.0f, 0.0f, 0.0f};
 
				CVector3 vVertexPos0=CVector3(pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].x,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].y,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].z);
				CVector3 vVertexPos1=CVector3(pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].x,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].y,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].z);
				CVector3 vVertexPos2=CVector3(pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].x,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].y,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].z);

				CVector2 vUv0=CVector2(pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].tx,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].ty);
				CVector2 vUv1=CVector2(pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].tx,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].ty);
				CVector2 vUv2=CVector2(pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].tx,pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].ty);

				CVector3 vNormal=pObject->pFaces[zzz].normal;

				CalcTangentVector(vVertexPos0,vVertexPos1,vVertexPos2,vUv0,vUv1,vUv2,vNormal, tangent);				

				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].tangent[0] = tangent[0];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].tangent[1] = tangent[1];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].tangent[2] = tangent[2];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[0]].tangent[3] = tangent[3];
						
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].tangent[0] = tangent[0];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].tangent[1] = tangent[1];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].tangent[2] = tangent[2];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[1]].tangent[3] = tangent[3];

				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].tangent[0] = tangent[0];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].tangent[1] = tangent[1];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].tangent[2] = tangent[2];
				pObject->pVertexy[pObject->pFaces[zzz].vertIndex[2]].tangent[3] = tangent[3];
 
				 
	}  //perface
 
 
 
 
					Game->Log()->write("<%s:%i %s> VAO constructing.",__FILE__,__LINE__,__FUNCTION__);

	 
					/*BUILD VAO PROPERLY*/


					/* Allocate and assign a Vertex Array Object to our handle */
					glGenVertexArrays(1, &pObject->vao);
					glBindVertexArray(pObject->vao);

					//Wee need VBO bounded to continue with VAO
					glGenBuffers( 1, &pObject->m_nVBOVertices );					
					glBindBuffer( GL_ARRAY_BUFFER, pObject->m_nVBOVertices );	
					glBufferData( GL_ARRAY_BUFFER, sizeof(XYN_vbo_vert)*pObject->numOfVerts, &pObject->pVertexy[0], GL_STATIC_DRAW );
 
					//Setup the VertexPosition
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(XYN_vbo_vert),(char*)0);				

					//Setup the VertexNormal
					glEnableVertexAttribArray(1);	
					glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE,sizeof(XYN_vbo_vert),BUFFER_OFFSET(sizeof(GLfloat)*5));

					//Setup the VertexTexCoord
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE,sizeof(XYN_vbo_vert),BUFFER_OFFSET(sizeof(GLfloat)*3));	 

					//Setup the VertexTangent
					glEnableVertexAttribArray(3);
					glVertexAttribPointer(3, 4, GL_FLOAT,GL_FALSE,sizeof(XYN_vbo_vert),BUFFER_OFFSET(sizeof(GLfloat)*8)); 

			 
					//Generate IBO
  					glGenBuffers( 1, &pObject->m_nVBOIndices);					
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pObject->m_nVBOIndices );	
					glBufferData( GL_ELEMENT_ARRAY_BUFFER,  sizeof(unsigned short)*pObject->numIndices, &pObject->m_pIndices[0], GL_STATIC_DRAW );
		 
					// At this point the VAO is set up with 4 attributes
					// referencing the same buffer object, and another buffer object
					// as source for index data. We can now unbind the VAO, go do
					// something else, and bind it again later when we want to render
					// with it.
					glBindVertexArray(0);
					glBindBuffer( GL_ARRAY_BUFFER, 0 );	
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );	
					glDisableVertexAttribArray(0);	
					glDisableVertexAttribArray(1);	
					glDisableVertexAttribArray(2);	
					glDisableVertexAttribArray(3);	



	Game->Log()->write("<%s:%i %s> VBO done.",__FILE__,__LINE__,__FUNCTION__);
	  
	// Free all the memory for this mesh since we just co	nverted it to our structures
	 delete [] m_pVertices;   
	 delete [] m_pIndices; 
	 delete [] m_pTexCoords;
	 delete [] m_pTexIndices; 
		 
 
		 
}

Game->Log()->write("<%s:%i %s> Model loaded.",__FILE__,__LINE__,__FUNCTION__);

// Make sure the current frame of animation is set to 0
pModel->currentFrame = 0;
pModel->numOfObjects=numObjects;

return true;
}

void XYNLoader::ConvertDataStructures(t3DModel *pModel, tMeshInfo meshInfo)
{
	Game->Log()->write("<%s:%i %s> ",__FILE__,__LINE__,__FUNCTION__);

	int i = 0;

	// Increase the number of objects (sub-objects) in our model since we are loading a new one
	pModel->numOfObjects++;
		
	// Create a empty object structure to store the object's info before we add it to our list
	x3DObject currentMesh = {0};

	// Assign the vertex, texture coord and face count to our new structure.
	// Currently we have no UV coordinates so we set the size to 0.
	currentMesh.numOfVerts   = meshInfo.numVertices;
	currentMesh.numTexVertex = meshInfo.numTexCoords;
	currentMesh.numOfFaces   = meshInfo.numIndices/3;



	// Allocate memory for the vertices, texture coordinates and face data.
	// We set the texture coordinates to NULL for now (until the next tutorial).
	currentMesh.pVertexy = new XYN_vbo_vert [ meshInfo.numVertices ];		
	currentMesh.pVerts    = new CVector3 [currentMesh.numOfVerts];
	currentMesh.pTexVerts = new CVector2 [currentMesh.numTexVertex];
	currentMesh.pFaces    = new tFace [currentMesh.numOfFaces];
 

	pModel->modelSize.x=0;
	pModel->modelSize.y=0;
	pModel->modelSize.z=0;

	// Go through all of the vertices and assign them over to our structure
	for (i=0; i < currentMesh.numOfVerts; i++)
	{
		currentMesh.pVerts[i].x =  m_pVertices[i].x/1.0f;
		currentMesh.pVerts[i].y =  m_pVertices[i].y/1.0f;
		currentMesh.pVerts[i].z =  m_pVertices[i].z/1.0f;
		currentMesh.pVertexy[i].x = m_pVertices[i].x/1.0f;
		currentMesh.pVertexy[i].y = m_pVertices[i].y/1.0f;
		currentMesh.pVertexy[i].z = m_pVertices[i].z/1.0f;
		currentMesh.pVertexy[i].tx = -1;
		currentMesh.pVertexy[i].ty = -1;
	 

		if(currentMesh.pVerts[i].x>pModel->modelSize.x)pModel->modelSize.x=currentMesh.pVerts[i].x;
		if(currentMesh.pVerts[i].y>pModel->modelSize.y)pModel->modelSize.y=currentMesh.pVerts[i].y;
		if(currentMesh.pVerts[i].z>pModel->modelSize.z)pModel->modelSize.z=currentMesh.pVerts[i].z;
		 

	}
	Game->Log()->write("<%s:%i %s> Model size: %1.0f %1.0f %1.0f.",__FILE__,__LINE__,__FUNCTION__,pModel->modelSize.x,pModel->modelSize.y,pModel->modelSize.z);

	// Go through all of the UV coords and assign them over to our structure
	for (i=0; i < currentMesh.numTexVertex; i++)
	{
		currentMesh.pTexVerts[i].x = m_pTexCoords[i].x;
		currentMesh.pTexVerts[i].y = m_pTexCoords[i].y;
 	}

	
	// Go through all of the face data and assign it over to OUR structure.
	// Currently we don't support texture coordinates.
	for(i=0; i < currentMesh.numOfFaces; i++)
	{
		// Assign the vertex indices to our face data
		currentMesh.pFaces[i].vertIndex[0] = m_pIndices[i*3];
		currentMesh.pFaces[i].vertIndex[1] = m_pIndices[i*3 + 1];
		currentMesh.pFaces[i].vertIndex[2] = m_pIndices[i*3 + 2];

		// Assign the texture coord indices to our face data (same as the vertex indices)
		currentMesh.pFaces[i].coordIndex[0] = m_pTexIndices[i*3];
		currentMesh.pFaces[i].coordIndex[1] = m_pTexIndices[i*3 + 1];
		currentMesh.pFaces[i].coordIndex[2] = m_pTexIndices[i*3 + 2];

 


		// Set the texture ID for this face (smooth groups)
		currentMesh.pFaces[i].textureID = (int)m_pTexCoords[currentMesh.pFaces[i].coordIndex[0]].z;
	
	}
 
 
	// Store the number of materials for this mesh
	currentMesh.numOfMaterials = meshInfo.numMaterials;


	
 

	// Here we add the current object to our list object list
	pModel->pObject.push_back(currentMesh);
	
	
	 
 
}



void XYNLoader::CleanUp()
{
	// Close the current file pointer
	fclose(m_FilePointer);						
}
 


}//namespace