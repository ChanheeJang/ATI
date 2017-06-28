// PointFloat.h: interface for the CPointFloat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTFLOAT_H__7C959AA0_EBBA_11D3_9A5E_52544C0B46BA__INCLUDED_)
#define AFX_POINTFLOAT_H__7C959AA0_EBBA_11D3_9A5E_52544C0B46BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPointFloat  
{
public:
	CPointFloat operator-(CPointFloat a);
	CPointFloat operator *=(float a);
	CPointFloat operator+=(CPointFloat a);
	CPointFloat operator*=(CPointFloat a);
	CPointFloat operator/=(CPointFloat a);
	CPointFloat(float a, float b);
	float y;
	float x;
	CPointFloat();
	virtual ~CPointFloat();
	CPointFloat operator =(CPointFloat &a);
	CPointFloat operator +(CPointFloat a);
};

#endif // !defined(AFX_POINTFLOAT_H__7C959AA0_EBBA_11D3_9A5E_52544C0B46BA__INCLUDED_)
