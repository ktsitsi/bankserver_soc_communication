#include <iostream>
#include <cstdlib>

using namespace std;

template <class A_type>
class List{
private:
	A_type* first;					//A ListNode pointer in the beginning of the List
	A_type* current;				//A ListNode pointer to the current Node.

public:
	List();							//Constructor for the objeccts of the List Class
	~List();						//Destructor for the objects of the List Class
	void Insert(A_type& insnode);
	void Remove(A_type& rmnode);	
	bool Empty();					//Checks if the list is empty or not
	void Print();
	void ReversePrint();
	int Length();
	void Push(A_type& psnode);
	void Pop();
	void SetCurrentToStart();
	void SetCurrent(A_type* scurr);
	A_type* Find(A_type& fnode);	//Find the ListNode
	A_type* Head();					//Return the Head of the List
	A_type* Last();					//Return the Last Node of the List
	void Sort();
	A_type* GetCurrent();
	A_type* GetNext();				//Return the Next ListNode after the current
	A_type* GetPrev();				//Return the Previous ListNode before the current
};

/*--------------------Generic List Class Functions----------------------*/

template <class A_type> 
void List<A_type>::Sort(){
	A_type *before,*curr,*after;
	for(int i=0;i<(this->Length());i++)
	{

		curr = first;
		std::cout<<this->Length()<<std::endl;
		for(int j=0;j<(this->Length()-1-i);j++)
		{
			if((*curr) > (*(curr->GetNext())))
			{
				curr->Print();
				std::cout<<"SWAP WITH ";
				curr->GetNext()->Print();
				before = curr->GetPrev();
				after = curr->GetNext();
				if(before != NULL)
					before->SetNext(after);
				else
					first = after;
				after->SetPrev(before);
				curr->SetNext(after->GetNext());
				if(after->GetNext()!= NULL)
					(after->GetNext())->SetPrev(curr);
				curr->SetPrev(after);
				after->SetNext(curr);
			}
			else
			{
				curr->Print();
				std::cout<<"NOSWAP ";
				curr->GetNext()->Print();
				curr = curr->GetNext();
			}
		}
	}
	this->SetCurrentToStart();
  
}

template <class A_type> 
List<A_type>::List(){
	first = NULL;
	current = NULL;
}

template <class A_type>
List<A_type>::~List(){
	A_type* ret;
	while(first!=NULL){
		ret = first;
		first = ret->GetNext();
		if(first!=NULL){
			first->SetPrev(NULL);
		}
		current = first;
		ret->SetNext(NULL);
		ret->SetPrev(NULL);
		delete ret;
	}
}

template <class A_type>
bool List<A_type>::Empty(){
	if(first == NULL){
		return true;
	}
	return false;
}

template <class A_type>
void List<A_type>::Print(){
	A_type* iter = first;
	if(iter!=NULL){
		while(iter->GetNext()!=NULL){
			iter->Print();
			iter = iter->GetNext();
		}
		iter->Print();
	}
	else{
		cout<<"->Empty List!"<<endl;
	}
}

template <class A_type>
void List<A_type>::ReversePrint(){
	A_type* iter = this->Last();
	if(iter!=NULL){
		while(iter->GetPrev()!=NULL){
			iter->Print();
			iter = iter->GetPrev();
		}
		iter->Print();
	}
	else{
		cout<<"->Empty List"<<endl;
	}
}

template <class A_type>
int List<A_type>::Length(){
	A_type* iter = first;
	int length = 0;
	if(iter!=NULL){
		while(iter->GetNext()!=NULL){
			length+=1;
			iter = iter->GetNext();
		}
		length+=1;
	}
	else{
		return 0;
	}
	return length;
}

template <class A_type>
void List<A_type>::SetCurrentToStart(){
	current = first;
}

template <class A_type>
void List<A_type>::SetCurrent(A_type* scurr){
	current = scurr;
}

/*Stack Procedures*/
template <class A_type>
void List<A_type>::Push(A_type& psnode){
	A_type* pushed = new A_type();				//Constructing new node
	*pushed = psnode;							//Assign overload operator
	if(this->Empty())
	{
		first = pushed;				//member of class first is constructed
		current = first;
		first->SetNext(NULL);
		first->SetPrev(NULL);
	}
	else
	{
		pushed->SetPrev(NULL);
		pushed->SetNext(first);
		first->SetPrev(pushed);
		if(current == first)
			current = pushed;
		first = pushed;
	}
}

template <class A_type>
void List<A_type>::Pop()
{
	A_type* poped = first;
	first = poped->GetNext();

	if(first != NULL)
		first->SetPrev(NULL);
	current = first;
	poped->SetNext(NULL);
	poped->SetPrev(NULL);
	delete poped;
}

template <class A_type>
void List<A_type>::Insert(A_type& insnode)
{
	A_type* inserted = new A_type();
	*inserted = insnode;

	if(this->Empty())
	{
		first = inserted;
		first->SetNext(NULL);
		first->SetPrev(NULL);
		current = first;
	}
	else
	{							
		A_type* iter = first;
		while((iter->GetNext()) != NULL)
		{
			iter = iter->GetNext();
		}
		
		inserted->SetNext(NULL);
		inserted->SetPrev(iter);
		iter->SetNext(inserted);
	}
}
/* This Function returns true if the element 
 *that we are looking for in the list is found
 * and false if it is not found. */

template<class A_type>
A_type* List<A_type>::Find(A_type& search_node)
{
	if(this->Empty())
	{
		return NULL;
	}
	else
	{
		A_type* iter = first;
		do
		{
			if(*iter == search_node)
				return iter;
			else
				iter = iter->GetNext();

		}while(iter != NULL);
	}
	return NULL;
}

template<class A_type>
void List<A_type>::Remove(A_type& rmnode){
	/*Find the rmnode in the list and delete it according the 3 cases of being in the beginning at the end etc.*/
	if(!(this->Empty()))
	{
		A_type* found;
		if((found = this->Find(rmnode))!=NULL)
		{
			A_type* temp;
			if(found->GetPrev()==NULL && found->GetNext()!=NULL)
			{
				/*rm from front*/
				temp = found->GetNext();
				found->SetNext(NULL);
				found->SetPrev(NULL);
				temp->SetPrev(NULL);
				first = current = temp;
				delete found;
			}
			else if(found->GetPrev()!=NULL && found->GetNext()!=NULL)
			{
				/*RM FROM THE MIDDLE*/
				temp = found->GetPrev();
				temp->SetNext(found->GetNext());
				temp->GetNext()->SetPrev(temp);
				found->SetPrev(NULL);
				found->SetNext(NULL);
				current = first;
				delete found;
			}
			else if(found->GetPrev()!=NULL && found->GetNext()==NULL)
			{
				/*rm the last in the list*/
				temp = found->GetPrev();
				temp->SetNext(NULL);
				found->SetPrev(NULL);
				current = first;
				delete found;
			}
			else
			{
				/*rm the only element of the list*/
				first = NULL;
				current = first;
				delete found;
			}
		}
	}
}

template<class A_type>
A_type* List<A_type>::Head(){
	return first;
}

template<class A_type>
A_type* List<A_type>::Last(){
	if(!(this->Empty())){
		A_type* iter = first;
		while((iter->GetNext()) != NULL)
		{
			iter = iter->GetNext();
		}
		return iter;
	}
	else
		return NULL;
}

template<class A_type>
A_type* List<A_type>::GetCurrent(){
	return current;
}

template<class A_type>
A_type* List<A_type>::GetNext(){
	if(current->GetNext()!=NULL)
		current = current->GetNext();		//Think about this function
	return current;
}

template<class A_type>
A_type* List<A_type>::GetPrev(){
	if(current->GetPrev()!=NULL)
		current = current->GetPrev();
	return current;
}