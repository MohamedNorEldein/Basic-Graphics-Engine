
/*
 * update  14/9/2022
 */


#ifndef DATA_STRUCURE_DATA_STRUCTURES_H
#define DATA_STRUCURE_DATA_STRUCTURES_H
#pragma warning(disable : 4996)

#include <iostream>
#include <cstring>
#include <valarray>

#define break_line     std::cout<<"------------------------------------------------------------------------------------------------------------"<<'\n';

#define node_ptr tree_node*

class NotFound : std::exception {
};

class ItemDoseExist : std::exception {
public:
    void *ptr;

    explicit ItemDoseExist(void *ptr) : ptr(ptr) {}
};

class EmptyTree : std::exception {
};

class ArrayIsFilled : std::exception {
};

class ArrayIsEmpty : std::exception {
};

class ListIsEmpty : std::exception {
};
typedef float hash_value;
typedef unsigned char byte;

namespace __BUFFER {

    typedef struct BUFFER {
        int len;
        byte *data;
    } BUFFER;

    typedef struct BUFFER_ARRAY : BUFFER {

        size_t size;
        int array_length;
        // data are not charcters but they will be treated as chars as a char = 1 byte and
        // copying functionality is simple and copy data bit by bit

    } BUFFER_ARRAY;

// used in short readerStack as buffer based readerStack:
    void _push_head(BUFFER &buf, void *item, size_t size, int array_length);

    byte *_pop_head(BUFFER &buf, size_t size);

    void _push_tail(BUFFER &buf, byte *item, size_t size, int array_length);

// used in short queue and double queue as buffer based queue:
    byte *_pop_tail(BUFFER &buf, size_t size, int array_length);

    byte *_read_head(BUFFER &buf, size_t size);

//-----------------------------------
    void _push_head(BUFFER *buf, void *item, size_t size, int array_length);

    byte *_pop_head(BUFFER *buf, size_t size);

    void _push_tail(BUFFER *buf, void *item, size_t size, int array_length);

    byte *_pop_tail(BUFFER *buf, size_t size, int array_length);

    byte *_read_head(BUFFER *buf, size_t size);



#define buf(x) (buf->data + (x) * size)
#define pbuf(x) *(int*)(buf->data + (x) * size)

    void *find(BUFFER *buf, void *data, int start, int end, size_t size, hash_value(*hash_function)(void *, void *));

    void ordered_insert(BUFFER *buf, void *data, size_t size, short array_length,
                        hash_value(*hash_function)(void *, void *));

    void ordered_delete(BUFFER *buf, void *data, size_t size, hash_value(*hash_function)(void *, void *));
}

template<typename TYPE>
class Array {
    __BUFFER::BUFFER_ARRAY buf;
public:

    explicit Array(short array_length) {
        buf.size = sizeof(TYPE);
        buf.array_length = array_length;
        buf.len = 0;
        buf.data = (byte *) (new TYPE[array_length]);
    }

    ~Array() {
        delete[] buf.data;
    }
    TYPE operator[](int i){
        return *(TYPE*)buf.data[i];
    }
    void push_head(TYPE item) {
        __BUFFER::_push_head(buf, (byte *) (&item), buf.size, buf.array_length);
//        std::cout << *(T*)(char *)(&item)<<'\t__'<< (item) << std::endl;
    }

    TYPE pop_head() {
        return *(TYPE *) (__BUFFER::_pop_head(buf, buf.size));
    }

    void push_tail(TYPE item) {
        __BUFFER::_push_tail(buf, (byte *) (&item), buf.size, buf.array_length);
    }

    TYPE pop_tail() {
        return *(TYPE *) (__BUFFER::_pop_tail(buf, buf.size, buf.array_length));
    }

};



#endif //DATA_STRUCURE_DATA_STRUCTURES_H
