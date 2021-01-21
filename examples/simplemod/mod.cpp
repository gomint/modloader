//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/ModLoader.h>

#include <cstring>

extern "C" {

    constexpr std::uint8_t SIMPLEMOD_UUID[16] = { 0xa5, 0x84, 0x44, 0x4e, 0x8f, 0x8f, 0x4f, 0xf8, 0xa0, 0xd5, 0xa0, 0x38, 0xde, 0x9f, 0xa2, 0x54 };
    constexpr GoMint::SemanticVersion SIMPLEMOD_VERSION = { 0, 1, 0 };

    GoMint::ILogger* g_logger;


    /**
     * Invoked by the ModLoader when first reading this DLL file.
     *
     * @param info An information structure for the mod to fill out
     */
    MOD_API void MOD_CALL get_mod_information(GoMint::ModInformation* info) {
        info->m_structureVersion = GoMint::ModInformation::STRUCTURE_VERSION;
        std::memcpy(info->m_uuid, SIMPLEMOD_UUID, 16);
        info->m_version = SIMPLEMOD_VERSION;
        info->m_modLoaderVersion = { 0, 1, 0 };
        std::memcpy(info->m_longName, "SimpleMod", 10);
        std::memcpy(info->m_shortName, "SimpleMod", 10);
        std::memcpy(info->m_author, "Ciel DeVille", 13);
        std::memcpy(info->m_description, "-/-", 4);
    }

    MOD_API GoMint::ModLoadResult MOD_CALL on_mod_load(GoMint::IModLoader* loader, GoMint::ILogger* logger) {
        g_logger = logger;
        g_logger->info("SimpleMod loaded: What a day to be alive!");
        return GoMint::ModLoadResult::Success;
    }

    MOD_API void MOD_CALL on_mod_unload() {
        g_logger->info("SimpleMod unloaded");
        g_logger = nullptr;
    }

}