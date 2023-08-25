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

class Ramp : public Test {
  public:

    Ramp() {
      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );

        // Construct a ramp out of many polygons to ensure there's no
        // issue with particles moving across vertices

        float x, y;
        const float xstep = 5.0, ystep = 5.0;

        for( y = 30.0; y > 0.0; y -= ystep ) {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 3 ] = {
              b2Vec2( -25.0, y ),
              b2Vec2( -25.0, y - ystep ),
              b2Vec2( 0.0, 15.0 ) };
          shape.Set( vertices, 3 );
          ground->CreateFixture( &shape, 0.0f );
        }

        for( x = -25.0; x < 25.0; x += xstep ) {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 3 ] = {
              b2Vec2( x, 0.0 ),
              b2Vec2( x + xstep, 0.0 ),
              b2Vec2( 0.0, 15.0 ) };
          shape.Set( vertices, 3 );
          ground->CreateFixture( &shape, 0.0f );
        }
      }

      m_particleSystem->SetRadius( 0.25f );
      const uint32 particleType = Test::GetParticleParameterValue();
      if( particleType == b2_waterParticle )
        m_particleSystem->SetDamping( 0.2f );

      {
        b2CircleShape shape;
        shape.m_p.Set( -20, 33 );
        shape.m_radius = 3;
        b2ParticleGroupDef pd;
        pd.flags = particleType;
        pd.shape = &shape;
        b2ParticleGroup* const group = m_particleSystem->CreateParticleGroup( pd );
        if( pd.flags & b2_colorMixingParticle )
          ColorParticleGroup( group, 0 );
      }
    }

    static Test* Create() {
      return new Ramp;
    }
};

static int testIndex = RegisterTest( "Particles", "Ramp", Ramp::Create );
