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

class LiquidTimer : public Test {
  public:

    LiquidTimer() {
      // Setup particle parameters.
      Test::SetParticleParameters( k_paramDef, k_paramDefCount );

      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );

        b2ChainShape shape;
        const b2Vec2 vertices [ 4 ] = {
            b2Vec2( -2, 0 ),
            b2Vec2( 2, 0 ),
            b2Vec2( 2, 4 ),
            b2Vec2( -2, 4 ) };
        shape.CreateLoop( vertices, 4 );
        ground->CreateFixture( &shape, 0.0f );
      }

      m_particleSystem->SetRadius( 0.025f );
      {
        b2PolygonShape shape;
        shape.SetAsBox( 2, 0.4f, b2Vec2( 0, 3.6f ), 0 );
        b2ParticleGroupDef pd;
        pd.flags = Test::GetParticleParameterValue();
        pd.shape = &shape;
        b2ParticleGroup* const group = m_particleSystem->CreateParticleGroup( pd );
        if( pd.flags & b2_colorMixingParticle )
          ColorParticleGroup( group, 0 );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( -2, 3.2f ), b2Vec2( -1.2f, 3.2f ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( -1.1f, 3.2f ), b2Vec2( 2, 3.2f ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( -1.2f, 3.2f ), b2Vec2( -1.2f, 2.8f ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( -1.1f, 3.2f ), b2Vec2( -1.1f, 2.8f ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( -1.6f, 2.4f ), b2Vec2( 0.8f, 2 ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( 1.6f, 1.6f ), b2Vec2( -0.8f, 1.2f ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( -1.2f, 0.8f ), b2Vec2( -1.2f, 0 ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( -0.4f, 0.8f ), b2Vec2( -0.4f, 0 ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( 0.4f, 0.8f ), b2Vec2( 0.4f, 0 ) );
        body->CreateFixture( &shape, 0.1f );
      }

      {
        b2BodyDef bd;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( 1.2f, 0.8f ), b2Vec2( 1.2f, 0 ) );
        body->CreateFixture( &shape, 0.1f );
      }
    }

    static Test* Create() {
      return new LiquidTimer;
    }

    static const ParticleParameter::Value k_paramValues [];
    static const ParticleParameter::Definition k_paramDef [];
    static const uint32 k_paramDefCount;
};

const ParticleParameter::Value LiquidTimer::k_paramValues [] =
    {
        {b2_tensileParticle | b2_viscousParticle,
         ParticleParameter::k_DefaultOptions, "tensile + viscous"},
};
const ParticleParameter::Definition LiquidTimer::k_paramDef [] =
    {
        {LiquidTimer::k_paramValues,
         B2_ARRAY_SIZE( LiquidTimer::k_paramValues )},
        { ParticleParameter::k_particleTypesPtr,
         ParticleParameter::k_particleTypesCount    },
};
const uint32 LiquidTimer::k_paramDefCount =
    B2_ARRAY_SIZE( LiquidTimer::k_paramDef );

static int testIndex = RegisterTest( "Particles", "Liquid Timer", LiquidTimer::Create );
