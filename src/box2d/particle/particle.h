/*
 * Copyright (c) 2013 Google, Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef B2_PARTICLE
#define B2_PARTICLE

#include "box2d/common/draw.h"
#include "box2d/common/intrusive_list.h"
#include "box2d/common/math.h"
#include "box2d/common/settings.h"

struct b2Color;
class b2ParticleGroup;

/// @file

/// The particle type. Can be combined with the | operator.
enum b2ParticleFlag {
  /// Water particle.
  b2_waterParticle = 0,
  /// Removed after next simulation step.
  b2_zombieParticle = 1 << 1,
  /// Zero velocity.
  b2_wallParticle = 1 << 2,
  /// With restitution from stretching.
  b2_springParticle = 1 << 3,
  /// With restitution from deformation.
  b2_elasticParticle = 1 << 4,
  /// With viscosity.
  b2_viscousParticle = 1 << 5,
  /// Without isotropic pressure.
  b2_powderParticle = 1 << 6,
  /// With surface tension.
  b2_tensileParticle = 1 << 7,
  /// Mix color between contacting particles.
  b2_colorMixingParticle = 1 << 8,
  /// Call b2DestructionListener on destruction.
  b2_destructionListenerParticle = 1 << 9,
  /// Prevents other particles from leaking.
  b2_barrierParticle = 1 << 10,
  /// Less compressibility.
  b2_staticPressureParticle = 1 << 11,
  /// Makes pairs or triads with other particles.
  b2_reactiveParticle = 1 << 12,
  /// With high repulsive force.
  b2_repulsiveParticle = 1 << 13,
  /// Call b2ContactListener when this particle is about to interact with
  /// a rigid body or stops interacting with a rigid body.
  /// This results in an expensive operation compared to using
  /// b2_fixtureContactFilterParticle to detect collisions between
  /// particles.
  b2_fixtureContactListenerParticle = 1 << 14,
  /// Call b2ContactListener when this particle is about to interact with
  /// another particle or stops interacting with another particle.
  /// This results in an expensive operation compared to using
  /// b2_particleContactFilterParticle to detect collisions between
  /// particles.
  b2_particleContactListenerParticle = 1 << 15,
  /// Call b2ContactFilter when this particle interacts with rigid bodies.
  b2_fixtureContactFilterParticle = 1 << 16,
  /// Call b2ContactFilter when this particle interacts with other
  /// particles.
  b2_particleContactFilterParticle = 1 << 17,
};

/// A particle definition holds all the data needed to construct a particle.
/// You can safely re-use these definitions.
struct b2ParticleDef {
    b2ParticleDef() {
      flags = 0;
      position = b2Vec2_zero;
      velocity = b2Vec2_zero;
      color = b2Color_zero;
      lifetime = 0.0f;
      userData = NULL;
      group = NULL;
    }

#if LIQUIDFUN_EXTERNAL_LANGUAGE_API
    /// Set position with direct floats
    void SetPosition( float x, float y );

    /// Set color with direct ints.
    void SetColor( int32 r, int32 g, int32 b, int32 a );
#endif // LIQUIDFUN_EXTERNAL_LANGUAGE_API

    /// \brief Specifies the type of particle (see #b2ParticleFlag).
    ///
    /// A particle may be more than one type.
    /// Multiple types are chained by logical sums, for example:
    /// pd.flags = b2_elasticParticle | b2_viscousParticle
    uint32 flags;

    /// The world position of the particle.
    b2Vec2 position;

    /// The linear velocity of the particle in world co-ordinates.
    b2Vec2 velocity;

    /// The color of the particle.
    b2Color color;

    /// Lifetime of the particle in seconds.  A value <= 0.0f indicates a
    /// particle with infinite lifetime.
    float lifetime;

    /// Use this to store application-specific body data.
    void* userData;

    /// An existing particle group to which the particle will be added.
    b2ParticleGroup* group;
};

/// A helper function to calculate the optimal number of iterations.
int32 b2CalculateParticleIterations(
    float gravity, float radius, float timeStep );

/// Handle to a particle. Particle indices are ephemeral: the same index might
/// refer to a different particle, from frame-to-frame. If you need to keep a
/// reference to a particular particle across frames, you should acquire a
/// b2ParticleHandle. Use #b2ParticleSystem::GetParticleHandleFromIndex() to
/// retrieve the b2ParticleHandle of a particle from the particle system.
class b2ParticleHandle : public b2TypedIntrusiveListNode< b2ParticleHandle > {
    // Allow b2ParticleSystem to use SetIndex() to associate particle handles
    // with particle indices.
    friend class b2ParticleSystem;

  public:
    /// Initialize the index associated with the handle to an invalid index.
    b2ParticleHandle():
        m_index( b2_invalidParticleIndex ) {}

    /// Empty destructor.
    ~b2ParticleHandle() {}

    /// Get the index of the particle associated with this handle.
    int32 GetIndex() const { return m_index; }

  private:
    /// Set the index of the particle associated with this handle.
    void SetIndex( int32 index ) { m_index = index; }

  private:
    // Index of the particle within the particle system.
    int32 m_index;
};

#if LIQUIDFUN_EXTERNAL_LANGUAGE_API
inline void b2ParticleDef::SetPosition( float x, float y ) {
  position.Set( x, y );
}

inline void b2ParticleDef::SetColor( int32 r, int32 g, int32 b, int32 a ) {
  color.Set( (uint8) r, (uint8) g, (uint8) b, (uint8) a );
}
#endif // LIQUIDFUN_EXTERNAL_LANGUAGE_API

#endif