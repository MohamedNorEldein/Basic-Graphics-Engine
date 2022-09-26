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
#define StackBufferCount ( 100) 

#define StackPush(X)		arrayptr.push_head((X))
#define StackPop()			arrayptr.pop_head()
#define StackRead() 		arrayptr[arrayptr.length() - 1]

Array<AugmantedTree::tree_node*> AugmantedTree::tree::arrayptr(100);
void* AugmantedTree::tree::data = 0;


namespace AugmantedTree {
	using namespace __BUFFER;
	/************************************************************************/
	/*                              the Stack Functions                    */

	node_ptr tree::min_item(node_ptr parent) {
		// left part
		StackPush(parent);

		if (parent->left == nullptr) {
			return parent;
		}
		else {
			return min_item(parent->left);
		}
	}

	node_ptr tree::max_item(node_ptr parent) {
		// right part
		StackPush(parent);

		if (parent->right == 0) {
			return parent;
		}
		else {
			return max_item(parent->right);
		}
	}

	/*********************************************************************/
   /*                      Buffer Methods                               */

// assending reading.....................
	void read_buf(BUFFER* buffer, void** p, void (*func)(void**)) {
		unsigned char* reader = buffer->data;
		size_t size = ((tree*)p[1])->size;
		//        printf("n = %d  ",buffer->len);
		while (reader < buffer->data + buffer->len * size) {
			p[0] = reader;
			func(p);
			reader += size;
		}
	}

	void read_buf(BUFFER* buffer, size_t size) {
		unsigned char* reader = buffer->data;

		while (reader < buffer->data + buffer->len * size) {
			printf("%d ", *(int*)((*(tree_node**)reader)));
			reader += size;
		}
		printf("\n");

	}

	void asc_read(tree_node* parent, void** p, void (*func)(void**)) {
		if (parent == nullptr) { return; }
		asc_read(parent->left, p, func);
		read_buf(parent, p, func);
		asc_read(parent->right, p, func);

	}

	void tree::ascending_read(void** p, void (*func)(void**)) {
		asc_read(root, p, func);
	}

	// descending reading.......
	void des_read_buf(BUFFER* buffer, void** p, node_ptr(*func)(void**)) {
		size_t size = ((tree*)p[1])->size;
		unsigned char* reader = buffer->data + (buffer->len - 1) * size;
		while (reader >= buffer->data) {
			p[1] = reader;
			func(p);
			reader -= size;
		}
	}

	void dsc_read(tree_node* parent, void** p, node_ptr(*func)(void**)) {
		if (parent == nullptr) { return; }

		dsc_read(parent->right, p, func);
		des_read_buf(parent, p, func);
		dsc_read(parent->left, p, func);


	}

	void tree::descending_read(void** p, node_ptr(*func)(void**)) {
		dsc_read(root, p, func);
	}
	/******************************************************************/
	/*                      AVL Methods                              */

	int height(tree_node* parent) {
		if (parent == nullptr) { return -1; }
		return parent->height;
	}

	void set_height(tree_node* parent) {
		if (parent == nullptr) { return; }
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
		if (parent->height == 1
			) {
			return 0;
		}

		return height(parent->left) - height(parent->right);
	}

	node_ptr avl_function(node_ptr parent) {
		set_height(parent);
		if (balance(parent) > 1) {
			if (balance(parent->left) < 0) {
				parent->left = right_r(parent->left);
				parent = left_r(parent);
				//set_height(parent);
				return parent;
			}
			parent = left_r(parent);
			//set_height(parent);
			return parent;
		}
		else if (balance(parent) < -1) {
			if (balance(parent->right) > 0) {
				parent->right = (left_r(parent->right));
				parent = right_r(parent);
			//	set_height(parent);
				return parent;
			}
			parent = right_r(parent);
			//set_height(parent);
			return parent;
		}
		else {
			//set_height(parent);
			return parent;
		}
	}

	/*********************************************************************/
	/*                      Node Functions                              */

	tree_node* tree::_findNode(tree_node* parent)
	{
		if (parent == nullptr) {
			return nullptr;
		}
		if (hash_function(data, parent->data + (parent->len - 1) * size) > 0) {
			return _findNode(parent->right);
		}
		if (hash_function(data, parent->data) < 0) {
			return _findNode(parent->left);
		}
		return parent;
	}
	
	void* tree::_findData() {

		tree_node* node = _findNode(root);
		if (!node)
			throw NotFound();
		return find(node, data, 0, node->len - 1, size, hash_function);
	}

	void tree::AVLtree() {
		tree_node* head = 0;
		while ((arrayptr.length()) > 0)
		{
			head = StackPop();
			{
				head->left = avl_function(head->left);
				head->right = avl_function(head->right);
			}
		}
		root = avl_function(root);
	}

	tree_node* tree::newNode() {
		/*
		* create a new node from Glopal data
		*/
		tree_node* node = new tree_node{ 0 };

		node->len = 0;
		node->data = (unsigned char*)malloc(size * array_length);

		__BUFFER::_push_head(node, data, size, array_length);
		return (tree_node*)node;
	}

	void tree::attachNode(tree_node* parent, tree_node* child) {
		/*
		* attach a new leaf child to a parent
		* 1. find the side the child should be on
		* 2. assign the parent equivilant side to child
		* 3. push child to stack
		*/
		StackPush(child);

		if (hash_function(child->data, parent->data + (parent->len - 1) * size) > 0) {
			parent->right = child;
			return;
		}
		if (hash_function(child->data, parent->data) < 0) {
			parent->left = child;
			return;
		}
		printf("attach node to a parent which is not attachable\n");
		printf("child [%d ,%d] parent [%d ,%d]\n\n",
			*(int*)(child->data + (child->len - 1) * size),
			*(int*)(child->data),
			*(int*)(parent->data + (parent->len - 1) * size),
			*(int*)(parent->data)
		);

		return;
	}

	void tree::killNode() {
		tree_node* child = StackPop();
		tree_node* parent = StackRead();
		if (parent->right == child) {
			delete child;
			parent->right = 0;
			printf("kill right \n");
			return;
		}
		if (parent->left == child) {
			delete child;
			parent->left = 0;
			printf("kill left \n");
			return;
		}
		printf("screw tree \n");
	}

	void* tree::_insert(tree_node* parent) {
		if (parent == nullptr) {
			attachNode(StackRead(), newNode());
			return StackRead()->data;
		}
		StackPush(parent);
//		printf("data %d lower bound %d upper bound %d\n", *(int*)data, *(int*)TREE_DATA(StackRead(), 0), *(int*)TREE_DATA(StackRead(), StackRead()->len - 1));
		if (parent->len < array_length) {
			return ordered_insert(parent, data, size, array_length, hash_function);
		}

		if (hash_function(data, TREE_DATA(parent, 0)) < 0.0f) {
			return _insert(parent->left);
		}

		if (hash_function(data, TREE_DATA(parent, parent->len - 1)) > 0.0f) {
			return _insert(parent->right);
		}		
		void* Old_data = data;
		data = _pop_head(parent, size);
		_insert(parent->right);
		return ordered_insert(parent, Old_data, size, array_length, hash_function);
	}

	void tree::_remove(tree_node* parent) {

		if (parent == nullptr) {
			throw NotFound();
		}
		StackPush(parent);

		if (hash_function(data, TREE_DATA(parent, 0)) < 0.0f) {
			_remove(parent->left);
			return;
		}
		if (hash_function(data, TREE_DATA(parent, parent->len - 1)) > 0.0f) {
			_remove(parent->right);
			return;
		}

		ordered_delete(parent, data, size, hash_function);

		if (parent->left != NULL) {
			tree_node* node = max_item(parent->left);
			data = _pop_tail(node, size, array_length);
			ordered_insert(parent, data, size, array_length, hash_function);
			if (node->len == 0) {
				killNode();
				_remove(StackRead());
			}
			return;
		}
		if (parent->right != NULL) {
			tree_node* node = min_item(parent->left);

			data = _pop_head(min_item(parent->right), size);
			ordered_insert(parent, data, size, array_length, hash_function);
			if (node->len == 0) {
				killNode();
				_remove(StackRead());
			}
			return;
		}
		if ((parent->len == 0)) {
			killNode();
			return;
		}
	}

	/**************************************************************/
	/*					Tree Methods							 */

	tree::tree(hash_value(*hash_function)(void*, void*), size_t size, short array_length) :
		size(size),
		root(new tree_node{ 0 }),
		len(0),
		hash_function(hash_function),
		array_length(array_length)
	{
		root->data = (byte*)malloc(array_length * size);
	}

	void* tree::_find(void* _data) {
		data = _data;
		return _findData();
	}

	void tree::_insert(void* _data) {
		len++;
		//tree_root = avl_function(insert_func(tree_root, data));	
		data = _data;

		arrayptr.clear();
		_insert(root);

		AVLtree();

	}

	void tree::_remove(void* _data) {
		if (len == 0)
			throw EmptyTree();
		len--;
		//tree_root = avl_function(insert_func(tree_root, data));		
		data = _data;

		arrayptr.clear();
		_remove(root);

		AVLtree();
	}

	void delete_rec(node_ptr node) {
		if (node == nullptr) { return; }

		delete_rec(node->right);
		delete_rec(node->left);
		delete[] node->data;
		delete node;
	}

	tree::~tree() {
		std::cout << "delete tree at :" << this << '\n';

		delete_rec(root);
	}

	void _print_(tree_node* parent, void* tr, int n = 0) {
		if (parent == nullptr) { return; }
		void* p[2] = { nullptr, tr };
		n++;


		_print_(parent->left, p[1], n);
		std::cout << space(10 * (n - 1));
		std::cout << "h:" << parent->height << ' ' << ' ';
		read_buf(parent, p, [](void** p) {                 printf("%d ", *(int*)p[0]);
			});
		std::cout << '\n';
		_print_(parent->right, p[1], n);

	}

	void tree::_print() {
		_print_(root, this);
	}

	node_ptr tree::copy(node_ptr parent) {
		if (parent == nullptr) { return nullptr; }

		auto node = (tree_node*)malloc(sizeof(tree_node));

		node->height = parent->height;
		node->len = parent->len;

		node->data = new unsigned char[array_length * size];
		memcpy(node->data, parent->data, parent->len * size);

		node->left = copy(parent->left);
		node->right = copy(parent->right);

		return node;
	}


	void iterator_start(tree_node* parent, stack* iterator) {
		if (parent == nullptr) { return; }

		iterator->push(parent);
		iterator_start(parent->left, iterator);
	}

	hash_value strhash(void* t1, void* t2) {

		return strcmp((const char*)t2, (const char*)t1);
	}
}

