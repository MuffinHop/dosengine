#include "src\misc\definer.h"
void identity( struct Matrix4x4 *matrix) {
	(*matrix).m[0][0] = 65536;
	(*matrix).m[0][1] = 0;
	(*matrix).m[0][2] = 0;
	(*matrix).m[0][3] = 0;
	(*matrix).m[1][0] = 0;
	(*matrix).m[1][1] = 65536;
	(*matrix).m[1][2] = 0;
	(*matrix).m[1][3] = 0;
	(*matrix).m[2][0] = 0;
	(*matrix).m[2][1] = 0;
	(*matrix).m[2][2] = 65536; 
	(*matrix).m[2][3] = 0;
	(*matrix).m[3][0] = 0;
	(*matrix).m[3][1] = 0;
	(*matrix).m[3][2] = 0;
	(*matrix).m[3][3] = 65536;
}
void rotate( struct Matrix4x4 *matrix, struct Vec *v) {
	long cx = FLOAT_TO_FIXED( cos( (*v).x ) );
	long cy = FLOAT_TO_FIXED( cos( (*v).y ) );
	long cz = FLOAT_TO_FIXED( cos( (*v).z ) );
	long sx = FLOAT_TO_FIXED( sin( (*v).x ) );
	long sy = FLOAT_TO_FIXED( sin( (*v).y ) );
	long sz = FLOAT_TO_FIXED( sin( (*v).z ) );

	(*matrix).m[0][0] = FIXEDMUL(cy,cz);
	(*matrix).m[0][1] = FIXEDMUL(cy,sz);
	(*matrix).m[0][2] = -sy;
	(*matrix).m[0][3] = 0;
	(*matrix).m[1][0] = FIXEDMUL(FIXEDMUL(sx,sy),cz)-FIXEDMUL(cx,sz); 	
	(*matrix).m[1][1] = FIXEDMUL(FIXEDMUL(sx,sy),sz)+FIXEDMUL(cx,cz);	
	(*matrix).m[1][2] = FIXEDMUL(sx,cy);				
	(*matrix).m[1][3] = 0;
	(*matrix).m[2][0] = FIXEDMUL(FIXEDMUL(cx,sy),cz)+FIXEDMUL(sx,sz); 	
	(*matrix).m[2][1] = FIXEDMUL(FIXEDMUL(cx,sy),sz)-FIXEDMUL(sx,cz);	
	(*matrix).m[2][2] = FIXEDMUL(cx,cy);				
	(*matrix).m[2][3] = 0;
	(*matrix).m[3][0] = 0;
	(*matrix).m[3][1] = 0;
	(*matrix).m[3][2] = 0;
	(*matrix).m[3][3] = 65536;
}
struct Vec get_translation( struct Matrix4x4 *matrix) {
	return VecCreateL( (*matrix).m[3][0], (*matrix).m[3][1], (*matrix).m[3][2] );
}