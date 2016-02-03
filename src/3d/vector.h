struct Vec {
   long x, y, z;
}; 
#define FLOAT_TO_FIXED(f) (long)(f * 65536);
#define FIXEDMUL(x, y) ((x>>8)*(y>>8))
extern long SIN1024[1024];
extern long COS1024[1024];
inline struct Vec VecCreateF( float x_, float y_, float z_) {
    struct Vec r_;
    r_.x = FLOAT_TO_FIXED(x_);
    r_.y = FLOAT_TO_FIXED(y_);
    r_.z = FLOAT_TO_FIXED(z_);
    return r_;
};
struct Vec VecCreateL( long x_, long y_, long z_);
inline void add( struct Vec *A, struct Vec *B) {
    (*A).x+=(*B).x;
    (*A).y+=(*B).y;
    (*A).z+=(*B).z;
}
struct Vec sub( struct Vec A, struct Vec B);
struct Vec mul( struct Vec A, struct Vec B);
struct Vec norm( struct Vec A);
inline float dot( struct Vec A, struct Vec B) {
    return A.x*B.x+A.y*B.y+A.z*B.z;
}
struct Vec cross( struct Vec A, struct Vec B);
inline void rot2D( struct Vec *A, long rad ) {
    long X, Y;
    long sina = SIN1024[rad&1023];
    long cosa = COS1024[rad&1023];
    X = FIXEDMUL((*A).x , cosa ) - FIXEDMUL((*A).y , sina);
    Y = FIXEDMUL((*A).x , sina ) + FIXEDMUL((*A).y , cosa);
    (*A).x=X;
    (*A).y=Y;
}