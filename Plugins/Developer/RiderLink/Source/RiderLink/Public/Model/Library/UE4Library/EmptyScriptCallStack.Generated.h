//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a RdGen v1.10.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#ifndef EMPTYSCRIPTCALLSTACK_GENERATED_H
#define EMPTYSCRIPTCALLSTACK_GENERATED_H


#include "protocol/Protocol.h"
#include "types/DateTime.h"
#include "impl/RdSignal.h"
#include "impl/RdProperty.h"
#include "impl/RdList.h"
#include "impl/RdSet.h"
#include "impl/RdMap.h"
#include "base/ISerializersOwner.h"
#include "base/IUnknownInstance.h"
#include "serialization/ISerializable.h"
#include "serialization/Polymorphic.h"
#include "serialization/NullableSerializer.h"
#include "serialization/ArraySerializer.h"
#include "serialization/InternedSerializer.h"
#include "serialization/SerializationCtx.h"
#include "serialization/Serializers.h"
#include "ext/RdExtBase.h"
#include "task/RdCall.h"
#include "task/RdEndpoint.h"
#include "task/RdSymmetricCall.h"
#include "std/to_string.h"
#include "std/hash.h"
#include "std/allocator.h"
#include "util/enum.h"
#include "util/gen_util.h"

#include <cstring>
#include <cstdint>
#include <vector>
#include <ctime>

#include "thirdparty.hpp"
#include "instantiations_UE4Library.h"

#include "IScriptCallStack.Generated.h"

#include "UE4TypesMarshallers.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Runtime/Core/Public/Containers/ContainerAllocationPolicies.h"


#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable:4250 )
#pragma warning( disable:4307 )
#pragma warning( disable:4267 )
#pragma warning( disable:4244 )
#pragma warning( disable:4100 )
#endif

/// <summary>
/// <p>Generated from: UE4Library.kt:158</p>
/// </summary>
namespace JetBrains {
namespace EditorPlugin {

class RIDERLINK_API EmptyScriptCallStack : public IScriptCallStack {

private:
    // custom serializers

public:
    // constants
    static constexpr rd::wstring_view message{L"Script call stack: [Empty] (FFrame::GetStackTrace() called from native code)", 76};

protected:
    // fields

private:
    // initializer
    void initialize();

public:
    
    // default ctors and dtors
    
    EmptyScriptCallStack();
    
    EmptyScriptCallStack(EmptyScriptCallStack const &) = default;
    
    EmptyScriptCallStack& operator=(EmptyScriptCallStack const &) = default;
    
    EmptyScriptCallStack(EmptyScriptCallStack &&) = default;
    
    EmptyScriptCallStack& operator=(EmptyScriptCallStack &&) = default;
    
    virtual ~EmptyScriptCallStack() = default;
    
    // reader
    static EmptyScriptCallStack read(rd::SerializationCtx& ctx, rd::Buffer & buffer);
    
    // writer
    void write(rd::SerializationCtx& ctx, rd::Buffer& buffer) const override;
    
    // virtual init
    
    // identify
    
    // getters
    
    // intern

private:
    // equals trait
    bool equals(rd::ISerializable const& object) const override;

public:
    // equality operators
    friend bool operator==(const EmptyScriptCallStack &lhs, const EmptyScriptCallStack &rhs);
    friend bool operator!=(const EmptyScriptCallStack &lhs, const EmptyScriptCallStack &rhs);
    // hash code trait
    size_t hashCode() const noexcept override;
    // type name trait
    std::string type_name() const override;
    // static type name trait
    static std::string static_type_name();

private:
    // polymorphic to string
    std::string toString() const override;

public:
    // external to string
    friend std::string to_string(const EmptyScriptCallStack & value);
};

}
}

// hash code trait
namespace rd {

template <>
struct hash<JetBrains::EditorPlugin::EmptyScriptCallStack> {
    size_t operator()(const JetBrains::EditorPlugin::EmptyScriptCallStack & value) const noexcept {
        return value.hashCode();
    }
};

}

#ifdef _MSC_VER
#pragma warning( pop )
#endif



#endif // EMPTYSCRIPTCALLSTACK_GENERATED_H
