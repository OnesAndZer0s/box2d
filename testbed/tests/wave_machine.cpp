// MIT License

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
#include "test.h"

class WaveMachine : public Test {
  public:

    WaveMachine() {
      b2Body* ground = NULL;
      {
        b2BodyDef bd;
        ground = m_world->CreateBody( &bd );
      }

      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.allowSleep = false;
        bd.position.Set( 0.0f, 1.0f );
        b2Body* body = m_world->CreateBody( &bd );

        b2PolygonShape shape;
        shape.SetAsBox( 0.05f, 1.0f, b2Vec2( 2.0f, 0.0f ), 0.0 );
        body->CreateFixture( &shape, 5.0f );
        shape.SetAsBox( 0.05f, 1.0f, b2Vec2( -2.0f, 0.0f ), 0.0 );
        body->CreateFixture( &shape, 5.0f );
        shape.SetAsBox( 2.0f, 0.05f, b2Vec2( 0.0f, 1.0f ), 0.0 );
        body->CreateFixture( &shape, 5.0f );
        shape.SetAsBox( 2.0f, 0.05f, b2Vec2( 0.0f, -1.0f ), 0.0 );
        body->CreateFixture( &shape, 5.0f );

        b2RevoluteJointDef jd;
        jd.bodyA = ground;
        jd.bodyB = body;
        jd.localAnchorA.Set( 0.0f, 1.0f );
        jd.localAnchorB.Set( 0.0f, 0.0f );
        jd.referenceAngle = 0.0f;
        jd.motorSpeed = 0.05f * b2_pi;
        jd.maxMotorTorque = 1e7f;
        jd.enableMotor = true;
        m_joint = (b2RevoluteJoint*) m_world->CreateJoint( &jd );
      }

      m_particleSystem->SetRadius( 0.025f );
      const uint32 particleType = Test::GetParticleParameterValue();
      m_particleSystem->SetDamping( 0.2f );

      {
        b2ParticleGroupDef pd;
        pd.flags = particleType;

        b2PolygonShape shape;
        shape.SetAsBox( 0.9f, 0.9f, b2Vec2( 0.0f, 1.0f ), 0.0 );

        pd.shape = &shape;
        b2ParticleGroup* const group = m_particleSystem->CreateParticleGroup( pd );
        if( pd.flags & b2_colorMixingParticle )
          ColorParticleGroup( group, 0 );
      }

      m_time = 0;
    }

    void Step( Settings& settings ) {
      Test::Step( settings );
      if( settings.m_hertz > 0 )
        m_time += 1 / settings.m_hertz;
      m_joint->SetMotorSpeed( 0.05f * cosf( m_time ) * b2_pi );
    }

    static Test* Create() {
      return new WaveMachine;
    }

    b2RevoluteJoint* m_joint;
    float m_time;
};

static int testIndex = RegisterTest( "Particles", "Wave Machine", WaveMachine::Create );
