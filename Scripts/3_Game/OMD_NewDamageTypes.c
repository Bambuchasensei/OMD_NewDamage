class OMD_NewDamageJumpOutImpactMul
{
	string impact;
	float mul;
};

class OMD_NewDamageJumpOutNameMul
{
	string name;
	float mul;
};

class OMD_NewDamageConfigData
{
	bool activeEnabled = true;
	bool debugEnabled = false;
	ref array<string> debugAdminIds;
	// Chat debug command prefix. Default backslash — VPP/COT often eat messages starting with "/".
	string debugChatPrefix = "\\";
	bool globalHpLogEnabled = false;
	bool hitCalibrationLogEnabled = false;

	bool organResolveByCompEnabled = true;
	bool vitalSystemEnabled = true;

	float organMinBloodDamage = 0;
	float organMinHealthDamageFirearm = 1;
	float organMinHealthDamageMelee = 3;
	float organMinSpeedCoefFirearm = 0.2;
	float organSideDeadzone = 0.05;
	float organChanceMult = 1.0;

	bool meleeOrgansEnabled = true;
	float meleeOrganChanceMult = 0.25;
	float meleeArteryChanceMult = 0.5;
	bool meleeBrainFromMelee = false;
	bool brainEnabled = true;
	float brainDeathThreshold = 100;
	float brainFractureMul = 2.0;
	float brainNoFractureMul = 0.15;
	float meleeSharpMinBleedThreshold = 0;
	ref array<string> meleeForceBluntItems;
	bool organSourceEnabled_infected = true;
	bool organSourceEnabled_animal = true;
	bool organSourceEnabled_blunt = true;
	bool organSourceEnabled_fist = true;

	string organSideMode = "bulletDir";
	string organSideAxis = "x";

	int torsoBulletCountMax = 5;

	float hitComp_spine3_heart = 0.15;
	float hitComp_spine3_lung = 0.60;
	float hitComp_spine3_none = 0.25;

	float hitComp_spine2_heart = 0.10;
	float hitComp_spine2_lung = 0.45;
	float hitComp_spine2_liver = 0.20;
	float hitComp_spine2_none = 0.25;

	float hitComp_spine1_liver = 0.30;
	float hitComp_spine1_guts = 0.35;
	float hitComp_spine1_spleen = 0.10;
	float hitComp_spine1_none = 0.25;

	float hitComp_spine_liver = 0.25;
	float hitComp_spine_guts = 0.40;
	float hitComp_spine_spleen = 0.10;
	float hitComp_spine_none = 0.25;

	float hitComp_shoulder_lung = 0.25;
	float hitComp_shoulder_none = 0.75;

	float hitComp_torsoUnknown_lung = 0.28;
	float hitComp_torsoUnknown_liver = 0.18;
	float hitComp_torsoUnknown_guts = 0.18;
	float hitComp_torsoUnknown_spleen = 0.08;
	float hitComp_torsoUnknown_none = 0.28;

	float hitComp_pelvis_guts = 0.30;
	float hitComp_pelvis_femoral = 0.25;
	float hitComp_pelvis_spleen = 0.10;
	float hitComp_pelvis_none = 0.35;

	float hitComp_upleg_guts = 0.15;
	float hitComp_upleg_femoral = 0.40;
	float hitComp_upleg_none = 0.45;

	float hitComp_neck_carotid = 0.40;
	float hitComp_neck_heavy = 0.40;
	float hitComp_neck_light = 0.20;

	float hitComp_head_brain = 0.40;
	float hitComp_head_face = 0.50;
	float hitComp_head_none = 0.10;

	float hitComp_brain_brain = 1.0;

	float subclavianArteryChance = 0.15;
	float limbArteryChance = 0.15;

	// Drain model: stage = loss per second. 0 = no loss.
	// Calibrated for healthy player (100 HP / 5000 blood → fatal blood 2500).
	float drainHp_heart_L1 = 0.238;
	float drainHp_heart_L2 = 0.556;
	float drainHp_heart_L3 = 1.667;
	float drainBlood_heart_L1 = 0;
	float drainBlood_heart_L2 = 0;
	float drainBlood_heart_L3 = 0;

	float drainHp_lungL_L1 = 0.111;
	float drainHp_lungL_L2 = 0.208;
	float drainHp_lungL_L3 = 0.556;
	float drainBlood_lungL_L1 = 0;
	float drainBlood_lungL_L2 = 0;
	float drainBlood_lungL_L3 = 0;

	float drainHp_lungR_L1 = 0.111;
	float drainHp_lungR_L2 = 0.208;
	float drainHp_lungR_L3 = 0.556;
	float drainBlood_lungR_L1 = 0;
	float drainBlood_lungR_L2 = 0;
	float drainBlood_lungR_L3 = 0;

	float drainHp_liver_L1 = 0;
	float drainHp_liver_L2 = 0;
	float drainHp_liver_L3 = 0;
	float drainBlood_liver_L1 = 3.472;
	float drainBlood_liver_L2 = 5.952;
	float drainBlood_liver_L3 = 11.905;

	float drainHp_guts_L1 = 0;
	float drainHp_guts_L2 = 0;
	float drainHp_guts_L3 = 0;
	float drainBlood_guts_L1 = 1.042;
	float drainBlood_guts_L2 = 1.667;
	float drainBlood_guts_L3 = 3.472;

	float drainHp_spleen_L1 = 0.111;
	float drainHp_spleen_L2 = 0.208;
	float drainHp_spleen_L3 = 0.556;
	float drainBlood_spleen_L1 = 0;
	float drainBlood_spleen_L2 = 0;
	float drainBlood_spleen_L3 = 0;

	float drainHp_face_L1 = 0;
	float drainHp_face_L2 = 0;
	float drainHp_face_L3 = 0;
	float drainBlood_face_L1 = 3.472;
	float drainBlood_face_L2 = 6.944;
	float drainBlood_face_L3 = 13.889;

	float drainHp_carotid_L1 = 0;
	float drainHp_carotid_L2 = 0;
	float drainHp_carotid_L3 = 0;
	float drainBlood_carotid_L1 = 20.833;
	float drainBlood_carotid_L2 = 41.667;
	float drainBlood_carotid_L3 = 83.333;

	float drainHp_subclavianL_L1 = 0;
	float drainHp_subclavianL_L2 = 0;
	float drainHp_subclavianL_L3 = 0;
	float drainBlood_subclavianL_L1 = 20.833;
	float drainBlood_subclavianL_L2 = 41.667;
	float drainBlood_subclavianL_L3 = 83.333;

	float drainHp_subclavianR_L1 = 0;
	float drainHp_subclavianR_L2 = 0;
	float drainHp_subclavianR_L3 = 0;
	float drainBlood_subclavianR_L1 = 20.833;
	float drainBlood_subclavianR_L2 = 41.667;
	float drainBlood_subclavianR_L3 = 83.333;

	float drainHp_femoral_L1 = 0;
	float drainHp_femoral_L2 = 0;
	float drainHp_femoral_L3 = 0;
	float drainBlood_femoral_L1 = 16.667;
	float drainBlood_femoral_L2 = 33.333;
	float drainBlood_femoral_L3 = 62.5;

	float drainHp_armsL_L1 = 0;
	float drainHp_armsL_L2 = 0;
	float drainHp_armsL_L3 = 0;
	float drainBlood_armsL_L1 = 13.889;
	float drainBlood_armsL_L2 = 27.778;
	float drainBlood_armsL_L3 = 55.556;

	float drainHp_armsR_L1 = 0;
	float drainHp_armsR_L2 = 0;
	float drainHp_armsR_L3 = 0;
	float drainBlood_armsR_L1 = 13.889;
	float drainBlood_armsR_L2 = 27.778;
	float drainBlood_armsR_L3 = 55.556;

	float drainHp_legsL_L1 = 0;
	float drainHp_legsL_L2 = 0;
	float drainHp_legsL_L3 = 0;
	float drainBlood_legsL_L1 = 13.889;
	float drainBlood_legsL_L2 = 27.778;
	float drainBlood_legsL_L3 = 55.556;

	float drainHp_legsR_L1 = 0;
	float drainHp_legsR_L2 = 0;
	float drainHp_legsR_L3 = 0;
	float drainBlood_legsR_L1 = 13.889;
	float drainBlood_legsR_L2 = 27.778;
	float drainBlood_legsR_L3 = 55.556;

	float vitalBloodDrainMult = 1.0;
	float vitalShockDrainMult = 1.0;
	// Soft organ shock only — never used to force KO (see organShockFloorOffset).
	float organShockPerSec_L1 = 0.02;
	float organShockPerSec_L2 = 0.05;
	float organShockPerSec_L3 = 0.08;
	// Keep OMD organ/artery shock at/above UNCONSCIOUS_THRESHOLD + offset (vanilla KO = 25).
	float organShockFloorOffset = 5.0;
	bool organUnconChanceEnabled = false;

	// Unconscious drain multipliers. Values < 0 inherit unconsciousBleedMult (R1).
	bool unconsciousSlowdownEnabled = true;
	float unconsciousBleedMult = 0.30;
	float unconsciousOrganTimerMult = -1;
	float unconsciousOrganBloodDrainMult = -1;
	float unconsciousOrganShockDrainMult = -1;
	float unconsciousVanillaBleedMult = -1;
	float unconsciousInediaDeepWoundBleedMult = -1;
	float unconsciousHpDrainMult = -1;

	bool liverActivateInediaInternalBleed = true;

	// Local bone bias: left/low torso near Pelvis boosts spleen weight (0 = off).
	float spleenModelPosBiasEnabled = 1;
	float spleenModelPosBiasMult = 1.5;
	float spleenLocalXMax = -0.02;
	float spleenLocalYMaxFromSpine3 = -0.05;

	bool debuffsEnabled = true;
	float debuffTickSeconds = 1.0;
	string debuffStackMode = "maxPerAxis";

	ref OMD_NewDamageDebuffStageConfig heart_L1;
	ref OMD_NewDamageDebuffStageConfig heart_L2;
	ref OMD_NewDamageDebuffStageConfig heart_L3;
	ref OMD_NewDamageDebuffStageConfig lung_L1;
	ref OMD_NewDamageDebuffStageConfig lung_L2;
	ref OMD_NewDamageDebuffStageConfig lung_L3;
	ref OMD_NewDamageDebuffStageConfig face_L1;
	ref OMD_NewDamageDebuffStageConfig face_L2;
	ref OMD_NewDamageDebuffStageConfig face_L3;
	ref OMD_NewDamageDebuffStageConfig artery_L1;
	ref OMD_NewDamageDebuffStageConfig artery_L2;
	ref OMD_NewDamageDebuffStageConfig artery_L3;

	bool lungUseWorstSide = true;
	float heartHeartbeatSfx_L1 = 0.3;
	float heartHeartbeatSfx_L2 = 0.6;
	float heartHeartbeatSfx_L3 = 1.0;
	string sfxHeartHeartbeat_L1 = "OMD_Heart_Heartbeat_L1_SoundSet";
	string sfxHeartHeartbeat_L2 = "OMD_Heart_Heartbeat_L2_SoundSet";
	string sfxHeartHeartbeat_L3 = "OMD_Heart_Heartbeat_L3_SoundSet";
	float heartDarkenStrength_L1 = 0.15;
	float heartDarkenStrength_L2 = 0.35;
	float heartDarkenStrength_L3 = 0.70;
	string sfxLungCoughBlood = "OMD_Lung_CoughBlood_SoundSet";
	string sfxLungGurgle = "OMD_Lung_Gurgle_SoundSet";
	string sfxLungBreathHeavy = "OMD_Lung_BreathHeavy_SoundSet";

	float faceWoundBlurStrength_L1 = 0.40;
	float faceWoundBlurStrength_L2 = 0.70;
	float faceWoundBlurStrength_L3 = 0.90;

	// One-shot Inedia pain on organ stage raise: map stage→Inedia level, then raise limb % to threshold.
	// Inedia borders: ZERO≤10, LOW≤30, MEDIUM≤50, BADLY≤90, CRITICAL≤100 (levels 0..4).
	int inediaPainTargetLevel_OrganStage1 = 2;
	int inediaPainTargetLevel_OrganStage2 = 3;
	int inediaPainTargetLevel_OrganStage3 = 4;
	float inediaPainPercent_Level2 = 35;
	float inediaPainPercent_Level3 = 55;
	float inediaPainPercent_Level4 = 95;

	// Server lung cough via SymptomManager (vanilla SYMPTOM_COUGH). Intervals in seconds.
	float lungCoughIntervalMin_L1 = 40;
	float lungCoughIntervalMax_L1 = 60;
	float lungCoughIntervalMin_L2 = 20;
	float lungCoughIntervalMax_L2 = 40;
	float lungCoughIntervalMin_L3 = 10;
	float lungCoughIntervalMax_L3 = 20;
	float lungCoughBloodChance_L1 = 0.15;
	float lungCoughBloodChance_L2 = 0.35;
	float lungCoughBloodChance_L3 = 0.60;
	float lungCoughBloodLoss = 50;
	bool lungCoughBloodHandsEnabled = true;

	// Cumulative organ damage scale 0..100 (heart/lungL/lungR/liver/spleen/guts/face). Levels 1/2/3 from thresholds.
	float organLevelThreshold_L1 = 20;
	float organLevelThreshold_L2 = 50;
	float organLevelThreshold_L3 = 80;
	float organSelfHealMax = 50;
	float organSelfHealPerSec = 0.002;
	float organCriticalThreshold = 80;
	bool organCriticalHealable = false;
	float healMultBase = 1.0;
	float healMultSleeping = 2.0;
	float organDamageMult_heart = 2.0;
	float organDamageMult_lung = 1.5;
	float organDamageMult_liver = 1.5;
	float organDamageMult_spleen = 1.2;
	float organDamageMult_guts = 1.0;
	float organDamageMult_face = 1.0;

	// Fall Health is cut in EEOnDamageCalculated, before the hit. Floor applies only to the scaled base.
	// Extra HP starts above fallExtraHpStartHeight and can kill. Organ pool is per meter, capped only if max > 0.
	// TransportHit / crew crash still scale global Health after the engine hit.
	float fallGlobalHealthMult = 0.3;
	float transportGlobalHealthMult = 0.4;
	float crashGlobalHealthMult = 0.4;
	float globalHealthFloor_fall = 10;
	float globalHealthFloor_transport = 10;
	float globalHealthFloor_crash = 10;
	bool fallOrgansEnabled = true;
	bool transportOrgansEnabled = true;
	bool crashOrgansEnabled = true;
	float fallOrganMinHeight = 3;
	float fallOrganDmgPerMeter = 6.0;
	float fallOrganPoolMax = 0;
	float fallExtraHpStartHeight = 20;
	float fallExtraHpPerMeter = 5.0;
	float fallExtraHpMax = 0;
	float fallOrganHitChance = 1.0;
	float fallOrganRandMin = 0.7;
	float fallOrganRandMax = 1.3;
	float organShare_transport = 0.8;
	float organShare_crash = 1.2;
	float crashOrganPoolMax = 150;
	float transportOrganPoolMax = 150;
	float crashOrganRandMin = 0.7;
	float crashOrganRandMax = 1.3;
	float crashLimbDamageMult = 1.5;
	float fallFaceDmgPerMeter = 1.2;
	float fallFaceChance = 0.5;
	float fallArteryChance = 0.5;
	float fallArteryWeight_femoral = 1;
	float fallArteryWeight_legsL = 1;
	float fallArteryWeight_legsR = 1;
	float fallArteryWeight_armsL = 1;
	float fallArteryWeight_armsR = 1;
	float fallArteryWeight_subclavianL = 0.35;
	float fallArteryWeight_subclavianR = 0.35;
	float crashFaceHitDamage = 20.0;
	float crashFaceChance = 0.5;
	float crashArteryChance = 0.5;
	float crashArteryWeight_femoral = 1;
	float crashArteryWeight_legsL = 1;
	float crashArteryWeight_legsR = 1;
	float crashArteryWeight_armsL = 1;
	float crashArteryWeight_armsR = 1;
	float crashArteryWeight_carotid = 1;
	float crashArteryWeight_subclavianL = 1;
	float crashArteryWeight_subclavianR = 1;
	float fallChanceFullHeight = 20;
	float crashChanceFullSpeedKmh = 100;
	float organSpread_heart = 1;
	float organSpread_lung = 1.5;
	float organSpread_liver = 1.5;
	float organSpread_spleen = 1.5;
	float organSpread_guts = 1;

	bool jumpOutInjuriesEnabled = true;
	float jumpOutMinIncoming = 30.0;
	float jumpOutMaxIncoming = 100.0;
	float organShare_jumpout = 0.6;
	float jumpOutOrganPoolMax = 60.0;
	float jumpOutLegPainMax = 60.0;
	float jumpOutLegFractureChanceMin = 5.0;
	float jumpOutLegFractureChanceMax = 85.0;
	float jumpOutFaceHitChancePercent = 15.0;
	float jumpOutFaceHitDamage = 10.0;
	float jumpOutArteryHitChancePercent = 5.0;
	float jumpOutSurfaceDefaultMul = 0.8;
	ref array<ref OMD_NewDamageJumpOutImpactMul> jumpOutSurfaceImpactMuls;
	ref array<ref OMD_NewDamageJumpOutNameMul> jumpOutSurfaceNameMuls;

	bool hudLimbTintLayerEnabled = true;
	bool hudLimbTintRecolorBadges = false;
	ref array<string> hudLimbColorPriority;

	bool selfTreatAllowAll = false;
	float selfTreatTimeMult = 1.5;
	float selfTreatFailChanceAdd = 0.15;
	ref OMD_NewDamageSelfTreatAllowedConfig selfTreatAllowed;
	ref OMD_NewDamageTreatableConfig treatable;
	ref array<ref OMD_NewDamageTreatmentEntry> treatments;

	bool holdPressureEnabled = true;
	bool holdPressureSelfEnabled = true;
	bool holdPressureTargetEnabled = true;
	bool holdPressureRequiresEmptyHands = true;
	string holdPressureMode = "mode";
	float holdPressureTimedSeconds = 8;
	float holdPressureTickSeconds = 0.5;
	float holdPressureBaseReduce = -1;
	float holdPressureBleedReduceMin = 0.50;
	float holdPressureBleedReduceMax = 1.00;
	bool holdPressureUseMedSkill = false;
	string holdPressureMedSkillId = "med";
	int holdPressureMedLevelMax = 10;
	float holdPressureTimerReduceScale = 1.0;
	float holdPressureArteryLevelPenalty = 0.10;
	float holdPressureArteryLevelMultMin = 0.70;
	bool holdPressureAppliesToDeepWound = true;
	bool holdPressureAppliesToLimbArtery = true;
	bool holdPressureAppliesToFemoral = true;
	bool holdPressureAppliesToCarotid = true;
	bool holdPressureAppliesToSubclavian = true;
	bool holdPressureAppliesToVanillaBleed = false;
	bool holdPressureAppliesToOrgans = false;
	int holdPressureMaxWounds = 1;
	int holdPressureMaxHoldersPerWound = 1;
	string holdPressureSelectMode = "mostDangerous";
	bool holdPressureManualSelectEnabled = true;
	bool holdPressureBlockIfTourniquet = true;
	bool holdPressureBlockIfBandaged = true;
	bool holdPressureAllowInventoryAccess = true;
	bool holdPressureInterruptOnDamage = true;
	// Deprecated: exit uses any dmg>0 (hp/shock/blood). Kept for old JSON compat.
	float holdPressureInterruptHealthDamage = 1;
	// Deprecated: RMB/input exit is instant (EmoteManager LocalPress). Kept for old JSON compat.
	float holdPressureExitHoldSeconds = 0.3;
	float holdPressureMaxDistance = 2.0;
	string holdPressureAnimSelf = "CMD_ACTIONFB_SALINEBLOODBAG";
	string holdPressureAnimTarget = "CMD_ACTIONFB_SALINEBLOODBAGTARGET";
	string holdPressureStanceSelf = "erectOrCrouch";
	bool holdPressureSilent = true;
	bool holdPressureMedXpEnabled = false;
	float holdPressureMedXpIntervalSec = 30;
	float holdPressureMedXpAmount = 2;
};

// Flat stage block for heart/lung/face/artery. Unused fields stay at sentinel (-1 / true).
class OMD_NewDamageDebuffStageConfig
{
	float sprintSpeedMult = -1;
	bool sprintAllowed = true;
	bool runAllowed = true;
	float runSpeedMult = -1;
	float moveSpeedMult = -1;
	float staminaRegenMult = -1;
	float staminaCapMult = -1;
	float staminaDrainMult = -1;
	float aimShake = -1;
	float adsShockPerSec = -1;
	float shockPerSec = -1;
	float shockPerSecBonus = -1;
	float shockPulsePerSec = -1;
	float unconChancePerSec = -1;
	float vignetteStrength = -1;
	float edgeBlurStrength = -1;
	float gurgleSfxStrength = -1;
	float coughChancePerMin = -1;
	bool coughBloodEnabled = false;
	float sprintAttemptShock = -1;
	float legsSprintMult = -1;
	bool legsSprintAllowed = true;
	bool legsRunAllowed = true;
	float armsAimShake = -1;
	float dropChancePerHit = -1;
	float actionTimeMult = -1;
};

class OMD_NewDamageTreatableConfig
{
	bool heart = false;
	bool lung = true;
	bool liver = true;
	bool guts = true;
	bool spleen = false;
	bool face = true;
	bool artery_limb = true;
	bool artery_femoral = true;
	bool artery_carotid = true;
	bool artery_subclavian = true;
	bool scratch = true;
	bool deep_limb = true;
	bool bullet_limb = true;
	bool bullet_torso = true;
	bool bullet_neck = true;
};

class OMD_NewDamageSelfTreatAllowedConfig
{
	bool scratch = true;
	bool deep_limb = true;
	bool bullet_limb = true;
	bool artery_limb = false;
	bool artery_neck = false;
	bool bullet_torso = false;
	bool bullet_neck = false;
	bool heart = false;
	bool lung = false;
	bool liver = false;
	bool guts = false;
	bool spleen = false;
	bool face = false;
	bool back_wound = false;
};

class OMD_NewDamageTreatmentEntry
{
	string damageId = "";
	ref array<string> items;
	string result = "";
	float time = 0;
	float failChance = 0;
};

enum OMD_NewDamage_ERPCs
{
	RPC_PRESS_MODE_EXIT = 3917501
};
