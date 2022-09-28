
/*
 * update  14/9/2022
 */


#ifndef DATA_STRUCURE_DATA_STRUCTURES_H
#define DATA_STRUCURE_DATA_STRUCTURES_H
#pragma warning(disable : 4996)

#include <iostream>
#include <cstring>


#define break_line     std::cout<<"------------------------------------------------------------------------------------------------------------"<<'\n';

#define node_ptr AugmantedTree::tree_node*

class NotFound : std::exception {
};

class ItemDoseExist : std::exception {
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

#define TREE_DATA(x,i) ((x)->data + (i) * size)
#define buf(x) (buf->data + (x) * size)

//#define pbuf(x) *(int*)(buf->data + (x) * size)

namespace __BUFFER {

    typedef struct BUFFER {
        int len;
        byte* data;
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

    void *find(BUFFER *buf, void *data, int start, int end, size_t size, hash_value(*hash_function)(void *, void *));

    void* ordered_insert(BUFFER *buf, void *data, size_t size, short array_length,
                        hash_value(*hash_function)(void *, void *));

    void ordered_delete(BUFFER *buf, void *data, size_t size, hash_value(*hash_function)(void *, void *));
}

typedef unsigned int UINT;
template<typename TYPE, UINT ARRAY_LENGTH = 0u>
class Array 
{
protected:
    UINT len;
    TYPE data[ARRAY_LENGTH];

protected:
    virtual int find(TYPE& item, UINT start, UINT end) {
        if (end == start) {
            return start;
        }
        int v = item - data[int((start + end) / 2)];
        if (v > 0) {
            return find(item, (start + end) / 2 + 1, end);
        }
        if (v < 0) {
            return find(item, start, (start + end) / 2);
        }
        return ((start + end) / 2);
    }

public:

    Array()
        :len(0)
    {
    }

    ~Array() {
    }

    TYPE& operator[](int i){
        
        return data[i];
    }
    void push_head(const TYPE& item) {
        if (len < ARRAY_LENGTH) {
            data[len] = item;
            len++;
            return;
        }
        throw ArrayIsFilled();
    }

    TYPE& pop_head() {
        if (len < ARRAY_LENGTH) {
            len--;
            return data[len];
        }
        throw ArrayIsEmpty();
    }

    void push_tail(const TYPE& item) {
        memcpy(data, data + 1, sizeof(TYPE) * len);
        data[0] = item;
        len++;
    }

    TYPE& pop_tail() {
        TYPE item = data[0];
        len--;
        memcpy(data + 1, data, sizeof(TYPE) * len);
        return item;
    }



    virtual TYPE& find(TYPE& item) {
        return data[find(item, 0u, len)];
    }

    void insert(TYPE& item) {
        UINT pos = find(item, 0u, len);
        memcpy(data + pos, pos + data + 1, sizeof(TYPE) * len);
        data[pos] = item;
        len++;
        
    }

    void clear() {
        len = 0;
    }

    int length() {
        return  len;
    }

    bool empty() {

        if (!len)
            return true;
        return false;
    }

};


template<typename TYPE >
class Array<TYPE,0u>
{
protected:
    UINT ARRAY_LENGTH, len;
    TYPE* data;

protected:
    virtual int find(TYPE& item, UINT start, UINT end) {
        if (end == start) {
            return start;
        }
        int v = item - data[int((start + end) / 2)];
        if (v > 0) {
            return find(item, (start + end) / 2 + 1, end);
        }
        if (v < 0) {
            return find(item, start, (start + end) / 2);
        }
        return ((start + end) / 2);
    }

public:

    Array( UINT ARRAY_LENGTH)
        :len(0), ARRAY_LENGTH(ARRAY_LENGTH)
    {
        data = (TYPE*)malloc(ARRAY_LENGTH * sizeof(TYPE));
    }

    ~Array() {
        delete[] data;
    }

    TYPE& operator[](int i) {

        return data[i];
    }

    void push_head(const TYPE& item) {
        if (len < ARRAY_LENGTH) {
            data[len] = item;
            len++;
            return;
        }
        throw ArrayIsFilled();
    }

    TYPE& pop_head() {
        if (len < ARRAY_LENGTH) {
            len--;
            return data[len];
        }
        throw ArrayIsEmpty();
    }

    void push_tail(const TYPE& item) {
        memcpy(data, data + 1, sizeof(TYPE) * len);
        data[0] = item;
        len++;
    }

    TYPE& pop_tail() {
        TYPE item = data[0];
        len--;
        memcpy(data + 1, data, sizeof(TYPE) * len);
        return item;
    }



    virtual TYPE& find(TYPE& item) {
        return data[find(item, 0u, len)];
    }

    virtual void insert(TYPE& item) {
        UINT pos = find(item, 0u, len);
        memcpy(data + pos, pos + data + 1, sizeof(TYPE) * len);
        data[pos] = item;
        len++;
    }

    void clear() {
        len = 0;
    }

    int length() {
        return  len;
    }

    bool empty() {

        if (!len)
            return true;
        return false;
    }

};



#endif //DATA_STRUCURE_DATA_STRUCTURES_H
