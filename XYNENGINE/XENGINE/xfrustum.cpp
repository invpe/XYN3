
#include "xmain.h"
 
namespace X
{
// This is the index in our selection buffer that has the closet object ID clicked
#define FIRST_OBJECT_ID  3								

// We create an enum of the sides so we don't have to call each side 0 or 1.
// This way it makes it more understandable and readable when dealing with frustum sides.
enum FrustumSide
{
	RIGHT	= 0,		// The RIGHT side of the frustum
	LEFT	= 1,		// The LEFT	 side of the frustum
	BOTTOM	= 2,		// The BOTTOM side of the frustum
	TOP		= 3,		// The TOP side of the frustum
	BACK	= 4,		// The BACK	side of the frustum
	FRONT	= 5			// The FRONT side of the frustum
}; 

// Like above, instead of saying a number for the ABC and D of the plane, we
// want to be more descriptive.
enum PlaneData
{
	A = 0,				// The X value of the plane's normal
	B = 1,				// The Y value of the plane's normal
	C = 2,				// The Z value of the plane's normal
	D = 3				// The distance the plane is from the origin
};

///////////////////////////////// NORMALIZE PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This normalizes a plane (A side) from a given frustum.
/////
///////////////////////////////// NORMALIZE PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void NormalizePlane(float frustum[6][4], int side)
{
	// Here we calculate the magnitude of the normal to the plane (point A B C)
	// Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
	// To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
	float magnitude = (float)sqrt( frustum[side][A] * frustum[side][A] + 
								   frustum[side][B] * frustum[side][B] + 
								   frustum[side][C] * frustum[side][C] );

	// Then we divide the plane's values by it's magnitude.
	// This makes it easier to work with.
	frustum[side][A] /= magnitude;
	frustum[side][B] /= magnitude;
	frustum[side][C] /= magnitude;
	frustum[side][D] /= magnitude; 
}


void CFrustum::CalculateFrustumVertices()   
{   
    Plane Planes[6];   
    for(int i = 0; i < 6; i++)   
    {   
        Planes[i].a = m_Frustum[i][A];   
        Planes[i].b = m_Frustum[i][B];   
        Planes[i].c = m_Frustum[i][C];   
        Planes[i].d = m_Frustum[i][D];   
    }   
   

    // near plane vertices   
    Planes[0].PlaneIntersection(&m_FrustumVertices[0], &Planes[5], &Planes[2], &Planes[1]);   
    Planes[0].PlaneIntersection(&m_FrustumVertices[1], &Planes[5], &Planes[2], &Planes[0]);   
    Planes[0].PlaneIntersection(&m_FrustumVertices[2], &Planes[5], &Planes[3], &Planes[0]);   
    Planes[0].PlaneIntersection(&m_FrustumVertices[3], &Planes[5], &Planes[3], &Planes[1]);   
   
    // far plane vertices   
    Planes[0].PlaneIntersection(&m_FrustumVertices[4], &Planes[4], &Planes[2], &Planes[1]);   
    Planes[0].PlaneIntersection(&m_FrustumVertices[5], &Planes[4], &Planes[2], &Planes[0]);   
    Planes[0].PlaneIntersection(&m_FrustumVertices[6], &Planes[4], &Planes[3], &Planes[0]);   
    Planes[0].PlaneIntersection(&m_FrustumVertices[7], &Planes[4], &Planes[3], &Planes[1]);   
}   
   
// This function calculates the appropriate vertices for the current split's frustum   
// Note that this is almost a direct copy&paste from the Direct3D demo, so I won't go   
// into detail about this. There's lots of documentation about this on the net.   
void CFrustum::CalculateFrustumCorners(CVector3 pPoints[8], CVector3 vPos, CVector3 vView,   
                                       CVector3 vUp, float fNear, float fFar, float fScale,   
                                       float fFOV, float fAspect)   
{   
    CVector3 vZ= vView - vPos;   
    vZ.Normalize();   
   
    CVector3 vX;   
    vX = vX.Cross(vUp, vZ);   
    vX.Normalize();   
   
    CVector3 vY;   
    vY = vY.Cross(vZ, vX);   
         
    float fNearPlaneHeight = tanf(GET_RADIANS(fFOV) * 0.5f) * fNear;   
    float fNearPlaneWidth = fNearPlaneHeight * fAspect;   
   
    float fFarPlaneHeight = tanf(GET_RADIANS(fFOV) * 0.5f) * fFar;   
    float fFarPlaneWidth = fFarPlaneHeight * fAspect;   
   
    CVector3 vNearPlaneCenter = vPos + (vZ *= fNear);   
    CVector3 vFarPlaneCenter = vPos + (vZ *= fFar);   
   
    pPoints[0] = CVector3(vNearPlaneCenter - (vX*=fNearPlaneWidth) - (vY*=fNearPlaneHeight));   
    pPoints[1] = CVector3(vNearPlaneCenter - (vX*=fNearPlaneWidth) + (vY*=fNearPlaneHeight));   
    pPoints[2] = CVector3(vNearPlaneCenter + (vX*=fNearPlaneWidth) + (vY*=fNearPlaneHeight));   
    pPoints[3] = CVector3(vNearPlaneCenter + (vX*=fNearPlaneWidth) - (vY*=fNearPlaneHeight));   
   
    pPoints[4] = CVector3(vFarPlaneCenter - (vX*=fFarPlaneWidth) - (vY*=fFarPlaneHeight));   
    pPoints[5] = CVector3(vFarPlaneCenter - (vX*=fFarPlaneWidth) + (vY*=fFarPlaneHeight));   
    pPoints[6] = CVector3(vFarPlaneCenter + (vX*=fFarPlaneWidth) + (vY*=fFarPlaneHeight));   
    pPoints[7] = CVector3(vFarPlaneCenter + (vX*=fFarPlaneWidth) - (vY*=fFarPlaneHeight));   
   
    CVector3 vCenter(0.0f, 0.0f, 0.0f);   
    for(int i = 0; i <  8; i++)    
        vCenter = vCenter + pPoints[i];   
   
    vCenter /= 8.0f;   
   
    for(int i = 0; i < 8; i++)    
        pPoints[i] = pPoints[i] + ((pPoints[i]-vCenter)*=(fScale-1.0f));   
}   
   
void CFrustum::DrawFrustum()   
{   
    glDisable(GL_LIGHTING);   
    glDisable(GL_TEXTURE_2D);   
   
    glBegin(GL_LINES);   
        glColor3ub(255, 255, 0);   
        glPushMatrix();   
        glTranslatef(0,50,0);   
            // near plane   
            glVertex3fv(m_FrustumVertices[0]);   
            glVertex3fv(m_FrustumVertices[1]);   
   
            glVertex3fv(m_FrustumVertices[1]);   
            glVertex3fv(m_FrustumVertices[2]);   
   
            glVertex3fv(m_FrustumVertices[2]);   
            glVertex3fv(m_FrustumVertices[3]);   
               
            glVertex3fv(m_FrustumVertices[3]);   
            glVertex3fv(m_FrustumVertices[0]);   
   
            // left plane   
            glVertex3fv(m_FrustumVertices[4]);   
            glVertex3fv(m_FrustumVertices[0]);   
   
            glVertex3fv(m_FrustumVertices[0]);   
            glVertex3fv(m_FrustumVertices[3]);   
   
            glVertex3fv(m_FrustumVertices[3]);   
            glVertex3fv(m_FrustumVertices[7]);   
               
            glVertex3fv(m_FrustumVertices[7]);   
            glVertex3fv(m_FrustumVertices[4]);   
   
            // right plane   
            glVertex3fv(m_FrustumVertices[1]);   
            glVertex3fv(m_FrustumVertices[5]);   
   
            glVertex3fv(m_FrustumVertices[5]);   
            glVertex3fv(m_FrustumVertices[6]);   
   
            glVertex3fv(m_FrustumVertices[6]);   
            glVertex3fv(m_FrustumVertices[2]);   
               
            glVertex3fv(m_FrustumVertices[2]);   
            glVertex3fv(m_FrustumVertices[1]);   
   
            // far plane   
            glVertex3fv(m_FrustumVertices[4]);   
            glVertex3fv(m_FrustumVertices[5]);   
   
            glVertex3fv(m_FrustumVertices[5]);   
            glVertex3fv(m_FrustumVertices[6]);   
   
            glVertex3fv(m_FrustumVertices[6]);   
            glVertex3fv(m_FrustumVertices[7]);   
               
            glVertex3fv(m_FrustumVertices[7]);   
            glVertex3fv(m_FrustumVertices[4]);   
   
        glPushMatrix();   
    glEnd();   
   
    glEnable(GL_LIGHTING);   
    glEnable(GL_TEXTURE_2D);   
}  

void CFrustum::UpdateFrustumFaster(const CMatrix4x4 &m_Projection, const CMatrix4x4 &m_View)
{
	float   clip[16];
	GLfloat   t;
 
	 
	 // Combine the two matrices (multiply projection by modelview) 
	 // but keep in mind this function will only work if you do NOT
	 // rotate or translate your projection matrix                  
	clip[ 0] = m_View[ 0] * m_Projection[ 0];
	clip[ 1] = m_View[ 1] * m_Projection[ 5];
    clip[ 2] = m_View[ 2] * m_Projection[10] + m_View[ 3] * m_Projection[14];
    clip[ 3] = m_View[ 2] * m_Projection[11];

    clip[ 4] = m_View[ 4] * m_Projection[ 0];
    clip[ 5] = m_View[ 5] * m_Projection[ 5];
    clip[ 6] = m_View[ 6] * m_Projection[10] + m_View[ 7] * m_Projection[14];
    clip[ 7] = m_View[ 6] * m_Projection[11];

    clip[ 8] = m_View[ 8] * m_Projection[ 0];
    clip[ 9] = m_View[ 9] * m_Projection[ 5];
    clip[10] = m_View[10] * m_Projection[10] + m_View[11] * m_Projection[14];
    clip[11] = m_View[10] * m_Projection[11];

    clip[12] = m_View[12] * m_Projection[ 0];
    clip[13] = m_View[13] * m_Projection[ 5];
    clip[14] = m_View[14] * m_Projection[10] + m_View[15] * m_Projection[14];
    clip[15] = m_View[14] * m_Projection[11];

	// Extract the numbers for the RIGHT plane 
	m_Frustum[0][0] = clip[ 3] - clip[ 0];
	m_Frustum[0][1] = clip[ 7] - clip[ 4];
	m_Frustum[0][2] = clip[11] - clip[ 8];
	m_Frustum[0][3] = clip[15] - clip[12];

	//Normalize the result 
	t = GLfloat(sqrt( m_Frustum[0][0] * m_Frustum[0][0] + m_Frustum[0][1] * m_Frustum[0][1] + m_Frustum[0][2] * m_Frustum[0][2] ));
	m_Frustum[0][0] /= t;
	m_Frustum[0][1] /= t;
	m_Frustum[0][2] /= t;
	m_Frustum[0][3] /= t;

    //Extract the numbers for the LEFT plane 
    m_Frustum[1][0] = clip[ 3] + clip[ 0];
    m_Frustum[1][1] = clip[ 7] + clip[ 4];
    m_Frustum[1][2] = clip[11] + clip[ 8];
    m_Frustum[1][3] = clip[15] + clip[12];

   // Normalize the result
    t = GLfloat(sqrt( m_Frustum[1][0] * m_Frustum[1][0] + m_Frustum[1][1] * m_Frustum[1][1] + m_Frustum[1][2] * m_Frustum[1][2] ));
    m_Frustum[1][0] /= t;
    m_Frustum[1][1] /= t;
    m_Frustum[1][2] /= t;
    m_Frustum[1][3] /= t;

	//Extract the BOTTOM plane
    m_Frustum[2][0] = clip[ 3] + clip[ 1];
    m_Frustum[2][1] = clip[ 7] + clip[ 5];
    m_Frustum[2][2] = clip[11] + clip[ 9];
    m_Frustum[2][3] = clip[15] + clip[13];

    //Normalize the result
    t = GLfloat(sqrt( m_Frustum[2][0] * m_Frustum[2][0] + m_Frustum[2][1] * m_Frustum[2][1] + m_Frustum[2][2] * m_Frustum[2][2] ));
    m_Frustum[2][0] /= t;
    m_Frustum[2][1] /= t;
    m_Frustum[2][2] /= t;
    m_Frustum[2][3] /= t;

   // Extract the TOP plane 
    m_Frustum[3][0] = clip[ 3] - clip[ 1];
    m_Frustum[3][1] = clip[ 7] - clip[ 5];
    m_Frustum[3][2] = clip[11] - clip[ 9];
    m_Frustum[3][3] = clip[15] - clip[13];

    //Normalize the result 
    t = GLfloat(sqrt( m_Frustum[3][0] * m_Frustum[3][0] + m_Frustum[3][1] * m_Frustum[3][1] + m_Frustum[3][2] * m_Frustum[3][2] ));
    m_Frustum[3][0] /= t;
    m_Frustum[3][1] /= t;
    m_Frustum[3][2] /= t;
    m_Frustum[3][3] /= t;

    //Extract the FAR plane
    m_Frustum[4][0] = clip[ 3] - clip[ 2];
    m_Frustum[4][1] = clip[ 7] - clip[ 6];
    m_Frustum[4][2] = clip[11] - clip[10];
    m_Frustum[4][3] = clip[15] - clip[14];

    //Normalize the result 
    t = GLfloat(sqrt( m_Frustum[4][0] * m_Frustum[4][0] + m_Frustum[4][1] * m_Frustum[4][1] + m_Frustum[4][2] * m_Frustum[4][2] ));
    m_Frustum[4][0] /= t;
    m_Frustum[4][1] /= t;
    m_Frustum[4][2] /= t;
    m_Frustum[4][3] /= t;

    // Extract the NEAR plane 
    m_Frustum[5][0] = clip[ 3] + clip[ 2];
    m_Frustum[5][1] = clip[ 7] + clip[ 6];
    m_Frustum[5][2] = clip[11] + clip[10];
    m_Frustum[5][3] = clip[15] + clip[14];

    // Normalize the result
    t = GLfloat(sqrt( m_Frustum[5][0] * m_Frustum[5][0] + m_Frustum[5][1] * m_Frustum[5][1] + m_Frustum[5][2] * m_Frustum[5][2] ));
    m_Frustum[5][0] /= t;
    m_Frustum[5][1] /= t;
    m_Frustum[5][2] /= t;
    m_Frustum[5][3] /= t; 

}
void CFrustum::CalculateFrustum(const CMatrix4x4 &m_Projection, const CMatrix4x4 &m_View)
{    
 float   clip[16];								// This will hold the clipping planes

  
	// Now that we have our modelview and projection matrix, if we combine these 2 matrices,
	// it will give us our clipping planes.  To combine 2 matrices, we multiply them.
 
	clip[ 0] = m_View[ 0] * m_Projection[ 0] + m_View[ 1] * m_Projection[ 4] + m_View[ 2] * m_Projection[ 8] + m_View[ 3] * m_Projection[12];
	clip[ 1] = m_View[ 0] * m_Projection[ 1] + m_View[ 1] * m_Projection[ 5] + m_View[ 2] * m_Projection[ 9] + m_View[ 3] * m_Projection[13];
	clip[ 2] = m_View[ 0] * m_Projection[ 2] + m_View[ 1] * m_Projection[ 6] + m_View[ 2] * m_Projection[10] + m_View[ 3] * m_Projection[14];
	clip[ 3] = m_View[ 0] * m_Projection[ 3] + m_View[ 1] * m_Projection[ 7] + m_View[ 2] * m_Projection[11] + m_View[ 3] * m_Projection[15];

	clip[ 4] = m_View[ 4] * m_Projection[ 0] + m_View[ 5] * m_Projection[ 4] + m_View[ 6] * m_Projection[ 8] + m_View[ 7] * m_Projection[12];
	clip[ 5] = m_View[ 4] * m_Projection[ 1] + m_View[ 5] * m_Projection[ 5] + m_View[ 6] * m_Projection[ 9] + m_View[ 7] * m_Projection[13];
	clip[ 6] = m_View[ 4] * m_Projection[ 2] + m_View[ 5] * m_Projection[ 6] + m_View[ 6] * m_Projection[10] + m_View[ 7] * m_Projection[14];
	clip[ 7] = m_View[ 4] * m_Projection[ 3] + m_View[ 5] * m_Projection[ 7] + m_View[ 6] * m_Projection[11] + m_View[ 7] * m_Projection[15];

	clip[ 8] = m_View[ 8] * m_Projection[ 0] + m_View[ 9] * m_Projection[ 4] + m_View[10] * m_Projection[ 8] + m_View[11] * m_Projection[12];
	clip[ 9] = m_View[ 8] * m_Projection[ 1] + m_View[ 9] * m_Projection[ 5] + m_View[10] * m_Projection[ 9] + m_View[11] * m_Projection[13];
	clip[10] = m_View[ 8] * m_Projection[ 2] + m_View[ 9] * m_Projection[ 6] + m_View[10] * m_Projection[10] + m_View[11] * m_Projection[14];
	clip[11] = m_View[ 8] * m_Projection[ 3] + m_View[ 9] * m_Projection[ 7] + m_View[10] * m_Projection[11] + m_View[11] * m_Projection[15];

	clip[12] = m_View[12] * m_Projection[ 0] + m_View[13] * m_Projection[ 4] + m_View[14] * m_Projection[ 8] + m_View[15] * m_Projection[12];
	clip[13] = m_View[12] * m_Projection[ 1] + m_View[13] * m_Projection[ 5] + m_View[14] * m_Projection[ 9] + m_View[15] * m_Projection[13];
	clip[14] = m_View[12] * m_Projection[ 2] + m_View[13] * m_Projection[ 6] + m_View[14] * m_Projection[10] + m_View[15] * m_Projection[14];
	clip[15] = m_View[12] * m_Projection[ 3] + m_View[13] * m_Projection[ 7] + m_View[14] * m_Projection[11] + m_View[15] * m_Projection[15];
	
	// Now we actually want to get the sides of the frustum.  To do this we take
	// the clipping planes we received above and extract the sides from them.

	// This will extract the RIGHT side of the frustum
	m_Frustum[RIGHT][A] = clip[ 3] - clip[ 0];
	m_Frustum[RIGHT][B] = clip[ 7] - clip[ 4];
	m_Frustum[RIGHT][C] = clip[11] - clip[ 8];
	m_Frustum[RIGHT][D] = clip[15] - clip[12];

	// Now that we have a normal (A,B,C) and a distance (D) to the plane,
	// we want to normalize that normal and distance.

	// Normalize the RIGHT side
	NormalizePlane(m_Frustum, RIGHT);

	// This will extract the LEFT side of the frustum
	m_Frustum[LEFT][A] = clip[ 3] + clip[ 0];
	m_Frustum[LEFT][B] = clip[ 7] + clip[ 4];
	m_Frustum[LEFT][C] = clip[11] + clip[ 8];
	m_Frustum[LEFT][D] = clip[15] + clip[12];

	// Normalize the LEFT side
	NormalizePlane(m_Frustum, LEFT);

	// This will extract the BOTTOM side of the frustum
	m_Frustum[BOTTOM][A] = clip[ 3] + clip[ 1];
	m_Frustum[BOTTOM][B] = clip[ 7] + clip[ 5];
	m_Frustum[BOTTOM][C] = clip[11] + clip[ 9];
	m_Frustum[BOTTOM][D] = clip[15] + clip[13];

	// Normalize the BOTTOM side
	NormalizePlane(m_Frustum, BOTTOM);

	// This will extract the TOP side of the frustum
	m_Frustum[TOP][A] = clip[ 3] - clip[ 1];
	m_Frustum[TOP][B] = clip[ 7] - clip[ 5];
	m_Frustum[TOP][C] = clip[11] - clip[ 9];
	m_Frustum[TOP][D] = clip[15] - clip[13];

	// Normalize the TOP side
	NormalizePlane(m_Frustum, TOP);

	// This will extract the BACK side of the frustum
	m_Frustum[BACK][A] = clip[ 3] - clip[ 2];
	m_Frustum[BACK][B] = clip[ 7] - clip[ 6];
	m_Frustum[BACK][C] = clip[11] - clip[10];
	m_Frustum[BACK][D] = clip[15] - clip[14];

	// Normalize the BACK side
	NormalizePlane(m_Frustum, BACK);

	// This will extract the FRONT side of the frustum
	m_Frustum[FRONT][A] = clip[ 3] + clip[ 2];
	m_Frustum[FRONT][B] = clip[ 7] + clip[ 6];
	m_Frustum[FRONT][C] = clip[11] + clip[10];
	m_Frustum[FRONT][D] = clip[15] + clip[14];

	// Normalize the FRONT side
	NormalizePlane(m_Frustum, FRONT);

}

// The code below will allow us to make checks within the frustum.  For example,
// if we want to see if a point, a sphere, or a cube lies inside of the frustum.
// Because all of our planes point INWARDS (The normals are all pointing inside the frustum)
// we then can assume that if a point is in FRONT of all of the planes, it's inside.

///////////////////////////////// POINT IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This determines if a point is inside of the frustum
/////
///////////////////////////////// POINT IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CFrustum::PointInFrustum( float x, float y, float z )
{
	// Go through all the sides of the frustum
	for(int i = 0; i < 6; i++ )
	{
		// Calculate the plane equation and check if the point is behind a side of the frustum
		if(m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= 0)
		{
			// The point was behind a side, so it ISN'T in the frustum
			return false;
		}
	}

	// The point was inside of the frustum (In front of ALL the sides of the frustum)
	return true;
}


///////////////////////////////// SPHERE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This determines if a sphere is inside of our frustum by it's center and radius.
/////
///////////////////////////////// SPHERE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CFrustum::SphereInFrustum( float x, float y, float z, float radius )
{
	// Go through all the sides of the frustum
	for(int i = 0; i < 6; i++ )	
	{
		// If the center of the sphere is farther away from the plane than the radius
		if( m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= -radius )
		{
			// The distance was greater than the radius so the sphere is outside of the frustum
			return false;
		}
	}
	
	// The sphere was inside of the frustum!
	return true;
}


///////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This determines if a cube is in or around our frustum by it's center and 1/2 it's length
/////
///////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CFrustum::CubeInFrustum( float x, float y, float z, float size )
{
	// Basically, what is going on is, that we are given the center of the cube,
	// and half the length.  Think of it like a radius.  Then we checking each point
	// in the cube and seeing if it is inside the frustum.  If a point is found in front
	// of a side, then we skip to the next side.  If we get to a plane that does NOT have
	// a point in front of it, then it will return false.

	// *Note* - This will sometimes say that a cube is inside the frustum when it isn't.
	// This happens when all the corners of the bounding box are not behind any one plane.
	// This is rare and shouldn't effect the overall rendering speed.

	for(int i = 0; i < 6; i++ )
	{
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;

		// If we get here, it isn't in the frustum
		return false;
	}

	return true;
}


///////////////////////////////// BOX IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This determines if a BOX is in or around our frustum by it's min and max points
/////
///////////////////////////////// BOX IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CFrustum::BoxInFrustum( float x, float y, float z, float x2, float y2, float z2)
{
	// Go through all of the corners of the box and check then again each plane
	// in the frustum.  If all of them are behind one of the planes, then it most
	// like is not in the frustum.
	for(int i = 0; i < 6; i++ )
	{
		if(m_Frustum[i][A] * x  + m_Frustum[i][B] * y  + m_Frustum[i][C] * z  + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x2 + m_Frustum[i][B] * y  + m_Frustum[i][C] * z  + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x  + m_Frustum[i][B] * y2 + m_Frustum[i][C] * z  + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x2 + m_Frustum[i][B] * y2 + m_Frustum[i][C] * z  + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x  + m_Frustum[i][B] * y  + m_Frustum[i][C] * z2 + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x2 + m_Frustum[i][B] * y  + m_Frustum[i][C] * z2 + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x  + m_Frustum[i][B] * y2 + m_Frustum[i][C] * z2 + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x2 + m_Frustum[i][B] * y2 + m_Frustum[i][C] * z2 + m_Frustum[i][D] > 0)  continue;

		// If we get here, it isn't in the frustum
		return false;
	}

	// Return a true for the box being inside of the frustum
	return true;
}


///////////////////////////////// RENDER DEBUG LINES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This goes through all of the lines that we stored in our list and draws them
/////
///////////////////////////////// RENDER DEBUG LINES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CDebug::RenderDebugLines()				// This renders all of the lines
{
 
	glBegin(GL_LINES);						// Start rendering lines

		glColor3ub(255, 255, 0);			// Turn the lines yellow

		// Go through the whole list of lines stored in the vector m_vLines.
		for(unsigned int i = 0; i < m_vLines.size(); i++)
		{
			// Pass in the current point to be rendered as part of a line
			glVertex3f(m_vLines[i].x, m_vLines[i].y, m_vLines[i].z);
		}	

	glEnd();				
}


///////////////////////////////// ADD DEBUG LINE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This adds a debug LINE to the stack of lines
/////
///////////////////////////////// ADD DEBUG LINE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CDebug::AddDebugLine(CVector3 vPoint1, CVector3 vPoint2)
{
	// Add the 2 points that make up the line into our line list.
	m_vLines.push_back(vPoint1);
	m_vLines.push_back(vPoint2);
}


///////////////////////////////// ADD DEBUG BOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This adds a debug BOX to the stack of lines
/////
///////////////////////////////// ADD DEBUG BOX\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CDebug::AddDebugBox(CVector3 vCenter, float width, float height, float depth)
{
	// So we can work with the code better, we divide the dimensions in half.
	// That way we can create the box from the center outwards.
	width /= 2.0f;	height /= 2.0f;	depth /= 2.0f;

	// Below we create all the 8 points so it will be easier to input the lines
	// of the box.  With the dimensions we calculate the points.
	CVector3 vTopLeftFront( vCenter.x - width, vCenter.y + height, vCenter.z + depth);
	CVector3 vTopLeftBack(  vCenter.x - width, vCenter.y + height, vCenter.z - depth);
	CVector3 vTopRightBack( vCenter.x + width, vCenter.y + height, vCenter.z - depth);
	CVector3 vTopRightFront(vCenter.x + width, vCenter.y + height, vCenter.z + depth);

	CVector3 vBottom_LeftFront( vCenter.x - width, vCenter.y - height, vCenter.z + depth);
	CVector3 vBottom_LeftBack(  vCenter.x - width, vCenter.y - height, vCenter.z - depth);
	CVector3 vBottomRightBack( vCenter.x + width, vCenter.y - height, vCenter.z - depth);
	CVector3 vBottomRightFront(vCenter.x + width, vCenter.y - height, vCenter.z + depth);

	////////// TOP LINES ////////// 

	// Store the top front line of the box
	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vTopRightFront);

	// Store the top back line of the box
	m_vLines.push_back(vTopLeftBack);  		m_vLines.push_back(vTopRightBack);

	// Store the top left line of the box
	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vTopLeftBack);

	// Store the top right line of the box
	m_vLines.push_back(vTopRightFront);		m_vLines.push_back(vTopRightBack);

	////////// BOTTOM LINES ////////// 

	// Store the bottom front line of the box
	m_vLines.push_back(vBottom_LeftFront);	m_vLines.push_back(vBottomRightFront);

	// Store the bottom back line of the box
	m_vLines.push_back(vBottom_LeftBack);	m_vLines.push_back(vBottomRightBack);

	// Store the bottom left line of the box
	m_vLines.push_back(vBottom_LeftFront);	m_vLines.push_back(vBottom_LeftBack);

	// Store the bottom right line of the box
	m_vLines.push_back(vBottomRightFront);	m_vLines.push_back(vBottomRightBack);

	////////// SIDE LINES ////////// 

	// Store the bottom front line of the box
	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vBottom_LeftFront);

	// Store the back left line of the box
	m_vLines.push_back(vTopLeftBack);		m_vLines.push_back(vBottom_LeftBack);

	// Store the front right line of the box
	m_vLines.push_back(vTopRightBack);		m_vLines.push_back(vBottomRightBack);

	// Store the front left line of the box
	m_vLines.push_back(vTopRightFront);		m_vLines.push_back(vBottomRightFront);
}


}