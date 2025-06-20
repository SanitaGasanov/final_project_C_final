#include "network.h"

///////////////////////////////
///////////	   A   ///////////
//////////////////////////////


//build - call from Main
EdgeList* build_net(int n)
{
	if (n <= 0)
	{
		printf("Invalid input.");
		exit(1);
	}
	int m;
	scanf("%d", &m); //input number of edges
	if (m < 0)
	{
		printf("Invalid input.");
		exit(1);
	}
	EdgeList* net = (EdgeList*)malloc(n * sizeof(EdgeList));  //allocation
	checkAllocation(net);
	nullInsertArray(net, n);
	BuildNetwork(net, n, m);
	return net;
}
///////////////////////////////////////////////////////////////////////////////////

void checkAllocation(void* arr)
{
	if (!arr)
	{
		exit(1);
	}
}
///////////////////////////////////////////////////////////////////////////////////
//init each cell with NULL
void nullInsertArray(EdgeList* net, int n)
{
	for (int i = 0; i < n; i++)
	{
		net[i].head = NULL;
		net[i].tail = NULL;
	}
}
////////////////////////////////////////////////////////////////////////////////////
//input validation
bool validInput(int* a, int* b, int* price, int n)
{
	if (scanf("%d %d %d", a, b, price) != 3)
	{
		printf("Invalid input.\n");
		exit(1);
	}
	if (*a < 0 || *a >= n || *b < 0 || *b >= n || *price < 0)
	{
		printf("Invalid input.\n");
		exit(1);
	}
	return true; //
}

///////////////////////////////////////////////////////////////////////////////////
//build the network 
void BuildNetwork(EdgeList* net, int n, int m)

{
	int a, b, price;
	for (int i = 0; i < m; i++) //triplets input
	{

		validInput(&a, &b, &price, n);
		updateList(&net[a], b, price);
		updateList(&net[b], a, price);

	}
}

//////////////////////////////////////////////////////////////////////////////
//helper for updatelist - create node
EdgeNode* BuildNode(int neighbor, int cost)
{

	EdgeNode* newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
	checkAllocation(newNode);

	newNode->e.neighbor = neighbor;
	newNode->e.cost = cost;
	newNode->next = NULL;

	return newNode;
}
/////////////////////////////////////////////////////////////////////////////
//insert node at beginning of the list
void addFirst(EdgeList* list, EdgeNode* node)
{
	node->next = list->head;
	list->head = node;

	if (list->tail == NULL) // if list was empty, update tail
		list->tail = node;
}
///////////////////////////////////////////////////////////////////////////
// insert node at end of the list
void addLast(EdgeList* list, EdgeNode* node)
{
	node->next = NULL;

	if (list->head == NULL) //list is empty
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		list->tail = node;
	}
}
////////////////////////////////////////////////////////////////////////
// insert node after the given previous node
void addMiddle(EdgeNode* prev, EdgeNode* node)
{
	node->next = prev->next;
	prev->next = node;

}
///////////////////////////////////////////////////////////////////////////
//update list: insert new edge, avoid duplicates
void updateList(EdgeList* list, int b, int price)
{

	//check if the neighbor already exists in the list
	EdgeNode* curr = list->head;
	while (curr != NULL)
	{
		if (curr->e.neighbor == b) //edge already exists
		{
			if (curr->e.cost != price) //if other price, update it
			{
				curr->e.cost = price;
			}
			return; //exit – don't insert duplicate
		}
		curr = curr->next;
	}

	//if not found – insert it in sorted position


	if (list->head == NULL) //empty list
	{
		EdgeNode* connectedComp = BuildNode(b, price);
		list->head = connectedComp;
		list->tail = connectedComp;
	}
	else //list not empty – find correct position
	{

		if (b < list->head->e.neighbor) // Insert at start
		{
			EdgeNode* connectedComp = BuildNode(b, price);
			addFirst(list, connectedComp);
		}
		else if (b > list->tail->e.neighbor) // Insert at end
		{

			EdgeNode* connectedComp = BuildNode(b, price);
			addLast(list, connectedComp);
		}
		else // Insert in middle
			{
		    bool inside = false;
			EdgeNode* current = list->head;
			while (current->next != NULL)
			{
				if (b > (current->e.neighbor) && b < (current->next->e.neighbor))
				{
					EdgeNode* connectedComp = BuildNode(b, price);
					addMiddle(current, connectedComp);
					inside = true;
					break;
				}
				current = current->next;
			}
			
			if(!inside) //if the Node didn't get in, insures to add the node
			{
			    EdgeNode* connectedComp = BuildNode(b, price);
		    	addLast(list, connectedComp);
			}
		}
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////
///////////	    B   ///////////
//////////////////////////////


////////////////////////////////////////////////////////////////////////
/////////////////////main function of #2////////////////////////////////
///////////////////////////////////////////////////////////////////////

int* build_prim_tree(EdgeList net[], int n)
{
	int* prim_arr;
	int* min_arr;
	CandidateList* priority_list;
	CandidateNode** location_arr;
	BYTE* InT_arr;
	int u;

	prim_arr = CreatePrimArr(n);   //we set and create in arrs the default parameters
	min_arr = CreateMinArr(n);
	////////////
	priority_list = CreatePrioriryList(n);             //we build priority list and according that we create the location arr
	location_arr = CreateLocationArr(priority_list, n);
	/////////////
	InT_arr = CreateInTArr(n);          //we set and create in InT arr false
	////////////////
	int v0 = 0;   //we set according the settings that v0=0.
	//////////////
	*(min_arr + v0) = 0;
	*(prim_arr + v0) = -1;
	//////////////
	while (priority_list->head->next != priority_list->tail)   //when priority list is not empty- head and tail are dummy nodes
	{
		u = DeleteMin(priority_list);         //is it correct? //we always recieve 0 in begining couse all min in priority are int_max
		if (*(min_arr + u) == INT_MAX)
		{
			FreeMeomery(priority_list, min_arr, InT_arr, location_arr);
			FreeNetArray(net, n);
			free(prim_arr);

			printf("No spanning tree available.");
			exit(1);
		}
		else
		{
			SetBitInInTArr(InT_arr, u);   //turn on the bit
		}
		int price_of_link;                //parameter for price
		bool if_bit_on;                  //parameter for bit
		for (int v = 0; v < n; v++)     // v is computer 
		{
			price_of_link = cost(net, u, v);
			if_bit_on = CheckBitInInTArr(InT_arr, v);

			if ((!if_bit_on) && (price_of_link < (*(min_arr + v))))  // if v not in prim and price link<min v
			{
				*(min_arr + v) = price_of_link;
				*(prim_arr + v) = u;
				DecreaseKey(location_arr, v, price_of_link);
			}
		}
	}
	FreeMeomery(priority_list, min_arr, InT_arr, location_arr);
	FreeNetArray(net, n);
	return prim_arr;
}
//////////////////////////////////////////////////////////////////////                    
////////////////////////create min arr////////////////////////////////
int* CreateMinArr(int num_of_computers)                   //arr num 2
{
	int i = 0;  //counter
	int* min_arr;

	min_arr = (int*)malloc(sizeof(int) * num_of_computers);
	checkAllocation(min_arr);

	while (i < num_of_computers)
	{
		*(min_arr + i) = INT_MAX;   //It's a default parameter
		i++;
	}
	return min_arr;
}
//////////////////////////////////////////////////////////////////////                    
////////////////////////create min arr////////////////////////////////
int* CreatePrimArr(int num_of_computers)                   //  arr ezer for prim
{
	int i = 0;  //counter
	int* prim_arr;

	prim_arr = (int*)malloc(sizeof(int) * num_of_computers);
	checkAllocation(prim_arr);

	while (i < num_of_computers)
	{
		*(prim_arr + i) = (-1);   //It's a default parameter
		i++;
	}

	return prim_arr;
}
//////////////////////////////////////////////////////////////////////                     
////////////////////////create InT arr////////////////////////////////
BYTE* CreateInTArr(int num_of_computers)
{
	BYTE* bits_arr_result;
	int bytes_of_arr;   /// how many bytes in arr

	if (num_of_computers % 8 == 0)   //calculate the bytes
	{
		bytes_of_arr = num_of_computers / 8;
	}
	else
	{
		bytes_of_arr = (num_of_computers / 8) + 1;
	}

	bits_arr_result = (BYTE*)calloc(bytes_of_arr, sizeof(BYTE));   //set meomery
	checkAllocation(bits_arr_result);

	return bits_arr_result;
}
//////////////////////////////////////////////////////////////////////                  
////////////////////turn on bit in Int arr////////////////////////////
void SetBitInInTArr(BYTE* bits_arr, int v)
{
	int my_byte = v / 8;     //which byte   
	int my_bit = v % 8;     //which bit
	unsigned char mask = 1;// its like 00000001 -  in byte we have 8 bits
	mask = mask << my_bit;// change num 1 acoording my bit

	*(bits_arr + my_byte) = *(bits_arr + my_byte) | mask;  //change bit to 1 
}
///////////////////////////////////////////////////////////////////////                                     
////////////////////////if bit if 1 or false //////////////////////////
bool CheckBitInInTArr(BYTE* bits_arr, int v)
{
	int my_byte = v / 8;     //which byte   
	int my_bit = v % 8;     //which bit
	unsigned char mask = 1;// its like 00000001 -  in byte we have 8 bits
	mask = mask << my_bit;// change num 1 acoording my bit

	return (bits_arr[my_byte] & mask) != 0;  //chacking with the mask if 1 or 0- if 0 false if 1 true
}

//////////////////////////////////////////////////////////////////////                    
///////////////////////////create new node////////////////////////////
CandidateNode* CreateNode(int computer, int min)
{
	CandidateNode* new_node;
	new_node = (CandidateNode*)malloc(sizeof(CandidateNode) * 1);
	checkAllocation(new_node);

	new_node->c.computer = computer;
	new_node->c.min = min;
	new_node->next = NULL;
	new_node->prev = NULL;

	return new_node;
}
//////////////////////////////////////////////////////////////////////                     need to free                     
////////////////////////make empty list///////////////////////////////
CandidateList* CreateEmptyList()
{
	CandidateList* my_list;

	my_list = (CandidateList*)malloc(sizeof(CandidateList) * 1);
	checkAllocation(my_list);

	my_list->head = CreateNode(-2, -2); // set values for dummy nodes (-2 cant be num of computer or cost)  
	my_list->tail = CreateNode(-2, -2); //carete the empty priority list
	my_list->head->next = my_list->tail;
	my_list->tail->prev = my_list->head;

	return my_list;

}
//////////////////////////////////////////////////////////////////////                   
////////////////////////create priority list//////////////////////////
CandidateList* CreatePrioriryList(int num_of_computers)
{
	CandidateList* my_list;
	my_list = CreateEmptyList();

	CandidateNode* prev_node = my_list->head;
	CandidateNode* new_node;
	int i = 0;

	while (i < num_of_computers)
	{

		new_node = CreateNode(i, INT_MAX);
		prev_node->next = new_node;
		new_node->prev = prev_node;
		prev_node = new_node;
		i++;
	}
	prev_node->next = my_list->tail;             /////////importent//////////////
	my_list->tail->prev = prev_node;

	return my_list;
}
//////////////////////////////////////////////////////////////////////                                     
////////////////////////build location arr///////////////////////////
CandidateNode** CreateLocationArr(CandidateList* priority_list, int num_of_computers)
{
	CandidateNode** location_arr;
	CandidateNode* new_pointer = priority_list->head->next;  //because head is dummy node
	int i = 0;

	location_arr = (CandidateNode**)malloc(sizeof(CandidateNode*) * num_of_computers);
	checkAllocation(location_arr);

	while (i < num_of_computers)              //num_of_computers = lenght of priority list. so ew didnt do double check
	{
		*(location_arr + i) = new_pointer;
		new_pointer = new_pointer->next;
		i++;
	}

	return location_arr;
}
//////////////////////////////////////////////////////////////////////                                      
////////////////////////Decrease min in min arr///////////////////////
void DecreaseKey(CandidateNode** location_arr, int v, int new_min)
{
	CandidateNode* my_pointer = *(location_arr + v);    // get relevant address of computer in priority list
	my_pointer->c.min = new_min;                       //change to new min cost
}
//////////////////////////////////////////////////////////////////////                                     
/////////Delete node with min cost from list+return computer ////////         /// i need to chack this is before the change
int DeleteMin(CandidateList* priority)
{
	CandidateNode* currnt_node = priority->head->next;    //secound node in list because head is dummy
	CandidateNode* node_to_delete = priority->head->next;

	CandidateNode* prev_node = priority->head;          // prev and next nodes to chang list
	CandidateNode* next_node = currnt_node->next;

	int v = currnt_node->c.computer;                      // computer of min cost to begining
	int min_cost = currnt_node->c.min;                  // min cost of first current node to begining

	while (currnt_node != priority->tail)       // until one node before tail because tail is dummy
	{

		if ((currnt_node->c.min < min_cost) && (currnt_node->c.min != INT_MAX))   // if min in current node is the min until now in list
		{
			min_cost = currnt_node->c.min;   //update node to delete, v, and min cost
			v = currnt_node->c.computer;
			node_to_delete = currnt_node;
		}
		currnt_node = currnt_node->next;
	}

	prev_node = node_to_delete->prev;      //after we get node to delete- set prev and next nodes
	next_node = node_to_delete->next;

	prev_node->next = next_node;         // link prev and next nodes - delete from priority
	next_node->prev = prev_node;

	free(node_to_delete);            //free node address from meomery 

	return v;
}
///////////////////////////////////////////////////////////////////////                                     
/////////////////return the cost between conputers ///////////////////
int cost(EdgeList Net[], int u, int v)
{
	EdgeNode* current = Net[u].head;
	while (current != NULL)
	{
		if (current->e.neighbor == v)
			return current->e.cost; //return the price

		current = current->next;
	}
	return INT_MAX; //if there is no connection 
}
///////////////////////////////////////////////////////////////////////                                     
/////////////////////////free all meomery ////////////////////////////
void FreeMeomery(CandidateList* priority_list, int* min_arr, BYTE* InT_arr, CandidateNode** location_arr)
{

	CandidateNode* current_node = priority_list->head->next;
	CandidateNode* to_delete_node;

	while (current_node != priority_list->tail) // if we in condition in the while loop in the main
	{
		to_delete_node = current_node;
		current_node = current_node->next;
		free(to_delete_node);
	}

	free(priority_list->head);   //its the dummy nodes of list
	free(priority_list->tail);  //we end the loos and its mean that we have only dummy head and tail	 
	free(priority_list);       //free meomery of list

	free(min_arr);
	free(InT_arr);
	free(location_arr);
}
///////////////////////////////////////////////////////////////////////                                     
/////////////////////free arr net - the input /////////////////////////
void FreeNetArray(EdgeList* net, int n)
{
	for (int i = 0; i < n; i++)  // run on all computers
	{
		EdgeNode* current = net[i].head; //get the list of curr comp
		while (current)
		{
			EdgeNode* temp = current;
			current = current->next;
			free(temp);
		}
	}
	free(net); //free the array
}
////////////////////////////////////////////////////////////////////////




///////////////////////////
/////////  C   ////////////
//////////////////////////

/////////////////////////////////////////////////////////////////////////

EdgeList* build_paths(int* Prim, int n)
{

	EdgeList* PrimPaths = (EdgeList*)malloc(n * sizeof(EdgeList));
	checkAllocation(PrimPaths);
	nullInsertArray(PrimPaths, n); //puts NULL in PrimPaths lists

	for (int v = 0; v < n; v++) //run on all computers
	{
		int u = Prim[v]; //the parent
		if (u != -1)  //if its not root -
		{
			//add to PrimPaths

			updateList(&PrimPaths[u], v, 0);
			updateList(&PrimPaths[v], u, 0);
		}
	}
	free(Prim);
	return PrimPaths;
}
//////////////////////////////////////////////////////////////////////////
 

//////////////////////////
//////////// D //////////////
////////////////////////////


//////////////
#define WHITE 0
#define GRAY 1
#define BLACK 2

////////////////////////////////////////////////////////////////////////////////////////
void find_and_print_path(EdgeList* PrimPath, int n, int first, int last)
{
	bool valid = validComptoConnect(first, last, n);
	if (!valid)
	{
		printf("Invalid input.");
		exit(1);
	}
	
	int* color = createColorArray(n);
	int* parent = createParentArray(n);

	FindPath(first, last, PrimPath, color, parent);
	PrintPath(last, first, parent);
	FreeMeomeryFuncD(PrimPath, n, color, parent);
}


////////////////////////////////////////////////////////////////////
int* createColorArray(int n)
{
	int* color = (int*)malloc(n * sizeof(int));
	checkAllocation(color);

	for (int i = 0; i < n; i++)
		color[i] = WHITE;

	return color;
}
////////////////////////////////////////////////////////////////////
int* createParentArray(int n)
{
	int* parent = (int*)malloc(n * sizeof(int));
	checkAllocation(parent);

	for (int i = 0; i < n; i++)
		parent[i] = -1;

	return parent;
}
////////////////////////////////////////////////////////////////////
void FindPath(int first, int last, EdgeList* PrimPath, int* color, int* parent)
{
	color[first] = GRAY; //put status GRAY

	EdgeList currentList = PrimPath[first];
	EdgeNode* currentNode = currentList.head;

	for (; currentNode != NULL; currentNode = currentNode->next)
	{
		int v = currentNode->e.neighbor; //get the computer

		if (color[v] == WHITE) //if its still white
		{
			parent[v] = first;
			if (v == last)
				return;
			else
				FindPath(v, last, PrimPath, color, parent); //rec call
		}
	}
	color[first] = BLACK; //finish and put status BLACK
}
////////////////////////////////////////////////////////////////////////////////////////////
void PrintPath(int v, int first, int* parent)
{
	if (v == first)
	{
		printf("%d ", v);
	}
	else
	{
		PrintPath(parent[v], first, parent);
		printf("%d ", v);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////
void FreeMeomeryFuncD(EdgeList* PrimPath, int n, int* color, int* parent)
{
	free(color);
	free(parent);
	FreeNetArray(PrimPath, n);
}
//////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

bool validComptoConnect(int first, int last, int n)
{

	if (first < 0 || first >= n || last < 0 || last >= n )
	{
		return false;
	}
	return true;
}
