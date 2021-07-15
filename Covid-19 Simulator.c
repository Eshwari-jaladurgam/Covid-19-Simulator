#include <stdio.h>//Header files
#include <stdlib.h>
#include <time.h>
#define SIZE 10001//the max size of queue
#define MAX_VERTICES 10000//The max no.of vertices in the network
#define MAX_EDGES 3000//The max no.of edges in the network
int counter=0,suscept_counter=0,infect_counter=0,recover_counter=0;//initializing all counters to zero
// function for initializing the adjacency matrix
void graph_initialize(int **adjacency_matrix, int no_of_vertices)  
{
    for(int i=0; i<no_of_vertices; i=i+1)//the size of matrix is (no_of_vertices)x (no_of_vertices)
    {
        for(int j=0; j<no_of_vertices; j=j+1)
        {
            adjacency_matrix[i][j] = 0; // initially all values are zero
        }
    }
}
// function for creating the graph
void create_graph(int **adjacent_matrix, int no_of_vertices, int max_no_of_edges)  
{
    int vertex_counter,edge_counter,link_vertex;
    for(vertex_counter=0; vertex_counter < no_of_vertices; vertex_counter=vertex_counter+1) // counter for incrementing the vertices
    {
        for(edge_counter=0; edge_counter < max_no_of_edges; edge_counter=edge_counter+1)  // counter for incrementing the edges
        {
            if(rand()%2==1) //  if this condition satisfies, vertex has a edge
            {
                link_vertex = rand() % no_of_vertices;  // randomly generating the vertex for creating an edge between vertices
                adjacent_matrix[vertex_counter][link_vertex] = 1; // if there is an edge between the two vertices then make it 1
            }
        }
    }
}
//Functions for priority queue!!
int size = 0;//initial size of priority queue
struct pqueue//struct type data for priority queue
{   
    int day;// the time when the event occurs,considering as priority element
    int event_action;//the type of event(1 for transmission,2 for recovery)
    int num;//the concerned node
};
struct pqueue A[SIZE];//declaring the priority queue globally
void initialize_heap()//initializing the heap values with zeros
{   
    for(int i=0; i<SIZE; i++)
    {
        A[i].day = 0;
        A[i].event_action=0;
        A[i].num=0;
    }
}
void swap(int *a, int *b) {//swap function 
  int temp = *b;
  *b = *a;
  *a = temp;
}
// Function to heapify the tree(min)
void heapify( int size, int i) {
  if (size == 1) {
    return;
  } else {
    // Find the smallest among root, left child and right child
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < size && A[l].day < A[smallest].day)
      smallest = l;
    if (r < size && A[r].day < A[smallest].day)
      smallest = r;

    // Swap and continue heapifying if root is not smallest
    if (smallest != i) {
      swap(&A[i].day, &A[smallest].day);
      swap(&A[i].event_action, &A[smallest].event_action);
      swap(&A[i].num, &A[smallest].num);
      heapify( size, smallest);
    }
  }
}
// Function to insert an element into the tree
void insert(int n,int a,int node) {
  if (size == 0) {
    A[0].day=n;A[0].event_action=a;A[0].num=node;//creating a node
    size += 1;
    
  } else {
    A[size].day=n;A[size].event_action=a;A[size].num=node;//inserting at the end and then heapifying!
    size += 1;
    for (int i = size / 2 - 1; i >= 0; i--) {
      heapify( size, i);
    }
  }
}
// Function to delete an element from the tree
void deleteRoot() {
  A[0].day=A[size-1].day;//swapping with last element and remove node
  A[0].event_action=A[size-1].event_action;
  A[0].num=A[size-1].num;
  size -= 1;
  heapify( size, 0);//heapifying the array
}
//Functions for SIR !!
struct status{//struct for storing status of node and time
    int s;int t;
};
// function for storing the initial status of the vertexes
void initial_array(struct status*arr,int no_of_vertexes,int tmax){
    for(int i=0;i<no_of_vertexes;i++){
        arr[i].s=0;//(0 is susceptible,1 is infected,2 is recovered);intially all are susceptible
        arr[i].t=10*tmax;//vertex has some infinite time
    }
}
//function for initial infecteds and adding them to pqueue
//the pqueue gives the priority acc to  min-times(day) 
void intial_infects(struct status*arr,int no_of_infected_vertexes){
    for(int i=0;i<no_of_infected_vertexes;i++){
        arr[i].t=0;//infected ones event time is made zero
        insert(0,1,i);//adding them in pqueue
    }
}
//function for finding min-value of three values
int minimum(int source_time,int target_time,int tmax){
    if(source_time<target_time && source_time<tmax){
        return source_time;
    }
    else{
        if(target_time<tmax){
            return target_time;
        }
        else{
           return tmax; 
        }
    }
}
//function for coin-tossing-biased
int time_taken(float val){//float val as the probabilty of heads
    int temp=rand()%100;//generating random no.between 0 and 100
    int counter=1;
    while(temp>val*100){//condition for tail apperance & counting until head appears
        temp=rand()%100;
        counter++;
    }
    
    return counter;
}
//Function for the transmission to neighbours
void find_trans_SIR( struct status*arr,int event_time,float tp,int source,int target,int tmax){
    if (arr[target].s==0){//transmission only if the node is susceptible
        int inf_time=event_time+time_taken(tp);//the node gets affected after time_taken no. of days
        if(inf_time<minimum(arr[source].t,arr[target].t,tmax)){
            insert(inf_time,1,target);//inserting the new event to the queue
            arr[target].t=inf_time;//updating the time status of the target value
        }
    }
}
// Function for transmit event action (when node is susceptible and has the chance to transmit)
void process_trans_SIR(int**adjagency_matrix,struct status*arr,int value,int event_time,float tp,float rp,
    int *times_arr,int*suscept_arr,int*infect_arr,int*recover_arr,int tmax,int no_of_vertexes){
        times_arr[counter]=counter;
        suscept_arr[counter]=--suscept_counter;//reducing the susceptible count
        infect_arr[counter]=++infect_counter;//incrementing the infected 
        recover_arr[counter]=recover_counter;//the recovery remains same
        counter=counter+1;
        arr[value].s=1;//updating the status of the node to infected
        arr[value].t=event_time+time_taken(rp);//it gets recovered  after time_taken(rp) days
        if(arr[value].t<tmax){
            insert(arr[value].t,2,value);//inserting the event to the queue with event action as recovery
        }
        for(int adjacent_node=0;adjacent_node<no_of_vertexes;adjacent_node++){//checking for all neighbours(possibility of infection)
            if(adjagency_matrix[value][adjacent_node]==1){
                find_trans_SIR(arr,event_time,tp,value,adjacent_node,tmax);
            }
        }
}
//function for recovery of node 
void process_rec_SIR(struct status*arr,int value,int event_time,int*times_arr,int*suscept_arr,int*infect_arr,int*recover_arr){
    times_arr[counter]=counter;
    suscept_arr[counter]=suscept_counter;//count remains same
    infect_arr[counter]=--infect_counter;//removing from infected list hence decrementing
    recover_arr[counter]=++recover_counter;//adding to recover list hence incrementing
    counter=counter+1;
    arr[value].s=2;//updating status to recover( no more transmission)
}
void initialize_array(int *array, int tmax) // function for initializing the values of times,susceptible,infected and recovered array to 0
{
    for(int i=0;i<tmax;i=i+1)
    {
        array[i] = 0;
    }
}
void display_array(int *array, int tmax)  // function for displaying the times,susceptible,infected,recovered arrays
{
    for(int i=0; i<tmax; i=i+1)
    {
        printf("%d\n",array[i]);
    }
    printf("\n");
}
//MAIN 
int main(){
    int tmax=300;//perfoming the task for 300 days
    float tp=0.5,rp=0.2;//transmission rate & recovery rate
    srand ( time(NULL) );
    int numberOfVertices =0;
    while(numberOfVertices<1){//until we get verties greater than 1
    numberOfVertices=rand() % MAX_VERTICES;//randomly selecting no.of vertices
    }
    printf("Number of vertices: %d\n",numberOfVertices);
    srand ( time(NULL) );
    int maxNumberOfEdges = rand() % MAX_EDGES;//randomly selecting no.of edges 
    printf("Number of edges: %d\n",maxNumberOfEdges);
    //graph implentation using adjacency_matrix
    int **adjacency_matrix = (int**)malloc(numberOfVertices*sizeof(int *));//initializing the adjacency_matrix
    for (int i=0; i<numberOfVertices; i++)
            adjacency_matrix[i] = (int*)malloc(numberOfVertices*sizeof(int));  
            // adjacency matrix is a square matrix of size=no_of_vertexes
    graph_initialize(adjacency_matrix,numberOfVertices);  // initializing the adjacency matrix with all values to zero
    create_graph(adjacency_matrix,numberOfVertices,maxNumberOfEdges); //creating a graph 
    initialize_heap();//initializing priority queue
    int no_of_infected_vertexes=rand() % numberOfVertices;//inital infected veryices
    while(no_of_infected_vertexes == 0)
    {
        no_of_infected_vertexes = rand() % numberOfVertices;//randomly generating infected vertices
    }
    struct status*arr=(struct status*)malloc(sizeof(struct status)*numberOfVertices);//allocating memory for the status array
    initial_array(arr,numberOfVertices,tmax);//the array with susceptible nodes initially
    intial_infects(arr,no_of_infected_vertexes);//with initial infecteds
    int*times_arr=(int*)malloc(sizeof(int)*tmax);//initializing arr for day count
    initialize_array(times_arr,tmax);
    int*suscept_arr=(int*)malloc(sizeof(int)*tmax);//for susceptible node count
    initialize_array(suscept_arr,tmax);
    int*infect_arr=(int*)malloc(sizeof(int)*tmax);//for infected node count
    initialize_array(infect_arr,tmax);
    int*recover_arr=(int*)malloc(sizeof(int)*tmax);//for recover node count
    initialize_array(recover_arr,tmax);
    times_arr[0]=0;//0th day
    suscept_arr[0]=numberOfVertices;//all the vertices are initially susceptible
    infect_arr[0]=0;//none are infected
    recover_arr[0]=0;//none are recovered
    counter++;
    suscept_counter=numberOfVertices;
    int time_counter=1;//starting day counter as 1
    while(size!=0 && time_counter<tmax){// looping  till  queue is empty or max days are done!
        int event_time=time_counter;//considering the first node of queue
        int event_action=A[0].event_action;
        int value=A[0].num;
        deleteRoot();//deleting the root
        if(event_action==1){//Transmit action
            if(arr[value].s==0){//the node gets infected if it's susceptible
                process_trans_SIR(adjacency_matrix,arr,value,event_time,tp,rp,times_arr,suscept_arr,infect_arr,recover_arr,tmax,
                numberOfVertices);
            }
        }
        else{//recovery action
            process_rec_SIR(arr,value,event_time,times_arr,suscept_arr,infect_arr,recover_arr);
        }
        time_counter++;//day count incrementing
    }
    //Printing the time,S,I,R lists
    printf("Time\tS\tI\tR\n");
    for (int i = 0; i < counter; ++i)
    {
        printf("%d\t%d\t%d\t%d\n", times_arr[i], suscept_arr[i], infect_arr[i], recover_arr[i]);
    }
}
//the no.of recovery is coming 0  for higher vertices ,but sometimes it works;and correctly for smaller values! 





