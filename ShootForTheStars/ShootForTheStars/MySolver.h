/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2017/07
----------------------------------------------*/
#ifndef __MYSOLVER_H_
#define __MYSOLVER_H_

#include "MyRigidBody.h"

namespace Simplex
{

class MySolver
{
	vector3 m_v3Acceleration = ZERO_V3; //Acceleration of the MySolver
	vector3 m_v3Position = ZERO_V3; //Position of the MySolver
	vector3 m_v3Velocity = ZERO_V3; //Velocity of the MySolver
	float m_fMass = 1.0f; //Mass of the solver
	float m_fFriction = 0.1f; //Power of the friction to be applied to the object
	float m_fGravity = -0.035f; //Force of gravity

	uint m_uTimesBounced = 1; // Number of times the ball has bounced
public:
	/*
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT: object instance
	*/
	MySolver(void);
	/*
	USAGE: Copy Constructor
	ARGUMENTS: object to copy
	OUTPUT: object instance
	*/
	MySolver(MySolver const& other);
	/*
	USAGE: Copy Assignment Operator
	ARGUMENTS: object to copy
	OUTPUT: this instance
	*/
	MySolver& operator=(MySolver const& other);
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~MySolver(void);
	/*
	USAGE: Content Swap
	ARGUMENTS: object to swap content with
	OUTPUT: ---
	*/
	void Swap(MySolver& other);

	/*
	USAGE: Sets the position of the solver
	ARGUMENTS: vector3 a_v3Position -> position to set
	OUTPUT: ---
	*/
	void SetPosition(vector3 a_v3Position);
	/*
	USAGE: Gets the position of the solver
	ARGUMENTS: ---
	OUTPUT: position of the solver
	*/
	vector3 GetPosition(void);

	/*
	USAGE: Sets the velocity of the solver
	ARGUMENTS: vector3 a_v3Velocity -> velocity to set
	OUTPUT: ---
	*/
	void SetVelocity(vector3 a_v3Velocity);
	/*
	USAGE: Gets the velocity of the solver
	ARGUMENTS: ---
	OUTPUT: velocity of the solver
	*/
	vector3 GetVelocity(void);

	/*
	USAGE: Sets the mass of the solver
	ARGUMENTS: float a_fMass -> mass to set
	OUTPUT: ---
	*/
	void SetMass(float a_fMass);
	/*
	USAGE: Gets mass of the solver
	ARGUMENTS: ---
	OUTPUT: mass of the object
	*/
	float GetMass(void);

	/*
	USAGE: Sets the friction of the solver
	ARGUMENTS: float a_fFriction -> friction to set
	OUTPUT: ---
	*/
	void SetFriction(float a_fFriction);
	/*
	USAGE: Gets the friction of the solver
	ARGUMENTS: ---
	OUTPUT: friction of the object
	*/
	float GetFriction(void);

	void SetGravity(float a_fGravity);

	float GetGravity(void);

	/*
	USAGE: Applies friction to the movement
	ARGUMENTS: float a_fFriction = 0.1f -> friction to apply negative friction gets minimized to 0.01f
	OUTPUT: ---
	*/
	void ApplyFriction(float a_fFriction = 0.1f);
	/*
	USAGE: Applies a force to the solver
	ARGUMENTS: vector3 a_v3Force -> Force to apply
	OUTPUT: ---
	*/
	void ApplyForce(vector3 a_v3Force);
	/*
	USAGE: Updates the MySolver
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Update(void);
	/*
	USAGE: Resolve the collision between two solvers
	ARGUMENTS: MySolver* a_pOther -> other solver to resolve collision with
	OUTPUT: ---
	*/
	void ResolveCollision(MySolver* a_pOther);
private:
	/*
	Usage: Deallocates member fields
	Arguments: ---
	Output: ---
	*/
	void Release(void);
	/*
	Usage: Allocates member fields
	Arguments: ---
	Output: ---
	*/
	void Init(void);
};//class

} //namespace Simplex

#endif //__SOLVER_H_

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/