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

class DamBreak : public Test {
  public:

    DamBreak() {
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
      m_particleSystem->SetDamping( 0.2f );

      {
        b2PolygonShape shape;
        shape.SetAsBox( 0.8f, 1.0f, b2Vec2( -1.2f, 1.01f ), 0 );
        b2ParticleGroupDef pd;
        pd.flags = Test::GetParticleParameterValue();
        pd.shape = &shape;
        b2ParticleGroup* const group = m_particleSystem->CreateParticleGroup( pd );
        if( pd.flags & b2_colorMixingParticle )
          ColorParticleGroup( group, 0 );
      }
    }

    static Test* Create() {
      return new DamBreak;
    }
};

static int testIndex = RegisterTest( "Particles", "Dam Break", DamBreak::Create );
