#include "D3DCore_impl.h"

//Random Methods
// Returns random float in [0, 1).
D3DX10INLINE float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

void D3DCore_Impl::Random_Seed( unsigned int seed )
{
	srand( seed );
}

int D3DCore_Impl::Random_Int( int min, int max )
{
	return min + rand()%(max-min + 1);
}

float D3DCore_Impl::Random_Float( float min, float max )
{
	return min + RandF()*(max-min);
}

D3DXVECTOR3 D3DCore_Impl::Random_UnitVec3()
{
	D3DXVECTOR3 v(RandF(), RandF(), RandF());
	D3DXVec3Normalize(&v, &v);
	return v;
}