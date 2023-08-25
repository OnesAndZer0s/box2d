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

#ifndef B2_DRAW_H
#define B2_DRAW_H

#include "box2d/api.h"
#include "box2d/common/math.h"

/// Color for debug drawing. Each value has the range [0,1].
struct B2_API b2Color {
    b2Color() {}

    b2Color( float rIn, float gIn, float bIn, float aIn = 1.0f ):
        r( rIn ),
        g( gIn ),
        b( bIn ),
        a( aIn ) {}

    void Set( float rIn, float gIn, float bIn, float aIn = 1.0f ) {
      r = rIn;
      g = gIn;
      b = bIn;
      a = aIn;
    }

    /// Assign a b2Color to this instance.
    b2Color& operator=( const b2Color& color ) {
      Set( color.r, color.g, color.b, color.a );
      return *this;
    }

    /// True when all four color elements equal 0. When true, a particle color
    /// buffer isn't allocated by CreateParticle().
    ///
    bool IsZero() const {
      return !r && !g && !b && !a;
    }

    /// Multiplies r, g, b, a members by s where s is a value between 0.0
    /// and 1.0.
    b2Color& operator*=( float s ) {
      Set( ( r * s ), ( g * s ), ( b * s ), ( a * s ) );
      return *this;
    }

    /// Scales r, g, b, a members by s where s is a value between 0 and 255.
    b2Color& operator*=( uint8 s ) {
      // 1..256 to maintain the complete dynamic range.
      const int32 scale = (int32) s + 1;
      Set( (float) ( (int32) r * scale ),
          (float) ( (int32) g * scale ),
          (float) ( (int32) b * scale ),
          (float) ( (int32) a * scale ) );
      return *this;
    }

    /// Scales r, g, b, a members by s returning the modified b2Color.
    b2Color operator*( float s ) const {
      return MultiplyByScalar( s );
    }

    /// Scales r, g, b, a members by s returning the modified b2Color.
    b2Color operator*( uint8 s ) const {
      return MultiplyByScalar( s );
    }

    /// Add two colors.  This is a non-saturating addition so values
    /// overflows will wrap.
    b2Inline b2Color& operator+=( const b2Color& color ) {
      r += color.r;
      g += color.g;
      b += color.b;
      a += color.a;
      return *this;
    }

    /// Add two colors.  This is a non-saturating addition so values
    /// overflows will wrap.
    b2Color operator+( const b2Color& color ) const {
      b2Color newColor( *this );
      newColor += color;
      return newColor;
    }

    /// Subtract a color from this color.  This is a subtraction without
    /// saturation so underflows will wrap.
    b2Inline b2Color& operator-=( const b2Color& color ) {
      r -= color.r;
      g -= color.g;
      b -= color.b;
      a -= color.a;
      return *this;
    }

    /// Subtract a color from this color returning the result.  This is a
    /// subtraction without saturation so underflows will wrap.
    b2Color operator-( const b2Color& color ) const {
      b2Color newColor( *this );
      newColor -= color;
      return newColor;
    }

    /// Compare this color with the specified color.
    bool operator==( const b2Color& color ) const {
      return r == color.r && g == color.g && b == color.b && a == color.a;
    }

    /// Mix mixColor with this color using strength to control how much of
    /// mixColor is mixed with this color and vice versa.  The range of
    /// strength is 0..128 where 0 results in no color mixing and 128 results
    /// in an equal mix of both colors.  strength 0..128 is analogous to an
    /// alpha channel value between 0.0f..0.5f.
    b2Inline void Mix( b2Color* const mixColor, const int32 strength ) {
      MixColors( this, mixColor, strength );
    }

    /// Mix colorA with colorB using strength to control how much of
    /// colorA is mixed with colorB and vice versa.  The range of
    /// strength is 0..128 where 0 results in no color mixing and 128 results
    /// in an equal mix of both colors.  strength 0..128 is analogous to an
    /// alpha channel value between 0.0f..0.5f.
    static b2Inline void MixColors( b2Color* const colorA,
        b2Color* const colorB,
        const int32 strength ) {
      const uint8 dr = (uint8) ( strength * ( colorB->r - colorA->r ) );
      const uint8 dg = (uint8) ( strength * ( colorB->g - colorA->g ) );
      const uint8 db = (uint8) ( strength * ( colorB->b - colorA->b ) );
      const uint8 da = (uint8) ( strength * ( colorB->a - colorA->a ) );
      colorA->r += dr;
      colorA->g += dg;
      colorA->b += db;
      colorA->a += da;
      colorB->r -= dr;
      colorB->g -= dg;
      colorB->b -= db;
      colorB->a -= da;
    }

  private:
    /// Generalization of the multiply operator using a scalar in-place
    /// multiplication.
    template< typename T >
    b2Color MultiplyByScalar( T s ) const {
      b2Color color( *this );
      color *= s;
      return color;
    }

  public:
    float r, g, b, a;

  protected:
    /// Maximum value of a b2Color component.
    static const float k_maxValue;
    /// 1.0 / k_maxValue.
    static const float k_inverseMaxValue;
    /// Number of bits used to store each b2Color component.
    static const float k_bitsPerComponent;
};

extern b2Color b2Color_zero;

/// Implement and register this class with a b2World to provide debug drawing of physics
/// entities in your game.
class B2_API b2Draw {
  public:
    b2Draw();

    virtual ~b2Draw() {}

    enum {
      e_shapeBit = 0x0001,        ///< draw shapes
      e_jointBit = 0x0002,        ///< draw joint connections
      e_aabbBit = 0x0004,         ///< draw axis aligned bounding boxes
      e_pairBit = 0x0008,         ///< draw broad-phase pairs
      e_centerOfMassBit = 0x0010, ///< draw center of mass frame
      e_particleBit = 0x0020      ///< draw particles

    };

    /// Set the drawing flags.
    void SetFlags( uint32 flags );

    /// Get the drawing flags.
    uint32 GetFlags() const;

    /// Append flags to the current flags.
    void AppendFlags( uint32 flags );

    /// Clear flags from the current flags.
    void ClearFlags( uint32 flags );

    /// Draw a closed polygon provided in CCW order.
    virtual void DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) = 0;

    /// Draw a solid closed polygon provided in CCW order.
    virtual void DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) = 0;

    /// Draw a circle.
    virtual void DrawCircle( const b2Vec2& center, float radius, const b2Color& color ) = 0;

    /// Draw a solid circle.
    virtual void DrawSolidCircle( const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color ) = 0;

    /// Draw a particle array
    virtual void DrawParticles( const b2Vec2* centers, float radius, const b2Color* colors, int32 count ) = 0;

    /// Draw a line segment.
    virtual void DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color ) = 0;

    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    virtual void DrawTransform( const b2Transform& xf ) = 0;

    /// Draw a point.
    virtual void DrawPoint( const b2Vec2& p, float size, const b2Color& color ) = 0;

  protected:
    uint32 m_drawFlags;
};

#endif
