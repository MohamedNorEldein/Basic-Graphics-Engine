//
// Created by m7mdn on 7/25/2022.
//
#include "DataStructures.h"

#ifndef DATASTRUCTURES_CPP_DOUBLELINKEDLIST_H
#define DATASTRUCTURES_CPP_DOUBLELINKEDLIST_H


namespace DoubleLinkedList {

    typedef struct Node : public __BUFFER::BUFFER {
        Node *next;
        Node *back;

    } Node;

    class _list {
    protected:
        DoubleLinkedList::Node *head;
        DoubleLinkedList::Node *tail;

    public:

        _list();

        _list(size_t size, short array_length);

        ~_list();

        Node *get_head();

        Node *get_tail();

        void _push(unsigned char *data, size_t size, short array_length);

        unsigned char *_pop(size_t size, short array_length);


    };

    template<typename TYPE>
    class list : protected _list {
        size_t size;
        short array_length;

    public:

        explicit list(short array_length) : _list(), array_length(array_length) {
        }

        void push(TYPE item) {
            _push((unsigned char *) (&item), size, array_length);
        }

        void push(TYPE *item) {
            _push((unsigned char *) (item), size, array_length);
        }

        void push(void *item) {
            _push((unsigned char *) (item), size, array_length);
        }

        TYPE pop() {
            return *(TYPE *) _pop(size, array_length);
        }


    };


    class _ring {
    private:
        size_t size;

    protected:
        Node *head;

    public:
        explicit _ring(size_t size);

        ~_ring();

        void _push_front(unsigned char *data);

        unsigned char *_pop_front();

        unsigned char *_go_front();

        unsigned char *_go_back();

        unsigned char *_pop_back();

    };

    template<typename TYPE>
    class ring : protected _ring {
    public:

        ring() : _ring(sizeof(TYPE)) {
        }

        void push_front(TYPE item) {       //will set_item at the beginning
            _push_front((unsigned char *) (&item));
        }

//        void push_last(T item){       //will set_item at the beginning
//            _push_((unsigned char *) (&item));
//        }

        TYPE pop_front() {                 // will pop the last element pushed
            unsigned char *data_ptr = _pop_front();
            TYPE data = *(TYPE *) data_ptr;
            delete[] data_ptr;
            return data;
        }

        TYPE pop_back() {                 // will pop the last element pushed
            unsigned char *data_ptr = _pop_back();
            TYPE data = *(TYPE *) data_ptr;
            delete[] data_ptr;
            return data;
        }

        TYPE forward() {
            return *(TYPE *) _go_front();
        }

        TYPE backward() {
            return *(TYPE *) _go_back();
        }
    };


}


#endif //DATASTRUCTURES_CPP_DOUBLELINKEDLIST_H
