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

#include <memory.h>
#include <string.h>

class Soup : public Test {
  public:

    Soup() {
      // Disable the selection of wall and barrier particles for this test.
      InitializeParticleParameters( b2_wallParticle | b2_barrierParticle );

      {
        b2BodyDef bd;
        m_ground = m_world->CreateBody( &bd );

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -4, -1 ),
              b2Vec2( 4, -1 ),
              b2Vec2( 4, 0 ),
              b2Vec2( -4, 0 ) };
          shape.Set( vertices, 4 );
          m_ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -4, -0.1f ),
              b2Vec2( -2, -0.1f ),
              b2Vec2( -2, 2 ),
              b2Vec2( -4, 3 ) };
          shape.Set( vertices, 4 );
          m_ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( 2, -0.1f ),
              b2Vec2( 4, -0.1f ),
              b2Vec2( 4, 3 ),
              b2Vec2( 2, 2 ) };
          shape.Set( vertices, 4 );
          m_ground->CreateFixture( &shape, 0.0f );
        }
      }

      m_particleSystem->SetRadius( 0.035f );
      {
        b2PolygonShape shape;
        shape.SetAsBox( 2, 1, b2Vec2( 0, 1 ), 0 );
        b2ParticleGroupDef pd;
        pd.shape = &shape;
        pd.flags = Test::GetParticleParameterValue();
        b2ParticleGroup* const group =
            m_particleSystem->CreateParticleGroup( pd );
        if( pd.flags & b2_colorMixingParticle )
          ColorParticleGroup( group, 0 );
      }

      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2CircleShape shape;
        shape.m_p.Set( 0, 0.5f );
        shape.m_radius = 0.1f;
        body->CreateFixture( &shape, 0.1f );
        m_particleSystem->DestroyParticlesInShape( shape,
            body->GetTransform() );
      }

      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2PolygonShape shape;
        shape.SetAsBox( 0.1f, 0.1f, b2Vec2( -1, 0.5f ), 0 );
        body->CreateFixture( &shape, 0.1f );
        m_particleSystem->DestroyParticlesInShape( shape,
            body->GetTransform() );
      }

      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2PolygonShape shape;
        shape.SetAsBox( 0.1f, 0.1f, b2Vec2( 1, 0.5f ), 0.5f );
        body->CreateFixture( &shape, 0.1f );
        m_particleSystem->DestroyParticlesInShape( shape,
            body->GetTransform() );
      }

      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( 0, 2 ), b2Vec2( 0.1f, 2.1f ) );
        body->CreateFixture( &shape, 1 );
        b2MassData massData =
            { 0.1f, 0.5f * ( shape.m_vertex1 + shape.m_vertex2 ), 0.0f };
        body->SetMassData( &massData );
      }

      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( 0.3f, 2.0f ), b2Vec2( 0.4f, 2.1f ) );
        body->CreateFixture( &shape, 1 );
        b2MassData massData =
            { 0.1f, 0.5f * ( shape.m_vertex1 + shape.m_vertex2 ), 0.0f };
        body->SetMassData( &massData );
      }

      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2EdgeShape shape;
        shape.Set( b2Vec2( -0.3f, 2.1f ), b2Vec2( -0.2f, 2.0f ) );
        body->CreateFixture( &shape, 1 );
        b2MassData massData =
            { 0.1f, 0.5f * ( shape.m_vertex1 + shape.m_vertex2 ), 0.0f };
        body->SetMassData( &massData );
      }
    }

    static Test* Create() {
      return new Soup;
    }

  protected:
    b2Body* m_ground;
};
