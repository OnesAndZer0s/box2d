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

// Optionally disables particle / fixture and particle / particle contacts.
class ParticleContactDisabler : public b2ContactFilter {
  public:
    ParticleContactDisabler():
        m_enableFixtureParticleCollisions( true ),
        m_enableParticleParticleCollisions( true ) {
    }

    virtual ~ParticleContactDisabler() {}

    // Blindly enable / disable collisions between fixtures and particles.
    virtual bool ShouldCollide( b2Fixture* fixture,
        b2ParticleSystem* particleSystem,
        int32 particleIndex ) {
      B2_NOT_USED( fixture );
      B2_NOT_USED( particleSystem );
      B2_NOT_USED( particleIndex );
      return m_enableFixtureParticleCollisions;
    }

    // Blindly enable / disable collisions between particles.
    virtual bool ShouldCollide( b2ParticleSystem* particleSystem,
        int32 particleIndexA, int32 particleIndexB ) {
      B2_NOT_USED( particleSystem );
      B2_NOT_USED( particleIndexA );
      B2_NOT_USED( particleIndexB );
      return m_enableParticleParticleCollisions;
    }

  public:
    bool m_enableFixtureParticleCollisions;
    bool m_enableParticleParticleCollisions;
};

class ParticleCollisionFilter : public Test {
  public:

    ParticleCollisionFilter() {
      // must also set b2_particleContactFilterParticle and
      // b2_fixtureContactFilterParticle flags for particle group
      m_world->SetContactFilter( &m_contactDisabler );

      m_world->SetGravity( b2Vec2( 0, 0 ) );

      // Create the container.
      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );
        b2ChainShape shape;
        const b2Vec2 vertices [ 4 ] = {
            b2Vec2( -kBoxSize, -kBoxSize + kOffset ),
            b2Vec2( kBoxSize, -kBoxSize + kOffset ),
            b2Vec2( kBoxSize, kBoxSize + kOffset ),
            b2Vec2( -kBoxSize, kBoxSize + kOffset ) };
        shape.CreateLoop( vertices, 4 );
        b2FixtureDef def;
        def.shape = &shape;
        def.density = 0;
        def.restitution = 1.0;
        ground->CreateFixture( &def );
      }

      // create the particles
      m_particleSystem->SetRadius( 0.5f );
      {
        b2PolygonShape shape;
        shape.SetAsBox( 1.5f, 1.5f, b2Vec2( kBoxSizeHalf, kBoxSizeHalf + kOffset ), 0.0f );
        b2ParticleGroupDef pd;
        pd.shape = &shape;
        pd.flags = b2_powderParticle | b2_particleContactFilterParticle | b2_fixtureContactFilterParticle;
        m_particleGroup =
            m_particleSystem->CreateParticleGroup( pd );

        b2Vec2* velocities =
            m_particleSystem->GetVelocityBuffer() +
            m_particleGroup->GetBufferIndex();
        for( int i = 0; i < m_particleGroup->GetParticleCount(); ++i ) {
          b2Vec2& v = *( velocities + i );
          v.Set( RandomFloat(), RandomFloat() );
          v.Normalize();
          v *= kSpeedup;
        }
      }
    }

    void Step( Settings& settings ) override {
      Test::Step( settings );

      const int32 index = m_particleGroup->GetBufferIndex();
      b2Vec2* const velocities =
          m_particleSystem->GetVelocityBuffer() + index;
      for( int32 i = 0; i < m_particleGroup->GetParticleCount(); i++ ) {
        // Add energy to particles based upon the temperature.
        b2Vec2& v = velocities [ i ];
        v.Normalize();
        v *= kSpeedup;
      }

      // key help
      {
        static const char* k_keys [] = {
            "Keys: (a) toggle Fixture collisions",
            "   (s) toggle particle collisions" };
        for( uint32 i = 0; i < B2_ARRAY_SIZE( k_keys ); ++i ) {
          g_debugDraw.DrawString( 5, m_textLine, k_keys [ i ] );
          m_textLine += DRAW_STRING_NEW_LINE;
        }
      }
    }

    void Keyboard( int key ) override {
      switch( key ) {
        case GLFW_KEY_A:
          ToggleFixtureCollisions();
          break;
        case GLFW_KEY_S:
          ToggleParticleCollisions();
          break;
        default:
          Test::Keyboard( key );
          break;
      }
    }

    void ToggleFixtureCollisions() {
      m_contactDisabler.m_enableFixtureParticleCollisions = !m_contactDisabler.m_enableFixtureParticleCollisions;
    }

    void ToggleParticleCollisions() {
      m_contactDisabler.m_enableParticleParticleCollisions = !m_contactDisabler.m_enableParticleParticleCollisions;
    }

    static Test* Create() {
      return new ParticleCollisionFilter;
    }

  private:
    ParticleContactDisabler m_contactDisabler;
    b2ParticleGroup* m_particleGroup;

    static const float kBoxSize;
    static const float kBoxSizeHalf;
    static const float kOffset;
    static const float kParticlesContainerSize;
    static const float kSpeedup;
};

const float ParticleCollisionFilter::kBoxSize = 10.0f;
const float ParticleCollisionFilter::kBoxSizeHalf = kBoxSize / 2;
const float ParticleCollisionFilter::kOffset = 20.0f;
const float ParticleCollisionFilter::kParticlesContainerSize = kOffset + 0.5f;
const float ParticleCollisionFilter::kSpeedup = 8.0f;

static int testIndex = RegisterTest( "Particles", "Collision Filter", ParticleCollisionFilter::Create );
