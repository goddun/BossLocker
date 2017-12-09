#include "stdafx.h"
#include"Utility.h"


// v1 = ��������, v2 = �߰�����, v3 = ������  
// fDetail = 0.0 ���� 1.0 ������ ��. 0.0�� ��������, 1.0�� �������� ��Ÿ����.  

Vector3 _getBezier(const Vector3 v1, const Vector3 v2, const Vector3 v3, const Vector3 v4, const float fDetail)
{
	Vector3 vResult = { 0,0,0 };

	vResult.x = v1.x * B1(fDetail) + v2.x * B2(fDetail) + v3.x * B3(fDetail) + v4.x * B4(fDetail);
	vResult.y = v1.y * B1(fDetail) + v2.y * B2(fDetail) + v3.y * B3(fDetail) + v4.y * B4(fDetail);
	vResult.z = v1.z * B1(fDetail) + v2.z * B2(fDetail) + v3.z * B3(fDetail) + v4.z * B4(fDetail);

	return vResult;
}