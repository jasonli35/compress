#include <list>
#include "HCTree.hpp"

void destructorHelper(HCNode *node){
    if(node == nullptr){
        return;
    }
    destructorHelper(node->c0);
    destructorHelper(node->c1);
    delete node;
}

HCTree::~HCTree(){
    destructorHelper(root);
}



void HCTree::build(const vector<unsigned short int>& freqs){
    

    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp>pq;

    unsigned short int currFreq;
    for(int i = 0; i < 256; i++){
        currFreq = freqs.at(i);
        if(currFreq > 0){
            HCNode* newNode = new HCNode(currFreq, (unsigned char)i);
            leaves.at(i) = newNode;
            pq.push(newNode);
        }
    }
    HCNode *parent = nullptr;
    while(pq.size() > 1){
        HCNode *lowestFrq = pq.top();
        pq.pop();
        HCNode *secondLowest = pq.top();
        pq.pop();
        parent = new HCNode(lowestFrq->count + secondLowest->count, lowestFrq->symbol);
        parent->c0 = lowestFrq;
        parent->c1 = secondLowest;
        lowestFrq->p = parent;
        secondLowest->p = parent;
        pq.push(parent);
    }
    root = parent;


}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    
    HCNode *curr = leaves[symbol];
    list<int> result;
    while(curr != root){
        if(curr->p->c0 == curr){
            result.push_front(0); 
        }
        else{
            result.push_front(1);
        }
        curr = curr->p;
    }
    for(list<int>::iterator it = result.begin(); it != result.end(); ++it){
        out.write_bit(*it);
    }
}

unsigned char HCTree::decode(FancyInputStream & in) const{
    HCNode *curr = root;
    int bit;
    
    while(curr->c0 != nullptr && curr->c1 != nullptr){
        bit = in.read_bit();
        if(bit == 0){
            curr = curr->c0;
        }
        else {
            curr = curr->c1;
        }
 
    }
    return curr->symbol;

}








