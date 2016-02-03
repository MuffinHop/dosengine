#include "src\misc\definer.h"
struct Vec VecCreateL( long x_, long y_, long z_) {
    struct Vec r_;
    r_.x = x_;
    r_.y = y_;
    r_.z = z_;
    return r_;
};

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
struct Vec cross( struct Vec A, struct Vec B) {
    return VecCreateL(A.y*B.z-A.z*B.y,A.z*B.x-A.x*B.z,A.x*B.y-A.y*B.x);
}
