//
// Created by m7mdn on 7/25/2022.
//

#include "SingleLinkedList.h"

namespace SingleLinkedList {

    _stack *merge_in_place(_stack *x, _stack *y, size_t size, bool (*hash_function)(void *p1, void *p2)) {
        _stack *result = new _stack;
        while (true) {
            try {
                std::cout << *(int *) x->_read_head__(size) << '\t' << *(int *) y->_read_head__(size) << '\n';

                if (!hash_function(x->_read_head__(size), y->_read_head__(size))) {
                    result->_push(x->_pop(size), size);
                } else {
                    result->_push(y->_pop(size), size);
                }
            } catch (ListIsEmpty) { break; }
        }
        while (true) {
            try { result->_push(x->_pop(size), size); } catch (ListIsEmpty) { break; }
        }
        while (true) {
            try { result->_push(y->_pop(size), size); } catch (ListIsEmpty) { break; }
        }

        return result;
    }

    _queue *
    merge_in_place(_queue *x, _queue *y, size_t size, short array_length, bool (*hash_function)(void *p1, void *p2)) {
        _queue *result = new _queue();
        while (true) {
            try {
                std::cout << *(int *) x->_readTop(size) << '\t' << *(int *) y->_readTop(size) << '\n';

                if (!hash_function(x->_readTop(size), y->_readTop(size))) {
                    result->_push(x->_pop(size, array_length), size, array_length);
                } else {
                    result->_push(y->_pop(size, array_length), size, array_length);
                }
            } catch (ListIsEmpty) { break; }
        }
        while (true) {
            try { result->_push(x->_pop(size, array_length), size, array_length); } catch (ListIsEmpty) { break; }
        }
        while (true) {
            try { result->_push(y->_pop(size, array_length), size, array_length); } catch (ListIsEmpty) { break; }
        }

        return result;
    }


    _stack::_stack() : head(nullptr), array_length(1), len(0) {}

    void _stack::_push(unsigned char *data, size_t size) {
        len++;
        if (head == nullptr) {
            head = new Node;
            head->data = (unsigned char *) malloc(array_length * size);
            head->len = 0;
            head->next = 0;
            __BUFFER::_push_head(head, data, size, array_length);
        } else {
            try {
                __BUFFER::_push_head(head, data, size, array_length);
            } catch (ArrayIsFilled &) {

                array_length = short(array_length + 1);
                Node *nh = new Node;
                nh->data = (unsigned char *) malloc(array_length * size);
                nh->next = head;
                nh->len = 0;
                head = nh;
                __BUFFER::_push_head(head, data, size, array_length);
            }
        }
    }

    unsigned char *_stack::_pop(size_t size) {
        if (head == nullptr) {
            throw ListIsEmpty();
        } else {
            try {
                --len;
                return __BUFFER::_pop_head(head, size);
            } catch (ArrayIsEmpty &) {
//                std::cout<<"head :"<<head<<'\span';
                Node *_next = head;
                head = head->next;
                _next->next = nullptr;
                delete[] _next->data;
                delete _next;
                ++len;
                return _pop(size);
            }
        }
    }

    void _stack::_start_(Node *&node, unsigned char *&reader, size_t size) {
        node = head;
        reader = head->data + (head->len - 1) * size;
    }

    unsigned char *_stack::_next_(Node *&node, unsigned char *reader, size_t size) {
        if (node == 0) {
            return 0;
        }
        if (reader > node->data) {
            return reader - size;
        } else {
            node = node->next;
            if (node == nullptr) {
                reader = nullptr;
                return reader;
            }
            return node->data + (node->len - 1) * size;
        }
    }


    unsigned char *_stack::_read_head__(size_t size) {
        if (head == nullptr) { return nullptr; }
        try {
            return _read_head(head, size);
        } catch (ArrayIsEmpty &) {
            return _read_head(head->next, size);
        }
    }

    int delete_node(Node *head, Node *tail = nullptr) {

        if (head == tail) {
            return 0;
        } else {
            delete[] head->data;
            delete_node(head->next, tail);
            delete head;
            return 0;
        }
    }


    void swap(void *first, void *second, size_t size) {
        if (first == second) { return; }
        unsigned char* item = (unsigned char*)alloca(size);

        memcpy(item, first, size);
        memcpy(first, second, size);
        memcpy(second, item, size);

    }

    void b_sort(Node *node, unsigned char *start, int n, size_t size, bool (*hash_function)(void *p1, void *p2),
                unsigned char *next(Node *&node, unsigned char *reader, size_t size)) {
        short i;
        bool not_sorted = true;
        unsigned char *reader;
        void *back;
        Node *node1;

        while (not_sorted) {
            not_sorted = false;

            i = 0;
            reader = start;
            back = start;
            node1 = node;

            reader = next(node1, reader, size);

            while (i < n and reader != 0) {
                if (hash_function(reader, back)) {
                    swap(back, reader, size);
                    not_sorted = true;
                }
                back = reader;
                reader = next(node1, reader, size);
                i++;
            }
        }
    }

    void print(Node *node, unsigned char *reader, int length,
               unsigned char *next(Node *&node, unsigned char *reader, size_t size)) {
        int i = 0;
        while (i < length and reader != nullptr) {
            std::cout << *(int *) reader << '\n';
            reader = next(node, reader, 4);
            i++;
        }
        break_line
    }

    void q_sort(Node *node_i, unsigned char *start, int length, size_t size, bool (*hash_function)(void *p1, void *p2),
                unsigned char *next(Node *&node, unsigned char *reader, size_t size)) {
        if (length < 4) {
            b_sort(node_i, start, length, size, hash_function, next);
            return;
        }

        Node *start_node = node_i;
        Node *node_j = node_i;

        unsigned char *reader_i = start;
        unsigned char *reader_j = start;
        unsigned char *reader_j1 = start;

        reader_j = next(node_j, reader_j, size);
        reader_i = next(node_i, reader_i, size);
        int i = 0;
        int j = 0;

        while (i < length and reader_i != nullptr) {
            if (hash_function(reader_i, start)) {
                swap(reader_i, reader_j, size);
                reader_j1 = reader_j;
                reader_j = next(node_j, reader_j, size);
                j++;
            }
            reader_i = next(node_i, reader_i, size);
            i++;
        }

        swap(reader_j1, start, size);

        q_sort(start_node, start, j - 1, size, hash_function, next);
        q_sort(node_j, reader_j, i - j, size, hash_function, next);
    }

    int delete_node(RNode *head, RNode *tail = nullptr) {

        if (head == tail) {
            return 0;
        } else {
            free(head->data);
            delete_node(head->next, tail);
            delete head;
            return 0;
        }
    }

    _stack::~_stack() {
        delete_node(head);
    }

    void _stack::_sort(size_t size, bool (*hash_function)(void *, void *)) {

        q_sort(head, _read_head__(size), len, size, hash_function, _stack::_next_);
    }
    //------------------------------------------------------------------------------------------
    // ** queue **

    void _queue::_start_(Node *&node, unsigned char *&reader, size_t size) {
        node = tail;
        reader = tail->data;
    }

    unsigned char *_queue::_next_(Node *&node, unsigned char *reader, size_t size) {
        if (node == nullptr) { return nullptr; }
        if (node->data + (node->len - 1) * size > reader) {
            return reader + size;
        } else {
            node = node->next;
            if (node == nullptr) {
                return nullptr;
            }
            return node->data;
        }
    }

    unsigned char *_queue::_readTop(size_t size) {
        return tail->data;
    }

    _queue::_queue() : head(0), tail(0), len(0) {
    }

    _queue::~_queue() {
        delete_node(tail, nullptr);
    }

    void _queue::_push(unsigned char *data, size_t size, short array_length) {
        ++len;
        if (head == nullptr) {
            head = new Node;
            head->len = 0;
            head->data = new unsigned char[array_length * size];
            head->next = 0;
            tail = head;
            __BUFFER::_push_head(head, data, size, array_length);
        } else {
            try {
                __BUFFER::_push_head(head, data, size, array_length);
            } catch (ArrayIsFilled &) {
//                std::cout << "new buffer is instantiated of length = " << array_length << std::endl;
                Node *nh = new Node;
                nh->len = 0;
                nh->data = new unsigned char[array_length * size];
                nh->next = nullptr;
                head->next = nh;
                head = nh;
                __BUFFER::_push_head(head, data, size, array_length);
            }
        }
    }

    unsigned char *_queue::_pop(size_t size, short array_length) {
        --len;
        if (head == nullptr) {
            throw ListIsEmpty();
        } else if (head != tail) {
            if (tail->len != 0) {
                tail->len--;
                tail->data += size;
                return tail->data - size;

            } else {

                auto a = tail->next;
                delete[] (tail->data - array_length * size);
                delete tail;

                tail = a;
                ++len;
                return _pop(size, array_length);
            }
        } else {
            try {
                return __BUFFER::_pop_tail(tail, size, array_length);
            } catch (ArrayIsEmpty &) {
                throw ListIsEmpty();
            }
        }
    }

    int _queue::_length() { return len; }

    void _queue::_sort(size_t size, bool (*hash_function)(void *p1, void *p2)) {
        q_sort(tail, tail->data, len, size, hash_function, _queue::_next_);
    }

    //------------------------------------------------------------------------------------------
    // ** ring **
    _ring::_ring(size_t size) : size(size), tail(nullptr) {
    }

    _ring::~_ring() {
        if (tail == 0) {
            delete tail;
            return;
        }
        delete_node(tail->next, tail);
        free(tail->data);
        delete tail;
    }

    void _ring::_push(unsigned char *data) {
        if (tail == nullptr) {
            tail = new RNode{nullptr, malloc(size)};
            tail->next = tail;
            memcpy(tail->data, data, size);
        } else {
            RNode *node = new RNode{tail->next, malloc(size)};
            tail->next = node;
            memcpy(tail->next->data, data, size);
        }
    }


    void *_ring::_read() {
        if (tail == nullptr) {
            throw ListIsEmpty();
        } else {
            tail = tail->next;
            return tail->data;
        }
    }

    void *_ring::_pop_front() {
        if (tail == nullptr) {
            throw ListIsEmpty();
        } else {
            void *item;
            RNode *index;
            index = tail->next;
            item = tail->next->data;
            tail->next = tail->next->next;
            delete index;

            return item;
        }
    }
}