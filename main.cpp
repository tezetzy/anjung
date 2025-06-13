#include <jni/mod/amlmod.h>
#include <jni/mod/logger.h>
#include <jni/mod/config.h>


MYMOD(net.jayzxy.gtasa.abtfixes, GTA:SA ABT Fixes, 1.0, Jayzxy)

/* Saves */
static uintptr_t pGTASA = 0;
void* hGTASA;
/* */
constexpr float fMagic = 50.0f / 30.0f;

float* ms_fTimeStep;
float* ms_fAspectRatio;
float* ms_fTimeScale;
float* m_f3rdPersonCHairMultX;
float* m_f3rdPersonCHairMultY;

constexpr float ar43 = 4.0f / 3.0f;
#define fAspectCorrection (*ms_fAspectRatio - ar43)
#define fAspectCorrectionDiv (*ms_fAspectRatio / ar43)
#define GetTimeStep() (*ms_fTimeStep)
#define GetTimeStepInSeconds() (GetTimeStep() / 50.0f)
#define GetTimeStepMagic() (GetTimeStep() / fMagic)
#define GetTimeStepInvMagic() (fMagic / GetTimeStep())
float save; // Untuk simpan timestep

/* hoam */
/* lele */
// Struct weapon & player
struct CWeaponSlot {
    int m_eWeaponType;
};
enum eWeaponType
{
    WEAPON_UNARMED = 0,
    WEAPON_COUNTRYRIFLE = 33,
    // Tambahkan jika perlu
};

struct CWeapon
{
    int m_eWeaponType;
    // Tambahan field jika perlu
};

struct CPlayerPed
{
    char pad[0x5A0];                         // Perkirakan offset ke weapon
    CWeapon m_WeaponSlots[13];               // 13 slot senjata
    int m_nCurrentWeapon;
};

struct CPlayerInfo
{
    CPlayerPed* pPed;
};
CPlayerInfo* WorldPlayers;
CPlayerPed* (*FindPlayerPed)(int playerId);

float fWideScreenWidthScale, fWideScreenHeightScale;
DECL_HOOKv(DrawCrosshair)
{
    constexpr float X = 1024.0f / 1920.0f; // prev. 0.530, now it's 0.533333..3
    constexpr float Y = 768.0f / 1920.0f; // unchanged :p

    CPlayerPed* player = WorldPlayers[0].pPed;
    if(player->m_WeaponSlots[player->m_nCurrentWeapon].m_eWeaponType == WEAPON_COUNTRYRIFLE)
    {
        // Weirdo logic but ok
        float save1 = *m_f3rdPersonCHairMultX; *m_f3rdPersonCHairMultX = 0.530f - 0.84f * ar43 * 0.01115f; // 0.01125f;
        float save2 = *m_f3rdPersonCHairMultY; *m_f3rdPersonCHairMultY = 0.400f + 0.84f * ar43 * 0.038f; // 0.03600f;
        DrawCrosshair();
        *m_f3rdPersonCHairMultX = save1; *m_f3rdPersonCHairMultY = save2;
        return;
    }
    float save1 = *m_f3rdPersonCHairMultX; *m_f3rdPersonCHairMultX = 0.530f - fAspectCorrection * 0.01115f; // 0.01125f;
    float save2 = *m_f3rdPersonCHairMultY; *m_f3rdPersonCHairMultY = 0.400f + fAspectCorrection * 0.038f; // 0.03600f;
    DrawCrosshair();
    *m_f3rdPersonCHairMultX = save1; *m_f3rdPersonCHairMultY = save2;
}
//
struct CVector2D { float x, y; };
DECL_HOOKv(ControlGunMove, void* self, CVector2D* vec2D)
{
    save = *ms_fTimeStep;
    *ms_fTimeStep = fMagic;
    ControlGunMove(self, vec2D);
    *ms_fTimeStep = save;
}
extern "C" void OnGameLoaded()
{
    pGTASA = aml->GetLib("libGTASA.so");
    hGTASA = dlopen("libGTASA.so", RTLD_LAZY);

    SET_TO(ms_fTimeStep,            aml->GetSym(hGTASA, "_ZN6CTimer12ms_fTimeStepE"));
    SET_TO(m_f3rdPersonCHairMultX,  aml->GetSym(hGTASA, "_ZN7CCamera22m_f3rdPersonCHairMultXE"));
    SET_TO(m_f3rdPersonCHairMultY,  aml->GetSym(hGTASA, "_ZN7CCamera22m_f3rdPersonCHairMultYE"));
    SET_TO(ms_fAspectRatio,         aml->GetSym(hGTASA, "_ZN5CDraw15ms_fAspectRatioE"));
    SET_TO(ms_fTimeScale,           aml->GetSym(hGTASA, "_ZN6CTimer13ms_fTimeScaleE"));
    //SET_TO(WorldPlayers,            aml->GetSym(pGTASA + BYBIT(0x6783C8, 0x84E7A8)));
    WorldPlayers = (CPlayerInfo*)(pGTASA + 0x84E7A8);
    
    HOOKPLT(DrawCrosshair, pGTASA + 0x672880);
    HOOKPLT(ControlGunMove, pGTASA + 0x66F9D0);
}
