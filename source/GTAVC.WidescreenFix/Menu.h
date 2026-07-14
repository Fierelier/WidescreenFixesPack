#pragma once

#include <stdafx.h>
#include "common.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // needed for timeBeginPeriod()/timeEndPeriod()

#include "Skeleton.h"

enum CutsceneBordersMode : int8_t
{
    Off = 0,
    Letterbox = 1,
    Pillarbox = 2,
    Both = 3
};

enum FrameLimiterMode : int8_t
{
    eOff, e30, e40, e50, e60, e75, e100, e120, e144, e165, e200, e240
};

struct tSkinInfo
{
    int32_t skinId;
    char skinNameDisplayed[256];
    char skinNameOriginal[256];
    char date[256];
    tSkinInfo* nextSkin;
};

enum eMenuSprites
{
    MENUSPRITE_BACKGROUND,
    MENUSPRITE_VCLOGO,
    MENUSPRITE_MOUSE,
    MENUSPRITE_MAPTOP01,
    MENUSPRITE_MAPTOP02,
    MENUSPRITE_MAPTOP03,
    MENUSPRITE_MAPMID01,
    MENUSPRITE_MAPMID02,
    MENUSPRITE_MAPMID03,
    MENUSPRITE_MAPBOT01,
    MENUSPRITE_MAPBOT02,
    MENUSPRITE_MAPBOT03,
    MENUSPRITE_WILDSTYLE,
    MENUSPRITE_FLASH,
    MENUSPRITE_KCHAT,
    MENUSPRITE_FEVER,
    MENUSPRITE_VROCK,
    MENUSPRITE_VCPR,
    MENUSPRITE_ESPANTOSO,
    MENUSPRITE_EMOTION,
    MENUSPRITE_WAVE,
    MENUSPRITE_MP3,
    MENUSPRITE_DOWNOFF,
    MENUSPRITE_DOWNON,
    MENUSPRITE_UPOFF,
    MENUSPRITE_UPON,

    NUM_MENU_SPRITES
};

class CSprite2d
{
public:
    RwTexture* m_pTexture;
};

class CMenuManager
{
public:
    int8_t m_StatsScrollDirection;
    float m_StatsScrollSpeed;
    uint8_t field_8;
    bool m_PrefsUseVibration;
    bool m_PrefsShowHud;
    int32_t m_PrefsRadarMode;
    bool m_DisplayControllerOnFoot;
    bool m_bShutDownFrontEndRequested;
    bool m_bStartUpFrontEndRequested;
    int32_t m_KeyPressedCode;
    int32_t m_PrefsBrightness;
    float m_PrefsLOD;
    int8_t m_PrefsShowSubtitles;
    int8_t m_PrefsShowLegends;
    int8_t m_PrefsUseWideScreen;
    int8_t m_PrefsVsync;
    int8_t m_PrefsVsyncDisp;
    int8_t m_PrefsFrameLimiter;
    int8_t m_nPrefsAudio3DProviderIndex;
    int8_t m_PrefsSpeakers;
    int8_t m_PrefsDMA;
    int8_t m_PrefsSfxVolume;
    int8_t m_PrefsMusicVolume;
    int8_t m_PrefsRadioStation;
    uint8_t m_PrefsStereoMono; // unused except restore settings
    int32_t m_nCurrOption;
    bool m_bQuitGameNoCD;
    bool m_bMenuMapActive;
    bool m_AllowNavigation;
    uint8_t field_37;
    bool m_bMenuActive;
    bool m_bWantToRestart;
    bool m_bFirstTime;
    bool m_bActivateSaveMenu;
    bool m_bWantToLoad;
    float m_fMapSize;
    float m_fMapCenterX;
    float m_fMapCenterY;
    uint32_t OS_Language;
    int32_t m_PrefsLanguage;
    int32_t field_54;
    int8_t m_bLanguageLoaded;
    uint8_t m_PrefsAllowNastyGame;
    int8_t m_PrefsMP3BoostVolume;
    int8_t m_ControlMethod;
    int32_t m_nPrefsVideoMode;
    int32_t m_nDisplayVideoMode;
    int32_t m_nMouseTempPosX;
    int32_t m_nMouseTempPosY;
    bool m_bGameNotLoaded;
    int8_t m_lastWorking3DAudioProvider;
    bool m_bFrontEnd_ReloadObrTxtGxt;
    int32_t* pEditString;
    uint8_t field_74[4];
    int32_t* pControlEdit;
    bool m_OnlySaveMenu;
    int32_t m_firstStartCounter;
    CSprite2d m_aFrontEndSprites[NUM_MENU_SPRITES];
    bool m_bSpritesLoaded;
    int32_t m_LeftMostRadioX;
    int32_t m_ScrollRadioBy;
    int32_t m_nCurrScreen;
    int32_t m_nPrevScreen;
    int32_t m_nCurrSaveSlot;
    uint32_t m_LastScreenSwitch;
    int32_t m_nMenuFadeAlpha;
    int32_t m_nOptionHighlightTransitionBlend;
    bool bMenuChangeOngoing;
    int32_t MouseButtonJustClicked;
    int32_t JoyButtonJustClicked;
    bool DisplayComboButtonErrMsg;
    bool m_NoEmptyBinding;
    bool m_ShowEmptyBindingError;
    int32_t m_nHelperTextAlpha;
    bool m_bPressedPgUpOnList;
    bool m_bPressedPgDnOnList;
    bool m_bPressedUpOnList;
    bool m_bPressedDownOnList;
    bool m_bPressedScrollButton;
    uint8_t field_129;
    uint8_t field_12A;
    uint8_t field_12B;
    int32_t m_nMousePosX;
    int32_t m_nMousePosY;
    int32_t m_nMouseOldPosX;
    int32_t m_nMouseOldPosY;
    int32_t m_nHoverOption;
    bool m_bShowMouse;
    int32_t m_nOptionMouseHovering;
    bool m_bStartWaitingForKeyBind;
    bool m_bWaitingForNewKeyBind;
    bool m_bKeyChangeNotProcessed;
    int32_t m_CurrCntrlAction;
    uint8_t field_150;
    uint8_t field_151;
    uint8_t field_152;
    uint8_t field_153;
    int32_t m_nSelectedContSetupColumn;
    bool m_bKeyIsOK;
    bool field_159;
    uint8_t m_nCurrExLayer;
    char m_PrefsSkinFile[256];
    char m_aSkinName[256];
    uint8_t field_35B;
    int32_t m_nHelperTextMsgId;
    tSkinInfo m_pSkinListHead;
    tSkinInfo* m_pSelectedSkin;
    int32_t m_nFirstVisibleRowOnList;
    float m_nScrollbarTopMargin;
    int32_t m_nTotalListRow;
    int32_t m_nSkinsTotal;
    uint8_t field_67C[4];
    int32_t m_nSelectedListRow;
    bool m_bSkinsEnumerated;
    enum LANGUAGE
    {
        LANGUAGE_AMERICAN,
        LANGUAGE_FRENCH,
        LANGUAGE_GERMAN,
        LANGUAGE_ITALIAN,
        LANGUAGE_SPANISH,
    };
    bool GetIsMenuActive() { return !!m_bMenuActive; }
};

inline GameRef<CMenuManager> FrontendMenuManager([]() -> CMenuManager*
{
    auto pattern = hook::pattern("B9 ? ? ? ? 6A 01 68 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E8");
    if (!pattern.empty())
        return *pattern.get_first<CMenuManager*>(1);
    return nullptr;
});
