#include <assert.h>
#include <iostream>
#include "LQueue.h"
#ifndef NPQueue_H
#define NPQueue_H

template< typename E, typename P> class QLink{
  public:
    P priority;             // priority value
    LQueue<E>* queue;       // linked queue to store elements
    QLink *prev;
    QLink *next;
    // Constructors
    QLink(const P& priorval, QLink* preval = NULL, QLink* nextval = NULL){
      priority = priorval;
      queue = new LQueue<E>();
      prev = preval;
      next = nextval;
  }
  QLink(QLink* preval = NULL, QLink* nextval=NULL) { queue = new LQueue<E>(); prev = preval; next = nextval;}
};
// Double Linked
template <typename E, typename P> 
class NPQueue {
  private:
    QLink<E, P>* head;
    QLink<E, P>* tail; 
    QLink<E, P>* curr;       // pointer to priority Qlink node
    Link<E>* currElem;       // pointer to element link node 
    int cnt;                 // total elements in queue
    void init() {
      curr = head = new QLink<E, P>;
      tail = new QLink<E, P>(head);
      curr->next = tail;
      cnt = 0;
    }
    void removeall(){
      //clear();
      delete head;
      delete tail;
    }
  public:
    NPQueue () { init(); }
    ~NPQueue() { removeall(); }
    
    // inserting in priority position considering fifo 
    void enqueue(const E& it, const P& pt ){
      bool exist = false;
      curr=head;                                // point to head
      int i=0;
      while(curr->next!=tail){                  // loop until curr->next is tail
        if (pt == curr->next->priority) {       // if node with pririty pt present
          curr->next->queue->enqueue(it);       // enqueue it to it
          exist = true;  break; 
        }
        if (pt > curr->next->priority) { break; } // if pt > curr next priority break loop
        curr = curr->next;
      } 
      if (!exist){                              // if priority node doesnot exist
        QLink<E,P>* temp = new QLink<E,P>(pt, curr, curr->next); // create new node
        curr->next = curr->next->prev = temp; 
        temp->queue->enqueue(it);                                // enqueue it to the node
		    curr->queue->tail->next = temp->queue->head->next;       // set tail next of curr node's queue to head next of new
		    temp->queue->tail->next = temp->next->queue->head->next; // set tail next of new node's queue to head next of node after
      } 
      cnt++;
    }

    // remove top priority element
    E dequeue() {
      curr = head;                                      // set curr to head
      QLink<E,P>* ltemp = curr->next;                  // store top priority node
      LQueue<E>* qtemp = ltemp->queue;
      E it = qtemp->dequeue();
      if (qtemp->length()==0){ 
        curr->next = ltemp->next;                       // set curr-> next to second priority node
        curr->next->prev = curr;                        // set prev of second priority node to curr
        delete ltemp;                                   // delete ltemp
      }
      return it;                                         // return key of top node
    }
    // clear all the elements and its priority
    void clear() {
      while( head->next != tail){                     // loop until head->next is tail
        curr = head -> next;                          // set curr to head->next
        head -> next = head -> next -> next;          // set head->next to link after
        delete curr;                                  // delete curr
      }
      tail->prev = head;                              // set tail->prev to head 
    }

    // returns the pos of priority node if present
    int find(const P& pt){
      int i=0;
      curr = head;
      while (curr->next!=tail){
        if (pt == curr->next->priority) { return i;}
        curr = curr->next;
        i++;
      }
      return -1;
    }
    void remove(const E& it, const P& pt){
      if (find(pt)!=-1){                                  // find pt
        QLink<E,P>* ltemp = curr->next;                   // store Qlink node
        ltemp->queue->remove(it);                         // remove it from Qlink 
        if (ltemp->queue->length()==0){                   // if no element in Qlink remove Qlink
          curr->next = ltemp->next;                       // set curr-> next to second priority node
          curr->next->prev = curr;                        // set prev of second priority node to curr
          delete ltemp;                                   // delete ltemp
        }
        curr->queue->tail->next = curr->next->queue->head->next; // set curr tail next to curr next head next
        cnt--;
      }
    }

    void insert(const E& it, const P& newpt){
      QLink<E,P>* temp = new QLink<E,P>(newpt, curr, curr->next);  // create Qlink with curr as prev and curr->next as next
      curr->next = curr->next->prev = temp;                        // set curr->next and curr->next->prev to new Qlink
      temp->queue->enqueue(it);                                    // enqueue it to Qlink
      curr->queue->tail->next = temp->queue->head->next;           // point curr tail next to new Qlink head next
      temp->queue->tail->next = temp->next->queue->head->next;     // point new Qlink tail next to Qlink after head next
      cnt++;
    }
    // increase the priority of element
    void pup(const E& it, const P& pt, const P& inc=1){
      // loop through list to find element it 
      remove(it, pt);
      P newpt = pt+inc;
      bool exist =false;
      while(curr!=head){                  // find correct pos for it
        if (newpt == curr->priority){     // if Qlink with priority newpt exist
          curr->queue->enqueue(it);       // enqueue it 
          exist=true; cnt++;
          break;
        }
        if (newpt < curr->priority) { break; } // if newpt < curr-> priority break loop
        curr = curr->prev;
      }
      if(!exist){ insert(it, newpt); }    // if doesnot exist insert it with priority newpt
      top();

    }
    // decrease the priority of element
    void pdown(const E& it, const P& pt, const P& dec=1){
      remove(it, pt);
      P newpt = pt-dec;
      bool exist =false;
      while(curr->next!=tail){                  // find correct pos for it
        if (newpt == curr->next->priority){     // if Qlink with priority newpt exist
          curr->next->queue->enqueue(it);       // enqueue it
          exist = true; cnt++;
          break;
        }
        if (newpt > curr->next->priority) { break; } // if newpt > curr->next->priority break loop
        curr = curr->next;
      }
      if(!exist){ insert(it, newpt); }         // if doesnot exist insert it with priority ne
      top();
    }
    // get top priority element
    const E& top(){
      curr = head;
      currElem = curr->next->queue->head;
      return currElem->next->element;
    }
    // get next element
    const E& getNext(){
      if (currElem->next!=NULL) {
        currElem = currElem->next;
      }
      return currElem->next->element;
    }
    // get key
    const E& getKey() const {
      assert(currElem->next != NULL);
      return currElem->next->element;
    }
    // get priority value
    const P& getPValue() const {
      assert(curr->next != tail);
      return curr->next->priority;
    }
    // move to previous 
    void prev(){
      if (curr != head) curr = curr->prev;
    }
    // move to next
    void next(){
      if (curr->next != tail) curr = curr->next;
    }
    // length of the queue
    int length() const { return cnt; }

};
#endif
