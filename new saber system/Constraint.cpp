#include "Constraint.h"

MatMN Constraint::GetInvM() const
{
    MatMN invM(6, 6);

    invM.Zero();

    invM.rows[0][0] = a->invMass;
    invM.rows[1][1] = a->invMass;
    invM.rows[2][2] = a->invInertia;

    invM.rows[3][3] = b->invMass;
    invM.rows[4][4] = b->invMass;
    invM.rows[5][5] = b->invInertia;

    return invM;
}

VecN Constraint::GetVelocities() const
{
    VecN V(6);
    V[0] = a->velocity.x;
    V[1] = a->velocity.y;
    V[2] = a->angularvelocity;
    V[3] = b->velocity.x;
    V[4] = b->velocity.y;
    V[5] = b->angularvelocity;
    return V;
}


JointConstraint::JointConstraint() : Constraint(), jacobian(1,6), cachedLambda(1), bias(0.0f)
{
    cachedLambda.Zero();
}

JointConstraint::JointConstraint(Body* a, Body* b, const Vec2f& anchorPoint) 
    : Constraint(), jacobian(1,6), cachedLambda(1), bias(0.0f)
{
    cachedLambda.Zero();
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldSpaceToLocalSpace(anchorPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(anchorPoint);
}

void JointConstraint::PreSolve(const float dt)
{
    const Vec2f pa = a->LocalSpaceToWorldSpace(aPoint);
    const Vec2f pb = b->LocalSpaceToWorldSpace(bPoint);

    const Vec2f ra = pa - a->position;
    const Vec2f rb = pb - b->position;

    jacobian.Zero();

    Vec2f J1 = (pa - pb) * 2.0;
    jacobian.rows[0][0] = J1.x; // A linear velocity.x
    jacobian.rows[0][1] = J1.y; // A linear velocity.y

    float J2 = ra.Cross(pa - pb) * 2.0;
    jacobian.rows[0][2] = J2;   // A angular velocity

    Vec2f J3 = (pb - pa) * 2.0;
    jacobian.rows[0][3] = J3.x; // B linear velocity.x
    jacobian.rows[0][4] = J3.y; // B linear velocity.y

    float J4 = rb.Cross(pb - pa) * 2.0;
    jacobian.rows[0][5] = J4;   // B angular velocity

    //warm start 
    const MatMN Jt = jacobian.Transpose();
    VecN impulses = Jt * cachedLambda;

    a->ApplyImpulseLinear(Vec2f(impulses[0], impulses[1]));
    a->ApplyImpulseAngular(impulses[2]);
    b->ApplyImpulseLinear(Vec2f(impulses[3], impulses[4]));
    b->ApplyImpulseAngular(impulses[5]);

    const float beta = 0.5f;
    float C = (pb - pa).Dot(pb - pa);
    C = std::max(0.0f, C - 0.01f);
    bias = (beta / dt) * C;

}

void JointConstraint::Solve()
{
    const VecN V = GetVelocities();
    const MatMN invM = GetInvM();

    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();

    // Calculate the numerator
    MatMN lhs = J * invM *Jt; // A
    VecN rhs = J * V * -1.0f;  // b
    rhs[0] -= bias;
    // Solve the values of lambda using Ax=b (Gaus-Seidel method)
    VecN lambda = MatMN::SolveGaussSeidel(lhs, rhs);
    cachedLambda += lambda;

    // Compute the final impulses with direction and magnitude
    VecN impulses = Jt * lambda;

    a->ApplyImpulseLinear(Vec2f(impulses[0], impulses[1]));
    a->ApplyImpulseAngular(impulses[2]);
    b->ApplyImpulseLinear(Vec2f(impulses[3], impulses[4]));
    b->ApplyImpulseAngular(impulses[5]);


}

void JointConstraint::PostSolve()
{
}

PenetrationConstraint::PenetrationConstraint() : Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f)
{
    cachedLambda.Zero();
}

PenetrationConstraint::PenetrationConstraint(Body* a, Body* b, const Vec2f& aCollisionPoint, Vec2f& bCollisionPoint, const Vec2f& normal)
    : Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f)
{
   
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldSpaceToLocalSpace(aCollisionPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(bCollisionPoint);
    this->normal = a->WorldSpaceToLocalSpace(normal);
    cachedLambda.Zero();
}

void PenetrationConstraint::PreSolve(const float dt)
{
    const Vec2f pa = a->LocalSpaceToWorldSpace(aPoint);
    const Vec2f pb = b->LocalSpaceToWorldSpace(bPoint);
    Vec2f n = a->LocalSpaceToWorldSpace(normal);

    const Vec2f ra = pa - a->position;
    const Vec2f rb = pb - b->position;

    jacobian.Zero();

    Vec2f J1 = -n;
    jacobian.rows[0][0] = J1.x; // A linear velocity.x
    jacobian.rows[0][1] = J1.y; // A linear velocity.y

    float J2 = -ra.Cross(n);
    jacobian.rows[0][2] = J2;   // A angular velocity

    Vec2f J3 = n;
    jacobian.rows[0][3] = J3.x; // B linear velocity.x
    jacobian.rows[0][4] = J3.y; // B linear velocity.y

    float J4 = rb.Cross(n);
    jacobian.rows[0][5] = J4;   // B angular velocity

    //warm start 
   const MatMN Jt = jacobian.Transpose();
    VecN impulses = Jt * cachedLambda;

  a->ApplyImpulseLinear(Vec2f(impulses[0], impulses[1]));
  a->ApplyImpulseAngular(impulses[2]);
  b->ApplyImpulseLinear(Vec2f(impulses[3], impulses[4]));
  b->ApplyImpulseAngular(impulses[5]);

    const float beta = 0.2f;
    float C = (pb - pa).Dot(-n);
    C = std::min(0.0f, C + 0.01f);
    bias = (beta / dt) * C;
}

void PenetrationConstraint::Solve()
{
    const VecN V = GetVelocities();
    const MatMN invM = GetInvM();

    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();

    // Compute lambda using Ax=b (Gauss-Seidel method) 
    MatMN lhs = J * invM * Jt;  // A
    VecN rhs = J * V * -1.0f;   // b
    rhs[0] -= bias;
    VecN lambda = MatMN::SolveGaussSeidel(lhs, rhs);

   
    // Compute the impulses with both direction and magnitude
    VecN impulses = Jt * lambda;

    // Apply the impulses to both bodies 
    a->ApplyImpulseLinear(Vec2f(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(Vec2f(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse
}

void PenetrationConstraint::PostSolve()
{
}
