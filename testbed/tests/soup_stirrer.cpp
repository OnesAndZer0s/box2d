#include <math.h>

// Soup stirred by a circular dynamic body.
// A force vector (following a circle) is continuously applied to the body
// while by default the body is attached to a joint restricting motion
// to the x-// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "settings.h"
#include "soup.cpp"
#include "test.h"

#include <math.h>

class SoupStirrer : public Soup {
  public:
    SoupStirrer():
        m_stirrer( NULL ),
        m_joint( NULL ),
        m_oscillationOffset( 0.0f ) {
      m_particleSystem->SetDamping( 1.0f );

      // Shape of the stirrer.
      b2CircleShape shape;
      shape.m_p.Set( 0, 0.7f );
      shape.m_radius = 0.4f;

      // Create the stirrer.
      b2BodyDef bd;
      bd.type = b2_dynamicBody;
      m_stirrer = m_world->CreateBody( &bd );
      m_stirrer->CreateFixture( &shape, 1.0f );

      // Destroy all particles under the stirrer.
      b2Transform xf;
      xf.SetIdentity();
      m_particleSystem->DestroyParticlesInShape( shape, xf );

      // By default attach the body to a joint to restrict movement.
      CreateJoint();
    }

    // Create a joint to fix the stirrer to a single axis of movement.
    void CreateJoint() {
      b2Assert( !m_joint );
      // Create a prismatic joint and connect to the ground, and have it
      // slide along the x axis.
      // Disconnect the body from this joint to have more fun.
      b2PrismaticJointDef prismaticJointDef;
      prismaticJointDef.bodyA = m_ground;
      prismaticJointDef.bodyB = m_stirrer;
      prismaticJointDef.collideConnected = true;
      prismaticJointDef.localAxisA.Set( 1, 0 );
      prismaticJointDef.localAnchorA = m_stirrer->GetPosition();
      m_joint = m_world->CreateJoint( &prismaticJointDef );
    }

    // Enable the joint if it's disabled, disable it if it's enabled.
    virtual void ToggleJoint() {
      if( m_joint ) {
        m_world->DestroyJoint( m_joint );
        m_joint = 0;
      } else {
        CreateJoint();
      }
    }

    // Press "t" to enable / disable the joint restricting the stirrer's
    // movement.
    void Keyboard( int key ) override {
      switch( key ) {
        case GLFW_KEY_T:
          ToggleJoint();
          break;
        default:
          Soup::Keyboard( key );
          break;
      }
    }

    // Click the soup to toggle between enabling / disabling the joint.
    void MouseUp( const b2Vec2& p ) override {
      if( InSoup( p ) )
        ToggleJoint();
    }

    // Determine whether a point is in the soup.
    bool InSoup( const b2Vec2& pos ) {
      // The soup dimensions are from the container initialization in the
      // Soup test.
      return pos.y > -1.0f && pos.y < 2.0f && pos.x > -3.0f && pos.x < 3.0f;
    }

    // Apply a force to the stirrer.
    void Step( Settings& settings ) override {
      // Magnitude of the force applied to the body.
      static const float k_forceMagnitude = 10.0f;
      // How often the force vector rotates.
      static const float k_forceOscillationPerSecond = 0.2f;
      static const float k_forceOscillationPeriod =
          1.0f / k_forceOscillationPerSecond;
      // Maximum speed of the body.
      static const float k_maxSpeed = 2.0f;

      m_oscillationOffset += ( 1.0f / settings.m_hertz );
      if( m_oscillationOffset > k_forceOscillationPeriod )
        m_oscillationOffset -= k_forceOscillationPeriod;

      // Calculate the force vector.
      const float forceAngle = m_oscillationOffset *
                               k_forceOscillationPerSecond * 2.0f * b2_pi;
      const b2Vec2 forceVector =
          b2Vec2( sinf( forceAngle ), cosf( forceAngle ) ) * k_forceMagnitude;

      // Only apply force to the body when it's within the soup.
      if( InSoup( m_stirrer->GetPosition() ) &&
          m_stirrer->GetLinearVelocity().Length() < k_maxSpeed )
        m_stirrer->ApplyForceToCenter( forceVector, true );
      Test::Step( settings );
    }

    static Test* Create() {
      return new SoupStirrer;
    }

  private:
    b2Body* m_stirrer;
    b2Joint* m_joint;
    float m_oscillationOffset;
};

static int testIndexTwo = RegisterTest( "Particles", "Soup-Stirrer", SoupStirrer::Create );
static int testIndex = RegisterTest( "Particles", "Soup", Soup::Create );
