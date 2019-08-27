# Hotel Queue Service
Used doubly linked list that behaves as a queue. This program has several functions which covers most of the real life scenarios that occure when the restaurant is full and you have to wait outside of the restaurant along with other people. 
Functions : 
int  length() - Returns the number of people waiting in the line. 
void PushBucketBuzzer(int number) - Adds one more to the list
bool PopBucketBuzzer() - Takes off one person from the list
int  give_buzzer()	 - Give the buzzer to the new person to wait in the line
int seat() - Takes off the buzzer from the list once the customer is seated
bool kick_out(int Buzzer) - Kicks out the people misbehaving while waiting
bool take_bribe(int Buzzer) - Help customers to go forward by giving bribe to the manager
