class OMD_NewDamageWeaponsConfigData
{
	bool activeEnabled = true;
	bool debugEnabled = false;
	ref map<string, float> brainWeaponMul;
	float brainWeightBaseKg = 1.0;
	float brainWeightPower = 0.5;
	float brainWeightMulMin = 0.5;
	float brainWeightMulMax = 2.5;
};

class OMD_NewDamageWeaponsConfig
{
	protected static const string CONFIG_DIR = "$profile:omd";
	protected static const string CONFIG_PATH = "$profile:omd/OMD_NewDamage_Weapons.json";
	protected static ref OMD_NewDamageWeaponsConfigData s_Data;


	static OMD_NewDamageWeaponsConfigData Get()
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
			s_Data = new OMD_NewDamageWeaponsConfigData();
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

		OMD_NewDamageWeaponsConfigData loaded;
		loaded = new OMD_NewDamageWeaponsConfigData();
		JsonFileLoader<OMD_NewDamageWeaponsConfigData>.JsonLoadFile(CONFIG_PATH, loaded);
		s_Data = loaded;
		EnsureDefaults();
		LogLoaded();
	}


	protected static void EnsureDefaults()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageWeaponsConfigData();
		}

		bool filledNew;
		filledNew = false;
		if (!s_Data.brainWeaponMul)
		{
			s_Data.brainWeaponMul = new map<string, float>;
			filledNew = true;
		}

		if (EnsureMulKey("all", 1.0))
		{
			filledNew = true;
		}

		if (EnsureMulKey("blunt", 2.8))
		{
			filledNew = true;
		}

		if (EnsureMulKey("sharp", 0.5))
		{
			filledNew = true;
		}

		if (EnsureMulKey("fist", 0.5))
		{
			filledNew = true;
		}

		if (EnsureMulKey("infected", 1.0))
		{
			filledNew = true;
		}

		if (EnsureMulKey("animal", 1.0))
		{
			filledNew = true;
		}

		if (EnsureScalarKeys())
		{
			filledNew = true;
		}

		if (filledNew)
		{
			Save();
		}
	}


	protected static bool EnsureMulKey(string key, float value)
	{
		if (!s_Data)
		{
			return false;
		}

		if (!s_Data.brainWeaponMul)
		{
			return false;
		}

		if (s_Data.brainWeaponMul.Contains(key))
		{
			return false;
		}

		s_Data.brainWeaponMul.Set(key, value);
		return true;
	}


	protected static bool EnsureScalarKeys()
	{
		if (!s_Data)
		{
			return false;
		}

		if (!FileExist(CONFIG_PATH))
		{
			return false;
		}

		FileHandle handle;
		handle = OpenFile(CONFIG_PATH, FileMode.READ);
		if (!handle)
		{
			return false;
		}

		string line;
		bool foundActive;
		foundActive = false;
		bool foundBase;
		foundBase = false;
		bool foundPower;
		foundPower = false;
		bool foundMin;
		foundMin = false;
		bool foundMax;
		foundMax = false;
		while (FGets(handle, line) >= 0)
		{
			if (line.Contains("activeEnabled"))
			{
				foundActive = true;
			}

			if (line.Contains("brainWeightBaseKg"))
			{
				foundBase = true;
			}

			if (line.Contains("brainWeightPower"))
			{
				foundPower = true;
			}

			if (line.Contains("brainWeightMulMin"))
			{
				foundMin = true;
			}

			if (line.Contains("brainWeightMulMax"))
			{
				foundMax = true;
			}
		}

		CloseFile(handle);

		bool filled;
		filled = false;
		if (!foundActive)
		{
			s_Data.activeEnabled = true;
			filled = true;
		}

		if (!foundBase)
		{
			s_Data.brainWeightBaseKg = 1.0;
			filled = true;
		}

		if (!foundPower)
		{
			s_Data.brainWeightPower = 0.5;
			filled = true;
		}

		if (!foundMin)
		{
			s_Data.brainWeightMulMin = 0.5;
			filled = true;
		}

		if (!foundMax)
		{
			s_Data.brainWeightMulMax = 2.5;
			filled = true;
		}

		return filled;
	}


	static void Save()
	{
		if (!s_Data)
		{
			s_Data = new OMD_NewDamageWeaponsConfigData();
		}

		if (!FileExist(CONFIG_DIR))
		{
			MakeDirectory(CONFIG_DIR);
		}

		JsonFileLoader<OMD_NewDamageWeaponsConfigData>.JsonSaveFile(CONFIG_PATH, s_Data);
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

		Print("[OMD][DMG] weapons config loaded");
	}


	static void SeedDefaults()
	{
		s_Data = new OMD_NewDamageWeaponsConfigData();
		s_Data.activeEnabled = true;
		s_Data.debugEnabled = false;
		s_Data.brainWeaponMul = new map<string, float>;
		s_Data.brainWeaponMul.Set("all", 1.0);
		s_Data.brainWeaponMul.Set("blunt", 2.8);
		s_Data.brainWeaponMul.Set("sharp", 0.5);
		s_Data.brainWeaponMul.Set("fist", 0.5);
		s_Data.brainWeaponMul.Set("infected", 1.0);
		s_Data.brainWeaponMul.Set("animal", 1.0);
		s_Data.brainWeightBaseKg = 1.0;
		s_Data.brainWeightPower = 0.5;
		s_Data.brainWeightMulMin = 0.5;
		s_Data.brainWeightMulMax = 2.5;
	}

};
