#include <iostream>
#include <random>
#include <assert.h>





template<class T, class CompareType = std::less<T>, class URNG = std::default_random_engine>
class treap {
private:
    URNG random_gen;
    CompareType compare;
    class node {
    public:
        T value;
        float priority;
        node* left;
        node* right;
        node* parent;

        node(T value, float priority) : value(value), priority(priority), left(nullptr), right(nullptr), parent(nullptr){}

        template<typename... Args>
        node(float priority, Args...  args) : value(args...), priority(priority), left(nullptr), right(nullptr), parent(nullptr){}


        bool has_left(){
            return left != nullptr;
        }
        bool has_right(){
            return right != nullptr;
        }
        void replace(node* from, node* to){
            if(left == from){
                left = to;
            }else if(right == from){
                right = to;
            }else{
                throw 1;
            }
        }
        void clear(){
            parent = nullptr;
            if(has_left())  left->clear();
            delete left;
            if(has_right()) right->clear();
            delete right;
        }
        void toJSON(std::ostream& out){
            out << "{";
            out << "v:"<<value<<",";
            out << "p:"<<priority<<",";
            out << "l:";
            if(left == nullptr){
                out << "null";
            }else{
                left->toJSON(out);
            }
            out << ",";

            out << "r:";
            if(right == nullptr){
                out << "null";
            }else{
                right->toJSON(out);
            }
            out << "}";
        }

    };
    node* root;

public:

    treap() : treap({}) {
    }
    treap(std::initializer_list<T> init_list) : treap(init_list, std::less<T>()) {
    }
    treap(CompareType comparetype, std::initializer_list<T> init_list) : treap(init_list, comparetype){
    }
    treap(std::initializer_list<T> init_list, CompareType comparetype) : treap(init_list, comparetype, URNG(164894)){
    }

    treap(CompareType comparetype, std::initializer_list<T> init_list, URNG&& urng) : treap(init_list, comparetype, urng){
    }
    treap(URNG&& urng, std::initializer_list<T> init_list, CompareType comparetype) : treap(init_list, comparetype, urng){
    }
    treap(URNG&& urng, CompareType comparetype, std::initializer_list<T> init_list) : treap(init_list, comparetype, urng){
    }
    treap(CompareType comparetype, URNG&& urng, std::initializer_list<T> init_list) : treap(init_list, comparetype, urng){
    }
    treap(std::initializer_list<T> init_list, CompareType comparetype, URNG&& urng){
        root = nullptr;
        random_gen = urng;
        compare = comparetype;
        for(T element : init_list){
            insert(element);
        }
    }
    bool empty(){
        return root == nullptr;
    }

    node* lowest_node(){
        node* n = root;
        if(n == nullptr){
            return nullptr;
        }
        while(n->has_left()){
            n = n->left;
        }
        return n;
    }
    T& lowest(){
        node* n = lowest_node();
        if(n == nullptr){
            throw 2;
        }
        return n->value;
    }
    node* highest_node(){
        node* n = root;
        if(n == nullptr){
            return nullptr;
        }
        while(n->has_right()){
            n = n->right;
        }
        return n;
    }
    T& highest(){
        node* n = highest_node();
        if(n == nullptr){
            throw 3;
        }
        return n->value;
    }
    void insertBST(node* n){
        if(root != nullptr){
            insertBST(root, n);
        }else{
            root = n;
        }
    }
    void insertBST(node* that, node* n){
        if(compare(n->value, that->value)){
            if(that->left == nullptr){
                that->left = n;
                n->parent = that;
            }else{
                insertBST(that->left, n);
            }
        }else{
            if(that->right == nullptr){
                that->right = n;
                n->parent = that;
            }else{
                insertBST(that->right, n);
            }
        }
    }
    void insert(T& value){
        float random_priority = std::generate_canonical<float, 10>(random_gen);
        node* n = new node(value, random_priority);

        insertBST(n);
        while(n != root){
            if(n->priority > n->parent->priority){
                if(n->parent->value < n->value){
                    rotate_left(n);
                }else{
                    rotate_right(n);
                }
            }else{
                break;
            }
        }
    }
    template<typename... Args>
    void emplace(Args... args){
        float random_priority = std::generate_canonical<float, 10>(random_gen);
        node* n = new node(random_priority, args...);

        insertBST(n);
        while(n != root){
            if(n->priority > n->parent->priority){
                if(n->parent->value < n->value){
                    rotate_left(n);
                }else{
                    rotate_right(n);
                }
            }else{
                break;
            }
        }
    }
    void rotate_right(node* x){
        node* y = x->parent;
        node* b = x->right;

        x->parent = y->parent;
        if(x->parent != nullptr) x->parent->replace(y, x);
        y->parent = x;
        x->right = y;
        y->left = b;
        if(b != nullptr) b->parent = y;

        if(x->parent == nullptr) root = x;
    }
    void rotate_left(node* y){
        node* x = y->parent;
        node* b = y->left;

        y->parent = x->parent;
        if(y->parent != nullptr) y->parent->replace(x, y);
        x->parent = y;
        x->right = b;
        if(b != nullptr) b->parent = x;
        y->left = x;

        if(y->parent == nullptr) root = y;
    }

    void clear(){
        node* n = root;
        n->clear();
    }
    ~treap(){
        clear();
    }
    void toJSON(std::ostream& out){
        if(root == nullptr){
            out << "null";
        }else{
            root->toJSON(out);
        }
    }

};

