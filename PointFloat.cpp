// PointFloat.cpp: implementation of the CPointFloat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PointFloat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointFloat::CPointFloat()
{

}

CPointFloat::~CPointFloat()
{

}

CPointFloat::CPointFloat(float a, float b)
{
	x = a, y = b;
}

CPointFloat CPointFloat::operator+=(CPointFloat a)
{
	x = x + a.x;
	y = y + a.y;

	return *this;
}

CPointFloat CPointFloat::operator*=(CPointFloat a)
{
	x = x * a.x;
	y = y * a.y;

	return *this;
}

CPointFloat CPointFloat::operator/=(CPointFloat a)
{
	if( x == 0.0 )	x = 0.0;
	else			x = x / a.x;
	if( y == 0.0 )	y = 0.0;
	else			y = y / a.y;
	
	return *this;
}

CPointFloat CPointFloat::operator =(CPointFloat& a)
{
	x = a.x;
	y = a.y;

	return *this;
}


CPointFloat CPointFloat::operator +(CPointFloat a)
{
	CPointFloat ret;

	ret.x = x+a.x;
	ret.y = y+a.y;

	return ret;
}

CPointFloat CPointFloat::operator *=(float a)
{
	x *= a;
	y *= a;

	return *this;
}

CPointFloat CPointFloat::operator -(CPointFloat a)
{
	CPointFloat ret;

	ret.x = x-a.x;
	ret.y = y-a.y;

	return ret;
}
