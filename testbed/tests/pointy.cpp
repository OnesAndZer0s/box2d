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

// Test behavior when particles fall on a convex ambigious Body
// contact fixture junction.
class Pointy : public Test {
  public:
    Pointy() {
      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );

        // Construct a triangle out of many polygons to ensure there's no
        // issue with particles falling directly on an ambiguous corner

        const float xstep = 1.0;
        for( float x = -10.0; x < 10.0; x += xstep ) {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 3 ] = {
              b2Vec2( x, -10.0 ),
              b2Vec2( x + xstep, -10.0 ),
              b2Vec2( 0.0, 25.0 ) };
          shape.Set( vertices, 3 );
          ground->CreateFixture( &shape, 0.0f );
        }
      }

      m_particleSystem->SetRadius( 0.25f );
      const uint32 particleType = Test::GetParticleParameterValue();
      if( particleType == b2_waterParticle )
        m_particleSystem->SetDamping( 0.2f );

      // Create killfield shape and transform
      m_killfieldShape = b2PolygonShape();
      m_killfieldShape.SetAsBox( 50.0, 1.0 );

      // Put this at the bottom of the world
      m_killfieldTransform = b2Transform();
      b2Vec2 loc = b2Vec2( -25, 1 );
      m_killfieldTransform.Set( loc, 0 );
    }

    void Step( Settings& settings ) override // override from Test
    {
      Test::Step( settings );

      int32 flags = Test::GetParticleParameterValue();
      b2ParticleDef pd;

      pd.position.Set( 0.0, 33.0 );
      pd.velocity.Set( 0.0, -1.0 );
      pd.flags = flags;

      if( flags & ( b2_springParticle | b2_elasticParticle ) ) {
        int32 count = m_particleSystem->GetParticleCount();
        pd.velocity.Set( count & 1 ? -1.0f : 1.0f, -5.0f );
        pd.flags |= b2_reactiveParticle;
      }

      m_particleSystem->CreateParticle( pd );

      // kill every particle near the bottom of the screen
      m_particleSystem->DestroyParticlesInShape( m_killfieldShape, m_killfieldTransform );
    }

    static Test* Create() {
      return new Pointy;
    }

  private:
    b2PolygonShape m_killfieldShape;
    b2Transform m_killfieldTransform;
};

static int testIndex = RegisterTest( "Particles", "Pointy", Pointy::Create );
