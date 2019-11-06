 #ifndef RENDERTEMP_H
#define RENDERTEMP_H

#include <stdint.h>
#include <memory>

#include "IShader.h"
#include "PODArray.h"
#include "DgMap_AVL.h"

namespace Engine
{
  typedef void(*RenderCommandFn)(void*);

  class RenderSortCriterion
  {

  };

  class RenderAPI
  {
  public:

    //void Execute(RenderCommand, void *, uint32_t size);

  };

  class RenderState
  {
  public:

    enum class Type
    {
      DrawCall,
      Command
    };

    enum class CommandType
    {
      None,
      CreateBuffer,
      DeleteBuffer
    };

    enum class Layer
    {
      None,
      Game,
      Effect,
      HUD
    };

    enum class Translucency
    {
      None,
      Additive,
      Subtractive
    };

  public:

    typedef uint64_t myInt;

  public:

    RenderState()
      : m_data(0)
    {
      SetLayer(Layer::None);
    }

    void SetType(Type);

    //Draw calls...
    void SetLayer(Layer);
    void SetTranslucency(Translucency);
    void SetDepth(float a_min, float a_max, float a_val);
    void SetMaterial(unsigned);
    void SetVAO(unsigned);

    //Commands...
    void SetCommandType(CommandType);
    void SetCommandID(int);
    //void SetSequence(unsigned);

    myInt GetLayer() const;
    myInt GetTranslucency() const;
    myInt GetDepth() const;
    myInt GetMaterial() const;
    myInt GetVAO() const;

    Type  GetType() const;
    //myInt GetSequence(unsigned) const;
    uint32_t   GetCommandID() const;

    myInt m_data;
  };

  class RenderCommandQueue
  {
  public:
  
    void* Allocate(RenderState, uint32_t size);
    void Execute();
  
    void PushCriterion(RenderSortCriterion *);
    void ClearCriterion();

  private:

    struct KV
    {
      uint64_t renderState;
      uint32_t dataOffset;
      uint32_t dataSize;
    };

    struct Boundary
    {
      uint32_t offset;
      uint32_t count;
    };

    PODArray<RenderSortCriterion*>  m_sortCriterion;
    PODArray<Boundary>              m_boundaries;
    PODArray<KV>                    m_renderQueue;

    //command function (64)| arguments size (32)| arguments...
    PODArray<uint8_t>               m_data;

  };

  class Renderer
  {
  public:

    typedef int32_t ID;

    static Renderer * Instance();

    void BeginScene();
    void* Submit(RenderState, uint32_t size);
    void EndScene();
    void RenderAndWait();

    void WaitAndRender();

    uint32_t RegisterCommand(RenderCommandFn);
    uint32_t RegisterShader(std::shared_ptr<IShader>);
    //uint32_t RegisterVAO(std::shared_ptr<IVAO>);
    //uint32_t RegisterTexture(std::shared_ptr<ITexture>);
    //uint32_t RegisterMaterial(std::shared_ptr<IMaterial>);

  public:

    RenderCommandQueue m_commandQueue;

    Dg::Map_AVL<RenderCommandFn, uint32_t>  m_registeredRenderCommands;
    PODArray<RenderCommandFn>               m_renderCommandMap;

    //Should these belong in an asset manager?
    //std::unique_ptr<ShaderLibrary> m_shaderLibrary;
    //std::unique_ptr<VAOLibrary> m_vaoLibrary;
    //std::unique_ptr<TextureLibrary> m_textureLibrary;

  };

  //We have allowed for a RenderCommand to have up to 8 arguments


#ifdef _MSC_VER // Microsoft compilers

#define EXPAND(x) x
#define __NARGS(_1, _2, _3, _4, _5, _6, _7, _8, _9, \
                _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
                _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
                _30, _31, _32, _33,  VAL, ...) VAL

#define NARGS_1(...) EXPAND(__NARGS(__VA_ARGS__,\
                32, 31, 30, \
                29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
                19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
                9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define AUGMENTER(...) unused, __VA_ARGS__
#define NARGS(...) NARGS_1(AUGMENTER(__VA_ARGS__))

#else // TODO Not tested

#define NARGS(...) __NARGS(0, ## __VA_ARGS__, 32, 31, 30, \
                29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
                19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
                9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __NARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, \
                _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
                _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
                _30, _31, _32, N,...) N

#endif

#define ADD_ITEM_0(tag) ERROR__ADD_ITEM_0__INVALID_NUMBER_OF_ARGUMENTS
#define ADD_ITEM_1(tag, code) ERROR__ADD_ITEM_1__INVALID_NUMBER_OF_ARGUMENTS
#define ADD_ITEM_2(tag, state, code)
#define ADD_ITEM_3(tag, a0, state, code) ADD_LAST_ ## tag(a0)
#define ADD_ITEM_4(tag, a0, a1, state, code) ADD_NEXT_ ## tag(a0) ADD_ITEM_3(tag, a1, state, code)
#define ADD_ITEM_5(tag, a0, a1, a2, state, code) ADD_NEXT_ ## tag(a0) ADD_ITEM_4(tag, a1, a2, state, code)
#define ADD_ITEM_6(tag, a0, a1, a2, a3, state, code) ADD_NEXT_ ## tag(a0) ADD_ITEM_5(tag, a1, a2, a3, state, code)
#define ADD_ITEM_7(tag, a0, a1, a2, a3, a4, state, code) ADD_NEXT_ ## tag(a0) ADD_ITEM_6(tag, a1, a2, a3, a4, state, code)
#define ADD_ITEM_8(tag, a0, a1, a2, a3, a4, a5, state, code) ADD_NEXT_ ## tag(a0) ADD_ITEM_7(tag, a1, a2, a3, a4, a5, state, code)
#define ADD_ITEM_9(tag, a0, a1, a2, a3, a4, a5, a6, state, code) ADD_NEXT_ ## tag(a0) ADD_ITEM_8(tag, a1, a2, a3, a4, a5, a6, state, code)
#define ADD_ITEM_10(tag, a0, a1, a2, a3, a4, a5, a6, a7, state, code) ADD_NEXT_ ## tag(a0) ADD_ITEM_9(tag, a1, a2, a3, a4, a5, a6, a7, state, code)

#define ADD_CONS_ASSGN_0(tag) ERROR__ADD_CONS_ASSGN_0__INVALID_NUMBER_OF_ARGUMENTS
#define ADD_CONS_ASSGN_1(tag, code) ERROR__ADD_CONS_ASSGN_1__INVALID_NUMBER_OF_ARGUMENTS
#define ADD_CONS_ASSGN_2(tag, state, code)
#define ADD_CONS_ASSGN_3(tag, a0, state, code) : ADD_ITEM_3(tag, a0, state, code)
#define ADD_CONS_ASSGN_4(tag, a0, a1, state, code) : ADD_ITEM_4(tag, a0, a1, state, code)
#define ADD_CONS_ASSGN_5(tag, a0, a1, a2, state, code) : ADD_ITEM_5(tag, a0, a1, a2, state, code)
#define ADD_CONS_ASSGN_6(tag, a0, a1, a2, a3, state, code) : ADD_ITEM_6(tag, a0, a1, a2, a3, state, code)
#define ADD_CONS_ASSGN_7(tag, a0, a1, a2, a3, a4, state, code) : ADD_ITEM_7(tag, a0, a1, a2, a3, a4, state, code)
#define ADD_CONS_ASSGN_8(tag, a0, a1, a2, a3, a4, a5, state, code) : ADD_ITEM_8(tag, a0, a1, a2, a3, a4, a5, state, code)
#define ADD_CONS_ASSGN_9(tag, a0, a1, a2, a3, a4, a5, a6, state, code) : ADD_ITEM_9(tag, a0, a1, a2, a3, a4, a5, a6, state, code)
#define ADD_CONS_ASSGN_10(tag, a0, a1, a2, a3, a4, a5, a6, a7, state, code) : ADD_ITEM_10(tag, a0, a1, a2, a3, a4, a5, a6, a7, state, code)

#define GET_CODE_1(code) ERROR__GET_CODE_1__INVALID_NUMBER_OF_ARGUMENTS
#define GET_CODE_2(state, code) code
#define GET_CODE_3(a0, state, code) code
#define GET_CODE_4(a0, a1, state, code) code
#define GET_CODE_5(a0, a1, a2, state, code) code
#define GET_CODE_6(a0, a1, a2, a3, state, code) code
#define GET_CODE_7(a0, a1, a2, a3, a4, state, code) code
#define GET_CODE_8(a0, a1, a2, a3, a4, a5, state, code) code
#define GET_CODE_9(a0, a1, a2, a3, a4, a5, a6, state, code) code
#define GET_CODE_10(a0, a1, a2, a3, a4, a5, a6, a7, state, code) code

#define GET_STATE_1(code) ERROR__GET_STATE_1__INVALID_NUMBER_OF_ARGUMENTS
#define GET_STATE_2(state, code) state
#define GET_STATE_3(a0, state, code) state
#define GET_STATE_4(a0, a1, state, code) state
#define GET_STATE_5(a0, a1, a2, state, code) state
#define GET_STATE_6(a0, a1, a2, a3, state, code) state
#define GET_STATE_7(a0, a1, a2, a3, a4, state, code) state
#define GET_STATE_8(a0, a1, a2, a3, a4, a5, state, code) state
#define GET_STATE_9(a0, a1, a2, a3, a4, a5, a6, state, code) state
#define GET_STATE_10(a0, a1, a2, a3, a4, a5, a6, a7, state, code) state

#define GET_ARGS_1(arg) ERROR__GET_ARGS_1__INVALID_NUMBER_OF_ARGUMENTS
#define GET_ARGS_2(state, code)
#define GET_ARGS_3(a0, state, code) a0
#define GET_ARGS_4(a0, a1, state, code) a0, a1
#define GET_ARGS_5(a0, a1, a2, state, code) a0, a1, a2
#define GET_ARGS_6(a0, a1, a2, a3, state, code) a0, a1, a2, a3
#define GET_ARGS_7(a0, a1, a2, a3, a4, state, code) a0, a1, a2, a3, a4
#define GET_ARGS_8(a0, a1, a2, a3, a4, a5, state, code) a0, a1, a2, a3, a4, a5
#define GET_ARGS_9(a0, a1, a2, a3, a4, a5, a6, state, code) a0, a1, a2, a3, a4, a5, a6
#define GET_ARGS_10(a0, a1, a2, a3, a4, a5, a6, a7, state, code) a0, a1, a2, a3, a4, a5, a6, a7

#define HELPER1(name, count) name ## _ ## count
#define HELPER(name, count) HELPER1(name, count)

#define RENDER_PASTE2(a, b) a ## b
#define RENDER_PASTE(a, b) RENDER_PASTE2(a, b)
#define RENDER_UNIQUE(x) RENDER_PASTE(x, __LINE__)

#define ADD_NEXT_CONSTRUCTOR_ARG(arg) typename ::std::remove_const<typename ::std::remove_reference<decltype(arg)>::type>::type a_ ## arg,
#define ADD_LAST_CONSTRUCTOR_ARG(arg) typename ::std::remove_const<typename ::std::remove_reference<decltype(arg)>::type>::type a_ ## arg
#define ADD_CONSTRUCTOR_ARGS(...) RENDER_PASTE(HELPER(ADD_ITEM, NARGS(__VA_ARGS__)),(CONSTRUCTOR_ARG, __VA_ARGS__))

#define ADD_NEXT_CONSTRUCTOR_ASSIGNMENT(arg) arg(a_ ## arg),
#define ADD_LAST_CONSTRUCTOR_ASSIGNMENT(arg) arg(a_ ## arg)
#define ADD_CONSTRUCTOR_ASSIGNMENT(...) RENDER_PASTE(HELPER(ADD_CONS_ASSGN, NARGS(__VA_ARGS__)),(CONSTRUCTOR_ASSIGNMENT, __VA_ARGS__))

#define ADD_NEXT_EXTRACT_MEMBER(arg) auto & arg = ((RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg;
#define ADD_LAST_EXTRACT_MEMBER(arg) ADD_NEXT_EXTRACT_MEMBER(arg)
#define EXTRACT_MEMBERS(...) RENDER_PASTE(HELPER(ADD_ITEM, NARGS(__VA_ARGS__)),(EXTRACT_MEMBER, __VA_ARGS__))

#define ADD_NEXT_DEFINE_MEMBER(arg) typename ::std::remove_const<typename ::std::remove_reference<decltype(arg)>::type>::type arg;
#define ADD_LAST_DEFINE_MEMBER(arg) ADD_NEXT_DEFINE_MEMBER(arg)
#define DEFINE_MEMBERS(...) RENDER_PASTE(HELPER(ADD_ITEM, NARGS(__VA_ARGS__)),(DEFINE_MEMBER, __VA_ARGS__))

#define GET_CODE(...) RENDER_PASTE(HELPER(GET_CODE, NARGS(__VA_ARGS__)),(__VA_ARGS__))
#define GET_STATE(...) RENDER_PASTE(HELPER(GET_STATE, NARGS(__VA_ARGS__)),(__VA_ARGS__))
#define GET_ARGS(...) RENDER_PASTE(HELPER(GET_ARGS, NARGS(__VA_ARGS__)),(__VA_ARGS__))

  //Must have the format:
  // RENDER_SUBMIT(args..., RenderSate, code)
#define RENDER_SUBMIT(...) \
    struct RENDER_UNIQUE(RenderCommand) \
    {\
      RENDER_UNIQUE(RenderCommand)(\
        ADD_CONSTRUCTOR_ARGS(__VA_ARGS__)\
        )\
        ADD_CONSTRUCTOR_ASSIGNMENT(__VA_ARGS__)\
      {}\
        static void Execute(void* argBuffer)\
        {\
          EXTRACT_MEMBERS(__VA_ARGS__)\
          GET_CODE(__VA_ARGS__)\
        }\
        DEFINE_MEMBERS(__VA_ARGS__)\
    };\
    {\
		  uint32_t id = ::Engine::Renderer::Instance()->RegisterCommand(RENDER_UNIQUE(RenderCommand)::Execute);\
      RenderState newState = GET_STATE(__VA_ARGS__);\
      newState.SetCommandID(id);\
		  auto mem = ::Engine::Renderer::Instance()->Submit(newState, sizeof(RENDER_UNIQUE(RenderCommand)));\
		  new (mem) RENDER_UNIQUE(RenderCommand)(GET_ARGS(__VA_ARGS__));\
	  }
}

#endif