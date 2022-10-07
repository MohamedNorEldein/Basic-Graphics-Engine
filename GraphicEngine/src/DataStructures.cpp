//
// Created by m7md_nor on 9/4/2021.
// modefied on 25/9/2022
//

#include "DataStructures.h"

namespace __BUFFER {
// pass by refrence....
    void _push_head(BUFFER &buf, void *item, size_t size, int array_length) {
        if (buf.len == array_length) {
            throw ArrayIsFilled();
        } else {
//            copy_item(buf, item, buf.data + buf.len * buf.size);
            memcpy(buf.data + buf.len * size, item, size);
            buf.len++;
        }
    }

    unsigned char *_pop_head(BUFFER &buf, size_t size) {
        if (buf.len > 0) {
            buf.len--;
            return buf.data + buf.len * size;
        } else {
            throw ArrayIsEmpty();
        }
    }

    unsigned char *_read_head(BUFFER &buf, size_t size) {

        if (buf.len > 0) {
            return buf.data + (buf.len - 1) * size;
        } else {
            throw ArrayIsEmpty();
        }
    }


    void _push_tail(BUFFER &buf, unsigned char *item, size_t size, int array_length) {
        if (buf.len == array_length) {
            throw ArrayIsFilled();
        } else {
            memcpy(buf.data + size, buf.data, size * buf.len);
            memcpy(buf.data, item, size);
            buf.len++;
        }

    }

    unsigned char *_pop_tail(BUFFER &buf, size_t size, int array_length) {
        if (buf.len == 0) {
            throw ArrayIsEmpty();
        } else {
            buf.len--;

            unsigned char* item = (unsigned char*)alloca(size);
            memcpy(item, buf.data, size);
            memcpy(buf.data, buf.data + size, size * buf.len);
            memcpy(buf.data + (array_length - 1) * size, item, size);

            return buf.data + (array_length - 1) * size;

        }

    }

// pass by pointer....
    void _push_head(BUFFER *buf, void *item, size_t size, int array_length) {
        if (buf->len > array_length - 1) {
            throw ArrayIsFilled();
        } else {
            memcpy(buf->data + buf->len * size, item, size);
            buf->len++;
        }
    }

    unsigned char *_pop_head(BUFFER *buf, size_t size) {
        if (buf->len > 0) {
            buf->len--;
            return buf->data + buf->len * size;
        } else {
            throw ArrayIsEmpty();
        }
    }

    unsigned char *_read_head(BUFFER *buf, size_t size) {
        if (buf == 0) { throw ListIsEmpty(); }
        if (buf->len > 0) {
            return buf->data + (buf->len - 1) * size;
        } else {
            throw ArrayIsEmpty();
        }
    }


    void _push_tail(BUFFER *buf, unsigned char *item, size_t size, int array_length) {
        if (buf->len == array_length) {
            throw ArrayIsFilled();
        } else {
            memcpy(buf->data + size, buf->data, size * buf->len);
            memcpy(buf->data, item, size);
            buf->len++;
        }

    }

    unsigned char *_pop_tail(BUFFER *buf, size_t size, int array_length) {
        if (buf->len == 0) {
            throw ArrayIsEmpty();
        } else {
            buf->len--;

            unsigned char* item = (unsigned char*)alloca(size);
            memcpy(item, buf->data, size);
            memcpy(buf->data, buf->data + size, size * buf->len);
            memcpy(buf->data + (array_length - 1) * size, item, size);

            return buf->data + (array_length - 1) * size;

        }
    }

    void *find(BUFFER *buf, void *data, int start, int end, size_t size, hash_value(*hash_function)(void *, void *)) {

        if (end == start) {
            return buf(start);
        }
        int v = hash_function(data, buf((start + end)/2));
        if(v>0){
            return find(buf,data,(start+end)/2+1,end,size,hash_function);
        }
        if(v<0){
            return find(buf,data,start,(start+end)/2,size,hash_function);
        }
        return buf((start + end) / 2);
    }

    void* ordered_insert(BUFFER* buf, void* data, size_t size, short array_length,
        hash_value(*hash_function)(void*, void*)) {
        if (buf->len == array_length) {
            throw ArrayIsFilled();
        }
        void* place = find(buf, data, 0, buf->len, size, hash_function);
        if (hash_function(data, place) == 0 and buf->len != 0) {
            throw ItemDoseExist();
            return nullptr;
        }

        buf->len++;
        memcpy((unsigned char*)place + size, place,
            (buf->data + (buf->len - 1) * size) - (unsigned char*)place);
        memcpy(place, data, size);

        return place;
    }

    void ordered_delete(BUFFER *buf, void *data, size_t size, hash_value(*hash_function)(void *, void *)) {
        if (buf->len == 0) {
            throw ArrayIsEmpty();
        }
        void *place = find(buf, data, 0, buf->len, size, hash_function);
//        printf("%x__  %x__", place, data);
        if (place == buf->data + buf->len * size or hash_function(data, place) != 0) {
            throw NotFound();
        } else {
            buf->len--;
            memcpy(place, (unsigned char *) place + size, (buf->data + buf->len * size) - (unsigned char *) place);
        }
    }

}
