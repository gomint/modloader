//
// A lot of this code builds upon the foundation provided by Don Clugston in his incredibly detailed
// article "Member Function Pointers and the Fastest Possible C++ Delegates". Massive thanks to you,
// Mr Clugston!
//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_HELPERS_H
#define MODLOADER_HELPERS_H

#include <cstdint>

namespace GoMint::SymExtract {

    /*
     * Invocation Targets.
     *
     * These are helper classes which possess the respective inheritance attributes
     * on all supported compilers and can therefore serve as decent invocation targets
     * when invoking a member function pointer on them.
     */
    class SingleInvocable {
        int unused;
    };

    class MultiInvocableBaseA {
        int unused;
    };

    class MultiInvocableBaseB {
        int unused;
    };

    class MultiInvocable : public MultiInvocableBaseA, public MultiInvocableBaseB {
        // unused
    };

    template<typename Input, typename Output>
    inline Output horribleCast(const Input input) {
        union {
            Output m_out;
            Input m_in;
        } horribleUnion;

        static_assert(sizeof(Input) == sizeof(horribleUnion) &&
                      sizeof(Input) == sizeof(Output),
                      "As horrible as casts can get: this will never work out in your favor.");

        horribleUnion.m_in = input;
        return horribleUnion.m_out;
    }

    // The upcoming code is an extreme hack, please do not punch me for writing this
    //
    // During symbol extraction we extract the resolved addresses of functions, even virtual ones.
    // This means that we can always construct member function pointers as if they were non-virtual.
    //
    // The following code uses the mechanisms underlying the MSVC and GNU compilers respectively
    // and some hidden knowledge about how these compilers represent member function pointers to
    // construct member function pointers at will.

#if defined(_MSC_VER)
    struct MicrosoftSingleMFP {
        std::uintptr_t m_address;
    };

    struct MicrosoftMultiMFP {
        std::uintptr_t m_address;
        std::intptr_t m_thisdelta;
    };

    template<typename MemberFunctionPtr>
    inline MemberFunctionPtr createMemberFunctionPointer(std::uintptr_t address) {
        static_assert(
                sizeof(MicrosoftSingleMFP) == sizeof(MemberFunctionPtr),
                "single-inheritance member function pointer size mismatch"
        );

        union {
            MicrosoftSingleMFP m_table;
            MemberFunctionPtr m_mfp;
        } u;

        u.m_table.m_address = address;
        return u.m_mfp;
    }

    template<typename MemberFunctionPtr>
    inline MemberFunctionPtr createMemberFunctionPointer(std::uintptr_t address, std::intptr_t thisdelta) {
        static_assert(
                sizeof(MicrosoftMultiMFP) == sizeof(MemberFunctionPtr),
                "multi-inheritance member function pointer size mismatch"
        );

        union {
            MicrosoftMultiMFP m_table;
            MemberFunctionPtr m_mfp;
        } u;


        u.m_table.m_address = address;
        u.m_table.m_thisdelta = thisdelta;
        return u.m_mfp;
    }

    template<typename MemberFunctionPtr>
    inline std::uintptr_t retrieveFunctionAddress(MemberFunctionPtr ptr) {
        union {
            std::uintptr_t m_address;
            MemberFunctionPtr m_ptr;
        } u;

        u.m_ptr = ptr;
        return u.m_address;
    };
#elif defined(__GNUC__)
    struct GnuMFP {
        std::uintptr_t m_address;       // As long as the address is even, this will be treated a a direct address and not a vtable index
        std::intptr_t  m_thisdelta;
    };

    template<typename MemberFunctionPtr>
    inline MemberFunctionPtr createMemberFunctionPointer(std::uintptr_t address) {
        static_assert(
                sizeof(GnuMFP) == sizeof(MemberFunctionPtr),
                "single-inheritance member function pointer size mismatch"
        );

        union {
            GnuMFP m_table;
            MemberFunctionPtr m_mfp;
        } u;

        u.m_table.m_address = address;
        u.m_table.m_thisdelta = static_cast<std::intptr_t>(0);
        return u.m_mfp;
    }

    template<typename MemberFunctionPtr>
    inline MemberFunctionPtr createMemberFunctionPointer(std::uintptr_t address, std::intptr_t thisdelta) {
        static_assert(
                sizeof(GnuMFP) == sizeof(MemberFunctionPtr),
                "multi-inheritance member function pointer size mismatch"
        );

        union {
            GnuMFP m_table;
            MemberFunctionPtr m_mfp;
        } u;


        u.m_table.m_address = address;
        u.m_table.m_thisdelta = thisdelta;
        return u.m_mfp;
    }

    template<typename MemberFunctionPtr>
    inline std::uintptr_t retrieveFunctionAddress(MemberFunctionPtr ptr) {
        union {
            std::uintptr_t m_address;
            MemberFunctionPtr m_ptr;
        } u;

        u.m_ptr = ptr;
        return u.m_address;
    };
#endif

}

#endif //MODLOADER_HELPERS_H
