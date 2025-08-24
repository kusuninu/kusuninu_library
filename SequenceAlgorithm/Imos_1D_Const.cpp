#include<bits/stdc++.h>
template<class T = long long>
struct Imos_1D_Const{
    Imos_1D_Const(int N) : _N(N){
        assert(0<_N);
        A = new T[_N];
        for(int i = 0;i < _N;i++) A[i] = 0;
        B = new T[_N];
        for(int i = 0;i < _N;i++) B[i] = 0;

    }
    Imos_1D_Const(const std::vector<T>& V) : _N(V.size()){
        assert(0<_N);
        A = new T[_N];
        A[0] = V[0];
        for(int i = 0;i < _N - 1;i++) A[i+1] = V[i+1] - V[i];
        B = new T[_N];
        for(int i = 0;i < _N;i++) B[i] = V[i];
    }
    //add f to [l,r)
    void add(int l,int r,T f){
        assert(0<=l&&l<=r&&r<=_N);
        if(l!=_N) A[l] = A[l] + f;
        if(r!=_N) A[r] = A[r] - f;
        initialized = false;
    }
    T get(int p){
        assert(0<=p&&p<_N);
        if(initialized) return B[p];
        B[0] = A[0];
        for(int i = 0;i < _N - 1;i++) B[i+1] = B[i] + A[i+1];
        initialized = true;
        return B[p];
    }

    private:
    T* A;
    T* B;
    int _N;
    bool initialized = true;
};