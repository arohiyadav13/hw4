#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int height(Node * root)
{
  if(root == NULL)
  {
    return 0;  
  }
  int left_height = height(root->left);
  int right_height = height(root->right);
  if (left_height > right_height) 
  {
    return left_height + 1;
  } 
  else 
  {
    return right_height + 1;
  }
}


bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL)
    {
      return true; 
    }
    if(root->right == NULL && root->left == NULL)
    {
      return true; 
    }
    int left_height = height(root->left);
    int right_height = height(root->right);
    if (root->left == nullptr) 
    {
      return right_height == 1;
    }
    if (root->right == nullptr) 
    {
      return left_height == 1;
    }
    return left_height == right_height;
}

