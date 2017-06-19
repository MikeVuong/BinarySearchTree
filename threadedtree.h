#include <iostream>
using namespace std;

template <class T>
class ThreadedTree {
	struct Node {
		T data_;
		Node* left_;
		Node* right_;
		bool leftCheck , rightCheck; //Checks to see whether the node is threaded or not
		Node(const T& data = T{}, Node* left = nullptr, Node* right = nullptr, bool lbo = false, bool rbo = false)
		{
			data_ = data;
			left_ = left;
			right_ = right;
			leftCheck = lbo;
			rightCheck = rbo;
		}
	};
	Node* root;

public:
	class const_iterator {
	protected:
		Node* curr_;
		const_iterator(Node* c) { curr_ = c; }
	public:
		const_iterator() {
			curr_ = nullptr;
		}
		const_iterator operator++(int) { //Prefix
		
			const_iterator cnst = *this;
			operator++();
			return cnst;			
		}
		const_iterator operator--(int) { //Prefix
			const_iterator cnst = *this;
			operator--();
			return cnst;
		}
		const_iterator operator++() { //Postfix
		
			Node* tmp = this->curr_->right_;
			if (tmp != nullptr)
			{
				if (curr_->rightCheck == false) //If its threaded
				{
					while (tmp->leftCheck == false && tmp->left_ != nullptr) //If its not threaded
					{
						tmp = tmp->left_;
					}
					this->curr_ = tmp;
					return this->curr_;
				}
				else if (curr_->rightCheck == true)
				{
					this->curr_ = tmp;
					return this->curr_;
				}
			}
			
		}
		const_iterator operator--() { //Postfix

			Node* tmp = this->curr_->left_;
			if (tmp != nullptr)
			{
				if (curr_->leftCheck== false) //If its threaded
				{
					while (tmp->rightCheck == false && tmp->right_ != nullptr) //If its not threaded
					{
						tmp = tmp->right_;
					}
					this->curr_ = tmp;
					return this->curr_;
				}
				else if (curr_->leftCheck == true)
				{
					this->curr_ = tmp;
					return this->curr_;
				}
			}
		}
		const T& operator*() {
			return curr_->data_;
		}
		bool operator==(const const_iterator& rhs) const {
			return curr_ == rhs.curr_;
		}
		bool operator!=(const const_iterator& rhs) const {
			return curr_ != rhs.curr_;
		}
		friend class ThreadedTree;
	};
	class iterator :public const_iterator {
		iterator(Node* c) :const_iterator(c) {}
	public:
		iterator() :const_iterator() {}
		const T& operator*() {
			return this->curr_->data_;
		}
		iterator operator++(int) {
			const_iterator cnst = *this;
			operator++();
			return cnst;
		}
		iterator operator--(int) {
			const_iterator cnst = *this;
			operator--();
			return cnst;
		}
		//Goes through the list to find the next biggest number
		iterator operator++() {
			
			Node* tmp = this->curr_->right_;
			if (tmp != nullptr)
			{
				if (this->curr_->rightCheck == false) //If its threaded
				{
					while (tmp->leftCheck == false && tmp->left_ != nullptr) //If its not threaded
					{
						tmp = tmp->left_;
					}
					this->curr_ = tmp;
					return this->curr_;
				}
				else if (this->curr_->rightCheck == true)
				{
					this->curr_ = tmp;
					return this->curr_;
				}
			}
		}
		//Next smallest number in the list
		iterator operator--() {
			
			Node* tmp = this->curr_->left_;
			if (tmp != nullptr)
			{
				if (this->curr_->leftCheck == false) //If its threaded
				{
					while (tmp->rightCheck == false && tmp->right_ != nullptr) //If its not threaded
					{
						tmp = tmp->right_;
					}
					this->curr_ = tmp;
					return this->curr_;
				}
				else if (this->curr_->leftCheck == true)
				{
					this->curr_ = tmp;
					return this->curr_;
				}
			}
		}

		friend class ThreadedTree;
	};

	ThreadedTree() {
		root = nullptr;
	}

	iterator insert(const T& data) {
		Node* temp = root;
		Node* tempThread; //Temp thread for saving later
		Node* succThread = nullptr;
		Node* successor = temp;
		bool leftThread = false;
		bool rightThread = false;

		//If the tree is empty, create the new node as a root
		if (!root)
		{
			root = new Node(data);
			return iterator(root);
			
		}

		while (temp != nullptr) // Leaves loop after finding the end
		{
		
			successor = temp;

			//Checks for whether the node has threads in either left or right direction	
			if (successor->left_ == root) 
			{
				leftThread = true;
			}
			else if(successor->left_ != root)
			{
				leftThread = false;
			}
			
			if (successor->right_ == root)
			{
				rightThread = true;
			}
			else if (successor->right_ != root)
			{
				rightThread = false;
			}

			//For redirecting the thread to another node from successor
			if (temp->left_ == successor)
			{
				succThread = temp->left_;
				temp->left_ = nullptr;
				leftThread = true;
			}

			if (temp->right_ == successor )
			{
				succThread = temp->right_;
				temp->right_ = nullptr;
				rightThread = true;
			}

			if (temp->data_ < data)
			{
				//If it has a right thread remake to null for later use
				if (rightThread == true)
				{
					tempThread = successor->right_;
					temp->right_ = nullptr;
					temp->rightCheck = false;
				}

				//If the right side is a thread destroy and remake
				if (temp->rightCheck == true && rightThread == false) 
				{
					succThread = temp->right_;
					temp->right_ = nullptr;
				}
				temp = temp->right_;
			}
			else
			{
				//Checks for left threads, if there is change to nullptr for later use
				if (leftThread == true)
				{
					tempThread = successor->left_;
					temp->left_ = nullptr;
					temp->leftCheck = false;
				}
				if (temp->leftCheck == true && leftThread == false)
				{
					succThread = temp->left_;
					temp->left_ = nullptr;
				}
				temp = temp->left_;
			}

		}

		//Make new threads before replacing the old ones !!
		temp = new Node(data, nullptr, nullptr,false,false); //Makes new node in nullptr

		// Initial Left thread
		if (temp->data_ > successor->data_ && successor == root) 
		{
			temp->left_ = successor;
			temp->leftCheck = true;
		}
		// Initial Right thread
		else if (temp->data_ < successor->data_ && successor == root) 
		{
			temp->right_ = successor;
			temp->rightCheck = true;
		}
		//Threads that are not root
		else if (temp->data_ > successor->data_ && temp->leftCheck == false) 
		{
			temp->left_ = successor;
			temp->leftCheck = true;
		}
		else if (temp->data_  < successor->data_ && temp -> rightCheck == false)
		{
			temp->right_ = successor;
			temp->rightCheck = true;
		}
		//Creating threads back to root
		if (leftThread == true && succThread == nullptr && temp->data_ < successor->data_) 
		{
			temp->left_ = tempThread;
			temp->leftCheck = true;
		}
		//Creating threads back to root
		else if (rightThread == true && succThread == nullptr && temp->data_ > successor->data_) 
		{
			temp->right_ = tempThread;
			temp->rightCheck = true;
		}
		
		if (succThread != nullptr)
		{
			if (temp->data_ > successor -> data_)
			{
				if (temp->data_ > succThread->data_)
				{
					temp->right_ = root;
					temp->rightCheck = true;
				}
				else
				{
					temp->right_ = succThread;
					temp->rightCheck = true;
				}
			}
			else if (temp->data_ < successor->data_)
			{
				if (temp->data_ < succThread -> data_)
				{
					temp->left_ = root;
					temp->leftCheck = true;
				}
				else
				{
					temp->left_ = succThread;
					temp->leftCheck = true;
				}
			}
		}
		//Connects previous node to new node
		if (successor->data_ < data) 
		{
			successor->right_ = temp;
			successor->rightCheck = false;//Changing back cause thread turned into a child
		}
		else
		{
			successor->left_ = temp;
			successor->leftCheck = false;
		}

		return iterator(temp);
	}
	//Returns an iterator based off the number being sent in
	iterator find(const T& data) const {
		Node* tmp = root;
		while (tmp->data_ != data)
		{
			if (tmp->data_ > data)
			{
				tmp = tmp->left_;
			}
			else if (tmp->data_ < data)
			{
				tmp = tmp->right_;
			}
		}
		return iterator(tmp);
	}
	//Returns the smallest number in the list
	iterator begin() {
		if (root == nullptr)
		{
			return end();
		}
		Node* temp = root;
		while (temp->left_ != nullptr)
		{
			temp = temp->left_;
		}
		return iterator(temp);
	}
	//Returns the largest number in the list
	iterator end() {

		Node* tmp = root;

		if (tmp != nullptr)
		{
			while (tmp->right_ != nullptr)
			{
				tmp = tmp->right_;
			}
		}
		return iterator(tmp);
	}
	//Returns the smallest number in the list
	const_iterator begin()const 
	{
		if (root == nullptr)
		{
			return end();
		}
		Node* temp = root;
		while (temp->left_ != nullptr)
		{
			temp = temp->left_;
		}
		return iterator(temp);
	}
	//Returns the largest number in the list
	const_iterator end() const {
		Node* tmp = root;
		ThreadedTree<T>::iterator it = nullptr;
		while (tmp != nullptr)
		{
			tmp = tmp->right_;
		}
		it = tmp;
		return it;
	}
	//Destructor
	~ThreadedTree() {
		delete(root);
	}
};