#include <iostream>
#include "list.h"
#include <pthread.h>

class ListNode{
	std::string client_name;								//ListNodes are the nodes of accounts in the hash
	pthread_mutex_t reserve_mtx;							//atomically mutexes on each listnode
	bool data_type;	
	int amount;												
	ListNode* next;											//A pointer to the next ListNode
	ListNode* prev;											//A pointer to the previous ListNode
	List<ListNode>* accepted_reserves;
public:
	ListNode(std::string client_name_arg,int amount_arg, bool dtype=false);		//Constructor of the ListNode
	ListNode(std::string client_name_arg,bool dtype = false);
	ListNode(bool dtype = false);								//Destructor of the ListNode
	~ListNode();
	ListNode* GetNext();									//Return the next of this ListNode
	ListNode* GetPrev();
	int GetClientId();									//Return the prev of this ListNode
	int GetAmount();										//Return the address of the Graph Node
	std::string GetItem();
	void Decrement(int trans_amount);
	void Increment(int trans_amount);
	void Lock();
	void Unlock();
	void AddSender(ListNode& newtrans_client);
	void Print();
	std::string GetClientName();									//Return the bank account of the ListNode
	bool operator==(const ListNode& Ln);					//Overload operator for equality
	void operator=(const ListNode& Ln);						//Overload operator for assignment
	bool operator>(const ListNode& d);
	void SetNext(ListNode* freshnx);
	void SetPrev(ListNode* freshpr);
};

class ClientNode{
	std::string client_name;
	ClientNode* next;
	ClientNode* prev;
public:
	ClientNode(std::string cl_name);
	ClientNode();
	std::string GetItem();
	ClientNode* GetNext();
	ClientNode* GetPrev();
	void SetNext(ClientNode* freshnx);
	void SetPrev(ClientNode* freshpv);
	bool operator>(const ClientNode& d);
	bool operator==(const ClientNode& Cn);
	void operator=(const ClientNode& Cn);
	void Print();
};