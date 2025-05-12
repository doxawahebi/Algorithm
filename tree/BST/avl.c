#include<stdio.h>
#include<stdlib.h>

// rotation을 해도 갖고 있는 서브트리가 달라지지 않으면 height가 달라지지 않는다.
// l : h, r : h-1
// l : h - 1, r : h
// l : 0, r : 0

// 첫 번째 노드는 무조건 루트의 왼쪽이여야 됨.

typedef struct _Node{
    struct _Node* l;
    struct _Node* r;
    struct _Node* par; // Left, Right, par
    int k; // key
    int bf; // balance factor
}Node;

typedef struct _AVLTree{
    Node* root;
}AVLTree;
int avlcmp(int a, int b){
    if (a < b){ return -1; }
    else if (a > b){ return 1; }
    else{ return 0; }
}
Node* find(AVLTree *avl, int key){
    Node* p = avl->root->l;

    while (p != NULL){
        int cmp = avlcmp(key, p->k);
        if (cmp == 0) { return p; }
        p = (cmp < 0) ? p->l : p->r;
    }
    
    return NULL;
}
Node* createNode(int key){
    Node* node = malloc(sizeof(Node));
    node->l = NULL;
    node->r = NULL;
    node->par = NULL;
    node->k = key;
    node->bf = 0;

    return node;
}
Node* _rotateLL(Node* x){
    Node* y = x->l;

    // 
    x->l = y->r;
    if (x->l != NULL){
        x->l->par = x;
    }

    // tree y, tree x swap
    y->par = x->par;
    if (x == x->par->l){
        x->par->l = y;
    } else{
        x->par->r = y;
    }
    
    y->r = x;
    x->par = y;

    return y;
}
Node* _rotateRR(Node* x){
    Node* y = x->r;
    x->r = y->l;
    if (x->r != NULL){
        x->r->par = x;
    }
    
    y->par = x->par;
    if(x->par != NULL){
        if (x == x->par->l){
            x->par->l = y;
        } else {
            x->par->r = y;
        }
    }
    
    y->l = x;
    x->par = y;

    return y;
}
Node* balanceL(Node* par){
    if (par->l->bf == par->bf){
        par = _rotateLL(par);
        par->bf = par->r->bf = 0;
    } else {
        int oldbf = par->l->r->bf;
        _rotateRR(par->l);
        par = _rotateLL(par);
        par->bf = 0;
        if (oldbf == -1){
            par->l->bf = 0;
            par->r->bf = 1;
        } else if (oldbf == 1){
            par->l->bf = -1;
            par->r->bf = 0;
        } else if (oldbf == 0){
            par->l->bf = par->r->bf = 0;
        }
    }
    
    return par;
}
Node* balanceR(Node* par){
    if (par->r->bf == par->bf){
        par = _rotateRR(par);
        par->bf = par->l->bf = 0;
    } else {
        int oldbf = par->r->l->bf;
        _rotateLL(par->r);
        par = _rotateRR(par);
        par->bf = 0;
        if (oldbf == -1){
            par->l->bf = 0;
            par->r->bf = 1;
        } else if (oldbf == 1){
            par->l->bf = -1;
            par->r->bf = 0;
        } else if (oldbf == 0){
            par->l->bf = par->r->bf = 0;
        }
    }
    
    return par;
}
// insert할 곳을 찾을 때 single path로 path가 생긴다. while문으로 쉽게 구현 가능. path에 있는 모든 노드는 영향을 받을 수도 있고 아닐 수도 있다.
// 
// recursive로 구현하는 이유는 bottom-up을 구현하기 편하기 때문.
Node* insert(AVLTree* avl, int key){

    Node* par = avl->root;
    Node* cur = par->l;

    while (cur != NULL){
        int cmp = avlcmp(key, cur->k);

        par = cur;

        if(cmp < 0) { cur = cur->l; }
        else if (cmp > 0) { cur = cur->r; }
        else { return cur; } // do not allow duplicate
    }

    Node* ret;
    ret = cur = createNode(key);
    cur->par = par;
    
    // 첫 번째 노드의 시작은 루트의 left에 존재해야 됨.
    if (par == avl->root || avlcmp(key, par->k) < 0){
        par->l = cur;
    }
    else{
        par->r = cur;
    }

    while (cur != avl->root->l){
        if (cur == par->l){
            if (par->bf == 1){
                par->bf--;
                break;
            } else if (par->bf == 0){
                par->bf--;
            } else if (par->bf == -1){
                balanceL(par);
                break;
            }
        }else{
            if (par->bf == -1){
                par->bf++;
                break;
            } else if (par->bf == 0){
                par->bf++;
            } else if (par->bf == 1){
                balanceR(par);
                break;
            }
        }

        cur = par;
        par = cur->par;
    }
    
    return ret;
}
void initAVLTree(AVLTree* avl){
    avl->root = createNode(0);
}
AVLTree* makeAVLTree(){
    // root에 대한 sentinel node 
    // 이거 안 쓰면 rotation 연산에 의해 root 위치도 변경되는 것을 고려해야 됨.
    // root는 수정 불가능해야됨.
    AVLTree* t = (AVLTree*)malloc(sizeof(AVLTree));
    initAVLTree(t);

    return t;
}

int main(){
    AVLTree* avl = makeAVLTree();

    insert(avl, 1);
    insert(avl, 2);
    insert(avl, 3);
    insert(avl, 4);
    insert(avl, 5);

    Node* p = find(avl, 4);
    printf("%d", avl->root);
}