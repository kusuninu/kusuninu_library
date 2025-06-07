#include<bits/stdc++.h>
template<class T = long long>
struct Imos_2D_Const{
    Imos_2D_Const(int N,int M) : _N(N),_M(M){
        assert(0<_N&&0<_M);
        A = new T*[_N];
        for(int i = 0;i < _N;i++){
            A[i] = new T[_M];
            for(int j = 0;j < _M;j++) A[i][j] = 0;
        }
        B = new T*[_N+1];
        for(int i = 0;i < _N;i++){
            B[i] = new T[_M+1];
            for(int j = 0;j < _M;j++) B[i][j] = 0;
        }
        B[_N] = new T[_M+1];

    }
    Imos_2D_Const(const std::vector<std::vector<T>>& V) : _N(V.size()){
        assert(0<_N);
        _M = V[0].size();
        assert(0<_M);
        A = new T*[_N];
        for(int i = 0;i < _N;i++){
            assert(V[i].size()==_M);
            A[i] = new T[_M];
            for(int j = 0;j < _M;j++) A[i][j] = V[i][j];
        }
        B = new T*[_N+1];
        B[0][0] = A[0][0];
        for(int j = 1;j < _M;j++) B[0][j] = A[0][j] - A[0][j-1];
        for(int i = 1;i < _N;i++){
            B[i] = new T[_M+1];
            B[i][0] = A[i][0] - A[i-1][0];
            for(int j = 1;j < _M;j++) B[i][j] = A[i][j] - A[i][j-1] - A[i-1][j] + A[i-1][j-1];
        }
        B[_N] = new T[_M+1];
    }
    //add f to [l1,r1) * [l2,r2)
    void add(int l1,int r1,int l2,int r2,T f){
        assert(0<=l1&&l1<=r1&&r1<=_N&&0<=l2&&l2<=r2&&r2<=_M);
        B[l1][l2] += f;
        B[l1][r2] -= f;
        B[r1][l2] -= f;
        B[r1][r2] += f;
        initialized = false;
    }
    T get(int p,int q){
        assert(0<=p&&p<_N&&0<=q&&q<_M);
        if(initialized) return A[p][q];
        for(int j = 0;j < _M;j++){
            A[0][j] = B[0][j];
        }
        for(int i = 0;i < _N - 1;i++){
            for(int j = 0;j < _M;j++){
                A[i+1][j] = A[i][j] + B[i+1][j];
            }
        }
        for(int i = 0;i < _N;i++){
            for(int j = 0;j < _M - 1;j++){
                A[i][j+1] = A[i][j] + A[i][j+1];
            }
        }
        initialized = true;
        return A[p][q];
    }

    private:
    T** A;
    T** B;
    int _N;
    int _M;
    bool initialized = true;
};