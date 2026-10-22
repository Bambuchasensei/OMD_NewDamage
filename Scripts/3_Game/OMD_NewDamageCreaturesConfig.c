class OMD_NewDamageCreaturesConfigData
{
	bool activeEnabled = true;
	bool debugEnabled = false;
	ref array<string> GroupOrder;
	ref map<string, ref array<string>> Groups;
	ref map<string, ref map<string, float>> OrganChancePercent;
	ref map<string, float> OrganHitDamage;
	ref map<string, float> OrganDamageMultiplier;
	ref map<string, ref array<string>> OrganZones;
	ref map<string, float> FaceHitChancePercent;
	ref map<string, float> FaceHitDamage;
	ref map<string, float> ArteryHitChancePercent;
};

class OMD_NewDamageCreaturesConfig
{
	protected static const string CONFIG_DIR = "$profile:omd";
	protected static const string CONFIG_PATH = "$profile:omd/OMD_NewDamage_Creatures.json";
	protected static ref OMD_NewDamageCreaturesConfigData s_Data;


	static OMD_NewDamageCreaturesConfigData Get()
	{
		if (!s_Data)
		{
			Load();
		}

		return s_Data;
	}


	static bool IsActive()
	{
		OMD_NewDamageCreaturesConfigData data;
		data = Get();
		if (!data)
		{
			return false;
		}

		return data.activeEnabled;
	}


	static void Load()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageCreaturesConfigData();
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

		OMD_NewDamageCreaturesConfigData loaded;
		loaded = new OMD_NewDamageCreaturesConfigData();
		JsonFileLoader<OMD_NewDamageCreaturesConfigData>.JsonLoadFile(CONFIG_PATH, loaded);
		s_Data = loaded;
		EnsureMaps();
		LogLoaded();
	}


	protected static void EnsureMaps()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageCreaturesConfigData();
		}

		bool anyPresent;
		anyPresent = false;
		if (s_Data.Groups)
		{
			anyPresent = true;
		}

		if (s_Data.OrganChancePercent)
		{
			anyPresent = true;
		}

		if (s_Data.OrganHitDamage)
		{
			anyPresent = true;
		}

		if (s_Data.OrganDamageMultiplier)
		{
			anyPresent = true;
		}

		if (s_Data.OrganZones)
		{
			anyPresent = true;
		}

		if (s_Data.GroupOrder)
		{
			anyPresent = true;
		}

		if (!anyPresent)
		{
			SeedDefaults();
			Save();
			return;
		}

		if (!s_Data.GroupOrder)
		{
			s_Data.GroupOrder = new array<string>;
		}

		if (!s_Data.Groups)
		{
			s_Data.Groups = new map<string, ref array<string>>;
		}

		bool filledNew;
		filledNew = false;
		if (!s_Data.OrganChancePercent)
		{
			s_Data.OrganChancePercent = new map<string, ref map<string, float>>;
			FillDefaultOrganChances();
			filledNew = true;
		}

		if (!s_Data.OrganHitDamage)
		{
			s_Data.OrganHitDamage = new map<string, float>;
			FillDefaultHitDamage();
			filledNew = true;
		}

		if (!s_Data.FaceHitChancePercent)
		{
			s_Data.FaceHitChancePercent = new map<string, float>;
			FillDefaultFaceChances();
			filledNew = true;
		}

		if (!s_Data.FaceHitDamage)
		{
			s_Data.FaceHitDamage = new map<string, float>;
			FillDefaultFaceDamage();
			filledNew = true;
		}

		if (!s_Data.ArteryHitChancePercent)
		{
			s_Data.ArteryHitChancePercent = new map<string, float>;
			FillDefaultArteryChances();
			filledNew = true;
		}

		if (filledNew)
		{
			Save();
		}

		if (!s_Data.OrganDamageMultiplier)
		{
			s_Data.OrganDamageMultiplier = new map<string, float>;
		}

		if (!s_Data.OrganZones)
		{
			s_Data.OrganZones = new map<string, ref array<string>>;
		}
	}


	static void Save()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageCreaturesConfigData();
		}

		if (!FileExist(CONFIG_DIR))
		{
			MakeDirectory(CONFIG_DIR);
		}

		JsonFileLoader<OMD_NewDamageCreaturesConfigData>.JsonSaveFile(CONFIG_PATH, s_Data);
	}


	protected static void LogLoaded()
	{
		if (!s_Data)
		{
			return;
		}

		if (!OMD_NewDamageConfig.IsDebugEnabled())
		{
			return;
		}

		Print("[OMD][DMG] creatures config loaded");
	}


	static void SeedDefaults()
	{
		s_Data = new OMD_NewDamageCreaturesConfigData();
		s_Data.activeEnabled = true;
		s_Data.debugEnabled = false;
		s_Data.GroupOrder = new array<string>;
		s_Data.Groups = new map<string, ref array<string>>;
		s_Data.OrganChancePercent = new map<string, ref map<string, float>>;
		s_Data.OrganHitDamage = new map<string, float>;
		s_Data.OrganDamageMultiplier = new map<string, float>;
		s_Data.OrganZones = new map<string, ref array<string>>;
		s_Data.FaceHitChancePercent = new map<string, float>;
		s_Data.FaceHitDamage = new map<string, float>;
		s_Data.ArteryHitChancePercent = new map<string, float>;

		BeginGroup("bears");
		AddMember("bears", "Animal_UrsusArctos");

		BeginGroup("wolves");
		AddMember("wolves", "Animal_CanisLupus");

		BeginGroup("dogs");
		AddMember("dogs", "Animal_CanisFamiliaris_Dobermann_ColorBase");
		AddMember("dogs", "Animal_CanisFamiliaris_GermanShepherd_ColorBase");

		BeginGroup("police");
		AddMember("police", "ZmbM_PolicemanFat");
		AddMember("police", "ZmbF_PoliceWomanNormal");
		AddMember("police", "ZmbM_PolicemanSpecForce");
		AddMember("police", "ZmbM_PolicemanSpecForce_Heavy");

		BeginGroup("slow");
		AddMember("slow", "ZmbF_BlueCollarFat_White");
		AddMember("slow", "ZmbM_CitizenBFat_Blue");
		AddMember("slow", "ZmbM_CitizenBFat_Green");
		AddMember("slow", "ZmbM_CitizenBFat_Red");
		AddMember("slow", "ZmbM_FarmerFat_Beige");
		AddMember("slow", "ZmbM_FarmerFat_Blue");
		AddMember("slow", "ZmbM_FarmerFat_Brown");
		AddMember("slow", "ZmbM_FarmerFat_Green");
		AddMember("slow", "ZmbF_VillagerOld_Blue");
		AddMember("slow", "ZmbF_VillagerOld_Green");
		AddMember("slow", "ZmbF_VillagerOld_Red");
		AddMember("slow", "ZmbF_VillagerOld_White");
		AddMember("slow", "ZmbM_VillagerOld_Blue");
		AddMember("slow", "ZmbM_VillagerOld_Green");
		AddMember("slow", "ZmbM_VillagerOld_White");
		AddMember("slow", "ZmbM_PatrolNormal_Winter");

		BeginGroup("medic");
		AddMember("medic", "ZmbM_ParamedicNormal_Blue");
		AddMember("medic", "ZmbM_ParamedicNormal_Green");
		AddMember("medic", "ZmbM_ParamedicNormal_Red");
		AddMember("medic", "ZmbM_ParamedicNormal_Black");
		AddMember("medic", "ZmbF_ParamedicNormal_Blue");
		AddMember("medic", "ZmbF_ParamedicNormal_Green");
		AddMember("medic", "ZmbF_ParamedicNormal_Red");
		AddMember("medic", "ZmbF_DoctorSkinny");
		AddMember("medic", "ZmbM_DoctorFat");
		AddMember("medic", "ZmbF_NurseFat");
		AddMember("medic", "ZmbF_PatientOld");
		AddMember("medic", "ZmbM_PatientSkinny");
		AddMember("medic", "ZmbM_NBC_Yellow");
		AddMember("medic", "ZmbM_NBC_Grey");
		AddMember("medic", "AP_zmb_ecolog_yellow");
		AddMember("medic", "AP_zmb_ecolog_orange");
		AddMember("medic", "AP_zmb_ecolog_blue");
		AddMember("medic", "AP_zmb_ecolog_grey");

		BeginGroup("hunter");
		AddMember("hunter", "ZmbM_HunterOld_Autumn");
		AddMember("hunter", "ZmbM_HunterOld_Spring");
		AddMember("hunter", "ZmbM_HunterOld_Summer");
		AddMember("hunter", "ZmbM_HunterOld_Winter");

		BeginGroup("militaryhard");
		AddMember("militaryhard", "ZmbM_usSoldier_Heavy_Woodland");
		AddMember("militaryhard", "ZmbM_PatrolNormal_Winter");
		AddMember("militaryhard", "AP_zmb_kamikaze");
		AddMember("militaryhard", "AP_zmb_kamikaze_nobomb");
		AddMember("militaryhard", "AP_zmb_kamikaze_half");

		BeginGroup("village");
		AddMember("village", "ZmbF_JoggerSkinny_Blue");
		AddMember("village", "ZmbF_JoggerSkinny_Brown");
		AddMember("village", "ZmbF_JoggerSkinny_Green");
		AddMember("village", "ZmbF_JoggerSkinny_Red");
		AddMember("village", "ZmbF_MilkMaidOld_Beige");
		AddMember("village", "ZmbF_MilkMaidOld_Black");
		AddMember("village", "ZmbF_MilkMaidOld_Green");
		AddMember("village", "ZmbF_MilkMaidOld_Grey");
		AddMember("village", "ZmbF_VillagerOld_Blue");
		AddMember("village", "ZmbF_VillagerOld_Green");
		AddMember("village", "ZmbF_VillagerOld_Red");
		AddMember("village", "ZmbF_VillagerOld_White");
		AddMember("village", "ZmbM_FarmerFat_Beige");
		AddMember("village", "ZmbM_FarmerFat_Blue");
		AddMember("village", "ZmbM_FarmerFat_Brown");
		AddMember("village", "ZmbM_FarmerFat_Green");
		AddMember("village", "ZmbM_Jacket_beige");
		AddMember("village", "ZmbM_Jacket_black");
		AddMember("village", "ZmbM_Jacket_blue");
		AddMember("village", "ZmbM_Jacket_bluechecks");
		AddMember("village", "ZmbM_Jacket_brown");
		AddMember("village", "ZmbM_Jacket_greenchecks");
		AddMember("village", "ZmbM_Jacket_grey");
		AddMember("village", "ZmbM_Jacket_khaki");
		AddMember("village", "ZmbM_Jacket_magenta");
		AddMember("village", "ZmbM_Jacket_stripes");
		AddMember("village", "ZmbM_JoggerSkinny_Blue");
		AddMember("village", "ZmbM_JoggerSkinny_Green");
		AddMember("village", "ZmbM_JoggerSkinny_Red");
		AddMember("village", "ZmbM_VillagerOld_Blue");
		AddMember("village", "ZmbM_VillagerOld_Green");
		AddMember("village", "ZmbM_VillagerOld_White");

		BeginGroup("kamikadze");
		AddMember("kamikadze", "AP_zmb_kamikaze");
		AddMember("kamikadze", "AP_zmb_kamikaze_nobomb");
		AddMember("kamikadze", "AP_zmb_kamikaze_half");

		BeginGroup("demonHelpers");
		AddMember("demonHelpers", "CDAJ_ZmbM_TribeDemon");

		BeginGroup("omon");
		AddMember("omon", "AP_zmb_omon");

		BeginGroup("demon");
		AddMember("demon", "CDAJ_ZmbM_Demon");

		BeginGroup("city");
		AddMember("city", "ZmbF_BlueCollarFat_White");
		AddMember("city", "ZmbF_CitizenANormal_Beige");
		AddMember("city", "ZmbF_CitizenANormal_Blue");
		AddMember("city", "ZmbF_CitizenANormal_Brown");
		AddMember("city", "ZmbF_CitizenBSkinny");
		AddMember("city", "ZmbF_Clerk_Normal_Blue");
		AddMember("city", "ZmbF_Clerk_Normal_Green");
		AddMember("city", "ZmbF_Clerk_Normal_Red");
		AddMember("city", "ZmbF_Clerk_Normal_White");
		AddMember("city", "ZmbF_JournalistNormal_Blue");
		AddMember("city", "ZmbF_JournalistNormal_Green");
		AddMember("city", "ZmbF_JournalistNormal_Red");
		AddMember("city", "ZmbF_JournalistNormal_White");
		AddMember("city", "ZmbF_ShortSkirt_beige");
		AddMember("city", "ZmbF_ShortSkirt_black");
		AddMember("city", "ZmbF_ShortSkirt_brown");
		AddMember("city", "ZmbF_ShortSkirt_checks");
		AddMember("city", "ZmbF_ShortSkirt_green");
		AddMember("city", "ZmbF_ShortSkirt_grey");
		AddMember("city", "ZmbF_ShortSkirt_red");
		AddMember("city", "ZmbF_ShortSkirt_stripes");
		AddMember("city", "ZmbF_ShortSkirt_white");
		AddMember("city", "ZmbF_ShortSkirt_yellow");
		AddMember("city", "ZmbF_SkaterYoung_Brown");
		AddMember("city", "ZmbF_SkaterYoung_Striped");
		AddMember("city", "ZmbF_SkaterYoung_Violet");
		AddMember("city", "ZmbF_SurvivorNormal_Blue");
		AddMember("city", "ZmbF_SurvivorNormal_Orange");
		AddMember("city", "ZmbF_SurvivorNormal_Red");
		AddMember("city", "ZmbF_SurvivorNormal_White");
		AddMember("city", "ZmbM_CitizenASkinny_Blue");
		AddMember("city", "ZmbM_CitizenASkinny_Brown");
		AddMember("city", "ZmbM_CitizenASkinny_Grey");
		AddMember("city", "ZmbM_CitizenASkinny_Red");
		AddMember("city", "ZmbM_CitizenBFat_Blue");
		AddMember("city", "ZmbM_CitizenBFat_Green");
		AddMember("city", "ZmbM_CitizenBFat_Red");
		AddMember("city", "ZmbM_ClerkFat_Brown");
		AddMember("city", "ZmbM_ClerkFat_Grey");
		AddMember("city", "ZmbM_ClerkFat_Khaki");
		AddMember("city", "ZmbM_ClerkFat_White");
		AddMember("city", "ZmbM_CommercialPilotOld_Blue");
		AddMember("city", "ZmbM_CommercialPilotOld_Brown");
		AddMember("city", "ZmbM_CommercialPilotOld_Grey");
		AddMember("city", "ZmbM_CommercialPilotOld_Olive");
		AddMember("city", "ZmbM_Jacket_beige");
		AddMember("city", "ZmbM_Jacket_black");
		AddMember("city", "ZmbM_Jacket_blue");
		AddMember("city", "ZmbM_Jacket_bluechecks");
		AddMember("city", "ZmbM_Jacket_brown");
		AddMember("city", "ZmbM_Jacket_greenchecks");
		AddMember("city", "ZmbM_Jacket_grey");
		AddMember("city", "ZmbM_Jacket_khaki");
		AddMember("city", "ZmbM_Jacket_magenta");
		AddMember("city", "ZmbM_Jacket_stripes");
		AddMember("city", "ZmbM_JournalistSkinny");
		AddMember("city", "ZmbM_MotobikerFat_Beige");
		AddMember("city", "ZmbM_MotobikerFat_Black");
		AddMember("city", "ZmbM_SkaterYoung_Blue");
		AddMember("city", "ZmbM_SkaterYoung_Brown");
		AddMember("city", "ZmbM_SkaterYoung_Green");
		AddMember("city", "ZmbM_SkaterYoung_Grey");

		BeginGroup("militaryconvoy");
		AddMember("militaryconvoy", "ZmbM_usSoldier_Officer_Desert");
		AddMember("militaryconvoy", "ZmbM_usSoldier_Officer_Convoy");

		BeginGroup("fishermen");
		AddMember("fishermen", "ZmbM_FishermanOld_Blue");
		AddMember("fishermen", "ZmbM_FishermanOld_Green");
		AddMember("fishermen", "ZmbM_FishermanOld_Gray");
		AddMember("fishermen", "ZmbM_FishermanOld_Red");

		BeginGroup("fast");
		AddMember("fast", "ZmbF_ShortSkirt_beige");
		AddMember("fast", "ZmbF_ShortSkirt_black");
		AddMember("fast", "ZmbF_ShortSkirt_brown");
		AddMember("fast", "ZmbF_ShortSkirt_checks");
		AddMember("fast", "ZmbF_ShortSkirt_green");
		AddMember("fast", "ZmbF_ShortSkirt_grey");
		AddMember("fast", "ZmbF_ShortSkirt_red");
		AddMember("fast", "ZmbF_ShortSkirt_stripes");
		AddMember("fast", "ZmbF_ShortSkirt_white");
		AddMember("fast", "ZmbF_ShortSkirt_yellow");
		AddMember("fast", "ZmbF_SkaterYoung_Brown");
		AddMember("fast", "ZmbF_SkaterYoung_Striped");
		AddMember("fast", "ZmbF_SkaterYoung_Violet");
		AddMember("fast", "ZmbM_SkaterYoung_Blue");
		AddMember("fast", "ZmbM_SkaterYoung_Brown");
		AddMember("fast", "ZmbM_SkaterYoung_Green");
		AddMember("fast", "ZmbM_SkaterYoung_Grey");
		AddMember("fast", "ZmbM_PrisonerSkinny");

		BeginGroup("military");
		AddMember("military", "ZmbM_usSoldier_normal_Woodland");
		AddMember("military", "ZmbM_usSoldier_normal_Desert");
		AddMember("military", "ZmbM_SoldierNormal");
		AddMember("military", "ZmbM_PatrolNormal_PautRev");
		AddMember("military", "ZmbM_PatrolNormal_Autumn");
		AddMember("military", "ZmbM_PatrolNormal_Flat");
		AddMember("military", "ZmbM_PatrolNormal_Summer");

		BeginGroup("animals");
		AddMember("animals", "AnimalBase");

		FillDefaultOrganChances();
		FillDefaultHitDamage();
		FillDefaultFaceChances();
		FillDefaultFaceDamage();
		FillDefaultArteryChances();

		SetMultiplier("all", 0.3);
		SetMultiplier("military", 0.4);
		SetMultiplier("bears", 1.0);
		SetMultiplier("wolves", 0.6);
		SetMultiplier("dogs", 0.4);
		SetMultiplier("animals", 0.5);
		SetMultiplier("fist", 0.15);
		SetMultiplier("blunt", 0.4);

		SetZones("all", "Torso", "");
		SetZones("bears", "Torso", "Head");
		SetZones("wolves", "Torso", "Head");
		SetZones("dogs", "Torso", "Head");
		SetZones("fist", "Torso", "");
		SetZones("blunt", "Torso", "");
	}


	protected static void BeginGroup(string name)
	{
		array<string> members;
		members = new array<string>;
		s_Data.Groups.Set(name, members);
		s_Data.GroupOrder.Insert(name);
	}


	protected static void AddMember(string name, string classname)
	{
		array<string> members;
		members = s_Data.Groups.Get(name);
		if (!members)
		{
			return;
		}

		members.Insert(classname);
	}


	protected static float StageOneThreshold()
	{
		float threshold;
		threshold = 20;
		OMD_NewDamageConfigData mainCfg;
		mainCfg = OMD_NewDamageConfig.Get();
		if (!mainCfg)
		{
			return threshold;
		}

		if (mainCfg.organLevelThreshold_L1 > 0)
		{
			threshold = mainCfg.organLevelThreshold_L1;
		}

		return threshold;
	}


	protected static void FillDefaultOrganChances()
	{
		SetOrganRow("all", 2, 4, 4, 4, 5, 6);
		SetOrganRow("military", 3, 5, 5, 5, 6, 7);
		SetOrganRow("bears", 6, 10, 10, 10, 12, 15);
		SetOrganRow("wolves", 3, 6, 6, 6, 8, 10);
		SetOrganRow("dogs", 2, 5, 5, 5, 6, 8);
		SetOrganRow("animals", 2, 4, 4, 4, 5, 6);
		SetOrganRow("fist", 0.5, 0.5, 0.5, 0.5, 0.5, 0.5);
		SetOrganRow("blunt", 2, 3, 3, 4, 4, 4);
	}


	protected static void FillDefaultHitDamage()
	{
		float threshold;
		threshold = StageOneThreshold();
		s_Data.OrganHitDamage.Set("all", threshold / 3.0);
		s_Data.OrganHitDamage.Set("bears", threshold / 1.5);
		s_Data.OrganHitDamage.Set("wolves", threshold / 2.5);
		s_Data.OrganHitDamage.Set("fist", threshold / 6.0);
		s_Data.OrganHitDamage.Set("blunt", threshold / 3.0);
	}


	protected static void SetOrganRow(string key, float heart, float lungL, float lungR, float spleen, float liver, float guts)
	{
		map<string, float> row;
		row = new map<string, float>;
		row.Set("heart", heart);
		row.Set("lungL", lungL);
		row.Set("lungR", lungR);
		row.Set("spleen", spleen);
		row.Set("liver", liver);
		row.Set("guts", guts);
		s_Data.OrganChancePercent.Set(key, row);
	}


	protected static void FillDefaultFaceChances()
	{
		s_Data.FaceHitChancePercent.Set("all", 5);
		s_Data.FaceHitChancePercent.Set("military", 6);
		s_Data.FaceHitChancePercent.Set("bears", 25);
		s_Data.FaceHitChancePercent.Set("wolves", 15);
		s_Data.FaceHitChancePercent.Set("dogs", 10);
		s_Data.FaceHitChancePercent.Set("animals", 10);
		s_Data.FaceHitChancePercent.Set("fist", 2);
		s_Data.FaceHitChancePercent.Set("blunt", 8);
	}


	protected static void FillDefaultFaceDamage()
	{
		s_Data.FaceHitDamage.Set("all", 10);
		s_Data.FaceHitDamage.Set("bears", 20);
		s_Data.FaceHitDamage.Set("wolves", 12);
		s_Data.FaceHitDamage.Set("fist", 4);
		s_Data.FaceHitDamage.Set("blunt", 10);
	}


	protected static void FillDefaultArteryChances()
	{
		s_Data.ArteryHitChancePercent.Set("all", 3);
		s_Data.ArteryHitChancePercent.Set("military", 4);
		s_Data.ArteryHitChancePercent.Set("bears", 15);
		s_Data.ArteryHitChancePercent.Set("wolves", 10);
		s_Data.ArteryHitChancePercent.Set("dogs", 8);
		s_Data.ArteryHitChancePercent.Set("animals", 6);
		s_Data.ArteryHitChancePercent.Set("fist", 0);
		s_Data.ArteryHitChancePercent.Set("blunt", 1);
	}


	protected static void SetMultiplier(string key, float value)
	{
		s_Data.OrganDamageMultiplier.Set(key, value);
	}


	protected static void SetZones(string key, string zoneA, string zoneB)
	{
		array<string> zones;
		zones = new array<string>;
		zones.Insert(zoneA);
		if (zoneB != "")
		{
			zones.Insert(zoneB);
		}

		s_Data.OrganZones.Set(key, zones);
	}

};
