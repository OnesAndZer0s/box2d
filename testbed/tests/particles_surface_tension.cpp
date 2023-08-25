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

class ParticlesSurfaceTension : public Test {
  public:

    ParticlesSurfaceTension() {
      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -4, -1 ),
              b2Vec2( 4, -1 ),
              b2Vec2( 4, 0 ),
              b2Vec2( -4, 0 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -4, -0.1f ),
              b2Vec2( -2, -0.1f ),
              b2Vec2( -2, 2 ),
              b2Vec2( -4, 2 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( 2, -0.1f ),
              b2Vec2( 4, -0.1f ),
              b2Vec2( 4, 2 ),
              b2Vec2( 2, 2 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }
      }

      m_particleSystem->SetRadius( 0.035f );

      {
        b2CircleShape shape;
        shape.m_p.Set( 0, 2 );
        shape.m_radius = 0.5f;
        b2ParticleGroupDef pd;
        pd.flags = b2_tensileParticle | b2_colorMixingParticle;
        pd.shape = &shape;
        pd.color.Set( 255, 0, 0, 255 );
        m_particleSystem->CreateParticleGroup( pd );
      }

      {
        b2CircleShape shape;
        shape.m_p.Set( -1, 2 );
        shape.m_radius = 0.5f;
        b2ParticleGroupDef pd;
        pd.flags = b2_tensileParticle | b2_colorMixingParticle;
        pd.shape = &shape;
        pd.color.Set( 0, 255, 0, 255 );
        m_particleSystem->CreateParticleGroup( pd );
      }

      {
        b2PolygonShape shape;
        const b2Vec2 vertices [ 4 ] = {
            b2Vec2( 0, 3 ),
            b2Vec2( 2, 3 ),
            b2Vec2( 2, 3.5f ),
            b2Vec2( 0, 3.5f ) };
        shape.Set( vertices, 4 );
        b2ParticleGroupDef pd;
        pd.flags = b2_tensileParticle | b2_colorMixingParticle;
        pd.shape = &shape;
        pd.color.Set( 0, 0, 255, 255 );
        m_particleSystem->CreateParticleGroup( pd );
      }

      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2CircleShape shape;
        shape.m_p.Set( 0, 8 );
        shape.m_radius = 0.5f;
        body->CreateFixture( &shape, 0.5f );
      }
    }

    static Test* Create() {
      return new ParticlesSurfaceTension;
    }
};

static int testIndex = RegisterTest( "Particles", "Surface Tension", ParticlesSurfaceTension::Create );
