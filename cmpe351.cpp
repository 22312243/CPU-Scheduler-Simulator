/* This is code developed by Aya Ouriad */
#include "cmpe351.h"
#include <iostream>  
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
using namespace std;

//Process structure 
struct Process {
    int procId;
    int execTime;  
    int priority;
    int arrival;
};

struct Node {
    Process procData;
    int waitVal;
    int isDone;
    Node* nextNode;
};
/*create one node first*/
Node* createNode(Process p){
    Node* nodePtr = new Node;
    nodePtr->procData = p;
    nodePtr->waitVal = 0;
    nodePtr->isDone = 0;
    nodePtr->nextNode = nullptr;
    return nodePtr;
}
/*add a node at the end*/
void addEnd(Node** headRef, Process p){
    Node* newNode = createNode(p);
    if(*headRef == NULL){
        *headRef = newNode;
        return;
    }

    Node* curNode = *headRef;
    while (curNode->nextNode != NULL)
    curNode = curNode->nextNode;
    curNode->nextNode = newNode;
}
/* count number of nodes*/
int count(Node* headNode){
    int counter = 0;
    while (headNode != NULL){
        counter++;
        headNode = headNode->nextNode;
    }
    return counter;
}
/*find node with given procId*/
Node* find(Node* headNode, int procId){
    while(headNode != NULL) {
        if (headNode->procData.procId == procId)
        return headNode;
    headNode = headNode->nextNode;
    }
    return NULL;
}
/* make a complete copy of the list */
Node* copy(Node* origNode) {
    if (origNode == NULL) return NULL;

    Node* newHead = createNode(origNode->procData);
    newHead->isDone = origNode->isDone;
    newHead->waitVal = origNode->waitVal;
    Node* tailNode = newHead;
    origNode = origNode->nextNode;

    while(origNode != NULL){
        Node* tmpNode = createNode(origNode->procData);
        tmpNode->isDone = origNode->isDone;
        tmpNode->waitVal = origNode->waitVal;

        tailNode->nextNode = tmpNode;
        tailNode = tmpNode;
        origNode = origNode->nextNode;
    }
    return newHead;
}

void freeAll(Node* headNode){
    Node* tmpNode;
    while(headNode != NULL){
        tmpNode = headNode;
        headNode = headNode->nextNode;
        delete tmpNode;
    }
}
struct Queue {
    int queueId;
    Node* headProcNode;
    Queue* nextQueue;
};
Queue* getQueue(Queue** listHead, int wantedId){
    Queue* qPtr = *listHead;
    Queue* prevPtr = NULL;

    while(qPtr != NULL && qPtr->queueId != wantedId) {
        prevPtr = qPtr;
        qPtr = qPtr->nextQueue;
    }
    if(qPtr != NULL) return qPtr;
    /*create new queue*/
    Queue* freshQueue = new Queue;
    freshQueue->queueId = wantedId;
    freshQueue->headProcNode = NULL;
    freshQueue->nextQueue = NULL;
    if(prevPtr == NULL)
    *listHead = freshQueue;
else 
    prevPtr->nextQueue = freshQueue;
return freshQueue;
}
int readInput(const char* fileName, Queue** queueHead){
    ifstream file(fileName);
    if(!file){
        cout<<"Error: cannot open "<<fileName<<'\n';
        return 0;
    }
    int procCounter = 0;
    int execTime, priority, arrival, queueId;
    char ch;

    while(file >> ch)
    { 
        /*execTime*/
        execTime= 0;
        while(ch!= ':'&& file){
            execTime = execTime * 10 + (ch - '0');
            file.get(ch);

        }
        /*priority*/
        priority = 0;
        file.get(ch);
        while(ch != ':'&& file){
            priority = priority * 10 + (ch - '0');
            file.get(ch);
        }
        /*arrival*/
        arrival = 0;
        file.get(ch);
        while(ch != ':'&& file){
            arrival = arrival * 10 + (ch - '0');
            file.get(ch);
        }
        /*queueId*/
        queueId = 0;
         file.get(ch);
        while(ch != '\n'&& file){
          queueId = queueId * 10 + (ch - '0');
            file.get(ch);}
        Process p;
        p.procId = procCounter++;
        p.execTime = execTime;
        p.priority = priority;
        p.arrival = arrival;

        Queue* q = getQueue(queueHead, queueId);
        addEnd(&q->headProcNode, p);
        }
return 1;
    
}

// First Come First Serve scheduling
void fcfs(Queue* q, ofstream& outFile)
{
    int currTime=0; //keeping track of CPU time 
    Node* cur = q->headProcNode; //start with the first process in the queue
    double totalWait=0;  //sum of all waiting times
    int processCounter=0;  // number of processes

    outFile <<q->queueId<<":1";
    cout <<q->queueId<<":1";

// go through each process in the queue
    while(cur != NULL)
    {
        // if CPU is idle, jump to the arrival time of this process
        if(currTime < cur->procData.arrival){
            currTime = cur->procData.arrival;
        }
       //waiting time = time when CPU starts - arrival time
    int waitTime = currTime - cur->procData.arrival;
    //write waiting time to file and screen 
    outFile << ":"<<waitTime;
    cout << ":"<< waitTime;

    //update totals
    totalWait += waitTime;
    processCounter++;

    currTime += cur->procData.execTime;
    //go to next process
    cur = cur->nextNode;
}
  //average waiting time
  double averageWait;
  if(processCounter ==0){
    averageWait = 0.0;
  }else{
    averageWait = totalWait / processCounter;
  }

  outFile << ":"<<averageWait<<"\n";
  cout << ":"<<averageWait<<"\n";

}
void resetFlags(Node* head){
    for (Node* n = head; n; n = n->nextNode) {
        n->isDone = 0;
    }
}

void sjf(Queue* q, ofstream& outFile){
    int time = 0;
    int doneJobs = 0;
    int totalJobs = count(q->headProcNode);
    double totalWait = 0;
    
    // queue id (2 = SJF)
    outFile << q->queueId << ":2";
    cout << q->queueId << ":2";

    //keep going until all jobs are done
    while(doneJobs < totalJobs)
    {
        Node* shortest =NULL; 
        // look through all jobs to find the shortest one that has arrived
        Node* cur = q->headProcNode;
        while(cur != NULL){
            if(cur->isDone == 0 && cur->procData.arrival <= time){
                if(shortest == NULL || cur->procData.execTime < shortest->procData.execTime){
                    shortest = cur;
                }
            }
            cur = cur->nextNode;
        }
        // if no job is ready yet move forward to the next arrival
        if(shortest == NULL){
            int nextArrival = INT_MAX;
            cur = q->headProcNode;
            while(cur != NULL){
                if(cur->isDone == 0 && cur->procData.arrival < nextArrival){
                    nextArrival = cur->procData.arrival;
                }
                cur = cur->nextNode;
            }
            time = nextArrival;
            continue;
        }
        //waiting time=current time - arrival time
        int waitTime = time - shortest->procData.arrival;
        outFile << ":" << waitTime;
        cout << ":" << waitTime;

        totalWait+=waitTime;
        time+=shortest->procData.execTime;
        shortest->isDone = 1;
        doneJobs++;

    }
    
        double averageWait;
        if(totalJobs == 0){
            averageWait = 0.0;
        }else {
            averageWait = totalWait/totalJobs;
        }

        outFile << ":" << averageWait <<"\n";
        cout << ":" << averageWait <<"\n";

}

int main(int argc, char *argv[])
{
    if(argc != 3){
        cout<< "Usage: " << argv[0] <<" input.txt output.txt\n";
        return 1;
    }
    /* read file to build queue list */
    Queue* allQueues = nullptr;
    if(!readInput(argv[1], &allQueues))
    return 1;

   for (Queue* q = allQueues; q; q = q->nextQueue){
    cout<<"Queue "<< q->queueId <<"has"<<count(q->headProcNode)<<" jobs\n";
   
   }
   ofstream outFile(argv[2]);
   if(!outFile){
    cout<<"could not open the output file."<< endl;
    return 1;
   }
   //run FCFS and SJF scheduling for each queue
   Queue* q = allQueues;
   while(q != NULL){
    fcfs(q, outFile);

    resetFlags(q->headProcNode);

    sjf(q, outFile);

    q = q->nextQueue;
   }
   //free memory
Queue* curQueue = allQueues;
while(curQueue != nullptr){
    freeAll(curQueue->headProcNode);
    Queue* tmpQueue = curQueue;
    curQueue = curQueue->nextQueue;
    delete tmpQueue;
}
return 0; 
  
}
