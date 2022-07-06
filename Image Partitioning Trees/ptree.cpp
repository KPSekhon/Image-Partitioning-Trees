/*
*  File:        ptree.cpp
*  Description: Implementation of a partitioning tree class for CPSC 221 PA3
*  Date:        2022-03-03 01:53
*
*               ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS TO THE BOTTOM OF THIS FILE
*/

#include "ptree.h"
#include "hue_utils.h" // useful functions for calculating hue averages

using namespace cs221util;
using namespace std;

// The following definition may be convenient, but is not necessary to use
typedef pair<unsigned int, unsigned int> pairUI;

/////////////////////////////////
// PTree private member functions
/////////////////////////////////

/*
*  Destroys all dynamically allocated memory associated with the current PTree object.
*  You may want to add a recursive helper function for this!
*  POST: all nodes allocated into the heap have been released.
*/
void PTree::Clear() {
  // add your implementation below
  deleteNode(root);
  
}

/*
*  Copies the parameter other PTree into the current PTree.
*  Does not free any memory. Should be called by copy constructor and operator=.
*  You may want a recursive helper function for this!
*  PARAM: other - the PTree which will be copied
*  PRE:   There is no dynamic memory associated with this PTree.
*  POST:  This PTree is a physically separate copy of the other PTree.
*/
void PTree::Copy(const PTree& other) {
  // add your implementation below
  copyTree(root, other.root);

  
}

/*
*  Private helper function for the constructor. Recursively builds the tree
*  according to the specification of the constructor.
*  You *may* change this if you like, but we have provided here what we
*  believe will be sufficient to use as-is.
*  PARAM:  im - full reference image used for construction
*  PARAM:  ul - upper-left image coordinate of the currently building Node's image region
*  PARAM:  w - width of the currently building Node's image region
*  PARAM:  h - height of the currently building Node's image region
*  RETURN: pointer to the fully constructed Node
*/
Node* PTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  Node* node = nullptr;
  if(w>0 && h>0){

  node = new Node();
  //cout<<"The node being created is "<<node<<endl;
  node->upperleft =ul;
  node->height = h;
  //cout<<node->height<<endl;
  node->width = w;
  //cout<<node->width<<endl;
  node->avg = avgCalculator(im,ul,w,h);
  //cout<<node->avg<<endl;
  if(ul.first + w > im.width()){
    cout<<"BADDD"<<endl;
  }


  // make avg calculator that takes it upperleft, width and height

// leaf conditon don't need to recursively call anymore
  if(w==1&& h==1){
    return node;
  }
  if(w>h || w==h){
    unsigned int split;
    unsigned int x = ul.first+ (w/2);
    if(w%2!=0){
      split = w+1;
      split = split/2;
      node->A = BuildNode(im, ul, w/2,h);
      node->B = BuildNode(im, {x, ul.second}, split, h);
    } else{
      node->A = BuildNode(im, ul, w/2,h);
      node->B = BuildNode(im, {x, ul.second}, w/2, h);
    }
  // horizonatal flip case
// correctly adjust height and with then recusivelly call
// makse sure to have leaf condition
  }else{
// vertical flip case
unsigned int split;
unsigned int y = ul.second+ (h/2);
if(h%2!=0){
      split = h+1;
      split = split/2;
      node->A = BuildNode(im, ul, w, h/2);
      node->B = BuildNode(im, {ul.first, y}, w, split);
    } else{
      node->A = BuildNode(im, ul, w, h/2);
      node->B = BuildNode(im, {ul.first, y}, w, h/2);
    }
  }

}
  // replace the line below with your implementation
  return node;
}

////////////////////////////////
// PTree public member functions
////////////////////////////////

/*
*  Constructor that builds the PTree using the provided PNG.
*
*  The PTree represents the sub-image (actually the entire image) from (0,0) to (w-1, h-1) where
*  w-1 and h-1 are the largest valid image coordinates of the original PNG.
*  Each node corresponds to a rectangle of pixels in the original PNG, represented by
*  an (x,y) pair for the upper-left corner of the rectangle, and two unsigned integers for the
*  number of pixels on the width and height dimensions of the rectangular sub-image region the
*  node defines.
*
*  A node's two children correspond to a partition of the node's rectangular region into two
*  equal (or approximately equal) size regions which are either tiled horizontally or vertically.
*
*  If the rectangular region of a node is taller than it is wide, then its two children will divide
*  the region into vertically-tiled sub-regions of equal height:
*  +-------+
*  |   A   |
*  |       |
*  +-------+
*  |   B   |
*  |       |
*  +-------+
*
*  If the rectangular region of a node is wider than it is tall, OR if the region is exactly square,
*  then its two children will divide the region into horizontally-tiled sub-regions of equal width:
*  +-------+-------+
*  |   A   |   B   |
*  |       |       |
*  +-------+-------+
*
*  If any region cannot be divided exactly evenly (e.g. a horizontal division of odd width), then
*  child B will receive the larger half of the two subregions.
*
*  When the tree is fully constructed, each leaf corresponds to a single pixel in the PNG image.
* 
*  For the average colour, this MUST be computed separately over the node's rectangular region.
*  Do NOT simply compute this as a weighted average of the children's averages.
*  The functions defined in hue_utils.h and implemented in hue_utils.cpp will be very useful.
*  Computing the average over many overlapping rectangular regions sounds like it will be
*  inefficient, but as an exercise in theory, think about the asymptotic upper bound on the
*  number of times any given pixel is included in an average calculation.
* 
*  PARAM: im - reference image which will provide pixel data for the constructed tree's leaves
*  POST:  The newly constructed tree contains the PNG's pixel data in each leaf node.
*/
PTree::PTree(PNG& im) {
  // add your implementation below
  unsigned int width = im.width();
  unsigned int height = im.height();
  if(width>0 && height>0){
  root = BuildNode(im,{0,0},width, height);
  }
}

/*
*  Copy constructor
*  Builds a new tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  This tree is constructed as a physically separate copy of other tree.
*/
PTree::PTree(const PTree& other) {
  // add your implementation below
  Copy(other);
  
}

/*
*  Assignment operator
*  Rebuilds this tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  If other is a physically different tree in memory, all pre-existing dynamic
*           memory in this tree is deallocated and this tree is reconstructed as a
*           physically separate copy of other tree.
*         Otherwise, there is no change to this tree.
*/
PTree& PTree::operator=(const PTree& other) {
  // add your implementation below
  if(this!=&other){
    Clear();
    Copy(other);
  }

  return *this;
}

/*
*  Destructor
*  Deallocates all dynamic memory associated with the tree and destroys this PTree object.
*/
PTree::~PTree() {
  // add your implementation below
  //you would want to recusivelly call on A and B and then dealloacte the root for every sub tree
  // when you get to the case that A and B are null, then you would deallocate that node
  // set the pointer to null and return, when you return you would go into B and since it's a leaf
  // it would also deallocate, now when you have returned into the subtree both it's A and B are now
  // null so it would dealllocate that Node
  Clear();
  
}

/*
*  Traverses the tree and puts the leaf nodes' color data into the nodes'
*  defined image regions on the output PNG.
*  For non-pruned trees, each leaf node corresponds to a single pixel that will be coloured.
*  For pruned trees, each leaf node may cover a larger rectangular region that will be
*  entirely coloured using the node's average colour attribute.
*
*  You may want to add a recursive helper function for this!
*
*  RETURN: A PNG image of appropriate dimensions and coloured using the tree's leaf node colour data
*/
PNG PTree::Render() const {
  // replace the line below with your implementation

  /*
  For the recursive helper you might want to have a condition that check if A and B
  are null, which would indicate a pruned tree or a condition if w==1 and h==1, 
  otherwise keep calling it
 might want to do PNG img = PNG, then send in the node plus &img


  */
 PNG img = PNG();
 img.resize(root->width, root->height);
 img = *(renderImg(root, &img));


  return img;
}

/*
*  Trims subtrees as high as possible in the tree. A subtree is pruned
*  (its children are cleared/deallocated) if ALL of its leaves have colour
*  within tolerance of the subtree root's average colour.
*  Pruning criteria should be evaluated on the original tree, and never on a pruned
*  tree (i.e. we expect that Prune would be called on any tree at most once).
*  When processing a subtree, you should determine if the subtree should be pruned,
*  and prune it if possible before determining if it has subtrees that can be pruned.
* 
*  You may want to add (a) recursive helper function(s) for this!
*
*  PRE:  This tree has not been previously pruned (and is not copied/assigned from a tree that has been pruned)
*  POST: Any subtrees (as close to the root as possible) whose leaves all have colour
*        within tolerance from the subtree's root colour will have their children deallocated;
*        Each pruned subtree's root becomes a leaf node.
*/
void PTree::Prune(double tolerance) {
  // add your implementation below
  pruneMachine(root, tolerance);
  
}

/*
*  Returns the total number of nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::Size() const {
  // replace the line below with your implementation
  /*
  probably have base case if node == nullptr, return 0
  if node->A && node-B == nullptr, return 1
  and then have case
  call function on node->A
  call function on node-> B
  then add 1 for itself
  then return

  */
  return sizeCount(root);
}

/*
*  Returns the total number of leaf nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::NumLeaves() const {
  // replace the line below with your implementation

  /*
  probably have base case if node == nullptr, return 0
  if node->A && node-B == nullptr, return 1
  and then have case
  call function on node->A
  call function on node-> B

  */
  return leavesCount(root);
}

/*
*  Rearranges the nodes in the tree, such that a rendered PNG will be flipped horizontally
*  (i.e. mirrored over a vertical axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped horizontally.
*/
void PTree::FlipHorizontal() {
  // add your implementation below
  horizontalFlip(root, root->width);
  
}

/*
*  Like the function above, rearranges the nodes in the tree, such that a rendered PNG
*  will be flipped vertically (i.e. mirrored over a horizontal axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped vertically.
*/
void PTree::FlipVertical() {
  // add your implementation below
  verticalFlip(root, root->height);
  
}

/*
    *  Provides access to the root of the tree.
    *  Dangerous in practice! This is only used for testing.
    */
Node* PTree::GetRoot() {
  return root;
}

//////////////////////////////////////////////
// PERSONALLY DEFINED PRIVATE MEMBER FUNCTIONS
//////////////////////////////////////////////
void PTree:: pruneMachine(Node*& node, double tolerance){
  if(node== nullptr){
    return;
  }
  if(node->A == nullptr && node->B == nullptr){
    return;
  }
  if(leavesInTol(node, node->avg, tolerance)){
    deleteNode(node->A);
    deleteNode(node->B);
  } else{
    pruneMachine(node->A, tolerance);
    pruneMachine(node->B, tolerance);
  }
}


bool PTree::leavesInTol(Node* node, HSLAPixel p, double tol){
  if(node==nullptr){
    return true;
  }
  if(node->A == nullptr && node->B == nullptr){
    double currVal = node->avg.dist(p);
    return currVal <= tol;
  }
  return leavesInTol(node->A, p, tol) && leavesInTol(node->B, p, tol);
}






PNG* PTree::renderImg(Node* node, PNG* im) const{
  /*
  For the recursive helper you might want to have a condition that check if A and B
  are null, which would indicate a pruned tree or a condition if w==1 and h==1, 
  otherwise keep calling it
 might want to do PNG img = PNG, then send in the node plus &img
  */
  if(node==nullptr){
    return im;
  }
  if(node->A== nullptr && node->B == nullptr){
    HSLAPixel* curr;
    for(unsigned int y= node->upperleft.second; y < node->height + node->upperleft.second; y++){
       for(unsigned int x= node->upperleft.first; x < node->width + node->upperleft.first; x++){
         curr = im->getPixel(x,y);
         *curr = node->avg;
    }
  }

  }else{
    renderImg(node->A, im);
    renderImg(node->B, im);
  }
  return im;
}

int PTree:: sizeCount(Node* node) const{
/*
  probably have base case if node == nullptr, return 0
  if node->A && node-B == nullptr, return 1
  and then have case
  call function on node->A
  call function on node-> B
  then add 1 for itself
  then return
  */
if(node==nullptr){
  return 0;
}
if(node->A== nullptr && node->B){
  return 1;
}
return sizeCount(node->A) + sizeCount(node->B) + 1;
}

int PTree:: leavesCount(Node* node) const{
/*
  probably have base case if node == nullptr, return 0
  if node->A && node-B == nullptr, return 1
  and then have case
  call function on node->A
  call function on node-> B
  */
if(node==nullptr){
  return 0;
}
if(node->A== nullptr && node->B== nullptr){
  //cout<<"I made into here"<<endl;
  return 1;
}
int left = leavesCount(node->A);
//cout<<"The A val is "<<left<<endl;
int right = leavesCount(node->B);
//cout<<"The B val is "<<right<<endl;
return left + right;
}


HSLAPixel PTree::avgCalculator(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h){
  // case where its one, leaf node
  HSLAPixel avg;
  HSLAPixel* curr;
  double total=0;
  double alp=0;
  double lum=0; 
  double sat=0;
  double hueX=0;
  double hueY=0;
  double hue;
  
  for(unsigned int y =(unsigned int) ul.second; y < h+ul.second; y++){
    for(unsigned int x =(unsigned int) ul.first; x < w+ul.first; x++){
      curr = im.getPixel(x,y);
      alp+= curr->a;
      lum+= curr->l;
      sat+= curr->s;
      hueX+=Deg2X(curr->h);
      hueY+=Deg2Y(curr->h);
      total++;
    }
  }
  
  alp = alp/total;
  lum = lum/total;
  sat = sat/total;
  hueX = hueX/total;
  hueY = hueY/total;
  hue = XY2Deg(hueX, hueY);
  avg.a = alp;
  avg.h = hue;
  avg.s = sat;
  avg.l = lum;
  avg = HSLAPixel(hue,sat,lum);

  curr = nullptr;

  return avg;
  
}

void PTree:: horizontalFlip(Node* node, unsigned int origWidth){
  if(node==nullptr){
    return;
  } else if(node->A == nullptr && node->B == nullptr){
    unsigned int x, y;
    x = node->upperleft.first;
    y = node->upperleft.second;
    if(node->width == 1 && node->height==1){
    x = (x - origWidth + 1)*-1;
    } else{
    // prune case
     x = ((x+ node->width-1) - origWidth + 1)*-1;
    }
    node->upperleft = {x,y};
    return;
  }
  else{
    // upperleft impacts color swaps
    // leaf swaps requied plus upperleft - width potentially == new upperleft
    horizontalFlip(node->A, origWidth);
    horizontalFlip(node->B, origWidth);
    if(node->width >= node->height){
      node->upperleft = node->B->upperleft;
      Node* temp = node->A;
      node->A = node->B;
     node->B = temp;
    } else{
      node->upperleft= node->A->upperleft;
    }
  }
}
void PTree:: verticalFlip(Node* node, unsigned int origHeight){
  if(node==nullptr){
    return;
  } else if(node->A == nullptr && node->B == nullptr){
    unsigned int x, y;
    x = node->upperleft.first;
    y = node->upperleft.second;
    if(node->width==1 && node->height==1){
    y = (y - origHeight + 1)*-1;
    } else{
      y = ((y+ node->height-1) - origHeight + 1)*-1;
    }
    node->upperleft = {x,y};
    return;
  }
  else{
    // upperleft impacts color swaps
    // leaf swaps requied plus upperleft - width potentially == new upperleft
    verticalFlip(node->A, origHeight);
    verticalFlip(node->B, origHeight);
    if(node->width <= node->height){
      node->upperleft = node->B->upperleft;
      Node* temp = node->A;
      node->A = node->B;
     node->B = temp;
    } else{
      node->upperleft= node->A->upperleft;
    }
  }
}


void PTree::copyTree(Node*& curr, Node* other){
  if(other!=nullptr){
    curr = new Node(other->upperleft, other->width, other->height, other->avg, other->A, other->B);
    if(other->A!=nullptr){
      copyTree(curr->A, other->A);
    }
    if(other->B!=nullptr){
      copyTree(curr->B, other->B);
    }
  }
}

void PTree::deleteNode(Node*& node){
   //you would want to recusivelly call on A and B and then dealloacte the root for every sub tree
  // when you get to the case that A and B are null, then you would deallocate that node
  // set the pointer to null and return, when you return you would go into B and since it's a leaf
  // it would also deallocate, now when you have returned into the subtree both it's A and B are now
  // null so it would dealllocate that Node
  if(node == nullptr){
    return;
  }
  if(node-> A == nullptr && node->B == nullptr){
    //cout<<"The node being deleted is "<<node<<endl;
    delete node;
    node = nullptr;
    return;
  } else{
    deleteNode(node->A);
    deleteNode(node->B);
    //cout<<"The node being deleted is "<<node<<endl;
    delete node;
    node = nullptr;
    return;
  }
}
