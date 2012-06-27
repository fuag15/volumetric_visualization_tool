#ifndef ARCBALL_H
#define ARCBALL_H

#include <GL/gl.h>
#include <GL/glu.h>

/* Arcball, written by Bradley Smith, March 24, 2006
 *
 * Using the arcball:
 *   Call arcball_setzoom after setting up the projection matrix.
 *
 *     The arcball, by default, will act as if a sphere with the given
 *     radius, centred on the origin, can be directly manipulated with
 *     the mouse. Clicking on a point should drag that point to rest under
 *     the current mouse position. eye is the position of the eye relative
 *     to the origin. up is unused.
 *
 *     Alternatively, pass the value: (-radius/|eye|)
 *     This puts the arcball in a mode where the distance the mouse moves
 *     is equivalent to rotation along the axes. This acts much like a
 *     trackball. (It is for this mode that the up vector is required,
 *     which must be a unit vector.)
 *
 *     You should call arcball_setzoom after use of gluLookAt.
 *     gluLookAt(eye.x,eye.y,eye.z, ?,?,?, up.x,up.y,up.z);
 *     The arcball derives its transformation information from the
 *     openGL projection and viewport matrices. (modelview is ignored)
 *
 *     If looking at a point different from the origin, the arcball will still
 *     act as if it centred at (0,0,0). (You can use this to translate
 *     the arcball to some other part of the screen.)
 *
 *   Call arcball_start with a mouse position, and the arcball will
 *     be ready to manipulate. (Call on mouse button down.)
 *   Call arcball_move with a mouse position, and the arcball will
 *     find the rotation necessary to move the start mouse position to
 *     the current mouse position on the sphere. (Call on mouse move.)
 *   Call arcball_rotate after resetting the modelview matrix in your
 *     drawing code. It will call glRotate with its current rotation.
 *   Call arcball_reset if you wish to reset the arcball rotation.
 */

#include <cmath> // for sqrt

class vec // simple 3D vector class
{
  public:
    float x,y,z;

    vec() {}
    vec( float xx, float yy, float zz )
    { x=xx; y=yy; z=zz; }

    inline vec operator + (vec t) // addition
    { return vec(x+t.x,y+t.y,z+t.z); }
    inline vec operator - (vec t) // subtraction
    { return vec(x-t.x,y-t.y,z-t.z); }
    inline vec operator * (float t) // dot product
    { return vec(x*t,y*t,z*t); }
    inline float operator * (vec t) // scalar product
    { return x*t.x + y*t.y + z*t.z; }
    inline vec operator ^ (vec t) // cross product
    { return vec( y*t.z-z*t.y, t.x*z-x*t.z, x*t.y-y*t.x ); }

    inline float length() // pythagorean length
    { return sqrt(x*x + y*y + z*z); }
    inline vec unit() // normalized to a length of 1
    { float l = length();
      if (l == 0.0) return vec(0.0,0.0,0.0);
      return vec(x/l,y/l,z/l); }
    inline bool zero() // returns true if a zero vector
    { return x==0 && y==0 && z==0; }
    inline bool equals(vec t) // returns true if exactly equal
    { return x==t.x && y==t.y && z==t.z; }
};


class arcball
{
	public:
		arcball();
		void arcball_setzoom(float radius, vec eye, vec up);
		GLfloat* arcball_rotate();
		void arcball_reset();
		void arcball_start(int mx, int my);
		void arcball_move(int mx, int my);
		GLfloat ab_quat[16];
		GLfloat ab_last[16];
		GLfloat ab_next[16];

		// the distance from the origin to the eye
		GLfloat ab_zoom;
		GLfloat ab_zoom2;
		// the radius of the arcball
		GLfloat ab_sphere;
		GLfloat ab_sphere2;
		// the distance from the origin of the plane that intersects
		// the edge of the visible sphere (tangent to a ray from the eye)
		GLfloat ab_edge;
		// whether we are using a sphere or plane
		bool ab_planar;
		GLfloat ab_planedist;

		vec ab_start;
		vec ab_curr;
		vec ab_eye;
		vec ab_eyedir;
		vec ab_up;
		vec ab_out;

		GLdouble ab_glp[16];
		GLdouble ab_glm[16];
		int ab_glv[4];
		
		void quaternion(GLfloat* q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		void quatidentity(GLfloat* q);
		void quatcopy(GLfloat* dst, GLfloat* src);
		void quatnext(GLfloat* dest, GLfloat* left, GLfloat* right);
		vec edge_coords(vec m);
		vec sphere_coords(GLdouble mx, GLdouble my);
		vec planar_coords(GLdouble mx, GLdouble my);
};



#endif
