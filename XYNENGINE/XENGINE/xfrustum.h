#ifndef _FRUSTUM_H
#define _FRUSTUM_H
 
namespace X
{
using namespace std;
// This will allow us to create an object to keep track of our frustum
class CFrustum 
{

public:

	// Call this every time the camera moves to update the frustum
	void CalculateFrustum(const CMatrix4x4 &m_Projection, const CMatrix4x4 &m_View);
	void UpdateFrustumFaster(const CMatrix4x4 &m_Projection,  const CMatrix4x4 &m_View);
	// This takes a 3D point and returns TRUE if it's inside of the frustum
	bool PointInFrustum(float x, float y, float z);

	// This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
	bool SphereInFrustum(float x, float y, float z, float radius);

	// This takes the center and half the length of the cube.
	bool CubeInFrustum(float x, float y, float z, float size);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// This checks if a box is in the frustum
	bool BoxInFrustum(float x, float y, float z, float sizeX, float sizeY, float sizeZ);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


	void DrawFrustum() ;
	void CalculateFrustumCorners(CVector3 pPoints[8], CVector3 vPos, CVector3 vView,   
                                       CVector3 vUp, float fNear, float fFar, float fScale,   
                                       float fFOV, float fAspect)  ;


private:


		void CalculateFrustumVertices()   ;
		// This holds the A B C and D values for each side of our frustum.
		float m_Frustum[6][4];
		CVector3 m_FrustumVertices[8]; 
};


// This is our debug lines class to view the octree visually
class CDebug 
{

public:
	
	// This adds a line to our list of debug lines
	void AddDebugLine(CVector3 vPoint1, CVector3 vPoint2);

	// This adds a rectangle with a given center, width, height and depth to our list
	void AddDebugBox(CVector3 vCenter, float width, float height, float depth);

	// This renders all of the lines
	void RenderDebugLines();		

	// This clears all of the debug lines
	void Clear();					

private:

	// This is the vector list of all of our lines
	vector<CVector3> m_vLines;		

};

}
#endif

