#include "Structures.h"

static inline bool lessAZ(const std::pair<int,int>& x, const std::pair<int,int>& y){
        if (x.first != y.first){
                return x.first < y.first;
        }
        return x.second < y.second;
}

static inline Particles makeKey(const std::pair<int,int>& a, const std::pair<int,int>& b){
        Particles k;
        if (lessAZ(a,b) || (a==b)){
                k.A = a; k.B = b;
        }
        else{
                k.A = b; k.B = a;
        }
        return k;
}

