//
// Created by m7mdn on 7/25/2022.
//

#include "DataStructures.h"
#include "SingleLinkedList.h"

#ifndef DATASTRUCTURES_CPP_AUGMANTEDTREE_H
#define DATASTRUCTURES_CPP_AUGMANTEDTREE_H


namespace AugmantedTree {
    using namespace __BUFFER;

    typedef struct tree_leaf : BUFFER {
        bool leaf;
    } tree_leaf;

    typedef struct tree_node : tree_leaf {
        tree_node *left;
        tree_node *right;
        int height;
    } tree_node;

    typedef SingleLinkedList::stack<node_ptr> stack;

    template<typename T>
    class treeIterator;

    class tree {
        size_t size;
        short array_length;

        bool (*condition)(void *);

        tree_node *insert_func(tree_node *parent, void *data);

    protected:
        tree_node *tree_root;
        int len;

        hash_value
        (*hash_function)(void *, void *);  // return true if the first item is greeter than the second argument

        tree_node *read_node(tree_node *parent, void *data, void **p, node_ptr(*func_found)(node_ptr, void **),
                             node_ptr(*func_not_found)(node_ptr, void **));

        void *read_item(tree_node *parent, void *data, void **p, void *(*func_found)(void **),
                        void *(*func_not_found)(void **));

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

        friend tree_node *removeFound(node_ptr parent, void **p);

        friend void c_read_buf(BUFFER *buffer, void **p, void (*func)(void **));

        friend tree_node *remove_rec(tree_node *parent, void **p);
    };

    template<typename T>
    hash_value hash(void* a, void* b){
        return (*(T*) a - *(T *)b);
    }
/*
 * a Tree is a non-Ordered data structure as each item dont have an index to define them.
 */
    template<typename T>
    class Tree : protected tree {
    private:
        node_ptr getroot() {
            return tree_root;
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
            return tree_root;
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
            return tree_root;
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

#endif //DATASTRUCTURES_CPP_AUGMANTEDTREE_H
