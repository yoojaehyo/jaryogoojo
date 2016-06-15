#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <conio.h>
#include <malloc.h>
#include <math.h>
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
void delete_case1(Node* n);
void delete_case2(Node* n);
void delete_case3(Node* n);
void delete_case4(Node* n);
void delete_case5(Node* n);
void delete_case6(Node* n);
void rotate_left(Node* n);
void rotate_right(Node* n);


int f_max,f_min,t_max,t_min;

typedef struct adj{
    Profile* data;
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
    int tweet;
    int fnum;
    int tnum;
    Adj* friends;
    Adj* friended;
    Adjw* tweets;
} Profile; //Profile of user

typedef struct words{
    string word;
    int tweeted;
    Adj* tweeters;
} Word; //Information of word

//----------------- Black-Red tree for words

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
    if(n->parent!=NULL)
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
    }
    else N=n;

    if(N->left!=NULL || N->right!=NULL){
        Node* child=(N->right==NULL)? N->left : N->right;
        child->parent = N->parent;
        if(N==N->parent->left)
            N->parent->left=child;
        else
            N->parent->right=child; //replacing child into N

        if(N->color == BLACK){
            if(child->color == RED)
                child->color = BLACK;
            else
                delete_case1(child);
        }
    }
    else if(N->color==BLACK){
        Node* s=sibling(N);
        //검은색이고 삭제하려는 노드의 자식이 없을떄...
        if(N->parent==RED){
            if(N==N->parent->right){

            }
            else{

            }
        }

    }
    delete N;
    N=NULL;
}

void delete_case1(Node* n){
    if(n->parent==NULL)
        delete_case2(n);
}

void delete_case2(Node* n){
    Node* s = sibling(n);

    if(s->color==RED){
        n->parent->color = RED;
        s->color = BLACK;
        if(n==n->parent->left)
            rotate_left(n->parent);
        else
            rotate_right(n->parent);
    }
    delete_case3(n);
}

void delete_case3(Node* n){
    Node* s = sibling(n);

    if(n->parent->color==BLACK &&
       s->color==BLACK &&
       s->left->color==BLACK &&
       s->right->color==BLACK){
        s->color=RED;
        delete_case1(n->parent);
    }
    else
        delete_case4(n);
}

void delete_case4(Node* n){
    Node* s = sibling(n);

    if(n->parent->color==RED &&
       s->color==BLACK &&
       s->left->color==BLACK &&
       s->right->color==BLACK){
        s->color = RED;
        n->parent->color = BLACK;
    }
    else
        delete_case5(n);
}

void delete_case5(Node* n){
    Node* s = sibling(n);
    if(s->color==BLACK){
        if(n==n->parent->left &&
           s->right->color==BLACK &&
           s->left->color==RED){
            s->color=RED;
            s->left->color=BLACK;
            rotate_right(s);
        }
        else if(n==n->parent->right &&
                s->left->color==BLACK &&
                s->right->color==RED){
            s->color=RED;
            s->right->color=BLACK;
            rotate_left(s);
        }
    }
    delete_case6(n);
}

void delete_case6(Node* n){
    Node* s=sibling(n);

    s->color=n->parent->color;
    n->parent->color=BLACK;

    if(n==n->parent->left){
        s->right->color=BLACK;
        rotate_left(n->parent);
    }
    else{
        s->left->color=BLACK;
        rotate_right(n->parent);
    }
}

// inserting Profile's node

void insert_p(string user, string date, string name){
    int r=0;
    Node* no;
    Node* cu;

    no=new Node;
    no->color=RED;

    no->P=new Profile;
    no->P->user=user;
    no->P->date=date;
    no->P->name=name;

    no->P->tweet=0;
    no->P->fnum=0;
    no->P->tnum=0;

    no->P->friends=NULL;

    no->P->friended=NULL;

    no->P->tweets=NULL;

    no->parent=NULL;
    no->right=NULL;
    no->left=NULL;

    if(Prof==NULL)
        Prof=no;
    else{
        for(cu=Prof;cu!=NULL;){
            no->parent=cu;
            if(cu->P->user.compare(user)>=0)
                r=1,cu=cu->right;
            else
                r=0,cu=cu->left;
        }

        if(r==1)
            no->parent->right=no;
        else
            no->parent->left=no;
    }
    insert_case1(no);
}

void delete_p(Node* O){
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
        a->data->fnum--;

        prev=a;
        a=a->next;
        delete prev;
    } // delete friend

    Adjw* prevw;
    for(c=O->P->tweets ; c!=NULL ; ){
        prev=NULL;
        for(b=c->data->tweeters ; ; prev=b,b=b->next)
            if(b->data==O->P) break;

        if(prev==NULL)
            c->data->tweeters=b->next;
        else
            prev->next=b->next;
        delete b;

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
    if(Prof->right==NULL && Prof->left==NULL){
        delete Prof;
        Prof=NULL;
    }
    else
        delete_one(O);
}   //delete profile from Prof

void search_size(Node* a, int* size, int* fnum, int* tnum){
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

Profile* search_p_s(Node* a, string u){
    Profile* b=NULL;
    if(a->P->user.compare(u)==0)
        return a->P;
    if(a->right!=NULL)
        b=search_p_s(a->right, u);
    if(b==NULL && a->left!=NULL)
        b=search_p_s(a->left, u);
    return b;
} //searching string

void insert_friend(string u1, string u2){
    Profile* a;
    Profile* b;
    a=search_p_s(Prof, u1);
    b=search_p_s(Prof, u2);
    if(a!=NULL && b!=NULL){
        Adj* c=new Adj;
        c->data=b;
        c->next=a->friends;
        a->friends=c;

        Adj* d=new Adj;
        d->data=a;
        d->next=b->friended;
        b->friended=d;

        a->fnum++;
    }
} //u1 has friend, u2

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
                //insert_w(id,date,name);
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
            delete_p(Prof);
            getch();
        }
        /*
        else if(m==2){
            //RBT tree -> create (nlogn), check (logn), free (n)
        }

        else if(m==3){
            //RBT tree
        }

        else if(m==4){
            FOA.clear();
            fusers.clear();
            cout<< "Enter a word: ";
            getline(cin, name);
            cout<< "\nUsers who tweeted the word:\n";

            for(x=Wo.begin();x!=Wo.end();++x)
                if(x->word.compare(name)==0) break;

            if(x!=Wo.end())
                for(s=x->tweeters.begin();s!=x->tweeters.end();++s){
                    p=*s;
                    fusers.push_front(p->name);

                    for(s2=p->friends.begin();s2!=p->friends.end();++s2){
                        q=*s2;
                        FOA.push_front(q->name);
                    }
                }
            fusers.unique();
            FOA.unique();

            for(i=fusers.begin();i!=fusers.end();++i)
                cout << *i << endl;
            cout<< "\nPress any key to continue... ";
            getch();
        }

        else if(m==5){
            cout<< "Friends of users found in question 4: " <<endl;
            for(i=FOA.begin();i!=FOA.end();++i)
                cout << *i << endl;
            cout << "\nPress any key to continue... " <<endl;
            getch();
        }

        else if(m==6){
            cout<< "Enter a word: ";
            getline(cin, name);
            for(x=Wo.begin();x!=Wo.end();++x)
                if(x->word.compare(name)==0) break;
            if(x!=Wo.end())
                delete_w(x);
            cout<< "\nDelete Completed!" <<endl;
            cout<< "\nPress any key to continue... " <<endl;
            getch();
        }

        else if(m==7){
            cout<< "Enter a word: ";
            getline(cin, name);
            cout<< "\nDeleted users: " <<endl;
            while(1){
                for(x=Wo.begin();x!=Wo.end();++x)
                    if(x->word==name) break;

                if(x!=Wo.end()){
                    for(;!x->tweeters.empty();){
                        p=x->tweeters.front();
                        cout<<p->name<<endl;
                        delete_p(p);
                    }
                    delete_w(x);
                }
                else break;
            }
            cout<< "\nDelete Completed!" <<endl;
            cout<< "\nPress any key to continue... " <<endl;
            getch();
        }

        else if(m==8){

        }

        else if(m==9){

        }
        */
        else{
            cout << "Wrong number... Check menu again please.\n\nPress any key to continue... ... ";
            getch();
        }
    }

    cout << "Program terminated successfully. Thank you." << endl;
    return 0;
}
