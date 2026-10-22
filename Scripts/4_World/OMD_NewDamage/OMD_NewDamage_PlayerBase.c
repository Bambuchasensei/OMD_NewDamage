modded class PlayerBase
{
	protected static const int OMD_ND_STORE_VERSION = 8;
	protected static const int OMD_ND_STORE_VERSION_V1 = 1;
	protected static const int OMD_ND_STORE_VERSION_V2 = 2;
	protected static const int OMD_ND_STORE_VERSION_V3 = 3;
	protected static const int OMD_ND_STORE_VERSION_V4 = 4;
	protected static const int OMD_ND_STORE_VERSION_V5 = 5;
	protected static const int OMD_ND_STORE_VERSION_V6 = 6;
	protected static const int OMD_ND_STORE_VERSION_V7 = 7;
	protected static const int OMD_ND_SIDE_NONE = 0;
	protected static const int OMD_ND_SIDE_LEFT = 1;
	protected static const int OMD_ND_SIDE_RIGHT = 2;
	protected static const int OMD_ND_ORGAN_LEVEL_MAX = 3;
	protected static const int OMD_ND_ORGAN_DMG_SYNC_A_MAX = 268435455;
	protected static const int OMD_ND_ORGAN_DMG_SYNC_B_MAX = 65535;
	protected static const int OMD_ND_ARTERY_SYNC_MAX = 1048575;
	protected static const float OMD_ND_ORGAN_DAMAGE_MAX = 100;

	protected static const int OMD_ND_PRESS_ART_ARMS_L = 1;
	protected static const int OMD_ND_PRESS_ART_ARMS_R = 2;
	protected static const int OMD_ND_PRESS_ART_LEGS_L = 4;
	protected static const int OMD_ND_PRESS_ART_LEGS_R = 8;
	protected static const int OMD_ND_PRESS_ART_CAROTID = 16;
	protected static const int OMD_ND_PRESS_ART_FEMORAL = 32;
	protected static const int OMD_ND_PRESS_ART_SUBCLAVIAN_L = 64;
	protected static const int OMD_ND_PRESS_ART_SUBCLAVIAN_R = 128;

	protected static const int OMD_ND_PRESS_DEEP_HEAD = 1;
	protected static const int OMD_ND_PRESS_DEEP_TORSO = 2;
	protected static const int OMD_ND_PRESS_DEEP_ARMS = 4;
	protected static const int OMD_ND_PRESS_DEEP_HANDS = 8;
	protected static const int OMD_ND_PRESS_DEEP_LEGS = 16;
	protected static const int OMD_ND_PRESS_DEEP_FEET = 32;

	protected static const int OMD_ND_PRESS_MODE_NONE = 0;
	protected static const int OMD_ND_PRESS_MODE_SELF = 1;
	protected static const int OMD_ND_PRESS_MODE_TARGET = 2;

	protected static const int OMD_ND_PRESS_WOUND_NONE = 0;
	protected static const int OMD_ND_PRESS_WOUND_CAROTID = 1;
	protected static const int OMD_ND_PRESS_WOUND_SUBCLAVIAN_L = 2;
	protected static const int OMD_ND_PRESS_WOUND_SUBCLAVIAN_R = 3;
	protected static const int OMD_ND_PRESS_WOUND_FEMORAL = 4;
	protected static const int OMD_ND_PRESS_WOUND_ARMS_L = 5;
	protected static const int OMD_ND_PRESS_WOUND_ARMS_R = 6;
	protected static const int OMD_ND_PRESS_WOUND_LEGS_L = 7;
	protected static const int OMD_ND_PRESS_WOUND_LEGS_R = 8;
	protected static const int OMD_ND_PRESS_WOUND_DEEP_HEAD = 9;
	protected static const int OMD_ND_PRESS_WOUND_DEEP_TORSO = 10;
	protected static const int OMD_ND_PRESS_WOUND_DEEP_ARMS = 11;
	protected static const int OMD_ND_PRESS_WOUND_DEEP_HANDS = 12;
	protected static const int OMD_ND_PRESS_WOUND_DEEP_LEGS = 13;
	protected static const int OMD_ND_PRESS_WOUND_DEEP_FEET = 14;

	protected bool m_OMD_HitOverrideActive;
	protected string m_OMD_HitOverrideComp;
	protected vector m_OMD_HitOverrideModelPos;

	protected int m_OMD_ND_ArtArmsLLevel;
	protected float m_OMD_ND_ArtArmsLTimer;
	protected int m_OMD_ND_ArtArmsRLevel;
	protected float m_OMD_ND_ArtArmsRTimer;
	protected int m_OMD_ND_ArtCarotidLevel;
	protected int m_OMD_ND_ArtCarotidSide;
	protected float m_OMD_ND_ArtCarotidTimer;
	protected int m_OMD_ND_ArtFemoralLevel;
	protected int m_OMD_ND_ArtFemoralSide;
	protected float m_OMD_ND_ArtFemoralTimer;
	protected int m_OMD_ND_ArtSubclavianLLevel;
	protected float m_OMD_ND_ArtSubclavianLTimer;
	protected int m_OMD_ND_ArtSubclavianRLevel;
	protected float m_OMD_ND_ArtSubclavianRTimer;
	protected int m_OMD_ND_ArtLegsLLevel;
	protected float m_OMD_ND_ArtLegsLTimer;
	protected int m_OMD_ND_ArtLegsRLevel;
	protected float m_OMD_ND_ArtLegsRTimer;
	protected int m_OMD_ND_FaceLevel;
	protected float m_OMD_ND_FaceDamage;
	protected float m_OMD_ND_BrainDamage;
	protected int m_OMD_ND_BrainLevel;
	protected int m_OMD_ND_BrainLevelSync;
	protected int m_OMD_ND_GutsLevel;
	protected float m_OMD_ND_GutsTimer;
	protected float m_OMD_ND_GutsDamage;
	protected int m_OMD_ND_HeartLevel;
	protected float m_OMD_ND_HeartTimer;
	protected float m_OMD_ND_HeartDamage;
	protected int m_OMD_ND_LiverLevel;
	protected float m_OMD_ND_LiverTimer;
	protected float m_OMD_ND_LiverDamage;
	protected int m_OMD_ND_LungLLevel;
	protected float m_OMD_ND_LungLTimer;
	protected float m_OMD_ND_LungLDamage;
	protected int m_OMD_ND_LungRLevel;
	protected float m_OMD_ND_LungRTimer;
	protected float m_OMD_ND_LungRDamage;
	protected int m_OMD_ND_SpleenLevel;
	protected float m_OMD_ND_SpleenTimer;
	protected float m_OMD_ND_SpleenDamage;
	protected bool m_OMD_ND_FreshFallActive;
	protected float m_OMD_ND_FreshFallHeight;
	protected float m_OMD_ND_FallPendingHp;
	protected bool m_OMD_ND_FallKillPending;
	protected bool m_OMD_ND_FallPendingActive;
	protected bool m_OMD_ND_JumpOutPending;
	protected Transport m_OMD_ND_JumpOutTransport;
	protected int m_OMD_ND_TorsoBullets;
	protected float m_OMD_ND_VitalDebugAccum;
	protected float m_OMD_ND_VitalTickAccum;
	protected int m_OMD_ND_OrganDamageSyncA;
	protected int m_OMD_ND_OrganDamageSyncB;
	protected int m_OMD_ND_ArteryLevelsSync;
	protected float m_OMD_ND_DebuffTickAccum;
	protected float m_OMD_ND_ClientFxAccum;
	protected float m_OMD_ND_HeartSfxAccum;
	protected float m_OMD_ND_LungSfxAccum;
	protected float m_OMD_ND_LungCoughAccum;
	protected float m_OMD_ND_LungCoughNext;
	protected float m_OMD_ND_CachedAimShake;
	protected bool m_OMD_ND_CachedSprintAllowed;
	protected bool m_OMD_ND_CachedRunAllowed;
	protected bool m_OMD_ND_RunSpeedLimited;
	protected float m_OMD_ND_CachedStaminaDrainMult;
	protected float m_OMD_ND_CachedStaminaRegenMult;
	protected float m_OMD_ND_CachedVignette;
	protected float m_OMD_ND_CachedEdgeBlur;
	protected float m_OMD_ND_CachedFaceBlur;
	protected float m_OMD_ND_CachedDarken;
	protected float m_OMD_ND_CachedDropChance;
	protected EffectSound m_OMD_ND_HeartLoopSound;
	protected EffectSound m_OMD_ND_LungLoopSound;

	protected bool m_OMD_ND_PressActive;
	protected string m_OMD_ND_PressWoundKey;
	protected string m_OMD_ND_PressHolderSteamId;
	protected float m_OMD_ND_PressReduce;
	protected float m_OMD_ND_PressTickAccum;
	protected bool m_OMD_ND_PressExternalHitPending;
	protected string m_OMD_ND_PressInterruptDamageSource;
	protected float m_OMD_ND_PressInterruptHitDamage;
	protected int m_OMD_ND_PressableArteryBits;
	protected int m_OMD_ND_PressableDeepBits;
	protected int m_OMD_ND_PressWoundIndex;
	protected bool m_OMD_ND_PressOccupied;
	protected bool m_OMD_ND_HolderPressModeActive;
	protected int m_OMD_ND_HolderPressModeKind;
	protected string m_OMD_ND_PressExitInputId;
	protected bool m_OMD_ND_PressExitRequestPending;
	protected bool m_OMD_ND_PressExitClientInModeWas;
	protected PlayerBase m_OMD_ND_PressHolderPlayer;
	protected PlayerBase m_OMD_ND_PressPatientPlayer;
	protected OMD_NewDamage_PressModeCB m_OMD_ND_PressModeCallback;
	protected bool m_OMD_ND_PressModeAnimDesired;
	protected int m_OMD_ND_PressModeAnimRetryCount;
	protected bool m_OMD_ND_PressModeAnimEverStarted;
	protected EntityAI m_OMD_ND_PressHandsItemTracked;
	protected ref array<string> m_OMD_ND_PressInterruptInputs;
	protected ref OMD_NewDamage_PressWoundVariantData m_OMD_ND_PressVariantData;

	protected bool m_OMD_Symptom_SprintWasAllowed = true;
	protected bool m_OMD_Symptom_RunWasAllowed = true;
	protected float m_OMD_Symptom_StaminaDrain = 1.0;
	protected float m_OMD_Symptom_StaminaRegen = 1.0;
	protected float m_OMD_Symptom_AimShake = 1.0;
	protected float m_OMD_Symptom_ShockPerSec;
	protected float m_OMD_Symptom_AdsShockPerSec;
	protected float m_OMD_Symptom_SprintShockPerSec;
	protected float m_OMD_Symptom_UnconPerSec;
	protected bool m_OMD_Symptom_DeathLogged;
	protected string m_OMD_Symptom_DeathCause;
	protected ref map<string, float> m_OMD_Symptom_RateCache;
	protected ref map<string, float> m_OMD_Symptom_HealLogAt;


	protected void OMD_NewDamage_ResetSymptomLogCache()
	{
		m_OMD_Symptom_SprintWasAllowed = true;
		m_OMD_Symptom_RunWasAllowed = true;
		m_OMD_Symptom_StaminaDrain = 1.0;
		m_OMD_Symptom_StaminaRegen = 1.0;
		m_OMD_Symptom_AimShake = 1.0;
		m_OMD_Symptom_ShockPerSec = 0;
		m_OMD_Symptom_AdsShockPerSec = 0;
		m_OMD_Symptom_SprintShockPerSec = 0;
		m_OMD_Symptom_UnconPerSec = 0;
		m_OMD_Symptom_DeathLogged = false;
		m_OMD_Symptom_DeathCause = "";
		m_OMD_Symptom_RateCache = null;
		m_OMD_Symptom_HealLogAt = null;
	}


	protected void OMD_NewDamage_LogCachedBool(string zone, int level, string effect, bool active, bool wasActive, float value, string cause)
	{
		if (active == wasActive)
		{
			return;
		}

		string state;
		state = "off";
		if (active)
		{
			state = "on";
		}
		OMD_LogSymptom("OMD", zone, level, effect, state, value, cause);
	}


	protected void OMD_NewDamage_LogCachedFloat(string zone, int level, string effect, float value, float previous, float neutral, string cause)
	{
		if (value == previous)
		{
			return;
		}

		string state;
		state = "off";
		if (value != neutral)
		{
			state = "on";
		}
		OMD_LogSymptom("OMD", zone, level, effect, state, value, cause);
	}


	protected void OMD_NewDamage_LogDeath(string zone, int level, string cause)
	{
		if (m_OMD_Symptom_DeathLogged)
		{
			return;
		}

		m_OMD_Symptom_DeathLogged = true;
		m_OMD_Symptom_DeathCause = "";
		OMD_LogSymptom("OMD", zone, level, "Death", "on", 0, cause);
	}

};
