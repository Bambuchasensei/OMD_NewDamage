modded class PlayerBase
{

	protected float OMD_NewDamage_ClampOrganDamage(float damage)
	{
		if (damage < 0)
		{
			return 0;
		}

		if (damage > OMD_ND_ORGAN_DAMAGE_MAX)
		{
			return OMD_ND_ORGAN_DAMAGE_MAX;
		}

		return damage;
	}


	protected int OMD_NewDamage_ClampOrganLevel(int level)
	{
		if (level < 0)
		{
			return 0;
		}

		if (level > OMD_ND_ORGAN_LEVEL_MAX)
		{
			return OMD_ND_ORGAN_LEVEL_MAX;
		}

		return level;
	}


	protected int OMD_NewDamage_DamageToLevel(float damage, OMD_NewDamageConfigData cfg)
	{
		float dmg;
		dmg = OMD_NewDamage_ClampOrganDamage(damage);
		if (!cfg)
		{
			cfg = OMD_NewDamageConfig.Get();
		}

		if (!cfg)
		{
			return 0;
		}

		if (dmg >= cfg.organLevelThreshold_L3)
		{
			return 3;
		}

		if (dmg >= cfg.organLevelThreshold_L2)
		{
			return 2;
		}

		if (dmg >= cfg.organLevelThreshold_L1)
		{
			return 1;
		}

		return 0;
	}


	protected float OMD_NewDamage_LevelToThresholdDamage(int level, OMD_NewDamageConfigData cfg)
	{
		if (!cfg)
		{
			cfg = OMD_NewDamageConfig.Get();
		}

		if (!cfg)
		{
			return 0;
		}

		if (level <= 0)
		{
			return 0;
		}

		if (level == 1)
		{
			return cfg.organLevelThreshold_L1;
		}

		if (level == 2)
		{
			return cfg.organLevelThreshold_L2;
		}

		return cfg.organLevelThreshold_L3;
	}


	protected float OMD_NewDamage_GetOrganDamageValue(string organKey)
	{
		if (organKey == "heart")
		{
			return m_OMD_ND_HeartDamage;
		}

		if (organKey == "lung" || organKey == "lungL")
		{
			return m_OMD_ND_LungLDamage;
		}

		if (organKey == "lungR")
		{
			return m_OMD_ND_LungRDamage;
		}

		if (organKey == "liver")
		{
			return m_OMD_ND_LiverDamage;
		}

		if (organKey == "guts")
		{
			return m_OMD_ND_GutsDamage;
		}

		if (organKey == "spleen")
		{
			return m_OMD_ND_SpleenDamage;
		}

		if (organKey == "face")
		{
			return m_OMD_ND_FaceDamage;
		}

		return 0;
	}


	protected void OMD_NewDamage_SetCachedOrganLevel(string organKey, int level)
	{
		int clamped;
		clamped = OMD_NewDamage_ClampOrganLevel(level);
		if (organKey == "heart")
		{
			m_OMD_ND_HeartLevel = clamped;
			return;
		}

		if (organKey == "lung" || organKey == "lungL")
		{
			m_OMD_ND_LungLLevel = clamped;
			return;
		}

		if (organKey == "lungR")
		{
			m_OMD_ND_LungRLevel = clamped;
			return;
		}

		if (organKey == "liver")
		{
			m_OMD_ND_LiverLevel = clamped;
			return;
		}

		if (organKey == "guts")
		{
			m_OMD_ND_GutsLevel = clamped;
			return;
		}

		if (organKey == "spleen")
		{
			m_OMD_ND_SpleenLevel = clamped;
			return;
		}

		if (organKey == "face")
		{
			m_OMD_ND_FaceLevel = clamped;
		}
	}


	protected int OMD_NewDamage_GetCachedOrganLevel(string organKey)
	{
		if (organKey == "heart")
		{
			return m_OMD_ND_HeartLevel;
		}

		if (organKey == "lung" || organKey == "lungL")
		{
			return m_OMD_ND_LungLLevel;
		}

		if (organKey == "lungR")
		{
			return m_OMD_ND_LungRLevel;
		}

		if (organKey == "liver")
		{
			return m_OMD_ND_LiverLevel;
		}

		if (organKey == "guts")
		{
			return m_OMD_ND_GutsLevel;
		}

		if (organKey == "spleen")
		{
			return m_OMD_ND_SpleenLevel;
		}

		if (organKey == "face")
		{
			return m_OMD_ND_FaceLevel;
		}

		return 0;
	}


	protected void OMD_NewDamage_RefreshScaleOrganLevel(string organKey)
	{
		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		float damage;
		damage = OMD_NewDamage_GetOrganDamageValue(organKey);
		int level;
		level = OMD_NewDamage_DamageToLevel(damage, cfg);
		OMD_NewDamage_SetCachedOrganLevel(organKey, level);
	}


	protected void OMD_NewDamage_RefreshAllScaleOrganLevels()
	{
		OMD_NewDamage_RefreshScaleOrganLevel("heart");
		OMD_NewDamage_RefreshScaleOrganLevel("lungL");
		OMD_NewDamage_RefreshScaleOrganLevel("lungR");
		OMD_NewDamage_RefreshScaleOrganLevel("liver");
		OMD_NewDamage_RefreshScaleOrganLevel("guts");
		OMD_NewDamage_RefreshScaleOrganLevel("spleen");
		bool refreshFace;
		refreshFace = false;
		if (GetGame())
		{
			if (GetGame().IsServer())
			{
				refreshFace = true;
			}
		}

		if (refreshFace)
		{
			OMD_NewDamage_RefreshScaleOrganLevel("face");
		}

		m_OMD_ND_BrainLevel = OMD_NewDamage_DamageToLevel(m_OMD_ND_BrainDamage, OMD_NewDamageConfig.Get());
	}


	protected void OMD_NewDamage_MigrateOrganLevelsToDamage()
	{
		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		m_OMD_ND_HeartDamage = OMD_NewDamage_LevelToThresholdDamage(m_OMD_ND_HeartLevel, cfg);
		m_OMD_ND_LungLDamage = OMD_NewDamage_LevelToThresholdDamage(m_OMD_ND_LungLLevel, cfg);
		m_OMD_ND_LungRDamage = OMD_NewDamage_LevelToThresholdDamage(m_OMD_ND_LungRLevel, cfg);
		m_OMD_ND_LiverDamage = OMD_NewDamage_LevelToThresholdDamage(m_OMD_ND_LiverLevel, cfg);
		m_OMD_ND_GutsDamage = OMD_NewDamage_LevelToThresholdDamage(m_OMD_ND_GutsLevel, cfg);
		m_OMD_ND_SpleenDamage = OMD_NewDamage_LevelToThresholdDamage(m_OMD_ND_SpleenLevel, cfg);
		OMD_NewDamage_MigrateFaceLevelToDamage();
		OMD_NewDamage_RefreshAllScaleOrganLevels();
	}


	protected void OMD_NewDamage_MigrateFaceLevelToDamage()
	{
		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		float fromLevel;
		fromLevel = OMD_NewDamage_LevelToThresholdDamage(m_OMD_ND_FaceLevel, cfg);
		if (m_OMD_ND_FaceDamage < fromLevel)
		{
			m_OMD_ND_FaceDamage = fromLevel;
		}
	}


	protected int OMD_NewDamage_PackOrganDamageByte(float damage)
	{
		int packed;
		packed = Math.Round(OMD_NewDamage_ClampOrganDamage(damage));
		if (packed < 0)
		{
			packed = 0;
		}

		if (packed > 100)
		{
			packed = 100;
		}

		return packed;
	}


	protected void OMD_NewDamage_SyncVitalLevelsToNet()
	{
		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		int syncA;
		syncA = 0;
		syncA = syncA + (OMD_NewDamage_PackOrganDamageByte(m_OMD_ND_HeartDamage) * 1);
		syncA = syncA + (OMD_NewDamage_PackOrganDamageByte(m_OMD_ND_LungLDamage) * 128);
		syncA = syncA + (OMD_NewDamage_PackOrganDamageByte(m_OMD_ND_LungRDamage) * 16384);
		syncA = syncA + (OMD_NewDamage_PackOrganDamageByte(m_OMD_ND_LiverDamage) * 2097152);
		m_OMD_ND_OrganDamageSyncA = syncA;

		int syncB;
		syncB = 0;
		syncB = syncB + (OMD_NewDamage_PackOrganDamageByte(m_OMD_ND_GutsDamage) * 1);
		syncB = syncB + (OMD_NewDamage_PackOrganDamageByte(m_OMD_ND_SpleenDamage) * 128);
		syncB = syncB + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_FaceLevel) * 16384);
		m_OMD_ND_OrganDamageSyncB = syncB;

		int arteryPacked;
		arteryPacked = 0;
		arteryPacked = arteryPacked + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtArmsLLevel) * 1);
		arteryPacked = arteryPacked + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtArmsRLevel) * 4);
		arteryPacked = arteryPacked + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtLegsLLevel) * 16);
		arteryPacked = arteryPacked + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtLegsRLevel) * 64);
		arteryPacked = arteryPacked + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtCarotidLevel) * 256);
		arteryPacked = arteryPacked + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtFemoralLevel) * 1024);
		arteryPacked = arteryPacked + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtSubclavianLLevel) * 4096);
		arteryPacked = arteryPacked + (OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtSubclavianRLevel) * 16384);

		int carSide;
		carSide = m_OMD_ND_ArtCarotidSide;
		if (carSide < 0)
		{
			carSide = 0;
		}
		if (carSide > 2)
		{
			carSide = 2;
		}
		arteryPacked = arteryPacked + (carSide * 65536);

		int femSide;
		femSide = m_OMD_ND_ArtFemoralSide;
		if (femSide < 0)
		{
			femSide = 0;
		}
		if (femSide > 2)
		{
			femSide = 2;
		}
		arteryPacked = arteryPacked + (femSide * 262144);
		m_OMD_ND_ArteryLevelsSync = arteryPacked;

		int brainLevelSync;
		brainLevelSync = m_OMD_ND_BrainLevel;
		if (brainLevelSync < 0)
		{
			brainLevelSync = 0;
		}

		if (brainLevelSync > OMD_ND_ORGAN_LEVEL_MAX)
		{
			brainLevelSync = OMD_ND_ORGAN_LEVEL_MAX;
		}

		m_OMD_ND_BrainLevelSync = brainLevelSync;

		SetSynchDirty();
	}

};
