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

#include "test.h"

// Call b2ParticleSystem::ApplyLinearImpulse or b2ParticleSystem::ApplyForce
// to all particles, in the direction indicated by the user.
class Impulse : public Test {
    enum {
      kBoxLeft = -2,
      kBoxRight = 2,
      kBoxBottom = 0,
      kBoxTop = 4,
    };

  public:
    Impulse() {
      m_useLinearImpulse = false;

      // Create the containing box.
      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );

        const b2Vec2 box [] = {
            b2Vec2( kBoxLeft, kBoxBottom ),
            b2Vec2( kBoxRight, kBoxBottom ),
            b2Vec2( kBoxRight, kBoxTop ),
            b2Vec2( kBoxLeft, kBoxTop ) };
        b2ChainShape shape;
        shape.CreateLoop( box, sizeof( box ) / sizeof( box [ 0 ] ) );
        ground->CreateFixture( &shape, 0.0f );
      }

      m_particleSystem->SetRadius( 0.025f );
      m_particleSystem->SetDamping( 0.2f );

      // Create the particles.
      {
        b2PolygonShape shape;
        shape.SetAsBox( 0.8f, 1.0f, b2Vec2( 0.0f, 1.01f ), 0 );
        b2ParticleGroupDef pd;
        pd.flags = Test::GetParticleParameterValue();
        pd.shape = &shape;
        b2ParticleGroup* const group =
            m_particleSystem->CreateParticleGroup( pd );
        if( pd.flags & b2_colorMixingParticle )
          ColorParticleGroup( group, 0 );
      }
    }

    void MouseUp( const b2Vec2& p ) override {
      Test::MouseUp( p );

      // Apply an impulse to the particles.
      const bool isInsideBox = kBoxLeft <= p.x && p.x <= kBoxRight &&
                               kBoxBottom <= p.y && p.y <= kBoxTop;
      if( isInsideBox ) {
        const b2Vec2 kBoxCenter( 0.5f * ( kBoxLeft + kBoxRight ),
            0.5f * ( kBoxBottom + kBoxTop ) );
        b2Vec2 direction = p - kBoxCenter;
        direction.Normalize();
        ApplyImpulseOrForce( direction );
      }
    }

    void Keyboard( int key ) override {
      switch( key ) {
        case GLFW_KEY_L:
          {
            m_useLinearImpulse = true;
          }
          break;

        case GLFW_KEY_F:
          {
            m_useLinearImpulse = false;
          }
          break;
      }
    }

    static Test* Create() {
      return new Impulse;
    }

  private:
    // Move the particles a bit more in 'direction' by apply a force or impulse
    // (depending on m_useLinearImpuse).
    void ApplyImpulseOrForce( const b2Vec2& direction ) {
      b2ParticleSystem* particleSystem = m_world->GetParticleSystemList();
      b2ParticleGroup* particleGroup = particleSystem->GetParticleGroupList();
      const int32 numParticles = particleGroup->GetParticleCount();

      if( m_useLinearImpulse ) {
        const float kImpulseMagnitude = 0.005f;
        const b2Vec2 impulse = kImpulseMagnitude * direction *
                               (float) numParticles;
        particleGroup->ApplyLinearImpulse( impulse );
      } else {
        const float kForceMagnitude = 1.0f;
        const b2Vec2 force = kForceMagnitude * direction *
                             (float) numParticles;
        particleGroup->ApplyForce( force );
      }
    }

    bool m_useLinearImpulse;
};

static int testIndex = RegisterTest( "Particles", "Impulse", Impulse::Create );
