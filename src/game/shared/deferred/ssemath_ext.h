#ifndef SSEMATH_EXT_H
#define SSEMATH_EXT_H

#include "mathlib\ssemath.h"

FORCEINLINE float Dot4SIMD2( const fltx4 &a, const fltx4 &b )
{
	fltx4 x = MulSIMD( a, b );
	fltx4 y = _mm_movehl_ps( x, x );
	y = _mm_add_ss( y, x );
	x = _mm_shuffle_ps( x, x, _MM_SHUFFLE( 0, 0, 0, 1 ) );
	x = _mm_add_ss( y, x );

	return x.m128_f32[0];
}

FORCEINLINE void NormalizeInPlaceSIMD( fltx4 &v )
{
	fltx4 var1 = MulSIMD( v, v );
	fltx4 var2 = _mm_shuffle_ps( var1, var1, _MM_SHUFFLE( 1, 3, 2, 0 ) ); //0123 -> 2013,
	var1 = AddSIMD( var1, var2 );
	var2 = _mm_shuffle_ps( var2, var2, _MM_SHUFFLE( 1, 3, 2, 0 ) );//2013 -> 1203
	var1 = AddSIMD( var1, var2 );
	v = MulSIMD( v, ReciprocalSqrtEstSIMD( var1 ) );
}

FORCEINLINE void NormaliseFourInPlace( fltx4& v0, fltx4& v1, fltx4& v2, fltx4& v3 )
{
	TransposeSIMD( v0, v1, v2, v3 );

	fltx4 var1 = MulSIMD( v0, v0 );
	var1 = MaddSIMD( v1, v1, var1 );
	var1 = MaddSIMD( v2, v2, var1 );
	var1 = MaddSIMD( v3, v3, var1 );
	var1 = ReciprocalSqrtEstSIMD( var1 );

	v0 = MulSIMD( v0, var1 );
	v1 = MulSIMD( v1, var1 );
	v2 = MulSIMD( v2, var1 );
	v3 = MulSIMD( v3, var1 );

	TransposeSIMD( v0, v1, v2, v3 );
}

// assumes that m is transposed before hand
FORCEINLINE fltx4 FourDotProducts( fltx4* m, fltx4 v )
{
	fltx4 dot=MulSIMD( m[0], _mm_shuffle_ps( v, v, MM_SHUFFLE_REV( 0, 0, 0, 0 ) ) );
	dot=MaddSIMD( m[1], _mm_shuffle_ps( v, v, MM_SHUFFLE_REV( 1, 1, 1, 1 ) ), dot);
	dot=MaddSIMD( m[2], _mm_shuffle_ps( v, v, MM_SHUFFLE_REV( 2, 2, 2, 2 ) ), dot);
	dot=MaddSIMD( m[3], _mm_shuffle_ps( v, v, MM_SHUFFLE_REV( 3, 3, 3, 3 ) ), dot);

	return dot;
}

// assumes that all are transposed before hand
FORCEINLINE void FourCrossProducts
(
	const fltx4& a0,
	const fltx4& a1,
	const fltx4& a2,
	const fltx4& a3,
	const fltx4& b0,
	const fltx4& b1,
	const fltx4& b2,
	const fltx4& b3,
	fltx4& out0,
	fltx4& out1,
	fltx4& out2,
	fltx4& out3
)
{
	/*The required transpose
	fltx4 x0 = a0;
	fltx4 x1 = a1;
	fltx4 x2 = a2;
	fltx4 x3 = a3;
	fltx4 y0 = b0;
	fltx4 y1 = b1;
	fltx4 y2 = b2;
	fltx4 y3 = b3;

	TransposeSIMD( x0, x1, x2, x3 );
	TransposeSIMD( y0, y1, y2, y3 );*/

	out0 =SubSIMD(MulSIMD(a1,b2),MulSIMD(a2,b1));
	out1 =SubSIMD(MulSIMD(a2,b0),MulSIMD(a0,b2));
	out2 =SubSIMD(MulSIMD(a0,b1),MulSIMD(a1,b0));
	out3 = LoadOneSIMD();

	TransposeSIMD( out0, out1, out2, out3 );
}

#endif