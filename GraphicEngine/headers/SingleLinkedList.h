//
// Created by m7mdn on 7/25/2022.
//

#include "DataStructures.h"

#ifndef DATASTRUCTURES_CPP_SINGLELINKEDLIST_H
#define DATASTRUCTURES_CPP_SINGLELINKEDLIST_H


namespace SingleLinkedList {

    typedef struct Node : public __BUFFER::BUFFER {
        Node *next;
    } Node;


    void b_sort(Node *node, unsigned char *start, short n, size_t size, bool (*hash_function)(void *p1, void *p2),
                unsigned char *next(Node *&node, unsigned char *reader, size_t size));

    void q_sort(Node *node_i, unsigned char *start, int length, size_t size, bool (*hash_function)(void *p1, void *p2),
                unsigned char *next(Node *&node, unsigned char *reader, size_t size));

    template<typename T>
    class stackIterator;

    class _stack {
    public:
        short array_length;
        int len;
        Node *head;

    public:

        int _length() const {
            return len;
        }

        void _start_(Node *&node, unsigned char *&reader, size_t size);

        static unsigned char *_next_(Node *&node, unsigned char *reader, size_t size);

        _stack();

        ~_stack();

        void _push(unsigned char *data, size_t size);

        unsigned char *_pop(size_t size);

        unsigned char *_read_head__(size_t size);

        void _sort(size_t size, bool (*hash_function)(void *, void *));

        friend _stack *merge_in_place(_stack *x, _stack *y, size_t size, bool (*hash_function)(void *p1, void *p2));
    };

    _stack *merge_in_place(_stack *x, _stack *y, size_t size, bool (*hash_function)(void *p1, void *p2));

    class _queue {
        //  tail (pop) .. -> .. data .. -> .. head (set_item)
    protected:
        Node *head;
        Node *tail;

        int len;
    public:

        void _start_(Node *&node, unsigned char *&reader, size_t size);

        static unsigned char *_next_(Node *&node, unsigned char *reader, size_t size);

        _queue();

        ~_queue();

        void _push(unsigned char *data, size_t size, short array_length);

        unsigned char *_pop(size_t size, short array_length);

        unsigned char *_readTop(size_t size);

        int _length();

        void _sort(size_t size, bool (*hash_function)(void *p1, void *p2));

    };

    _queue *
    merge_in_place(_queue *x, _queue *y, size_t size, short array_length, bool (*hash_function)(void *p1, void *p2));


    template<typename T>
    class stack : protected _stack {
        size_t size;
    public:
        stack() : _stack(), size(sizeof(T)) {
        }

        void push(T item) {
            _push((unsigned char *) (&item), size);

        }

        T &pop() {
            return *(T *) _pop(size);
        }

        T top() {
            return *(T *) _read_head__(size);
        }

        void sort() {
            _sort(size, [](void *p1, void *p2) {
                if ((*(T *) p1) < (*(T *) p2)) {
                    return true;
                }
                return false;
            });
        }

        int length() {
            return len;
        }

        friend stackIterator<T>;
    };

    template<typename T>
    class stackIterator {
        unsigned char *reader;
        Node *node;
        size_t size;
        _stack *s;
    public:
        stackIterator() {
            s = nullptr;
            reader = nullptr;
            node = nullptr;
            size = sizeof(T);
        }

        explicit stackIterator(stack<T> *stack) {
            s = stack;
            reader = s->head->data;
            node = s->head;
            size = sizeof(T);
        }

        void start(stack<T> *stack) {
            s = stack;
            reader = s->head->data;
            node = s->head;
            size = sizeof(T);
            s->_start_(node, reader, size);

        }

        void start(stack <T> &stack) {
            s = &stack;
            if (s->head == nullptr) {
                reader = nullptr;
                node = nullptr;
                return;
            }
            reader = s->head->data;
            node = s->head;
            size = sizeof(T);
            s->_start_(node, reader, size);

        }

        void operator++() {
            reader = stack<T>::_next_(node, reader, size);
        }

        T &read() {
            return *(T *) reader;
        }

        bool end() {
            if (reader == nullptr or node == nullptr) {
                return false;
            }
            return true;
        }

    };

    template<typename T>
    class queueIterator;

    template<typename TYPE>
    class queue : protected _queue {

        size_t size;
        short array_length;
    public:
        queue() : _queue(), size(sizeof(TYPE)), array_length(4) {
        }

        void push(const TYPE& item) {
            _push((unsigned char *) (&item), size, array_length);

        }

        TYPE& pop() {
            return *(TYPE *) _pop(size, array_length);
        }

        TYPE& top() {
            return *(TYPE *) _readTop(size);
        }

/*
        void operator++(){ reader = _next_(node, reader,size);}

        void start(){_start_(node,reader,size);}

        bool _end(){
            if(reader== nullptr){
                return false;
            }
            return true;
        }

        TYPE& read(){
            return *(TYPE *) reader;
        }
        */

        void sort() {
            _sort(size, [](void *p1, void *p2) {
                if (*(TYPE *) p1 > *(TYPE *) p2) {
                    return true;
                }
                return false;
            });
        }

        friend class queueIterator<TYPE>;
    };


    template<typename T>
    class queueIterator {
        unsigned char *reader;
        Node *node;
        size_t size;
        queue<T> *s;
    public:
        queueIterator() {
            s = nullptr;
            reader = nullptr;
            node = nullptr;
            size = sizeof(T);
        }

        explicit queueIterator(queue<T> *stack) {
            s = stack;
            reader = s->head->data;
            node = s->head;
            size = sizeof(T);
        }

        void start(queue<T> *stack) {
            s = stack;
            reader = s->head->data;
            node = s->head;
            size = sizeof(T);
            s->_start_(node, reader, size);

        }

        void start(queue<T> &stack) {
            s = &stack;
            if (s->head == nullptr) {
                reader = nullptr;
                node = nullptr;
                return;
            }
            reader = s->head->data;
            node = s->head;
            size = sizeof(T);
            s->_start_(node, reader, size);

        }

        void operator++() {
            reader = _queue::_next_(node, reader, size);
        }

        T &read() {
            return *(T *) reader;
        }

        bool end() {
            if (reader == nullptr or node == nullptr) {
                return false;
            }
            return true;
        }
    };

    typedef struct RNode {
        RNode *next;
        void *data;
    } RNode;

    class _ring {

        size_t size;
    protected:
        RNode *tail;

    public:
        explicit _ring(size_t size);

        ~_ring();

        void _push(unsigned char *data);

        void *_pop_front();

        void *_read();

    };

    template<typename TYPE>
    class ring : protected _ring {
    public:
        ring() : _ring(sizeof(TYPE)) {
        }

        void *get_head() {
            return tail;
        }

        void push(TYPE item) {       //will set_item at the beginning
            _push((unsigned char *) (&item));
        }

        void push(TYPE *item) {       //will set_item at the beginning
            _push((unsigned char *) (item));
        }

        TYPE &pop() {                 // will pop the last element pushed
            unsigned char *data_ptr = _pop_front();
            TYPE data = *(TYPE *) data_ptr;
            delete[] data_ptr;
            return data;
        }

        TYPE *read() {
            return (TYPE *) _read();
        }
    };

    int delete_node(Node *head, Node *tail);

}


#endif //DATASTRUCTURES_CPP_SINGLELINKEDLIST_H
