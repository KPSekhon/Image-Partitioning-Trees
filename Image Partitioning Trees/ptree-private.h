/*
*  File:        ptree-private.h
*  Description: Private functions for the PTree class
*  Date:        2022-03-06 03:30
*
*               DECLARE YOUR PTREE PRIVATE MEMBER FUNCTIONS HERE
*/

#ifndef _PTREE_PRIVATE_H_
#define _PTREE_PRIVATE_H_

/////////////////////////////////////////////////
// DEFINE YOUR PRIVATE MEMBER FUNCTIONS HERE
//
// Just write the function signatures.
//
// Example:
//
// Node* MyHelperFunction(int arg_a, bool arg_b)
/////////////////////////////////////////////////
 void deleteNode(Node*& node);
  HSLAPixel avgCalculator(PNG& im,pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h);
  PNG* renderImg(Node* node, PNG* im) const;
  int sizeCount(Node* node) const;
  int leavesCount(Node* node) const;
  void horizontalFlip(Node* node, unsigned int origWidth);
  void verticalFlip(Node* node, unsigned int origHeight);
  void copyTree(Node*& curr, Node* other);
  bool leavesInTol(Node* node, HSLAPixel p, double tol);
  void pruneMachine(Node*& node, double tolerance);
#endif