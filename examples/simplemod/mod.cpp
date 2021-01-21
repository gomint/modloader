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
     * Invoked by the ModLoader when first loading the mod into memory.
     *
     * @return The version of the ModInformation structure this mod will fill out
     */
    MOD_API std::uint32_t MOD_CALL get_mod_information_version() {
        return GoMint::ModInformation::STRUCTURE_VERSION;
    }

    /**
     * Invoked by the ModLoader when first reading this DLL file.
     *
     * @param info An information structure for the mod to fill out
     */
    MOD_API void MOD_CALL get_mod_information(GoMint::ModInformation* info) {
        std::memcpy(info->m_uuid, SIMPLEMOD_UUID, 16);
        info->m_version = SIMPLEMOD_VERSION;
        info->m_modLoaderVersion = MODLOADER_VERSION;
        strcpy_s(info->m_longName, "Simple Modding Example");
        strcpy_s(info->m_shortName, "SimpleMod");
        strcpy_s(info->m_author, "Ciel DeVille");
        strcpy_s(info->m_description, "A simple modification that shows how the mod loader works.");
    }

    MOD_API GoMint::ModActivationResult MOD_CALL on_mod_activate(GoMint::IModLoader* loader, GoMint::ILogger* logger) {
        g_logger = logger;
        g_logger->info("SimpleMod activated: What a day to be alive!");
        return GoMint::ModActivationResult::Success;
    }

    MOD_API void MOD_CALL on_mod_deactivate() {
        g_logger->info("SimpleMod deactivated");
        g_logger = nullptr;
    }

}