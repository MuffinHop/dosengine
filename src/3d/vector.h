struct Vec {
   long x, y, z;
}; 
struct Vec VecCreateF( float x_, float y_, float z_);
struct Vec VecCreateL( long x_, long y_, long z_);
struct Vec add( struct Vec A, struct Vec B);
struct Vec sub( struct Vec A, struct Vec B);
struct Vec mul( struct Vec A, struct Vec B);
struct Vec norm( struct Vec A);
float dot( struct Vec A, struct Vec B);
struct Vec cross( struct Vec A, struct Vec B);
struct Vec rot2D( struct Vec A, float rad );