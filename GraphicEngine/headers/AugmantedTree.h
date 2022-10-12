 /*
* an AVL tree based system with
* last edit in 26 septemper 2022
* 
*/

#include "DataStructures.h"
#include "SingleLinkedList.h"

#ifndef DATASTRUCTURES_CPP_AUGMANTEDTREE_H
#define DATASTRUCTURES_CPP_AUGMANTEDTREE_H

#define BTREE_BUFFER_SIZE 10

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
std::string space(int n);

namespace AugmantedTree {
    using namespace __BUFFER;

    typedef struct tree_node : BUFFER {
        tree_node *left;
        tree_node *right;
        int height;
        
    } tree_node;

    typedef SingleLinkedList::stack<node_ptr> stack;

    class tree {

    private:
        /* per member data */
        size_t size;
        short array_length;
    private:
        /* per class data */
        static void* data ;
        static Array<tree_node*> arrayptr;

    private:
        /* private methods */
        tree_node* _findNode(tree_node* parent);
        void* _findData();
        void AVLtree();
        tree_node* newNode();
        void attachNode(tree_node* parent, tree_node* child);
        void killNode();
        void* _insert(tree_node* parent);
        void _remove(tree_node* parent);
        node_ptr min_item(node_ptr parent);
        node_ptr max_item(node_ptr parent);

    protected:
        tree_node *root;
        int len;
        hash_value (*hash_function)(void *, void *);  // return true if the first item is greeter than the second argument
        void descending_read(void **p, node_ptr(*func)(void **));

    public:
        tree(hash_value(*hash_function)(void *, void *), size_t size, short array_length);
        virtual ~tree();
        void ascending_read(void **p, void (*func)(void **));
        void conditional_read(void **p, void (*func)(void **), bool (*condition)(void *));
        void *_find(void *data);
        void _print();
        void _insert(void *data);
        void _remove(void *data);
        node_ptr copy(node_ptr parent);

// friend functions ............
        friend void read_buf(BUFFER *buffer, void **p, void (*func)(void **));
        friend void des_read_buf(BUFFER *buffer, void **p, node_ptr(*func)(void **));
    };

    template<typename T>
    hash_value hash(void* a, void* b){
        return (*(T*) a - *(T *)b);
    }
/*
 * a Tree is a non-Ordered data structure as each item dont have an index to define them.
 */
    template<typename T>
    class treeIterator;

    template<typename T>
    class Tree : protected tree {
    private:
        node_ptr getroot() {
            return root;
        }

    public:
        Tree(hash_value(*hash_function)(void *, void *), short array_length = 5) :
                tree(hash_function, sizeof(T), array_length) {

        }
        Tree(short array_length = 5) :
                tree(hash<T>, sizeof(T), array_length) {
        }
        void insert(T &data) {
            _insert(&data);
        }

        void insert(T &&data) {
            T d = data;
            _insert(&d);
        }


        void remove(T &data) {
            _remove(&data);
        }

        T &find(T &data) {
            return *(T*)_find(&data);
        }
        T& operator[](T& data){
            return *(T*) _find(&data);
        }
        void print() {
            _print();
        }

        int length() {
            return this->len;
        }

        friend treeIterator<T>;
    };

    void iterator_start(tree_node *parent, stack *iterator);

    template<typename T>
    class treeIterator {
    private:
        unsigned char *reader;
        unsigned char *last;
        stack *iterator;
        int counter;
        Tree<T> *tree;
    public:

        treeIterator() : iterator(new stack) {}

        void start(Tree<T> &tree1) {
            counter = 0;
            tree = &tree1;
            iterator_start(tree->getroot(), iterator);
            reader = iterator->top()->data;
            last = iterator->top()->data;
            last += (iterator->top()->len - 1) * sizeof(T);
        }

        void operator++() {
            counter++;
            if (last > reader) {
                reader += sizeof(T);
                return;
            }
            if (iterator->length() > 0) {
                if (iterator->top()->leaf) {
                    iterator->pop();
                } else {

                    iterator_start((iterator->pop())->right, iterator);
                }
                reader = iterator->top()->data;
                last = reader;
                last += (iterator->top()->len - 1) * sizeof(T);

                return;
            }
        }

        T &read() { return *(T *) reader; }

        bool end() {
            if (counter < tree->len) { return true; }
            return false;
        }

        friend void iterator_start(tree_node *parent, stack *iterator);

    };

    /******************************************************/
                          /*MAP*/

    template<typename keyType, typename DataType>
    hash_value hash(void* t1, void* t2) {
       
        return (*((keyType*)t1)) - (*((keyType*)t2));
    }

    hash_value strhash(void* t1, void* t2);


    template<typename keyType, typename DataType>
    class Map:
            protected tree
    {
    private:
        node_ptr getroot() {
            return root;
        }

        struct T {
            keyType key;
            DataType data;
        };


    public:

        Map( short array_length = 5) :
                tree(hash<keyType, DataType >, sizeof(T), array_length) {

        }

        ~Map() 
        {
            std::cout << "delete Map at :" << this << '\n';

        }

        Map(hash_value(*hash_function)(void*, void*),short array_length = 5) :
            tree(hash_function, sizeof(T), array_length) {

        }

        void insert(const keyType& key ,const DataType & data) {
            T st{ key, data };
            _insert((void*) & st);
        }

        void remove(const keyType& data) {
            _remove((void*) & data);
        }

        T& find(const keyType & data) {
            return *(T *)_find((void*) & data);
        }

        DataType& operator[](const keyType& data) {
            return ((T *)_find((void*)& data))->data;
        }

        void print() {
            _print();
        }

    };
   

    template<typename DataType>
    class Map<const char * ,  DataType>
        :
        protected tree
    {
    private:
        node_ptr getroot() {
            return root;
        }

        struct strT {
            char key[50];
            DataType data;
        };

    public:

        Map(short array_length = 5) :
            tree(strhash, sizeof(strT), array_length) {

        }

        void insert(const char* key, const DataType& data) {

            strT st{0};
            strcpy(st.key, key);
            st.data = data;
            _insert((void*)&st);
        }

        void remove(const char* data) {
            _remove((void*)data);
        }

        strT& find(const char * data) {
            return *(strT*)_find((void*)data);
        }

        DataType& operator[](const char * data) {
            return ((strT*)_find((void*)data))->data;
        }

        void print() {
            _print();
        }

    };

}


namespace BTREE {

    //typedef int DATATYPE;
    typedef struct BTN {
        UINT hight;
        BTN(UINT hight) : hight(hight)
        {
        }
        BTN() :hight(0u)
        {
        }
    }BTN;


    template<typename DATATYPE>
    struct Node :BTN {
        DATATYPE* key;
        BTN* left, * right;
        Node(UINT hight, DATATYPE* key, BTN* left, BTN* right) :
            key(key), left(left), right(right), BTN(hight)
        {

        }
    };

#define Node Node<DATATYPE>

    template<typename DATATYPE>
    struct Leaf:BTN {
        UINT length;
        DATATYPE data[BTREE_BUFFER_SIZE];

        Leaf():BTN(0u) {
            length = 0;
        }

        ~Leaf() = default;

        DATATYPE* find(DATATYPE* item) {
            UINT i = 0;
            while(i < length )
            {
                if (*item == data[i])
                    return data + i;
                ++i;
            }
            return nullptr;
        }

        void leafInsert(DATATYPE* item) {
            if (length == 0) {
                data[0] = (*item);
                length++;
                return; 
            }
            
            UINT i = length;
            length++;

            while ((i > 0) && (*item < data[i - 1]))
            {
                data[i] = data[i - 1];
                i--;
            }
            data[i] = *item;
            
        }

        UINT leafremove(DATATYPE* item) {
            UINT i = 0;
            while ((i < length) && (data[i]< *item))
            {
                i++;
            }
            if (data[i] != *item) {
                return NULL;
            }
            length--;
            while (i < length)
            {
                data[i] = data[i + 1];
                i++;
            }
        }

        void print() {
           
            for (UINT i = 0; i < length; i++)
            {
                printf("%d ", data[i]);
            }
            printf("\n");
        }

        void read(void* p, void (*func)(DATATYPE item,void*p)) {

            for (UINT i = 0; i < length; i++)
            {
                func(data[i], p);
            }
        }

        Node* split() {
            /* split the leaf to two leafs connectd to a child leaf which is to be connected to the parent */
            Leaf* leaf2 = new Leaf();
            
            this->length /= 2;
            leaf2->length = BTREE_BUFFER_SIZE - this->length;
            memcpy(leaf2->data, this->data + this->length, leaf2->length * sizeof(DATATYPE));

            return new Node{ 1u,leaf2->data,(Node*)this,(Node*)leaf2 };
        }

        Node* insert( DATATYPE* item)
        {
            leafInsert(item);
            if (length == BTREE_BUFFER_SIZE)
                return split();
            return (Node*)this;
        }
        
        Node* remove(DATATYPE* item) {
            if (leafremove(item) == NULL) {
                throw NotFound();
            }

            if (length == 0) {
                //delete this;
                return nullptr;
            }
            return (Node*)this;
        }

    };

#define Leaf Leaf<DATATYPE>

    template<typename DATATYPE>
    class Btree {
    private:
        Node* root;
        UINT length;

    private:

        void set_height(Node* parent) {
            if (parent->hight == 0u) { return; }
            parent->hight = 1 + max(parent->right->hight, parent->left->hight);
        }

        BTN* remove(Node* parent,DATATYPE* item) {
            if (parent->hight == 0) {
               return ((Leaf*)parent)->remove(item);
            }

            if (*item > *(parent->key))
            {
                parent->right = remove((Node*)(parent->right), item);
                if (parent->right == nullptr)
                {
                    BTN* node = parent->left;
                    delete parent;
                    return node;
                }
                set_height(parent);
                return parent;
            }
            parent->left = remove((Node*)parent->left, item);
            if (parent->left == nullptr)
            {
                BTN* node = parent->right;
                delete parent;
                return node;
            }
            set_height(parent);
            return parent;
        }

        BTN* insert(Node* parent, DATATYPE* item) {
            if (parent->hight == 0) {
                return ((Leaf*)parent)->insert(item);
            }

            if (*item > *(parent->key))
            {
                parent->right = insert((Node*)(parent->right), item);
                set_height(parent);
                return parent;
            }
            parent->left = insert((Node*)parent->left, item);
            set_height(parent);
            return parent;
        }

        BTN* find(Node* parent, DATATYPE* item) {
            if (parent->hight == 0) {
                return parent;
            }
            if (*item > *(parent->key))
            {
                parent->right = find((Node*)(parent->right), item);
                return parent;
            }
            parent->left = find((Node*)parent->left, item);
            return parent;

        }

        void print(Node* parent) {
            space(10 - 3*parent->hight);
            if (parent->hight == 0u) {
                ((Leaf*)parent)->print();
                return;
            }
            print((Node*)parent->left);
            print((Node*)parent->right);

        }

        void read(Node* parent, void* p, void (*func)(DATATYPE item, void* p)) {
            if (parent->hight == 0u) {
                ((Leaf*)parent)->read(p, func);
                return;
            }
            read((Node*)parent->left, p, func);
            read((Node*)parent->right, p, func);

        }

    public:
        Btree():length(0),root( (Node*) new Leaf())
        {
        }

        void insert(const DATATYPE& item) {
            length++;
            root = (Node*)insert(root, (DATATYPE*) & item);
        }

        DATATYPE* find(const DATATYPE& item) {
            return ((Leaf*)find(root, (DATATYPE*)&item))->find((DATATYPE*)&item);
        }

        void print() {
            print(root);
        }

        void read(void* p, void (*func)(DATATYPE item, void* p)) {
            read(root, p, func);
        }

        void remove(const DATATYPE& item) {
            if (length == 0)
                throw EmptyTree();
            length--;
            root = (Node*)remove(root, (DATATYPE*)&item);
        }

    };
    
    template<typename KEYTYPE,typename DATATYPE>
    struct DATA {
        KEYTYPE key;
        DATATYPE data;

        DATA(KEYTYPE key, DATATYPE data):
            key(key), data(data)
        {
        }
    };

    template<typename KEYTYPE, typename DATATYPE>
    bool operator < (const DATA<KEYTYPE, DATATYPE> &a,const DATA<KEYTYPE, DATATYPE>& b) {
        return a.key < b.key;
    }

    template<typename KEYTYPE, typename DATATYPE>
    bool operator > (const DATA<KEYTYPE, DATATYPE>& a, const DATA<KEYTYPE, DATATYPE>& b) {
        return a.key > b.key;
    }

    template<typename KEYTYPE, typename DATATYPE>
    bool operator == (const DATA<KEYTYPE, DATATYPE>& a, const DATA<KEYTYPE, DATATYPE>& b) {
        return (a.key == b.key);
    }

    template<typename KEYTYPE, typename DATATYPE>
    class Map:
        public Btree< DATA<KEYTYPE, DATATYPE>>
    {

    };
    
}
#undef Node

#endif //DATASTRUCTURES_CPP_AUGMANTEDTREE_H
