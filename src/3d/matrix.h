struct Matrix4x4{
	long m[4][4];
};
void identity( struct Matrix4x4 *matrix);
void rotate( struct Matrix4x4 *matrix,  struct Vec *v);
struct Vec get_translation( struct Matrix4x4 *matrix);