#ifndef _POS2D_HEADER_
#define _POS2D_HEADER_

struct Pos2D{
	int x;
	int y;
	Pos2D(int cX, int cY);
	Pos2D();
	friend bool operator==(Pos2D &a, Pos2D &b);
	friend bool operator!=(Pos2D &a, Pos2D &b);
	friend Pos2D operator+(const Pos2D &a, const Pos2D &b);
	friend Pos2D operator-(const Pos2D &a, const Pos2D &b);
	float Distance(Pos2D point);
};

#endif