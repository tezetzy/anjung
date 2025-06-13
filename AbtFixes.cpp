#include <dlfcn.h>
#include <android/log.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

uintptr_t pGTASA;
void* hGTASA;

constexpr float fMagic = 50.0f / 30.0f;
float* _ms_fTimeStep;
float* ms_fTimeStep;
float* ms_fAspectRatio;
float* ms_fTimeScale;
float* m_f3rdPersonCHairMultX;
float* m_f3rdPersonCHairMultY;
float save;

constexpr float ar43 = 4.0f / 3.0f;
#define fAspectCorrection (*ms_fAspectRatio - ar43)

float fWideScreenWidthScale, fWideScreenHeightScale;

/* --- Struct --- */
struct CVector2D { float x, y; };

enum eWeaponType {
    WEAPON_UNARMED = 0,
    WEAPON_COUNTRYRIFLE = 33,
};

struct CWeapon { int m_eWeaponType; };

struct CVector { float x, y, z; };

struct CPed
{
    void* vtable;   // 0x00
    // ...
    CVector m_vecPosition; // 0x30
    // ...
};

struct CPlayerPed {
    char pad[0x5A0];
    CWeapon m_WeaponSlots[13];
    int m_nCurrentWeapon;
};

struct CPlayerInfo {
    CPlayerPed* pPed;
};

CPlayerInfo* WorldPlayers;
CPlayerPed* (*FindPlayerPed)(int playerId);  // optional

/* --- Memory Helper --- */
void Unprotect(uintptr_t addr, size_t len)
{
    uintptr_t page = addr & ~(getpagesize() - 1);
    mprotect((void*)page, len + (addr - page), PROT_READ | PROT_WRITE | PROT_EXEC);
}

void PatchBytes(uintptr_t addr, const void* patch, size_t len)
{
    Unprotect(addr, len);
    memcpy((void*)addr, patch, len);
}

void HookFunction(uintptr_t addr, void* func)
{
    Unprotect(addr, sizeof(void*));
    *(uintptr_t*)addr = (uintptr_t)func;
}

/* --- Hooks --- */

// Fungsi asli (akan di-set setelah hook)
void (*_DrawCrosshair)();
void DrawCrosshair_Hook()
{
    constexpr float X = 1024.0f / 1920.0f;
    constexpr float Y = 768.0f / 1920.0f;

    CPlayerPed* player = WorldPlayers[0].pPed;
    if (player && player->m_WeaponSlots[player->m_nCurrentWeapon].m_eWeaponType == WEAPON_COUNTRYRIFLE)
    {
        float save1 = *m_f3rdPersonCHairMultX;
        float save2 = *m_f3rdPersonCHairMultY;
        *m_f3rdPersonCHairMultX = 0.530f - 0.84f * ar43 * 0.01115f;
        *m_f3rdPersonCHairMultY = 0.400f + 0.84f * ar43 * 0.038f;
        _DrawCrosshair();
        *m_f3rdPersonCHairMultX = save1;
        *m_f3rdPersonCHairMultY = save2;
        return;
    }
    float save1 = *m_f3rdPersonCHairMultX;
    float save2 = *m_f3rdPersonCHairMultY;
    *m_f3rdPersonCHairMultX = 0.530f - fAspectCorrection * 0.01115f;
    *m_f3rdPersonCHairMultY = 0.400f + fAspectCorrection * 0.038f;
    _DrawCrosshair();
    *m_f3rdPersonCHairMultX = save1;
    *m_f3rdPersonCHairMultY = save2;
}

// Fungsi asli
/*void (*_ControlGunMove)(void* self, CVector2D* vec2D);
void ControlGunMove_Hook(void* self, CVector2D* vec2D)
{
    save = *ms_fTimeStep;
    *ms_fTimeStep = fMagic;
    _ControlGunMove(self, vec2D);
    *ms_fTimeStep = save;
}
*/
/* --- Entry Point --- */
__attribute__((constructor)) void lib_main()
{
    hGTASA = dlopen("libGTASA.so", RTLD_LAZY);
    pGTASA = (uintptr_t)hGTASA;

    float* a = (float*)dlsym(hGTASA, "_ZN7CCamera22m_f3rdPersonCHairMultXE");
    m_f3rdPersonCHairMultX = (float*)dlsym(hGTASA, "_ZN7CCamera22m_f3rdPersonCHairMultXE");
    m_f3rdPersonCHairMultY = (float*)dlsym(hGTASA, "_ZN7CCamera22m_f3rdPersonCHairMultYE");
    ms_fAspectRatio = (float*)dlsym(hGTASA, "_ZN5CDraw15ms_fAspectRatioE");  // CDraw::ms_fAspectRatio
    ms_fTimeScale = (float*)dlsym(hGTASA, "_ZN6CTimer13ms_fTimeScaleE");     // CTimer::ms_fTimeScale


    // Set pointer ke fungsi asli sebelum di-hook
    _ms_fTimeStep = (float*)(pGTASA + 0x869684);
    _DrawCrosshair = (void(*)())(pGTASA + 0x672880);
   /* _ControlGunMove = (void(*)(void*, CVector2D*))(pGTASA + 0x3A1220);
*/
    HookFunction((uintptr_t)_DrawCrosshair, (void*)DrawCrosshair_Hook);
   /* HookFunction((uintptr_t)_ControlGunMove, (void*)ControlGunMove_Hook);
*/
}
