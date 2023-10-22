#pragma once  
#ifndef _GAMEMATH_H 
#define _GAMEMATH_H 
  
#define PI (float)3.1415  
#define ROUNDOFF(x) (x  0.0001f && x > -0.0001f) ? 0.0f : x  
#define GET_RADIANS(degree) (float)(degree / 180.0f * PI)  
#define GET_DEGREES(radian) (float)(radian / PI * 180.0f) 
#define mrand(a,b)     ((a)+(rand()%((b)*10-(a)*10+10))/10) 
#define Absolute(num) (float)( ((num  0) ? 0 - num : num) )   
#define BEHIND		0 
#define INTERSECTS	1 
#define INFRONT		2 
#define DW_AS_FLT(DW) (*(FLOAT*)&(DW)) 
#define FLT_AS_DW(F) (*(DWORD*)&(F)) 
#define FLT_SIGN(F) ((FLT_AS_DW(F) & 0x80000000L)) 
#define ALMOST_ZERO(F) ((FLT_AS_DW(F) & 0x7f800000L)==0) 
#define IS_SPECIAL(F)  ((FLT_AS_DW(F) & 0x7f800000L)==0x7f800000L) 
#define EPSILON		1e-6f
 
template<class Type> inline Type Clamp(Type A, Type Min, Type Max) 
{ 
  if(A<Min) return Min; 
  if(A>Max) return Max; 
  return A; 
} 
 



class CTexCoord 
{ 
   public: 
      CTexCoord() 
         { 
            tu = 0.0f; tv = 0.0f; 
         } 
 
      CTexCoord(float u, float v) 
         { 
            tu = u; tv = v; 
         } 
 
	  operator float* () const {return (float*) this;} 
	  operator const float* () const {return (const float*) this;} 
 
      float tu, tv; 
}; 
 
class CVector2 
{ 
public: 
	CVector2() {} 
 
	CVector2(float X, float Y) 
	{ 
		x = X, y = Y; 
	} 
 
	CVector2 operator+(CVector2 vVector) 
	{ 
		return CVector2(x + vVector.x, y + vVector.y); 
	} 
 
	CVector2 operator+=(float scalar) 
	{ 
		return CVector2(x + scalar, y + scalar); 
	} 
 
	CVector2 operator-(CVector2 vVector) 
	{ 
		return CVector2(x - vVector.x, y - vVector.y); 
	} 
 
	CVector2 operator-=(float scalar) 
	{ 
		return CVector2(x - scalar, y - scalar); 
	} 
 
	CVector2 operator*(CVector2 vVector) 
	{ 
		return CVector2(x * vVector.x, y * vVector.y); 
	} 
 
	CVector2 operator*=(float scalar) 
	{ 
		return CVector2(x * scalar, y * scalar); 
	} 
 
	CVector2 operator/(CVector2 vVector) 
	{ 
		return CVector2(x / vVector.x, y / vVector.y); 
	} 
 
	CVector2 operator/=(float scalar) 
	{ 
		return CVector2(x / scalar, y / scalar); 
	} 
 
	operator float* () const {return (float*) this;} 
	operator const float* () const {return (const float*) this;} 
 
	float x, y; 
}; 
 
class CVector3 
{ 
public: 
	CVector3() {} 
 
	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z; 
	} 
 
	void operator=(CVector3 vVector) 
	{ 
		x = vVector.x;	y = vVector.y;	z = vVector.z; 
	} 
	 
	CVector3 operator+(const CVector3 &vVector) 
	{ 
		return CVector3(x + vVector.x, y + vVector.y, z + vVector.z); 
	} 
 
	CVector3 operator+=(float scalar) 
	{ 
		return CVector3(x + scalar, y + scalar, z + scalar); 
	} 

	CVector3& operator+=(const btVector3 &v)
	{
		x+=v.x();
		y+=v.y();
		z+=v.z();

		return *this;
	}
CVector3& operator=(const btVector3 &v)
	{
		x=v.x();
		y=v.y();
		z=v.z();

		return *this;
	}
	CVector3& operator-=(const btVector3 &v)
	{
		x-=v.x();
		y-=v.y();
		z-=v.z();

		return *this;
	}
	CVector3& operator+=(const CVector3& v)
	{
		// Add the vector to our current vector
		x += v.x;
		y += v.y;
		z += v.z;

		// Return the changed vector back
		return *this;
	}
	bool operator!=(const CVector3 & rhs)    
	{   
	    return !((*this)==rhs);   
	}   
	CVector3 operator-=(const CVector3& v)
	{
		x -= v.x, y -= v.y, z -= v.z;
		return *this;
	}

	bool operator==(const CVector3 & rhs) const   
	{   
    if(x==rhs.x && y==rhs.y && z==rhs.z)   
        return true;   
   
    return false;   
	}   
   void set(float x_, float y_, float z_);

 
	CVector3 operator-(const CVector3 &vVector) 
	{ 
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z); 
	} 
 
	CVector3 operator-=(float scalar) 
	{ 
		return CVector3(x - scalar, y - scalar, z - scalar); 
	} 
 
	CVector3 operator*(const CVector3 &vVector) 
	{ 
		return CVector3(x * vVector.x, y * vVector.y, z * vVector.z); 
	} 

	CVector3 operator*(float num)
	{
		return CVector3(x * num, y * num, z * num);
	}
	 
	CVector3 operator*=(float scalar) 
	{ 
		return CVector3(x * scalar, y * scalar, z * scalar); 
	} 
 


	CVector3 operator/(CVector3 vVector) 
	{ 
		return CVector3(x / vVector.x, y / vVector.y, z / vVector.z); 
	} 
 
	CVector3 operator/=(float scalar) 
	{ 
		return CVector3(x / scalar, y / scalar, z / scalar); 
	} 

 	// The dot product between two CVector3 vectors
	GLfloat Dot(const CVector3& v) const
	{
		return ( (x * v.x) + (y * v.y) + (z * v.z) );
	}


	CVector3 Negate(CVector3 vVector); 
	float GetMagnitude(CVector3 vVector); 
	CVector3 Normalize(CVector3 vVector); 
	void Normalize(); 
	static CVector3 Cross(CVector3 vVector1, CVector3 vVector2); 
	static float Dot(CVector3 vVector1, CVector3 vVector2); 
	CVector3 Cross(CVector3 vVector2);	
	double AngleBetweenVectors(CVector3 vVector1, CVector3 vVector2); 
	CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2); 
	float Distance(const CVector3 &vPoint1, const CVector3 &vPoint2); 
	CVector3 ClosestPointOnLine(CVector3 vLine[2], CVector3 vPoint); 
	CVector3 CalcNormal(CVector3 vPoly[3]); 
	CVector3 GetCollisionOffset(CVector3 &vNormal, float radius, float distance); 
	void CalculateTBNMatrix(CVector3 pVerts[3], CTexCoord pTexCoords[3],  
		                    CVector3 pTBNMatrix[3]);  
 
	CVector3 AddVector(CVector3 vVector1, CVector3 vVector2);
	CVector3 DivideVectorByScaler(CVector3 vVector1, GLfloat Scaler);

	operator float* () const {return (float*) this;} 
	operator const float* () const {return (const float*) this;} 
 
	float x, y, z; 
}; 
 
class CVector4 
{ 
   public: 
      CVector4(); 
      CVector4(float X, float Y, float Z); 
      CVector4(float X, float Y, float Z, float W); 
	  CVector4(const CVector3 &v); 
      CVector4(const CVector4 &v); 
 
      void operator=(CVector4 v); 
      CVector4 operator-(CVector4 v); 
      CVector4 operator+(CVector4 v); 
      CVector4 operator*(CVector4 v); 
      CVector4 operator/(CVector4 v); 
      CVector4 operator+(float f); 
      CVector4 operator-(float f); 
      CVector4 operator*(float f); 
      CVector4 operator/(float f); 
 
      void operator +=(CVector4 v); 
      void operator -=(CVector4 v); 
      void operator *=(CVector4 v); 
      void operator /=(CVector4 v); 
      void operator +=(float f); 
      void operator -=(float f); 
      void operator *=(float f); 
      void operator /=(float f); 
 
      bool operator ==(CVector4 v); 
      bool operator !=(CVector4 v); 
 
      void CrossProduct(CVector4 v1, CVector4 v2); 
      void CrossProduct3(CVector4 v1, CVector4 v2, 
                         CVector4 v3); 
      float DotProduct3(CVector4 v1); 
      float DotProduct4(CVector4 v1); 
      float GetLength(); 
      void Normalize(); 
      void CalcNormal(CVector4 Triangle[]); 
      void CalculateTangentVector(CVector4 Point1, 
                                  CVector4 Point2, 
                                  CVector4 Point3, 
                                  CVector4 NormalOfA, 
                                  CTexCoord P1Tex, 
                                  CTexCoord P2Tex, 
                                  CTexCoord P3Tex); 
      void ExtendVertexPos(CVector4 lightPos, 
                           float Extend); 
      void ExtendVertexPos(CVector4 currentVertex, 
                           CVector4 lightPos, 
                           float Extend); 
      CVector4 GetRotatedX(double angle);  
      CVector4 GetRotatedY(double angle); 
      CVector4 GetRotatedZ(double angle); 
      CVector4 GetRotatedAxis(double angle, CVector4 axis); 
      void CalculateBinormalVector(CVector4 tangent, 
                                   CVector4 normal); 
      void ClampTo01(); 
 
	  operator float* () const {return (float*) this;} 
	  operator const float* () const {return (const float*) this;} 
 
      float x, y, z, w; 
}; 
 
class Plane 
{ 
public: 
	void ComputePlaneEquation(CVector3 v[3]); 
	bool CollidedWithLine(CVector3 vLine[2]); 
	int ClassifySphere(CVector3 &vCenter, float radius, CVector3 &vPoint, float &distance, 
					   CVector3 &vNormal); 
	float PlaneDistanceFromOrigin(CVector3 Normal, CVector3 Point); 
	bool PlaneIntersection(CVector3 *InterSectionPoint, const Plane *p0, const Plane *p1, 
		                   const Plane* p2); 
 
	float a, b, c, d; 
}; 
 
class tFace 
{ 
protected: 
	CVector3 GetInterSectionPoint(CVector3 vLine[2]); 
 
public: 
	tFace() {} 
	tFace(CVector3 p1, CVector3 p2, CVector3 p3); 
	~tFace(); 
 
	// Line-Polygon collision detection only for triangles (can easily be modified) 
	bool CollidedWithLine(CVector3 vLine[2], CVector3 vPoly[3]); 
	bool CollidedWithSphere(CVector3 vPoly[3], CVector3 &vNormal, CVector3 &vCenter, float radius); 
	bool InsidePolygon(CVector3 vIPoint, CVector3 vPoly[], int VertexCount); 
	bool EdgeSphereCollision(CVector3 &vCenter, CVector3 vPoly[], int VertexCount, float radius); 
	 
	int vertIndex[3]; 
	int coordIndex[3]; 
	int neighbourIndices[3]; 
	CVector3 pFaceNormal; 
	Plane PlaneEq; 
	bool visible; 
}; 


class CMatrix4x4 
{ 
   public: 
      CMatrix4x4(); 
      CMatrix4x4(const CMatrix4x4 &m); 
      CMatrix4x4(float r11, float r12, float r13, float r14, 
                 float r21, float r22, float r23, float r24, 
                 float r31, float r32, float r33, float r34, 
                 float r41, float r42, float r43, float r44); 
      ~CMatrix4x4(); 
 
      void Clear(); 
      void Zero(); 
 
	  CVector4 GetRow(int position) const; 
	  CVector4 GetColumn(int position) const; 
 
      void operator=(CMatrix4x4 m); 
	  void operator=(float _Matrix[16]);

      CMatrix4x4 operator-(CMatrix4x4 m); 
      CMatrix4x4 operator+(CMatrix4x4 m); 
      CMatrix4x4 operator*(CMatrix4x4 m); 
      CMatrix4x4 operator/(CMatrix4x4 m); 
 
      void operator +=(CMatrix4x4 m); 
      void operator -=(CMatrix4x4 m); 
      void operator *=(CMatrix4x4 m); 
      void operator /=(CMatrix4x4 m); 
       
      CMatrix4x4 operator-(float f); 
      CMatrix4x4 operator+(float f); 
      CMatrix4x4 operator*(float f); 
      CMatrix4x4 operator/(float f); 
 
      CVector4 operator *(CVector4 v); 
 
      void operator -=(float f); 
      void operator +=(float f); 
      void operator *=(float f); 
      void operator /=(float f); 
 
      bool operator ==(CMatrix4x4 v); 
      bool operator !=(CMatrix4x4 v); 
 
      void Transpose(); 
	  void Translate(CVector4 Translate); 
      void Translate(float x, float y, float z); 
      void Rotate(double angle, int x, int y, 
                  int z); 
      bool inverseMatrix(CMatrix4x4 m); 
      bool invertMatrix(CMatrix4x4 m); 
      CVector4 VectorMatrixMultiply(CVector4 v); 
      CVector4 VectorMatrixMultiply3x3(CVector4 v); 
	  void scaleTranslateToFit(const CVector3 vMin, const CVector3 vMax); 
	  void makeScaleMtx(float x, float y, float z); 
	  CVector3 mulHomogenPoint(CVector3 v); 
      CVector4 transformPoint(CVector4 v); 
	  CVector3 transformPoint(CVector3 v); 
      void CreateShadowMatrix(CVector4 planeNormal, 
                              CVector4 lightPos); 

	  void createProjectionMatrix(float fovx, float aspect, float znear, float zfar);
	  void createViewMatrix( CVector3 eye,  CVector3 target,  CVector3 up);


	  operator float* () const {return (float*) this;} 
	  operator const float* () const {return (const float*) this;} 
  
	  float operator[](int row);
	  const float operator[](int row) const;



private:
      float matrix[16]; 
}; 


class Interpolate
{
public:
	static CVector3 Linear( CVector3 &v1, CVector3 &v2, float mu );
	static CVector3 Cosine( CVector3 &v1, CVector3 &v2, float mu );
	static CVector3 Cubic( CVector3 &v0, CVector3 &v1, CVector3 &v2, CVector3 &v3, float mu );
	static CVector3 Hermite( CVector3 &v1, CVector3 &v2, CVector3 &t1, CVector3 &t2, float mu );
	static CVector3 Cardinal( CVector3 &v0, CVector3 &v1, CVector3 &v2, CVector3 &v3, float mu, float tightness );
	static CVector3 CatmullRom( CVector3 &v0, CVector3 &v1, CVector3 &v2, CVector3 &v3, float mu );
};



//-----------------------------------------------------------------------------
// A row-major 3x3 matrix class.
//
// Matrices are concatenated in a left to right order.
// Multiplies vectors to the left of the matrix.

class CMatrix3x3
{
    friend CVector3 operator*(const CVector3 &lhs, const CMatrix3x3 &rhs);
    friend CMatrix3x3 operator*(float scalar, const CMatrix3x3 &rhs);

public:
    static const CMatrix3x3 IDENTITY;
    static CMatrix3x3 createFromAxes(const CVector3 &x, const CVector3 &y, const CVector3 &z);
    static CMatrix3x3 createFromAxesTransposed(const CVector3 &x, const CVector3 &y, const CVector3 &z);
    static CMatrix3x3 createFromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
    static CMatrix3x3 createMirror(const CVector3 &planeNormal);
    static CMatrix3x3 createOrient(const CVector3 &from, const CVector3 &to);
    static CMatrix3x3 createRotate(const CVector3 &axis, float degrees);
    static CMatrix3x3 createScale(float sx, float sy, float sz);


    CMatrix3x3();
    CMatrix3x3(float m11, float m12, float m13,
            float m21, float m22, float m23,
            float m31, float m32, float m33);
    ~CMatrix3x3();

    float *operator[](int row);
    const float *operator[](int row) const;
	operator float* () const {return (float*) this;} 
	operator const float* () const {return (const float*) this;} 

    bool operator==(const CMatrix3x3 &rhs) const;
    bool operator!=(const CMatrix3x3 &rhs) const;

    CMatrix3x3 &operator+=(const CMatrix3x3 &rhs);
    CMatrix3x3 &operator-=(const CMatrix3x3 &rhs);
    CMatrix3x3 &operator*=(const CMatrix3x3 &rhs);
    CMatrix3x3 &operator*=(float scalar);
    CMatrix3x3 &operator/=(float scalar);

    CMatrix3x3 operator+(const CMatrix3x3 &rhs) const;
    CMatrix3x3 operator-(const CMatrix3x3 &rhs) const;
    CMatrix3x3 operator*(const CMatrix3x3 &rhs) const;
    CMatrix3x3 operator*(float scalar) const;
    CMatrix3x3 operator/(float scalar) const;

	
    float determinant() const;
	
	void createNormalMatrix(const CMatrix4x4 &rModelViewMatrix);
    void fromAxes(const CVector3 &x, const CVector3 &y, const CVector3 &z);
    void fromAxesTransposed(const CVector3 &x, const CVector3 &y, const CVector3 &z);
    void fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
    void identity();
    CMatrix3x3 inverse() const;
    void orient(const CVector3 &from, const CVector3 &to);
    void rotate(const CVector3 &axis, float degrees);
    void scale(float sx, float sy, float sz);
    void toAxes(CVector3 &x, CVector3 &y, CVector3 &z) const;
    void toAxesTransposed(CVector3 &x, CVector3 &y, CVector3 &z) const;
    void toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const;
	const float *getMatrix();
    CMatrix3x3 transpose() const;
	
private:
	  float mtx[3][3];
};


inline CVector3 operator*(const CVector3 &lhs, const CMatrix3x3 &rhs)
{
    return CVector3(
        (lhs.x * rhs.mtx[0][0]) + (lhs.y * rhs.mtx[1][0]) + (lhs.z * rhs.mtx[2][0]),
        (lhs.x * rhs.mtx[0][1]) + (lhs.y * rhs.mtx[1][1]) + (lhs.z * rhs.mtx[2][1]),
        (lhs.x * rhs.mtx[0][2]) + (lhs.y * rhs.mtx[1][2]) + (lhs.z * rhs.mtx[2][2]));
}

 
class CQuaternion  
{ 
   public: 
	CQuaternion() { x = y = z = 0.0f;   w = 1.0f;  }
	
	// Creates a constructor that will allow us to initialize the quaternion when creating it
	CQuaternion(float X, float Y, float Z, float W) 
	{ 
		x = X;		y = Y;		z = Z;		w = W;
	}

	// Creates a quaternion from a CVector4
	CQuaternion(CVector4 &v) 
	{ 
		x = v.x;		y = v.y;		z = v.z;		w = v.w;
	}


      void operator=(const CQuaternion &q) 
         { 
            // This will make this quaternion equal to q. 
            w = q.w; x = q.x; y = q.y; z = q.z; 
         } 

 
      CQuaternion operator*(const CQuaternion &q) 
         { 
            return CQuaternion(w * q.x + x * q.w + y * q.z - z * q.y, 
                               w * q.y - x * q.z + y * q.w + z * q.x, 
                               w * q.z + x * q.y - y * q.x + z * q.w, 
                               w * q.w - x * q.x - y * q.y - z * q.z); 
         } 
 

CVector3 CQuaternion::operator*(CVector3 v)
{

	// First we store the x,y,z of the quaternion in a CVector3
	CVector3 q(x, y, z);
	
	// The is the first part of the equation: ( vec * ( 2w^2 - 1) )
	CVector3 t1 = v * (2.0f * w * w - 1.0f);

	// This is the second part of the equation: (2 * (q_xyz dot vec) * q_xyz)
 
	CVector3 t2;
	t2 =   q * q.Dot(v) * 2.0f;
		//CVector3 t2=2.0f * q.Dot(v) * q;

	
	// This is the third and final part of the equation: (2 * w * (q_xyz cross vec))
	CVector3 t3 =  q.Cross(v) *2*w;

	//*w

	// Now add up all the vectors and return the final resultant vector
	return t1 + t2 + t3;
}





      CQuaternion operator+(const CQuaternion &q) 
         { 
            CQuaternion result; 
 
            CVector4 v1(x, y, z), v2(q.x, q.y, q.z), cross, v3; 
 
            cross.CrossProduct(v2, v1); 
            v1 *= q.w; 
            v2 *= w; 
             
            v3 = v1 + v2; 
            v3 = cross + v3; 
 
            v3.w = w * q.w - (x * q.x + y * q.y + z * q.z); 
             
            result.x = v3.x; result.y = v3.y; result.z = v3.z; result.w = v3.w; 
 
            return result; 
         } 
 
      void CreateQuatFromAxis(CVector4 a, float radians) 
         { 
            float sine = 0.0f; 
             
            radians = radians * 0.5f; 
            sine = (float)sin(radians); 
             
            a.Normalize(); 
 
            x = a.x; y = a.y; z = a.z; 
 
            x *= sine; 
            y *= sine; 
            z *= sine; 
            w = (float)cos(radians); 
         } 
 
      float Length() 
         { 
            return (float)sqrt(x * x + y * y + z * z + w * w); 
         } 
 
 
      void Normal() 
         { 
            float len = Length(); 
            len = 1 / len; 
 
            x = x * len; 
            y = y * len; 
            z = z * len; 
            w = w * len; 
         } 
 
 
      CQuaternion Conjugate() 
         { 
            return CQuaternion(-x, -y, -z, w); 
         } 
 
 
      CQuaternion CrossProduct(const CQuaternion &q) 
         { 
            CQuaternion crossProduct; 
 
            crossProduct.x = w * q.x + x * q.w + y * q.z - z * q.y; 
            crossProduct.y = w * q.y + x * q.z + y * q.w - z * q.x; 
            crossProduct.z = w * q.z + x * q.y + y * q.x - z * q.w; 
            crossProduct.w = w * q.w - x * q.x - y * q.y - z * q.z; 
 
            return crossProduct; 
         } 
 
	   void Rotatef(float amount, float xAxis, float yAxis, float zAxis) 
         { 
            // Normalize if we have to. 
            if((xAxis + yAxis + zAxis) != 1) 
            { 
               float length = (float)sqrt(xAxis * xAxis + yAxis * yAxis + zAxis * zAxis); 
               xAxis /= length; yAxis /= length; zAxis /= length; 
            } 
 
            // Convert the angle degrees into radians. 
            float angle = GET_RADIANS(amount); 
 
            // Call this once for optimization. 
	         float sine = (float)sin(angle / 2.0f); 
 
            // Create the quaternion. 
	         x = xAxis * sine; 
	         y = yAxis * sine; 
	         z = zAxis * sine; 
            w = (float)cos(angle / 2.0f); 
         } 
 
      void RotationRadiansf(double X, double Y, double Z) 
         { 
            double cosX, cosY, cosZ; 
            double sinX, sinY, sinZ; 
            double cosXY, sinXY; 
 
            sinX = sin(X * 0.5); 
            cosX = cos(X * 0.5); 
             
            sinY = sin(Y * 0.5); 
            cosY = cos(Y * 0.5); 
             
            sinZ = sin(Z * 0.5); 
            cosZ = cos(Z * 0.5); 
 
            cosXY = cosX * cosY; 
            sinXY = sinX * sinY; 
 
            x = (float)(sinX * cosY * cosZ - cosX * sinY * sinZ); 
            y = (float)(cosX * sinY * cosZ + sinX * cosY * sinZ); 
            z = (float)(cosXY * sinZ - sinXY * cosZ); 
            w = (float)(cosXY * cosZ + sinXY * sinZ);  
         } 
 
	   void CreateMatrix(float *pMatrix) 
         { 
            // Error checking... 
	         if(!pMatrix) 
               return; 
 
	         // Calculate the first row. 
            pMatrix[0]  = 1.0f - 2.0f * (y * y + z * z);  
            pMatrix[1]  = 2.0f * (x * y + z * w); 
            pMatrix[2]  = 2.0f * (x * z - y * w); 
            pMatrix[3]  = 0.0f;   
 
            pMatrix[4]  = 2.0f * (x * y - z * w);   
            pMatrix[5]  = 1.0f - 2.0f * (x * x + z * z);  
            pMatrix[6]  = 2.0f * (z * y + x * w);   
            pMatrix[7]  = 0.0f;   
 
            pMatrix[8]  = 2.0f * (x * z + y * w); 
            pMatrix[9]  = 2.0f * (y * z - x * w); 
            pMatrix[10] = 1.0f - 2.0f * (x * x + y * y);   
            pMatrix[11] = 0.0f;   
 
            pMatrix[12] = 0;   
            pMatrix[13] = 0;   
            pMatrix[14] = 0;   
            pMatrix[15] = 1.0f; 
         } 
 
     CQuaternion Slerp(CQuaternion &q1, CQuaternion &q2, float t)
	 {
	// Create a local quaternion to store the interpolated quaternion
	CQuaternion qInterpolated;

	// This function is the milk and honey of our quaternion code, the rest of
	// the functions are an appendage to what is done here.  Everyone understands
	// the terms, "matrix to quaternion", "quaternion to matrix", "create quaternion matrix",
	// "quaternion multiplication", etc.. but "SLERP" is the stumbling block, even a little 
	// bit after hearing what it stands for, "Spherical Linear Interpolation".  What that
	// means is that we have 2 quaternions (or rotations) and we want to interpolate between 
	// them.  The reason what it's called "spherical" is that quaternions deal with a sphere.  
	// Linear interpolation just deals with 2 points primarily, where when dealing with angles
	// and rotations, we need to use sin() and cos() for interpolation.  If we wanted to use
	// quaternions for camera rotations, which have much more instant and jerky changes in 
	// rotations, we would use Spherical-Cubic Interpolation.  The equation for SLERP is this:
	//
	// q = (((b.a)^-1)^t)a
	//
	// Go here for an a detailed explanation and proofs of this equation:
	//
	// http://www.magic-software.com/Documentation/quat.pdf
	//
	// Now, Let's code it

	// Here we do a check to make sure the 2 quaternions aren't the same, return q1 if they are
	if(q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w) 
		return q1;

	// Following the (b.a) part of the equation, we do a dot product between q1 and q2.
	// We can do a dot product because the same math applied for a 3D vector as a 4D vector.
	float result = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

	// If the dot product is less than 0, the angle is greater than 90 degrees
	if(result < 0.0f)
	{
		// Negate the second quaternion and the result of the dot product
		q2 = CQuaternion(-q2.x, -q2.y, -q2.z, -q2.w);
		result = -result;
	}

	// Set the first and second scale for the interpolation
	float scale0 = 1 - t, scale1 = t;

	// Next, we want to actually calculate the spherical interpolation.  Since this
	// calculation is quite computationally expensive, we want to only perform it
	// if the angle between the 2 quaternions is large enough to warrant it.  If the
	// angle is fairly small, we can actually just do a simpler linear interpolation
	// of the 2 quaternions, and skip all the complex math.  We create a "delta" value
	// of 0.1 to say that if the cosine of the angle (result of the dot product) between
	// the 2 quaternions is smaller than 0.1, then we do NOT want to perform the full on 
	// interpolation using.  This is because you won't really notice the difference.

	// Check if the angle between the 2 quaternions was big enough to warrant such calculations
	if(1 - result > 0.1f)
	{
		// Get the angle between the 2 quaternions, and then store the sin() of that angle
		float theta = (float)acos(result);
		float sinTheta = (float)sin(theta);

		// Calculate the scale for q1 and q2, according to the angle and it's sine value
		scale0 = (float)sin( ( 1 - t ) * theta) / sinTheta;
		scale1 = (float)sin( ( t * theta) ) / sinTheta;
	}	

	// Calculate the x, y, z and w values for the quaternion by using a special
	// form of linear interpolation for quaternions.
	qInterpolated.x = (scale0 * q1.x) + (scale1 * q2.x);
	qInterpolated.y = (scale0 * q1.y) + (scale1 * q2.y);
	qInterpolated.z = (scale0 * q1.z) + (scale1 * q2.z);
	qInterpolated.w = (scale0 * q1.w) + (scale1 * q2.w);

	// Return the interpolated quaternion
	return qInterpolated;
}



      void Slerp(const CQuaternion &q1, const CQuaternion &q2, float t) 
      { 
         float cosTheta = 0.0f; 
         float sinTheta = 0.0f; 
         float beta = 0.0f; 
         float q2Array[4]; 
 
         // Temporary array to hold second quaternion. 
         q2Array[0] = q2.x; q2Array[1] = q2.y; q2Array[2] = q2.z; q2Array[3] = q2.w; 
 
         cosTheta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w; 
 
         if(cosTheta ==  0.0f) 
            { 
               // Flip sigh if so. 
               q2Array[0] = -q2Array[0]; q2Array[1] = -q2Array[1]; 
               q2Array[2] = -q2Array[2]; q2Array[3] = -q2Array[3]; 
               cosTheta = -cosTheta; 
            } 
 
         beta = 1.0f - t; 
 
         if(1.0f - cosTheta > 0.001f) 
            { 
               // We are using spherical interpolation. 
               cosTheta = (float)acos(cosTheta); 
               sinTheta = 1.0f / (float)sin(cosTheta); 
               beta = (float)sin(cosTheta * beta) * sinTheta; 
               t = (float)sin(cosTheta * t) * sinTheta; 
            } 
 
         // Interpolation. 
         x = beta * q1.x + t * q2Array[0]; 
         y = beta * q1.y + t * q2Array[1]; 
         z = beta * q1.z + t * q2Array[2]; 
         w = beta * q1.w + t * q2Array[3]; 
      } 
 
	   float x, y, z, w;                      // x, y , z, and w axis. 
}; 
 
static bool closeEnough(float f1, float f2);
    

GLfloat mathGetAngle (float x1, float y1,float x2,float y2);					//
GLfloat mathGetRotationToVector(CVector3 position,CVector3 destination);		//Kat obrotu do pozycji
GLfloat mathUnwindAngle_Degrees( GLfloat fAngle );								//
bool mathIsCWFaster( GLfloat fStartAngle, GLfloat fEndAngle );					//
GLfloat mathGet_rotation_vector( GLfloat fStartAngle, GLfloat fEndAngle );		//
GLfloat mathGetfdistance(GLfloat x, GLfloat z,int x2, int z2);					//
// Given the 3 vertices (position and texture coordinates) of a triangle
// calculate and return the triangle's tangent vector. The handedness of
// the local coordinate system is stored in tangent[3]. The bitangent is
// then: vec3 bitangent = cross(normal, tangent.xyz) * tangent.w.
void CalcTangentVector(const CVector3 &vpos1,         // triangle vertex 1 pos
                       const CVector3 &vpos2,         // triangle vertex 2 pos
					   const CVector3 &vpos3,         // triangle vertex 3 pos
                       const CVector2 &vtexCoord1,    // triangle vertex 1 uv
                       const CVector2 &vtexCoord2,    // triangle vertex 1 uv
                       const CVector2 &vtexCoord3,    // triangle vertex 1 uv
                       const CVector3 &vnormal,
                       float tangent[4]) ;           // returned surface tangent

 CVector3 QuatToEuler(btQuaternion* quat);


#endif
