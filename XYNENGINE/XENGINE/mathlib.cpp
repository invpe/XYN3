#include "xmain.h"

using namespace std;   


CVector3 CVector3::Negate(CVector3 vVector)   
{   
    CVector3 vResult;   
   
    vResult.x = -vVector.x;   
    vResult.y = -vVector.y;   
    vResult.z = -vVector.z;   
   
    return vResult;   
}   
   
float CVector3::GetMagnitude(CVector3 vVector)   
{   
    return (float)sqrt(vVector.x * vVector.x +   
                       vVector.y * vVector.y +   
                       vVector.z * vVector.z);   
}   
 inline void CVector3::set(float x_, float y_, float z_)
{
    x = x_, y = y_, z = z_;
}

  
CVector3 CVector3::Vector(CVector3 vPoint1, CVector3 vPoint2)   
{   
    CVector3 vVector = CVector3(vPoint1.x - vPoint2.x,   
                                vPoint1.y - vPoint2.y,   
                                vPoint1.z - vPoint2.z);   
   
    return vVector;   
}   
   // This adds 2 vectors together and returns the result
CVector3 CVector3::AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;							// The variable to hold the resultant vector
	
	vResult.x = vVector2.x + vVector1.x;		// Add Vector1 and Vector2 x's
	vResult.y = vVector2.y + vVector1.y;		// Add Vector1 and Vector2 y's
	vResult.z = vVector2.z + vVector1.z;		// Add Vector1 and Vector2 z's

	return vResult;								// Return the resultant vector
}
// This divides a vector by a single number (scalar) and returns the result
CVector3 CVector3::DivideVectorByScaler(CVector3 vVector1, GLfloat Scaler)
{
	CVector3 vResult;							// The variable to hold the resultant vector
	
	vResult.x = vVector1.x / Scaler;			// Divide Vector1's x value by the scaler
	vResult.y = vVector1.y / Scaler;			// Divide Vector1's y value by the scaler
	vResult.z = vVector1.z / Scaler;			// Divide Vector1's z value by the scaler

	return vResult;								// Return the resultant vector
}

 CVector3 CVector3::Cross(CVector3 vVector1, CVector3 vVector2)   
{   
    CVector3 vCross;   
   
    vCross.x = (vVector1.y * vVector2.z) - (vVector1.z * vVector2.y);   
    vCross.y = (vVector1.z * vVector2.x) - (vVector1.x * vVector2.z);   
    vCross.z = (vVector1.x * vVector2.y) - (vVector1.y * vVector2.x);   
   
    return vCross;   
}   
	CVector3 CVector3::Cross(CVector3 vVector2)
	{
		CVector3 vNormal;	

		// Calculate the cross product with the non-commutative equation
		vNormal.x = ((y * vVector2.z) - (z * vVector2.y));
		vNormal.y = ((z * vVector2.x) - (x * vVector2.z));
		vNormal.z = ((x * vVector2.y) - (y * vVector2.x));

		// Return the cross product
		return vNormal;										 
	}
   
float CVector3::Distance(const CVector3 &vPoint1, const CVector3 &vPoint2)   
{   
    return sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +   
                 (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +   
                 (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z));   
}   
   
float CVector3::Dot(CVector3 vVector1, CVector3 vVector2)   
{   
    return ((vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z));   
}   
   
double CVector3::AngleBetweenVectors(CVector3 vVector1, CVector3 vVector2)   
{   
    float DotProduct =  Dot(vVector1, vVector2);   
   
    float VectorsMagnitude = GetMagnitude(vVector1) * GetMagnitude(vVector2);   
   
    double angle = acos(DotProduct / VectorsMagnitude);   
   
    if(_isnan(angle))   
        return 0;   
   
    return angle;   
}   
   
CVector3 CVector3::ClosestPointOnLine(CVector3 vLine[2], CVector3 vPoint)   
{   
    CVector3 vVector1 = vPoint - vLine[0];   
    CVector3 vVector2 = vVector2.Normalize(vLine[1] - vLine[0]);   
   
    float d = this->Distance(vLine[0], vLine[1]);   
   
    float t = this->Dot(vVector2, vVector1);   
   
    if(t = 0)   
        return vLine[0];   
   
    if(t >= d)   
        return vLine[1];   
   
    CVector3 vVector3 = vVector2 *= t;   
   
    CVector3 vClosestPoint = vLine[0] + vVector3;   
   
    return vClosestPoint;   
}   
   
CVector3 CVector3::Normalize(CVector3 vVector)   
{   
    float fMagnitude;          
   
    fMagnitude = GetMagnitude(vVector);   
   
    vVector.x /= fMagnitude;   
    vVector.y /= fMagnitude;   
    vVector.z /= fMagnitude;   
   
    return vVector;   
}   
   
CVector3 CVector3::CalcNormal(CVector3 vPoly[3])   
{   
    CVector3 v1, v2;   
   
    v1.x = vPoly[0].x - vPoly[1].x;   
    v1.y = vPoly[0].y - vPoly[1].y;   
    v1.z = vPoly[0].z - vPoly[1].z;   
   
    v2.x = vPoly[1].x - vPoly[2].x;   
    v2.y = vPoly[1].y - vPoly[2].y;   
    v2.z = vPoly[1].z - vPoly[2].z;   
   
    CVector3 vResult = Cross(v1, v2);   
    vResult = Normalize(vResult);   
   
    return vResult;    
}   
   
CVector3 CVector3::GetCollisionOffset(CVector3 &vNormal, float radius, float distance)   
{   
    CVector3 vOffset = CVector3(0, 0, 0);   
       
    if(distance > 1)   
    {   
        float distanceOver = radius - distance;   
        vOffset = vNormal *= distanceOver;   
    }   
    else   
    {   
        float distanceOver = radius + distance;   
        vOffset = vNormal *= -distanceOver;   
    }   
   
    return vOffset;   
}   
   
void CVector3::Normalize()   
{   
    float fMagnitude;          
   
    fMagnitude = GetMagnitude(*this);   
   
    x /= fMagnitude;   
    y /= fMagnitude;   
    z /= fMagnitude;   
}   
 	 CVector3 operator*(float scalar, const CVector3& v)
	{
		return CVector3(v.x * scalar, v.y * scalar, v.z * scalar);
	}
   
void CVector3::CalculateTBNMatrix(CVector3 pVerts[3], CTexCoord pTexCoords[3],CVector3 pTBNMatrix[3])   
{   
  /*  int i = 0;   
   
    int nNextIndex = (i + 1) % 3;   
    int nPrevIndex = (i + 2) % 3;   
   
    CVector3 v2v1 = pVerts[nNextIndex] - pVerts[i];   
    CVector3 v3v1 = pVerts[nPrevIndex] - pVerts[i];   
   
    float c2c1_T = pTexCoords[nNextIndex].tu - pTexCoords[i].tu;   
    float c2c1_B = pTexCoords[nNextIndex].tv - pTexCoords[i].tv;   
   
    float c3c1_T = pTexCoords[nPrevIndex].tu - pTexCoords[i].tu;   
    float c3c1_B = pTexCoords[nPrevIndex].tv - pTexCoords[i].tv;   
   
    float fDenominator = c2c1_T * c3c1_B - c3c1_T * c2c1_B;   
    if (ROUNDOFF(fDenominator) == 0.0f)    
    {   
        pTBNMatrix[0] = CVector3(1.0f, 0.0f, 0.0f);   
        pTBNMatrix[1] = CVector3(0.0f, 1.0f, 0.0f);   
        pTBNMatrix[2] = CVector3(0.0f, 0.0f, 1.0f);   
    }   
    else   
    {   
        float fScale1 = 1.0f / fDenominator;   
   
        CVector3 T, B, N;   
        T = CVector3((c3c1_B * v2v1.x - c2c1_B * v3v1.x) * fScale1,   
                    (c3c1_B * v2v1.y - c2c1_B * v3v1.y) * fScale1,   
                    (c3c1_B * v2v1.z - c2c1_B * v3v1.z) * fScale1);   
   
        B = CVector3((-c3c1_T * v2v1.x + c2c1_T * v3v1.x) * fScale1,   
                    (-c3c1_T * v2v1.y + c2c1_T * v3v1.y) * fScale1,   
                    (-c3c1_T * v2v1.z + c2c1_T * v3v1.z) * fScale1);   
   
        N = N.Cross(T, B);   
   
        float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) + (B.x * N.y * T.z - B.z * N.y * T.x) + (N.x * T.y * B.z - N.z * T.y * B.x));   
           
        pTBNMatrix[0].x = B.Cross(B, N).x * fScale2;   
        pTBNMatrix[0].y = N.Negate(N.Cross(N, T)).x * fScale2;   
        pTBNMatrix[0].z = T.Cross(T, B).x * fScale2;   
        pTBNMatrix[0].Normalize();   
   
        pTBNMatrix[1].x = B.Negate(B.Cross(B, N)).y * fScale2;   
        pTBNMatrix[1].y = N.Cross(N, T).y * fScale2;   
        pTBNMatrix[1].z = T.Negate(T.Cross(T, B)).y * fScale2;   
        pTBNMatrix[1].Normalize();   
   
        pTBNMatrix[2].x = B.Cross(B, N).z * fScale2;   
        pTBNMatrix[2].y = N.Negate(N.Cross(N, T)).z * fScale2;   
        pTBNMatrix[2].z = T.Cross(T, B).z * fScale2;   
        pTBNMatrix[2].Normalize();   
    }   */
}   
   
CVector4::CVector4()   
{   
   // Initialize the variables to 0.   
   x = y = z = 0.0;   
}   
   
   
CVector4::CVector4(float X, float Y, float Z)   
{   
   // Initialize the varibles to the data in X, Y, and Z.   
   x = X;   
   y = Y;   
   z = Z;   
}   
   
   
CVector4::CVector4(float X, float Y, float Z, float W)   
{   
   // Initialize the varibles to the data in X, Y, and Z.   
   x = X;   
   y = Y;   
   z = Z;   
   w = W;   
}   
   
CVector4::CVector4(const CVector3 &v)   
{   
    x = v.x;   
    y = v.y;   
    z = v.z;   
    w = 1.0f;   
}   
   
CVector4::CVector4(const CVector4 &v)   
{   
   // Initialize this object to v.   
   x = v.x;   
   y = v.y;   
   z = v.z;   
   w = v.w;   
}   
   
   
void CVector4::operator =(CVector4 v)   
{   
   // Make this objects x, y, and z equal to the object on the right of the = sign.   
   x = v.x;   
   y = v.y;   
   z = v.z;   
   w = v.w;   
}   
   
   
CVector4 CVector4::operator -(CVector4 v)   
{   
   // Return the value of this vector - v.   
   return CVector4(x - v.x, y - v.y, z - v.z);   
}   
   
   
CVector4 CVector4::operator +(CVector4 v)   
{   
   // Return the value of this vector + v.   
   return CVector4(x + v.x, y + v.y, z + v.z);   
}   
   
   
CVector4 CVector4::operator *(CVector4 v)   
{   
   // Return the value of this vector * v.   
   return CVector4(x * v.x, y * v.y, z * v.z);   
}   
   
   
CVector4 CVector4::operator /(CVector4 v)   
{   
   // Return the value of this vector / v.   
   return CVector4(x / v.x, y / v.y, z / v.z);   
}   
   
   
CVector4 CVector4::operator +(float f)   
{   
   // Return the value of this vector + f.   
   return CVector4(x + f, y + f, z + f);   
}   
   
   
CVector4 CVector4::operator -(float f)   
{   
   // Return the value of this vector - f.   
   return CVector4(x - f, y - f, z - f);   
}   
   
   
CVector4 CVector4::operator *(float f)   
{   
   // Return the value of this vector * f.   
   return CVector4(x * f, y * f, z * f);   
}   
   
   
CVector4 CVector4::operator /(float f)   
{   
   // Return the value of this vector / f.  We do this by multiplying the recip.   
   f = 1/f;   
   
   return CVector4(x * f, y * f, z * f);   
}   
   
   
void CVector4::operator +=(CVector4 v)   
{   
   // Add this by object v and store results here.   
   x += v.x;   
   y += v.y;   
   z += v.z;   
}   
   
   
void CVector4::operator -=(CVector4 v)   
{   
   // Subtract this by object v and store results here.   
   x -= v.x;   
   y -= v.y;   
   z -= v.z;   
}   
   
   
void CVector4::operator *=(CVector4 v)   
{   
   // Mul this by object v and store results here.   
   x *= v.x;   
   y *= v.y;   
   z *= v.z;   
}   
   
   
void CVector4::operator /=(CVector4 v)   
{   
   // Divide this by object v and store results here.   
   x /= v.x;   
   y /= v.y;   
   z /= v.z;   
}   
   
   
void CVector4::operator +=(float f)   
{   
   // Add this by object f and store results here.   
   x += f;   
   y += f;   
   z += f;   
}   
   
   
void CVector4::operator -=(float f)   
{   
   // Subract this by object f and store results here.   
   x -= f;   
   y -= f;   
   z -= f;   
}   
   
   
void CVector4::operator *=(float f)   
{   
   // Multiply this by object f and store results here.   
   x *= f;   
   y *= f;   
   z *= f;   
}   
   
   
void CVector4::operator /=(float f)   
{   
   // Divide this by object f and store results here by multiplying by the recip.   
   f = 1/f;   
   
   x *= f;   
   y *= f;   
   z *= f;   
}   
   
   
bool CVector4::operator ==(CVector4 v)   
{   
   // Return true if all equal each other, false if one or more don't.   
   return ((x == v.x) && (y== v.y) && (z == v.z));   
}   
   
   
bool CVector4::operator !=(CVector4 v)   
{   
   // Return true if one or all don't equal each other, false if they equal.   
   return !((x == v.x) && (y== v.y) && (z == v.z));   
}   
   
   
void CVector4::CrossProduct(CVector4 v1, CVector4 v2)   
{   
   // Get the cross product of v1 and v2 and store it in this vector.   
   x = ((v1.y * v2.z) - (v1.z * v2.y));   
   y = ((v1.z * v2.x) - (v1.x * v2.z));   
   z = ((v1.x * v2.y) - (v1.y * v2.x));   
}   
   
   
void CVector4::CrossProduct3(CVector4 v1, CVector4 v2, CVector4 v3)   
{   
   // Get the cross product of v1, v2, and v3.   
   x = v1.y * v2.z * v3.w +   
       v1.z * v2.w * v3.y +   
       v1.w * v2.y * v3.z -    
       v1.y * v2.w * v3.z -   
       v1.z * v2.y * v3.w -   
       v1.w * v2.z * v3.y;   
   
   y = v1.x * v2.w * v3.z +   
       v1.z * v2.x * v3.w +   
       v1.w * v2.z * v3.x -   
       v1.x * v2.z * v3.w -   
       v1.z * v2.w * v3.x -   
       v1.w * v2.x * v3.z;   
   
   z = v1.x * v2.y * v3.w +   
       v1.y * v2.w * v3.x +   
       v1.w * v2.x * v3.y -   
       v1.x * v2.w * v3.y -   
       v1.y * v2.x * v3.w -   
       v1.w * v2.y * v3.x;   
   
   w = v1.x * v2.z * v3.y +   
       v1.y * v2.x * v3.z +   
       v1.z * v2.y * v3.x -   
       v1.x * v2.y * v3.z -   
       v1.y * v2.z * v3.x -   
       v1.z * v2.x * v3.y;   
}   
   
   
float CVector4::DotProduct3(CVector4 v1)   
{   
   // Get the dot product of v1 and this object and return it.   
   return x * v1.x + y * v1.y + z * v1.z;   
}   
   
   
float CVector4::DotProduct4(CVector4 v1)   
{   
   // Get the dot product of v1 and this object and return it.   
   return x * v1.x + y * v1.y + z * v1.z + w * v1.w;   
}   
   
   
float CVector4::GetLength()   
{   
   // Return the length for this object.   
   return (float)sqrt((x * x + y * y + z * z));   
}   
   
   
void CVector4::Normalize()   
{   
   // Reduce this object to a unit vector.   
   float lenght = GetLength();   
   
   if(lenght == 0.0f)   
      lenght = 1.0f;   
   
   x = x/lenght;   
   y = y/lenght;   
   z = z/lenght;   
   w = w/lenght;   
}   
   
   
void CVector4::CalcNormal(CVector4 Triangle[])   
{   
   // Normalize a triangle and store results in this object.   
   CVector4 v1, v2;   
   
   v1.x = Triangle[0].x - Triangle[1].x;   
   v1.y = Triangle[0].y - Triangle[1].y;   
   v1.z = Triangle[0].z - Triangle[1].z;   
   v1.w = Triangle[0].w - Triangle[1].w;   
   
   v2.x = Triangle[1].x - Triangle[2].x;   
   v2.y = Triangle[1].y - Triangle[2].y;   
   v2.z = Triangle[1].z - Triangle[2].z;   
   v2.w = Triangle[1].w - Triangle[2].w;   
   
   CrossProduct(v1, v2);   
   Normalize();   
}   
   
   
void CVector4::CalculateTangentVector(CVector4 Point1, CVector4 Point2, CVector4 Point3,   
                                      CVector4 NormalOfA, CTexCoord P1Tex, CTexCoord P2Tex,   
                                      CTexCoord P3Tex)   
{   
   // Get the vector between point 1 and point 2.   
   CVector4 VectorAB = Point2 - Point1;   
   
   // Get the vector between point 1 and point 3.   
    CVector4 VectorAC = Point3 - Point1;   
   
   // Compute the components of the vectors to the vertex normal of the first point.   
   CVector4 ProjAB = VectorAB - (NormalOfA * NormalOfA.DotProduct4(VectorAB));   
   CVector4 ProjAC = VectorAC - (NormalOfA * NormalOfA.DotProduct4(VectorAC));   
   
   // Calculate the tu difference of point 2 and 1 then of point 3 and 1.   
   float TexCoorUAB = P2Tex.tu - P1Tex.tu;   
   float TexCoorUAC = P3Tex.tu - P1Tex.tu;   
   
    // Calculate the tv difference of point 2 and 1 then of point 3 and 1.   
   float TexCoorVAB = P2Tex.tv - P1Tex.tv;   
   float TexCoorVAC = P3Tex.tv - P1Tex.tv;   
   
   // Switch the sign if the u of point 1 and 3 * v of 1 and 2 is greater than u of 1 and 2 *   
   // v of point 1 and 3.   
   if((TexCoorUAC * TexCoorVAB) > (TexCoorUAB * TexCoorVAC))   
      {   
         TexCoorUAC = -TexCoorUAC;   
         TexCoorUAB = -TexCoorUAB;   
      }   
   
   // Calculate the tangent vector, normalize it, then return it (the normal values).   
   CVector4 Tangent = (ProjAB * TexCoorUAC) - (ProjAC * TexCoorUAB);   
   Tangent.Normalize();   
   
   *this += Tangent;   
}   
   
   
void CVector4::ExtendVertexPos(CVector4 currentVertex, CVector4 lightPos, float Extend)   
{   
    CVector4 lightDir;  // Direction of the light to the vertex position.   
    CVector4 newPos;    // New extended vertex position to make up the shadow volume.   
   
    // Get the light direction from the vertex position and the light position.   
    lightDir = currentVertex - lightPos;   
   
    // Now that we know where its going we add it to the position of the light so   
    // we get the correct, new position.  We multiply it by a passed it value to   
    // give the volume some distance or things won't come out quite as we want.   
    newPos = lightPos + lightDir * Extend;   
   
    x = newPos.x;   
    y = newPos.y;   
    z = newPos.z;   
    w = newPos.w;   
}   
   
   
void CVector4::ExtendVertexPos(CVector4 lightPos, float Extend)   
{   
    CVector4 lightDir;  // Direction of the light to the vertex position.   
    CVector4 newPos;    // New extended vertex position to make up the shadow volume.   
   
    // Get the light direction from the vertex position and the light position.   
    lightDir = CVector4(x, y, z, w) - lightPos;   
   
    // Now that we know where its going we add it to the position of the light so   
    // we get the correct, new position.  We multiply it by a passed it value to   
    // give the volume some distance or things won't come out quite as we want.   
    newPos = lightPos + lightDir * Extend;   
   
    x = newPos.x;   
    y = newPos.y;   
    z = newPos.z;   
    w = newPos.w;   
}   
   
   
CVector4 CVector4::GetRotatedX(double angle)   
{   
   float sinAngle = (float)sin(PI * angle / 180);   
    float cosAngle = (float)cos(PI * angle / 180);   
   
    return CVector4(x, y * cosAngle - z * sinAngle, y * sinAngle + z * cosAngle, w);   
}   
   
   
CVector4 CVector4::GetRotatedY(double angle)   
{   
   float sinAngle = (float)sin(PI * angle / 180);   
    float cosAngle = (float)cos(PI * angle / 180);   
   
    return CVector4(x * cosAngle + z * sinAngle, y, -x * sinAngle + z * cosAngle, w);   
}   
   
   
CVector4 CVector4::GetRotatedZ(double angle)   
{   
   float sinAngle = (float)sin(PI * angle / 180);   
    float cosAngle = (float)cos(PI * angle / 180);   
   
    return CVector4(x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle, z, w);   
}   
   
   
CVector4 CVector4::GetRotatedAxis(double angle, CVector4 axis)   
{   
    if(angle == 0.0) return(*this);   
   
    axis.Normalize();   
   
    CVector4 RotationRow1, RotationRow2, RotationRow3;   
   
    double newAngle = PI * angle / 180;   
    float sinAngle = (float)sin(newAngle);   
    float cosAngle = (float)cos(newAngle);   
    float oneSubCos = 1.0f - cosAngle;   
   
    RotationRow1.x = (axis.x) * (axis.x) + cosAngle * (1 - (axis.x) * (axis.x));   
    RotationRow1.y = (axis.x) * (axis.y) * (oneSubCos) - sinAngle * axis.z;   
    RotationRow1.z = (axis.x) * (axis.z) * (oneSubCos) + sinAngle * axis.y;   
   
    RotationRow2.x = (axis.x) * (axis.y) * (oneSubCos) + sinAngle * axis.z;   
    RotationRow2.y = (axis.y) * (axis.y) + cosAngle * (1 - (axis.y) * (axis.y));   
    RotationRow2.z = (axis.y) * (axis.z) * (oneSubCos) - sinAngle * axis.x;   
       
    RotationRow3.x = (axis.x) * (axis.z) * (oneSubCos) - sinAngle * axis.y;   
    RotationRow3.y = (axis.y) * (axis.z) * (oneSubCos) + sinAngle * axis.x;   
    RotationRow3.z = (axis.z) * (axis.z) + cosAngle * (1 - (axis.z) * (axis.z));   
   
    return CVector4(this->DotProduct3(RotationRow1),   
                   this->DotProduct3(RotationRow2),   
                   this->DotProduct3(RotationRow3));   
}   
   
   
void CVector4::CalculateBinormalVector(CVector4 tangent, CVector4 normal)   
{   
   this->CrossProduct(tangent, normal);   
}   
   
   
void CVector4::ClampTo01()   
{   
   CVector4 temp(x, y, z, w);   
   temp.Normalize();   
   temp = temp * 0.5f + CVector4(0.5f, 0.5f, 0.5f);   
   
   x = temp.x;   
   y = temp.y;   
   z = temp.z;   
}   
   






 



CMatrix4x4::CMatrix4x4()   
{   
   // Initialize all array values.   
   Clear();   
}   
   
   
CMatrix4x4::CMatrix4x4(const CMatrix4x4 &m)   
{   
   matrix[0]  = m.matrix[0];   
   matrix[4]  = m.matrix[4];   
   matrix[8]  = m.matrix[8];   
   matrix[12] = m.matrix[12];   
   
   matrix[1]  = m.matrix[1];   
   matrix[5]  = m.matrix[5];   
   matrix[9]  = m.matrix[9];   
   matrix[13] = m.matrix[13];   
   
   matrix[2]  = m.matrix[2];   
   matrix[6]  = m.matrix[6];   
   matrix[10] = m.matrix[10];   
   matrix[14] = m.matrix[14];   
   
   matrix[3]  = m.matrix[3];   
   matrix[7]  = m.matrix[7];   
   matrix[11] = m.matrix[11];   
   matrix[15] = m.matrix[15];   
}   
   
   
CMatrix4x4::CMatrix4x4(float r11, float r12, float r13, float r14,   
                       float r21, float r22, float r23, float r24,   
                       float r31, float r32, float r33, float r34,   
                       float r41, float r42, float r43, float r44)   
{   
    // To set the matrix identity you set all the values in the matrix like so...   
    matrix[0]  = r11; matrix[1]  = r12; matrix[2]  = r13; matrix[3]  = r14;   
    matrix[4]  = r21; matrix[5]  = r22; matrix[6]  = r23; matrix[7]  = r24;   
    matrix[8]  = r31; matrix[9]  = r32; matrix[10] = r33; matrix[11] = r34;   
    matrix[12] = r41; matrix[13] = r42; matrix[14] = r43; matrix[15] = r44;   
}   
   
   
CMatrix4x4::~CMatrix4x4()   
{   
    
}   
 
const float CMatrix4x4::operator[](int row) const
{
    return matrix[row];
}

float CMatrix4x4::operator[](int row)
{
    return matrix[row];
}


CVector4 CMatrix4x4::GetRow(int position) const   
{   
    if(position==0)   
        return CVector4(matrix[0], matrix[4], matrix[8], matrix[12]);   
       
    if(position==1)   
        return CVector4(matrix[1], matrix[5], matrix[9], matrix[13]);   
       
    if(position==2)   
        return CVector4(matrix[2], matrix[6], matrix[10], matrix[14]);   
       
    if(position==3)   
        return CVector4(matrix[3], matrix[7], matrix[11], matrix[15]);   
   
    return CVector4(0.0f, 0.0f, 0.0f, 0.0f);   
}   
   
CVector4 CMatrix4x4::GetColumn(int position) const   
{   
    if(position==0)   
        return CVector4(matrix[0], matrix[1], matrix[2], matrix[3]);   
       
    if(position==1)   
        return CVector4(matrix[4], matrix[5], matrix[6], matrix[7]);   
       
    if(position==2)   
        return CVector4(matrix[8], matrix[9], matrix[10], matrix[11]);   
       
    if(position==3)   
        return CVector4(matrix[12], matrix[13], matrix[14], matrix[15]);   
   
    return CVector4(0.0f, 0.0f, 0.0f, 0.0f);   
}   
   
void CMatrix4x4::Transpose()   
{   
    CMatrix4x4 backup;   
    backup = *this;   
   
    matrix[0] = backup.matrix[0];   
    matrix[1] = backup.matrix[4];   
    matrix[2] = backup.matrix[8];   
    matrix[3] = backup.matrix[12];   
    matrix[4] = backup.matrix[1];   
    matrix[5] = backup.matrix[5];   
    matrix[6] = backup.matrix[9];   
    matrix[7] = backup.matrix[13];   
    matrix[8] = backup.matrix[2];   
    matrix[9] = backup.matrix[6];   
    matrix[10] = backup.matrix[10];   
    matrix[11] = backup.matrix[14];   
    matrix[12] = backup.matrix[3];   
    matrix[13] = backup.matrix[7];   
    matrix[14] = backup.matrix[11];   
    matrix[15] = backup.matrix[15];   
}   
  
void CMatrix4x4::operator = (float _Matrix[16])
{
	matrix[0] = _Matrix[0];
	matrix[1] = _Matrix[1];
	matrix[2] = _Matrix[2];
	matrix[3] = _Matrix[3];
	matrix[4] = _Matrix[4];
	matrix[5] = _Matrix[5];
	matrix[6] = _Matrix[6];
	matrix[7] = _Matrix[7];
	matrix[8] = _Matrix[8];
	matrix[9] = _Matrix[9];
	matrix[10] = _Matrix[10];
	matrix[11] = _Matrix[11];
	matrix[12] = _Matrix[12];
	matrix[13] = _Matrix[13];
	matrix[14] = _Matrix[14];
	matrix[15] = _Matrix[15];
}
void CMatrix4x4::operator =(CMatrix4x4 m)   
{   
   // Set the matrix values to the values sent in to m.   
   matrix[0]  = m.matrix[0]; matrix[1]   = m.matrix[1]; matrix[2]   = m.matrix[2];   
   matrix[3]  = m.matrix[3];   
    matrix[4]  = m.matrix[4]; matrix[5]   = m.matrix[5]; matrix[6]   = m.matrix[6];   
   matrix[7]  = m.matrix[7];   
    matrix[8]  = m.matrix[8]; matrix[9]   = m.matrix[9]; matrix[10]  = m.matrix[10];   
   matrix[11] = m.matrix[11];   
    matrix[12] = m.matrix[12]; matrix[13] = m.matrix[13]; matrix[14] = m.matrix[14];   
   matrix[15] = m.matrix[15];   
}   
   
   
CMatrix4x4 CMatrix4x4::operator -(CMatrix4x4 m)   
{   
   // Return the value of this matrix - m.   
   return CMatrix4x4(matrix[0] - m.matrix[0], matrix[1] - m.matrix[1], matrix[2] - m.matrix[2],   
                     matrix[3] - m.matrix[3], matrix[4] - m.matrix[4], matrix[5] - m.matrix[5],   
                     matrix[6] - m.matrix[6], matrix[7] - m.matrix[7], matrix[8] - m.matrix[8],   
                     matrix[9] - m.matrix[9], matrix[10] - m.matrix[10], matrix[11] - m.matrix[11],   
                     matrix[12] - m.matrix[12], matrix[13] - m.matrix[13],   
                     matrix[14] - m.matrix[14], matrix[15] - m.matrix[15]);   
}   
   
   
CMatrix4x4 CMatrix4x4::operator +(CMatrix4x4 m)   
{   
   // Return the value of this matrix + m.   
   return CMatrix4x4(matrix[0] + m.matrix[0], matrix[1] + m.matrix[1], matrix[2] + m.matrix[2],   
                     matrix[3] + m.matrix[3], matrix[4] + m.matrix[4], matrix[5] + m.matrix[5],   
                     matrix[6] + m.matrix[6], matrix[7] + m.matrix[7], matrix[8] + m.matrix[8],   
                     matrix[9] + m.matrix[9], matrix[10] + m.matrix[10], matrix[11] + m.matrix[11],   
                     matrix[12] + m.matrix[12], matrix[13] + m.matrix[13],   
                     matrix[14] + m.matrix[14], matrix[15] + m.matrix[15]);   
}   
   
   
CMatrix4x4 CMatrix4x4::operator *(CMatrix4x4 m)   
{   
   // Return the value of this vector * m.   
   return CMatrix4x4(matrix[0] * m.matrix[0] + matrix[4] * m.matrix[1] + matrix[8] *   
                     m.matrix[2] + matrix[12] * m.matrix[3],   
                           matrix[1] * m.matrix[0] + matrix[5] * m.matrix[1] + matrix[9] *   
                     m.matrix[2] + matrix[13] * m.matrix[3],   
                           matrix[2] * m.matrix[0] + matrix[6] * m.matrix[1] + matrix[10] *   
                     m.matrix[2] + matrix[14] * m.matrix[3],   
                           matrix[3] * m.matrix[0] + matrix[7] * m.matrix[1] + matrix[11] *   
                     m.matrix[2] + matrix[15] * m.matrix[3],   
                           matrix[0] * m.matrix[4] + matrix[4] * m.matrix[5] + matrix[8] *   
                     m.matrix[6] + matrix[12] * m.matrix[7],   
                           matrix[1] * m.matrix[4] + matrix[5] * m.matrix[5] + matrix[9] *   
                     m.matrix[6] + matrix[13] * m.matrix[7],   
                           matrix[2] * m.matrix[4] + matrix[6] * m.matrix[5] + matrix[10] *   
                     m.matrix[6] + matrix[14] * m.matrix[7],   
                           matrix[3] * m.matrix[4] + matrix[7] * m.matrix[5] + matrix[11] *   
                     m.matrix[6] + matrix[15] * m.matrix[7],   
                           matrix[0] * m.matrix[8] + matrix[4] * m.matrix[9] + matrix[8] *   
                     m.matrix[10] + matrix[12] * m.matrix[11],   
                           matrix[1] * m.matrix[8] + matrix[5] * m.matrix[9] + matrix[9] *   
                     m.matrix[10] + matrix[13] * m.matrix[11],   
                           matrix[2] * m.matrix[8] + matrix[6] * m.matrix[9] + matrix[10] *   
                     m.matrix[10] + matrix[14] * m.matrix[11],   
                           matrix[3] * m.matrix[8] + matrix[7] * m.matrix[9] + matrix[11] *   
                     m.matrix[10] + matrix[15] * m.matrix[11],   
                           matrix[0] * m.matrix[12] + matrix[4] * m.matrix[13] + matrix[8] *   
                     m.matrix[14] + matrix[12] * m.matrix[15],   
                           matrix[1] * m.matrix[12] + matrix[5] * m.matrix[13] + matrix[9] *   
                     m.matrix[14] + matrix[13] * m.matrix[15],   
                           matrix[2] * m.matrix[12] + matrix[6] * m.matrix[13] + matrix[10] *   
                     m.matrix[14] + matrix[14] * m.matrix[15],   
                           matrix[3] * m.matrix[12] + matrix[7] * m.matrix[13] + matrix[11] *   
                     m.matrix[14] + matrix[15] * m.matrix[15]);   
}   
   
   
CMatrix4x4 CMatrix4x4::operator /(CMatrix4x4 m)   
{   
   return CMatrix4x4(matrix[0] / m.matrix[0] + matrix[4] / m.matrix[1] + matrix[8] /   
                     m.matrix[2] + matrix[12] / m.matrix[3],   
                           matrix[1] / m.matrix[0] + matrix[5] / m.matrix[1] + matrix[9] /   
                     m.matrix[2] + matrix[13] / m.matrix[3],   
                           matrix[2] / m.matrix[0] + matrix[6] / m.matrix[1] + matrix[10] /   
                     m.matrix[2] + matrix[14] / m.matrix[3],   
                           matrix[3] / m.matrix[0] + matrix[7] / m.matrix[1] + matrix[11] /   
                     m.matrix[2] + matrix[15] / m.matrix[3],   
                           matrix[0] / m.matrix[4] + matrix[4] / m.matrix[5] + matrix[8] /   
                     m.matrix[6] + matrix[12] / m.matrix[7],   
                           matrix[1] / m.matrix[4] + matrix[5] / m.matrix[5] + matrix[9] /   
                     m.matrix[6] + matrix[13] / m.matrix[7],   
                           matrix[2] / m.matrix[4] + matrix[6] / m.matrix[5] + matrix[10] /   
                     m.matrix[6] + matrix[14] / m.matrix[7],   
                           matrix[3] / m.matrix[4] + matrix[7] / m.matrix[5] + matrix[11] /   
                     m.matrix[6] + matrix[15] / m.matrix[7],   
                           matrix[0] / m.matrix[8] + matrix[4] / m.matrix[9] + matrix[8] /   
                     m.matrix[10] + matrix[12] / m.matrix[11],   
                           matrix[1] / m.matrix[8] + matrix[5] / m.matrix[9] + matrix[9] /   
                     m.matrix[10] + matrix[13] / m.matrix[11],   
                           matrix[2] / m.matrix[8] + matrix[6] / m.matrix[9] + matrix[10] /   
                     m.matrix[10] + matrix[14] / m.matrix[11],   
                           matrix[3] / m.matrix[8] + matrix[7] / m.matrix[9] + matrix[11] /   
                     m.matrix[10] + matrix[15] / m.matrix[11],   
                           matrix[0] / m.matrix[12] + matrix[4] / m.matrix[13] + matrix[8] /   
                     m.matrix[14] + matrix[12] / m.matrix[15],   
                           matrix[1] / m.matrix[12] + matrix[5] / m.matrix[13] + matrix[9] /   
                     m.matrix[14] + matrix[13] / m.matrix[15],   
                           matrix[2] / m.matrix[12] + matrix[6] / m.matrix[13] + matrix[10] /   
                     m.matrix[14] + matrix[14] / m.matrix[15],   
                           matrix[3] / m.matrix[12] + matrix[7] / m.matrix[13] + matrix[11] /   
                     m.matrix[14] + matrix[15] / m.matrix[15]);   
}   
   
   
CMatrix4x4 CMatrix4x4::operator -(float f)   
{   
   return CMatrix4x4(matrix[0]  - f, matrix[1]  - f, matrix[2]  - f, matrix[3]  - f,   
                     matrix[4]  - f, matrix[5]  - f, matrix[6]  - f, matrix[7]  - f,   
                     matrix[8]  - f, matrix[9]  - f, matrix[10] - f, matrix[11] - f,   
                     matrix[12] - f, matrix[13] - f, matrix[14] - f, matrix[15] - f);   
}   
   
   
CMatrix4x4 CMatrix4x4::operator +(float f)   
{   
   return CMatrix4x4(matrix[0]  + f, matrix[1]  + f, matrix[2]  + f, matrix[3]  + f,   
                     matrix[4]  + f, matrix[5]  + f, matrix[6]  + f, matrix[7]  + f,   
                     matrix[8]  + f, matrix[9]  + f, matrix[10] + f, matrix[11] + f,   
                     matrix[12] + f, matrix[13] + f, matrix[14] + f, matrix[15] + f);   
}   
   
   
CMatrix4x4 CMatrix4x4::operator *(float f)   
{   
   return CMatrix4x4(matrix[0]  * f, matrix[1]  * f, matrix[2]  * f, matrix[3]  * f,   
                     matrix[4]  * f, matrix[5]  * f, matrix[6]  * f, matrix[7]  * f,   
                     matrix[8]  * f, matrix[9]  * f, matrix[10] * f, matrix[11] * f,   
                     matrix[12] * f, matrix[13] * f, matrix[14] * f, matrix[15] * f);   
}   
   
   
CMatrix4x4 CMatrix4x4::operator /(float f)   
{   
   // Return the value of this vector / f.  We do this by multiplying the recip.   
   if(f == 0) f = 1;   
   f = 1/f;   
   
   return CMatrix4x4(matrix[0]  * f, matrix[1]  * f, matrix[2]  * f, matrix[3]  * f,   
                     matrix[4]  * f, matrix[5]  * f, matrix[6]  * f, matrix[7]  * f,   
                     matrix[8]  * f, matrix[9]  * f, matrix[10] * f, matrix[11] * f,   
                     matrix[12] * f, matrix[13] * f, matrix[14] * f, matrix[15] * f);   
}   
   
   
bool CMatrix4x4::operator ==(CMatrix4x4 m)   
{   
   // Return true if all equal each other, false if one or more don't.   
   for(int i = 0; i < 16; i++)   
      {   
         if(matrix[i] != m.matrix[i])   
            return false;   
      }   
   
   return true;   
}   
   
   
void CMatrix4x4::operator +=(CMatrix4x4 m)   
{   
   (*this) = (*this) + m;   
}   
   
   
void CMatrix4x4::operator -=(CMatrix4x4 m)   
{   
   (*this) = (*this) - m;   
}   
   
   
void CMatrix4x4::operator *=(CMatrix4x4 m)   
{   
   (*this) = (*this) * m;   
}   
   
   
void CMatrix4x4::operator /=(CMatrix4x4 m)   
{   
   (*this) = (*this) / m;   
}   
   
   
bool CMatrix4x4::operator !=(CMatrix4x4 m)   
{   
   return !((*this) == m);   
}   
   
   
void CMatrix4x4::operator -=(float f)   
{   
   (*this) = (*this) - f;   
}   
   
   
void CMatrix4x4::operator +=(float f)   
{   
   (*this) = (*this) + f;   
}   
   
   
void CMatrix4x4::operator *=(float f)   
{   
   (*this) = (*this) * f;   
}   
   
   
void CMatrix4x4::operator /=(float f)   
{   
   // Divide this by object f and store results here.   
   (*this) = (*this) / f;   
}   
   
   
void CMatrix4x4::Clear()   
{   
   // To set the matrix identity you set all the values in the matrix like so...   
   matrix[0]  = 1.0f; matrix[1]  = 0.0f; matrix[2]  = 0.0f; matrix[3]  = 0.0f;   
    matrix[4]  = 0.0f; matrix[5]  = 1.0f; matrix[6]  = 0.0f; matrix[7]  = 0.0f;   
    matrix[8]  = 0.0f; matrix[9]  = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;   
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;   
   
   // If you remember matrices from math class you will notice that there is a diagonal   
   // line of 1's at matrix[0], [5], [10], and [15].  This is how you reset a matrix.   
}   
   
   
void CMatrix4x4::Zero()   
{   
   // To set the matrix to zero you set all the values in the matrix like so...   
   matrix[0]  = 0.0f; matrix[1]  = 0.0f; matrix[2]  = 0.0f; matrix[3]  = 0.0f;   
    matrix[4]  = 0.0f; matrix[5]  = 0.0f; matrix[6]  = 0.0f; matrix[7]  = 0.0f;   
    matrix[8]  = 0.0f; matrix[9]  = 0.0f; matrix[10] = 0.0f; matrix[11] = 0.0f;   
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 0.0f;   
}   
   
   
void CMatrix4x4::Translate(CVector4 Translate)   
{   
   Clear();   
   
   // To translate a 4x4 matrix you must replace the bottom row values.  The first   
   // which is matrix[12] is for x, [13] is the y, and so on.  The last one is set to 1.0.   
   matrix[12] = Translate.x;   
   matrix[13] = Translate.y;   
   matrix[14] = Translate.z;   
   matrix[15] = 1.0f;   
}   
   
   
void CMatrix4x4::Translate(float x, float y, float z)   
{   
   Clear();   
   
   // To translate a 4x4 matrix you must replace the bottom row values.  The first   
   // which is matrix[12] is for x, [13] is the y, and so on.  The last one is set to 1.0.   
   matrix[12] = x;   
   matrix[13] = y;   
   matrix[14] = z;   
   matrix[15] = 1.0f;   
}   
   
   
void CMatrix4x4::Rotate(double angle, int x, int y, int z)   
{   
   float sine = (float)sin(angle);   
   float cosine = (float)cos(angle);   
   
   if(x)   
      {   
          matrix[5] = cosine;   
          matrix[6] = sine;   
          matrix[9] = -sine;   
          matrix[10] = cosine;   
      }   
   
   if(y)   
      {   
          matrix[0] = cosine;   
          matrix[2] = -sine;   
          matrix[8] = sine;   
          matrix[10] = cosine;   
      }   
   
   if(z)   
      {   
          matrix[0] = cosine;   
          matrix[1] = sine;   
          matrix[4] = -sine;   
          matrix[5] = cosine;   
      }   
}   
   
   
bool CMatrix4x4::inverseMatrix(CMatrix4x4 m)   
{   
   float tempMatrix[16] = {0};   
   float d12, d13, d23, d24, d34, d41;   
   
    d12 = m.matrix[2]  * m.matrix[7]  - m.matrix[3]  * m.matrix[6];   
    d13 = m.matrix[2]  * m.matrix[11] - m.matrix[3]  * m.matrix[10];   
    d23 = m.matrix[6]  * m.matrix[11] - m.matrix[7]  * m.matrix[10];   
    d24 = m.matrix[6]  * m.matrix[15] - m.matrix[7]  * m.matrix[14];   
    d34 = m.matrix[10] * m.matrix[15] - m.matrix[11] * m.matrix[14];   
    d41 = m.matrix[14] * m.matrix[3]  - m.matrix[15] * m.matrix[2];   
   
    tempMatrix[0] =   m.matrix[5] * d34 - m.matrix[9] * d24 + m.matrix[13] * d23;   
    tempMatrix[1] = -(m.matrix[1] * d34 + m.matrix[9] * d41 + m.matrix[13] * d13);   
    tempMatrix[2] =   m.matrix[1] * d24 + m.matrix[5] * d41 + m.matrix[13] * d12;   
    tempMatrix[3] = -(m.matrix[1] * d23 - m.matrix[5] * d13 + m.matrix[9]  * d12);   
   
    // Calculate the determinant.   
    float determinant = m.matrix[0] * tempMatrix[0] + m.matrix[4] * tempMatrix[1] +   
                       m.matrix[8] * tempMatrix[2] + m.matrix[12] * tempMatrix[3];   
   
    // Clear if the determinant is equal to zero.  0 means matrix have no inverse.   
    if(determinant == 0.0)   
       {   
           Clear();   
         return false;   
       }   
   
    float invDeterminant = 1.0f / determinant;   
       
    // Compute rest of inverse.   
    tempMatrix[0] *= invDeterminant;   
    tempMatrix[1] *= invDeterminant;   
    tempMatrix[2] *= invDeterminant;   
    tempMatrix[3] *= invDeterminant;   
   
    tempMatrix[4] = -(m.matrix[4] * d34 - m.matrix[8] * d24 + m.matrix[12] * d23) * invDeterminant;   
    tempMatrix[5] =   m.matrix[0] * d34 + m.matrix[8] * d41 + m.matrix[12] * d13  * invDeterminant;   
    tempMatrix[6] = -(m.matrix[0] * d24 + m.matrix[4] * d41 + m.matrix[12] * d12) * invDeterminant;   
    tempMatrix[7] =   m.matrix[0] * d23 - m.matrix[4] * d13 + m.matrix[8]  * d12  * invDeterminant;   
   
    // Pre-compute 2x2 dets for first two rows when computing cofactors    
    // of last two rows.   
    d12 = m.matrix[0]  * m.matrix[5]  - m.matrix[1]  * m.matrix[12];   
    d13 = m.matrix[0]  * m.matrix[9]  - m.matrix[1]  * m.matrix[8];   
    d23 = m.matrix[4]  * m.matrix[9]  - m.matrix[5]  * m.matrix[8];   
    d24 = m.matrix[4]  * m.matrix[13] - m.matrix[5]  * m.matrix[12];   
    d34 = m.matrix[8]  * m.matrix[13] - m.matrix[9]  * m.matrix[12];   
    d41 = m.matrix[12] * m.matrix[1]  - m.matrix[13] * m.matrix[0];   
   
    tempMatrix[8]  =   m.matrix[7] * d34 - m.matrix[11] * d24 + m.matrix[15] * d23 * invDeterminant;   
    tempMatrix[9]  = -(m.matrix[3] * d34 + m.matrix[11] * d41 + m.matrix[15] * d13) * invDeterminant;   
    tempMatrix[10] =   m.matrix[3] * d24 + m.matrix[7]  * d41 + m.matrix[15] * d12 * invDeterminant;   
    tempMatrix[11] = -(m.matrix[3] * d23 - m.matrix[7]  * d13 + m.matrix[11] * d12) * invDeterminant;   
    tempMatrix[12] = -(m.matrix[6] * d34 - m.matrix[10] * d24 + m.matrix[14] * d23) * invDeterminant;   
    tempMatrix[13] =   m.matrix[2] * d34 + m.matrix[10] * d41 + m.matrix[14] * d13 * invDeterminant;   
    tempMatrix[14] = -(m.matrix[2] * d24 + m.matrix[6]  * d41 + m.matrix[14] * d12) * invDeterminant;   
    tempMatrix[15] =   m.matrix[2] * d23 - m.matrix[6]  * d13 + m.matrix[10] * d12 * invDeterminant;   
   
   // Save the temp matrix to our matrix.   
   matrix[0]  = tempMatrix[0];  matrix[1]  = tempMatrix[1];   
   matrix[2]  = tempMatrix[2];  matrix[3]  = tempMatrix[3];   
    matrix[4]  = tempMatrix[4];  matrix[5]  = tempMatrix[5];   
   matrix[6]  = tempMatrix[6];  matrix[7]  = tempMatrix[7];   
    matrix[8]  = tempMatrix[8];  matrix[9]  = tempMatrix[9];   
   matrix[10] = tempMatrix[10]; matrix[11] = tempMatrix[11];   
    matrix[12] = tempMatrix[12]; matrix[13] = tempMatrix[13];   
   matrix[14] = tempMatrix[14]; matrix[15] = tempMatrix[15];   
   
   return true;   
}   
   
   
bool CMatrix4x4::invertMatrix(CMatrix4x4 m)   
{   
   // Transpose rotation   
  matrix[ 0] = m.matrix[ 0];  matrix[ 1] = m.matrix[ 4];  matrix[ 2] = m.matrix[ 8];   
  matrix[ 4] = m.matrix[ 1];  matrix[ 5] = m.matrix[ 5];  matrix[ 6] = m.matrix[ 9];   
  matrix[ 8] = m.matrix[ 2];  matrix[ 9] = m.matrix[ 6];  matrix[10] = m.matrix[10];   
     
  // Clear shearing terms   
  matrix[3] = 0.0f; matrix[7] = 0.0f; matrix[11] = 0.0f; matrix[15] = 1.0f;   
   
  // Translation is minus the dot of tranlation and rotations   
  matrix[12] = -(m.matrix[12]*m.matrix[0]) - (m.matrix[13]*m.matrix[1]) - (m.matrix[14]*m.matrix[ 2]);   
  matrix[13] = -(m.matrix[12]*m.matrix[4]) - (m.matrix[13]*m.matrix[5]) - (m.matrix[14]*m.matrix[ 6]);   
  matrix[14] = -(m.matrix[12]*m.matrix[8]) - (m.matrix[13]*m.matrix[9]) - (m.matrix[14]*m.matrix[10]);   
   return true;   
}   
   
   
// This function is used to multiply (tranform) vectors with matrices   
CVector4 CMatrix4x4::VectorMatrixMultiply(CVector4 v)   
{   
   CVector4 out;   
   
   out.x = (v.x * matrix[0]) + (v.y * matrix[4]) + (v.z * matrix[8]) + (v.w * matrix[12]);   
   out.y = (v.x * matrix[1]) + (v.y * matrix[5]) + (v.z * matrix[9]) + (v.w * matrix[13]);   
   out.z = (v.x * matrix[2]) + (v.y * matrix[6]) + (v.z * matrix[10]) + (v.w * matrix[14]);   
   out.w = (v.x * matrix[3]) + (v.y * matrix[7]) + (v.z * matrix[11]) + (v.w * matrix[15]);    
   
   return out;   
}   
   
   
CVector4 CMatrix4x4::VectorMatrixMultiply3x3(CVector4 v)   
{   
   CVector4 out;   
   
   out.x = (v.x * matrix[0]) + (v.y * matrix[4]) + (v.z * matrix[8]);   
   out.y = (v.x * matrix[1]) + (v.y * matrix[5]) + (v.z * matrix[9]);   
   out.z = (v.x * matrix[2]) + (v.y * matrix[6]) + (v.z * matrix[10]);   
   
   return out;   
}   
   
void CMatrix4x4::makeScaleMtx(float x, float y, float z)    
{   
    matrix[ 0] = x;   
    matrix[ 1] = 0.0f;   
    matrix[ 2] = 0.0f;   
    matrix[ 3] = 0.0f;   
   
    matrix[ 4] = 0.0f;   
    matrix[ 5] = y;   
    matrix[ 6] = 0.0f;   
    matrix[ 7] = 0.0f;   
   
    matrix[ 8] = 0.0f;   
    matrix[ 9] = 0.0f;   
    matrix[10] = z;   
    matrix[11] = 0.0f;   
   
    matrix[12] = 0.0f;   
    matrix[13] = 0.0f;   
    matrix[14] = 0.0f;   
    matrix[15] = 1.0f;   
}   
   
void CMatrix4x4::scaleTranslateToFit(const CVector3 vMin, const CVector3 vMax)   
{   
    matrix[ 0] = 2.0f/(vMax.x-vMin.x);   
    matrix[ 4] = 0.0f;   
    matrix[ 8] = 0.0f;   
    matrix[12] = -(vMax.x+vMin.x)/(vMax.x-vMin.x);   
   
    matrix[ 1] = 0.0f;   
    matrix[ 5] = 2.0f/(vMax.y-vMin.y);   
    matrix[ 9] = 0.0f;   
    matrix[13] = -(vMax.y+vMin.y)/(vMax.y-vMin.y);   
   
    matrix[ 2] = 0.0f;   
    matrix[ 6] = 0.0f;   
    matrix[10] = 2.0f/(vMax.z-vMin.z);   
    matrix[14] = -(vMax.z+vMin.z)/(vMax.z-vMin.z);   
   
    matrix[ 3] = 0.0f;   
    matrix[ 7] = 0.0f;   
    matrix[11] = 0.0f;   
    matrix[15] = 1.0f;   
}   
   
CVector3 CMatrix4x4::mulHomogenPoint(CVector3 v)   
{   
    float x = matrix[0]*v.x + matrix[4]*v.y + matrix[ 8]*v.z + matrix[12];   
    float y = matrix[1]*v.x + matrix[5]*v.y + matrix[ 9]*v.z + matrix[13];   
    float z = matrix[2]*v.x + matrix[6]*v.y + matrix[10]*v.z + matrix[14];   
    float w = matrix[3]*v.x + matrix[7]*v.y + matrix[11]*v.z + matrix[15];   
   
    CVector3 vResult;   
    vResult.x = x / w;   
    vResult.y = y / w;   
    vResult.z = z / w;   
   
    return vResult;   
}   
   
CVector4 CMatrix4x4::transformPoint(CVector4 v)   
{   
   float x = v.x;   
   float y = v.y;   
   float z = v.z;   
   
    v.x = x * matrix[0] +   
         y * matrix[4] +   
         z * matrix[8] + matrix[12];   
   
   v.y = x * matrix[1] +   
         y * matrix[5] +   
         z * matrix[9] + matrix[13];   
   
   v.z = x * matrix[2] +   
         y * matrix[6] +   
         z * matrix[10]+ matrix[14];   
   
   v.w = x * matrix[2] +   
         y * matrix[7] +   
         z * matrix[11]+ matrix[15];   
   
   return v;   
}   
   
CVector3 CMatrix4x4::transformPoint(CVector3 v)   
{   
   float x = v.x;   
   float y = v.y;   
   float z = v.z;   
   
    v.x = x * matrix[0] +   
         y * matrix[4] +   
         z * matrix[8] + matrix[12];   
   
   v.y = x * matrix[1] +   
         y * matrix[5] +   
         z * matrix[9] + matrix[13];   
   
   v.z = x * matrix[2] +   
         y * matrix[6] +   
         z * matrix[10]+ matrix[14];   
   
   return v;   
}   
  
void CMatrix4x4::createViewMatrix( CVector3 eye,  CVector3 target,  CVector3 up)
{

	CVector3 zAxis = eye - target;
	zAxis.Normalize();

	CVector3 xAxis = CVector3::Cross(up, zAxis);
	xAxis.Normalize();

	CVector3 yAxis = CVector3::Cross(zAxis, xAxis);
	yAxis.Normalize();

	matrix[0] = xAxis.x;
	matrix[4] = xAxis.y;
	matrix[8] = xAxis.z;
	matrix[12] = -CVector3::Dot(xAxis, eye);

	matrix[1] = yAxis.x;
	matrix[5] = yAxis.y;
	matrix[9] = yAxis.z;
	matrix[13] = -CVector3::Dot(yAxis, eye);

	matrix[2] = zAxis.x;
	matrix[6] = zAxis.y;
	matrix[10] = zAxis.z;
	matrix[14] = -CVector3::Dot(zAxis, eye);

	matrix[3] = 0.0f;
	matrix[7] = 0.0f;
	matrix[11] = 0.0f;
	matrix[15] = 1.0f;
	 
}
 
void CMatrix4x4::createProjectionMatrix(float fovx, float aspect, float znear, float zfar)
{
	// Construct a projection matrix based on the horizontal field of view
	// 'fovx' rather than the more traditional vertical field of view 'fovy'.

	float e = 1.0f / tanf(GET_RADIANS(fovx) / 2.0f);
	float aspectInv = 1.0f / aspect;
	float fovy = 2.0f * atanf(aspectInv / e);
	float xScale = 1.0f / tanf(0.5f * fovy);
	float yScale = xScale / aspectInv;

	matrix[0] = xScale;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = yScale;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = (zfar + znear) / (znear - zfar);
	matrix[11] = -1.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = (2.0f * zfar * znear) / (znear - zfar);
	matrix[15] = 0.0f;

}   
void CMatrix4x4::CreateShadowMatrix(CVector4 planeNormal, CVector4 lightPos)   
{   
   Clear();   
   
   // To create a shadow matrix we first need the dot product of the ground normal and   
   // the light position.  We store the result in a float called dotProduct.  We use the   
   // DotProduct function of our CVector4 class (doesn't this make the code look better?).   
   float dotProduct = planeNormal.DotProduct4(lightPos);   
   
   // Create the shadow matrix by adding our values like so...   
   matrix[0]  = dotProduct - lightPos.x * planeNormal.x;   
    matrix[4]  = 0.0f       - lightPos.x * planeNormal.y;   
    matrix[8]  = 0.0f       - lightPos.x * planeNormal.z;   
    matrix[12] = 0.0f       - lightPos.x * planeNormal.w;   
   
   matrix[1]  = 0.0f       - lightPos.y * planeNormal.x;   
    matrix[5]  = dotProduct - lightPos.y * planeNormal.y;   
    matrix[9]  = 0.0f       - lightPos.y * planeNormal.z;   
    matrix[13] = 0.0f       - lightPos.y * planeNormal.w;   
   
   matrix[2]  = 0.0f       - lightPos.z * planeNormal.x;   
    matrix[6]  = 0.0f       - lightPos.z * planeNormal.y;   
    matrix[10] = dotProduct - lightPos.z * planeNormal.z;   
    matrix[14] = 0.0f       - lightPos.z * planeNormal.w;   
   
   matrix[3]  = 0.0f       - lightPos.w * planeNormal.x;   
    matrix[7]  = 0.0f       - lightPos.w * planeNormal.y;   
    matrix[11] = 0.0f       - lightPos.w * planeNormal.z;   
    matrix[15] = dotProduct - lightPos.w * planeNormal.w;   
   
   // Notice that the dotProuct is being subracted from the light position and the plane   
   // normal diagonally.  You will also notice that the x light position is used in the   
   // first column, y for the second, and so on.  This is the math to create a shadow   
   // projection matrix.  I wouldn't both taking out a pencil and paper and tracing this   
   // on out.  Just know that this is how you set it up.   
   
   
}   
inline CMatrix3x3 operator*(float scalar, const CMatrix3x3 &rhs)
{
    return rhs * scalar;
}

inline CMatrix3x3 CMatrix3x3::createFromAxes(const CVector3 &x, const CVector3 &y, const CVector3 &z)
{
    CMatrix3x3 tmp;
    tmp.createFromAxes(x, y, z);
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::createFromAxesTransposed(const CVector3 &x, const CVector3 &y, const CVector3 &z)
{
    CMatrix3x3 tmp;
    tmp.fromAxesTransposed(x, y, z);
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::createFromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
{
    CMatrix3x3 tmp;
    tmp.fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::createOrient(const CVector3 &from, const CVector3 &to)
{
    CMatrix3x3 tmp;
    tmp.orient(from, to);
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::createRotate(const CVector3 &axis, float degrees)
{
    CMatrix3x3 tmp;
    tmp.rotate(axis, degrees);
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::createScale(float sx, float sy, float sz)
{
    CMatrix3x3 tmp;
    tmp.scale(sx, sy, sz);
    return tmp;
}
CMatrix3x3::CMatrix3x3()
 {
 }
CMatrix3x3::~CMatrix3x3()
 {

 }
inline CMatrix3x3::CMatrix3x3(float m11, float m12, float m13,
                        float m21, float m22, float m23,
                        float m31, float m32, float m33)
{
    mtx[0][0] = m11, mtx[0][1] = m12, mtx[0][2] = m13;
    mtx[1][0] = m21, mtx[1][1] = m22, mtx[1][2] = m23;
    mtx[2][0] = m31, mtx[2][1] = m32, mtx[2][2] = m33;
}

float *CMatrix3x3::operator[](int row)
{
    return mtx[row];
}

const float *CMatrix3x3::operator[](int row) const
{
    return mtx[row];
}

inline bool CMatrix3x3::operator==(const CMatrix3x3 &rhs) const
{
    return  closeEnough(mtx[0][0], rhs.mtx[0][0])
        &&  closeEnough(mtx[0][1], rhs.mtx[0][1])
        &&  closeEnough(mtx[0][2], rhs.mtx[0][2])
        &&  closeEnough(mtx[1][0], rhs.mtx[1][0])
        &&  closeEnough(mtx[1][1], rhs.mtx[1][1])
        &&  closeEnough(mtx[1][2], rhs.mtx[1][2])
        &&  closeEnough(mtx[2][0], rhs.mtx[2][0])
        &&  closeEnough(mtx[2][1], rhs.mtx[2][1])
        &&  closeEnough(mtx[2][2], rhs.mtx[2][2]);
}

inline bool CMatrix3x3::operator!=(const CMatrix3x3 &rhs) const
{
    return !(*this == rhs);
}

inline CMatrix3x3 &CMatrix3x3::operator+=(const CMatrix3x3 &rhs)
{
    mtx[0][0] += rhs.mtx[0][0], mtx[0][1] += rhs.mtx[0][1], mtx[0][2] += rhs.mtx[0][2];
    mtx[1][0] += rhs.mtx[1][0], mtx[1][1] += rhs.mtx[1][1], mtx[1][2] += rhs.mtx[1][2];
    mtx[2][0] += rhs.mtx[2][0], mtx[2][1] += rhs.mtx[2][1], mtx[2][2] += rhs.mtx[2][2];
    return *this;
}

inline CMatrix3x3 &CMatrix3x3::operator-=(const CMatrix3x3 &rhs)
{
    mtx[0][0] -= rhs.mtx[0][0], mtx[0][1] -= rhs.mtx[0][1], mtx[0][2] -= rhs.mtx[0][2];
    mtx[1][0] -= rhs.mtx[1][0], mtx[1][1] -= rhs.mtx[1][1], mtx[1][2] -= rhs.mtx[1][2];
    mtx[2][0] -= rhs.mtx[2][0], mtx[2][1] -= rhs.mtx[2][1], mtx[2][2] -= rhs.mtx[2][2];
    return *this;
}

inline CMatrix3x3 &CMatrix3x3::operator*=(const CMatrix3x3 &rhs)
{
    CMatrix3x3 tmp;

    // Row 1.
    tmp.mtx[0][0] = (mtx[0][0] * rhs.mtx[0][0]) + (mtx[0][1] * rhs.mtx[1][0]) + (mtx[0][2] * rhs.mtx[2][0]);
    tmp.mtx[0][1] = (mtx[0][0] * rhs.mtx[0][1]) + (mtx[0][1] * rhs.mtx[1][1]) + (mtx[0][2] * rhs.mtx[2][1]);
    tmp.mtx[0][2] = (mtx[0][0] * rhs.mtx[0][2]) + (mtx[0][1] * rhs.mtx[1][2]) + (mtx[0][2] * rhs.mtx[2][2]);

    // Row 2.
    tmp.mtx[1][0] = (mtx[1][0] * rhs.mtx[0][0]) + (mtx[1][1] * rhs.mtx[1][0]) + (mtx[1][2] * rhs.mtx[2][0]);
    tmp.mtx[1][1] = (mtx[1][0] * rhs.mtx[0][1]) + (mtx[1][1] * rhs.mtx[1][1]) + (mtx[1][2] * rhs.mtx[2][1]);
    tmp.mtx[1][2] = (mtx[1][0] * rhs.mtx[0][2]) + (mtx[1][1] * rhs.mtx[1][2]) + (mtx[1][2] * rhs.mtx[2][2]);

    // Row 3.
    tmp.mtx[2][0] = (mtx[2][0] * rhs.mtx[0][0]) + (mtx[2][1] * rhs.mtx[1][0]) + (mtx[2][2] * rhs.mtx[2][0]);
    tmp.mtx[2][1] = (mtx[2][0] * rhs.mtx[0][1]) + (mtx[2][1] * rhs.mtx[1][1]) + (mtx[2][2] * rhs.mtx[2][1]);
    tmp.mtx[2][2] = (mtx[2][0] * rhs.mtx[0][2]) + (mtx[2][1] * rhs.mtx[1][2]) + (mtx[2][2] * rhs.mtx[2][2]);

    *this = tmp;
    return *this;
}

inline CMatrix3x3 &CMatrix3x3::operator*=(float scalar)
{
    mtx[0][0] *= scalar, mtx[0][1] *= scalar, mtx[0][2] *= scalar;
    mtx[1][0] *= scalar, mtx[1][1] *= scalar, mtx[1][2] *= scalar;
    mtx[2][0] *= scalar, mtx[2][1] *= scalar, mtx[2][2] *= scalar;
    return *this;
}

inline CMatrix3x3 &CMatrix3x3::operator/=(float scalar)
{
    mtx[0][0] /= scalar, mtx[0][1] /= scalar, mtx[0][2] /= scalar;
    mtx[1][0] /= scalar, mtx[1][1] /= scalar, mtx[1][2] /= scalar;
    mtx[2][0] /= scalar, mtx[2][1] /= scalar, mtx[2][2] /= scalar;
    return *this;
}

inline CMatrix3x3 CMatrix3x3::operator+(const CMatrix3x3 &rhs) const
{
    CMatrix3x3 tmp(*this);
    tmp += rhs;
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::operator-(const CMatrix3x3 &rhs) const
{
    CMatrix3x3 tmp(*this);
    tmp -= rhs;
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::operator*(const CMatrix3x3 &rhs) const
{
    CMatrix3x3 tmp(*this);
    tmp *= rhs;
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::operator*(float scalar) const
{
    CMatrix3x3 tmp(*this);
    tmp *= scalar;
    return tmp;
}

inline CMatrix3x3 CMatrix3x3::operator/(float scalar) const
{
    CMatrix3x3 tmp(*this);
    tmp /= scalar;
    return tmp;
}

inline float CMatrix3x3::determinant() const
{
    return (mtx[0][0] * (mtx[1][1] * mtx[2][2] - mtx[1][2] * mtx[2][1]))
        - (mtx[0][1] * (mtx[1][0] * mtx[2][2] - mtx[1][2] * mtx[2][0]))
        + (mtx[0][2] * (mtx[1][0] * mtx[2][1] - mtx[1][1] * mtx[2][0]));
}

inline void CMatrix3x3::fromAxes(const CVector3 &x, const CVector3 &y, const CVector3 &z)
{
    mtx[0][0] = x.x,  mtx[0][1] = x.y,  mtx[0][2] = x.z;
    mtx[1][0] = y.x,  mtx[1][1] = y.y,  mtx[1][2] = y.z;
    mtx[2][0] = z.x,  mtx[2][1] = z.y,  mtx[2][2] = z.z;
}

inline void CMatrix3x3::fromAxesTransposed(const CVector3 &x, const CVector3 &y, const CVector3 &z)
{
    mtx[0][0] = x.x,  mtx[0][1] = y.x,  mtx[0][2] = z.x;
    mtx[1][0] = x.y,  mtx[1][1] = y.y,  mtx[1][2] = z.y;
    mtx[2][0] = x.z,  mtx[2][1] = y.z,  mtx[2][2] = z.z;
}

inline void CMatrix3x3::identity()
{
    mtx[0][0] = 1.0f, mtx[0][1] = 0.0f, mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f, mtx[1][1] = 1.0f, mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f, mtx[2][1] = 0.0f, mtx[2][2] = 1.0f;
}

inline void CMatrix3x3::toAxes(CVector3 &x, CVector3 &y, CVector3 &z) const
{
    x.set(mtx[0][0], mtx[0][1], mtx[0][2]);
    y.set(mtx[1][0], mtx[1][1], mtx[1][2]);
    z.set(mtx[2][0], mtx[2][1], mtx[2][2]);
}

inline void CMatrix3x3::toAxesTransposed(CVector3 &x, CVector3 &y, CVector3 &z) const
{
    x.set(mtx[0][0], mtx[1][0], mtx[2][0]);
    y.set(mtx[0][1], mtx[1][1], mtx[2][1]);
    z.set(mtx[0][2], mtx[1][2], mtx[2][2]);
}

inline CMatrix3x3 CMatrix3x3::transpose() const
{
    CMatrix3x3 tmp;

    tmp[0][0] = mtx[0][0], tmp[0][1] = mtx[1][0], tmp[0][2] = mtx[2][0];
    tmp[1][0] = mtx[0][1], tmp[1][1] = mtx[1][1], tmp[1][2] = mtx[2][1];
    tmp[2][0] = mtx[0][2], tmp[2][1] = mtx[1][2], tmp[2][2] = mtx[2][2];

    return tmp;
}

const float *CMatrix3x3::getMatrix()
{
	return &mtx[0][0];
}


void CMatrix3x3::createNormalMatrix(const CMatrix4x4 &rModelViewMatrix)
{
	 
	mtx[0][0]=rModelViewMatrix.GetColumn(0).x;
	mtx[0][1]=rModelViewMatrix.GetColumn(0).y;
	mtx[0][2]=rModelViewMatrix.GetColumn(0).z;

	mtx[1][0]=rModelViewMatrix.GetColumn(1).x;
	mtx[1][1]=rModelViewMatrix.GetColumn(1).y;
	mtx[1][2]=rModelViewMatrix.GetColumn(1).z;

	mtx[2][0]=rModelViewMatrix.GetColumn(2).x;
	mtx[2][1]=rModelViewMatrix.GetColumn(2).y;
	mtx[2][2]=rModelViewMatrix.GetColumn(2).z;
	 
	inverse().transpose();

}

CMatrix3x3 CMatrix3x3::inverse() const
{
    // If the inverse doesn't exist for this matrix, then the identity
    // matrix will be returned.

    CMatrix3x3 tmp;
    float d = determinant();

    if (closeEnough(d, 0.0f))
    {
        tmp.identity();
    }
    else
    {
        d = 1.0f / d;

        tmp.mtx[0][0] = d * (mtx[1][1] * mtx[2][2] - mtx[1][2] * mtx[2][1]);
        tmp.mtx[0][1] = d * (mtx[0][2] * mtx[2][1] - mtx[0][1] * mtx[2][2]);
        tmp.mtx[0][2] = d * (mtx[0][1] * mtx[1][2] - mtx[0][2] * mtx[1][1]);

        tmp.mtx[1][0] = d * (mtx[1][2] * mtx[2][0] - mtx[1][0] * mtx[2][2]);
        tmp.mtx[1][1] = d * (mtx[0][0] * mtx[2][2] - mtx[0][2] * mtx[2][0]);
        tmp.mtx[1][2] = d * (mtx[0][2] * mtx[1][0] - mtx[0][0] * mtx[1][2]);

        tmp.mtx[2][0] = d * (mtx[1][0] * mtx[2][1] - mtx[1][1] * mtx[2][0]);
        tmp.mtx[2][1] = d * (mtx[0][1] * mtx[2][0] - mtx[0][0] * mtx[2][1]);
        tmp.mtx[2][2] = d * (mtx[0][0] * mtx[1][1] - mtx[0][1] * mtx[1][0]);
    }

    return tmp;
}




void Plane::ComputePlaneEquation(CVector3 v[3])   
{   
    a = v[0].y*(v[1].z-v[2].z) + v[1].y*(v[2].z-v[0].z) + v[2].y*(v[0].z-v[1].z);   
    b = v[0].z*(v[1].x-v[2].x) + v[1].z*(v[2].x-v[0].x) + v[2].z*(v[0].x-v[1].x);   
    c = v[0].x*(v[1].y-v[2].y) + v[1].x*(v[2].y-v[0].y) + v[2].x*(v[0].y-v[1].y);   
    d = -( v[0].x*(v[1].y*v[2].z - v[2].y*v[1].z) +   
           v[1].x*(v[2].y*v[0].z - v[0].y*v[2].z) +   
           v[2].x*(v[0].y*v[1].z - v[1].y*v[0].z) );   
}   
   
bool Plane::CollidedWithLine(CVector3 vLine[2])   
{   
    float Distance1 = 0, Distance2 = 0;   
   
    Distance1 = (a * vLine[0].x,            // Ax   
                 b * vLine[0].y,            // By   
                 c * vLine[0].z) + d;       // Cz + D   
   
    Distance2 = (a * vLine[1].x,            // Ax   
                 b * vLine[1].y,            // By   
                 c * vLine[1].z) + d;       // Cz + D   
   
    if(Distance1 * Distance2 >= 0)   
        return false;   
   
    return true;   
}   
   
float Plane::PlaneDistanceFromOrigin(CVector3 Normal, CVector3 Point)   
{      
    float distance = 0;                                 // This variable holds the distance from the plane tot he origin   
   
    // Use the plane equation to find the distance (Ax + By + Cz + D = 0)  We want to find D.   
    // So, we come up with D = -(Ax + By + Cz)   
                                                        // Basically, the negated dot product of the normal of the plane and the point. (More about the dot product in another tutorial)   
    distance = - ((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));   
   
    return distance;                                    // Return the distance   
}   
   
int Plane::ClassifySphere(CVector3 &vCenter, float radius, CVector3 &vPoint, float &distance,   
                          CVector3 &vNormal)   
{   
    float d = (float)PlaneDistanceFromOrigin(vNormal, vPoint);   
    distance = (vNormal.x * vCenter.x + vNormal.y * vCenter.y + vNormal.z * vCenter.z + d);   
   
    if(fabs(distance) < radius)   
        return INTERSECTS;   
    else if(distance >= radius)   
        return INFRONT;   
   
    return BEHIND;   
}   
   
bool Plane::PlaneIntersection(CVector3 *InterSectionPoint, const Plane *p0, const Plane *p1,   
                              const Plane *p2)   
{   
    CVector3 n0( p0->a, p0->b, p0->c );   
    CVector3 n1( p1->a, p1->b, p1->c );   
    CVector3 n2( p2->a, p2->b, p2->c );   
   
    CVector3 n1_n2, n2_n0, n0_n1;     
       
    n1_n2 = n1_n2.Cross(n1, n2);   
    n2_n0 = n2_n0.Cross(n2, n0);   
    n0_n1 = n0_n1.Cross(n0, n1);   
   
    float cosTheta = n1_n2.Dot(n0, n1_n2);   
       
    if ( ALMOST_ZERO(cosTheta) || IS_SPECIAL(cosTheta) )   
        return false;   
   
    float secTheta = 1.f / cosTheta;   
   
    n1_n2 = n1_n2 *= p0->d;   
    n2_n0 = n2_n0 *= p1->d;   
    n0_n1 = n0_n1 *= p2->d;   
   
    *InterSectionPoint = (n1_n2.Negate(n1_n2 + n2_n0 + n0_n1)) *= secTheta;   
    return true;   
}   
   
tFace::~tFace()   
{   
}   
   
CVector3 tFace::GetInterSectionPoint(CVector3 vLine[2])   
{   
    CVector3 vLineDir = vLineDir.Vector(vLine[1], vLine[0]);   
    vLineDir.Normalize();   
   
    float Numerator = -(this->PlaneEq.a * vLine[0].x +   
                        this->PlaneEq.b * vLine[0].y +   
                        this->PlaneEq.c * vLine[0].z + this->PlaneEq.d);   
   
    CVector3 vPlaneNormal = CVector3(this->PlaneEq.a, this->PlaneEq.b, this->PlaneEq.c);   
    float Denominator = vPlaneNormal.Dot(vPlaneNormal, vLineDir);   
   
    if(Denominator == 0.0)   
        return vLine[0];   
   
    float dist = Numerator / Denominator;   
   
    CVector3 vInterSectionPoint;   
    vInterSectionPoint.x = (vLine[0].x + (vLineDir.x * dist));   
    vInterSectionPoint.y = (vLine[0].y + (vLineDir.y * dist));   
    vInterSectionPoint.z = (vLine[0].z + (vLineDir.z * dist));   
   
    return vInterSectionPoint;   
}   
   
bool tFace::InsidePolygon(CVector3 vIPoint, CVector3 vPoly[], int VertexCount)   
{   
    const double MATH_FACTOR = 0.9999;   
    CVector3 vA, vB;   
    double Angle = 0.0;   
   
    for(int i = 0; i < VertexCount; i++)   
    {   
        vA = vA.Vector(vPoly[i], vIPoint);   
   
        vB = vB.Vector(vPoly[(i + 1) % VertexCount], vIPoint);   
   
        Angle += vA.AngleBetweenVectors(vA, vB);   
    }   
   
    if(Angle >= (MATH_FACTOR * (2.0 * PI)) )   
        return true;   
   
    return false;   
}   
   
bool tFace::EdgeSphereCollision(CVector3 &vCenter, CVector3 vPoly[], int VertexCount,   
                                float radius)   
{   
    CVector3 vPoint;   
   
    for(int i = 0; i < VertexCount; i++)   
    {   
        CVector3 vLine[2] = { CVector3(vPoly[i]), CVector3(vPoly[(i + 1) % VertexCount]) };   
        vPoint = vPoint.ClosestPointOnLine(vLine, vCenter);   
   
        float distance = vPoint.Distance(vPoint, vCenter);   
   
        if(distance < radius)   
            return true;   
    }   
   
    return false;   
}   
   
bool tFace::CollidedWithLine(CVector3 vLine[2], CVector3 vPoly[3])   
{   
    if(!(PlaneEq.CollidedWithLine(vLine)))   
        return false;   
   
    CVector3 vInterSectionPoint = GetInterSectionPoint(vLine);   
   
    if(!(InsidePolygon(vInterSectionPoint, vPoly, 3)))   
        return false;   
   
    return true;   
}   
   
bool tFace::CollidedWithSphere(CVector3 vPoly[3], CVector3 &vNormal, CVector3 &vCenter, float radius)   
{   
    float distance = 0.0f;   
   
    int classification = this->PlaneEq.ClassifySphere(vCenter, radius, vPoly[0], distance,   
                                                      vNormal);   
   
    if(classification == INTERSECTS)   
    {   
        CVector3 vOffset = vNormal *= distance;   
   
        CVector3 vPosition = vCenter - vOffset;   
   
        if(InsidePolygon(vPosition, vPoly, 3))   
            return true;   
        else   
        {   
            if(EdgeSphereCollision(vCenter, vPoly, 3, radius))   
                return true;   
        }   
    }   
   
    return false;   
}  


CVector3 Interpolate::Linear( CVector3 &v1, CVector3 &v2, float mu )
{
	float invmu = 1.0f - mu;
	return CVector3(	v1.x * invmu + v2.x * mu,
						v1.y * invmu + v2.y * mu,
						v1.z * invmu + v2.z * mu );
}

CVector3 Interpolate::Cosine( CVector3 &v1, CVector3 &v2, float mu )
{
	mu = mu * 3.1415927f;
	mu = (1.0f - (float)cos(mu)) * 0.5f;

	float invmu = 1.0f - mu;
	return CVector3(	v1.x * invmu + v2.x * mu,
						v1.y * invmu + v2.y * mu,
						v1.z * invmu + v2.z * mu );
}

CVector3 Interpolate::Cubic( CVector3 &v0, CVector3 &v1, CVector3 &v2, CVector3 &v3, float mu )
{
	float mu2 = mu * mu;
	float mu3 = mu2 * mu;

	CVector3 p = (v3 - v2) - (v0 - v1);
	CVector3 q = (v0 - v1) - p;
	CVector3 r = v2 - v0;
	CVector3 s = v1;

	return CVector3(	p.x * mu3 + q.x * mu2 + r.x * mu + s.x,
						p.y * mu3 + q.y * mu2 + r.y * mu + s.y,
						p.z * mu3 + q.z * mu2 + r.z * mu + s.z );
}

CVector3 Interpolate::Hermite( CVector3 &v1, CVector3 &v2, CVector3 &t1, CVector3 &t2, float mu )
{
	float mu2 = mu * mu;
	float mu3 = mu2 * mu;

	float h1 = 2 * mu3 - 3 * mu2 + 1;
	float h2 = -2 * mu3 + 3 * mu2;
	float h3 = mu3 - 2 * mu2 + mu;
	float h4 = mu3 - mu2;

	return CVector3(	v1.x * h1 + v2.x * h2 + t1.x * h3 + t2.x * h4,
						v1.y * h1 + v2.y * h2 + t1.y * h3 + t2.y * h4,
						v1.z * h1 + v2.z * h2 + t1.z * h3 + t2.z * h4 );
}

CVector3 Interpolate::Cardinal( CVector3 &v0, CVector3 &v1, CVector3 &v2, CVector3 &v3, float mu, float tightness )
{
	CVector3 t1 = (v2 - v0) * tightness;
	CVector3 t2 = (v3 - v1) * tightness;
	return Hermite( v1, v2, t1, t2, mu );
}

CVector3 Interpolate::CatmullRom( CVector3 &v0, CVector3 &v1, CVector3 &v2, CVector3 &v3, float mu )
{
	return Cardinal( v0, v1, v2, v3, mu, 0.5f );
}

/*******************************************************************************/

GLfloat mathGetAngle (float x1, float y1,float x2,float y2) 
{
GLfloat    theAngle = atan2(x2-x1,y2-y1)*180/3.141592653589793238462643f;
return theAngle;
}
 
GLfloat mathGetRotationToVector(CVector3 position,CVector3 destination)
{
return mathGetAngle(position.x,position.z,destination.x,destination.z);
}

GLfloat mathUnwindAngle_Degrees( GLfloat fAngle )
{
	fAngle -= 360.0f * (int)( fAngle / 360.0f );
	if( fAngle < 0.0f )
		{
			fAngle += 360.0f;
		}
	return fAngle;
}
bool mathIsCWFaster( GLfloat fStartAngle, GLfloat fEndAngle )
{
   GLfloat ccw = mathUnwindAngle_Degrees( fStartAngle - fEndAngle );
   GLfloat cw = mathUnwindAngle_Degrees( fEndAngle - fStartAngle );
   return( cw < ccw );
}

GLfloat mathGet_rotation_vector( GLfloat fStartAngle, GLfloat fEndAngle )
{
	GLfloat ccw = mathUnwindAngle_Degrees( fStartAngle - fEndAngle );
	GLfloat cw = mathUnwindAngle_Degrees( fEndAngle - fStartAngle );
	if(cw<ccw)return cw;
	else return ccw;
}
 
GLfloat mathGetfdistance(GLfloat x, GLfloat z,int x2, int z2){
 	return (GLfloat) sqrt ( (GLfloat)((x-x2)*(x-x2)) + ((z-z2)*(z-z2)) );
}



  void Vec2Normalize(float v[2])
    {
        float magnitude = sqrtf(v[0] * v[0] + v[1] * v[1]);

        v[0] /= magnitude;
        v[1] /= magnitude;
    }

    void Vec3Normalize(float v[3])
    {
        float magnitude = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

        v[0] /= magnitude;
        v[1] /= magnitude;
        v[2] /= magnitude;
    }

    void Vec3Cross(const float v1[3], const float v2[3], float result[3])
    {
        result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
        result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
        result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
    }

    float Vec3Dot(const float v1[3], const float v2[3])
    {
        return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
    }
 CVector3 QuatToEuler(btQuaternion* quat)
 {
         double sqw = quat->w() * quat->w();
         double sqx = quat->x() * quat->x();
         double sqy = quat->y() * quat->y();
         double sqz = quat->z() * quat->z();
   
         return CVector3(
            (float)atan2l(2.0 * ( quat->y() * quat->z() + quat->x() * quat->w() ) , ( -sqx - sqy + sqz + sqw )), 
            (float)asinl(-2.0 * ( quat->x() * quat->z() - quat->y() * quat->w() )), 
            (float)atan2l(2.0 * ( quat->x() * quat->y() + quat->z() * quat->w() ) , ( sqx - sqy - sqz + sqw )));


      }
     static bool closeEnough(float f1, float f2)
    {
        // Determines whether the two floating-point values f1 and f2 are
        // close enough together that they can be considered equal.

        return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
    }



void CalcTangentVector(const CVector3 &vpos1,         // triangle vertex 1 pos
                       const CVector3 &vpos2,         // triangle vertex 2 pos
					   const CVector3 &vpos3,         // triangle vertex 3 pos
                       const CVector2 &vtexCoord1,    // triangle vertex 1 uv
                       const CVector2 &vtexCoord2,    // triangle vertex 1 uv
                       const CVector2 &vtexCoord3,    // triangle vertex 1 uv
                       const CVector3 &vnormal,
                       float tangent[4])            // returned surface tangent
{
float pos1[3];
float pos2[3];
float pos3[3];

float texCoord1[2];    // triangle vertex 1 uv
float texCoord2[2];    // triangle vertex 2 uv
float texCoord3[2];    // triangle vertex 3 uv
float normal[3];       // triangle surface normal

pos1[0]=vpos1.x;
pos1[1]=vpos1.y;
pos1[2]=vpos1.z;

pos2[0]=vpos2.x;
pos2[1]=vpos2.y;
pos2[2]=vpos2.z;

pos3[0]=vpos3.x;
pos3[1]=vpos3.y;
pos3[2]=vpos3.z;

normal[0]=vnormal.x;
normal[1]=vnormal.y;
normal[2]=vnormal.z;

texCoord1[0]=vtexCoord1.x;
texCoord1[1]=vtexCoord1.y;

texCoord2[0]=vtexCoord2.x;
texCoord2[1]=vtexCoord2.y;

texCoord3[0]=vtexCoord3.x;
texCoord3[1]=vtexCoord3.y;
 
    // Given the 3 vertices (position and texture coordinates) of a triangle
    // calculate and return the triangle's tangent vector.

    // Create 2 vectors in object space.
    //
    // edge1 is the vector from vertex positions pos1 to pos2.
    // edge2 is the vector from vertex positions pos1 to pos3.
    float edge1[3] = {pos2[0] - pos1[0], pos2[1] - pos1[1], pos2[2] - pos1[2]};
    float edge2[3] = {pos3[0] - pos1[0], pos3[1] - pos1[1], pos3[2] - pos1[2]};

    Vec3Normalize(edge1);
    Vec3Normalize(edge2);

    // Create 2 vectors in tangent (texture) space that point in the same
    // direction as edge1 and edge2 (in object space).
    //
    // texEdge1 is the vector from texture coordinates texCoord1 to texCoord2.
    // texEdge2 is the vector from texture coordinates texCoord1 to texCoord3.
    float texEdge1[2] = {texCoord2[0] - texCoord1[0], texCoord2[1] - texCoord1[1]};
    float texEdge2[2] = {texCoord3[0] - texCoord1[0], texCoord3[1] - texCoord1[1]};

    Vec2Normalize(texEdge1);
    Vec2Normalize(texEdge2);

    // These 2 sets of vectors form the following system of equations:
    //
    //  edge1 = (texEdge1.x * tangent) + (texEdge1.y * bitangent)
    //  edge2 = (texEdge2.x * tangent) + (texEdge2.y * bitangent)
    //
    // Using matrix notation this system looks like:
    //
    //  [ edge1 ]     [ texEdge1.x  texEdge1.y ]  [ tangent   ]
    //  [       ]  =  [                        ]  [           ]
    //  [ edge2 ]     [ texEdge2.x  texEdge2.y ]  [ bitangent ]
    //
    // The solution is:
    //
    //  [ tangent   ]        1     [ texEdge2.y  -texEdge1.y ]  [ edge1 ]
    //  [           ]  =  -------  [                         ]  [       ]
    //  [ bitangent ]      det A   [-texEdge2.x   texEdge1.x ]  [ edge2 ]
    //
    //  where:
    //        [ texEdge1.x  texEdge1.y ]
    //    A = [                        ]
    //        [ texEdge2.x  texEdge2.y ]
    //
    //    det A = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x)
    //
    // From this solution the tangent space basis vectors are:
    //
    //    tangent = (1 / det A) * ( texEdge2.y * edge1 - texEdge1.y * edge2)
    //  bitangent = (1 / det A) * (-texEdge2.x * edge1 + texEdge1.x * edge2)
    //     normal = cross(tangent, bitangent)

    float det = (texEdge1[0] * texEdge2[1]) - (texEdge1[1] * texEdge2[0]);
    float t[3];
    float b[3];
    
    if (fabsf(det) < 1e-6f)    // almost equal to zero
    {
        t[0] = 1.0f;
        t[1] = 0.0f;
        t[2] = 0.0f;

        b[0] = 0.0f;
        b[1] = 1.0f;
        b[2] = 0.0f;
    }
    else
    {
        det = 1.0f / det;

        t[0] = (texEdge2[1] * edge1[0] - texEdge1[1] * edge2[0]) * det;
        t[1] = (texEdge2[1] * edge1[1] - texEdge1[1] * edge2[1]) * det;
        t[2] = (texEdge2[1] * edge1[2] - texEdge1[1] * edge2[2]) * det;

        b[0] = (-texEdge2[0] * edge1[0] + texEdge1[0] * edge2[0]) * det;
        b[1] = (-texEdge2[0] * edge1[1] + texEdge1[0] * edge2[1]) * det;
        b[2] = (-texEdge2[0] * edge1[2] + texEdge1[0] * edge2[2]) * det;

        Vec3Normalize(t);
        Vec3Normalize(b);
    }

    // Calculate the handedness of the local tangent space.
    // The bitangent vector is the cross product between the triangle face
    // normal vector and the calculated tangent vector. The resulting bitangent
    // vector should be the same as the bitangent vector calculated from the
    // set of linear equations above. If they point in different directions
    // then we need to invert the cross product calculated bitangent vector. We
    // store this scalar multiplier in the tangent vector's 'w' component so
    // that the correct bitangent vector can be generated in the normal mapping
    // shader's vertex shader.

    float handedness = 0.0f;
    float bitangent[3] = {0.0f, 0.0f, 0.0f};

    Vec3Cross(normal, t, bitangent);
    handedness = (Vec3Dot(bitangent, b) < 0.0f) ? -1.0f : 1.0f;

    tangent[0] = t[0];
    tangent[1] = t[1];
    tangent[2] = t[2];
    tangent[3] = handedness;
}
