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

#include "../particle_emitter.h"
#include "settings.h"
#include "test.h"

// The "Multiple Systems" test uses two particle emitters to push a rigid body
// in opposing directions showing that particles from each system can interact
// with the same body and at the same time not interact with each other.
class MultipleParticleSystems : public Test {
  public:
    MultipleParticleSystems() {
      // Configure the default particle system's parameters.
      m_particleSystem->SetRadius( 0.05f );
      m_particleSystem->SetMaxParticleCount( k_maxParticleCount );
      m_particleSystem->SetDestructionByAge( true );

      // Create a secondary particle system.
      b2ParticleSystemDef particleSystemDef;
      particleSystemDef.radius = m_particleSystem->GetRadius();
      particleSystemDef.destroyByAge = true;
      m_particleSystem2 = m_world->CreateParticleSystem( &particleSystemDef );
      m_particleSystem2->SetMaxParticleCount( k_maxParticleCount );

      // Don't restart the test when changing particle types.
      Test::SetRestartOnParticleParameterChange( false );

      // Create the ground.
      {
        b2BodyDef bd;
        b2Body* const ground = m_world->CreateBody( &bd );
        b2PolygonShape shape;
        shape.SetAsBox( 5.0f, 0.1f );
        ground->CreateFixture( &shape, 0.0f );
      }

      // Create a dynamic body to push around.
      {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        b2Body* body = m_world->CreateBody( &bd );
        b2PolygonShape shape;
        b2Vec2 center( 0.0f, 1.2f );
        shape.SetAsBox( k_dynamicBoxSize.x, k_dynamicBoxSize.y, center,
            0.0f );
        body->CreateFixture( &shape, 0.0f );
        b2MassData massData = { k_boxMass, center, 0.0f };
        body->SetMassData( &massData );
      }

      // Initialize the emitters.
      for( uint32 i = 0; i < B2_ARRAY_SIZE( m_emitters ); ++i ) {
        const float mirrorAlongY = i & 1 ? -1.0f : 1.0f;
        RadialEmitter& emitter = m_emitters [ i ];
        emitter.SetPosition( b2Vec2( k_emitterPosition.x * mirrorAlongY,
            k_emitterPosition.y ) );
        emitter.SetSize( k_emitterSize );
        emitter.SetVelocity( b2Vec2( k_emitterVelocity.x * mirrorAlongY,
            k_emitterVelocity.y ) );
        emitter.SetEmitRate( k_emitRate );
        emitter.SetColor( i & 1 ? k_rightEmitterColor : k_leftEmitterColor );
        emitter.SetParticleSystem(
            i & 1 ? m_particleSystem2 : m_particleSystem );
      }
    }

    // Run a simulation step.
    void Step( Settings& settings ) override {
      const float dt = 1.0f / settings.m_hertz;
      Test::Step( settings );
      for( uint32 i = 0; i < B2_ARRAY_SIZE( m_emitters ); ++i )
        m_emitters [ i ].Step( dt, NULL, 0 );
    }

    // float GetDefaultViewZoom() const {
    //   return 0.2f;
    // }

    // Create the multiple particle systems test.
    static Test* Create() {
      return new MultipleParticleSystems;
    }

  private:
    b2ParticleSystem* m_particleSystem2;
    RadialEmitter m_emitters [ 2 ];

    // Maximum number of particles per system.
    static const int32 k_maxParticleCount;
    // Size of the box which is pushed around by particles.
    static const b2Vec2 k_dynamicBoxSize;
    // Mass of the box.
    static const float k_boxMass;
    // Emit rate of the emitters in particles per second.
    static const float k_emitRate;
    // Location of the left emitter (the position of the right one is
    // mirrored along the y-axis).
    static const b2Vec2 k_emitterPosition;
    // Starting velocity of particles from the left emitter (the velocity
    // of particles from the right emitter are mirrored along the y-axis).
    static const b2Vec2 k_emitterVelocity;
    // Size of particle emitters.
    static const b2Vec2 k_emitterSize;
    // Color of the left emitter's particles.
    static const b2Color k_leftEmitterColor;
    // Color of the right emitter's particles.
    static const b2Color k_rightEmitterColor;
};

const int32 MultipleParticleSystems::k_maxParticleCount = 500;
const b2Vec2 MultipleParticleSystems::k_dynamicBoxSize( 0.5f, 0.5f );
const float MultipleParticleSystems::k_boxMass = 1.0f;
const float MultipleParticleSystems::k_emitRate = 100.0f;
const b2Vec2 MultipleParticleSystems::k_emitterPosition( -5.0f, 4.0f );
const b2Vec2 MultipleParticleSystems::k_emitterVelocity( 7.0f, -4.0f );
const b2Vec2 MultipleParticleSystems::k_emitterSize( 1.0f, 1.0f );
const b2Color MultipleParticleSystems::k_leftEmitterColor(
    0x22, 0x33, 0xff, 0xff );
const b2Color MultipleParticleSystems::k_rightEmitterColor(
    0xff, 0x22, 0x11, 0xff );

static int testIndex = RegisterTest( "Particles", "Multiple Particle System", MultipleParticleSystems::Create );
