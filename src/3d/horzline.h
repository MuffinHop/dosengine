/*RAMBUFFER ONLY*/
long horzlbufindx = 0;
#define horzlbufferlen 320*200
word horzlbuffer[horzlbufferlen];
inline static void pushHorizontalLine( word x0, word x1, word y, byte color) {
	word pointer8, length8;
	long pointer32, color32;
    dword *videobuffer32;
    word i;
    if(y < 0) return;
    if(y > 200) return;
    if(horzlbufindx>horzlbufferlen) return;
	if( x0 > x1 ) { //if bigger xor-swap
		x0 ^= x1;
		x1 ^= x0;
		x0 ^= x1;
	}
	pointer8 = y * 320 + x0;
	length8 = x1 - x0;
	horzlbuffer[horzlbufindx++] = pointer8;
	horzlbuffer[horzlbufindx++] = length8;
	horzlbuffer[horzlbufindx++] = (word)color;

}
inline static void drawHorizontalLines( ) {
	byte color;
	word pointer8, length8;
	long pointer32, color32, x0;
    dword *videobuffer32;
    word i;
    for(i=0; i<horzlbufindx; i+=3){
		pointer8 = horzlbuffer[i+0];
		length8 = horzlbuffer[i+1];
		color = horzlbuffer[i+2];
		x0 = pointer8 % 320;
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
		if(length8>0)
		for ( ;length8 > 0;length8--) {
			if(x0 > 0 && x0 < 318)
				videobuffer[ pointer8 ] = color;
			pointer8++;
			x0++;
		}
	}
	horzlbufindx=0;
}