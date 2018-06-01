#pragma once
#include "../BlackBone/Config.h"
#include "../BlackBone/Process/Process.h"
#include "../BlackBone/Process/MultPtr.hpp"
#include "../BlackBone/Process/RPC/RemoteFunction.hpp"
#include "../BlackBone/PE/PEImage.h"
#include "../BlackBone/Misc/Utils.h"
#include "../BlackBone/Misc/DynImport.h"
#include "../BlackBone/Syscalls/Syscall.h"
#include "../BlackBone/Patterns/PatternSearch.h"
#include "../BlackBone/Asm/LDasm.h"
#include "../BlackBone/localHook/VTableHook.hpp"

#include <iostream>
#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blackbone;


/// <summary>
/// Some extension for Assert
/// </summary>
class AssertEx
{
public:
    template <typename T>
    static void IsZero( const T& actual, const wchar_t* message = nullptr, const __LineInfo* pLineInfo = nullptr )
    {
        Assert::AreEqual( T( 0 ), actual, message, pLineInfo );
    }

    template <typename T>
    static void IsNotZero( const T& actual, const wchar_t* message = nullptr, const __LineInfo* pLineInfo = nullptr )
    {
        Assert::AreNotEqual( T( 0 ), actual, message, pLineInfo );
    }

    static void NtSuccess( NTSTATUS status, const wchar_t* message = nullptr, const __LineInfo* pLineInfo = nullptr )
    {
        Assert::IsTrue( status >= 0, message, pLineInfo );
    }
};