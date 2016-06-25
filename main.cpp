#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <conio.h>
#include <malloc.h>
#include <math.h>
#include <vector>
const int BLACK=1;
const int RED=0;
using namespace std;
typedef struct profiles Profile;
typedef struct words Word;
typedef struct node Node;
void insert_case1(Node* n);
void insert_case2(Node* n);
void insert_case3(Node* n);
void insert_case4(Node* n);
void insert_case5(Node* n);
void delete_case11(Node* n);
void delete_case12(Node* n);
void delete_case13(Node* n);
void rotate_left(Node* n);
void rotate_right(Node* n);


int f_max,f_min,t_max,t_min;
int topnum;

typedef struct adj{
    Profile* data;
    int weight; // for shortest path
    string date;
    struct adj* next;
} Adj;

typedef struct adjw{
    Word* data;
    struct adjw* next;
} Adjw;

typedef struct profiles{
    string user;
    string date;
    string name;
    int fnum;
    int tnum;
    int check;
    int foa;
    int d; //used for shortest path
    Adj* friends;
    Adj* friended;
    Adjw* tweets;
    struct profiles* from; //used for shortest path
} Profile; //Profile of user

typedef struct words{
    string word;
    int tweeted;
    Adj* tweeters;
} Word; //Information of word

//----------------- Black-Red tree

typedef struct node{
    int color;  //Red : 0 Black : 1void insert_p(string user, string date, string name){
    Profile* P;
    Word* W;
    struct node* parent;
    struct node* left;
    struct node* right;
}Node;

Node* Prof=NULL; // root of Profile
Node* Wo=NULL; //root of Word
Adj* FOA=NULL; //Friends of All people found in question 3

void find_root(){
    while(Prof!=NULL && Prof->parent!=NULL)
        Prof=Prof->parent;
    while(Wo!=NULL && Wo->parent!=NULL)
        Wo=Wo->parent;
}

Node *grandparent(Node *n)
{
if ((n != NULL) && (n->parent != NULL))
    return n->parent->parent;
else
    return NULL;
}

Node *uncle(Node *n)
{
    Node *g = grandparent(n);
    if (g == NULL)
        return NULL; // No grandparent means no uncle
    if (n->parent == g->left)
        return g->right;
    else
        return g->left;
}

void insert_case1(Node *n)
{
    if(n->parent==NULL)
        n->color=BLACK;
    else
        insert_case2(n);
}

void insert_case2(Node *n)
{
    if(n->parent->color==BLACK)
        return;
    else
        insert_case3(n);
}

void insert_case3(Node *n){
    Node *u=uncle(n);
    Node *g;

    if(u!=NULL && u->color==RED){
        n->parent->color=BLACK;
        u->color = BLACK;
        g=grandparent(n);
        g->color=RED;
        insert_case1(g);
    }
    else
        insert_case4(n);
}

void insert_case4(Node *n){
    Node *g = grandparent(n);

    if(n==n->parent->right && n->parent==g->left){
        rotate_left(n->parent);
        n=n->left;
    }
    else if(n==n->parent->left && n->parent==g->right){
        rotate_right(n->parent);
        n=n->right;
    }
    insert_case5(n);
}

void rotate_left(Node *n){
    Node *c = n->right;
    Node *p = n->parent;

    if(c->left!=NULL)
        c->left->parent=n;

    n->right = c->left;
    n->parent = c;
    c->left = n;
    c->parent = p;

    if(p!=NULL){
        if(p->left==n)
            p->left=c;
        else
            p->right=c;
    }
}

void rotate_right(Node *n){
    Node *c = n->left;
    Node *p = n->parent;

    if(c->right!=NULL)
        c->right->parent=n;

    n->left=c->right;
    n->parent=c;
    c->right=n;
    c->parent=p;

    if(p!=NULL){
        if(p->right==n)
            p->right=c;
        else
            p->left=c;
    }
}

void insert_case5(Node *n){
    Node *g = grandparent(n);

    n->parent->color=BLACK;
    g->color=RED;
    if(n==n->parent->left)
        rotate_right(g);
    else
        rotate_left(g);
    find_root();
}

//  delete node

Node* sibling(Node* n){
    if(n->parent!=NULL)
        if(n==n->parent->left)
            return n->parent->right;
        else
            return n->parent->left;
    return NULL;
}

void delete_one(Node* n){
    Node* N=NULL;

    if(n->right!=NULL)
        for(N=n->right;N->left!=NULL;N=N->left);
    else if(n->left!=NULL)
        for(N=n->left;N->right!=NULL;N=N->right);

    if(N!=NULL){
        n->P=N->P;
        n->W=N->W;
        n=N;
    }

    if(n->left!=NULL || n->right!=NULL){ //only 1 child -> n=BLACK
        Node* child=(n->right==NULL)? n->left : n->right;
        child->parent = n->parent;
        if(n==n->parent->left)
            n->parent->left=child;
        else
            n->parent->right=child; //replacing child into N

        child->color = BLACK; // child was red.... their sibling do not exist
    }

    else if(n->color==BLACK)
        delete_case11(n); // N is doubly blacked

    if(n->parent->left==n)
        n->parent->left=NULL;
    else if(n->parent->right==n)
        n->parent->right=NULL;

    delete n;
    n=NULL;

    find_root();
}

int node_color(Node* n){
    return n==NULL? BLACK : n->color;
}

void delete_case11(Node* n){
    Node* s=sibling(n);

    if(s->color==RED)
        delete_case13(n); // if node's sibling is RED

    else if(node_color(s->left)==RED || node_color(s->right)==RED){
        if(n==n->parent->left){
            if(node_color(s->right)==RED){
                s->right->color=BLACK;
                rotate_left(n->parent);
            }
            else{
                s->left->color=BLACK;
                rotate_right(s);
                rotate_left(n->parent);
            }
        }
        else{
            if(node_color(s->left)==RED){
                s->left->color=BLACK;
                rotate_right(n->parent);
            }
            else{
                s->right->color=BLACK;
                rotate_left(s);
                rotate_right(n->parent);
            }
        }
    } // if node have red child

    else
        delete_case12(n);
}

void delete_case12(Node* n){
    Node* s=sibling(n);

    s->color=RED;
    if(n->parent->color==RED)
        n->parent->color=BLACK;
    else if(grandparent(n)!=NULL)
        delete_case11(n->parent);
}

void delete_case13(Node* n){
    Node* s=sibling(n);
    s->color=BLACK;
    n->parent->color=RED;
    if(n==n->parent->left)
        rotate_left(n->parent);
    else
        rotate_right(n->parent);
    delete_case11(n);
}

// inserting Profile's node

void insert_p(string user, string date, string name){
    int r=0;
    Node* no;
    Node* cu;

    no=new Node;
    no->color=RED;
    no->W=NULL;

    no->P=new Profile;
    no->P->check=0;
    no->P->user=user;
    no->P->date=date;
    no->P->name=name;

    no->P->fnum=0;
    no->P->tnum=0;
    no->P->d=-1;

    no->P->friends=NULL;

    no->P->friended=NULL;

    no->P->tweets=NULL;

    no->P->from=NULL;

    no->parent=NULL;
    no->right=NULL;
    no->left=NULL;

    if(Prof==NULL)
        Prof=no;
    else{
        for(cu=Prof;cu!=NULL;){
            no->parent=cu;
            if(cu->P->user.compare(user)>=0)
                r=0,cu=cu->left;
            else
                r=1,cu=cu->right;
        }

        if(r==1)
            no->parent->right=no;
        else
            no->parent->left=no;
    }
    insert_case1(no);
}

// deleting Profiles from Prof

void delete_p(Node* O){
    if(O==NULL) return;
    Adj* a;
    Adj* b;
    Adj* prev;

    Adjw* c;

    Profile* p;

    for(a=O->P->friended ; a!=NULL ; ){
        prev=NULL;
        for(b=a->data->friends ; ; prev=b,b=b->next)
            if(b->data==O->P) break;

        if(prev==NULL)
            a->data->friends=b->next;
        else
            prev->next=b->next;

        delete b;

        prev=a;
        a=a->next;
        delete prev;
    }

    for(a=O->P->friends ; a!=NULL ; ){
        prev=NULL;
        for(b=a->data->friended ; ; prev=b,b=b->next)
            if(b->data==O->P) break;

        if(prev==NULL)
            a->data->friended=b->next;
        else
            prev->next=b->next;

        delete b;
        a->data->fnum--;

        prev=a;
        a=a->next;
        delete prev;
    } // delete friend

    Adjw* prevw;
    for(c=O->P->tweets ; c!=NULL ; ){
        prev=NULL;
        for(b=c->data->tweeters ; ; prev=b, b=b->next )
            if(b->data==O->P) break;

        if(prev==NULL)
            c->data->tweeters=b->next;
        else
            prev->next=b->next;

        prev=b;
        b=b->next;
        delete prev;

        c->data->tweeted--;


        prevw=c;
        c=c->next;
        delete prevw;
    } // delete tweet

    prev=NULL;
    for(a=FOA;a!=NULL;prev=a,a=a->next)
        if(FOA->data==O->P) break;

    if(a!=NULL){
        if(prev==NULL)
            FOA->next=a->next;
        else
            prev->next=a->next;
        delete a;
    } // delete profile from FOA

    delete O->P;
    O->P=NULL;

    if(Prof->right==NULL && Prof->left==NULL){
        delete Prof;
        Prof=NULL;
    }
    else
        delete_one(O);
}   //delete profile from Prof

void search_size(Node* a, int* size, int* fnum, int* tnum){
    if(a==NULL) return;
    *size=*size+1;
    *fnum+=a->P->fnum;
    *tnum+=a->P->tnum;
    if(f_max<a->P->fnum) f_max=a->P->fnum;
    if(f_min>a->P->fnum) f_min=a->P->fnum;

    if(t_max<a->P->tnum) t_max=a->P->tnum;
    if(t_min>a->P->tnum) t_min=a->P->tnum;

    if(a->right!=NULL)
        search_size(a->right, size, fnum, tnum);
    if(a->left!=NULL)
        search_size(a->left, size, fnum, tnum);
} //searching size

Node* search_p_s(Node* a, string u){
    if(a==NULL) return NULL;

    if(a->P->user.compare(u)==0)
        return a;

    if(a->P->user.compare(u)>0)
        return search_p_s(a->left, u);

    return search_p_s(a->right, u);
} //searching string

void insert_friend(string u1, string u2){
    Profile* a;
    Profile* b;
    Adj* k;
    a=search_p_s(Prof, u1)->P;
    b=search_p_s(Prof, u2)->P;
    if(a!=NULL && b!=NULL){
        Adj* c=new Adj;
        c->data=b;
        c->next=NULL;
        if(a->friends==NULL)
            a->friends=c;
        else{
            for(k=a->friends;k!=NULL;k=k->next)
                if(k->data==c->data) break;
            if(k!=NULL){
                delete c;
                return;
            }
            c->next=a->friends;
            a->friends=c;
        }

        Adj* d=new Adj;
        d->data=a;
        d->next=b->friended;
        b->friended=d;

        b->fnum++;
    }
} //u1 has friend, u2

//searching Wo

Node* search_w_s(Node* a, string u){
    if(a==NULL) return NULL;

    if(a->W->word.compare(u)==0)
        return a;

    if(a->W->word.compare(u)<0){
        if(a->right==NULL) return a;
        return search_w_s(a->right, u);
    }

    if(a->left==NULL) return a;
    return search_w_s(a->left, u);
}

//----------------- Inserting Word into Wo

void insert_w(string user, string date, string word){
    int r=0;
    Node* w=search_w_s(Wo,word);
    Node* cu=w;
    if(w==NULL || w->W->word.compare(word)!=0){
        Node* no=new Node;
        no->W=new Word;
        no->P=NULL;
        no->color=RED;

        no->W->tweeted=0;
        no->W->word=word;
        no->W->tweeters=NULL;

        no->left=NULL;
        no->right=NULL;
        no->parent=NULL;

        w=no;
        r=1;
    }

    w->W->tweeted++;

    Adj* A=new Adj;
    A->data=search_p_s(Prof,user)->P;
    A->date=date;
    A->next=w->W->tweeters;
    w->W->tweeters=A;

    Adjw* B=new Adjw;
    B->data=w->W;
    B->next=A->data->tweets;
    A->data->tweets=B;
    A->data->tnum++;

    if(r==1){
        if(Wo==NULL) Wo=w;
        else{
            if(word.compare(cu->W->word)>=0)
                cu->right=w;
            else
                cu->left=w;
            w->parent=cu;
        }
        insert_case1(w);
    }
}

//deleting word from Wo
void delete_w(Node* O){
    if(O==NULL) return;
    Adj* prev;
    Adj* a;
    Adjw* wprev;
    Adjw* b;
    for(a=O->W->tweeters;a!=NULL;){
        wprev=NULL;
        for(b=a->data->tweets; ;wprev=b,b=b->next)
            if(b->data==O->W) break;

        if(wprev==NULL)
            a->data->tweets=b->next;
        else
            wprev->next=b->next;

        delete b;
        a->data->tnum--;

        prev=a;
        a=a->next;
        delete prev;
    }

    delete O->W;
    O->W=NULL;

    if(Wo->left==NULL && Wo->right==NULL){
        delete Wo; Wo=NULL;
    }
    else
        delete_one(O);
} // deleting word from Wo

Adj* S_P=NULL; // shortest path

// for shortest path coding
int hsize;
int curr;

typedef struct heap{
    int weight;
    Profile* from;
    Profile* there;
} Hnode;

vector <Hnode*> M_HEAP; // HEAP array

int hp(int c){
    if(c==0) return 0;
    return (c-1)/2;
}

int hleft(int c){
    return 2*c+1;
}

int hright(int c){
    return 2*c+2;
}

void m_swap(int a, int b){
    Hnode* sw;
    sw=M_HEAP[a];
    M_HEAP[a]=M_HEAP[b];
    M_HEAP[b]=sw;
}

void hinsert(int val, Profile* P, Profile* from){
    curr++;
    if(M_HEAP.size()==0) M_HEAP.reserve(hsize);
    else if(M_HEAP.capacity() <=curr){
        hsize*=2;
        M_HEAP.reserve(hsize);
    } // realloc if capacity not enough

    Hnode* h=new Hnode;
    h->from=from;
    h->there=P;
    h->weight=val;

    M_HEAP.push_back(h);

    val=curr-1;
    while(M_HEAP[hp(val)]->weight > M_HEAP[val]->weight)
        m_swap(hp(val),val), val=hp(val);
}

void hdelete_top(int F){
    if(curr==0) return;
    curr--;
    m_swap(0,curr);

    delete M_HEAP[curr];
    M_HEAP.pop_back();

    int val=0;
    while(F)
        if(hleft(val) < curr && M_HEAP[val]->weight > M_HEAP[hleft(val)]->weight)
            m_swap(val, hleft(val)), val=hleft(val);
        else if(hright(val) < curr && M_HEAP[val]->weight > M_HEAP[hright(val)]->weight)
            m_swap(val, hright(val)), val=hright(val);
        else
            break;
}

void SHP(Profile* P, int num){
    if(P==NULL) return;

    if(curr>0){
        cout << endl;
        cout << P->name << " : " << num << endl;
        Adj* B=new Adj;
        B->data=P;
        B->next=NULL;
        B->weight=M_HEAP[0]->weight;

        int i;

        if(S_P==NULL){
            P->from=M_HEAP[0]->from;
            S_P=B;
            P->d=num;
        }
        else{
            Adj* A;
            for(A=S_P, i=1; A->next!=NULL ;i++, A=A->next);

            if(i==5)
                delete B;
            
            else
                A->next=B;
            P->from=M_HEAP[0]->from;
            P->d=num;
            
        }
    }

    hdelete_top(1);

    for(Adj* A=P->friends; A!=NULL; A=A->next)
        if(A->data->d==-1)
            hinsert(num+A->data->fnum, A->data, P); // shortest path inserting into Min-heap

    for( ; curr>0 && M_HEAP[0]->there->d !=-1 ; hdelete_top(1));

    if(curr==0) return;
    SHP(M_HEAP[0]->there, M_HEAP[0]->weight);
}

// path vertex for SCC
typedef struct pathVertex{
    Adj* link;
    int value;
    struct pathVertex* next;
} Path;

Adj* scc_c=NULL; // SCC vertex
Path* SCC=NULL; // SCC list

// for SCC coding
void SCC_check(Profile* P){
    if(P==NULL) return;
    P->check=1;

    for(Adj* A=P->friends ; A!=NULL ; A=A->next)
        if(A->data->check==0)
            SCC_check(A->data);

    Adj* k= new Adj;
    k->data=P;
    k->next=scc_c;
    scc_c=k;
}  // Giving number to nodes

void SCC_chk(Node* O){
    if(O==NULL) return;
    if(O->P->check==0)
        SCC_check(O->P);
    SCC_chk(O->right);
    SCC_chk(O->left);
} // for All nodes

void SCC_free(Profile* P, Path* pa){
    if(P==NULL) return;
    P->check=0;

    Adj* L=new Adj;
    L->data = P;
    L->next = pa->link;
    pa->link = L;
    pa->value++;

    for(Adj* A=P->friended ; A!=NULL ; A=A->next)
        if(A->data->check==1)
            SCC_free(A->data, pa);
} // making SCC

void SCC_K(){
    SCC_chk(Prof);

    for(Adj* A=scc_c; A!=NULL ;A=A->next){
        if(A->data->check==1){
            Path* pa=new Path;
            pa->link=NULL;
            pa->next=NULL;
            pa->value=0;
            SCC_free(A->data,pa);
            Path* th;

            if(SCC==NULL)
                SCC=pa, topnum--;

            else if(topnum>0){
                if(SCC->value <= pa->value){
                    pa->next=SCC;
                    SCC=pa;
                }
                else{
                    for(th=SCC;th->next!=NULL;th=th->next)
                        if(th->next->value <= pa->value) break;
                    pa->next=th->next;
                    th->next=pa;
                }
                topnum--;
            }

            else{
                for(th=SCC;th!=NULL;th=th->next)
                    if(th->value <= pa->value) break;

                if(th==NULL){
                    for(Adj* K=th->link;K!=NULL;K=th->link){
                        th->link=th->link->next;
                        delete K;
                    }
                    delete pa;
                } // delete if link is not in top5

                else{
                    Adj* data1=pa->link;
                    Adj* data2;
                    int value1=pa->value;
                    int value2;
                    for(;th!=NULL;th=th->next){
                        data2=th->link; value2=th->value;
                        th->link=data1; th->value=value1;
                        data1=data2;    value1=value2;
                    }
                }

            }
        }
    }
} //for all nodes in scc_c, extract top 5 SCC.



// for top5 users, words
Adjw* top5_w=NULL;
Adj* top5_p=NULL;
void top5_word(Node* O){
    if(O==NULL) return;

    if(O->W->tweeted==0)
        delete_w(O); // except tweeted 0.
    else if(topnum>0){
        Adjw* A=new Adjw;
        A->data=O->W;
        A->next=NULL;
        if(top5_w==NULL)
            top5_w=A;
        else{
            if(top5_w->data->tweeted < O->W->tweeted)
                A->next=top5_w,top5_w=A;
            else{
                Adjw* B;
                for(B=top5_w;B->next!=NULL;B=B->next)
                    if(B->next->data->tweeted < O->W->tweeted)
                        break;
                A->next=B->next;
                B->next=A;
            }
        }
        topnum--;
    } // if less than 5
    else{
        Adjw* B;
        for(B=top5_w;B!=NULL;B=B->next)
            if(B->data->tweeted < O->W->tweeted)
                break;
        if(B!=NULL){
            Word* data1=O->W;
            Word* data2;
            for(;B!=NULL;B=B->next){
                data2=B->data;
                B->data=data1;
                data1=data2;
            }
        }
    } //if more than 5
    top5_word(O->right);
    top5_word(O->left);
}

void top5_profile(Node* O){
    if(O==NULL) return;
    if(topnum>0){
        Adj* A=new Adj;
        A->data=O->P;
        A->next=NULL;
        if(top5_p==NULL)
            top5_p=A;
        else{
            if(top5_p->data->tnum < O->P->tnum)
                A->next=top5_p,top5_p=A;
            else{
                Adj* B;
                for(B=top5_p;B->next!=NULL;B=B->next)
                    if(B->next->data->tnum < O->P->tnum)
                        break;
                A->next=B->next;
                B->next=A;
            }
        }
        topnum--;
    }
    else{
        Adj* B;
        for(B=top5_p;B!=NULL;B=B->next)
            if(B->data->tnum < O->P->tnum)
                break;
        if(B!=NULL){
            Profile* data1=O->P;
            Profile* data2;
            for(;B!=NULL;B=B->next){
                data2=B->data;
                B->data=data1;
                data1=data2;
            }
        }
    }
    top5_profile(O->right);
    top5_profile(O->left);
}

//free foa
void free_foa(Node* O){
    if(O==NULL) return;
    O->P->foa=0;
    free_foa(O->right);
    free_foa(O->left);
}

void free_d(Node* O){
    if(O==NULL) return;
    O->P->d=-1;
    free_d(O->left);
    free_d(O->right);
}

bool is_digits(const string &str){
    return str.find_first_not_of("0123456789") == string::npos;
} // if string is int or not

int stoi(string str){
    int m=0;
    for(int a=0;a<str.size();a++)
        m=m*10 + str[a]-'0';
    return m;
} // string to int

int main(){
    string id;
    string date;
    string name;
    ifstream input;
    Node* p;
    Node* q;
    Adj* fusers=NULL;
    int num;

    input.open("user.txt");
    if (input.is_open())
        while (getline(input, id))
            if(id.size()!=0){
                getline(input, date);
                getline(input, name);
                insert_p(id,date,name);
            }
    input.close(); // reading user profile and saving in Prof...
    input.open("word.txt");
    if (input.is_open())
        while (getline(input, id))
            if(id.size()!=0){
                getline(input,date);
                getline(input,name);
                insert_w(id,date,name);
            }
    input.close(); // reading words' information and saving in Wo...
    input.open("friend.txt");
    if (input.is_open())
        while (getline(input, id))
            if(id.size()!=0){
                getline(input,name);
                insert_friend(id,name);
            }
    input.close(); // reading friendship relation and saving in Wo...
//----------------- beginning of main function
    int a,b;
    int m;
    int fnum;
    int tnum;
    int psize;
    string menu;
    while(1){
        system("cls");
        cout << "0. Read data files\n1. display statistics\n2. Top 5 most tweeted words\n3. Top 5 most tweeted users\n4. Find users who tweeted a word (e.g. Yonsei)\n5. Find all people who are friends of the above users\n6. Delete all mentions of a word\n7. Delete all users who mentioned a word\n8. Find strongly connected components\n9. Find shortest path from a given user\n99. Quit\nSelect Menu: ";

        getline(cin,menu);
        while(menu.size()==0 || !is_digits(menu)){
            if(menu.size()!=0) cout << "\nPlease enter only integer, 0 or upper than it\nSelect Menu: ";
            getline(cin,menu);
        }

        m=stoi(menu);
        system("cls");

        if(m==99) break;

        else if(m==0){
            psize=fnum=tnum=0;
            search_size(Prof,&psize,&fnum,&tnum);
            cout << "Total users: " << psize << endl;
            cout << "Total friendship records: " << fnum << endl;
            cout << "Total tweets: " << tnum << endl;
            cout << "\nPress any key to continue... ";
            getch();
        }

        else if(m==1){
            f_max=f_min=t_max=t_min=fnum=tnum=0;
            psize=0;

            if(Prof!=NULL){
                f_max=f_min=Prof->P->fnum;
                t_max=t_min=Prof->P->tnum;
            }

            search_size(Prof,&psize,&fnum,&tnum);
            if(psize==0) psize++;
            cout << "Average number of friends: " << (int)round((double)fnum/(double)psize) << endl;
            cout << "Minimum number of friends: " << f_min << endl;
            cout << "Maximum number of friends: " << f_max << endl;

            cout << "\nAverage tweets per user: " << (int)round((double)tnum/(double)psize) << endl;
            cout << "Minimum tweets per user: " << t_min << endl;
            cout << "Maximum tweets per user: " << t_max << endl;
            cout << "\nPress any key to continue... ";
            getch();
        }

        else if(m==2){
            cout<< "Top 5 most tweeted words\n" <<endl;
            for(Adjw* A=top5_w;A!=NULL;A=top5_w){
                top5_w=top5_w->next;
                delete A;
            }
            top5_w=NULL;
            topnum=5;
            top5_word(Wo);

            topnum=1;
            for(Adjw* A=top5_w ; A!=NULL ; topnum++, A=A->next){
                cout<< topnum << ". " << A->data->word << endl;
                cout<< "tweeted "<< A->data->tweeted <<" time(s)\n" << endl;
            }
            cout << "\nPress any key to continue... ";
            getch();
        }

        else if(m==3){
            cout<< "Top 5 most tweeted people\n" <<endl;
            for(Adj* A=top5_p;A!=NULL;A=top5_p){
                top5_p=top5_p->next;
                delete A;
            }
            top5_p=NULL;
            topnum=5;
            top5_profile(Prof);

            topnum=1;
            for(Adj* A=top5_p ; A!=NULL ; topnum++, A=A->next){
                cout<< topnum << ". " << A->data->name << endl;
                cout<< "tweeted "<< A->data->tnum <<" time(s)\n" << endl;
            }
            cout << "\nPress any key to continue... ";
            getch();
        }

        else if(m==4){
            free_foa(Prof);

            for(Adj* A=FOA;A!=NULL;A=FOA){
                FOA=FOA->next;
                delete A;
            }

            for(Adj* A=fusers;A!=NULL;A=fusers){
                fusers=fusers->next;
                delete A;
            }

            cout<< "Enter a word: ";
            getline(cin, name);
            cout<< "\nUsers who tweeted the word:\n" <<endl;

            Node* X=search_w_s(Wo,name);

            Adj* P;
            Adj* Q;
            Adj* R;
            if(X!=NULL && X->W->word.compare(name)==0)
                for(Adj* s=X->W->tweeters;s!=NULL;s=s->next){
                    P=new Adj;
                    P->data=s->data;
                    P->date=s->date;
                    P->next=fusers;
                    fusers=P;
                    // to print fusers->name

                    for(Adj* s2=P->data->friends;s2!=NULL;s2=s2->next){
                        if(s2->data->foa==1) continue; // checking if same name exists in FOA
                        s2->data->foa=1;

                        Q=new Adj;
                        Q->data=s2->data;
                        Q->next=FOA;
                        FOA=Q;
                    }
                }

            for(P=fusers;P!=NULL;P=P->next){
                cout << "Username : " << P->data->name << endl;
                cout << "Tweeted on " << P->date << endl;
                cout << "\n";
            }
            cout<< "\nPress any key to continue... ";
            getch();
        }

        else if(m==5){
            cout<< "Friends of users found in question 4: " <<endl;
            for(Adj* i=FOA;i!=NULL;i=i->next)
                cout << i->data->name << endl;
            cout << "\nPress any key to continue... " <<endl;
            getch();
        }

        else if(m==6){
            cout<< "Enter a word: ";
            getline(cin, name);
            Node* x=search_w_s(Wo,name);
            if(x!=NULL && x->W->word.compare(name)==0)
                delete_w(x);
            cout<< "\nDelete Completed!" <<endl;
            cout<< "\nPress any key to continue... " <<endl;
            getch();
        }

        else if(m==7){
            cout<< "Enter a word: ";
            getline(cin, name);
            cout<< "\nDeleted users: " <<endl;

            Node* x=search_w_s(Wo,name);
            Node* P;

            if(x!=NULL && x->W->word.compare(name)==0){
                for(Adj* A=x->W->tweeters ; A!=NULL ; A=x->W->tweeters){
                    cout<<A->data->name<<endl;

                    P=search_p_s(Prof,A->data->user);

                    if(P->P->user.compare(A->data->user)==0)
                        delete_p(P);

                }
                delete_w(x);
            }

            cout<< "\nDelete Completed!" <<endl;
            cout<< "\nPress any key to continue... " <<endl;
            getch();
        }

        else if(m==8){
            cout<<"Strongly Connected Component Top5\n"<<endl;
            topnum=5;
            SCC_K();
            topnum=1;
            for(Path* A=SCC;A!=NULL;A=SCC, topnum++){
                cout<<topnum;

                if(topnum==1) cout<<"st ";
                else if(topnum==2) cout<<"nd ";
                else if(topnum==3) cout<<"rd ";
                else cout<<"th ";
                cout<<"strongly connected component"<<endl;

                for(Adj* B=A->link ; B!=NULL ; B=A->link){
                    A->link=A->link->next;
                    cout<<B->data->name<<" ";
                    delete B;
                }

                cout<<"\n"<<endl;
                SCC=SCC->next;
                delete A;
            }
            SCC=NULL;

            for(Adj* B=scc_c;B!=NULL;B=scc_c){
                scc_c=scc_c->next;
                delete B;
            }
            scc_c=NULL;

            cout<< "\nPress any key to continue... " <<endl;
            getch();
        }

        else if(m==9){
            free_d(Prof);
            cout<< "Enter a word: ";
            getline(cin, name);
            cout<< "\nShortest path from " << name <<endl;

            hsize=1000; curr=0;
            Node* O=search_p_s(Prof,name);

            if(O!=NULL){
                SHP(O->P,0);
                O->P->d=-1;
            }
            while(curr>0)
                hdelete_top(0);

            int i=1;

            for(Adj* A=S_P; A!=NULL; A=S_P){
                S_P=S_P->next;
                cout<< "\n" << i;
                if(i==1) cout<<"st";
                else if(i==2) cout<<"nd";
                else if(i==3) cout<<"rd";
                else cout<<"th";
                cout<<" shortest path" <<endl;

                Profile* B=A->data;
                B->d=-1;

                while(1){
                    if(B==A->data)
                        cout<< B->name <<"(" <<B->user <<") ";
                    else
                        cout<< "<- " << B->name <<"(" <<B->user <<") ";
                    if(B==O->P) break;
                    B=B->from;
                }
                cout<<"\nweight : " << A->weight;

                cout<<"\n";
                i++;
                delete A;
            }

            cout<< "\nPress any key to continue... " <<endl;
            getch();
        }

        else{
            cout << "Wrong number... Check menu again please.\n\nPress any key to continue... ... ";
            getch();
        }
    }

    cout << "Program terminated successfully. Thank you." << endl;
    return 0;
}
