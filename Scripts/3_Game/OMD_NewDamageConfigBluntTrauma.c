class OMD_NewDamageBluntTraumaConfigData
{
	bool bluntTraumaEnabled = true;
	float bluntOrganMinImpact = 10;
	float bluntOrganDmgPerImpact = 0.35;
	float bluntTorsoFractureMul = 1.5;
	float bluntOrganDmgMax = 25;
	ref map<string, ref map<string, float>> organWeights;
};

class OMD_NewDamageBluntTraumaConfig
{
	protected static const string CONFIG_DIR = "$profile:omd";
	protected static const string CONFIG_PATH = "$profile:omd/OMD_NewDamage_BluntTrauma.json";
	protected static ref OMD_NewDamageBluntTraumaConfigData s_Data;


	static OMD_NewDamageBluntTraumaConfigData Get()
	{
		if (!s_Data)
		{
			Load();
		}

		return s_Data;
	}


	static void Load()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageBluntTraumaConfigData();
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

		OMD_NewDamageBluntTraumaConfigData loaded;
		loaded = new OMD_NewDamageBluntTraumaConfigData();
		JsonFileLoader<OMD_NewDamageBluntTraumaConfigData>.JsonLoadFile(CONFIG_PATH, loaded);
		s_Data = loaded;
		EnsureDefaults();
		LogLoaded();
	}


	protected static void EnsureDefaults()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageBluntTraumaConfigData();
		}

		bool filledNew;
		filledNew = false;
		if (!s_Data.organWeights)
		{
			s_Data.organWeights = new map<string, ref map<string, float>>;
			FillDefaultZones();
			filledNew = true;
		}

		if (EnsureZone("chest", "heart", 0.4, "lungL", 0.3, "lungR", 0.3))
		{
			filledNew = true;
		}

		if (EnsureZone("back", "heart", 0.4, "lungL", 0.3, "lungR", 0.3))
		{
			filledNew = true;
		}

		if (EnsureZone("belly", "liver", 0.4, "spleen", 0.3, "guts", 0.3))
		{
			filledNew = true;
		}

		if (EnsureZone("side_l", "lungL", 0.5, "spleen", 0.5, "", 0))
		{
			filledNew = true;
		}

		if (EnsureZone("side_r", "lungR", 0.5, "liver", 0.5, "", 0))
		{
			filledNew = true;
		}

		if (filledNew)
		{
			Save();
		}
	}


	protected static bool EnsureZone(string zone, string organA, float weightA, string organB, float weightB, string organC, float weightC)
	{
		if (!s_Data)
		{
			return false;
		}

		if (!s_Data.organWeights)
		{
			return false;
		}

		if (s_Data.organWeights.Contains(zone))
		{
			return false;
		}

		SetZoneWeights(zone, organA, weightA, organB, weightB, organC, weightC);
		return true;
	}


	static void Save()
	{
		if (!s_Data)
		{
			return;
		}

		if (!FileExist(CONFIG_DIR))
		{
			MakeDirectory(CONFIG_DIR);
		}

		JsonFileLoader<OMD_NewDamageBluntTraumaConfigData>.JsonSaveFile(CONFIG_PATH, s_Data);
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

		Print("[OMD][BLUNT] config loaded");
	}


	static void SeedDefaults()
	{
		s_Data = new OMD_NewDamageBluntTraumaConfigData();
		s_Data.bluntTraumaEnabled = true;
		s_Data.bluntOrganMinImpact = 10;
		s_Data.bluntOrganDmgPerImpact = 0.35;
		s_Data.bluntTorsoFractureMul = 1.5;
		s_Data.bluntOrganDmgMax = 25;
		s_Data.organWeights = new map<string, ref map<string, float>>;
		FillDefaultZones();
	}


	protected static void FillDefaultZones()
	{
		if (!s_Data)
		{
			return;
		}

		if (!s_Data.organWeights)
		{
			s_Data.organWeights = new map<string, ref map<string, float>>;
		}

		SetZoneWeights("chest", "heart", 0.4, "lungL", 0.3, "lungR", 0.3);
		SetZoneWeights("back", "heart", 0.4, "lungL", 0.3, "lungR", 0.3);
		SetZoneWeights("belly", "liver", 0.4, "spleen", 0.3, "guts", 0.3);
		SetZoneWeights("side_l", "lungL", 0.5, "spleen", 0.5, "", 0);
		SetZoneWeights("side_r", "lungR", 0.5, "liver", 0.5, "", 0);
	}


	protected static void SetZoneWeights(string zone, string organA, float weightA, string organB, float weightB, string organC, float weightC)
	{
		if (!s_Data)
		{
			return;
		}

		if (!s_Data.organWeights)
		{
			return;
		}

		map<string, float> row;
		row = new map<string, float>;
		if (organA != "")
		{
			row.Set(organA, weightA);
		}

		if (organB != "")
		{
			row.Set(organB, weightB);
		}

		if (organC != "")
		{
			row.Set(organC, weightC);
		}

		s_Data.organWeights.Set(zone, row);
	}

};
