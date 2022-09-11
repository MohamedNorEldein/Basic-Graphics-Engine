// Created by m7mdn on 7/25/2022.
//

#include <AugmantedTree.h>

std::string space(int n) {
    std::string s = " ";
    for (int i = 0; i < n; i++) {
        s += ' ';
    }
    return s;
}


namespace AugmantedTree {
    using namespace __BUFFER;

    tree::tree(hash_value(*hash_function)(void *, void *), size_t size, short array_length) :
    size(size), tree_root(nullptr), len(0), hash_function(hash_function), array_length(array_length),
    condition(nullptr) {
    }


    node_ptr min_item(node_ptr parent) {
        // left part
        if (parent->left == nullptr) {
            return parent;
        } else {
            return min_item(parent->left);
        }
    }

    node_ptr max_item(node_ptr parent) {
        // right part
        if (parent->right == nullptr) {
            return parent;
        } else {
            return max_item(parent->right);
        }
    }

// assending reading.....................
    void read_buf(BUFFER *buffer, void **p, void (*func)(void **)) {
        unsigned char *reader = buffer->data;
        size_t size = ((tree *) p[1])->size;
//        printf("n = %d  ",buffer->len);
        while (reader < buffer->data + buffer->len * size) {
            p[0] = reader;
            func(p);
            reader += size;
        }
    }

    void asc_read(tree_node *parent, void **p, void (*func)(void **)) {
        if (parent == nullptr) { return; }
        asc_read(parent->left, p, func);
        read_buf(parent, p, func);
        asc_read(parent->right, p, func);

    }

    void tree::ascending_read(void **p, void (*func)(void **)) {
        asc_read(tree_root, p, func);
    }

// descending reading.......
    void des_read_buf(BUFFER *buffer, void **p, node_ptr(*func)(void **)) {
        size_t size = ((tree *) p[1])->size;
        unsigned char *reader = buffer->data + (buffer->len - 1) * size;
        while (reader >= buffer->data) {
            p[1] = reader;
            func(p);
            reader -= size;
        }
    }

    void dsc_read(tree_node *parent, void **p, node_ptr(*func)(void **)) {
        if (parent == nullptr) { return; }

        dsc_read(parent->right, p, func);
        des_read_buf(parent, p, func);
        dsc_read(parent->left, p, func);


    }

    void tree::descending_read(void **p, node_ptr(*func)(void **)) {
        dsc_read(tree_root, p, func);
    }

    void *tree::read_item(tree_node *parent, void *data, void **p, void *(*func_found)(void **),
                          void *(*func_not_found)(void **)) {
        if (parent == nullptr) {
            return func_not_found(p);
        }

        if (parent->leaf) {
//            std::cout<<"parent len= "<<parent->len<<'\n';

            p[1] = __BUFFER::find(parent, data, 0, parent->len, size, hash_function);
            if (hash_function(p[1], data) == 0) {
                return func_found(p);
            }
            return func_not_found(p);
        }

        if (hash_function(data, parent->data + (parent->len - 1) * size) > 0) {
            return read_item(parent->right, data, p, func_found, func_not_found);
        } else if (hash_function(data, parent->data) < 0) {
            return read_item(parent->left, data, p, func_found, func_not_found);
        } else {
            p[1] = __BUFFER::find(parent, data, 0, parent->len, size, hash_function);
//            std::cout<<"data is "<< * (int*) data<<"\n";
//            std::cout<<"p is "<< *(int*) p[1]<<"\n";
            return func_found(p);
        }
    }

    int height(tree_node *parent) {
        if (parent == nullptr) { return -1; }
        if (parent->leaf) { return -1; }
        return parent->height;
    }

    void set_height(tree_node *parent) {
        if (parent == nullptr) { return; }
        if (parent->leaf) { return; }
        parent->height = 1 + std::max(height(parent->right), height(parent->left));
    }

    node_ptr right_r(node_ptr x) {
        node_ptr y = x->right;
        node_ptr b = y->left;
        y->left = x;
        x->right = b;
        return y;
    }

    node_ptr left_r(node_ptr y) {
        node_ptr x = y->left;
        node_ptr b = x->right;
        y->left = b;
        x->right = y;
        return x;
    }

    int balance(node_ptr parent) {
        if (parent == nullptr) { return 0; }
        if (parent->leaf) { return 0; }

        return height(parent->left) - height(parent->right);
    }

    node_ptr avl_function(node_ptr parent) {

        if (balance(parent) > 1) {
            if (balance(parent->left) < 0) {
                parent->left = right_r(parent->left);
                parent = left_r(parent);
                set_height(parent);
                return parent;
            }
            parent = left_r(parent);
            set_height(parent);
            return parent;
        } else if (balance(parent) < -1) {
            if (balance(parent->right) > 0) {
                parent->right = (left_r(parent->right));
                parent = right_r(parent);
                set_height(parent);
                return parent;
            }
            parent = right_r(parent);
            set_height(parent);
            return parent;
        } else {
            set_height(parent);
            return parent;
        }
    }


    tree_node *tree::read_node(tree_node *parent, void *data, void **p, node_ptr(*func_found)(node_ptr, void **),
                               node_ptr(*func_not_found)(node_ptr, void **)) {
        if (parent == nullptr) {
            return func_not_found(parent, p);
        }
        if (parent->leaf) {
            p[0] = __BUFFER::find(parent, data, 0, parent->len, size, hash_function);
            if (hash_function(p[0], data) == 0) {
                return func_found(parent, p);
            }
            return func_not_found(parent, p);
        }
        // nodes
        if (hash_function(data, parent->data + (parent->len - 1) * size) > 0 and parent->len == array_length) {
            parent->right = avl_function(read_node(parent->right, data, p, func_found, func_not_found));
            return parent;
        } else if (hash_function(data, parent->data) < 0 and parent->len == array_length) {
            parent->left = avl_function(read_node(parent->left, data, p, func_found, func_not_found));
            return parent;
        } else {
            return func_found(parent, p);
        }

    }


    tree_node *tree::insert_func(tree_node *parent, void *data) {
        if (parent == nullptr) {
            auto node = new tree_leaf;

            node->len = 0;
            node->data = (unsigned char *) malloc(size * array_length);
            node->leaf = true;

            _push_head(node, data, size, array_length);
            return (tree_node *) node;
        }
        if (parent->leaf) {
            try {
                ordered_insert(parent, data, size, array_length, hash_function);
                return parent;
            } catch (ArrayIsFilled &) {
                auto node = new tree_node;
//                {array_length, parent->data, false, nullptr, nullptr};

                node->len = parent->len;
                node->data = parent->data;
                node->leaf = false;
                node->right = 0;
                node->left = 0;

                insert_func(node, data);
                delete parent;
                return node;
            }
        }
        // nodes
        if (hash_function(data, parent->data + (parent->len - 1) * size) > 0) {
            parent->right = avl_function(insert_func(parent->right, data));
            return parent;
        } else if (hash_function(data, parent->data) < 0) {
            parent->left = avl_function(insert_func(parent->left, data));
            return parent;
        } else {
            try {
                ordered_insert(parent, data, size, array_length, hash_function);
                return parent;
            } catch (ArrayIsFilled &) {
                parent->right = avl_function(insert_func(parent->right, _pop_head(parent, size)));
                ordered_insert(parent, data, size, array_length, hash_function);
                return parent;
            }
        }
    }

    void *tree::_find(void *data) {
        void *p[3] = {data, nullptr, this};

        return read_item(tree_root, data, p, [](void **p) -> void * {return p[1]; }, [](void **p) -> void * { throw NotFound(); });
    }


    void tree::_insert(void *data) {
        len++;
        tree_root = avl_function(insert_func(tree_root, data));
    }

    tree_node *remove_rec(tree_node *parent, void **p) {

        if (parent->leaf) {
            tree *tr = (tree *) p[1];
            memcpy(p[0], parent->data, tr->size);
            _pop_tail(parent, tr->size, tr->array_length);

            if (parent->len == 0) {
                delete parent->data;
                delete parent;
                return nullptr;
            }
            return parent;
        }
        if (parent->left == 0) {
            if (parent->right == 0) {
                parent->leaf = true;
                return remove_rec(parent, p);
            }
            tree *tr = (tree *) p[1];
            memcpy(p[0], parent->data, tr->size);
            _pop_tail(parent, tr->size, tr->array_length);
            p[0] = _read_head(parent, tr->size);
            parent->right = remove_rec(parent->right, p);
            return parent;
        }
        parent->left = remove_rec(parent->left, p);
        return parent;
    }

    tree_node *removeFound(node_ptr parent, void **p) {
        tree *tr = ((tree *) p[1]);
        if (parent->leaf) {
            ordered_delete(parent, p[0], tr->size, tr->hash_function);
            if (parent->len == 0) {
                delete[] parent->data;
                delete parent;
                return nullptr;
            }
            return parent;
        } else {
            ordered_delete(parent, p[0], tr->size, tr->hash_function);
            parent->len++;
            p[0] = parent->data + (parent->len - 1) * tr->size;
            parent->right = remove_rec(parent->right, p);
            return parent;
        }
    }


    void tree::_remove(void *data) {
        void *p[2] = {data, this};
        len--;
        tree_root = avl_function(
                read_node(tree_root, data, p, removeFound, [](node_ptr, void **) -> node_ptr { throw NotFound(); }));
    }


    void delete_rec(node_ptr node) {
        if (node == nullptr) { return; }
        if (node->leaf) {
            delete[] node->data;
            delete node;
            return;
        }

        delete_rec(node->right);
        delete_rec(node->left);
        delete[] node->data;
        delete node;
    }


    tree::~tree() {
        delete_rec(tree_root);
    }


    void _print_(tree_node *parent, void *tr, int n = 0) {
        if (parent == nullptr) { return; }
        void *p[2] = {nullptr, tr};
        n++;

        if (parent->leaf) {
            std::cout << space(10 * (n - 1));
            std::cout << "h:-1 ";
            read_buf(parent, p, [](void **p) { std::cout << *(int *) p[0] << ' '; });
            std::cout << '\n';
            return;
        }

        _print_(parent->left, p[1], n);
        std::cout << space(10 * (n - 1));
        std::cout << "h:" << parent->height << ' ' << ' ';
        read_buf(parent, p, [](void **p) { std::cout << *(int *) p[0] << ' '; });
        std::cout << '\n';
        _print_(parent->right, p[1], n);

    }

    void tree::_print() {
        _print_(tree_root, this);
    }

    node_ptr tree::copy(node_ptr parent) {
        if (parent == nullptr) { return nullptr; }
        if (parent->leaf) {
            auto node = new tree_leaf;
//            {parent->len,( unsigned char*) malloc(array_length*size), true};

            node->len = parent->len;
            node->data = (unsigned char *) malloc(array_length * size);
            node->leaf = true;

            memcpy(node->data, parent->data, parent->len * size);

            return (tree_node *) node;
        }

        auto node = (tree_node *) malloc(sizeof(tree_node));

        node->height = parent->height;
        node->len = parent->len;

        node->data = new unsigned char[array_length * size];
        memcpy(node->data, parent->data, parent->len * size);

        node->left = copy(parent->left);
        node->right = copy(parent->right);

        return node;
    }

    void c_read_buf(BUFFER *buffer, void **p, void (*func)(void **)) {
        unsigned char *reader = buffer->data;
        tree *tr = ((tree *) p[1]);
        while (reader < buffer->data + buffer->len * tr->size) {
            p[0] = reader;
            if (tr->condition(p[0])) {
                func(p);
            }
            reader += tr->size;
        }
    }

    void c_read(tree_node *parent, void **p, void (*func)(void **)) {
        if (parent == nullptr) { return; }
        if (parent->leaf) {
            c_read_buf(parent, p, func);
            return;
        }
        asc_read(parent->left, p, func);
        c_read_buf(parent, p, func);
        asc_read(parent->right, p, func);

    }

    void tree::conditional_read(void **p, void (*func)(void **), bool (*_condition)(void *)) {
        tree::condition = _condition;
        c_read(tree_root, p, func);
    }

    void iterator_start(tree_node *parent, stack *iterator) {
        if (parent == nullptr) { return; }
        if (parent->leaf) {
            iterator->push(parent);
            return;
        }
        iterator->push(parent);
        iterator_start(parent->left, iterator);
    }
}

