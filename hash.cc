#include "hash.h"
#include <cstdlib>		//malloc
#include <cstring>		//string operations
#include <sstream>		//stringstream
#include <unistd.h>		//usleep

int HashTable::hfunction(std::string client_name)
{
	int hindex = 0;										//THe functionality of hash function is to calculate the summation
	for(int i=0; i<=(int)client_name.length();i++)		//of the letters of the enteres string and then according the mod
		hindex += client_name[i];						//with the number of buckets puts it in the appropriate bucket
	return hindex % hash_entries;
}

HashTable::HashTable(int number_of_buckets)
{
	if(number_of_buckets == 0)								//if the given argument is 0 buckets at leas one is constructed-list
	 	hash_entries= 1;
	else
		hash_entries = number_of_buckets;
	
	bucket_list = new List<ListNode>*[hash_entries];			//a list(buckets) of pointers that point to lists of the included elements 
	bucket_mtx = (pthread_mutex_t*)malloc(hash_entries*sizeof(pthread_mutex_t));		// for each bucket construct the mutex of the bucket

	for(int i=0; i<hash_entries; i++){
		bucket_list[i] = new List<ListNode>;
		pthread_mutex_init(&bucket_mtx[i],NULL);				//initialize bucket mutex
	}
}

HashTable::~HashTable()
{																//destructor of the hash table
	for(int i=0 ; i<hash_entries; i++)
		delete bucket_list[i];
	
	delete[] bucket_list;
}

void HashTable::Print()
{
	for(int i=0; i<hash_entries; i++)
	{
		cout<<"In the "<<i<<" bucket"<<endl;
		bucket_list[i]->Print();
	}
}

ListNode* HashTable::Find(std::string client_name){
	
	int hindex = hfunction(client_name);			//first the hash function is calculated on the search node
	ListNode search_node(client_name);
	ListNode* found = NULL;
	pthread_mutex_lock(&bucket_mtx[hindex]);		//a mutex locks the whole appropriate bucket
	found = bucket_list[hindex]->Find(search_node);	//and the list.Find returns a ListNOde that ahs value the string element
	pthread_mutex_unlock(&bucket_mtx[hindex]);		//after found unlocks the bucket
	if(found != NULL)
		return found;
	else
		return NULL;
}

bool HashTable::Insert(std::string new_client_name, int new_amount,int delay)
{
	int hindex = hfunction(new_client_name);
	ListNode search_node(new_client_name,new_amount);		/*Creation of the ListNode*/
	pthread_mutex_lock(&bucket_mtx[hindex]);				/*MUTEX ON BUCKET*/
	if(delay > 0){
		usleep(delay*1000);									//usleep is in microsec and we want mili
	}
	if(!bucket_list[hindex]->Empty())						//check if the list that we are searching is empty
	{
		if(bucket_list[hindex]->Find(search_node) == NULL)	//check if already this element exists in our structure
		{	/*Record already exists*/
			bucket_list[hindex]->Insert(search_node);		//if not insert in the appropriate bucket list
			pthread_mutex_unlock(&bucket_mtx[hindex]);		//and unlock the bucket mutex
			return true;
		}
		else
		{	
			pthread_mutex_unlock(&bucket_mtx[hindex]);		//unlock even if it already exists becaule lock has been occur
			return false;
		}
	}
	else
	{
		bucket_list[hindex]->Insert(search_node);				//if bucket list is empty then instant insert
		pthread_mutex_unlock(&bucket_mtx[hindex]);				/*UNLOCK MUTEX ON BUCKET*/
		return true;
	}
}

void HashTable::Remove(std::string client_name)					//Remove is a function that in the project has no use
{
	int hindex = hfunction(client_name);
	ListNode rem_node(client_name);

	if(bucket_list[hindex]->Find(rem_node))
		bucket_list[hindex]->Remove(rem_node);
	else
		cerr << "The account is not in the structure in order to be deleted!" << endl;
}

bool HashTable::AddTransfer(std::string source_client,std::string dest_client,int trans_amount,int delay)
{
	ListNode* found_source = Find(source_client);						//given two accounts check if they truly exist in the structure
	ListNode* found_dest = Find(dest_client);
	ListNode newtrans_client(source_client,trans_amount,1);				//used for add_transfer first find the source_client
	if(found_source == NULL || found_dest == NULL)
	{																	//next the dest client
		return false;
	}																	//lock the node of the source account (multi-locking feature)
	else
	{
		//Locks according strcmp
		if(source_client.compare(dest_client)<0)						//Sorted Mutex Locks
		{
			found_source->Lock();
			found_dest->Lock();
		}
		else
		{
			found_dest->Lock();
			found_source->Lock();
		}
		//Checks sufficient balance if true then transfer
		if(found_source->GetAmount() >= trans_amount)
		{
			//Sufficient balance
			found_source->Decrement(trans_amount);
			found_dest->Increment(trans_amount);
			if(delay > 0){
					usleep(delay*1000);
			}
			found_dest->AddSender(newtrans_client);
			found_source->Unlock();						//Lock has occure in the feasible function that executes always before the AddTrans
			found_dest->Unlock();
			return true;
		}
		else											//else unlock and return false
		{
			found_source->Unlock();
			found_dest->Unlock();
			return  false;
		}
	}
}

bool HashTable::MultiAddTransfer(std::string source_client,List<ClientNode>& clientList,int trans_amount,int delay)
{
	ListNode *found_source,*found_dest;
	ClientNode* current;
	ClientNode source_node(source_client);
	bool client_nexist = false;

	if((found_source= Find(source_client))!= NULL){
		clientList.SetCurrentToStart();
		do{
			current = clientList.GetCurrent();
			if((found_dest = Find(current->GetItem()))==NULL)
			{
				client_nexist = true;
				break;
			}
			clientList.GetNext();
		}while(current != clientList.Last());
		//All clients exist
		if(!client_nexist)
		{
			clientList.Insert(source_node);
			std::cout<<"||||||||"<<std::endl;
			clientList.Print();
			std::cout<<"||||||||"<<std::endl;
			clientList.Sort();
			/*Lock all the participants in the transaction in order smaller to greater*/
			std::cout<<"~~~~~~~"<<std::endl;
			clientList.Print();
			std::cout<<"~~~~~~~"<<std::endl;
			do{
				current = clientList.GetCurrent();
				found_dest = Find(current->GetItem());
				std::cout<<"LOCK3"<<std::endl;
				found_dest->Print();
				found_dest->Lock();
				clientList.GetNext();
			}while(current != clientList.Last());	
			//Check for sufficient balance of the source
			clientList.Remove(source_node);		//Remove now from list the source
			if(found_source->GetAmount() >= (trans_amount*clientList.Length()))
			{
				//Sufficient Amount
				if(delay>0)
				{
					usleep(delay*1000);
				}
				clientList.SetCurrentToStart();
				do
				{
					current = clientList.GetCurrent();
					found_dest = Find(current->GetItem());
					//found_dest->Print();
					found_source->Decrement(trans_amount);
					found_dest->Increment(trans_amount);
					found_dest->Unlock();
					//dest_found add in list
					clientList.GetNext();
				}while(current != clientList.Last());
				found_source->Unlock();
				return true;
			}
			else
			{
				//Insufficient Amount
				do
				{
					current = clientList.GetCurrent();
					found_dest = Find(current->GetItem());
					found_dest->Unlock();
					//dest_found add in list
					clientList.GetNext();
				}while(current != clientList.Last());
				found_source->Unlock();
				return false;
			}
		}
		else
		{
			//At least on destination doesn't exist
			return false;
		}
	}
	else
	{
		//Source doesn't exist
		return false;
	}
}

int HashTable::PrintBalance(std::string source_client){
	ListNode *found_source;
	if((found_source = Find(source_client))!= NULL)
	{
		int return_amount;
		std::cout<<"LOCK4"<<std::endl;
		found_source->Lock();
		return_amount=found_source->GetAmount();
		found_source->Unlock();
		return return_amount;
	}
	else
	{
		return -1;
	}
}

bool HashTable::MultiPrint(List<ClientNode>& clientList,std::string &name_list)
{
	ListNode *found_dest;
	ClientNode* current;
	std::stringstream ss;
	bool client_nexist = false;

	//Check if all are in the hash
	clientList.SetCurrentToStart();
	do{
		current = clientList.GetCurrent();
		if((found_dest = Find(current->GetItem()))==NULL)
		{
			client_nexist = true;
			break;
		}
		clientList.GetNext();
	}while(current != clientList.Last());
	//If they all exist in the hash
	if(!client_nexist)
	{
		//Sort for ordering lock
		clientList.Sort();
		/*Lock all the participants in the transaction in order smaller to greater*/
		do{
			current = clientList.GetCurrent();
			found_dest = Find(current->GetItem());
			std::cout<<"LOCK5"<<std::endl;
			found_dest->Lock();
			clientList.GetNext();
		}while(current != clientList.Last());
		clientList.SetCurrentToStart();
		do
		{
			current = clientList.Head();
			found_dest = Find(current->GetItem());
			ss << found_dest->GetItem()<<"/"<<found_dest->GetAmount()<<":"<<' ';	//name of the print
			found_dest->Unlock();
			clientList.Pop();
		}while(!clientList.Empty());
		name_list = ss.str();
	}
	return true;
}

bool HashTable::ErrorMultiPrint(List<ClientNode>& clientList,std::string &name_list)
{
	ClientNode* current;
	std::stringstream ss;
	do
	{
		current = clientList.Head();
		ss << current->GetItem()<<":"<<' ';	//name of the print
		clientList.Pop();
	}while(!clientList.Empty());
	name_list = ss.str();

	return true;
}