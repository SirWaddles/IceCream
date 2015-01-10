#include "util/pos2d.h"
#include <cmath>

Pos2D::Pos2D(int cX, int cY):x(cX), y(cY){

}

Pos2D::Pos2D(){
	y=0;
	x=0;
}

bool operator==(Pos2D &a, Pos2D &b){
	if(a.x==b.x&&a.y==b.y){
		return true;
	}
	return false;
}

bool operator!=(Pos2D &a, Pos2D &b){
	if(!(a.x==b.x&&a.y==b.y)){
		return true;
	}
	return false;
}

Pos2D operator+(const Pos2D &a, const Pos2D &b){
	return Pos2D(a.x+b.x, a.y+b.y);
}

Pos2D operator-(const Pos2D &a, const Pos2D &b){
	return Pos2D(a.x-b.x, a.y-b.y);
}

float Pos2D::Distance(Pos2D point){
	float x1mx2=x-point.x;
	float y1my2=y-point.y;
	float tort=x1mx2*x1mx2+y1my2*y1my2;
	return sqrt(tort);
}