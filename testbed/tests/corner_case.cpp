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

class CornerCase : public Test {
  public:

    CornerCase() {
      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );

        // Construct a pathological corner intersection out of many
        // polygons to ensure there's no issue with particle oscillation
        // from many fixture contact impulses at the corner

        // left edge
        {
          b2PolygonShape shape;
          const b2Vec2 vertices [] = {
              b2Vec2( -20.0f, 30.0f ),
              b2Vec2( -20.0f, 0.0f ),
              b2Vec2( -25.0f, 0.0f ),
              b2Vec2( -25.0f, 30.f ) };
          shape.Set( vertices, B2_ARRAY_SIZE( vertices ) );
          ground->CreateFixture( &shape, 0.0f );
        }

        float x, y;
        const float yrange = 30.0f, ystep = yrange / 10.0f,
                    xrange = 20.0f, xstep = xrange / 2.0f;

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [] = {
              b2Vec2( -25.0f, 0.0f ),
              b2Vec2( 20.0f, 15.0f ),
              b2Vec2( 25.0f, 0.0f ) };
          shape.Set( vertices, B2_ARRAY_SIZE( vertices ) );
          ground->CreateFixture( &shape, 0.0f );
        }

        for( x = -xrange; x < xrange; x += xstep ) {
          b2PolygonShape shape;
          const b2Vec2 vertices [] = {
              b2Vec2( -25.0f, 0.0f ),
              b2Vec2( x, 15.0f ),
              b2Vec2( x + xstep, 15.0f ) };
          shape.Set( vertices, B2_ARRAY_SIZE( vertices ) );
          ground->CreateFixture( &shape, 0.0f );
        }

        for( y = 0.0; y < yrange; y += ystep ) {
          b2PolygonShape shape;
          const b2Vec2 vertices [] = {
              b2Vec2( 25.0f, y ),
              b2Vec2( 25.0f, y + ystep ),
              b2Vec2( 20.0f, 15.0f ) };
          shape.Set( vertices, B2_ARRAY_SIZE( vertices ) );
          ground->CreateFixture( &shape, 0.0f );
        }
      }

      m_particleSystem->SetRadius( 1.0f );
      const uint32 particleType = Test::GetParticleParameterValue();

      {
        b2CircleShape shape;
        shape.m_p.Set( 0, 35 );
        shape.m_radius = 12;
        b2ParticleGroupDef pd;
        pd.flags = particleType;
        pd.shape = &shape;
        b2ParticleGroup* const group =
            m_particleSystem->CreateParticleGroup( pd );
        if( pd.flags & b2_colorMixingParticle )
          ColorParticleGroup( group, 0 );
      }
    }

    static Test* Create() {
      return new CornerCase;
    }
};

static int testIndex = RegisterTest( "Particles", "Corner Case", CornerCase::Create );
