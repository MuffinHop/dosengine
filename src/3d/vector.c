#include "src\misc\definer.h"
struct Vec VecCreateF( float x_, float y_, float z_) {
    struct Vec r_;
    r_.x = FLOAT_TO_FIXED(x_);
    r_.y = FLOAT_TO_FIXED(y_);
    r_.z = FLOAT_TO_FIXED(z_);
    return r_;
};
struct Vec VecCreateL( long x_, long y_, long z_) {
    struct Vec r_;
    r_.x = x_;
    r_.y = y_;
    r_.z = z_;
    return r_;
};
struct Vec add( struct Vec A, struct Vec B) {
    return VecCreateL(A.x+B.x,A.y+B.y,A.z+B.z);
}
struct Vec sub( struct Vec A, struct Vec B) {
    return VecCreateL(A.x-B.x,A.y-B.y,A.z-B.z);
}
struct Vec mul( struct Vec A, struct Vec B) {
    return VecCreateL(A.x*B.x,A.y*B.y,A.z*B.z);
}
struct Vec norm( struct Vec A) {
    float L = sqrt( A.x*A.x + A.y*A.y + A.z * A.z);
    return VecCreateL(A.x/L,A.y/L,A.z/L);
}
float dot( struct Vec A, struct Vec B) {
    return A.x*B.x+A.y*B.y+A.z*B.z;
}
struct Vec cross( struct Vec A, struct Vec B) {
    return VecCreateL(A.y*B.z-A.z*B.y,A.z*B.x-A.x*B.z,A.x*B.y-A.y*B.x);
}
struct Vec rot2D( struct Vec A, float rad ) {
    long X, Y;
    long sina = SIN1024[(int)(rad*1024.0f)&1023];
    long cosa = COS1024[(int)(rad*1024.0f)&1023];
    X = FIXEDMUL(A.x , cosa ) - FIXEDMUL(A.y , sina);
    Y = FIXEDMUL(A.x , sina ) + FIXEDMUL(A.y , cosa);
    return VecCreateL(X, Y, A.z);
}