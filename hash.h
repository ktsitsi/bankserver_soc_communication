#include "datatypes.h"

class HashTable{								//Hashtable is a class for objects of hashtables
	List<ListNode>** bucket_list;				//contains a custom templated list of listnodes
	pthread_mutex_t* bucket_mtx;				//a mutex that locks each bucket of the hashtable
	int hash_entries;							//the preferable number of buckets consisting the Hashtable
	int hfunction(std::string client_name);		//private function that calculates the hash function for each inserted in the hash element
public:
	HashTable(int numofBuckets);				//Basic functions that handle the Hash Table - mutate-search-access
	~HashTable();
	ListNode* Find(std::string search_client);
	bool Insert(std::string new_client,int new_amount,int delay);
	void Remove(std::string rem_client);
	bool AddTransfer(std::string source_client,std::string dest_client,int trans_amount,int delay);
	bool MultiAddTransfer(std::string source_client,List<ClientNode>& clientList,int trans_amount,int delay);
	bool ErrorMultiPrint(List<ClientNode>& clientList,std::string &name_list);
	bool MultiPrint(List<ClientNode>& clientList,std::string &name_list);
	int PrintBalance(std::string source_client);
	void Print();
};
