#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    

using namespace std;

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  	Shape(string name) {
	  name_ = name;
	}
	
  	string getName() {
	  return name_;
	}
	
  	virtual double getArea() const = 0;
  	virtual double getVolume() const = 0;
	
private:
  string name_;


};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
  	Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") {
	  width_ = width;
	  length_ = length;
	}
	
  	double getArea() const {
	  return (double)length_*width_;
	}
	
  	double getVolume() const {
	  return 0;
	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) {
	  return Rectangle<T>(width_+rec.getWidth(),length_+rec.getLength());
	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
	  return Rectangle<T>(max(0.0,(double)width_-rec.getWidth()),max(0.0,(double)length_-rec.getLength()));

	} 
	
	T getWidth() const { 
	  return width_;
	}
	
	T getLength() const { 
	  return length_;
	}
private:
	T width_;
        T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  	Circle(double radius):Shape("Circle") {
	  radius_=radius;
	}
	
  	double getArea() const{
	  return radius_*radius_*M_PI;
	}
	
 	double getVolume() const{
	  return 0;
	}
	
  	Circle operator + (const Circle& cir){
	  return Circle(radius_+cir.getRadius());
	}
	
	Circle operator - (const Circle& cir){
	  return Circle(max(0.0,radius_-cir.getRadius()));
	} 

	double getRadius() const { 
	  return radius_;
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  	Sphere(double radius):Shape("Sphere") {
	  radius_= radius;
	}

  	double getVolume() const {
	  return 4.0*M_PI*radius_*radius_*radius_/3.0;
	}	
	
  	double getArea() const {
	   return 4.0*M_PI*radius_*radius_;
	}

	Sphere operator + (const Sphere& sph) {
	  return Sphere(radius_+sph.getRadius());
	}

	Sphere operator - (const Sphere& sph) {
	  return Sphere(max(0.0,radius_-sph.getRadius()));
	} 
	
	double getRadius() const {
	  return radius_;
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
class RectPrism : public Shape{
public:
  	RectPrism(double width, double length, double height):Shape("RectPrism") {
	  width_ = width;
	  length_=length;
	  height_=height;
	}
	
  	double getVolume() const {
	  return length_*width_*height_;
	}
  	
	double getArea() const {
	  return 2.0*(length_*width_+length_*height_+height_*width_);
	}
	
	RectPrism operator + (const RectPrism& rectp){
	  return RectPrism(width_+rectp.getWidth(),length_+rectp.getLength(),height_+rectp.getHeight());
	}
	
	RectPrism operator - (const RectPrism& rectp){
	  return RectPrism(max(0.0,width_-rectp.getWidth()),max(0.0,length_-rectp.getLength()),max(0.0,height_-rectp.getHeight()));
	}	
	
	double getWidth() const { 
	  return width_;
	}
	
	double getLength() const { 
	  return length_;
	}
	
	double getHeight() const { 
	  return height_;
	}
  
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {
  int num;
  double w,l,h,r;
  string shape;
  list<Shape*>shapes{};
  Shape * sh = NULL;
  ifstream ifs(file_name,std::ifstream::in);
  ifs>>num;
  for(int i = 0; i < num; i++) {
    ifs>>shape;
      if (shape == "Circle"){
      ifs>>r;
      sh = new Circle(r);
    }
    else if (shape == "Rectangle"){
      ifs>>w>>l;
      sh = new Rectangle<double>(w,l);
    }
    else if (shape == "Sphere"){
      ifs>>r;
      sh = new Sphere(r);
    }
    else if (shape == "RectPrism"){
      ifs>>w>>l>>h;
      sh = new RectPrism(w,l,h);
    }
    shapes.push_back(sh);
  }
  ifs.close();
  return shapes;
  }





// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
	for(list<Shape*>::iterator it = shapes.begin();it != shapes.end();it++) {
	  if((**it).getArea()>max_area) {
	    max_area = (**it).getArea();
	  }
	}
	
	return max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
	for(list<Shape*>::iterator it = shapes.begin();it != shapes.end();it++) {
	  if((**it).getVolume()>max_volume) {
	    max_volume = (**it).getVolume();
	  }
	}

	
	return max_volume;
}

#endif

