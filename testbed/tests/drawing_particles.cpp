#include "test.h"

class DrawingParticles : public Test {
  private:
    // Set bit 31 to distiguish these values from particle flags.
    enum Parameters {
      e_parameterBegin = ( 1UL << 31 ), // Start of this parameter namespace.
      e_parameterMove = e_parameterBegin | ( 1UL << 0 ),
      e_parameterRigid = e_parameterBegin | ( 1UL << 1 ),
      e_parameterRigidBarrier = e_parameterBegin | ( 1UL << 2 ),
      e_parameterElasticBarrier = e_parameterBegin | ( 1UL << 3 ),
      e_parameterSpringBarrier = e_parameterBegin | ( 1UL << 4 ),
      e_parameterRepulsive = e_parameterBegin | ( 1UL << 5 ),
    };

  public:

    DrawingParticles() {
      {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody( &bd );

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -4, -2 ),
              b2Vec2( 4, -2 ),
              b2Vec2( 4, 0 ),
              b2Vec2( -4, 0 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -4, -2 ),
              b2Vec2( -2, -2 ),
              b2Vec2( -2, 6 ),
              b2Vec2( -4, 6 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( 2, -2 ),
              b2Vec2( 4, -2 ),
              b2Vec2( 4, 6 ),
              b2Vec2( 2, 6 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }

        {
          b2PolygonShape shape;
          const b2Vec2 vertices [ 4 ] = {
              b2Vec2( -4, 4 ),
              b2Vec2( 4, 4 ),
              b2Vec2( 4, 6 ),
              b2Vec2( -4, 6 ) };
          shape.Set( vertices, 4 );
          ground->CreateFixture( &shape, 0.0f );
        }
      }

      m_colorIndex = 0;
      m_particleSystem->SetRadius( 0.05f );
      m_lastGroup = NULL;
      m_drawing = true;

      b2Assert( ( k_paramDef [ 0 ].CalculateValueMask() & e_parameterBegin ) == 0 );
      Test::SetParticleParameters( k_paramDef, k_paramDefCount );
      Test::SetRestartOnParticleParameterChange( false );

      m_particleFlags = Test::GetParticleParameterValue();
      m_groupFlags = 0;
    }

    // Determine the current particle parameter from the drawing state and
    // group flags.
    uint32 DetermineParticleParameter() const {
      if( m_drawing ) {
        if( m_groupFlags == ( b2_rigidParticleGroup |
                                b2_solidParticleGroup ) )
          return (uint32) e_parameterRigid;
        if( m_groupFlags == b2_rigidParticleGroup &&
            m_particleFlags == b2_barrierParticle )
          return (uint32) e_parameterRigidBarrier;
        if( m_particleFlags == ( b2_elasticParticle | b2_barrierParticle ) )
          return (uint32) e_parameterElasticBarrier;
        if( m_particleFlags == ( b2_springParticle | b2_barrierParticle ) )
          return (uint32) e_parameterSpringBarrier;
        if( m_particleFlags == ( b2_wallParticle | b2_repulsiveParticle ) )
          return (uint32) e_parameterRepulsive;
        return m_particleFlags;
      }
      return (uint32) e_parameterMove;
    }

    void Keyboard( int key ) override {
      m_drawing = key != GLFW_KEY_X;
      m_particleFlags = 0;
      m_groupFlags = 0;
      switch( key ) {
        case GLFW_KEY_E:
          m_particleFlags = b2_elasticParticle;
          m_groupFlags = b2_solidParticleGroup;
          break;
        case GLFW_KEY_P:
          m_particleFlags = b2_powderParticle;
          break;
        case GLFW_KEY_R:
          m_groupFlags = b2_rigidParticleGroup | b2_solidParticleGroup;
          break;
        case GLFW_KEY_S:
          m_particleFlags = b2_springParticle;
          m_groupFlags = b2_solidParticleGroup;
          break;
        case GLFW_KEY_T:
          m_particleFlags = b2_tensileParticle;
          break;
        case GLFW_KEY_V:
          m_particleFlags = b2_viscousParticle;
          break;
        case GLFW_KEY_W:
          m_particleFlags = b2_wallParticle;
          m_groupFlags = b2_solidParticleGroup;
          break;
        case GLFW_KEY_B:
          m_particleFlags = b2_barrierParticle | b2_wallParticle;
          break;
        case GLFW_KEY_H:
          m_particleFlags = b2_barrierParticle;
          m_groupFlags = b2_rigidParticleGroup;
          break;
        case GLFW_KEY_N:
          m_particleFlags = b2_barrierParticle | b2_elasticParticle;
          m_groupFlags = b2_solidParticleGroup;
          break;
        case GLFW_KEY_M:
          m_particleFlags = b2_barrierParticle | b2_springParticle;
          m_groupFlags = b2_solidParticleGroup;
          break;
        case GLFW_KEY_F:
          m_particleFlags = b2_wallParticle | b2_repulsiveParticle;
          break;
        case GLFW_KEY_C:
          m_particleFlags = b2_colorMixingParticle;
          break;
        case GLFW_KEY_Z:
          m_particleFlags = b2_zombieParticle;
          break;
        default:
          break;
      }
      Test::SetParticleParameterValue( DetermineParticleParameter() );
    }

    void MouseMove( const b2Vec2& p ) override {
      Test::MouseMove( p );
      if( m_drawing ) {
        b2CircleShape shape;
        shape.m_p = p;
        shape.m_radius = 0.2f;
        b2Transform xf;
        xf.SetIdentity();

        m_particleSystem->DestroyParticlesInShape( shape, xf );

        const bool joinGroup =
            m_lastGroup && m_groupFlags == m_lastGroup->GetGroupFlags();
        if( !joinGroup )
          m_colorIndex = ( m_colorIndex + 1 ) % k_ParticleColorsCount;
        b2ParticleGroupDef pd;
        pd.shape = &shape;
        pd.flags = m_particleFlags;
        if( ( m_particleFlags &
                ( b2_wallParticle | b2_springParticle | b2_elasticParticle ) ) ||
            ( m_particleFlags == ( b2_wallParticle | b2_barrierParticle ) ) )
          pd.flags |= b2_reactiveParticle;
        pd.groupFlags = m_groupFlags;
        pd.color = k_ParticleColors [ m_colorIndex ];
        pd.group = m_lastGroup;
        m_lastGroup = m_particleSystem->CreateParticleGroup( pd );
        m_mouseTracing = false;
      }
    }

    void MouseUp( const b2Vec2& p ) override {
      Test::MouseUp( p );
      m_lastGroup = NULL;
    }

    void ParticleGroupDestroyed( b2ParticleGroup* group ) override {
      Test::ParticleGroupDestroyed( group );
      if( group == m_lastGroup )
        m_lastGroup = NULL;
    }

    void SplitParticleGroups() {
      for( b2ParticleGroup* group = m_particleSystem->GetParticleGroupList(); group; group = group->GetNext() ) {
        if( group != m_lastGroup &&
            ( group->GetGroupFlags() & b2_rigidParticleGroup ) &&
            ( group->GetAllParticleFlags() & b2_zombieParticle ) ) {
          // Split a rigid particle group which may be disconnected
          // by destroying particles.
          m_particleSystem->SplitParticleGroup( group );
        }
      }
    }

    void Step( Settings& settings ) override {
      const uint32 parameterValue = Test::GetParticleParameterValue();
      m_drawing = ( parameterValue & e_parameterMove ) != e_parameterMove;
      if( m_drawing ) {
        switch( parameterValue ) {
          case b2_elasticParticle:
          case b2_springParticle:
          case b2_wallParticle:
            m_particleFlags = parameterValue;
            m_groupFlags = b2_solidParticleGroup;
            break;
          case e_parameterRigid:
            // b2_waterParticle is the default particle type in
            // LiquidFun.
            m_particleFlags = b2_waterParticle;
            m_groupFlags = b2_rigidParticleGroup |
                           b2_solidParticleGroup;
            break;
          case e_parameterRigidBarrier:
            m_particleFlags = b2_barrierParticle;
            m_groupFlags = b2_rigidParticleGroup;
            break;
          case e_parameterElasticBarrier:
            m_particleFlags = b2_barrierParticle | b2_elasticParticle;
            m_groupFlags = 0;
            break;
          case e_parameterSpringBarrier:
            m_particleFlags = b2_barrierParticle | b2_springParticle;
            m_groupFlags = 0;
            break;
          case e_parameterRepulsive:
            m_particleFlags = b2_repulsiveParticle | b2_wallParticle;
            m_groupFlags = b2_solidParticleGroup;
            break;
          default:
            m_particleFlags = parameterValue;
            m_groupFlags = 0;
            break;
        }
      }

      if( m_particleSystem->GetAllParticleFlags() & b2_zombieParticle )
        SplitParticleGroups();

      Test::Step( settings );
      g_debugDraw.DrawString(
          5, m_textLine, "Keys: (L) liquid, (E) elastic, (S) spring" );
      m_textLine += DRAW_STRING_NEW_LINE;
      g_debugDraw.DrawString(
          5, m_textLine, "(R) rigid, (W) wall, (V) viscous, (T) tensile" );
      m_textLine += DRAW_STRING_NEW_LINE;
      g_debugDraw.DrawString(
          5, m_textLine, "(F) repulsive wall, (B) wall barrier" );
      m_textLine += DRAW_STRING_NEW_LINE;
      g_debugDraw.DrawString(
          5, m_textLine, "(H) rigid barrier, (N) elastic barrier, (M) spring barrier" );
      m_textLine += DRAW_STRING_NEW_LINE;
      g_debugDraw.DrawString(
          5, m_textLine, "(C) color mixing, (Z) erase, (X) move" );
      m_textLine += DRAW_STRING_NEW_LINE;
    }

    static Test* Create() {
      return new DrawingParticles;
    }

    b2ParticleGroup* m_lastGroup;
    bool m_drawing;
    uint32 m_particleFlags;
    uint32 m_groupFlags;
    uint32 m_colorIndex;

    static const ParticleParameter::Value k_paramValues [];
    static const ParticleParameter::Definition k_paramDef [];
    static const uint32 k_paramDefCount;
};

const ParticleParameter::Value DrawingParticles::k_paramValues [] =
    {
        {b2_zombieParticle,
         ParticleParameter::k_DefaultOptions, "erase"          },
        { (uint32) e_parameterMove,
         ParticleParameter::k_DefaultOptions, "move"           },
        { (uint32) e_parameterRigid,
         ParticleParameter::k_DefaultOptions, "rigid"          },
        { (uint32) e_parameterRigidBarrier,
         ParticleParameter::k_DefaultOptions, "rigid barrier"  },
        { (uint32) e_parameterElasticBarrier,
         ParticleParameter::k_DefaultOptions, "elastic barrier"},
        { (uint32) e_parameterSpringBarrier,
         ParticleParameter::k_DefaultOptions, "spring barrier" },
        { (uint32) e_parameterRepulsive,
         ParticleParameter::k_DefaultOptions, "repulsive wall" }
};

const ParticleParameter::Definition DrawingParticles::k_paramDef [] =
    {
        {ParticleParameter::k_particleTypesPtr,
         ParticleParameter::k_particleTypesCount         },
        { DrawingParticles::k_paramValues,
         B2_ARRAY_SIZE( DrawingParticles::k_paramValues )},
};
const uint32 DrawingParticles::k_paramDefCount =
    B2_ARRAY_SIZE( DrawingParticles::k_paramDef );

static int testIndex = RegisterTest( "Particles", "Drawing Particles", DrawingParticles::Create );
