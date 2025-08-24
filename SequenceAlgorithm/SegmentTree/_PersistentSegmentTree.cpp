#include<bits/stdc++.h>
template <class S, S (*op)(S, S), S (*e)()>
struct PersistentSegmentTree{
    public:
        PersistentSegmentTree() : PersistentSegmentTree(0){}
        PersistentSegmentTree(int N) : PersistentSegmentTree(std::vector<S>(N,e())){}
        PersistentSegmentTree(const std::vector<S>& v) : n((int)v.size()),_v(v){
            init();
        }

        S get(int p){
            return get(roots.size()-1,p);
        }
        
        S get(int t,int p){
            assert(0<=t&&t<roots.size()&&0<=p&&p<n);
            return get(roots[t],0,n,p);
        }

        void set(int p,S x){
            set(roots.size()-1,p,x);
        }

        void set(int t,int p,S x){
            assert(0<=t&&t<roots.size()&&0<=p&&p<n);
            _set(t,p,x);
        }

        S prod(int l,int r){
            return prod(roots.size()-1,l,r);
        }

        S prod(int t,int l,int r){
            assert(0<=t&&t<roots.size()&&0<=l&&l<r&&r<=n);
            return prod(roots[t],0,n,l,r);
        }

        S all_prod(int t){
            assert(0<=t&&t<roots.size());
            return roots[t]->value;
        }

    private:
        struct node;
        using np = node*;

        struct node{
            S value;
            np left;
            np right;
            node() : value(e()),left(nullptr),right(nullptr){}
            node(S value,np left,np right) : value(value),left(left),right(right){}
            void update(){
                value = op(left->value,right->value);
            }
        };

        void init(){
            np root = new node();
            roots.push_back(root);
            init(root,0,n);
        }

        void init(np& nd,int a,int b){
            if(b-a==1){
                nd->value = _v[a];
                return;
            }
            int c = (a+b)>>1;
            nd->left = new node();
            nd->right = new node();
            init(nd->left,a,c);
            init(nd->right,c,b);
            nd->update();
        }

        S get(np& nd,int a,int b,int p){
            if(b-a==1) return nd->value;
            int c = (a+b)>>1;
            if(p<c) return get(nd->left,a,c,p);
            else return get(nd->right,c,b,p);
        }

        void _set(int t,int p,S x){
            np nd = new node();
            roots.push_back(nd);
            set(roots[t],roots[roots.size()-1],0,n,p,x);
        }

        void set(np& old_nd,np& new_nd,int a,int b,int p,S x){
            if(b-a==1){
                new_nd = new node();
                new_nd->value = x;
                return;
            }
            int c = (a+b)>>1;
            if(p<c){
                new_nd = new node(e(),nullptr,old_nd->right);
                set(old_nd->left,new_nd->left,a,c,p,x);
            }
            else{
                new_nd = new node(e(),old_nd->left,nullptr);
                set(old_nd->right,new_nd->right,c,b,p,x);
            }
            new_nd->update();
        }

        S prod(np& nd,int a,int b,int l,int r){
            if(r<=a||b<=l) return e();
            if(l<=a&&b<=r) return nd->value;
            int c = (a+b)>>1;
            return op(prod(nd->left,a,c,l,r),prod(nd->right,c,b,l,r));
        }

        int n;
        std::vector<S> _v;
        std::vector<np> roots;
};
