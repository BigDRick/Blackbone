#include "Common.h"

namespace Testing
{
    TEST_CLASS( RemoteCall )
    {
    public:
        TEST_METHOD_INITIALIZE( ClassInitialize )
        {
            AssertEx::NtSuccess( _explorer.Attach( L"explorer.exe" ) );
        }

        TEST_METHOD( NtQueryVirtualMemory )
        {
            auto barrier = _explorer.barrier().type;
            bool validArch = (barrier == wow_64_64 || barrier == wow_32_32);
            if (!validArch)
            {
                return;
            }

            auto hMainMod = _explorer.modules().GetMainModule();
            auto pRemote = _explorer.modules().GetNtdllExport( "NtQueryVirtualMemory" );

            Assert::IsNotNull( hMainMod.get() );
            Assert::IsTrue( pRemote.success() );

            uint8_t buf[1024] = { 0 };

            RemoteFunction<fnNtQueryVirtualMemory> pFN( _explorer, pRemote->procAddress );
            decltype(pFN)::CallArguments args(
                INVALID_HANDLE_VALUE,
                reinterpret_cast<PVOID>(hMainMod->baseAddress),
                MemorySectionName,
                reinterpret_cast<PVOID>(buf),
                static_cast<SIZE_T>(sizeof( buf )),
                reinterpret_cast<PSIZE_T>(0)
            );

            args.set( 3, AsmVariant( buf, sizeof( buf ) ) );
            auto result = pFN.Call( args );
            AssertEx::NtSuccess( result.status );
            AssertEx::NtSuccess( result.result() );

            std::wstring name( (wchar_t*)(buf + sizeof( UNICODE_STRING )) );
            Assert::AreNotEqual( name.npos, name.find( L"explorer.exe" ) );
        }

    private:
        Process _explorer;
    };

}