#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//RB 컬러
typedef enum {RED, BLACK} color;

//RB트리 노드
typedef struct rbnode
{
    int key;
    struct rbnode *right,*left,*parent;
    color clr;
}rbnode;

//root와 NIL 주소를 포함한 트리 구조체
typedef struct rbtree{
    rbnode* root;
    rbnode* NIL;
}rbtree;

//레벨 오더 출력을 위한 큐
struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

//큐 노드
struct QueueNode {
    struct rbnode* node;
    struct QueueNode* next;
};

//큐 생성함수
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

//빈 큐 확인
int isEmpty(struct Queue* queue) {
    return queue->front == NULL;
}

//큐 삽입 함수
void enqueue(struct Queue* queue, struct rbnode* node) {
    struct QueueNode* newQueueNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    newQueueNode->node = node;
    newQueueNode->next = NULL;
    if (queue->rear == NULL) {
        queue->front = newQueueNode;
        queue->rear = newQueueNode;
    } else {
        queue->rear->next = newQueueNode;
        queue->rear = newQueueNode;
    }
}

//큐 데이터 추출
struct rbnode* dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        return NULL;
    }
    struct QueueNode* temp = queue->front;
    rbnode* node = temp->node;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return node;
}

//레벨오더 트리 출력
void printLevelOrder(struct rbtree *myTree,FILE *ptr_output) {
    if (myTree->root == NULL) {
        return;
    }

    struct Queue* queue = createQueue();
    enqueue(queue, myTree->root);

    while (!isEmpty(queue)) {
        struct rbnode* currentNode = dequeue(queue);
        fprintf(ptr_output,"%d",currentNode->key);
        if (currentNode->left != myTree->NIL) {
            enqueue(queue, currentNode->left);
        }

        if (currentNode->right != myTree->NIL) {
            enqueue(queue, currentNode->right);
        }
        //마지막에 공백문자 들어가는 것 방지
        if(!isEmpty(queue))
            fprintf(ptr_output," ");
    }
}

//RB트리 노드 생성
rbnode* newNode(int key , rbtree* tree){
    rbnode *node = (rbnode*)malloc(sizeof(rbnode));
    node->key = key;
    node->left = node->right = node->parent = tree->NIL;
    node->clr = RED; //새로운 노드 컬러는 항상 RED
    return node;
}

//NIL 노드 생성
rbnode* createNIL(){
    rbnode *node = (rbnode*)malloc(sizeof(rbnode));
    node->key = 99999; //값은 임의로 99999 지정
    node->left = NULL;
    node->right = NULL;
    node->clr = BLACK;
    return node;
}

//트리 생성
rbtree* createRbtree(){
    rbtree* tree = (rbtree*)malloc(sizeof(rbtree));
    tree->NIL = createNIL();
    tree->root = tree->NIL;
    return tree;
}

//트리 최소값을 가지는 노드 검색
rbnode* findMin(rbnode* root, rbtree *tree){
    if (root==tree->NIL)
        return tree->NIL;
    else if (root->left!=tree->NIL)
        return findMin(root->left,tree);
    return root;
}

//트리 최대값을 가지는 노드 검색
rbnode* findMax(rbnode* root,rbtree *tree){
    if (root==tree->NIL)
        return tree->NIL;
    else if (root->right!=tree->NIL)
        return findMax(root->right,tree);
    else//(root->right==tree->NIL)
        return root;
}

rbnode* rightRotate(rbnode *z, rbtree *tree){
    rbnode *y = z->left;
    rbnode *T3 = y->right;
    rbnode *p = z->parent;
    //ROOT의 부모 노드를 NULL로 해서, ROOT를 제외하고 부모노드와 연결
    if(p!=NULL){
        if(p->right==z)
            p->right=y;
        else
            p->left=y;
    }
    else{
        tree->root=y;
    }
    

    z->left = T3;
    //NIL인 경우를 제외하고 부모와 연결
    if(T3!=tree->NIL)
        T3->parent = z;
    y->right = z;
    z->parent = y;
    y->parent = p;

    return y;

}

rbnode* leftRotate(rbnode *z, rbtree *tree){
    rbnode *y = z->right;
    rbnode *T2 = y->left;
    rbnode *p = z->parent;
    //ROOT의 부모 노드를 NULL로 해서, ROOT를 제외하고 부모노드와 연결
    if(p!=NULL){
        if(p->right==z)
            p->right=y;
        else
            p->left=y;
    }
    else{
        tree->root=y;
    }
    

    z->right = T2;
    //NIL인 경우를 제외하고 부모와 연결
    if(T2!=tree->NIL)
        T2->parent = z;
    y->left = z;
    z->parent = y;
    y->parent = p;

    return y;
}

void insertFixup(rbtree* tree, rbnode* root){
    if(root->parent!=NULL){
        if(root->parent->clr == RED){
            rbnode* uncle;
            if(root->parent->parent->left == root->parent)
                uncle = root->parent->parent->right;
            else
                uncle = root->parent->parent->left;
            if(uncle->clr == RED){
                root->parent->clr=uncle->clr=BLACK;
                root->parent->parent->clr=RED;
                insertFixup(tree,root->parent->parent);
            }
            else{
                if(root->parent->key > root->key){
                    //OL
                    if(root->parent->key < root->parent->parent->key){
                        //LL
                        root=root->parent->parent;
                        root=rightRotate(root,tree);
                        root->clr=BLACK;
                        root->right->clr=RED;
                    }
                    else{
                        //RL
                        root=root->parent;
                        root=rightRotate(root,tree);
                        root=root->parent;
                        root=leftRotate(root,tree);
                        root->clr=BLACK;
                        root->left->clr=RED;
                    }
                }
                else{
                    //OR
                    if(root->parent->key < root->parent->parent->key){
                        //LR
                        root=root->parent;
                        root=rightRotate(root,tree);
                        root=root->parent;
                        root=leftRotate(root,tree);
                        root->clr=BLACK;
                        root->right->clr=RED;
                    }
                    else{
                        //RR
                        root=root->parent->parent;
                        root=leftRotate(root,tree);
                        root->clr=BLACK;
                        root->left->clr=RED;
                    }
                }
            }
            
        }
    }
    
}

void insertNode(rbtree* myTree, rbnode* node){
    //빈 트리면 root 노드로 추가, 부모는 NULL로 설정
    if (myTree->root == myTree->NIL){
        myTree->root=node;
        myTree->root->parent=NULL;
    }
    else{
        rbnode* thisNode=myTree->root;
        while(thisNode!=myTree->NIL){
            if(node->key < thisNode->key){
                if(thisNode->left==myTree->NIL){
                    thisNode->left=node;
                    node->parent=thisNode;
                    break;
                }
                thisNode=thisNode->left;
            }
            else if(node->key > thisNode->key){
                if(thisNode->right==myTree->NIL){
                    thisNode->right=node;
                    node->parent=thisNode;
                    break;
                }
                thisNode=thisNode->right;
            }
            //중복 방지
            else
                break;
        }
        insertFixup(myTree,node);
    }
    
}

void changeKey(rbnode *nodeA,rbnode *nodeB){
    //A가 상위노드(thisNode), B가 하위노드(temp)
    int temp = nodeA->key;
    nodeA->key = nodeB->key;
    nodeB->key = temp;
}

void deleteFixup(rbtree* myTree, rbnode *C,rbnode *P,rbnode *S, color deletedColor){
    if(deletedColor!=RED){
        //if dClr==red: do nothing
        if(C->clr==RED)
            C->clr=BLACK;
        else{
            //C==BLACK
            if(S->clr==RED){
                //2-1; S : RED
                if(S->parent->right==S){
                    S=S->parent;
                    S=leftRotate(S,myTree);
                    S->clr=BLACK;
                    S->left->clr=RED;
                }
                else{
                    S=S->parent;
                    S=rightRotate(S,myTree);
                    S->clr=BLACK;
                    S->right->clr=RED;
                }
            }
            else{
                //Sibling : Black
                //2-3
                if(S->parent->right==S&&S->right->clr==RED){
                    //S is rC
                    S->clr=P->clr;
                    P->clr=BLACK;
                    S->right->clr=BLACK;
                    P=leftRotate(P,myTree);
                }
                else if(S->parent->left==S&&S->left->clr==RED){
                    //S is lC
                    P->clr=BLACK;
                    S->left->clr=BLACK;
                    P=rightRotate(P,myTree);
                }
                else if(S->parent->right==S&&S->left->clr==RED&&S->right->clr==BLACK){
                    //2-4;R Case
                    if(S==P->right){
                        S=rightRotate(S,myTree);
                        S->clr=BLACK;
                        S->right->clr=RED;
                        S=S->parent;
                        S=leftRotate(S,myTree);
                        if(S->left->clr==BLACK)
                            S->right->clr=BLACK;
                    }
                }
                else if(S->parent->left==S&&S->right->clr==RED&&S->left->clr==BLACK){
                    //2-4;L Case
                    S=leftRotate(S,myTree);
                    S->clr=BLACK;
                    S->left->clr=RED;
                    S=S->parent;
                    S=rightRotate(S,myTree);
                    if(S->right->clr==BLACK)
                        S->left->clr=BLACK;
                }
                else if(S->left->clr==BLACK&&S->right->clr==BLACK){
                    //2-2
                    S->clr=RED;
                    P->clr=BLACK;
                }
            }
        }
    }
}

void delete(rbtree* myTree, int x){
    rbnode *thisNode=myTree->root;
    rbnode *C, *P, *S;

    color deletedColor;
    while(thisNode!=myTree->NIL){
        if(thisNode->key > x){
            thisNode = thisNode->left;
        }
        else if(thisNode->key < x){
            thisNode = thisNode->right;
        }
        else{
            if(thisNode->left!=myTree->NIL&&thisNode->right!=myTree->NIL){
                //2 Child
                rbnode *temp = findMax(thisNode->left,myTree);
                changeKey(thisNode,temp);
                if(temp->parent==thisNode){
                    //thisNode와 temp가 부자지간
                    P=thisNode;
                    if(thisNode->left==temp)
                        S=thisNode->right;
                    else
                        S=thisNode->left;
                    if(temp->left==myTree->NIL){
                        thisNode->left=myTree->NIL;
                        C=myTree->NIL;
                        deletedColor=temp->clr;
                        free(temp);
                        deleteFixup(myTree,C,P,S,deletedColor);
                        break;
                    }
                    else{
                        thisNode->left=temp->left;
                        temp->left->parent=thisNode;
                        C=temp->left;
                        deletedColor=temp->clr;
                        free(temp);
                        deleteFixup(myTree,C,P,S,deletedColor);
                        break;
                    }
                }
                else{
                    //thisNode와 temp가 부자지간이 아님
                    if(temp->left==myTree->NIL){
                        temp->parent->right=myTree->NIL;
                        P=temp->parent;
                        C=myTree->NIL;
                        S=temp->parent->left;
                        deletedColor=temp->clr;
                        free(temp);
                        deleteFixup(myTree,C,P,S,deletedColor);
                        break;
                    }
                    else{
                        temp->parent->right=temp->left;
                        temp->left->parent=temp->parent;
                        P=temp->parent;
                        C=myTree->NIL;
                        S=temp->parent->right;
                        deletedColor=temp->clr;
                        free(temp);
                        deleteFixup(myTree,C,P,S,deletedColor);
                        break;
                    }
                }
            }
            else if(thisNode->left==myTree->NIL&&thisNode->right!=myTree->NIL){
                //R 1Child
                if(thisNode->parent->left==thisNode){
                    thisNode->parent->left=thisNode->right;
                    thisNode->right->parent=thisNode->parent;
                    C=thisNode->right;
                    P=thisNode->parent;
                    S=thisNode->parent->right;
                    deletedColor=thisNode->clr;
                    free(thisNode);
                    deleteFixup(myTree,C,P,S,deletedColor);
                    break;
                }
                else{
                    thisNode->parent->right=thisNode->right;
                    thisNode->right->parent=thisNode->parent;
                    C=thisNode->right;
                    P=thisNode->parent;
                    S=thisNode->parent->left;
                    deletedColor=thisNode->clr;
                    free(thisNode);
                    deleteFixup(myTree,C,P,S,deletedColor);
                    break;
                }
            }
            else if(thisNode->right==myTree->NIL&&thisNode->left!=myTree->NIL){
                //L 1Child
                C=thisNode->left;
                P=thisNode->parent;
                if(thisNode->parent->left==thisNode){
                    thisNode->parent->left=thisNode->left;
                    thisNode->left->parent=thisNode->parent;
                    S=thisNode->parent->right;
                    deletedColor=thisNode->clr;
                    free(thisNode);
                    deleteFixup(myTree,C,P,S,deletedColor);
                    break;
                }
                else{
                    thisNode->parent->right=thisNode->left;
                    thisNode->left->parent=thisNode->parent;
                    S=thisNode->parent->left;
                    deletedColor=thisNode->clr;
                    free(thisNode);
                    deleteFixup(myTree,C,P,S,deletedColor);
                    break;
                }
            }
            else{
                //0 Child
                if(thisNode->parent->left==thisNode){
                    thisNode->parent->left=myTree->NIL;
                    C=myTree->NIL;
                    P=thisNode->parent;
                    S=thisNode->parent->right;
                    deletedColor=thisNode->clr;
                    free(thisNode);
                    deleteFixup(myTree,C,P,S,deletedColor);
                    break;
                }
                else{
                    thisNode->parent->right=myTree->NIL;
                    C=myTree->NIL;
                    P=thisNode->parent;
                    S=thisNode->parent->left;
                    deletedColor=thisNode->clr;
                    free(thisNode);
                    deleteFixup(myTree,C,P,S,deletedColor);
                    break;
                }
            }
        }
    }
}

//inorder 출력 함수
void printInorder(rbtree *myTree, rbnode *node, int maxKey, FILE *ptr_output){
    if(node->left!=myTree->NIL)
        printInorder(myTree,node->left,maxKey,ptr_output);
    if(node->key!=maxKey)
        fprintf(ptr_output,"%d ",node->key);
    else
        fprintf(ptr_output,"%d",node->key);//마지막엔 공백 없이
    if(node->right!=myTree->NIL)
        printInorder(myTree,node->right,maxKey,ptr_output);
}

void printResult(rbtree *myTree, FILE *ptr_output){
    int maxKey = findMax(myTree->root,myTree)->key;
    printInorder(myTree,myTree->root,maxKey,ptr_output);
    fprintf(ptr_output,"\n");
    printLevelOrder(myTree,ptr_output);
}

int main(int argc, char *argv[]){
    FILE *ptr_input, *ptr_output;
    ptr_input = fopen(argv[1],"r");
    ptr_output = fopen(argv[2],"w");
    if(ptr_input == NULL || ptr_output == NULL){
        printf("Fail to read/create a file !\n");
        exit(1);
    }

    rbtree* myTree = createRbtree();
    rbnode *node;
    char *input_txt=malloc(1000*sizeof(char));
    char *ptr=NULL;
    int input_num;

    //insertion
    fscanf(ptr_input,"%[^\n]s",input_txt);
    //7 3 18 10 22 8 11 26 2 6 13
    ptr=strtok(input_txt," ");

    while(1){
        input_num=atoi(ptr);
        node=newNode(input_num,myTree);
        insertNode(myTree,node);
        if(myTree->root->clr==RED)
            myTree->root->clr=BLACK;

        ptr=strtok(NULL," ");
        if(ptr==NULL)
            break;
    }
    printResult(myTree,ptr_output);
    fprintf(ptr_output,"\n");


    //deletion
    fscanf(ptr_input,"\n%[^\n]s",input_txt);
    //18 11 3 10 22

    ptr=strtok(input_txt," ");
    while(1){
        input_num=atoi(ptr);
        delete(myTree,input_num);
        if(myTree->root->clr==RED)
            myTree->root->clr=BLACK;
        ptr=strtok(NULL," ");
        if(ptr==NULL)
            break;
    }

    printResult(myTree,ptr_output);

    return 0;
}