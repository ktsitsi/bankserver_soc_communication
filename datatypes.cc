#include "datatypes.h"
#include <string>

/*-------------------ListNode Class Functions------------------*/

ListNode::ListNode(std::string cl_name,int amount_arg,bool dtype)
{
	prev = NULL;
	next = NULL;
	client_name = cl_name;
	amount = amount_arg;
	data_type = dtype;
	pthread_mutex_init(&reserve_mtx,NULL);
	if(!dtype)
		accepted_reserves = new List<ListNode>;
	else
		accepted_reserves = NULL;
}

ListNode::ListNode(std::string cl_name,bool dtype)
{
	prev = NULL;
	next = NULL;
	client_name = cl_name;
	amount = 0.0;
	data_type = dtype;
	pthread_mutex_init(&reserve_mtx,NULL);
	if(!dtype)
		accepted_reserves = new List<ListNode>;
	else
		accepted_reserves = NULL;
}

ListNode::ListNode(bool dtype)
{
	prev = NULL;
	next = NULL;
	amount = 0.0;
	client_name = "\0";
	data_type = dtype;
	pthread_mutex_init(&reserve_mtx,NULL);
	if(!dtype)
		accepted_reserves = new List<ListNode>;
	else
		accepted_reserves = NULL;
}
ListNode::~ListNode()
{
	pthread_mutex_destroy(&reserve_mtx);
	if(!data_type)
		delete accepted_reserves;

}
void ListNode::Lock(){
	pthread_mutex_lock(&reserve_mtx);
}

void ListNode::Unlock(){
	pthread_mutex_unlock(&reserve_mtx);
}

void ListNode::Decrement(int trans_amount)
{
	amount -= trans_amount;
}

void ListNode::Increment(int trans_amount)
{
	amount += trans_amount;
}

void ListNode::AddSender(ListNode& newtrans_client)
{
	accepted_reserves->Insert(newtrans_client);
}

void ListNode::Print()
{
	std::cout<< client_name <<":"<< amount <<std::endl;
}

ListNode* ListNode::GetNext()
{
	return next;
}

ListNode* ListNode::GetPrev()
{
	return prev;
}

int ListNode::GetAmount()
{
	return amount;
}

std::string ListNode::GetItem(){
	return client_name;
}

void ListNode::SetNext(ListNode* freshnx){
	next = freshnx;
}

void ListNode::SetPrev(ListNode* freshpr){
	prev = freshpr;
}

bool ListNode::operator==(const ListNode& d)
{
	if(client_name == d.client_name)
		return true;
	return false;
}

bool ListNode::operator>(const ListNode& d)
{
	if(client_name.compare(d.client_name)>0)
		return true;
	return false;
}

void ListNode::operator=(const ListNode& d)
{
	client_name = d.client_name;
	amount = d.amount;
}

/*-----------------------ClientNode Functions--------------------*/

ClientNode::ClientNode(std::string cl_name){
	client_name = cl_name;
}

ClientNode::ClientNode(){
	client_name ="\0";
}

std::string ClientNode::GetItem()
{
	return client_name;
}

ClientNode* ClientNode::GetNext(){
	return next;
}

ClientNode* ClientNode::GetPrev(){
	return prev;
}

void ClientNode::SetNext(ClientNode* freshnx)
{
	next = freshnx;
}

void ClientNode::SetPrev(ClientNode* freshpv)
{
	prev = freshpv;
}

bool ClientNode::operator==(const ClientNode& Cn)
{
	return client_name == Cn.client_name;
}

bool ClientNode::operator>(const ClientNode& d)
{
	if(client_name.compare(d.client_name)>0)
		return true;
	return false;
}
void ClientNode::operator=(const ClientNode& Cn)
{
	client_name = Cn.client_name;
}

void ClientNode::Print()
{
	std::cout<<"Client Name:"<<client_name<<std::endl;
}