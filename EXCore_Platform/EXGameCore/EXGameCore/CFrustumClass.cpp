#include "stdafx.h"
#include "CFrustumClass.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CFrustumObject::CalculateFrustum(D3DMATRIX proj, D3DMATRIX modl)
{
	float clip[16];
    //Now that we have our modelview and projection matrix, if we combine these 2 matrices,
    //it will give us our clipping planes.  To combine 2 matrices, we multiply the
    D3DVECTOR up,eye,at,dir;
	eye.x = modl._41;
	eye.y = modl._42;
	eye.z = modl._43;
	m_matLib.GetOrientation(modl,&dir,&up);
	at.x = modl._41+dir.x;
	at.y = modl._42+dir.y;
	at.z = modl._43+dir.z;
	D3DXMatrixLookAtLH((D3DXMATRIX*)&modl, (D3DXVECTOR3*)&eye, (D3DXVECTOR3*)&at, (D3DXVECTOR3*)&up);//this does a view matrix out of the cameras world transform
    m_matLib.fastMatrixMultiply(proj, modl, proj);
	CopyMemory(clip,proj.m,sizeof(float)*16);



    //Now we actually want to get the sides of the frustum.  To do this we take
    //the clipping planes we received above and extract the sides from them.

    //This will extract the RIGHT side of the frustum
    m_Frustum[m_right][m_a] = clip[ 3] - clip[ 0];
    m_Frustum[m_right][m_b] = clip[ 7] - clip[ 4];
    m_Frustum[m_right][m_c] = clip[11] - clip[ 8];
    m_Frustum[m_right][m_d] = clip[15] - clip[12];

    //Now that we have a normal (A,B,C) and a distance (D) to the plane,
    //we want to normalize that normal and distance.

	
    //Normalize the RIGHT side
    NormalizePlane(m_Frustum, m_right);

    //This will extract the LEFT side of the frustum
    m_Frustum[m_left][m_a] = clip[ 3] + clip[ 0];
    m_Frustum[m_left][m_b] = clip[ 7] + clip[ 4];
    m_Frustum[m_left][m_c] = clip[11] + clip[ 8];
    m_Frustum[m_left][m_d] = clip[15] + clip[12];

    //Normalize the LEFT side
    NormalizePlane(m_Frustum, m_left);

    //This will extract the BOTTOM side of the frustum
    m_Frustum[m_bottom][m_a] = clip[ 3] + clip[ 1];
    m_Frustum[m_bottom][m_b] = clip[ 7] + clip[ 5];
    m_Frustum[m_bottom][m_c] = clip[11] + clip[ 9];
    m_Frustum[m_bottom][m_d] = clip[15] + clip[13];

    //Normalize the BOTTOM side
    NormalizePlane(m_Frustum, m_bottom);

    //This will extract the TOP side of the frustum
    m_Frustum[m_top][m_a] = clip[ 3] - clip[ 1];
    m_Frustum[m_top][m_b] = clip[ 7] - clip[ 5];
    m_Frustum[m_top][m_c] = clip[11] - clip[ 9];
    m_Frustum[m_top][m_d] = clip[15] - clip[13];

    //Normalize the TOP side
    NormalizePlane(m_Frustum, m_top);

    //This will extract the BACK side of the frustum
    m_Frustum[m_back][m_a] = clip[ 3] - clip[ 2];
    m_Frustum[m_back][m_b] = clip[ 7] - clip[ 6];
    m_Frustum[m_back][m_c] = clip[11] - clip[10];
    m_Frustum[m_back][m_d] = clip[15] - clip[14];

    //Normalize the BACK side
    NormalizePlane(m_Frustum, m_back);

    //This will extract the FRONT side of the frustum
    m_Frustum[m_front][m_a] = clip[ 3] + clip[ 2];
    m_Frustum[m_front][m_b] = clip[ 7] + clip[ 6];
    m_Frustum[m_front][m_c] = clip[11] + clip[10];
    m_Frustum[m_front][m_d] = clip[15] + clip[14];

    //Normalize the FRONT side
    NormalizePlane(m_Frustum, m_front);
}



void CFrustumObject::NormalizePlane(float frustum[6][4], int side)
{
    //Here we calculate the magnitude of the normal to the plane (point A B C)
    //Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
    //To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
    float magnitude = (float)sqrt(frustum[side][m_a] * frustum[side][m_a] +
                                  frustum[side][m_b] * frustum[side][m_b] +
                                  frustum[side][m_c] * frustum[side][m_c] );

    //Then we divide the plane't's magnitude.
    //This makes it easier to work with.
    frustum[side][m_a] /= magnitude;
    frustum[side][m_b] /= magnitude;
    frustum[side][m_c] /= magnitude;
    frustum[side][m_d] /= magnitude;
}

BOOL CFrustumObject::CubeInFrustum( float x, float y, float z, float size )
{
    if(m_bypass)
       return TRUE;
    //This test is a bit more work, but not too much more complicated.
    //Basically, what is going on is, that we are given the center of the cube,
    //and half the length.  Think of it like a radius.  Then we checking each point
    //in the cube and seeing if it is inside the frustum.  If a point is found in front
    //of a side, then we skip to the next side.  If we get to a plane that does NOT have
    //a point in front of it, then it will return false.

    //*Note* - This will sometimes say that a cube is inside the frustum when it isn't.
    //This happens when all the corners of the bounding box are not behind any one plane.
    //This is rare and shouldn't effect the overall rendering speed.

    for(int i = 0; i < 6; i++ ) {
      if(m_Frustum[i][m_a] * (x - size) + m_Frustum[i][m_b] * (y - size) + m_Frustum[i][m_c] * (z - size) + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * (x + size) + m_Frustum[i][m_b] * (y - size) + m_Frustum[i][m_c] * (z - size) + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * (x - size) + m_Frustum[i][m_b] * (y + size) + m_Frustum[i][m_c] * (z - size) + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * (x + size) + m_Frustum[i][m_b] * (y + size) + m_Frustum[i][m_c] * (z - size) + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * (x - size) + m_Frustum[i][m_b] * (y - size) + m_Frustum[i][m_c] * (z + size) + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * (x + size) + m_Frustum[i][m_b] * (y - size) + m_Frustum[i][m_c] * (z + size) + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * (x - size) + m_Frustum[i][m_b] * (y + size) + m_Frustum[i][m_c] * (z + size) + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * (x + size) + m_Frustum[i][m_b] * (y + size) + m_Frustum[i][m_c] * (z + size) + m_Frustum[i][m_d] > 0)
        continue;
      //If we get here, it isn't in the frustum
      return FALSE;
    }
    return TRUE;
}

BOOL CFrustumObject::BoundingBoxOutOfFrustum( D3DVECTOR & min, 
								              D3DVECTOR & max )
{
    if(m_bypass)
       return FALSE;

	//ab conversion for bounding box, a bit faster
    for(int i = 0; i < 6; i++ ) {
      if(m_Frustum[i][m_a] * min.x + m_Frustum[i][m_b] * min.y + m_Frustum[i][m_c] * min.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * max.x + m_Frustum[i][m_b] * min.y + m_Frustum[i][m_c] * min.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * min.x + m_Frustum[i][m_b] * max.y + m_Frustum[i][m_c] * min.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * max.x + m_Frustum[i][m_b] * max.y + m_Frustum[i][m_c] * min.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * min.x + m_Frustum[i][m_b] * min.y + m_Frustum[i][m_c] * max.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * max.x + m_Frustum[i][m_b] * min.y + m_Frustum[i][m_c] * max.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * min.x + m_Frustum[i][m_b] * max.y + m_Frustum[i][m_c] * max.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * max.x + m_Frustum[i][m_b] * max.y + m_Frustum[i][m_c] * max.z + m_Frustum[i][m_d] > 0)
        continue;
      //If we get here, it isn't in the frustum
      return TRUE;
    }
    return FALSE;
}

BOOL CFrustumObject::BoundingBoxOutOfFrustum( D3DXVECTOR3 & min, 
								              D3DXVECTOR3 & max )
{
	if(m_bypass)
       return FALSE;
	//ab conversion for bounding box, a bit faster
    for(int i = 0; i < 6; i++ ) {
      if(m_Frustum[i][m_a] * min.x + m_Frustum[i][m_b] * min.y + m_Frustum[i][m_c] * min.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * max.x + m_Frustum[i][m_b] * min.y + m_Frustum[i][m_c] * min.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * min.x + m_Frustum[i][m_b] * max.y + m_Frustum[i][m_c] * min.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * max.x + m_Frustum[i][m_b] * max.y + m_Frustum[i][m_c] * min.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * min.x + m_Frustum[i][m_b] * min.y + m_Frustum[i][m_c] * max.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * max.x + m_Frustum[i][m_b] * min.y + m_Frustum[i][m_c] * max.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * min.x + m_Frustum[i][m_b] * max.y + m_Frustum[i][m_c] * max.z + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * max.x + m_Frustum[i][m_b] * max.y + m_Frustum[i][m_c] * max.z + m_Frustum[i][m_d] > 0)
        continue;
      //If we get here, it isn't in the frustum
      return TRUE;
    }
    return FALSE;
}

BOOL CFrustumObject::BoundingBoxInView( ABBOX & cl_box )
{
	if(m_bypass)
       return TRUE;
	//ab conversion for bounding box, a bit faster
    for(int i = 0; i < 6; i++ ) {
      if(m_Frustum[i][m_a] * cl_box.minX + m_Frustum[i][m_b] * cl_box.minY + m_Frustum[i][m_c] * cl_box.minZ + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * cl_box.maxX + m_Frustum[i][m_b] * cl_box.minY + m_Frustum[i][m_c] * cl_box.minZ + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * cl_box.minX + m_Frustum[i][m_b] * cl_box.maxY + m_Frustum[i][m_c] * cl_box.minZ + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * cl_box.maxX + m_Frustum[i][m_b] * cl_box.maxY + m_Frustum[i][m_c] * cl_box.minZ + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * cl_box.minX + m_Frustum[i][m_b] * cl_box.minY + m_Frustum[i][m_c] * cl_box.maxZ + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * cl_box.maxX + m_Frustum[i][m_b] * cl_box.minY + m_Frustum[i][m_c] * cl_box.maxZ + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * cl_box.minX + m_Frustum[i][m_b] * cl_box.maxY + m_Frustum[i][m_c] * cl_box.maxZ + m_Frustum[i][m_d] > 0)
        continue;
      if(m_Frustum[i][m_a] * cl_box.maxX + m_Frustum[i][m_b] * cl_box.maxY + m_Frustum[i][m_c] * cl_box.maxZ + m_Frustum[i][m_d] > 0)
        continue;
      //If we get here, it isn't in the frustum
      return FALSE;
    }
    return TRUE;
}


BOOL CFrustumObject::SphereInFrustum( float x, float y, float z, float radius )
{
	if(m_bypass)
       return TRUE;
    //Now this function is almost identical to the PointInFrustum(), except we
    //now have to deal with a radius around the point.  The point is the center of
    //the radius.  So, the point might be outside of the frustum, but it doesn't
    //mean that the rest of the sphere is.  It could be half and half.  So instead of
    //checking if it's less than 0, we need to add on the radius to that.  Say the
    //equation produced -2, which means the center of the sphere is the distance of
    //2 behind the plane.  Well, what if the radius was 5?  The sphere is still inside,
    //so we would say, if(-2 < -5) then we are outside.  In that case it's false,
    //so we are inside of the frustum, but a distance of 3.  This is reflected below.

    //Go through all the sides of the frustum
    for(int i = 0; i < 6; i++ )
    //If the center of the sphere is farther away from the plane than the radius
      if( m_Frustum[i][m_a] * x + m_Frustum[i][m_b] * y + m_Frustum[i][m_c] * z + m_Frustum[i][m_d] <= -radius )
      //The distance was greater than the radius so the sphere is outside of the frustum
        return FALSE;
    //The sphere was inside of the frustum!
    return TRUE;
}


