#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>							//library declaration
#include <vector>
#include <utility>



class ServiceQueue {						//class delcaration

private:
	struct Node                             //structure declaration for a linked list ServiceQueue
	{
		Node* prev;
		Node* next;
		int data;
	};
	struct Bucket_Buzzer                    //structre declaration for a linked lsit for bucker buzzer
	{
		Bucket_Buzzer* next;
		int reusable;
	};
	Node *front;							//pointers declaration for Service Queue
	Node * back;

	Bucket_Buzzer *head;					//pointers declaration for Bucket Buzzer
	Bucket_Buzzer *tracker;

	int buzzer;								//variable declartaion
	int Length; 
	int bucket_buzzer;
	std::vector<Node*> the_queue;  // idx-0 is front of queue
	//std::vector<int> buzzer_bucket;  // keeps track of buzzer-IDs
									 //   that can be reused.  Operates
									 //   as a stack so buzzer that became
									 //   reusable most recently is on
									 //   top (i.e., at back of vector).

	/** Your private data members here!
	*   (you should have NO PUBLIC data members!
	*
	* Nested private types also go here.
	* For example, if your design needs some kind of
	*   structure, you would specify it here.
	*/


public:

	/**
	* Constructor
	* Description: intializes an empty service queue.
	*
	* RUNTIME REQUIREMENT: O(1)
	*
	* TODO
	*/
	ServiceQueue()                                 //constructor to initialize the vraibles and pointers
	{
		front = NULL;
		back = NULL;
		head = NULL;
		tracker = NULL;
		buzzer = 0;
		Length = 0;
	}

	/**
	* Destructor
	* Description:  deallocates all memory assciated
	*   with service queue
	*
	* RUNTIME REQUIREMENT:  O(N_b) where N_b is the number of buzzer
	*	IDs that have been used during the lifetime of the
	*	service queue; in general, at any particular instant
	*	the actual queue length may be less than N_b.
	*
	*	[See discussion of "re-using buzzers" below]
	*
	* TODO
	*/
	~ServiceQueue()                          //destructor to delete the entire linked list after use
	{
		Node* temp = front;
		Node *del;
		while (temp != NULL)
		{
			del = temp;
			temp = temp->next;
			delete temp;
		}
		Bucket_Buzzer *temporary = head;
		Bucket_Buzzer * Pointer;
		while (temporary != NULL)
		{
			Pointer = temporary;
			temporary = temporary->next;
			delete Pointer;
		}
	}

	/**
	* Function: snapshot()
	* param:  buzzers is an integer vector passed by ref
	* Description:  populates buzzers vector with a "snapshot"
	*               of the queue as a sequence of buzzer IDs
	*
	*
	* RUNTIME REQUIREMENT:  O(N)  (where N is the current queue
	*		length).
	*/
	void snapshot(std::vector<int> &buzzers)                  //snap shot function to copy the linked list in vector
	{
		buzzers.clear();   // you don't know the history of the 
						   //   buzzers vector, so we had better
						   //   clear it first
		//printf("its right here");
		Node* p1 = front;
		while (p1 != back->next)
		{
			buzzers.push_back(p1->data);
			p1 = p1->next;
		}		
	}

	/**
	* Function: length()
	* Description:  returns the current number of
	*    entries in the queue.
	*
	* RUNTIME REQUIREMENT:  O(1)
	*/
	int  length()                                    //length function to calculate the length of the function
	{

		return Length;   // placeholder

	}
	void PushBucketBuzzer(int number)					//push bucket buzzer function to insert unused buzzer in bucket
	{
		if (head == NULL)								//different test cases to satisfy the requirement
		{
			Bucket_Buzzer * temp = new Bucket_Buzzer;
			temp->reusable = number;
			temp->next = NULL;
			head = temp;
			///tracker = temp;
			//printf("temp value is : %d", temp->reusable);
		}
		else if (head->next == NULL)
		{
			Bucket_Buzzer * temp = new Bucket_Buzzer;
			temp->reusable = number;
			temp->next =head;
			head = temp;
			//printf("temp value is 1 : %d", temp->reusable);
		}
		else
		{
			Bucket_Buzzer * temp = new Bucket_Buzzer;
			temp->reusable = number;
			temp->next = head;
			//head->next = temp;
			head = temp;
			//printf("temp value is 2 : %d", temp->reusable);
		}
	}
	bool PopBucketBuzzer()								//pop buzzer function to pop value from the linked list
	{
		
		if (head == NULL)								//linked list test cases to satisfy requirement
		{
			return false;
		}
		else if (head->next == NULL)
		{
			//printf("temp value is 2 : %d", head->reusable);
			//printf("here null");
			bucket_buzzer = head->reusable;
			head = NULL;
			delete tracker;
			return true;
		}
		else
		{
			//printf("here 1 %d", head->reusable);
			bucket_buzzer = head->reusable;
			Bucket_Buzzer *del = head;
			head = head->next;
			delete del;
			return true;
		}
	}
	/**
	* Function: give_buzzer()
	* Return:   buzzer-ID (integer) assigned to the new customer.
	* Description:  This is the "enqueue" operation.  For us
	*    a "buzzer" is represented by an integer (starting
	*    from zero).  The function selects an available buzzer
	*    and places a new entry at the end of the service queue
	*    with the selected buzer-ID.
	*    This buzzer ID is returned.
	*    The assigned buzzer-ID is a non-negative integer
	*    with the following properties:
	*
	*       (1) the buzzer (really it's ID) is not currently
	*         taken -- i.e., not in the queue.  (It
	*         may have been in the queue at some previous
	*         time -- i.e., buzzer can be re-used).
	*	  This makes sense:  you can't give the same
	*	  buzzer to two people!
	*
	*       (2) Reusable Buzzers:  A re-usable buzzer is
	*	  a buzzer that _was_ in the queue at some previous
	*	  time, but currently is not.
	*
	*         REQUIREMENT:  If there is one or more reusable
	*         buzzer, you MUST return one of them; furthermore,
	*         it must be the buzzer that became reusable most
	*         MOST RECENTLY.
	*
	*       (3) if there are no previously-used / reusable buzzers,
	*         the SMALLEST possible buzzer-ID is used (retrieved from
	*         inventory).
	*	    Properties in this situation (where N is the current
	*	      queue length):
	*
	*		- The largest buzzer-ID used so far is N-1
	*
	*		- All buzzer-IDs in {0..N-1} are in the queue
	*			(in some order).
	*
	*		- The next buzzer-ID (from the basement) is N.
	*
	*    In other words, you can always get more buzzers (from
	*    the basement or something), but you don't fetch an
	*    additional buzzer unless you have to (i.e., no reusable buzzers).
	*
	* Comments/Reminders:
	*
	*	Rule (3) implies that when we start from an empty queue,
	*	the first buzzer-ID will be 0 (zero).
	*
	*	Rule (2) does NOT require that the _minimum_ reuseable
	*	buzzer-ID be used.  If there are multiple reuseable buzzers,
	*	any one of them will do.
	*
	*	Note the following property:  if there are no re-useable
	*	buzzers, the queue contains all buzzers in {0..N-1} where
	*       N is the current queue length (of course, the buzzer IDs
	*	may be in any order.)
	*
	* RUNTIME REQUIREMENT:  O(1)  ON AVERAGE or "AMORTIZED"
	*          In other words, if there have been M calls to
	*		give_buzzer, the total time taken for those
	*		M calls is O(M).
	*
	*		An individual call may therefore not be O(1) so long
	*		as when taken as a whole they average constant time.
	*
	*/
	int  give_buzzer()							//give buzzer function to give out buzzer
	{
		if (PopBucketBuzzer() == true)			//if the bucket has value
		{
			
			Node* temp = new Node;
			temp->data = bucket_buzzer;
			if (front == NULL)					//different test cases to add node into the list
			{
				temp->next = NULL;
				temp->prev = NULL;
				front = temp;
				back = temp;
				the_queue.resize(the_queue.size() + 1);
				the_queue[bucket_buzzer] = temp;
				Length++;
			}
			else
			{
				temp->next = NULL;
				temp->prev = back;
				back->next = temp;
				back = temp;
				the_queue.resize(the_queue.size() + 1);
				the_queue[bucket_buzzer] = temp;
				Length++;
			}
			return bucket_buzzer;
		}
		else
		{
			if (buzzer == 0)								//else statment to if the bucket does not have any elements. 
			{
				Node* temp = new Node;
				temp->data = buzzer;
				temp->next = NULL;
				temp->prev = NULL;
				front = temp;
				back = temp;
				the_queue.resize(the_queue.size() + 1);
				the_queue[buzzer] = (temp);
				Length++;
			}
			else if (buzzer == 1)
			{
				Node* temp = new Node;
				temp->data = buzzer;
				temp->next = NULL;
				temp->prev = back;
				front->next = temp;
				back = temp;
				the_queue.resize(the_queue.size() + 1);
				the_queue[buzzer] = (temp);
				Length++;

			}
			else
			{
				Node* temp = new Node;
				temp->data = buzzer;
				temp->next = NULL;
				temp->prev = back;
				back->next = temp;
				back = temp;
				the_queue.resize(the_queue.size() + 1);
				the_queue[buzzer] = (temp);
				Length++;
			}
			buzzer++;
			return buzzer - 1;
		}
		  // placeholder
	}

	/**
	* function: seat()
	* description:  if the queue is non-empty, it removes the first
	*	 entry from (front of queue) and returns the
	*	 buzzer ID.
	*	 Note that the returned buzzer can now be re-used.
	*
	*	 If the queue is empty (nobody to seat), -1 is returned to
	*	 indicate this fact.
	*
	* Returns:  buzzer ID of dequeued customer, or -1 if queue is empty.
	*
	* RUNTIME REQUIREMENT:  O(1)
	*/
	int seat()							//seat function to give seat
	{
		if (front == NULL)
		{
			return -1;
		}
		else
		{
			Node* del = front;			//to remove node from the linked list. 
			front = front->next;
			if (front == NULL)
			{
				PushBucketBuzzer(del->data);
				int value = del->data;
				delete del;
				Length--;
				return value;
			}
			else
			{
				PushBucketBuzzer(del->data);
				front->prev = NULL;
				int value = del->data;
				delete del;
				Length--;
				return value;
			}
		}// placeholder

	}


	/**
	* function: kick_out()
	*
	* description:  Some times buzzer holders cause trouble and
	*		a bouncer needs to take back their buzzer and
	*		tell them to get lost.
	*
	*		Specifially:
	*
	*		If the buzzer given by the 2nd parameter is
	*		in the queue, the buzzer is removed (and the
	*		buzzer can now be re-used) and 1 (one) is
	*		returned (indicating success).
	*
	* Return:  If the buzzer isn't actually currently in the
	*		queue, the queue is unchanged and false is returned
	*		(indicating failure).  Otherwise, true is returned.
	*
	* RUNTIME REQUIREMENT:  O(1)
	*/
	bool kick_out(int Buzzer)			//kick out function to kick the customer out of the queue
	{
		if (the_queue.size() < Buzzer)
		{
			return false;
		}
		Node * del = the_queue[Buzzer];
		PushBucketBuzzer(del->data);
		if ((the_queue[Buzzer]->next == NULL) && (the_queue[Buzzer] == back))
		{
			//printf("here %d", back->data);			//deleting the nodes from the list. 
			back = back->prev;
			back->next = NULL;
			del->prev = NULL;
			Length--;
			delete del;
			return true;
		}
		else if (the_queue[Buzzer] == front)
		{
			front = front->next;
			front->prev = NULL;
			Length--;
			delete del;
			return true;
		}
		else 
		{
			del->prev->next = del->next;
			del->next->prev = del->prev;
			del->next = NULL;
			Length--;
			del->prev = NULL;
			delete del;
			return true;
		}
		//printf("The que is here : %i",the_queue[2]->data);
		  // placeholder
	}

	/**
	* function:  take_bribe()
	* description:  some people just don't think the rules of everyday
	*		life apply to them!  They always want to be at
	*		the front of the line and don't mind bribing
	*		a bouncer to get there.
	*
	*	        In terms of the function:
	*
	*		  - if the given buzzer is in the queue, it is
	*		    moved from its current position to the front
	*		    of the queue.  1 is returned indicating success
	*		    of the operation.
	*		  - if the buzzer is not in the queue, the queue
	*		    is unchanged and 0 is returned (operation failed).
	*
	* Return:  If the buzzer isn't actually currently in the
	*		queue, the queue is unchanged and false is returned
	*		(indicating failure).  Otherwise, true is returned.
	*
	* RUNTIME REQUIREMENT:  O(1)
	*/
	bool take_bribe(int Buzzer)					//bribe function to to move customer forward. 
	{
		if (the_queue.size() < Buzzer)
		{
			return false;
		}
		Node * bribe = the_queue[Buzzer];		//linked list move customer forward. 
		if (the_queue[Buzzer] == back)
		{
			back = back->prev;
			bribe->next = front;
			front->prev = bribe;
			front = bribe;
			back->next = NULL;
			bribe->prev = NULL;
			return true;
		}
		else if (the_queue[Buzzer] == front)
		{
			return true;
		}
		else
		{
			Node *bribe_Next = the_queue[Buzzer]->next;
			bribe->next = front;
			bribe_Next->prev = bribe->prev;
			bribe_Next->prev->next = bribe_Next;
			bribe->prev = NULL;
			front->prev = bribe;
			front = bribe;
			return true;
		}
		
		return false;  // placeholder					//returning false if it is not ture. 
	}
};   // end ServiceQueue class

#endif
