/*	CS302 Assignment 7
 *  Carlos Georges
 *	CPU Scheduler
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;
struct processType //declare custom type
{
	int id;
	int length;
	int burst;
 	int priority;
 	bool operator<(const processType &rhs) { //overloaded comparison operator
 		if ((length * priority) < (rhs.length *rhs.priority)) //we check the specified condition
			return true;
		return false; 	
 		} 
};

template <class Type> //class declaration
class binMinHeap
{
public:
binMinHeap (int cap =10); //default argument constructor
binMinHeap (const binMinHeap <Type>&); //copy constructor
~binMinHeap (); //destructor
const binMinHeap <Type>& operator=(const binMinHeap <Type>&); //assignment operator
void insert (const Type&); //insert in heap order
void deleteMin (); //pop off root
Type getMin () const ; //return root node
bool isEmpty () const ; //getters
size_t getSize () const ;
private:
void bubbleUp ( size_t ); //pushes smallest elements to their position
void bubbleDown ( size_t ); 
size_t capacity ; //paramenters
size_t size;
Type * heapArray;
};

template <class Type>
binMinHeap<Type>::binMinHeap(int cap) { //default constructor
	capacity = cap; //initializing
	size = 0;
	heapArray = new Type[capacity];
}

template <class Type>
binMinHeap<Type>::binMinHeap(const binMinHeap<Type>& copy) { //copy constructor, uses assignment operator
	*this = copy; //utilize the assignment operator
}

template <class Type>
binMinHeap<Type>::~binMinHeap() { //destructor
	capacity = size = 0; //basic deallocation
	delete [] heapArray;
	heapArray = nullptr;
}

template <class Type>
const binMinHeap<Type>& binMinHeap<Type>::operator=(const binMinHeap<Type>& rhs) {
	capacity = rhs.capacity; //copy values
	size = rhs.size; 
	heapArray = new Type[capacity]; //allocate an array
	for (int i = 0; i < capacity; ++i) //deep copy
	heapArray[i] = rhs.heapArray[i];
	
	return *this; //return the object
}

template <class Type>
void binMinHeap<Type>::insert(const Type& item) {
	
	size++; //make room
	heapArray[size] = item; //put element at the end
	bubbleUp(size); //bubble up

	//resize
	if(size+1 == capacity){ //if we're full
	Type* temp = new Type[capacity*2]; //new bigger array
	for (int i = 0; i < capacity; ++i) //deep copy
		temp[i] = heapArray[i];
	
	delete [] heapArray; //deallocate the old array
	heapArray = temp; //make heap array point to the new one
	temp = nullptr; //optional

	capacity = capacity*2; //resize our heap
	}
	
}

template <class Type>
void binMinHeap<Type>::deleteMin() {
	heapArray[1] = heapArray[size]; //put last element as root
	size--; //reduce size
	bubbleDown(1); //bubble down the root
} 

template <class Type>
Type binMinHeap<Type>::getMin() const {
	return heapArray[1];
}

template <class Type>
bool binMinHeap<Type>::isEmpty() const {
	return size == 0; 
}

template <class Type>
void binMinHeap<Type>::bubbleUp(size_t index) {
	
	//base case
	if(index<=1) //if we reached the root we're done
		return;

	int parent = index / 2; //parent position
	Type temp = heapArray[parent];//for swapping

	if(heapArray[index] < heapArray[parent]) { //if child is smaller, swap
		heapArray[parent] = heapArray[index];
		heapArray[index] = temp; //swapped
		bubbleUp(parent); //bubble up if necessary
	}
	else
		return;
}

template <class Type>
void binMinHeap<Type>::bubbleDown(size_t index) {
	Type temp = heapArray[index]; //for swapping
	int left = 2* index, right = 2*index +1; //positions of children

	if(left > size) //out of bounds base case
		return;

	//if parent is smaller than both children, we're good - base case
	if (heapArray[index] < heapArray[left] && heapArray[index] <heapArray[right]) 
		return;

	if (heapArray[left] < heapArray[right]) 
	{
		heapArray[index] = heapArray[left]; //swap left and parent since left < right
		heapArray[left] = temp; 
		bubbleDown(left); //bubble down further if necessary
	}

	else { //same logic for right side
		heapArray[index] = heapArray [right];
		heapArray[right] = temp;
		bubbleDown(right);
	}		
}

template <class Type>
size_t binMinHeap<Type>::getSize() const {
	return size; 
}
int main(int argc, char const *argv[])
{
	string filename, dummy;		//required variables
	fstream inFile;
	binMinHeap<processType> myHeap;
	processType curr;
	int time = 0;
	
	do { //input prompt
		cout << "Enter filename: ";
		cin >> filename;
		inFile.open(filename);
	} while(!inFile.is_open());

	
	getline(inFile, dummy); //get rid of header
	while(1) { //until the break statement inside is reached
		inFile >> curr.id; //fill all the fields
		inFile >> curr.length;
		inFile >> curr.burst;
		inFile >> curr.priority;

		myHeap.insert(curr); //put it in the heap

		if (inFile.eof()) //if the last line read was the eof then exit
		break;
	}
	binMinHeap<processType> secondHeap (myHeap); //test heaps
	binMinHeap<processType> thirdHeap = secondHeap;
	//processing algorithm
	//change myHeap in this block to second or third heap to test
	//copy constructor and assignment operator
	while(!myHeap.isEmpty()) { //until we've processed the whole list
	
	curr = myHeap.getMin(); //set the current to the highest priority

	if (curr.length>= curr.burst) { //if theres enough time to take out a burst, do it
		time += curr.burst;			//add the time of a burst
		curr.length -= curr.burst; 	//decrease remaining time by burst
		}

	else {		//if we reached here then burst was less than time remaining, so we take out time remaining
		time += curr.length; //increase time by the remaining length
		curr.length -= curr.length; //this ensures process is finished
		
	}

	if (curr.length == 0) //length will never be negative and always zero when finished
		cout << "Process " << curr.id << " finished at time " << time << "ms" << endl; //output
	
	myHeap.deleteMin(); //take off the most recently processed node

	if (curr.length!= 0) //and if it wasnt finished put it back
		myHeap.insert(curr);	
	}

	
	
	return 0;
}
