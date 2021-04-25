#ifndef ___POINT_H_
#define ___POINT_H_

#include <iostream>

class Point{
   
public:
   typedef  double           TPCoord;


public:
  TPCoord pt[3];
  TPCoord normal[3];
  int color[4];

public:
  Point(){pt[0]=0.0; pt[1]=0.0; pt[2]=0.0;};
  Point(TPCoord x, TPCoord y, TPCoord z){pt[0]=x; pt[1]=y; pt[2]=z;};
  Point(TPCoord x, TPCoord y, TPCoord z,int R,int G,int B, int A){pt[0]=x; pt[1]=y; pt[2]=z;color[0]=R;color[1]=G;color[2]=B;color[3]=A;};
  TPCoord &operator[](char i){return pt[i];};
  
  void
  Set(TPCoord x, TPCoord y, TPCoord z){pt[0]=x; pt[1]=y; pt[2]=z;};
  void SetNormal(TPCoord x, TPCoord y, TPCoord z){normal[0]=x; normal[1]=y; normal[2]=z;};
  void   SetColor(TPCoord x, TPCoord y, TPCoord z,int R,int G,int B, int A){
      pt[0]=x;
      pt[1]=y;
      pt[2]=z;
      color[0]=R;
      color[1]=G;
      color[2]=B;
      color[3]=A;
  };
  Point &operator=(Point &p){
     pt[0]=p.pt[0]; pt[1]=p.pt[1]; pt[2]=p.pt[2];
     return *this;
  };
  
  void Zero(){pt[0] = 0.0; pt[1] = 0.0; pt[2] = 0.0;};
  
  void Print(){std::cout << "(" << pt[0] << ", " << pt[1] << ", " << pt[2] << ")";};
};

#endif
