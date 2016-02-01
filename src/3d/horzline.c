#include "src\misc\definer.h"
/*RAMBUFFER ONLY*/
void drawHorizontalLine( int x0, int x1, int y, byte color) {
	long pointer8, pointer32, length8, color32;
    dword *videobuffer32;
    if(y<0) return;
    if(y>200) return;
	if( x0 > x1 ) { //if bigger xor-swap
		x0 ^= x1;
		x1 ^= x0;
		x0 ^= x1;
	}
	pointer8 = y * 320 + x0;
	length8 = x1 - x0;
	pointer32 = 0;
	videobuffer32 = (dword *)&videobuffer[pointer8];
	color32 = color;
	color32 = color32 + (color32 << 8) + (color32 << 16) + (color32 << 24);
	for ( ;length8 > 4;length8-=4) {
		if( x0 > 320 - 6 ) {length8+=4;break;}
		if (x0 > 0  )
			videobuffer32[ pointer32 ] = color32;
		pointer32++;
		pointer8 += 4;
		x0+=4;
	}
	for ( ;length8 > 0;length8--) {
		if(x0 > 0 && x0 < 318)
			videobuffer[ pointer8 ] = color;
		pointer8++;
		x0++;
	}
}