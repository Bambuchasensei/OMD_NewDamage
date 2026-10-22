class OMD_NewDamageConfig
{
	protected static const string CONFIG_DIR = "$profile:omd";
	protected static const string CONFIG_PATH = "$profile:omd/OMD_NewDamage.json";
	protected static ref OMD_NewDamageConfigData s_Data;
	protected static bool s_ObsoleteKeysLogged;


	static OMD_NewDamageConfigData Get()
	{
		if (!s_Data)
		{
			Load();
		}

		bool symptomDebug;
		symptomDebug = false;
		if (s_Data)
		{
			symptomDebug = s_Data.debugEnabled;
		}
		OMD_SetNewDamageSymptomDebug(symptomDebug);

		return s_Data;
	}


	static void Load()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageConfigData();
		}

		if (!FileExist(CONFIG_DIR))
		{
			MakeDirectory(CONFIG_DIR);
		}

		if (!FileExist(CONFIG_PATH))
		{
			SeedDefaults();
			Save();
			LogLoaded();
			return;
		}

		OMD_NewDamageConfigData loaded = new OMD_NewDamageConfigData();
		JsonFileLoader<OMD_NewDamageConfigData>.JsonLoadFile(CONFIG_PATH, loaded);
		s_Data = loaded;
		WarnObsoleteKeysOnce();
		if (!s_Data.debugAdminIds)
		{
			s_Data.debugAdminIds = new array<string>;
		}
		if (s_Data.debugChatPrefix == "")
		{
			s_Data.debugChatPrefix = "\\";
		}
		if (!s_Data.meleeForceBluntItems)
		{
			s_Data.meleeForceBluntItems = new array<string>;
		}
		if (!s_Data.treatable)
		{
			s_Data.treatable = new OMD_NewDamageTreatableConfig();
		}
		if (!s_Data.selfTreatAllowed)
		{
			s_Data.selfTreatAllowed = new OMD_NewDamageSelfTreatAllowedConfig();
		}
		if (!s_Data.treatments)
		{
			s_Data.treatments = new array<ref OMD_NewDamageTreatmentEntry>;
		}
		if (s_Data.holdPressureBaseReduce < 0)
		{
			s_Data.holdPressureBaseReduce = s_Data.holdPressureBleedReduceMin;
		}

		if (s_Data.holdPressureExitHoldSeconds < 0.05)
		{
			s_Data.holdPressureExitHoldSeconds = 0.05;
		}

		if (s_Data.holdPressureMaxDistance < 0.1)
		{
			s_Data.holdPressureMaxDistance = 0.1;
		}

		if (s_Data.holdPressureMode == "")
		{
			s_Data.holdPressureMode = "mode";
		}

		if (s_Data.holdPressureAnimSelf == "" || s_Data.holdPressureAnimSelf == "CMD_ACTIONMOD_BLOODTEST")
		{
			s_Data.holdPressureAnimSelf = "CMD_ACTIONFB_SALINEBLOODBAG";
		}

		if (s_Data.holdPressureMode == "hold")
		{
			s_Data.holdPressureMode = "mode";
			s_Data.holdPressureAppliesToVanillaBleed = false;
			s_Data.holdPressureManualSelectEnabled = true;
			if (s_Data.holdPressureExitHoldSeconds <= 0)
			{
				s_Data.holdPressureExitHoldSeconds = 0.3;
			}

			if (s_Data.holdPressureMaxDistance <= 0)
			{
				s_Data.holdPressureMaxDistance = 2.0;
			}
		}

		if (s_Data.unconsciousBleedMult < 0)
		{
			s_Data.unconsciousBleedMult = 0.30;
		}

		EnsureDebuffDefaults(s_Data);
		OMD_NewDamage_RestoreMissingRunFlags(s_Data);
		EnsureOrganSourceFlags();
		EnsureCrashOrganKeys();
		EnsureBrainKeys();
		EnsureJumpOutKeys();
		EnsureOrganMinSpeedCoef();
		EnsureFaceFallCrashDamage();
		Save();
		LogLoaded();
	}


	protected static void EnsureOrganSourceFlags()
	{
		if (!s_Data)
		{
			return;
		}

		if (!FileExist(CONFIG_PATH))
		{
			return;
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return;
		}

		string line;
		bool foundInfected;
		foundInfected = false;
		bool foundAnimal;
		foundAnimal = false;
		bool foundBlunt;
		foundBlunt = false;
		bool foundFist;
		foundFist = false;
		while (FGets(handle, line) >= 0)
		{
			if (line.Contains("organSourceEnabled_infected"))
			{
				foundInfected = true;
			}

			if (line.Contains("organSourceEnabled_animal"))
			{
				foundAnimal = true;
			}

			if (line.Contains("organSourceEnabled_blunt"))
			{
				foundBlunt = true;
			}

			if (line.Contains("organSourceEnabled_fist"))
			{
				foundFist = true;
			}
		}

		CloseFile(handle);

		if (!foundInfected)
		{
			s_Data.organSourceEnabled_infected = true;
		}

		if (!foundAnimal)
		{
			s_Data.organSourceEnabled_animal = true;
		}

		if (!foundBlunt)
		{
			s_Data.organSourceEnabled_blunt = true;
		}

		if (!foundFist)
		{
			s_Data.organSourceEnabled_fist = true;
		}
	}


	protected static void FillJumpOutImpactMuls(OMD_NewDamageConfigData data)
	{
		if (!data)
		{
			return;
		}

		data.jumpOutSurfaceImpactMuls = new array<ref OMD_NewDamageJumpOutImpactMul>;
		AddJumpOutImpactMul(data, "Hit_Concrete", 1.0);
		AddJumpOutImpactMul(data, "Hit_Metal", 1.0);
		AddJumpOutImpactMul(data, "Hit_Gravel", 0.95);
		AddJumpOutImpactMul(data, "Hit_Wood", 0.9);
		AddJumpOutImpactMul(data, "Hit_Rubber", 0.85);
		AddJumpOutImpactMul(data, "Hit_Dirt", 0.8);
		AddJumpOutImpactMul(data, "Hit_Grass", 0.65);
		AddJumpOutImpactMul(data, "Hit_Foliage", 0.65);
		AddJumpOutImpactMul(data, "Hit_Textile", 0.7);
		AddJumpOutImpactMul(data, "Hit_Sand", 0.6);
		AddJumpOutImpactMul(data, "Hit_Snow", 0.55);
		AddJumpOutImpactMul(data, "Hit_Water", 0.3);
	}


	protected static void AddJumpOutImpactMul(OMD_NewDamageConfigData data, string impact, float mul)
	{
		if (!data)
		{
			return;
		}

		if (!data.jumpOutSurfaceImpactMuls)
		{
			return;
		}

		OMD_NewDamageJumpOutImpactMul entry;
		entry = new OMD_NewDamageJumpOutImpactMul();
		entry.impact = impact;
		entry.mul = mul;
		data.jumpOutSurfaceImpactMuls.Insert(entry);
	}


	protected static void FillJumpOutNameMuls(OMD_NewDamageConfigData data)
	{
		if (!data)
		{
			return;
		}

		data.jumpOutSurfaceNameMuls = new array<ref OMD_NewDamageJumpOutNameMul>;
		AddJumpOutNameMul(data, "cp_grass_tall", 0.55);
		AddJumpOutNameMul(data, "cp_rock", 1.1);
	}


	protected static void AddJumpOutNameMul(OMD_NewDamageConfigData data, string surfaceName, float mul)
	{
		if (!data)
		{
			return;
		}

		if (!data.jumpOutSurfaceNameMuls)
		{
			return;
		}

		OMD_NewDamageJumpOutNameMul entry;
		entry = new OMD_NewDamageJumpOutNameMul();
		entry.name = surfaceName;
		entry.mul = mul;
		data.jumpOutSurfaceNameMuls.Insert(entry);
	}


	protected static void EnsureJumpOutKeys()
	{
		if (!s_Data)
		{
			return;
		}

		if (!FileExist(CONFIG_PATH))
		{
			return;
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return;
		}

		string line;
		bool foundEnabled;
		foundEnabled = false;
		bool foundMin;
		foundMin = false;
		bool foundMax;
		foundMax = false;
		bool foundShare;
		foundShare = false;
		bool foundPoolMax;
		foundPoolMax = false;
		bool foundLegPain;
		foundLegPain = false;
		bool foundFractureMin;
		foundFractureMin = false;
		bool foundFractureMax;
		foundFractureMax = false;
		bool foundFaceChance;
		foundFaceChance = false;
		bool foundFaceDamage;
		foundFaceDamage = false;
		bool foundArteryChance;
		foundArteryChance = false;
		bool foundDefaultMul;
		foundDefaultMul = false;
		bool foundImpactMuls;
		foundImpactMuls = false;
		bool foundNameMuls;
		foundNameMuls = false;
		while (FGets(handle, line) >= 0)
		{
			if (line.Contains("jumpOutInjuriesEnabled"))
			{
				foundEnabled = true;
			}

			if (line.Contains("jumpOutMinIncoming"))
			{
				foundMin = true;
			}

			if (line.Contains("jumpOutMaxIncoming"))
			{
				foundMax = true;
			}

			if (line.Contains("organShare_jumpout"))
			{
				foundShare = true;
			}

			if (line.Contains("jumpOutOrganPoolMax"))
			{
				foundPoolMax = true;
			}

			if (line.Contains("jumpOutLegPainMax"))
			{
				foundLegPain = true;
			}

			if (line.Contains("jumpOutLegFractureChanceMin"))
			{
				foundFractureMin = true;
			}

			if (line.Contains("jumpOutLegFractureChanceMax"))
			{
				foundFractureMax = true;
			}

			if (line.Contains("jumpOutFaceHitChancePercent"))
			{
				foundFaceChance = true;
			}

			if (line.Contains("jumpOutFaceHitDamage"))
			{
				foundFaceDamage = true;
			}

			if (line.Contains("jumpOutArteryHitChancePercent"))
			{
				foundArteryChance = true;
			}

			if (line.Contains("jumpOutSurfaceDefaultMul"))
			{
				foundDefaultMul = true;
			}

			if (line.Contains("jumpOutSurfaceImpactMuls"))
			{
				foundImpactMuls = true;
			}

			if (line.Contains("jumpOutSurfaceNameMuls"))
			{
				foundNameMuls = true;
			}
		}

		CloseFile(handle);

		if (!foundEnabled)
		{
			s_Data.jumpOutInjuriesEnabled = true;
		}

		if (!foundMin)
		{
			s_Data.jumpOutMinIncoming = 30.0;
		}

		if (!foundMax)
		{
			s_Data.jumpOutMaxIncoming = 100.0;
		}

		if (!foundShare)
		{
			s_Data.organShare_jumpout = 0.6;
		}

		if (!foundPoolMax)
		{
			s_Data.jumpOutOrganPoolMax = 60.0;
		}

		if (!foundLegPain)
		{
			s_Data.jumpOutLegPainMax = 60.0;
		}

		if (!foundFractureMin)
		{
			s_Data.jumpOutLegFractureChanceMin = 5.0;
		}

		if (!foundFractureMax)
		{
			s_Data.jumpOutLegFractureChanceMax = 85.0;
		}

		if (!foundFaceChance)
		{
			s_Data.jumpOutFaceHitChancePercent = 15.0;
		}

		if (!foundFaceDamage)
		{
			s_Data.jumpOutFaceHitDamage = 10.0;
		}

		if (!foundArteryChance)
		{
			s_Data.jumpOutArteryHitChancePercent = 5.0;
		}

		if (!foundDefaultMul)
		{
			s_Data.jumpOutSurfaceDefaultMul = 0.8;
		}

		if (!foundImpactMuls)
		{
			FillJumpOutImpactMuls(s_Data);
		}

		if (!foundNameMuls)
		{
			FillJumpOutNameMuls(s_Data);
		}
	}


	protected static void EnsureCrashOrganKeys()
	{
		if (!s_Data)
		{
			return;
		}

		if (!FileExist(CONFIG_PATH))
		{
			return;
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return;
		}

		string line;
		bool foundEnabled;
		foundEnabled = false;
		bool foundShare;
		foundShare = false;
		bool foundPoolMax;
		foundPoolMax = false;
		while (FGets(handle, line) >= 0)
		{
			if (line.Contains("crashOrgansEnabled"))
			{
				foundEnabled = true;
			}

			if (line.Contains("organShare_crash"))
			{
				foundShare = true;
			}

			if (line.Contains("crashOrganPoolMax"))
			{
				foundPoolMax = true;
			}
		}

		CloseFile(handle);

		if (!foundEnabled)
		{
			s_Data.crashOrgansEnabled = true;
		}

		if (!foundShare)
		{
			s_Data.organShare_crash = 1.2;
		}

		if (!foundPoolMax)
		{
			s_Data.crashOrganPoolMax = 150;
		}
	}


	protected static void EnsureBrainKeys()
	{
		if (!s_Data)
		{
			return;
		}

		if (!FileExist(CONFIG_PATH))
		{
			return;
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return;
		}

		string line;
		bool foundEnabled;
		foundEnabled = false;
		bool foundThreshold;
		foundThreshold = false;
		bool foundFracture;
		foundFracture = false;
		bool foundNoFracture;
		foundNoFracture = false;
		while (FGets(handle, line) >= 0)
		{
			if (line.Contains("brainEnabled"))
			{
				foundEnabled = true;
			}

			if (line.Contains("brainDeathThreshold"))
			{
				foundThreshold = true;
			}

			if (line.Contains("brainFractureMul"))
			{
				foundFracture = true;
			}

			if (line.Contains("brainNoFractureMul"))
			{
				foundNoFracture = true;
			}
		}

		CloseFile(handle);

		if (!foundEnabled)
		{
			s_Data.brainEnabled = true;
		}

		if (!foundThreshold)
		{
			s_Data.brainDeathThreshold = 100;
		}

		if (!foundFracture)
		{
			s_Data.brainFractureMul = 2.0;
		}

		if (!foundNoFracture)
		{
			s_Data.brainNoFractureMul = 0.15;
		}
	}


	protected static void EnsureOrganMinSpeedCoef()
	{
		if (!s_Data)
		{
			return;
		}

		if (!FileExist(CONFIG_PATH))
		{
			return;
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return;
		}

		string line;
		bool foundMinSpeed;
		foundMinSpeed = false;
		while (FGets(handle, line) >= 0)
		{
			if (line.Contains("organMinSpeedCoefFirearm"))
			{
				foundMinSpeed = true;
			}
		}

		CloseFile(handle);

		if (!foundMinSpeed)
		{
			s_Data.organMinSpeedCoefFirearm = 0.2;
		}
	}


	protected static void EnsureFaceFallCrashDamage()
	{
		if (!s_Data)
		{
			return;
		}

		if (!FileExist(CONFIG_PATH))
		{
			return;
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return;
		}

		string line;
		bool foundFall;
		foundFall = false;
		bool foundCrash;
		foundCrash = false;
		while (FGets(handle, line) >= 0)
		{
			if (line.Contains("fallFaceDmgPerMeter"))
			{
				foundFall = true;
			}

			if (line.Contains("crashFaceHitDamage"))
			{
				foundCrash = true;
			}
		}

		CloseFile(handle);

		if (!foundFall)
		{
			s_Data.fallFaceDmgPerMeter = 1.2;
		}

		if (!foundCrash)
		{
			s_Data.crashFaceHitDamage = 20.0;
		}
	}


	protected static string OMD_NewDamage_ReadConfigText()
	{
		if (!FileExist(CONFIG_PATH))
		{
			return "";
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return "";
		}

		string text;
		text = "";
		string line;
		while (FGets(handle, line) >= 0)
		{
			text = text + line;
		}

		CloseFile(handle);
		return text;
	}


	protected static bool OMD_NewDamage_IsStageObjectKey(string jsonText, int nameAt, int nameLen)
	{
		if (nameAt > 0)
		{
			string before;
			before = jsonText.Get(nameAt - 1);
			if (before != "\"")
			{
				return false;
			}
		}

		int i;
		i = nameAt + nameLen;
		int len;
		len = jsonText.Length();
		if (i >= len)
		{
			return false;
		}

		string ch;
		ch = jsonText.Get(i);
		if (ch != "\"")
		{
			return false;
		}

		i = i + 1;
		while (i < len)
		{
			ch = jsonText.Get(i);
			bool skip;
			skip = false;
			if (ch == " ")
			{
				skip = true;
			}

			if (ch == "\t")
			{
				skip = true;
			}

			if (ch == "\n")
			{
				skip = true;
			}

			if (ch == "\r")
			{
				skip = true;
			}

			if (!skip)
			{
				return ch == ":";
			}

			i = i + 1;
		}

		return false;
	}


	protected static int OMD_NewDamage_FindStageKey(string jsonText, string stageName)
	{
		string rest;
		rest = jsonText;
		int base;
		base = 0;
		int guard;
		guard = 0;
		int nameLen;
		nameLen = stageName.Length();
		while (guard < 64)
		{
			guard = guard + 1;
			int at;
			at = rest.IndexOf(stageName);
			if (at < 0)
			{
				return -1;
			}

			int abs;
			abs = base + at;
			if (OMD_NewDamage_IsStageObjectKey(jsonText, abs, nameLen))
			{
				return abs;
			}

			int next;
			next = at + nameLen;
			base = base + next;
			int remain;
			remain = jsonText.Length() - base;
			if (remain < 1)
			{
				return -1;
			}

			rest = jsonText.Substring(base, remain);
		}

		return -1;
	}


	protected static bool OMD_NewDamage_JsonStageHasKey(string jsonText, string stageName, string key)
	{
		if (jsonText == "")
		{
			return true;
		}

		int start;
		start = OMD_NewDamage_FindStageKey(jsonText, stageName);
		if (start < 0)
		{
			return true;
		}

		int len;
		len = jsonText.Length();
		int i;
		i = start;
		int depth;
		depth = 0;
		bool inObject;
		inObject = false;
		int bodyStart;
		bodyStart = start;
		int bodyEnd;
		bodyEnd = len;
		string ch;
		while (i < len)
		{
			ch = jsonText.Get(i);
			if (!inObject)
			{
				if (ch == "{")
				{
					inObject = true;
					depth = 1;
					bodyStart = i;
				}
			}
			else
			{
				if (ch == "{")
				{
					depth = depth + 1;
				}

				if (ch == "}")
				{
					depth = depth - 1;
					if (depth == 0)
					{
						bodyEnd = i;
						i = len;
					}
				}
			}

			i = i + 1;
		}

		int bodyLen;
		bodyLen = bodyEnd - bodyStart;
		if (bodyLen < 1)
		{
			return true;
		}

		string body;
		body = jsonText.Substring(bodyStart, bodyLen);
		return body.Contains(key);
	}


	protected static void OMD_NewDamage_RestoreStageRunFlag(OMD_NewDamageDebuffStageConfig stage, string jsonText, string stageName)
	{
		if (!stage)
		{
			return;
		}

		if (!OMD_NewDamage_JsonStageHasKey(jsonText, stageName, "runAllowed"))
		{
			stage.runAllowed = true;
		}

		if (!OMD_NewDamage_JsonStageHasKey(jsonText, stageName, "legsRunAllowed"))
		{
			stage.legsRunAllowed = true;
		}
	}


	protected static void OMD_NewDamage_RestoreMissingRunFlags(OMD_NewDamageConfigData data)
	{
		if (!data)
		{
			return;
		}

		string jsonText;
		jsonText = OMD_NewDamage_ReadConfigText();
		if (jsonText == "")
		{
			return;
		}

		OMD_NewDamage_RestoreStageRunFlag(data.heart_L1, jsonText, "heart_L1");
		OMD_NewDamage_RestoreStageRunFlag(data.heart_L2, jsonText, "heart_L2");
		OMD_NewDamage_RestoreStageRunFlag(data.heart_L3, jsonText, "heart_L3");
		OMD_NewDamage_RestoreStageRunFlag(data.lung_L1, jsonText, "lung_L1");
		OMD_NewDamage_RestoreStageRunFlag(data.lung_L2, jsonText, "lung_L2");
		OMD_NewDamage_RestoreStageRunFlag(data.lung_L3, jsonText, "lung_L3");
		OMD_NewDamage_RestoreStageRunFlag(data.face_L1, jsonText, "face_L1");
		OMD_NewDamage_RestoreStageRunFlag(data.face_L2, jsonText, "face_L2");
		OMD_NewDamage_RestoreStageRunFlag(data.face_L3, jsonText, "face_L3");
		OMD_NewDamage_RestoreStageRunFlag(data.artery_L1, jsonText, "artery_L1");
		OMD_NewDamage_RestoreStageRunFlag(data.artery_L2, jsonText, "artery_L2");
		OMD_NewDamage_RestoreStageRunFlag(data.artery_L3, jsonText, "artery_L3");
	}


	protected static void EnsureDebuffDefaults(OMD_NewDamageConfigData data)
	{
		if (!data)
		{
			return;
		}

		if (!data.heart_L1)
		{
			data.heart_L1 = MakeHeartL1();
		}

		if (!data.heart_L2)
		{
			data.heart_L2 = MakeHeartL2();
		}

		if (!data.heart_L3)
		{
			data.heart_L3 = MakeHeartL3();
		}

		if (!data.lung_L1)
		{
			data.lung_L1 = MakeLungL1();
		}

		if (!data.lung_L2)
		{
			data.lung_L2 = MakeLungL2();
		}

		if (!data.lung_L3)
		{
			data.lung_L3 = MakeLungL3();
		}

		if (!data.face_L1)
		{
			data.face_L1 = MakeFaceL1();
		}

		if (!data.face_L2)
		{
			data.face_L2 = MakeFaceL2();
		}

		if (!data.face_L3)
		{
			data.face_L3 = MakeFaceL3();
		}

		if (!data.artery_L1)
		{
			data.artery_L1 = MakeArteryL1();
		}

		if (!data.artery_L2)
		{
			data.artery_L2 = MakeArteryL2();
		}

		if (!data.artery_L3)
		{
			data.artery_L3 = MakeArteryL3();
		}

		if (!data.hudLimbColorPriority)
		{
			data.hudLimbColorPriority = new array<string>;
			data.hudLimbColorPriority.Insert("artery_L3");
			data.hudLimbColorPriority.Insert("fracture_open");
			data.hudLimbColorPriority.Insert("deep_heavy");
			data.hudLimbColorPriority.Insert("artery_L2");
			data.hudLimbColorPriority.Insert("deep_med");
			data.hudLimbColorPriority.Insert("fracture_splinted");
			data.hudLimbColorPriority.Insert("scratch");
			data.hudLimbColorPriority.Insert("pain");
		}

		if (data.sfxHeartHeartbeat_L1 == "")
		{
			data.sfxHeartHeartbeat_L1 = "OMD_Heart_Heartbeat_L1_SoundSet";
		}

		if (data.sfxHeartHeartbeat_L2 == "")
		{
			data.sfxHeartHeartbeat_L2 = "OMD_Heart_Heartbeat_L2_SoundSet";
		}

		if (data.sfxHeartHeartbeat_L3 == "")
		{
			data.sfxHeartHeartbeat_L3 = "OMD_Heart_Heartbeat_L3_SoundSet";
		}

		if (data.sfxLungCoughBlood == "")
		{
			data.sfxLungCoughBlood = "OMD_Lung_CoughBlood_SoundSet";
		}

		if (data.sfxLungGurgle == "")
		{
			data.sfxLungGurgle = "OMD_Lung_Gurgle_SoundSet";
		}

		if (data.sfxLungBreathHeavy == "")
		{
			data.sfxLungBreathHeavy = "OMD_Lung_BreathHeavy_SoundSet";
		}

		if (data.debuffTickSeconds < 0.1)
		{
			data.debuffTickSeconds = 1.0;
		}

		if (data.organLevelThreshold_L1 <= 0)
		{
			data.organLevelThreshold_L1 = 20;
		}

		if (data.organLevelThreshold_L2 <= data.organLevelThreshold_L1)
		{
			data.organLevelThreshold_L2 = 50;
		}

		if (data.organLevelThreshold_L3 <= data.organLevelThreshold_L2)
		{
			data.organLevelThreshold_L3 = 80;
		}

		if (data.organSelfHealMax < 0)
		{
			data.organSelfHealMax = 50;
		}

		if (data.organSelfHealPerSec < 0)
		{
			data.organSelfHealPerSec = 0.002;
		}

		if (data.organCriticalThreshold < data.organLevelThreshold_L3)
		{
			data.organCriticalThreshold = data.organLevelThreshold_L3;
		}

		if (data.healMultBase <= 0)
		{
			data.healMultBase = 1.0;
		}

		if (data.healMultSleeping <= 0)
		{
			data.healMultSleeping = 2.0;
		}

		if (data.organDamageMult_heart <= 0)
		{
			data.organDamageMult_heart = 2.0;
		}

		if (data.organDamageMult_lung <= 0)
		{
			data.organDamageMult_lung = 1.5;
		}

		if (data.organDamageMult_liver <= 0)
		{
			data.organDamageMult_liver = 1.5;
		}

		if (data.organDamageMult_spleen <= 0)
		{
			data.organDamageMult_spleen = 1.2;
		}

		if (data.organDamageMult_guts <= 0)
		{
			data.organDamageMult_guts = 1.0;
		}

		if (data.organDamageMult_face <= 0)
		{
			data.organDamageMult_face = 1.0;
		}

		if (data.fallGlobalHealthMult < 0)
		{
			data.fallGlobalHealthMult = 0.3;
		}

		if (data.transportGlobalHealthMult < 0)
		{
			data.transportGlobalHealthMult = 0.4;
		}

		if (data.crashGlobalHealthMult < 0)
		{
			data.crashGlobalHealthMult = 0.4;
		}

		if (data.globalHealthFloor_fall < 0)
		{
			data.globalHealthFloor_fall = 10;
		}

		if (data.globalHealthFloor_transport < 0)
		{
			data.globalHealthFloor_transport = 10;
		}

		if (data.globalHealthFloor_crash < 0)
		{
			data.globalHealthFloor_crash = 10;
		}

		if (data.fallOrganMinHeight < 0)
		{
			data.fallOrganMinHeight = 3;
		}

		if (data.fallOrganDmgPerMeter < 0)
		{
			data.fallOrganDmgPerMeter = 6.0;
		}

		if (data.fallOrganPoolMax < 0)
		{
			data.fallOrganPoolMax = 0;
		}

		if (data.fallExtraHpStartHeight < 0)
		{
			data.fallExtraHpStartHeight = 20;
		}

		if (data.fallExtraHpPerMeter < 0)
		{
			data.fallExtraHpPerMeter = 5.0;
		}

		if (data.fallExtraHpMax < 0)
		{
			data.fallExtraHpMax = 0;
		}

		if (data.fallOrganHitChance < 0)
		{
			data.fallOrganHitChance = 1.0;
		}

		if (data.fallOrganHitChance > 1.0)
		{
			data.fallOrganHitChance = 1.0;
		}

		if (data.fallOrganRandMin < 0)
		{
			data.fallOrganRandMin = 0.7;
		}

		if (data.fallOrganRandMax < 0)
		{
			data.fallOrganRandMax = 1.3;
		}

		if (data.fallOrganRandMin > data.fallOrganRandMax)
		{
			data.fallOrganRandMin = 0.7;
			data.fallOrganRandMax = 1.3;
		}

		if (data.organShare_transport < 0)
		{
			data.organShare_transport = 0.8;
		}

		if (data.organShare_crash < 0)
		{
			data.organShare_crash = 1.2;
		}

		if (data.crashOrganPoolMax < 0)
		{
			data.crashOrganPoolMax = 150;
		}

		if (data.transportOrganPoolMax < 0)
		{
			data.transportOrganPoolMax = 150;
		}

		if (data.crashOrganRandMin < 0)
		{
			data.crashOrganRandMin = 0.7;
		}

		if (data.crashOrganRandMax < 0)
		{
			data.crashOrganRandMax = 1.3;
		}

		if (data.crashOrganRandMin > data.crashOrganRandMax)
		{
			data.crashOrganRandMin = 0.7;
			data.crashOrganRandMax = 1.3;
		}

		if (data.crashLimbDamageMult < 0)
		{
			data.crashLimbDamageMult = 1.5;
		}

		if (data.fallFaceChance < 0)
		{
			data.fallFaceChance = 0.5;
		}

		if (data.fallFaceChance > 1.0)
		{
			data.fallFaceChance = 1.0;
		}

		if (data.fallArteryChance < 0)
		{
			data.fallArteryChance = 0.5;
		}

		if (data.fallArteryChance > 1.0)
		{
			data.fallArteryChance = 1.0;
		}

		if (data.fallArteryWeight_femoral < 0)
		{
			data.fallArteryWeight_femoral = 1;
		}

		if (data.fallArteryWeight_legsL < 0)
		{
			data.fallArteryWeight_legsL = 1;
		}

		if (data.fallArteryWeight_legsR < 0)
		{
			data.fallArteryWeight_legsR = 1;
		}

		if (data.fallArteryWeight_armsL < 0)
		{
			data.fallArteryWeight_armsL = 1;
		}

		if (data.fallArteryWeight_armsR < 0)
		{
			data.fallArteryWeight_armsR = 1;
		}

		if (data.fallArteryWeight_subclavianL < 0)
		{
			data.fallArteryWeight_subclavianL = 0.35;
		}

		if (data.fallArteryWeight_subclavianR < 0)
		{
			data.fallArteryWeight_subclavianR = 0.35;
		}

		if (data.crashFaceChance < 0)
		{
			data.crashFaceChance = 0.5;
		}

		if (data.crashFaceChance > 1.0)
		{
			data.crashFaceChance = 1.0;
		}

		if (data.crashArteryChance < 0)
		{
			data.crashArteryChance = 0.5;
		}

		if (data.crashArteryChance > 1.0)
		{
			data.crashArteryChance = 1.0;
		}

		if (data.crashArteryWeight_femoral < 0)
		{
			data.crashArteryWeight_femoral = 1;
		}

		if (data.crashArteryWeight_legsL < 0)
		{
			data.crashArteryWeight_legsL = 1;
		}

		if (data.crashArteryWeight_legsR < 0)
		{
			data.crashArteryWeight_legsR = 1;
		}

		if (data.crashArteryWeight_armsL < 0)
		{
			data.crashArteryWeight_armsL = 1;
		}

		if (data.crashArteryWeight_armsR < 0)
		{
			data.crashArteryWeight_armsR = 1;
		}

		if (data.crashArteryWeight_carotid < 0)
		{
			data.crashArteryWeight_carotid = 1;
		}

		if (data.crashArteryWeight_subclavianL < 0)
		{
			data.crashArteryWeight_subclavianL = 1;
		}

		if (data.crashArteryWeight_subclavianR < 0)
		{
			data.crashArteryWeight_subclavianR = 1;
		}

		if (data.fallChanceFullHeight <= 0)
		{
			data.fallChanceFullHeight = 20;
		}

		if (data.crashChanceFullSpeedKmh <= 0)
		{
			data.crashChanceFullSpeedKmh = 100;
		}

		if (data.organSpread_heart < 0)
		{
			data.organSpread_heart = 1;
		}

		if (data.organSpread_lung < 0)
		{
			data.organSpread_lung = 1.5;
		}

		if (data.organSpread_liver < 0)
		{
			data.organSpread_liver = 1.5;
		}

		if (data.organSpread_spleen < 0)
		{
			data.organSpread_spleen = 1.5;
		}

		if (data.organSpread_guts < 0)
		{
			data.organSpread_guts = 1;
		}

		if (data.debuffStackMode == "")
		{
			data.debuffStackMode = "maxPerAxis";
		}

		if (data.organShockFloorOffset < 0)
		{
			data.organShockFloorOffset = 5.0;
		}

		if (!data.organUnconChanceEnabled)
		{
			OMD_NewDamage_ZeroStageUnconChance(data.heart_L1);
			OMD_NewDamage_ZeroStageUnconChance(data.heart_L2);
			OMD_NewDamage_ZeroStageUnconChance(data.heart_L3);
			OMD_NewDamage_ZeroStageUnconChance(data.lung_L1);
			OMD_NewDamage_ZeroStageUnconChance(data.lung_L2);
			OMD_NewDamage_ZeroStageUnconChance(data.lung_L3);
			OMD_NewDamage_ZeroStageUnconChance(data.face_L1);
			OMD_NewDamage_ZeroStageUnconChance(data.face_L2);
			OMD_NewDamage_ZeroStageUnconChance(data.face_L3);
			OMD_NewDamage_ZeroStageUnconChance(data.artery_L1);
			OMD_NewDamage_ZeroStageUnconChance(data.artery_L2);
			OMD_NewDamage_ZeroStageUnconChance(data.artery_L3);
		}
	}


	protected static void OMD_NewDamage_ZeroStageUnconChance(OMD_NewDamageDebuffStageConfig stage)
	{
		if (!stage)
		{
			return;
		}

		stage.unconChancePerSec = 0;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeHeartL1()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.sprintSpeedMult = 0.80;
		s.staminaRegenMult = 0.85;
		s.aimShake = 1.2;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeHeartL2()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.sprintAllowed = false;
		s.staminaRegenMult = 0.60;
		s.staminaCapMult = 0.70;
		s.adsShockPerSec = 0.20;
		s.vignetteStrength = 0.25;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeHeartL3()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.runSpeedMult = 0.55;
		s.staminaRegenMult = 0.25;
		s.shockPerSecBonus = 0.05;
		s.unconChancePerSec = 0;
		s.vignetteStrength = 0.55;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeLungL1()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.sprintAllowed = false;
		s.staminaDrainMult = 1.20;
		s.coughChancePerMin = 0.08;
		s.coughBloodEnabled = true;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeLungL2()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.runSpeedMult = 0.70;
		s.staminaDrainMult = 1.60;
		s.staminaCapMult = 0.50;
		s.sprintAttemptShock = 5.0;
		s.edgeBlurStrength = 0.35;
		s.gurgleSfxStrength = 0.5;
		s.coughChancePerMin = 0.20;
		s.coughBloodEnabled = true;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeLungL3()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.runAllowed = false;
		s.staminaDrainMult = 2.20;
		s.shockPerSecBonus = 0.05;
		s.edgeBlurStrength = 0.70;
		s.gurgleSfxStrength = 1.0;
		s.coughChancePerMin = 0.40;
		s.coughBloodEnabled = true;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeFaceL1()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.aimShake = 1.5;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeFaceL2()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.aimShake = 3.0;
		s.shockPerSec = 0.03;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeFaceL3()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.aimShake = 5.0;
		s.unconChancePerSec = 0;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeArteryL1()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.legsSprintMult = 0.85;
		s.armsAimShake = 1.5;
		s.dropChancePerHit = 0.05;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeArteryL2()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.legsSprintAllowed = false;
		s.armsAimShake = 3.0;
		s.dropChancePerHit = 0.15;
		return s;
	}


	protected static OMD_NewDamageDebuffStageConfig MakeArteryL3()
	{
		OMD_NewDamageDebuffStageConfig s;
		s = new OMD_NewDamageDebuffStageConfig();
		s.legsRunAllowed = false;
		s.armsAimShake = 5.0;
		s.dropChancePerHit = 0.35;
		s.unconChancePerSec = 0;
		return s;
	}


	protected static void WarnObsoleteKeysOnce()
	{
		if (s_ObsoleteKeysLogged)
		{
			return;
		}

		s_ObsoleteKeysLogged = true;

		if (!FileExist(CONFIG_PATH))
		{
			return;
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return;
		}

		string line;
		bool foundObsolete;
		foundObsolete = false;
		while (FGets(handle, line) >= 0)
		{
			if (line.Contains("DeathSeconds"))
			{
				foundObsolete = true;
				break;
			}

			if (line.Contains("arteryBleedMult_"))
			{
				foundObsolete = true;
				break;
			}

			if (line.Contains("subclavianBleedMult_"))
			{
				foundObsolete = true;
				break;
			}

			if (line.Contains("arteryBleedRate_"))
			{
				foundObsolete = true;
				break;
			}
		}

		CloseFile(handle);

		if (!foundObsolete)
		{
			return;
		}

		if (!s_Data)
		{
			return;
		}

		if (!s_Data.debugEnabled)
		{
			return;
		}

		Print("[OMD][DMG] obsolete key ignored (DeathSeconds / arteryBleedMult / arteryBleedRate)");
	}


	protected static void LogLoaded()
	{
		if (!s_Data)
		{
			return;
		}

		if (!s_Data.debugEnabled)
		{
			return;
		}

		string line;
		line = "[OMD][DMG] config loaded active=" + s_Data.activeEnabled.ToString();
		line = line + " debug=" + s_Data.debugEnabled.ToString();
		line = line + " vital=" + s_Data.vitalSystemEnabled.ToString();
		Print(line);
	}


	static void Save()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageConfigData();
		}

		if (!FileExist(CONFIG_DIR))
		{
			MakeDirectory(CONFIG_DIR);
		}

		JsonFileLoader<OMD_NewDamageConfigData>.JsonSaveFile(CONFIG_PATH, s_Data);
	}


	static void SeedDefaults()
	{
		s_Data = new OMD_NewDamageConfigData();
		s_Data.activeEnabled = true;
		s_Data.debugEnabled = false;
		s_Data.debugAdminIds = new array<string>;
		s_Data.debugChatPrefix = "\\";
		s_Data.globalHpLogEnabled = false;
		s_Data.hitCalibrationLogEnabled = false;

		s_Data.organResolveByCompEnabled = true;
		s_Data.vitalSystemEnabled = true;
		s_Data.organMinBloodDamage = 0;
		s_Data.organMinHealthDamageFirearm = 1;
		s_Data.organMinHealthDamageMelee = 3;
		s_Data.organMinSpeedCoefFirearm = 0.2;
		s_Data.organSideMode = "bulletDir";
		s_Data.organSideAxis = "x";
		s_Data.organSideDeadzone = 0.05;
		s_Data.organChanceMult = 1.0;
		s_Data.meleeOrgansEnabled = true;
		s_Data.meleeOrganChanceMult = 0.25;
		s_Data.meleeArteryChanceMult = 0.5;
		s_Data.meleeBrainFromMelee = false;
		s_Data.brainEnabled = true;
		s_Data.brainDeathThreshold = 100;
		s_Data.brainFractureMul = 2.0;
		s_Data.brainNoFractureMul = 0.15;
		s_Data.meleeSharpMinBleedThreshold = 0;
		s_Data.meleeForceBluntItems = new array<string>;
		s_Data.organSourceEnabled_infected = true;
		s_Data.organSourceEnabled_animal = true;
		s_Data.organSourceEnabled_blunt = true;
		s_Data.organSourceEnabled_fist = true;
		s_Data.torsoBulletCountMax = 5;

		s_Data.hitComp_spine3_heart = 0.15;
		s_Data.hitComp_spine3_lung = 0.60;
		s_Data.hitComp_spine3_none = 0.25;

		s_Data.hitComp_spine2_heart = 0.10;
		s_Data.hitComp_spine2_lung = 0.45;
		s_Data.hitComp_spine2_liver = 0.20;
		s_Data.hitComp_spine2_none = 0.25;

		s_Data.hitComp_spine1_liver = 0.30;
		s_Data.hitComp_spine1_guts = 0.35;
		s_Data.hitComp_spine1_spleen = 0.10;
		s_Data.hitComp_spine1_none = 0.25;

		s_Data.hitComp_spine_liver = 0.25;
		s_Data.hitComp_spine_guts = 0.40;
		s_Data.hitComp_spine_spleen = 0.10;
		s_Data.hitComp_spine_none = 0.25;

		s_Data.hitComp_shoulder_lung = 0.25;
		s_Data.hitComp_shoulder_none = 0.75;

		s_Data.hitComp_torsoUnknown_lung = 0.28;
		s_Data.hitComp_torsoUnknown_liver = 0.18;
		s_Data.hitComp_torsoUnknown_guts = 0.18;
		s_Data.hitComp_torsoUnknown_spleen = 0.08;
		s_Data.hitComp_torsoUnknown_none = 0.28;

		s_Data.hitComp_pelvis_guts = 0.30;
		s_Data.hitComp_pelvis_femoral = 0.25;
		s_Data.hitComp_pelvis_spleen = 0.10;
		s_Data.hitComp_pelvis_none = 0.35;

		s_Data.hitComp_upleg_guts = 0.15;
		s_Data.hitComp_upleg_femoral = 0.40;
		s_Data.hitComp_upleg_none = 0.45;

		s_Data.hitComp_neck_carotid = 0.40;
		s_Data.hitComp_neck_heavy = 0.40;
		s_Data.hitComp_neck_light = 0.20;

		s_Data.hitComp_head_brain = 0.40;
		s_Data.hitComp_head_face = 0.50;
		s_Data.hitComp_head_none = 0.10;

		s_Data.hitComp_brain_brain = 1.0;

		s_Data.subclavianArteryChance = 0.15;
		s_Data.limbArteryChance = 0.15;

		s_Data.drainHp_heart_L1 = 0.238;
		s_Data.drainHp_heart_L2 = 0.556;
		s_Data.drainHp_heart_L3 = 1.667;
		s_Data.drainBlood_heart_L1 = 0;
		s_Data.drainBlood_heart_L2 = 0;
		s_Data.drainBlood_heart_L3 = 0;

		s_Data.drainHp_lungL_L1 = 0.111;
		s_Data.drainHp_lungL_L2 = 0.208;
		s_Data.drainHp_lungL_L3 = 0.556;
		s_Data.drainBlood_lungL_L1 = 0;
		s_Data.drainBlood_lungL_L2 = 0;
		s_Data.drainBlood_lungL_L3 = 0;

		s_Data.drainHp_lungR_L1 = 0.111;
		s_Data.drainHp_lungR_L2 = 0.208;
		s_Data.drainHp_lungR_L3 = 0.556;
		s_Data.drainBlood_lungR_L1 = 0;
		s_Data.drainBlood_lungR_L2 = 0;
		s_Data.drainBlood_lungR_L3 = 0;

		s_Data.drainHp_liver_L1 = 0;
		s_Data.drainHp_liver_L2 = 0;
		s_Data.drainHp_liver_L3 = 0;
		s_Data.drainBlood_liver_L1 = 3.472;
		s_Data.drainBlood_liver_L2 = 5.952;
		s_Data.drainBlood_liver_L3 = 11.905;

		s_Data.drainHp_guts_L1 = 0;
		s_Data.drainHp_guts_L2 = 0;
		s_Data.drainHp_guts_L3 = 0;
		s_Data.drainBlood_guts_L1 = 1.042;
		s_Data.drainBlood_guts_L2 = 1.667;
		s_Data.drainBlood_guts_L3 = 3.472;

		s_Data.drainHp_spleen_L1 = 0.111;
		s_Data.drainHp_spleen_L2 = 0.208;
		s_Data.drainHp_spleen_L3 = 0.556;
		s_Data.drainBlood_spleen_L1 = 0;
		s_Data.drainBlood_spleen_L2 = 0;
		s_Data.drainBlood_spleen_L3 = 0;

		s_Data.drainHp_face_L1 = 0;
		s_Data.drainHp_face_L2 = 0;
		s_Data.drainHp_face_L3 = 0;
		s_Data.drainBlood_face_L1 = 3.472;
		s_Data.drainBlood_face_L2 = 6.944;
		s_Data.drainBlood_face_L3 = 13.889;

		s_Data.drainHp_carotid_L1 = 0;
		s_Data.drainHp_carotid_L2 = 0;
		s_Data.drainHp_carotid_L3 = 0;
		s_Data.drainBlood_carotid_L1 = 20.833;
		s_Data.drainBlood_carotid_L2 = 41.667;
		s_Data.drainBlood_carotid_L3 = 83.333;

		s_Data.drainHp_subclavianL_L1 = 0;
		s_Data.drainHp_subclavianL_L2 = 0;
		s_Data.drainHp_subclavianL_L3 = 0;
		s_Data.drainBlood_subclavianL_L1 = 20.833;
		s_Data.drainBlood_subclavianL_L2 = 41.667;
		s_Data.drainBlood_subclavianL_L3 = 83.333;

		s_Data.drainHp_subclavianR_L1 = 0;
		s_Data.drainHp_subclavianR_L2 = 0;
		s_Data.drainHp_subclavianR_L3 = 0;
		s_Data.drainBlood_subclavianR_L1 = 20.833;
		s_Data.drainBlood_subclavianR_L2 = 41.667;
		s_Data.drainBlood_subclavianR_L3 = 83.333;

		s_Data.drainHp_femoral_L1 = 0;
		s_Data.drainHp_femoral_L2 = 0;
		s_Data.drainHp_femoral_L3 = 0;
		s_Data.drainBlood_femoral_L1 = 16.667;
		s_Data.drainBlood_femoral_L2 = 33.333;
		s_Data.drainBlood_femoral_L3 = 62.5;

		s_Data.drainHp_armsL_L1 = 0;
		s_Data.drainHp_armsL_L2 = 0;
		s_Data.drainHp_armsL_L3 = 0;
		s_Data.drainBlood_armsL_L1 = 13.889;
		s_Data.drainBlood_armsL_L2 = 27.778;
		s_Data.drainBlood_armsL_L3 = 55.556;

		s_Data.drainHp_armsR_L1 = 0;
		s_Data.drainHp_armsR_L2 = 0;
		s_Data.drainHp_armsR_L3 = 0;
		s_Data.drainBlood_armsR_L1 = 13.889;
		s_Data.drainBlood_armsR_L2 = 27.778;
		s_Data.drainBlood_armsR_L3 = 55.556;

		s_Data.drainHp_legsL_L1 = 0;
		s_Data.drainHp_legsL_L2 = 0;
		s_Data.drainHp_legsL_L3 = 0;
		s_Data.drainBlood_legsL_L1 = 13.889;
		s_Data.drainBlood_legsL_L2 = 27.778;
		s_Data.drainBlood_legsL_L3 = 55.556;

		s_Data.drainHp_legsR_L1 = 0;
		s_Data.drainHp_legsR_L2 = 0;
		s_Data.drainHp_legsR_L3 = 0;
		s_Data.drainBlood_legsR_L1 = 13.889;
		s_Data.drainBlood_legsR_L2 = 27.778;
		s_Data.drainBlood_legsR_L3 = 55.556;

		s_Data.selfTreatAllowAll = false;
		s_Data.selfTreatTimeMult = 1.5;
		s_Data.selfTreatFailChanceAdd = 0.15;
		s_Data.selfTreatAllowed = new OMD_NewDamageSelfTreatAllowedConfig();
		s_Data.treatable = new OMD_NewDamageTreatableConfig();
		s_Data.treatments = new array<ref OMD_NewDamageTreatmentEntry>;

		s_Data.vitalBloodDrainMult = 1.0;
		s_Data.vitalShockDrainMult = 1.0;
		s_Data.organShockPerSec_L1 = 0.02;
		s_Data.organShockPerSec_L2 = 0.05;
		s_Data.organShockPerSec_L3 = 0.08;
		s_Data.organShockFloorOffset = 5.0;
		s_Data.organUnconChanceEnabled = false;
		s_Data.unconsciousSlowdownEnabled = true;
		s_Data.unconsciousBleedMult = 0.30;
		s_Data.unconsciousOrganTimerMult = -1;
		s_Data.unconsciousOrganBloodDrainMult = -1;
		s_Data.unconsciousOrganShockDrainMult = -1;
		s_Data.unconsciousVanillaBleedMult = -1;
		s_Data.unconsciousInediaDeepWoundBleedMult = -1;
		s_Data.unconsciousHpDrainMult = -1;
		s_Data.liverActivateInediaInternalBleed = true;
		s_Data.spleenModelPosBiasEnabled = 1;
		s_Data.spleenModelPosBiasMult = 1.5;
		s_Data.spleenLocalXMax = -0.02;
		s_Data.spleenLocalYMaxFromSpine3 = -0.05;
		s_Data.debuffsEnabled = true;
		s_Data.debuffTickSeconds = 1.0;
		s_Data.debuffStackMode = "maxPerAxis";
		s_Data.lungUseWorstSide = true;
		s_Data.heartHeartbeatSfx_L1 = 0.3;
		s_Data.heartHeartbeatSfx_L2 = 0.6;
		s_Data.heartHeartbeatSfx_L3 = 1.0;
		s_Data.sfxHeartHeartbeat_L1 = "OMD_Heart_Heartbeat_L1_SoundSet";
		s_Data.sfxHeartHeartbeat_L2 = "OMD_Heart_Heartbeat_L2_SoundSet";
		s_Data.sfxHeartHeartbeat_L3 = "OMD_Heart_Heartbeat_L3_SoundSet";
		s_Data.heartDarkenStrength_L1 = 0.15;
		s_Data.heartDarkenStrength_L2 = 0.35;
		s_Data.heartDarkenStrength_L3 = 0.70;
		s_Data.sfxLungCoughBlood = "OMD_Lung_CoughBlood_SoundSet";
		s_Data.sfxLungGurgle = "OMD_Lung_Gurgle_SoundSet";
		s_Data.sfxLungBreathHeavy = "OMD_Lung_BreathHeavy_SoundSet";
		s_Data.faceWoundBlurStrength_L1 = 0.40;
		s_Data.faceWoundBlurStrength_L2 = 0.70;
		s_Data.faceWoundBlurStrength_L3 = 0.90;
		s_Data.inediaPainTargetLevel_OrganStage1 = 2;
		s_Data.inediaPainTargetLevel_OrganStage2 = 3;
		s_Data.inediaPainTargetLevel_OrganStage3 = 4;
		s_Data.inediaPainPercent_Level2 = 35;
		s_Data.inediaPainPercent_Level3 = 55;
		s_Data.inediaPainPercent_Level4 = 95;
		s_Data.lungCoughIntervalMin_L1 = 40;
		s_Data.lungCoughIntervalMax_L1 = 60;
		s_Data.lungCoughIntervalMin_L2 = 20;
		s_Data.lungCoughIntervalMax_L2 = 40;
		s_Data.lungCoughIntervalMin_L3 = 10;
		s_Data.lungCoughIntervalMax_L3 = 20;
		s_Data.lungCoughBloodChance_L1 = 0.15;
		s_Data.lungCoughBloodChance_L2 = 0.35;
		s_Data.lungCoughBloodChance_L3 = 0.60;
		s_Data.lungCoughBloodLoss = 50;
		s_Data.lungCoughBloodHandsEnabled = true;
		s_Data.fallGlobalHealthMult = 0.3;
		s_Data.transportGlobalHealthMult = 0.4;
		s_Data.crashGlobalHealthMult = 0.4;
		s_Data.globalHealthFloor_fall = 10;
		s_Data.globalHealthFloor_transport = 10;
		s_Data.globalHealthFloor_crash = 10;
		s_Data.fallOrgansEnabled = true;
		s_Data.transportOrgansEnabled = true;
		s_Data.crashOrgansEnabled = true;
		s_Data.fallOrganMinHeight = 3;
		s_Data.fallOrganDmgPerMeter = 6.0;
		s_Data.fallOrganPoolMax = 0;
		s_Data.fallExtraHpStartHeight = 20;
		s_Data.fallExtraHpPerMeter = 5.0;
		s_Data.fallExtraHpMax = 0;
		s_Data.fallOrganHitChance = 1.0;
		s_Data.fallOrganRandMin = 0.7;
		s_Data.fallOrganRandMax = 1.3;
		s_Data.organShare_transport = 0.8;
		s_Data.organShare_crash = 1.2;
		s_Data.crashOrganPoolMax = 150;
		s_Data.transportOrganPoolMax = 150;
		s_Data.crashOrganRandMin = 0.7;
		s_Data.crashOrganRandMax = 1.3;
		s_Data.crashLimbDamageMult = 1.5;
		s_Data.fallFaceChance = 0.5;
		s_Data.fallArteryChance = 0.5;
		s_Data.fallArteryWeight_femoral = 1;
		s_Data.fallArteryWeight_legsL = 1;
		s_Data.fallArteryWeight_legsR = 1;
		s_Data.fallArteryWeight_armsL = 1;
		s_Data.fallArteryWeight_armsR = 1;
		s_Data.fallArteryWeight_subclavianL = 0.35;
		s_Data.fallArteryWeight_subclavianR = 0.35;
		s_Data.crashFaceChance = 0.5;
		s_Data.crashArteryChance = 0.5;
		s_Data.crashArteryWeight_femoral = 1;
		s_Data.crashArteryWeight_legsL = 1;
		s_Data.crashArteryWeight_legsR = 1;
		s_Data.crashArteryWeight_armsL = 1;
		s_Data.crashArteryWeight_armsR = 1;
		s_Data.crashArteryWeight_carotid = 1;
		s_Data.crashArteryWeight_subclavianL = 1;
		s_Data.crashArteryWeight_subclavianR = 1;
		s_Data.fallChanceFullHeight = 20;
		s_Data.crashChanceFullSpeedKmh = 100;
		s_Data.organSpread_heart = 1;
		s_Data.organSpread_lung = 1.5;
		s_Data.organSpread_liver = 1.5;
		s_Data.organSpread_spleen = 1.5;
		s_Data.organSpread_guts = 1;
		s_Data.hudLimbTintLayerEnabled = true;
		s_Data.hudLimbTintRecolorBadges = false;
		EnsureDebuffDefaults(s_Data);

		s_Data.holdPressureEnabled = true;
		s_Data.holdPressureSelfEnabled = true;
		s_Data.holdPressureTargetEnabled = true;
		s_Data.holdPressureRequiresEmptyHands = true;
		s_Data.holdPressureMode = "mode";
		s_Data.holdPressureTimedSeconds = 8;
		s_Data.holdPressureTickSeconds = 0.5;
		s_Data.holdPressureBaseReduce = 0.50;
		s_Data.holdPressureBleedReduceMin = 0.50;
		s_Data.holdPressureBleedReduceMax = 1.00;
		s_Data.holdPressureUseMedSkill = false;
		s_Data.holdPressureMedSkillId = "med";
		s_Data.holdPressureMedLevelMax = 10;
		s_Data.holdPressureTimerReduceScale = 1.0;
		s_Data.holdPressureArteryLevelPenalty = 0.10;
		s_Data.holdPressureArteryLevelMultMin = 0.70;
		s_Data.holdPressureAppliesToDeepWound = true;
		s_Data.holdPressureAppliesToLimbArtery = true;
		s_Data.holdPressureAppliesToFemoral = true;
		s_Data.holdPressureAppliesToCarotid = true;
		s_Data.holdPressureAppliesToSubclavian = true;
		s_Data.holdPressureAppliesToVanillaBleed = false;
		s_Data.holdPressureAppliesToOrgans = false;
		s_Data.holdPressureMaxWounds = 1;
		s_Data.holdPressureMaxHoldersPerWound = 1;
		s_Data.holdPressureSelectMode = "mostDangerous";
		s_Data.holdPressureManualSelectEnabled = true;
		s_Data.holdPressureBlockIfTourniquet = true;
		s_Data.holdPressureBlockIfBandaged = true;
		s_Data.holdPressureAllowInventoryAccess = true;
		s_Data.holdPressureInterruptOnDamage = true;
		s_Data.holdPressureInterruptHealthDamage = 1;
		s_Data.holdPressureExitHoldSeconds = 0.3;
		s_Data.holdPressureMaxDistance = 2.0;
		s_Data.holdPressureAnimSelf = "CMD_ACTIONFB_SALINEBLOODBAG";
		s_Data.holdPressureAnimTarget = "CMD_ACTIONFB_SALINEBLOODBAGTARGET";
		s_Data.holdPressureStanceSelf = "erectOrCrouch";
		s_Data.holdPressureSilent = true;
		s_Data.holdPressureMedXpEnabled = false;
		s_Data.holdPressureMedXpIntervalSec = 30;
		s_Data.holdPressureMedXpAmount = 2;
		s_Data.jumpOutInjuriesEnabled = true;
		s_Data.jumpOutMinIncoming = 30.0;
		s_Data.jumpOutMaxIncoming = 100.0;
		s_Data.organShare_jumpout = 0.6;
		s_Data.jumpOutOrganPoolMax = 60.0;
		s_Data.jumpOutLegPainMax = 60.0;
		s_Data.jumpOutLegFractureChanceMin = 5.0;
		s_Data.jumpOutLegFractureChanceMax = 85.0;
		s_Data.jumpOutFaceHitChancePercent = 15.0;
		s_Data.jumpOutFaceHitDamage = 10.0;
		s_Data.jumpOutArteryHitChancePercent = 5.0;
		s_Data.jumpOutSurfaceDefaultMul = 0.8;
		FillJumpOutImpactMuls(s_Data);
		FillJumpOutNameMuls(s_Data);
	}


	static bool IsActive()
	{
		OMD_NewDamageConfigData data;
		data = Get();
		if (!data)
		{
			return false;
		}

		return data.activeEnabled;
	}


	static bool IsDebugEnabled()
	{
		OMD_NewDamageConfigData data;
		data = Get();
		if (!data)
		{
			return false;
		}

		return data.debugEnabled;
	}


	static bool IsDebugAdminId(string steamId)
	{
		if (steamId == "")
		{
			return false;
		}

		OMD_NewDamageConfigData data;
		data = Get();
		if (!data)
		{
			return false;
		}

		if (!data.debugAdminIds)
		{
			return false;
		}

		if (data.debugAdminIds.Find(steamId) < 0)
		{
			return false;
		}

		return true;
	}


	static string GetDebugChatPrefix()
	{
		OMD_NewDamageConfigData data;
		data = Get();
		if (!data)
		{
			return "\\";
		}

		if (data.debugChatPrefix == "")
		{
			return "\\";
		}

		return data.debugChatPrefix;
	}


	static bool IsVitalSystemEnabled()
	{
		OMD_NewDamageConfigData data;
		data = Get();
		if (!data)
		{
			return false;
		}

		if (!data.activeEnabled)
		{
			return false;
		}

		return data.vitalSystemEnabled;
	}


	static bool IsGlobalHpLogEnabled()
	{
		OMD_NewDamageConfigData data;
		data = Get();
		if (!data)
		{
			return false;
		}

		return data.globalHpLogEnabled;
	}


	static bool IsHitCalibrationLogEnabled()
	{
		OMD_NewDamageConfigData data;
		data = Get();
		if (!data)
		{
			return false;
		}

		return data.hitCalibrationLogEnabled;
	}

};
