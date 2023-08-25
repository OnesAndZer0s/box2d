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

#include <algorithm>

// ParticleVFX tracks the particle system that's created whenever two of the
// circular rigid bodies collide.
class ParticleVFX {
  public:

    // Creates a particle tracker, centered on origin.
    // Size is how big of a particle splash to make (in meters)
    // Speed is the scale of how fast to make the particles shoot out
    // Lifetime in seconds.
    ParticleVFX( b2ParticleSystem* particleSystem, const b2Vec2& origin,
        float size, float speed, float lifetime,
        uint32 particleFlags ) {
      // Create a circle to house the particles of size size
      b2CircleShape shape;
      shape.m_p = origin;
      shape.m_radius = size;

      // Create particle def of random color.
      b2ParticleGroupDef pd;
      pd.flags = particleFlags;
      pd.shape = &shape;
      m_origColor.Set( rand() % 256, rand() % 256, rand() % 256, 255 );
      pd.color = m_origColor;
      m_particleSystem = particleSystem;

      // Create a circle full of particles
      m_pg = m_particleSystem->CreateParticleGroup( pd );

      m_initialLifetime = m_remainingLifetime = lifetime;
      m_halfLifetime = m_initialLifetime * 0.5f;

      // Set particle initial velocity based on how far away it is from
      // origin, exploding outwards.
      int32 bufferIndex = m_pg->GetBufferIndex();
      b2Vec2* pos = m_particleSystem->GetPositionBuffer();
      b2Vec2* vel = m_particleSystem->GetVelocityBuffer();
      for( int i = bufferIndex; i < bufferIndex + m_pg->GetParticleCount();
           i++ ) {
        vel [ i ] = pos [ i ] - origin;
        vel [ i ] *= speed;
      }
    }

    ~ParticleVFX() {
      m_pg->DestroyParticles( false );
    }

    // Calculates the brightness of the particles.
    // Piecewise linear function where particle is at 1.0 brightness until
    // t = lifetime/2, then linear falloff until t = 0, scaled by
    // m_halfLifetime.
    float ColorCoeff() {
      if( m_remainingLifetime >= m_halfLifetime )
        return 1.0f;
      return 1.0f - ( ( m_halfLifetime - m_remainingLifetime ) /
                        m_halfLifetime );
    }

    void Step( float dt ) {
      if( m_remainingLifetime > 0.0f ) {
        m_remainingLifetime = std::max( m_remainingLifetime - dt, 0.0f );
        float coeff = ColorCoeff();

        b2Color* colors = m_particleSystem->GetColorBuffer();
        int bufferIndex = m_pg->GetBufferIndex();

        // Set particle colors all at once.
        for( int i = bufferIndex;
             i < bufferIndex + m_pg->GetParticleCount(); i++ ) {
          b2Color& c = colors [ i ];
          c *= coeff;
          c.a = m_origColor.a;
        }
      }
    }

    // Are the particles entirely black?
    bool IsDone() { return m_remainingLifetime <= 0.0f; }

  private:
    float m_initialLifetime;
    float m_remainingLifetime;
    float m_halfLifetime;
    b2ParticleGroup* m_pg;
    b2ParticleSystem* m_particleSystem;
    b2Color m_origColor;
};

// Creates test that contains a series of circular rigid bodies with
// initial velocity. They ricochet off one another, and when they
// touch they create a shower of sparks that pushes them further.
class Sparky : public Test {
  public:
    Sparky() {
      // Set up array of sparks trackers.
      m_VFXIndex = 0;

      for( int i = 0; i < c_maxVFX; i++ )
        m_VFX [ i ] = NULL;

      CreateWalls();
      m_particleSystem->SetRadius( 0.25f );

      // Create a list of circles that will spark.
      for( int i = 0; i < c_maxCircles; i++ ) {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2CircleShape shape;
        shape.m_p.Set( 3.0f * RandomFloat(),
            SHAPE_HEIGHT_OFFSET + SHAPE_OFFSET * i );
        shape.m_radius = 2;
        b2Fixture* f = body->CreateFixture( &shape, 0.5f );
        // Tag this as a sparkable body.
        f->GetUserData().pointer = 1;
      }

      Test::SetRestartOnParticleParameterChange( false );
      Test::SetParticleParameterValue( b2_powderParticle );
    }

    // Handles bodies colliding.
    void BeginContact( b2Contact* contact ) override {
      Test::BeginContact( contact );
      // Check to see if these are two circles hitting one another.
      if( contact->GetFixtureA()->GetUserData().pointer != NULL ||
          contact->GetFixtureB()->GetUserData().pointer != NULL ) {
        b2WorldManifold worldManifold;
        contact->GetWorldManifold( &worldManifold );

        // Note that we overwrite any contact; if there are two collisions
        // on the same frame, only the last one showers sparks.
        // Two collisions are rare, and this also guarantees we will not
        // run out of places to store ParticleVFX explosions.
        m_contactPoint = worldManifold.points [ 0 ];
        m_contact = true;
      }
    }

    void Step( Settings& settings ) override {
      const uint32 particleFlags = Test::GetParticleParameterValue();
      Test::Step( settings );

      // If there was a contacts...
      if( m_contact ) {
        // ...explode!
        AddVFX( m_contactPoint, particleFlags );
        m_contact = false;
      }

      // Step particle explosions.
      for( int i = 0; i < c_maxVFX; i++ ) {
        ParticleVFX* vfx = m_VFX [ i ];
        if( vfx == NULL )
          continue;
        vfx->Step( 1.0f / settings.m_hertz );
        if( vfx->IsDone() ) {
          delete vfx;
          m_VFX [ i ] = NULL;
        }
      }
    }

    // Create an explosion of particles at origin=p
    void AddVFX( const b2Vec2& p, uint32 particleFlags ) {
      ParticleVFX* vfx = m_VFX [ m_VFXIndex ];
      if( vfx != NULL ) {
        delete vfx;
        m_VFX [ m_VFXIndex ] = NULL;
      }
      m_VFX [ m_VFXIndex ] = new ParticleVFX(
          m_particleSystem, p, RandomFloat( 1.0f, 2.0f ), RandomFloat( 10.0f, 20.0f ),
          RandomFloat( 0.5f, 1.0f ), particleFlags );
      if( ++m_VFXIndex >= c_maxVFX )
        m_VFXIndex = 0;
    }

    void CreateWalls() {
      // Create the walls of the world.
      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -40, -10 ),
              b2Vec2( 40, -10 ),
              b2Vec2( 40, 0 ),
              b2Vec2( -40, 0 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -40, 40 ),
              b2Vec2( 40, 40 ),
              b2Vec2( 40, 50 ),
              b2Vec2( -40, 50 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -40, -1 ),
              b2Vec2( -20, -1 ),
              b2Vec2( -20, 40 ),
              b2Vec2( -40, 40 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( 20, -1 ),
              b2Vec2( 40, -1 ),
              b2Vec2( 40, 40 ),
              b2Vec2( 20, 40 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }
      }
    }

    static Test* Create() {
      return new Sparky;
    }

  private:
    int m_VFXIndex;
    // Total number of bodies in the world.
    static const int c_maxCircles = 6;
    // You will need to raise this if you add more bodies.
    static const int c_maxVFX = 50;

    // How high to start the stack of sparky shapes.
    static const float SHAPE_HEIGHT_OFFSET;
    // Starting offset between sparky shapes.
    static const float SHAPE_OFFSET;

    ParticleVFX* m_VFX [ c_maxVFX ];
    // Was there a contact this frame?
    bool m_contact;
    // Where was the contact?
    b2Vec2 m_contactPoint;
};

const float Sparky::SHAPE_HEIGHT_OFFSET = 7;
const float Sparky::SHAPE_OFFSET = 4.5;

static int testIndex = RegisterTest( "Examples", "Sparky", Sparky::Create );
