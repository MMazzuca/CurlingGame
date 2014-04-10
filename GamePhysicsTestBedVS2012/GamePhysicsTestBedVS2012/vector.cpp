#include "vector.h"
#include <Windows.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <cmath>

Vector::Vector(const Point& p)
	: x(p.x), y(p.y), z(p.z)
{
}

Vector Vector::operator-() const
{
	return Vector(-x, -y, -z);
}

Vector Vector::operator+(const Vector& v) const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator-(const Vector& v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::Normalized() const
{
	return (*this) / Length();
}

Vector Vector::operator*(float s) const
{
	return Vector(x * s, y * s, z * s);
}

Vector Vector::operator/(float s) const
{
	return Vector(x / s, y / s, z / s);
}

float Vector::Length() const
{
	return sqrt(x*x + y*y + z*z);
}

float Vector::LengthSqr() const
{
	return (x*x + y*y + z*z);
}

Vector operator-(Point a, Point b)
{
	Vector v;

	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;

	return v;
}

Point Point::operator+(const Vector& v) const
{
	return Point(x + v.x, y + v.y, z + v.z);
}

Point Point::operator-(const Vector& v) const
{
	return Point(x - v.x, y - v.y, z - v.z);
}

void Point::DrawBox(const Vector &halfSize, const Vector &colour, Vector &position) {
		float halfWidth = halfSize.x;
	float halfHeight = halfSize.y;
		float halfDepth = halfSize.z;
		// set the object's color
		glColor3f(colour.x, colour.y, colour.z);
	
	// create the vertex positions
		Vector vertices[8]={	
		Vector(halfWidth + position.x,halfHeight + position.y ,halfDepth + position.z),
		Vector(-halfWidth + position.x,halfHeight+ position.y,halfDepth+ position.z),
		Vector(halfWidth+ position.x,-halfHeight+ position.y,halfDepth+ position.z),	
		Vector(-halfWidth+ position.x,-halfHeight+ position.y,halfDepth+ position.z),	
		Vector(halfWidth+ position.x,halfHeight+ position.y,-halfDepth+ position.z),
		Vector(-halfWidth+ position.x,halfHeight+ position.y,-halfDepth+ position.z),	
		Vector(halfWidth+ position.x,-halfHeight+ position.y,-halfDepth+ position.z),	
		Vector(-halfWidth+ position.x,-halfHeight+ position.y,-halfDepth+ position.z)
		};
	
		// create the indexes for each triangle, using the 
		// vertices above. Make it static so we don't waste 
		// processing time recreating it over and over again
		static int indices[36] = {
			0,1,2,
			3,2,1,
			4,0,6,
			6,0,2,
			5,1,4,
			4,1,0,
			7,3,1,
			7,1,5,
			5,4,7,
			7,4,6,
			7,2,3,
			7,6,2};
		// start processing vertices as triangles
		glBegin (GL_TRIANGLES);
	
		// increment the loop by 3 each time since we create a 
	// triangle with 3 vertices at a time.
	
	for (int i = 0; i < 36; i += 3) {
			// get the three vertices for the triangle based
		// on the index values set above
		// use const references so we don't copy the object
		// (a good rule of thumb is to never allocate/deallocate
			// memory during *every* render/update call. This should 
		// only happen sporadically)
			const Vector &vert1 = vertices[indices[i]];
			const Vector &vert2 = vertices[indices[i+1]];
		const Vector &vert3 = vertices[indices[i+2]];
	
			// create a normal that is perpendicular to the 
			// face (use the cross product)
			//Vector normal = (vert3-vert1).cross(vert2-vert1);
			//normal.normalize ();
	
		// set the normal for the subsequent vertices
			//glNormal3f(normal.getX(),normal.getY(),normal.getZ());
	
			// create the vertices
			glVertex3f (vert1.x, vert1.y, vert1.z);
			glVertex3f (vert2.x, vert2.y, vert2.z);
			glVertex3f (vert3.x, vert3.y, vert3.z);
		}
	
		// stop processing vertices
	glEnd();
}