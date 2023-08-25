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
#include "particle.h"

#include "box2d/common/draw.h"

#define B2PARTICLECOLOR_BITS_PER_COMPONENT ( sizeof( uint8 ) << 3 )
// Maximum value of a b2Color component.
#define B2PARTICLECOLOR_MAX_VALUE \
  ( ( 1U << B2PARTICLECOLOR_BITS_PER_COMPONENT ) - 1 )

/// Number of bits used to store each b2Color component.
const float b2Color::k_bitsPerComponent =
    B2PARTICLECOLOR_BITS_PER_COMPONENT;
const float b2Color::k_maxValue = (float) B2PARTICLECOLOR_MAX_VALUE;
const float b2Color::k_inverseMaxValue =
    1.0f / (float) B2PARTICLECOLOR_MAX_VALUE;

int32 b2CalculateParticleIterations(
    float gravity, float radius, float timeStep ) {
  // In some situations you may want more particle iterations than this,
  // but to avoid excessive cycle cost, don't recommend more than this.
  const int32 B2_MAX_RECOMMENDED_PARTICLE_ITERATIONS = 8;
  const float B2_RADIUS_THRESHOLD = 0.01f;
  int32 iterations =
      (int32) ceilf( b2Sqrt( gravity / ( B2_RADIUS_THRESHOLD * radius ) ) * timeStep );
  return b2Clamp( iterations, 1, B2_MAX_RECOMMENDED_PARTICLE_ITERATIONS );
}