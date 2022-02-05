#include <assert.h>
#include <iostream>
#ifndef LQueue_H
#define LQueue_H

// Singly linked  node
template< typename E> class Link{
  public:
    E element;
    Link *next;
    // Constructors
    Link(const E& elemval, Link* nextval = NULL){
      element = elemval;
      next = nextval;
  }
  Link(Link* nextval=NULL) { next = nextval;}
};

// Linked Queue
template <typename E> 
class LQueue {
  public:
    Link<E>* head;
    Link<E>* tail;
    Link<E>* curr;
    int cnt;
  
    void init() {
      curr = tail = head = new Link<E>;
      cnt = 0;
    }
    void removeall() {
      while( head != NULL){
        curr = head;
        head = head -> next;
        delete curr;
      }
    }
  public:
    LQueue () { init(); }
    ~LQueue() { removeall(); }
    
    // Insert
    void enqueue(const E& it){
      curr = tail;
      curr->next = new Link<E>(it, curr->next);
      tail = curr->next;
      cnt++;
      curr = head;
    }
    E dequeue() {
      // find correct pos for it
      curr=head;                                      // set curr to head
      Link<E>* ltemp = curr->next;                    // store top priority node
      E it = ltemp->element;                          // store element of top priority
      curr->next = ltemp->next;                       // set curr-> next to second priority node
      delete ltemp;                                   // delete ltemp
      return it;                                      // return key of top node
    }
    // Remove element
    void remove(const E& it) {
      //assert(curr->next != NULL);
      curr = head;
      while (curr!=tail){
        if (it == curr->next->element) {
          Link<E>* ltemp = curr->next;
          if (tail == curr-> next) tail = curr;
          curr->next = curr->next->next;
          delete ltemp;
          cnt--;
          break;
        }
        curr = curr->next;
      } 
    }
    void prev(){
      Link<E>* temp = head;
      while (temp->next!=curr) temp=temp->next;
      curr = temp;
    }
    void next(){
      if (curr != tail) curr = curr->next;
    }
    int length() const { return cnt; }
    int currPos() const{
      Link<E>* temp = head;
      int i;
      for(i=0; curr!=temp; i++)
        temp = temp->next;
      return i;
    }
    const E& top(){
      curr = head;
      return curr->next->element;
    }
    const E& getNext(){
      assert(curr->next != NULL);
      if (curr != tail) curr = curr->next;
      return curr->next->element;
    }
    const E& getKey() const {
      assert(curr->next != NULL);
      return curr->next->element;
    }

};
#endif