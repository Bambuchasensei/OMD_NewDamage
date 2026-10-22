modded class PlayerBase
{

	void PlayerBase()
	{
		RegisterNetSyncVariableInt("m_OMD_ND_PressableArteryBits", 0, 255);
		RegisterNetSyncVariableInt("m_OMD_ND_PressableDeepBits", 0, 63);
		RegisterNetSyncVariableInt("m_OMD_ND_PressWoundIndex", 0, 14);
		RegisterNetSyncVariableBool("m_OMD_ND_PressOccupied");
		RegisterNetSyncVariableBool("m_OMD_ND_HolderPressModeActive");
		RegisterNetSyncVariableInt("m_OMD_ND_HolderPressModeKind", 0, 2);
		RegisterNetSyncVariableInt("m_OMD_ND_OrganDamageSyncA", 0, OMD_ND_ORGAN_DMG_SYNC_A_MAX);
		RegisterNetSyncVariableInt("m_OMD_ND_OrganDamageSyncB", 0, OMD_ND_ORGAN_DMG_SYNC_B_MAX);
		RegisterNetSyncVariableInt("m_OMD_ND_ArteryLevelsSync", 0, OMD_ND_ARTERY_SYNC_MAX);
		RegisterNetSyncVariableInt("m_OMD_ND_BrainLevelSync", 0, 3);
		m_OMD_ND_CachedSprintAllowed = true;
		m_OMD_ND_CachedRunAllowed = true;
		m_OMD_ND_CachedStaminaDrainMult = 1.0;
		m_OMD_ND_CachedStaminaRegenMult = 1.0;
	}


	override void Init()
	{
		super.Init();
		OMD_NewDamage_ClearAll();
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if (!m_OMD_ND_PressVariantData)
			{
				m_OMD_ND_PressVariantData = new OMD_NewDamage_PressWoundVariantData(this);
			}
		}
	}


	override void EEKilled(Object killer)
	{
		OMD_NewDamage_CleanupPressOnLeave("dead");
		OMD_NewDamage_ClearAll();
		super.EEKilled(killer);
	}


	override void OnDisconnect()
	{
		OMD_NewDamage_CleanupPressOnLeave("disconnect");
		super.OnDisconnect();
	}


	override void EEDelete(EntityAI parent)
	{
		OMD_NewDamage_CleanupPressOnLeave("delete");
		super.EEDelete(parent);
	}


	void OMD_NewDamage_CleanupPressOnLeave(string reason)
	{
		if (GetGame())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.OMD_NewDamage_ScheduledPressModeAnimAttempt);
		}

		if (!GetGame() || !GetGame().IsServer())
		{
			m_OMD_ND_PressModeAnimDesired = false;
			m_OMD_ND_PressModeCallback = null;
			return;
		}

		if (m_OMD_ND_HolderPressModeActive)
		{
			OMD_NewDamage_RequestPressModeExit(reason, "");
		}

		if (m_OMD_ND_PressActive || m_OMD_ND_PressOccupied)
		{
			OMD_NewDamage_EndPressWound(reason);
		}

		OMD_NewDamage_ClearHolderPressMode();
	}


	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		ctx.Write(OMD_ND_STORE_VERSION);
		ctx.Write(m_OMD_ND_HeartDamage);
		ctx.Write(m_OMD_ND_HeartTimer);
		ctx.Write(m_OMD_ND_LungLDamage);
		ctx.Write(m_OMD_ND_LungLTimer);
		ctx.Write(m_OMD_ND_LungRDamage);
		ctx.Write(m_OMD_ND_LungRTimer);
		ctx.Write(m_OMD_ND_LiverDamage);
		ctx.Write(m_OMD_ND_LiverTimer);
		ctx.Write(m_OMD_ND_GutsDamage);
		ctx.Write(m_OMD_ND_GutsTimer);
		ctx.Write(m_OMD_ND_FaceLevel);
		ctx.Write(m_OMD_ND_ArtArmsLLevel);
		ctx.Write(m_OMD_ND_ArtArmsLTimer);
		ctx.Write(m_OMD_ND_ArtArmsRLevel);
		ctx.Write(m_OMD_ND_ArtArmsRTimer);
		ctx.Write(m_OMD_ND_ArtLegsLLevel);
		ctx.Write(m_OMD_ND_ArtLegsLTimer);
		ctx.Write(m_OMD_ND_ArtLegsRLevel);
		ctx.Write(m_OMD_ND_ArtLegsRTimer);
		ctx.Write(m_OMD_ND_ArtCarotidLevel);
		ctx.Write(m_OMD_ND_ArtCarotidSide);
		ctx.Write(m_OMD_ND_ArtCarotidTimer);
		ctx.Write(m_OMD_ND_ArtFemoralLevel);
		ctx.Write(m_OMD_ND_ArtFemoralSide);
		ctx.Write(m_OMD_ND_ArtFemoralTimer);
		ctx.Write(m_OMD_ND_TorsoBullets);
		ctx.Write(m_OMD_ND_ArtSubclavianLLevel);
		ctx.Write(m_OMD_ND_ArtSubclavianLTimer);
		ctx.Write(m_OMD_ND_ArtSubclavianRLevel);
		ctx.Write(m_OMD_ND_ArtSubclavianRTimer);
		ctx.Write(m_OMD_ND_SpleenDamage);
		ctx.Write(m_OMD_ND_SpleenTimer);
		ctx.Write(m_OMD_ND_FaceDamage);
		ctx.Write(m_OMD_ND_BrainDamage);
	}


	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
		{
			return false;
		}

		OMD_NewDamage_ClearPressRuntime();
		OMD_NewDamage_ClearHolderPressMode();

		int storeVersion;
		storeVersion = 0;
		if (!ctx.Read(storeVersion))
		{
			OMD_NewDamage_ClearAll();
			return true;
		}

		m_OMD_ND_BrainDamage = 0;
		m_OMD_ND_BrainLevel = 0;

		if (storeVersion == OMD_ND_STORE_VERSION_V1)
		{
			if (!OMD_NewDamage_LoadStoreV1(ctx))
			{
				OMD_NewDamage_ClearAll();
				return true;
			}

			OMD_NewDamage_ClampLoadedState();
			OMD_NewDamage_InitTimersFromLevels();
			OMD_NewDamage_MigrateOrganLevelsToDamage();
			OMD_NewDamage_RefreshPressableArterySync();
			OMD_NewDamage_SyncVitalLevelsToNet();
			return true;
		}

		if (storeVersion == OMD_ND_STORE_VERSION_V2)
		{
			if (!OMD_NewDamage_LoadStoreV2(ctx))
			{
				OMD_NewDamage_ClearAll();
				return true;
			}

			OMD_NewDamage_ClampLoadedState();
			OMD_NewDamage_InitMigratedLimbArteryTimers();
			OMD_NewDamage_MigrateOrganLevelsToDamage();
			OMD_NewDamage_RefreshPressableArterySync();
			OMD_NewDamage_SyncVitalLevelsToNet();
			return true;
		}

		if (storeVersion == OMD_ND_STORE_VERSION_V3)
		{
			if (!OMD_NewDamage_LoadStoreV3(ctx))
			{
				OMD_NewDamage_ClearAll();
				return true;
			}

			m_OMD_ND_ArtSubclavianLLevel = 0;
			m_OMD_ND_ArtSubclavianLTimer = 0;
			m_OMD_ND_ArtSubclavianRLevel = 0;
			m_OMD_ND_ArtSubclavianRTimer = 0;
			m_OMD_ND_SpleenLevel = 0;
			m_OMD_ND_SpleenTimer = 0;
			OMD_NewDamage_ClampLoadedState();
			OMD_NewDamage_MigrateOrganLevelsToDamage();
			OMD_NewDamage_RefreshPressableArterySync();
			OMD_NewDamage_SyncVitalLevelsToNet();
			return true;
		}

		if (storeVersion == OMD_ND_STORE_VERSION_V4)
		{
			if (!OMD_NewDamage_LoadStoreV4(ctx))
			{
				OMD_NewDamage_ClearAll();
				return true;
			}

			m_OMD_ND_SpleenLevel = 0;
			m_OMD_ND_SpleenTimer = 0;
			OMD_NewDamage_ClampLoadedState();
			OMD_NewDamage_MigrateOrganLevelsToDamage();
			OMD_NewDamage_RefreshPressableArterySync();
			OMD_NewDamage_SyncVitalLevelsToNet();
			return true;
		}

		if (storeVersion == OMD_ND_STORE_VERSION_V5)
		{
			if (!OMD_NewDamage_LoadStoreV5(ctx))
			{
				OMD_NewDamage_ClearAll();
				return true;
			}

			OMD_NewDamage_ClampLoadedState();
			OMD_NewDamage_MigrateOrganLevelsToDamage();
			OMD_NewDamage_RefreshPressableArterySync();
			OMD_NewDamage_SyncVitalLevelsToNet();
			return true;
		}

		if (storeVersion == OMD_ND_STORE_VERSION_V6)
		{
			if (!OMD_NewDamage_LoadStoreV6(ctx))
			{
				OMD_NewDamage_ClearAll();
				return true;
			}

			OMD_NewDamage_MigrateFaceLevelToDamage();
			OMD_NewDamage_ClampLoadedState();
			OMD_NewDamage_RefreshAllScaleOrganLevels();
			OMD_NewDamage_RefreshPressableArterySync();
			OMD_NewDamage_SyncVitalLevelsToNet();
			return true;
		}

		if (storeVersion == OMD_ND_STORE_VERSION_V7)
		{
			if (!OMD_NewDamage_LoadStoreV7(ctx))
			{
				OMD_NewDamage_ClearAll();
				return true;
			}

			m_OMD_ND_BrainDamage = 0;
			OMD_NewDamage_ClampLoadedState();
			OMD_NewDamage_MigrateFaceLevelToDamage();
			OMD_NewDamage_RefreshAllScaleOrganLevels();
			OMD_NewDamage_RefreshPressableArterySync();
			OMD_NewDamage_SyncVitalLevelsToNet();
			return true;
		}

		if (storeVersion != OMD_ND_STORE_VERSION)
		{
			OMD_NewDamage_ClearAll();
			return true;
		}

		if (!OMD_NewDamage_LoadStoreV8(ctx))
		{
			OMD_NewDamage_ClearAll();
			return true;
		}

		OMD_NewDamage_ClampLoadedState();
		OMD_NewDamage_MigrateFaceLevelToDamage();
		OMD_NewDamage_RefreshAllScaleOrganLevels();
		OMD_NewDamage_RefreshPressableArterySync();
		OMD_NewDamage_SyncVitalLevelsToNet();
		return true;
	}


	protected bool OMD_NewDamage_LoadStoreV1(ParamsReadContext ctx)
	{
		int lungLevel;
		lungLevel = 0;
		int lungSide;
		lungSide = OMD_ND_SIDE_NONE;
		int artArmsLevel;
		artArmsLevel = 0;
		int artLegsLevel;
		artLegsLevel = 0;

		if (!ctx.Read(m_OMD_ND_HeartLevel))
		{
			return false;
		}

		if (!ctx.Read(lungLevel))
		{
			return false;
		}

		if (!ctx.Read(lungSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LiverLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_GutsLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_FaceLevel))
		{
			return false;
		}

		if (!ctx.Read(artArmsLevel))
		{
			return false;
		}

		if (!ctx.Read(artLegsLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_TorsoBullets))
		{
			return false;
		}

		m_OMD_ND_LungLLevel = 0;
		m_OMD_ND_LungRLevel = 0;
		m_OMD_ND_HeartTimer = 0;
		m_OMD_ND_LungLTimer = 0;
		m_OMD_ND_LungRTimer = 0;
		m_OMD_ND_LiverTimer = 0;
		m_OMD_ND_GutsTimer = 0;
		m_OMD_ND_ArtCarotidTimer = 0;
		m_OMD_ND_ArtFemoralTimer = 0;
		m_OMD_ND_ArtArmsLLevel = artArmsLevel;
		m_OMD_ND_ArtArmsLTimer = 0;
		m_OMD_ND_ArtArmsRLevel = 0;
		m_OMD_ND_ArtArmsRTimer = 0;
		m_OMD_ND_ArtLegsLLevel = artLegsLevel;
		m_OMD_ND_ArtLegsLTimer = 0;
		m_OMD_ND_ArtLegsRLevel = 0;
		m_OMD_ND_ArtLegsRTimer = 0;
		m_OMD_ND_SpleenLevel = 0;
		m_OMD_ND_SpleenTimer = 0;

		if (lungLevel > 0)
		{
			if (lungSide == OMD_ND_SIDE_RIGHT)
			{
				m_OMD_ND_LungRLevel = lungLevel;
			}
			else
			{
				m_OMD_ND_LungLLevel = lungLevel;
			}
		}

		return true;
	}


	protected bool OMD_NewDamage_LoadStoreV2(ParamsReadContext ctx)
	{
		int artArmsLevel;
		artArmsLevel = 0;
		int artLegsLevel;
		artLegsLevel = 0;

		if (!ctx.Read(m_OMD_ND_HeartLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_HeartTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungLLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungRLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungRTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LiverLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LiverTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_GutsLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_GutsTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_FaceLevel))
		{
			return false;
		}

		if (!ctx.Read(artArmsLevel))
		{
			return false;
		}

		if (!ctx.Read(artLegsLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_TorsoBullets))
		{
			return false;
		}

		m_OMD_ND_ArtArmsLLevel = artArmsLevel;
		m_OMD_ND_ArtArmsLTimer = 0;
		m_OMD_ND_ArtArmsRLevel = 0;
		m_OMD_ND_ArtArmsRTimer = 0;
		m_OMD_ND_ArtLegsLLevel = artLegsLevel;
		m_OMD_ND_ArtLegsLTimer = 0;
		m_OMD_ND_ArtLegsRLevel = 0;
		m_OMD_ND_ArtLegsRTimer = 0;
		m_OMD_ND_SpleenLevel = 0;
		m_OMD_ND_SpleenTimer = 0;
		return true;
	}


	protected bool OMD_NewDamage_LoadStoreV3(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_OMD_ND_HeartLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_HeartTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungLLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungRLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungRTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LiverLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LiverTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_GutsLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_GutsTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_FaceLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtArmsLLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtArmsLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtArmsRLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtArmsRTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtLegsLLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtLegsLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtLegsRLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtLegsRTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_TorsoBullets))
		{
			return false;
		}

		return true;
	}


	protected bool OMD_NewDamage_LoadStoreV4(ParamsReadContext ctx)
	{
		if (!OMD_NewDamage_LoadStoreV3(ctx))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtSubclavianLLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtSubclavianLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtSubclavianRLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtSubclavianRTimer))
		{
			return false;
		}

		return true;
	}


	protected bool OMD_NewDamage_LoadStoreV5(ParamsReadContext ctx)
	{
		if (!OMD_NewDamage_LoadStoreV4(ctx))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_SpleenLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_SpleenTimer))
		{
			return false;
		}

		return true;
	}


	protected bool OMD_NewDamage_LoadStoreV6(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_OMD_ND_HeartDamage))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_HeartTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungLDamage))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungRDamage))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LungRTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LiverDamage))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_LiverTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_GutsDamage))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_GutsTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_FaceLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtArmsLLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtArmsLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtArmsRLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtArmsRTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtLegsLLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtLegsLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtLegsRLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtLegsRTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtCarotidTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralSide))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtFemoralTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_TorsoBullets))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtSubclavianLLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtSubclavianLTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtSubclavianRLevel))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_ArtSubclavianRTimer))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_SpleenDamage))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_SpleenTimer))
		{
			return false;
		}

		return true;
	}


	protected bool OMD_NewDamage_LoadStoreV7(ParamsReadContext ctx)
	{
		if (!OMD_NewDamage_LoadStoreV6(ctx))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_FaceDamage))
		{
			return false;
		}

		return true;
	}


	protected bool OMD_NewDamage_LoadStoreV8(ParamsReadContext ctx)
	{
		if (!OMD_NewDamage_LoadStoreV7(ctx))
		{
			return false;
		}

		if (!ctx.Read(m_OMD_ND_BrainDamage))
		{
			return false;
		}

		return true;
	}


	void OMD_NewDamage_ClearAll()
	{
		m_OMD_ND_HeartLevel = 0;
		m_OMD_ND_HeartTimer = 0;
		m_OMD_ND_HeartDamage = 0;
		m_OMD_ND_LungLLevel = 0;
		m_OMD_ND_LungLTimer = 0;
		m_OMD_ND_LungLDamage = 0;
		m_OMD_ND_LungRLevel = 0;
		m_OMD_ND_LungRTimer = 0;
		m_OMD_ND_LungRDamage = 0;
		m_OMD_ND_LiverLevel = 0;
		m_OMD_ND_LiverTimer = 0;
		m_OMD_ND_LiverDamage = 0;
		m_OMD_ND_GutsLevel = 0;
		m_OMD_ND_GutsTimer = 0;
		m_OMD_ND_GutsDamage = 0;
		m_OMD_ND_SpleenLevel = 0;
		m_OMD_ND_SpleenTimer = 0;
		m_OMD_ND_SpleenDamage = 0;
		m_OMD_ND_FaceDamage = 0;
		OMD_NewDamage_RefreshScaleOrganLevel("face");
		m_OMD_ND_BrainLevel = 0;
		m_OMD_ND_BrainDamage = 0;
		m_OMD_ND_BrainLevelSync = 0;
		m_OMD_ND_ArtArmsLLevel = 0;
		m_OMD_ND_ArtArmsLTimer = 0;
		m_OMD_ND_ArtArmsRLevel = 0;
		m_OMD_ND_ArtArmsRTimer = 0;
		m_OMD_ND_ArtLegsLLevel = 0;
		m_OMD_ND_ArtLegsLTimer = 0;
		m_OMD_ND_ArtLegsRLevel = 0;
		m_OMD_ND_ArtLegsRTimer = 0;
		m_OMD_ND_ArtCarotidLevel = 0;
		m_OMD_ND_ArtCarotidSide = OMD_ND_SIDE_NONE;
		m_OMD_ND_ArtCarotidTimer = 0;
		m_OMD_ND_ArtFemoralLevel = 0;
		m_OMD_ND_ArtFemoralSide = OMD_ND_SIDE_NONE;
		m_OMD_ND_ArtFemoralTimer = 0;
		m_OMD_ND_ArtSubclavianLLevel = 0;
		m_OMD_ND_ArtSubclavianLTimer = 0;
		m_OMD_ND_ArtSubclavianRLevel = 0;
		m_OMD_ND_ArtSubclavianRTimer = 0;
		m_OMD_ND_TorsoBullets = 0;
		m_OMD_ND_VitalTickAccum = 0;
		m_OMD_ND_VitalDebugAccum = 0;
		m_OMD_ND_DebuffTickAccum = 0;
		m_OMD_ND_CachedSprintAllowed = true;
		m_OMD_ND_CachedRunAllowed = true;
		OMD_NewDamage_ApplyRunSpeedLimit();
		m_OMD_ND_CachedAimShake = 1.0;
		m_OMD_ND_CachedStaminaDrainMult = 1.0;
		m_OMD_ND_CachedStaminaRegenMult = 1.0;
		m_OMD_ND_CachedVignette = 0;
		m_OMD_ND_CachedEdgeBlur = 0;
		m_OMD_ND_CachedFaceBlur = 0;
		m_OMD_ND_CachedDarken = 0;
		m_OMD_ND_CachedDropChance = 0;
		OMD_NewDamage_ResetSymptomLogCache();
		OMD_NewDamage_ClearPressRuntime();
		OMD_NewDamage_ClearHolderPressMode();
		OMD_NewDamage_RefreshPressableArterySync();
		OMD_NewDamage_RefreshPressableDeepSync();
		OMD_NewDamage_SyncVitalLevelsToNet();
	}


	void OMD_NewDamage_ClearPressRuntime()
	{
		m_OMD_ND_PressActive = false;
		m_OMD_ND_PressWoundKey = "";
		m_OMD_ND_PressHolderSteamId = "";
		m_OMD_ND_PressReduce = 0;
		m_OMD_ND_PressTickAccum = 0;
		m_OMD_ND_PressExternalHitPending = false;
		m_OMD_ND_PressInterruptDamageSource = "";
		m_OMD_ND_PressInterruptHitDamage = 0;
		m_OMD_ND_PressWoundIndex = OMD_ND_PRESS_WOUND_NONE;
		m_OMD_ND_PressOccupied = false;
		m_OMD_ND_PressHolderPlayer = null;
		if (GetGame() && GetGame().IsServer())
		{
			SetSynchDirty();
		}
	}


	void OMD_NewDamage_ClearHolderPressMode()
	{
		m_OMD_ND_HolderPressModeActive = false;
		m_OMD_ND_HolderPressModeKind = OMD_ND_PRESS_MODE_NONE;
		m_OMD_ND_PressPatientPlayer = null;
		m_OMD_ND_PressExitInputId = "";
		m_OMD_ND_PressExitClientInModeWas = false;
		OMD_NewDamage_ResetPressModeAnimLocalState("clear");
		OMD_NewDamage_StopPressModeAnim();
		if (GetGame() && GetGame().IsServer())
		{
			SetSynchDirty();
		}
	}


	bool OMD_NewDamage_CanOthersAccessInventory()
	{
		if (!IsAlive())
		{
			return true;
		}

		if (IsUnconscious())
		{
			return true;
		}

		if (IsRestrained())
		{
			return true;
		}

		if (!m_OMD_ND_HolderPressModeActive)
		{
			return false;
		}

		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		if (!cfg)
		{
			return true;
		}

		if (cfg.holdPressureAllowInventoryAccess)
		{
			return true;
		}

		return false;
	}


	OMD_NewDamage_PressWoundVariantData OMD_NewDamage_GetPressWoundVariantData()
	{
		if (!m_OMD_ND_PressVariantData)
		{
			if (GetGame().IsClient() || !GetGame().IsMultiplayer())
			{
				m_OMD_ND_PressVariantData = new OMD_NewDamage_PressWoundVariantData(this);
			}
		}

		return m_OMD_ND_PressVariantData;
	}


	bool OMD_NewDamage_IsHolderPressModeActive()
	{
		return m_OMD_ND_HolderPressModeActive;
	}


	string OMD_NewDamage_GetPressExitInputId()
	{
		return m_OMD_ND_PressExitInputId;
	}


	bool OMD_NewDamage_IsPressOccupied()
	{
		return m_OMD_ND_PressOccupied;
	}


	int OMD_NewDamage_GetPressWoundIndex()
	{
		return m_OMD_ND_PressWoundIndex;
	}


	int OMD_NewDamage_GetPressableDeepBits()
	{
		return m_OMD_ND_PressableDeepBits;
	}


	protected void OMD_NewDamage_RefreshPressableArterySync()
	{
		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		int bits;
		bits = 0;

		if (m_OMD_ND_ArtArmsLLevel > 0)
		{
			bits = bits + OMD_ND_PRESS_ART_ARMS_L;
		}

		if (m_OMD_ND_ArtArmsRLevel > 0)
		{
			bits = bits + OMD_ND_PRESS_ART_ARMS_R;
		}

		if (m_OMD_ND_ArtLegsLLevel > 0)
		{
			bits = bits + OMD_ND_PRESS_ART_LEGS_L;
		}

		if (m_OMD_ND_ArtLegsRLevel > 0)
		{
			bits = bits + OMD_ND_PRESS_ART_LEGS_R;
		}

		if (m_OMD_ND_ArtCarotidLevel > 0)
		{
			bits = bits + OMD_ND_PRESS_ART_CAROTID;
		}

		if (m_OMD_ND_ArtFemoralLevel > 0)
		{
			bits = bits + OMD_ND_PRESS_ART_FEMORAL;
		}

		if (m_OMD_ND_ArtSubclavianLLevel > 0)
		{
			bits = bits + OMD_ND_PRESS_ART_SUBCLAVIAN_L;
		}

		if (m_OMD_ND_ArtSubclavianRLevel > 0)
		{
			bits = bits + OMD_ND_PRESS_ART_SUBCLAVIAN_R;
		}

		if (bits == m_OMD_ND_PressableArteryBits)
		{
			return;
		}

		m_OMD_ND_PressableArteryBits = bits;
		SetSynchDirty();
	}


	int OMD_NewDamage_GetPressableArteryBits()
	{
		return m_OMD_ND_PressableArteryBits;
	}


	bool OMD_NewDamage_HasPressableArteryBit(int bit)
	{
		if (bit <= 0)
		{
			return false;
		}

		if (((m_OMD_ND_PressableArteryBits / bit) % 2) == 1)
		{
			return true;
		}

		return false;
	}


	protected void OMD_NewDamage_RefreshPressableDeepSync()
	{
		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		int bits;
		bits = 0;
		InediaInfectedAI_PainManager painMgr;
		painMgr = InediaInfectedAI_GetPainManager();
		if (painMgr)
		{
			if (painMgr.IsLimbDeepWounded("head"))
			{
				bits = bits + OMD_ND_PRESS_DEEP_HEAD;
			}

			if (painMgr.IsLimbDeepWounded("torso"))
			{
				bits = bits + OMD_ND_PRESS_DEEP_TORSO;
			}

			if (painMgr.IsLimbDeepWounded("arms"))
			{
				bits = bits + OMD_ND_PRESS_DEEP_ARMS;
			}

			if (painMgr.IsLimbDeepWounded("hands"))
			{
				bits = bits + OMD_ND_PRESS_DEEP_HANDS;
			}

			if (painMgr.IsLimbDeepWounded("legs"))
			{
				bits = bits + OMD_ND_PRESS_DEEP_LEGS;
			}

			if (painMgr.IsLimbDeepWounded("feet"))
			{
				bits = bits + OMD_ND_PRESS_DEEP_FEET;
			}
		}

		if (bits == m_OMD_ND_PressableDeepBits)
		{
			return;
		}

		m_OMD_ND_PressableDeepBits = bits;
		SetSynchDirty();
	}


	protected bool OMD_NewDamage_HasSyncedDeepBit(int bit)
	{
		if (bit <= 0)
		{
			return false;
		}

		if (((m_OMD_ND_PressableDeepBits / bit) % 2) == 1)
		{
			return true;
		}

		return false;
	}


	protected bool OMD_NewDamage_IsDeepLimbActive(string limb)
	{
		if (limb == "head")
		{
			if (OMD_NewDamage_HasSyncedDeepBit(OMD_ND_PRESS_DEEP_HEAD))
			{
				return true;
			}
		}

		if (limb == "torso")
		{
			if (OMD_NewDamage_HasSyncedDeepBit(OMD_ND_PRESS_DEEP_TORSO))
			{
				return true;
			}
		}

		if (limb == "arms")
		{
			if (OMD_NewDamage_HasSyncedDeepBit(OMD_ND_PRESS_DEEP_ARMS))
			{
				return true;
			}
		}

		if (limb == "hands")
		{
			if (OMD_NewDamage_HasSyncedDeepBit(OMD_ND_PRESS_DEEP_HANDS))
			{
				return true;
			}
		}

		if (limb == "legs")
		{
			if (OMD_NewDamage_HasSyncedDeepBit(OMD_ND_PRESS_DEEP_LEGS))
			{
				return true;
			}
		}

		if (limb == "feet")
		{
			if (OMD_NewDamage_HasSyncedDeepBit(OMD_ND_PRESS_DEEP_FEET))
			{
				return true;
			}
		}

		if (GetGame().IsServer())
		{
			InediaInfectedAI_PainManager painMgr;
			painMgr = InediaInfectedAI_GetPainManager();
			if (painMgr && painMgr.IsLimbDeepWounded(limb))
			{
				return true;
			}
		}

		return false;
	}


	protected bool OMD_NewDamage_HasSyncedPressableArtery(OMD_NewDamageConfigData cfg)
	{
		if (!cfg)
		{
			return false;
		}

		int bits;
		bits = m_OMD_ND_PressableArteryBits;
		if (bits == 0)
		{
			return false;
		}

		if (cfg.holdPressureAppliesToCarotid)
		{
			if (((bits / OMD_ND_PRESS_ART_CAROTID) % 2) == 1)
			{
				return true;
			}
		}

		if (cfg.holdPressureAppliesToSubclavian)
		{
			if (((bits / OMD_ND_PRESS_ART_SUBCLAVIAN_L) % 2) == 1)
			{
				return true;
			}

			if (((bits / OMD_ND_PRESS_ART_SUBCLAVIAN_R) % 2) == 1)
			{
				return true;
			}
		}

		if (cfg.holdPressureAppliesToFemoral)
		{
			if (((bits / OMD_ND_PRESS_ART_FEMORAL) % 2) == 1)
			{
				return true;
			}
		}

		if (cfg.holdPressureAppliesToLimbArtery)
		{
			if (((bits / OMD_ND_PRESS_ART_ARMS_L) % 2) == 1)
			{
				return true;
			}

			if (((bits / OMD_ND_PRESS_ART_ARMS_R) % 2) == 1)
			{
				return true;
			}

			if (((bits / OMD_ND_PRESS_ART_LEGS_L) % 2) == 1)
			{
				return true;
			}

			if (((bits / OMD_ND_PRESS_ART_LEGS_R) % 2) == 1)
			{
				return true;
			}
		}

		return false;
	}


	protected void OMD_NewDamage_ClampLoadedState()
	{
		m_OMD_ND_HeartDamage = OMD_NewDamage_ClampOrganDamage(m_OMD_ND_HeartDamage);
		m_OMD_ND_LungLDamage = OMD_NewDamage_ClampOrganDamage(m_OMD_ND_LungLDamage);
		m_OMD_ND_LungRDamage = OMD_NewDamage_ClampOrganDamage(m_OMD_ND_LungRDamage);
		m_OMD_ND_LiverDamage = OMD_NewDamage_ClampOrganDamage(m_OMD_ND_LiverDamage);
		m_OMD_ND_GutsDamage = OMD_NewDamage_ClampOrganDamage(m_OMD_ND_GutsDamage);
		m_OMD_ND_SpleenDamage = OMD_NewDamage_ClampOrganDamage(m_OMD_ND_SpleenDamage);
		m_OMD_ND_FaceDamage = OMD_NewDamage_ClampOrganDamage(m_OMD_ND_FaceDamage);
		m_OMD_ND_BrainDamage = OMD_NewDamage_ClampOrganDamage(m_OMD_ND_BrainDamage);
		m_OMD_ND_HeartLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_HeartLevel);
		m_OMD_ND_LungLLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_LungLLevel);
		m_OMD_ND_LungRLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_LungRLevel);
		m_OMD_ND_LiverLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_LiverLevel);
		m_OMD_ND_GutsLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_GutsLevel);
		m_OMD_ND_SpleenLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_SpleenLevel);
		m_OMD_ND_FaceLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_FaceLevel);
		m_OMD_ND_BrainLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_BrainLevel);
		m_OMD_ND_ArtArmsLLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtArmsLLevel);
		m_OMD_ND_ArtArmsRLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtArmsRLevel);
		m_OMD_ND_ArtLegsLLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtLegsLLevel);
		m_OMD_ND_ArtLegsRLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtLegsRLevel);
		m_OMD_ND_ArtCarotidLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtCarotidLevel);
		m_OMD_ND_ArtFemoralLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtFemoralLevel);
		m_OMD_ND_ArtSubclavianLLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtSubclavianLLevel);
		m_OMD_ND_ArtSubclavianRLevel = OMD_NewDamage_ClampOrganLevel(m_OMD_ND_ArtSubclavianRLevel);

		if (m_OMD_ND_ArtCarotidSide < OMD_ND_SIDE_NONE || m_OMD_ND_ArtCarotidSide > OMD_ND_SIDE_RIGHT)
		{
			m_OMD_ND_ArtCarotidSide = OMD_ND_SIDE_NONE;
		}

		if (m_OMD_ND_ArtFemoralSide < OMD_ND_SIDE_NONE || m_OMD_ND_ArtFemoralSide > OMD_ND_SIDE_RIGHT)
		{
			m_OMD_ND_ArtFemoralSide = OMD_ND_SIDE_NONE;
		}

		if (m_OMD_ND_TorsoBullets < 0)
		{
			m_OMD_ND_TorsoBullets = 0;
		}

		if (m_OMD_ND_HeartTimer < 0)
		{
			m_OMD_ND_HeartTimer = 0;
		}

		if (m_OMD_ND_LungLTimer < 0)
		{
			m_OMD_ND_LungLTimer = 0;
		}

		if (m_OMD_ND_LungRTimer < 0)
		{
			m_OMD_ND_LungRTimer = 0;
		}

		if (m_OMD_ND_LiverTimer < 0)
		{
			m_OMD_ND_LiverTimer = 0;
		}

		if (m_OMD_ND_GutsTimer < 0)
		{
			m_OMD_ND_GutsTimer = 0;
		}

		if (m_OMD_ND_SpleenTimer < 0)
		{
			m_OMD_ND_SpleenTimer = 0;
		}

		if (m_OMD_ND_ArtArmsLTimer < 0)
		{
			m_OMD_ND_ArtArmsLTimer = 0;
		}

		if (m_OMD_ND_ArtArmsRTimer < 0)
		{
			m_OMD_ND_ArtArmsRTimer = 0;
		}

		if (m_OMD_ND_ArtLegsLTimer < 0)
		{
			m_OMD_ND_ArtLegsLTimer = 0;
		}

		if (m_OMD_ND_ArtLegsRTimer < 0)
		{
			m_OMD_ND_ArtLegsRTimer = 0;
		}

		if (m_OMD_ND_ArtCarotidTimer < 0)
		{
			m_OMD_ND_ArtCarotidTimer = 0;
		}

		if (m_OMD_ND_ArtFemoralTimer < 0)
		{
			m_OMD_ND_ArtFemoralTimer = 0;
		}

		if (m_OMD_ND_ArtSubclavianLTimer < 0)
		{
			m_OMD_ND_ArtSubclavianLTimer = 0;
		}

		if (m_OMD_ND_ArtSubclavianRTimer < 0)
		{
			m_OMD_ND_ArtSubclavianRTimer = 0;
		}
	}


	protected void OMD_NewDamage_InitTimersFromLevels()
	{
		// Drain model: death timers obsolete. Keep store fields at 0 for save compat.
		m_OMD_ND_HeartTimer = 0;
		m_OMD_ND_LungLTimer = 0;
		m_OMD_ND_LungRTimer = 0;
		m_OMD_ND_LiverTimer = 0;
		m_OMD_ND_GutsTimer = 0;
		m_OMD_ND_SpleenTimer = 0;
		m_OMD_ND_ArtArmsLTimer = 0;
		m_OMD_ND_ArtArmsRTimer = 0;
		m_OMD_ND_ArtLegsLTimer = 0;
		m_OMD_ND_ArtLegsRTimer = 0;
		m_OMD_ND_ArtCarotidTimer = 0;
		m_OMD_ND_ArtFemoralTimer = 0;
		m_OMD_ND_ArtSubclavianLTimer = 0;
		m_OMD_ND_ArtSubclavianRTimer = 0;
	}


	protected void OMD_NewDamage_InitMigratedLimbArteryTimers()
	{
		m_OMD_ND_ArtArmsLTimer = 0;
		m_OMD_ND_ArtLegsLTimer = 0;
	}


	bool OMD_NewDamage_HasTourniquetControl(string limbKey)
	{
		return false;
	}


	protected float OMD_NewDamage_GetOrganTimer(string organKey)
	{
		if (organKey == "heart")
		{
			return m_OMD_ND_HeartTimer;
		}

		if (organKey == "lung" || organKey == "lungL")
		{
			return m_OMD_ND_LungLTimer;
		}

		if (organKey == "lungR")
		{
			return m_OMD_ND_LungRTimer;
		}

		if (organKey == "liver")
		{
			return m_OMD_ND_LiverTimer;
		}

		if (organKey == "guts")
		{
			return m_OMD_ND_GutsTimer;
		}

		if (organKey == "spleen")
		{
			return m_OMD_ND_SpleenTimer;
		}

		if (organKey == "armsL")
		{
			return m_OMD_ND_ArtArmsLTimer;
		}

		if (organKey == "armsR")
		{
			return m_OMD_ND_ArtArmsRTimer;
		}

		if (organKey == "legsL")
		{
			return m_OMD_ND_ArtLegsLTimer;
		}

		if (organKey == "legsR")
		{
			return m_OMD_ND_ArtLegsRTimer;
		}

		if (organKey == "carotid")
		{
			return m_OMD_ND_ArtCarotidTimer;
		}

		if (organKey == "femoral")
		{
			return m_OMD_ND_ArtFemoralTimer;
		}

		if (organKey == "subclavianL")
		{
			return m_OMD_ND_ArtSubclavianLTimer;
		}

		if (organKey == "subclavianR")
		{
			return m_OMD_ND_ArtSubclavianRTimer;
		}

		return 0;
	}


	protected float OMD_NewDamage_ResolveUnconsciousMult(float keyMult, OMD_NewDamageConfigData cfg)
	{
		if (!IsUnconscious())
		{
			return 1.0;
		}

		if (!cfg)
		{
			return 0.30;
		}

		if (!cfg.unconsciousSlowdownEnabled)
		{
			return 1.0;
		}

		if (keyMult >= 0)
		{
			return keyMult;
		}

		if (cfg.unconsciousBleedMult < 0)
		{
			return 0.30;
		}

		return cfg.unconsciousBleedMult;
	}


	float OMD_NewDamage_GetUnconsciousInediaDeepMult()
	{
		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		if (!cfg)
		{
			return OMD_NewDamage_ResolveUnconsciousMult(-1, null);
		}

		return OMD_NewDamage_ResolveUnconsciousMult(cfg.unconsciousInediaDeepWoundBleedMult, cfg);
	}


	protected bool OMD_NewDamage_IsArteryOrganKey(string organKey)
	{
		if (organKey == "armsL" || organKey == "armsR")
		{
			return true;
		}

		if (organKey == "legsL" || organKey == "legsR")
		{
			return true;
		}

		if (organKey == "carotid" || organKey == "femoral")
		{
			return true;
		}

		if (organKey == "subclavianL" || organKey == "subclavianR")
		{
			return true;
		}

		return false;
	}


	protected vector OMD_NewDamage_GetHitLocalToBone(vector modelPos, string boneName)
	{
		vector result;
		result = "0 0 0";

		int boneIdx;
		boneIdx = GetBoneIndexByName(boneName);
		if (boneIdx < 0)
		{
			return result;
		}

		vector boneMat[4];
		GetBoneTransformMS(boneIdx, boneMat);

		vector hitMat[4];
		Math3D.MatrixIdentity4(hitMat);
		hitMat[3] = modelPos;

		vector localMat[4];
		Math3D.MatrixInvMultiply4(boneMat, hitMat, localMat);
		result = localMat[3];
		return result;
	}


	protected vector OMD_NewDamage_GetBulletDirLocal(EntityAI source, vector modelPos)
	{
		vector result;
		result = "0 0 0";

		if (!source)
		{
			return result;
		}

		vector shooterLocal;
		shooterLocal = WorldToModel(source.GetPosition());
		result = modelPos - shooterLocal;

		float lenSq;
		lenSq = result.LengthSq();
		if (lenSq <= 0.0001)
		{
			result = "0 0 0";
			return result;
		}

		result.Normalize();
		return result;
	}


	protected string OMD_NewDamage_GetStanceName()
	{
		if (IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE + DayZPlayerConstants.STANCEMASK_RAISEDPRONE))
		{
			return "prone";
		}

		if (IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH + DayZPlayerConstants.STANCEMASK_RAISEDCROUCH))
		{
			return "crouch";
		}

		return "stand";
	}


	protected float OMD_NewDamage_GetGlobalArmorProjectileHealth(EntityAI item)
	{
		float mul;
		mul = -1;

		if (!item)
		{
			return mul;
		}

		string path;
		path = "CfgVehicles " + item.GetType() + " DamageSystem GlobalArmor Projectile Health damage";
		if (!GetGame().ConfigIsExisting(path))
		{
			return mul;
		}

		mul = GetGame().ConfigGetFloat(path);
		return mul;
	}


	protected bool OMD_NewDamage_IsCalibrationZone(string dmgZone)
	{
		if (dmgZone == "Head")
		{
			return true;
		}

		if (dmgZone == "Brain")
		{
			return true;
		}

		if (dmgZone == "Torso")
		{
			return true;
		}

		if (dmgZone == "LeftArm")
		{
			return true;
		}

		if (dmgZone == "RightArm")
		{
			return true;
		}

		return false;
	}


	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			OMD_NewDamage_ClientSymptomTick(deltaTime);
		}

		if (GetGame().IsServer())
		{
			OMD_NewDamage_VitalServerTick(deltaTime);
			OMD_NewDamage_DebuffServerTick(deltaTime);
			OMD_NewDamage_RefreshPressableDeepSync();
			OMD_NewDamage_PressServerTick(deltaTime);
			OMD_NewDamage_HolderPressModeServerTick(deltaTime);
		}
	}


	override bool CanSprint()
	{
		if (!m_OMD_ND_CachedSprintAllowed)
		{
			return false;
		}

		if (!m_OMD_ND_CachedRunAllowed)
		{
			return false;
		}

		return super.CanSprint();
	}


	float OMD_NewDamage_GetAimShakeMult()
	{
		if (m_OMD_ND_CachedAimShake < 1.0)
		{
			return 1.0;
		}

		return m_OMD_ND_CachedAimShake;
	}


	int OMD_NewDamage_GetHudHeartLevel()
	{
		return m_OMD_ND_HeartLevel;
	}


	int OMD_NewDamage_GetHudSpleenLevel()
	{
		return m_OMD_ND_SpleenLevel;
	}


	int OMD_NewDamage_GetHudLungLLevel()
	{
		return m_OMD_ND_LungLLevel;
	}


	int OMD_NewDamage_GetHudLungRLevel()
	{
		return m_OMD_ND_LungRLevel;
	}


	int OMD_NewDamage_GetHudGutsLevel()
	{
		return m_OMD_ND_GutsLevel;
	}


	int OMD_NewDamage_GetHudLiverLevel()
	{
		return m_OMD_ND_LiverLevel;
	}


	int OMD_NewDamage_GetHudFaceLevel()
	{
		return m_OMD_ND_FaceLevel;
	}


	int OMD_NewDamage_GetHudBrainLevel()
	{
		return m_OMD_ND_BrainLevelSync;
	}


	int OMD_NewDamage_GetHudArteryLevelForLimb(string limb)
	{
		string key;
		key = limb;
		key.ToLower();
		if (key == "head")
		{
			return m_OMD_ND_ArtCarotidLevel;
		}

		if (key == "torso")
		{
			int subMax;
			subMax = m_OMD_ND_ArtSubclavianLLevel;
			if (m_OMD_ND_ArtSubclavianRLevel > subMax)
			{
				subMax = m_OMD_ND_ArtSubclavianRLevel;
			}

			return subMax;
		}

		if (key == "arms")
		{
			int armsMax;
			armsMax = m_OMD_ND_ArtArmsLLevel;
			if (m_OMD_ND_ArtArmsRLevel > armsMax)
			{
				armsMax = m_OMD_ND_ArtArmsRLevel;
			}

			return armsMax;
		}

		if (key == "legs")
		{
			int legsMax;
			legsMax = m_OMD_ND_ArtLegsLLevel;
			if (m_OMD_ND_ArtLegsRLevel > legsMax)
			{
				legsMax = m_OMD_ND_ArtLegsRLevel;
			}

			if (m_OMD_ND_ArtFemoralLevel > legsMax)
			{
				legsMax = m_OMD_ND_ArtFemoralLevel;
			}

			return legsMax;
		}

		return 0;
	}


	bool OMD_NewDamage_IsHudLimbTintEnabled()
	{
		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		if (!cfg)
		{
			return true;
		}

		return cfg.hudLimbTintLayerEnabled;
	}


	int OMD_NewDamage_GetHudLimbTintLevel(string limb)
	{
		if (!OMD_NewDamage_IsHudLimbTintEnabled())
		{
			return 0;
		}

		return OMD_NewDamage_GetHudArteryLevelForLimb(limb);
	}


	float OMD_NewDamage_GetCachedDarken()
	{
		return m_OMD_ND_CachedDarken;
	}


	float OMD_NewDamage_GetCachedVignette()
	{
		return m_OMD_ND_CachedVignette;
	}


	float OMD_NewDamage_GetCachedEdgeBlur()
	{
		return m_OMD_ND_CachedEdgeBlur;
	}


	float OMD_NewDamage_GetCachedFaceBlur()
	{
		return m_OMD_ND_CachedFaceBlur;
	}

	protected void OMD_NewDamage_TryArteryDropOnHit()
	{
	}

	protected void OMD_NewDamage_DebuffServerTick(float deltaTime)
	{
	}


	protected void OMD_NewDamage_RebuildDebuffCache(OMD_NewDamageConfigData cfg)
	{
		m_OMD_ND_CachedSprintAllowed = true;
		m_OMD_ND_CachedRunAllowed = true;
		m_OMD_ND_CachedAimShake = 1.0;
		m_OMD_ND_CachedStaminaDrainMult = 1.0;
		m_OMD_ND_CachedStaminaRegenMult = 1.0;
		m_OMD_ND_CachedVignette = 0;
		m_OMD_ND_CachedEdgeBlur = 0;
		m_OMD_ND_CachedFaceBlur = 0;
		m_OMD_ND_CachedDarken = 0;
		m_OMD_ND_CachedDropChance = 0;

		OMD_NewDamageDebuffStageConfig stage;
		if (m_OMD_ND_HeartLevel >= 3)
		{
			OMD_NewDamage_MergeLowerLevelBans(OMD_NewDamage_GetHeartStage(cfg, 2), false);
		}

		stage = OMD_NewDamage_GetHeartStage(cfg, m_OMD_ND_HeartLevel);
		OMD_NewDamage_MergeDebuffStage(stage, false, false);

		int lungLevel;
		lungLevel = m_OMD_ND_LungLLevel;
		if (cfg.lungUseWorstSide)
		{
			if (m_OMD_ND_LungRLevel > lungLevel)
			{
				lungLevel = m_OMD_ND_LungRLevel;
			}
		}

		if (lungLevel >= 3)
		{
			OMD_NewDamage_MergeLowerLevelBans(OMD_NewDamage_GetLungStage(cfg, 2), false);
		}

		stage = OMD_NewDamage_GetLungStage(cfg, lungLevel);
		OMD_NewDamage_MergeDebuffStage(stage, false, false);

		if (m_OMD_ND_FaceLevel >= 3)
		{
			OMD_NewDamage_MergeLowerLevelBans(OMD_NewDamage_GetFaceStage(cfg, 2), false);
		}

		stage = OMD_NewDamage_GetFaceStage(cfg, m_OMD_ND_FaceLevel);
		OMD_NewDamage_MergeDebuffStage(stage, false, false);
		if (m_OMD_ND_FaceLevel == 1)
		{
			m_OMD_ND_CachedFaceBlur = OMD_NewDamage_MaxFloat(m_OMD_ND_CachedFaceBlur, cfg.faceWoundBlurStrength_L1);
		}

		if (m_OMD_ND_FaceLevel == 2)
		{
			m_OMD_ND_CachedFaceBlur = OMD_NewDamage_MaxFloat(m_OMD_ND_CachedFaceBlur, cfg.faceWoundBlurStrength_L2);
		}

		if (m_OMD_ND_FaceLevel >= 3)
		{
			m_OMD_ND_CachedFaceBlur = OMD_NewDamage_MaxFloat(m_OMD_ND_CachedFaceBlur, cfg.faceWoundBlurStrength_L3);
		}

		int artLegs;
		artLegs = OMD_NewDamage_GetWorstLegsArteryLevel();
		if (artLegs >= 3)
		{
			OMD_NewDamage_MergeLowerLevelBans(OMD_NewDamage_GetArteryStage(cfg, 2), true);
		}

		stage = OMD_NewDamage_GetArteryStage(cfg, artLegs);
		OMD_NewDamage_MergeDebuffStage(stage, true, false);

		int artArms;
		artArms = OMD_NewDamage_GetWorstArmsArteryLevel();
		if (artArms >= 3)
		{
			OMD_NewDamage_MergeLowerLevelBans(OMD_NewDamage_GetArteryStage(cfg, 2), false);
		}

		stage = OMD_NewDamage_GetArteryStage(cfg, artArms);
		OMD_NewDamage_MergeDebuffStage(stage, false, true);

		if (m_OMD_ND_HeartLevel == 1)
		{
			m_OMD_ND_CachedDarken = OMD_NewDamage_MaxFloat(m_OMD_ND_CachedDarken, cfg.heartDarkenStrength_L1);
		}

		if (m_OMD_ND_HeartLevel == 2)
		{
			m_OMD_ND_CachedDarken = OMD_NewDamage_MaxFloat(m_OMD_ND_CachedDarken, cfg.heartDarkenStrength_L2);
		}

		if (m_OMD_ND_HeartLevel >= 3)
		{
			m_OMD_ND_CachedDarken = OMD_NewDamage_MaxFloat(m_OMD_ND_CachedDarken, cfg.heartDarkenStrength_L3);
		}

		OMD_NewDamage_LogDebuffCacheSymptoms();
	}


	protected void OMD_NewDamage_MergeLowerLevelBans(OMD_NewDamageDebuffStageConfig lower, bool legsArtery)
	{
		if (!lower)
		{
			return;
		}

		if (!lower.sprintAllowed)
		{
			m_OMD_ND_CachedSprintAllowed = false;
		}

		if (!lower.runAllowed)
		{
			m_OMD_ND_CachedRunAllowed = false;
		}

		if (!legsArtery)
		{
			return;
		}

		if (!lower.legsSprintAllowed)
		{
			m_OMD_ND_CachedSprintAllowed = false;
		}

		if (!lower.legsRunAllowed)
		{
			m_OMD_ND_CachedRunAllowed = false;
		}
	}


	protected void OMD_NewDamage_MergeDebuffStage(OMD_NewDamageDebuffStageConfig stage, bool legsArtery, bool armsArtery)
	{
		if (!stage)
		{
			return;
		}

		if (!stage.sprintAllowed)
		{
			m_OMD_ND_CachedSprintAllowed = false;
		}

		if (!stage.runAllowed)
		{
			m_OMD_ND_CachedRunAllowed = false;
		}

		if (legsArtery)
		{
			if (!stage.legsSprintAllowed)
			{
				m_OMD_ND_CachedSprintAllowed = false;
			}

			if (!stage.legsRunAllowed)
			{
				m_OMD_ND_CachedRunAllowed = false;
			}
		}

		if (stage.aimShake > m_OMD_ND_CachedAimShake)
		{
			m_OMD_ND_CachedAimShake = stage.aimShake;
		}

		if (armsArtery)
		{
			if (stage.armsAimShake > m_OMD_ND_CachedAimShake)
			{
				m_OMD_ND_CachedAimShake = stage.armsAimShake;
			}

			if (stage.dropChancePerHit > m_OMD_ND_CachedDropChance)
			{
				m_OMD_ND_CachedDropChance = stage.dropChancePerHit;
			}
		}

		if (stage.staminaDrainMult > m_OMD_ND_CachedStaminaDrainMult)
		{
			m_OMD_ND_CachedStaminaDrainMult = stage.staminaDrainMult;
		}

		if (stage.staminaRegenMult >= 0)
		{
			if (stage.staminaRegenMult < m_OMD_ND_CachedStaminaRegenMult)
			{
				m_OMD_ND_CachedStaminaRegenMult = stage.staminaRegenMult;
			}
		}

		if (stage.vignetteStrength > m_OMD_ND_CachedVignette)
		{
			m_OMD_ND_CachedVignette = stage.vignetteStrength;
		}

		if (stage.edgeBlurStrength > m_OMD_ND_CachedEdgeBlur)
		{
			m_OMD_ND_CachedEdgeBlur = stage.edgeBlurStrength;
		}
	}


	protected float OMD_NewDamage_MaxFloat(float a, float b)
	{
		if (a > b)
		{
			return a;
		}

		return b;
	}


	protected int OMD_NewDamage_GetWorstLegsArteryLevel()
	{
		int level;
		level = m_OMD_ND_ArtLegsLLevel;
		if (m_OMD_ND_ArtLegsRLevel > level)
		{
			level = m_OMD_ND_ArtLegsRLevel;
		}

		if (m_OMD_ND_ArtFemoralLevel > level)
		{
			level = m_OMD_ND_ArtFemoralLevel;
		}

		return level;
	}


	protected int OMD_NewDamage_GetWorstArmsArteryLevel()
	{
		int level;
		level = m_OMD_ND_ArtArmsLLevel;
		if (m_OMD_ND_ArtArmsRLevel > level)
		{
			level = m_OMD_ND_ArtArmsRLevel;
		}

		return level;
	}


	protected OMD_NewDamageDebuffStageConfig OMD_NewDamage_GetHeartStage(OMD_NewDamageConfigData cfg, int level)
	{
		if (level <= 0 || !cfg)
		{
			return null;
		}

		if (level == 1)
		{
			return cfg.heart_L1;
		}

		if (level == 2)
		{
			return cfg.heart_L2;
		}

		return cfg.heart_L3;
	}


	protected OMD_NewDamageDebuffStageConfig OMD_NewDamage_GetLungStage(OMD_NewDamageConfigData cfg, int level)
	{
		if (level <= 0 || !cfg)
		{
			return null;
		}

		if (level == 1)
		{
			return cfg.lung_L1;
		}

		if (level == 2)
		{
			return cfg.lung_L2;
		}

		return cfg.lung_L3;
	}


	protected OMD_NewDamageDebuffStageConfig OMD_NewDamage_GetFaceStage(OMD_NewDamageConfigData cfg, int level)
	{
		if (level <= 0 || !cfg)
		{
			return null;
		}

		if (level == 1)
		{
			return cfg.face_L1;
		}

		if (level == 2)
		{
			return cfg.face_L2;
		}

		return cfg.face_L3;
	}


	protected OMD_NewDamageDebuffStageConfig OMD_NewDamage_GetArteryStage(OMD_NewDamageConfigData cfg, int level)
	{
		if (level <= 0 || !cfg)
		{
			return null;
		}

		if (level == 1)
		{
			return cfg.artery_L1;
		}

		if (level == 2)
		{
			return cfg.artery_L2;
		}

		return cfg.artery_L3;
	}


	protected void OMD_NewDamage_ClientSymptomTick(float deltaTime)
	{
		if (!IsAlive())
		{
			return;
		}

		if (!GetGame().GetPlayer())
		{
			return;
		}

		if (GetGame().GetPlayer() != this)
		{
			return;
		}

		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		if (!cfg)
		{
			return;
		}

		OMD_NewDamage_RebuildDebuffCache(cfg);
		OMD_NewDamage_ClientUpdateOrganPPE();
		m_OMD_ND_HeartSfxAccum = m_OMD_ND_HeartSfxAccum + deltaTime;
		m_OMD_ND_LungSfxAccum = m_OMD_ND_LungSfxAccum + deltaTime;
		OMD_NewDamage_ClientHeartSfx(cfg);
		OMD_NewDamage_ClientLungSfx(cfg);
	}


	protected void OMD_NewDamage_ClientUpdateOrganPPE()
	{
		PPERequesterBase reqBase;
		reqBase = PPERequesterBank.GetRequester(PPERequesterBank.REQ_OMD_NEWDAMAGE_ORGANS);
		PPERequester_OMD_NewDamageOrgans req;
		req = PPERequester_OMD_NewDamageOrgans.Cast(reqBase);
		if (!req)
		{
			return;
		}

		float vignette;
		vignette = m_OMD_ND_CachedDarken;
		if (m_OMD_ND_CachedVignette > vignette)
		{
			vignette = m_OMD_ND_CachedVignette;
		}

		float blur;
		blur = m_OMD_ND_CachedEdgeBlur;
		if (m_OMD_ND_CachedFaceBlur > blur)
		{
			blur = m_OMD_ND_CachedFaceBlur;
		}

		req.SetOrganEffects(vignette, blur);
	}


	protected void OMD_NewDamage_ClientHeartSfx(OMD_NewDamageConfigData cfg)
	{
		if (m_OMD_ND_HeartLevel <= 0)
		{
			return;
		}

		float interval;
		interval = 2.5;
		float vol;
		vol = cfg.heartHeartbeatSfx_L1;
		string setName;
		setName = cfg.sfxHeartHeartbeat_L1;
		if (m_OMD_ND_HeartLevel == 2)
		{
			interval = 1.8;
			vol = cfg.heartHeartbeatSfx_L2;
			setName = cfg.sfxHeartHeartbeat_L2;
		}

		if (m_OMD_ND_HeartLevel >= 3)
		{
			interval = 1.1;
			vol = cfg.heartHeartbeatSfx_L3;
			setName = cfg.sfxHeartHeartbeat_L3;
		}

		if (m_OMD_ND_HeartSfxAccum < interval)
		{
			return;
		}

		m_OMD_ND_HeartSfxAccum = 0;
		if (setName == "")
		{
			return;
		}

		PlaySoundSet(m_OMD_ND_HeartLoopSound, setName, 0, 0);
		if (m_OMD_ND_HeartLoopSound)
		{
			if (vol > 0)
			{
				m_OMD_ND_HeartLoopSound.SetSoundVolume(vol);
			}
		}
	}


	protected void OMD_NewDamage_ClientLungSfx(OMD_NewDamageConfigData cfg)
	{
		int lungLevel;
		lungLevel = m_OMD_ND_LungLLevel;
		if (cfg.lungUseWorstSide && m_OMD_ND_LungRLevel > lungLevel)
		{
			lungLevel = m_OMD_ND_LungRLevel;
		}

		if (lungLevel <= 0)
		{
			return;
		}

		OMD_NewDamageDebuffStageConfig stage;
		stage = OMD_NewDamage_GetLungStage(cfg, lungLevel);
		if (!stage)
		{
			return;
		}

		if (m_OMD_ND_LungSfxAccum < 3.0)
		{
			return;
		}

		m_OMD_ND_LungSfxAccum = 0;
		string lungSet;
		lungSet = "";
		if (stage.gurgleSfxStrength > 0 && cfg.sfxLungGurgle != "")
		{
			lungSet = cfg.sfxLungGurgle;
		}

		if (lungLevel >= 2 && cfg.sfxLungBreathHeavy != "")
		{
			lungSet = cfg.sfxLungBreathHeavy;
		}

		// Cough is server SymptomManager (SYMPTOM_COUGH); client only plays gurgle/breath loops.
		if (lungSet != "")
		{
			PlaySoundSet(m_OMD_ND_LungLoopSound, lungSet, 0, 0);
		}
	}


	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);
		AddAction(OMD_NewDamage_ActionPressWoundSelf, InputActionMap);
	}


	override void SetActionsRemoteTarget(out TInputActionMap InputActionMap)
	{
		super.SetActionsRemoteTarget(InputActionMap);
		AddAction(OMD_NewDamage_ActionPressWoundTarget, InputActionMap);
	}


	bool OMD_NewDamage_CanPressWound()
	{
		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		if (!cfg)
		{
			return false;
		}

		if (!cfg.activeEnabled)
		{
			return false;
		}

		if (!cfg.holdPressureEnabled)
		{
			return false;
		}

		if (m_OMD_ND_PressOccupied)
		{
			return false;
		}

		array<string> wounds;
		wounds = new array<string>;
		OMD_NewDamage_CollectPressWoundsSorted(this, wounds);
		if (wounds.Count() > 0)
		{
			return true;
		}

		return false;
	}


	string OMD_NewDamage_SelectMostDangerousWound(OMD_NewDamageConfigData cfg)
	{
		array<string> wounds;
		wounds = new array<string>;
		OMD_NewDamage_CollectPressWoundsSorted(this, wounds);
		if (wounds.Count() <= 0)
		{
			return "";
		}

		return wounds.Get(0);
	}


	void OMD_NewDamage_CollectPressWoundsSorted(PlayerBase patient, array<string> outWounds)
	{
		if (!outWounds)
		{
			return;
		}

		outWounds.Clear();
		if (!patient)
		{
			return;
		}

		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		if (!cfg)
		{
			return;
		}

		if (!cfg.activeEnabled || !cfg.holdPressureEnabled)
		{
			return;
		}

		if (patient.OMD_NewDamage_IsPressOccupied())
		{
			return;
		}

		array<string> keys;
		keys = new array<string>;
		array<float> scores;
		scores = new array<float>;

		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "carotid", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "subclavianL", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "subclavianR", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "femoral", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "armsL", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "armsR", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "legsL", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "legsR", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "deep:head", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "deep:torso", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "deep:arms", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "deep:hands", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "deep:legs", keys, scores);
		patient.OMD_NewDamage_AppendPressWoundCandidate(cfg, "deep:feet", keys, scores);

		int count;
		count = keys.Count();
		int i;
		int j;
		int bestIdx;
		string tmpKey;
		float tmpScore;
		for (i = 0; i < count; i++)
		{
			bestIdx = i;
			for (j = i + 1; j < count; j++)
			{
				if (scores.Get(j) > scores.Get(bestIdx))
				{
					bestIdx = j;
				}
			}

			if (bestIdx != i)
			{
				tmpKey = keys.Get(i);
				keys.Set(i, keys.Get(bestIdx));
				keys.Set(bestIdx, tmpKey);
				tmpScore = scores.Get(i);
				scores.Set(i, scores.Get(bestIdx));
				scores.Set(bestIdx, tmpScore);
			}

			outWounds.Insert(keys.Get(i));
		}
	}


	protected void OMD_NewDamage_AppendPressWoundCandidate(OMD_NewDamageConfigData cfg, string woundKey, array<string> keys, array<float> scores)
	{
		if (!cfg || woundKey == "")
		{
			return;
		}

		if (!OMD_NewDamage_IsPressWoundEligible(woundKey, cfg))
		{
			return;
		}

		if (OMD_NewDamage_IsPressWoundBlocked(woundKey, cfg))
		{
			return;
		}

		float score;
		score = OMD_NewDamage_GetPressWoundDangerScore(woundKey, cfg);
		if (score <= 0)
		{
			return;
		}

		keys.Insert(woundKey);
		scores.Insert(score);
	}


	protected bool OMD_NewDamage_IsPressWoundEligible(string woundKey, OMD_NewDamageConfigData cfg)
	{
		if (woundKey == "carotid")
		{
			if (!cfg.holdPressureAppliesToCarotid)
			{
				return false;
			}

			if (GetGame().IsServer())
			{
				return m_OMD_ND_ArtCarotidLevel > 0;
			}

			return OMD_NewDamage_HasPressableArteryBit(OMD_ND_PRESS_ART_CAROTID);
		}

		if (woundKey == "subclavianL")
		{
			if (!cfg.holdPressureAppliesToSubclavian)
			{
				return false;
			}

			if (GetGame().IsServer())
			{
				return m_OMD_ND_ArtSubclavianLLevel > 0;
			}

			return OMD_NewDamage_HasPressableArteryBit(OMD_ND_PRESS_ART_SUBCLAVIAN_L);
		}

		if (woundKey == "subclavianR")
		{
			if (!cfg.holdPressureAppliesToSubclavian)
			{
				return false;
			}

			if (GetGame().IsServer())
			{
				return m_OMD_ND_ArtSubclavianRLevel > 0;
			}

			return OMD_NewDamage_HasPressableArteryBit(OMD_ND_PRESS_ART_SUBCLAVIAN_R);
		}

		if (woundKey == "femoral")
		{
			if (!cfg.holdPressureAppliesToFemoral)
			{
				return false;
			}

			if (GetGame().IsServer())
			{
				return m_OMD_ND_ArtFemoralLevel > 0;
			}

			return OMD_NewDamage_HasPressableArteryBit(OMD_ND_PRESS_ART_FEMORAL);
		}

		if (woundKey == "armsL")
		{
			if (!cfg.holdPressureAppliesToLimbArtery)
			{
				return false;
			}

			if (GetGame().IsServer())
			{
				return m_OMD_ND_ArtArmsLLevel > 0;
			}

			return OMD_NewDamage_HasPressableArteryBit(OMD_ND_PRESS_ART_ARMS_L);
		}

		if (woundKey == "armsR")
		{
			if (!cfg.holdPressureAppliesToLimbArtery)
			{
				return false;
			}

			if (GetGame().IsServer())
			{
				return m_OMD_ND_ArtArmsRLevel > 0;
			}

			return OMD_NewDamage_HasPressableArteryBit(OMD_ND_PRESS_ART_ARMS_R);
		}

		if (woundKey == "legsL")
		{
			if (!cfg.holdPressureAppliesToLimbArtery)
			{
				return false;
			}

			if (GetGame().IsServer())
			{
				return m_OMD_ND_ArtLegsLLevel > 0;
			}

			return OMD_NewDamage_HasPressableArteryBit(OMD_ND_PRESS_ART_LEGS_L);
		}

		if (woundKey == "legsR")
		{
			if (!cfg.holdPressureAppliesToLimbArtery)
			{
				return false;
			}

			if (GetGame().IsServer())
			{
				return m_OMD_ND_ArtLegsRLevel > 0;
			}

			return OMD_NewDamage_HasPressableArteryBit(OMD_ND_PRESS_ART_LEGS_R);
		}

		string deepLimb;
		deepLimb = OMD_NewDamage_DeepLimbFromPressKey(woundKey);
		if (deepLimb != "")
		{
			if (!cfg.holdPressureAppliesToDeepWound)
			{
				return false;
			}

			return OMD_NewDamage_IsDeepLimbActive(deepLimb);
		}

		if (woundKey == "vanilla")
		{
			if (!cfg.holdPressureAppliesToVanillaBleed)
			{
				return false;
			}

			return OMD_NewDamage_HasActiveVanillaBleed();
		}

		return false;
	}


	protected float OMD_NewDamage_GetPressWoundDangerScore(string woundKey, OMD_NewDamageConfigData cfg)
	{
		if (woundKey == "carotid")
		{
			return 1000 + m_OMD_ND_ArtCarotidLevel;
		}

		if (woundKey == "subclavianL")
		{
			return 900 + m_OMD_ND_ArtSubclavianLLevel;
		}

		if (woundKey == "subclavianR")
		{
			return 900 + m_OMD_ND_ArtSubclavianRLevel;
		}

		if (woundKey == "femoral")
		{
			return 800 + m_OMD_ND_ArtFemoralLevel;
		}

		if (woundKey == "armsL")
		{
			return 700 + m_OMD_ND_ArtArmsLLevel;
		}

		if (woundKey == "armsR")
		{
			return 700 + m_OMD_ND_ArtArmsRLevel;
		}

		if (woundKey == "legsL")
		{
			return 700 + m_OMD_ND_ArtLegsLLevel;
		}

		if (woundKey == "legsR")
		{
			return 700 + m_OMD_ND_ArtLegsRLevel;
		}

		string deepLimb;
		deepLimb = OMD_NewDamage_DeepLimbFromPressKey(woundKey);
		if (deepLimb != "")
		{
			float rate;
			rate = 0;
			InediaInfectedAI_PainManager painMgr;
			painMgr = InediaInfectedAI_GetPainManager();
			if (painMgr && GetGame().IsServer())
			{
				rate = painMgr.GetDeepWoundBleedingRateForLimb(deepLimb);
			}

			float baseScore;
			baseScore = 500;
			if (deepLimb == "head")
			{
				baseScore = 560;
			}

			if (deepLimb == "torso")
			{
				baseScore = 550;
			}

			if (deepLimb == "arms")
			{
				baseScore = 540;
			}

			if (deepLimb == "hands")
			{
				baseScore = 530;
			}

			if (deepLimb == "legs")
			{
				baseScore = 520;
			}

			if (deepLimb == "feet")
			{
				baseScore = 510;
			}

			return baseScore + rate;
		}

		return 0;
	}


	string OMD_NewDamage_GetPressWoundLabel(string woundKey)
	{
		if (woundKey == "carotid")
		{
			return "#STR_OMD_ND_PRESS_WOUND_CAROTID";
		}

		if (woundKey == "femoral")
		{
			return "#STR_OMD_ND_PRESS_WOUND_FEMORAL";
		}

		if (woundKey == "subclavianL")
		{
			return "#STR_OMD_ND_PRESS_WOUND_SUBCLAVIAN_L";
		}

		if (woundKey == "subclavianR")
		{
			return "#STR_OMD_ND_PRESS_WOUND_SUBCLAVIAN_R";
		}

		if (woundKey == "armsL")
		{
			return "#STR_OMD_ND_PRESS_WOUND_ARMS_L";
		}

		if (woundKey == "armsR")
		{
			return "#STR_OMD_ND_PRESS_WOUND_ARMS_R";
		}

		if (woundKey == "legsL")
		{
			return "#STR_OMD_ND_PRESS_WOUND_LEGS_L";
		}

		if (woundKey == "legsR")
		{
			return "#STR_OMD_ND_PRESS_WOUND_LEGS_R";
		}

		if (woundKey == "deep:head")
		{
			return "#STR_OMD_ND_PRESS_WOUND_HEAD";
		}

		if (woundKey == "deep:torso")
		{
			return "#STR_OMD_ND_PRESS_WOUND_TORSO";
		}

		if (woundKey == "deep:arms")
		{
			return "#STR_OMD_ND_PRESS_WOUND_ARMS";
		}

		if (woundKey == "deep:hands")
		{
			return "#STR_OMD_ND_PRESS_WOUND_HANDS";
		}

		if (woundKey == "deep:legs")
		{
			return "#STR_OMD_ND_PRESS_WOUND_LEGS";
		}

		if (woundKey == "deep:feet")
		{
			return "#STR_OMD_ND_PRESS_WOUND_FEET";
		}

		return "#STR_OMD_ND_PRESS_WOUND";
	}


	string OMD_NewDamage_PressWoundIndexToKey(int woundIndex)
	{
		if (woundIndex == OMD_ND_PRESS_WOUND_CAROTID)
		{
			return "carotid";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_SUBCLAVIAN_L)
		{
			return "subclavianL";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_SUBCLAVIAN_R)
		{
			return "subclavianR";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_FEMORAL)
		{
			return "femoral";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_ARMS_L)
		{
			return "armsL";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_ARMS_R)
		{
			return "armsR";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_LEGS_L)
		{
			return "legsL";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_LEGS_R)
		{
			return "legsR";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_DEEP_HEAD)
		{
			return "deep:head";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_DEEP_TORSO)
		{
			return "deep:torso";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_DEEP_ARMS)
		{
			return "deep:arms";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_DEEP_HANDS)
		{
			return "deep:hands";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_DEEP_LEGS)
		{
			return "deep:legs";
		}

		if (woundIndex == OMD_ND_PRESS_WOUND_DEEP_FEET)
		{
			return "deep:feet";
		}

		return "";
	}


	protected string OMD_NewDamage_SelectMostDangerousSubclavian(OMD_NewDamageConfigData cfg)
	{
		string bestKey;
		bestKey = "";
		int bestLevel;
		bestLevel = 0;

		if (m_OMD_ND_ArtSubclavianLLevel > bestLevel)
		{
			if (!OMD_NewDamage_IsPressWoundBlocked("subclavianL", cfg))
			{
				bestLevel = m_OMD_ND_ArtSubclavianLLevel;
				bestKey = "subclavianL";
			}
		}

		if (m_OMD_ND_ArtSubclavianRLevel > bestLevel)
		{
			if (!OMD_NewDamage_IsPressWoundBlocked("subclavianR", cfg))
			{
				bestLevel = m_OMD_ND_ArtSubclavianRLevel;
				bestKey = "subclavianR";
			}
		}

		return bestKey;
	}


	protected string OMD_NewDamage_SelectMostDangerousLimbArtery(OMD_NewDamageConfigData cfg)
	{
		string bestKey;
		bestKey = "";
		int bestLevel;
		bestLevel = 0;

		if (m_OMD_ND_ArtArmsLLevel > bestLevel)
		{
			if (!OMD_NewDamage_IsPressWoundBlocked("armsL", cfg))
			{
				bestLevel = m_OMD_ND_ArtArmsLLevel;
				bestKey = "armsL";
			}
		}

		if (m_OMD_ND_ArtArmsRLevel > bestLevel)
		{
			if (!OMD_NewDamage_IsPressWoundBlocked("armsR", cfg))
			{
				bestLevel = m_OMD_ND_ArtArmsRLevel;
				bestKey = "armsR";
			}
		}

		if (m_OMD_ND_ArtLegsLLevel > bestLevel)
		{
			if (!OMD_NewDamage_IsPressWoundBlocked("legsL", cfg))
			{
				bestLevel = m_OMD_ND_ArtLegsLLevel;
				bestKey = "legsL";
			}
		}

		if (m_OMD_ND_ArtLegsRLevel > bestLevel)
		{
			if (!OMD_NewDamage_IsPressWoundBlocked("legsR", cfg))
			{
				bestLevel = m_OMD_ND_ArtLegsRLevel;
				bestKey = "legsR";
			}
		}

		return bestKey;
	}


	protected string OMD_NewDamage_SelectMostDangerousDeepWound(OMD_NewDamageConfigData cfg)
	{
		array<string> wounds;
		wounds = new array<string>;
		OMD_NewDamage_CollectPressWoundsSorted(this, wounds);
		int i;
		for (i = 0; i < wounds.Count(); i++)
		{
			string key;
			key = wounds.Get(i);
			if (OMD_NewDamage_DeepLimbFromPressKey(key) != "")
			{
				return key;
			}
		}

		return "";
	}


	protected bool OMD_NewDamage_HasActiveVanillaBleed()
	{
		if (IsBleeding())
		{
			return true;
		}

		if (GetBleedingSourceCount() > 0)
		{
			return true;
		}

		return false;
	}


	protected string OMD_NewDamage_GetPressWoundBlockReason(string woundKey, OMD_NewDamageConfigData cfg)
	{
		if (!cfg)
		{
			return "no_cfg";
		}

		if (cfg.holdPressureBlockIfTourniquet)
		{
			string tqLimb;
			tqLimb = OMD_NewDamage_TourniquetLimbForPressWound(woundKey);
			if (tqLimb != "" && OMD_NewDamage_HasTourniquetControl(tqLimb))
			{
				return "tourniquet";
			}

			if (woundKey == "deep:hands")
			{
				if (OMD_NewDamage_HasTourniquetControl("arms"))
				{
					return "tourniquet";
				}
			}

			if (woundKey == "deep:feet")
			{
				if (OMD_NewDamage_HasTourniquetControl("legs"))
				{
					return "tourniquet";
				}
			}
		}

		if (cfg.holdPressureBlockIfBandaged)
		{
			string deepLimb;
			deepLimb = OMD_NewDamage_DeepLimbFromPressKey(woundKey);
			if (deepLimb != "")
			{
				InediaInfectedAI_PainManager painMgr;
				painMgr = InediaInfectedAI_GetPainManager();
				if (painMgr && painMgr.IsLimbDeepWoundBandaged(deepLimb))
				{
					return "bandage";
				}
			}
		}

		return "";
	}


	protected bool OMD_NewDamage_IsPressWoundBlocked(string woundKey, OMD_NewDamageConfigData cfg)
	{
		string blockReason;
		blockReason = OMD_NewDamage_GetPressWoundBlockReason(woundKey, cfg);
		if (blockReason != "")
		{
			return true;
		}

		return false;
	}


	protected string OMD_NewDamage_TourniquetLimbForPressWound(string woundKey)
	{
		if (woundKey == "armsL" || woundKey == "armsR" || woundKey == "deep:arms")
		{
			return "arms";
		}

		if (woundKey == "deep:hands")
		{
			return "hands";
		}

		if (woundKey == "legsL" || woundKey == "legsR" || woundKey == "femoral" || woundKey == "deep:legs")
		{
			return "legs";
		}

		if (woundKey == "deep:feet")
		{
			return "feet";
		}

		return "";
	}


	protected string OMD_NewDamage_DeepLimbFromPressKey(string woundKey)
	{
		if (woundKey == "deep:head")
		{
			return "head";
		}

		if (woundKey == "deep:torso")
		{
			return "torso";
		}

		if (woundKey == "deep:arms")
		{
			return "arms";
		}

		if (woundKey == "deep:hands")
		{
			return "hands";
		}

		if (woundKey == "deep:legs")
		{
			return "legs";
		}

		if (woundKey == "deep:feet")
		{
			return "feet";
		}

		return "";
	}


	float OMD_NewDamage_ApplyPressDeepWoundBleedMultiplier(InediaInfectedAI_PainManager painMgr, float totalBleedRate)
	{
		if (!m_OMD_ND_PressActive)
		{
			return totalBleedRate;
		}

		if (!painMgr)
		{
			return totalBleedRate;
		}

		string deepLimb;
		deepLimb = OMD_NewDamage_DeepLimbFromPressKey(m_OMD_ND_PressWoundKey);
		if (deepLimb == "")
		{
			return totalBleedRate;
		}

		float reduce;
		reduce = m_OMD_ND_PressReduce;
		if (reduce <= 0)
		{
			return totalBleedRate;
		}

		if (reduce > 1)
		{
			reduce = 1;
		}

		float limbPart;
		limbPart = OMD_NewDamage_GetDeepLimbBleedContribution(painMgr, deepLimb);
		if (limbPart <= 0)
		{
			return totalBleedRate;
		}

		float result;
		result = totalBleedRate - (limbPart * reduce);
		if (result < 0)
		{
			result = 0;
		}

		return result;
	}


	protected float OMD_NewDamage_GetDeepLimbBleedContribution(InediaInfectedAI_PainManager painMgr, string limb)
	{
		if (!painMgr)
		{
			return 0;
		}

		if (!painMgr.IsLimbDeepWounded(limb))
		{
			return 0;
		}

		float limbBleed;
		limbBleed = 0;
		if (!painMgr.IsLimbDeepWoundBandaged(limb))
		{
			limbBleed = painMgr.GetDeepWoundBleedingRateForLimb(limb);
		}
		else
		{
			if (!painMgr.IsLimbDeepWoundBandageEffective(limb))
			{
				limbBleed = 0.5 * painMgr.GetDeepWoundBleedingRateForLimb(limb);
			}
		}

		if (limbBleed <= 0)
		{
			return 0;
		}

		float bloodScale;
		bloodScale = Math.InverseLerp(PlayerConstants.BLOOD_THRESHOLD_FATAL, PlayerConstants.BLEEDING_LOW_PRESSURE_BLOOD, GetHealth("GlobalHealth", "Blood"));
		bloodScale = Math.Clamp(bloodScale, PlayerConstants.BLEEDING_LOW_PRESSURE_MIN_MOD, 1);
		limbBleed = limbBleed * bloodScale;

		if (GetModifiersManager())
		{
			if (GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING))
			{
				if (painMgr.m_BloodClottingItemClottingMultiplier > 1)
				{
					limbBleed = limbBleed / painMgr.m_BloodClottingItemClottingMultiplier;
				}
			}
		}

		if (m_InediaInfectedAI_ConfigPlayers)
		{
			if (!IsUnconscious())
			{
				limbBleed = limbBleed * m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundConsciousBleedRateMultiplier;
			}
			else
			{
				limbBleed = limbBleed * m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundUnconsciousBleedRateMultiplier;
			}
		}

		float thickMult;
		thickMult = OMD_BridgeSkills.GetThickBloodBleedMultiplier(this);
		limbBleed = limbBleed * thickMult;
		return limbBleed;
	}


	void OMD_NewDamage_SetHolderPressMode(bool active, int modeKind, PlayerBase patient)
	{
		if (!GetGame().IsServer())
		{
			return;
		}

		m_OMD_ND_HolderPressModeActive = active;
		m_OMD_ND_HolderPressModeKind = modeKind;
		m_OMD_ND_PressPatientPlayer = patient;
		m_OMD_ND_PressExitInputId = "";
		if (!active)
		{
			OMD_NewDamage_ResetPressModeAnimLocalState("set_inactive");
			OMD_NewDamage_StopPressModeAnim();
		}
		else
		{
			OMD_NewDamage_ResetPressModeAnimLocalState("set_active");
			m_OMD_ND_PressModeAnimDesired = true;
			OMD_NewDamage_SchedulePressModeAnim();
		}

		SetSynchDirty();
	}


	void OMD_NewDamage_EndPressWound(string reason)
	{
		if (!m_OMD_ND_PressActive && !m_OMD_ND_PressOccupied)
		{
			return;
		}

		string woundKey;
		woundKey = m_OMD_ND_PressWoundKey;
		string hitSource;
		hitSource = m_OMD_ND_PressInterruptDamageSource;
		float hitDamage;
		hitDamage = m_OMD_ND_PressInterruptHitDamage;
		PlayerBase holder;
		holder = m_OMD_ND_PressHolderPlayer;
		string exitInputId;
		exitInputId = "";
		if (holder)
		{
			exitInputId = holder.OMD_NewDamage_GetPressExitInputId();
		}

		OMD_NewDamage_ClearPressRuntime();

		if (holder)
		{
			holder.OMD_NewDamage_SetHolderPressMode(false, OMD_ND_PRESS_MODE_NONE, null);
		}

		if (OMD_NewDamageConfig.IsDebugEnabled())
		{
			string line;
			line = "[OMD_NewDamage][PRESS] mode exit reason=" + reason;
			line = line + " wound=" + woundKey;
			if (exitInputId != "")
			{
				line = line + " input=" + exitInputId;
			}

			if (hitSource != "")
			{
				line = line + " hitSource=" + hitSource;
				line = line + " hitDamage=" + hitDamage.ToString();
			}
			Print(line);
		}
	}


	void OMD_NewDamage_RequestPressModeExit(string reason, string inputId)
	{
		if (!GetGame().IsServer())
		{
			return;
		}

		if (!m_OMD_ND_HolderPressModeActive)
		{
			return;
		}

		m_OMD_ND_PressExitInputId = inputId;
		PlayerBase patient;
		patient = m_OMD_ND_PressPatientPlayer;
		if (!patient)
		{
			patient = this;
		}

		patient.OMD_NewDamage_EndPressWound(reason);
	}


	bool OMD_NewDamage_IsPressActiveForHolder(string holderSteamId)
	{
		if (!m_OMD_ND_PressActive)
		{
			return false;
		}

		if (holderSteamId == "")
		{
			return true;
		}

		return m_OMD_ND_PressHolderSteamId == holderSteamId;
	}


	string OMD_NewDamage_GetPressWoundKey()
	{
		return m_OMD_ND_PressWoundKey;
	}


	string OMD_NewDamage_GetPressInterruptReason(PlayerBase holder, OMD_NewDamageConfigData cfg, bool isSelf)
	{
		if (!IsAlive())
		{
			return "dead";
		}

		if (IsUnconscious())
		{
			return "uncon";
		}

		if (IsRestrainStarted() || IsRestrained() || IsRestrainPrelocked())
		{
			return "restrain";
		}

		if (!cfg)
		{
			return "no_cfg";
		}

		if (cfg.holdPressureInterruptOnDamage)
		{
			if (m_OMD_ND_PressExternalHitPending)
			{
				return "damage";
			}
		}

		if (holder)
		{
			if (!holder.IsAlive())
			{
				return "dead";
			}

			if (holder.IsUnconscious())
			{
				return "uncon";
			}

			if (holder.IsRestrainStarted() || holder.IsRestrained() || holder.IsRestrainPrelocked())
			{
				return "restrain";
			}
		}

		if (!isSelf && holder)
		{
			float maxDist;
			maxDist = cfg.holdPressureMaxDistance;
			if (maxDist < 0.1)
			{
				maxDist = 0.1;
			}

			float dist;
			dist = vector.Distance(holder.GetPosition(), GetPosition());
			if (dist > maxDist)
			{
				return "distance";
			}
		}

		if (m_OMD_ND_PressWoundKey != "")
		{
			string blockReason;
			blockReason = OMD_NewDamage_GetPressWoundBlockReason(m_OMD_ND_PressWoundKey, cfg);
			if (blockReason != "")
			{
				return blockReason;
			}

			if (!OMD_NewDamage_IsPressWoundStillValid(m_OMD_ND_PressWoundKey, cfg))
			{
				return "wound_gone";
			}
		}

		return "";
	}


	bool OMD_NewDamage_ShouldInterruptPress(PlayerBase holder, OMD_NewDamageConfigData cfg, bool isSelf)
	{
		string reason;
		reason = OMD_NewDamage_GetPressInterruptReason(holder, cfg, isSelf);
		if (reason != "")
		{
			return true;
		}

		return false;
	}


	protected bool OMD_NewDamage_IsPressWoundStillValid(string woundKey, OMD_NewDamageConfigData cfg)
	{
		if (woundKey == "carotid")
		{
			return m_OMD_ND_ArtCarotidLevel > 0;
		}

		if (woundKey == "femoral")
		{
			return m_OMD_ND_ArtFemoralLevel > 0;
		}

		if (woundKey == "subclavianL")
		{
			return m_OMD_ND_ArtSubclavianLLevel > 0;
		}

		if (woundKey == "subclavianR")
		{
			return m_OMD_ND_ArtSubclavianRLevel > 0;
		}

		if (woundKey == "armsL")
		{
			return m_OMD_ND_ArtArmsLLevel > 0;
		}

		if (woundKey == "armsR")
		{
			return m_OMD_ND_ArtArmsRLevel > 0;
		}

		if (woundKey == "legsL")
		{
			return m_OMD_ND_ArtLegsLLevel > 0;
		}

		if (woundKey == "legsR")
		{
			return m_OMD_ND_ArtLegsRLevel > 0;
		}

		string deepLimb;
		deepLimb = OMD_NewDamage_DeepLimbFromPressKey(woundKey);
		if (deepLimb != "")
		{
			return OMD_NewDamage_IsDeepLimbActive(deepLimb);
		}

		if (woundKey == "vanilla")
		{
			return OMD_NewDamage_HasActiveVanillaBleed();
		}

		return false;
	}

	protected void OMD_NewDamage_PressServerTick(float deltaTime)
	{
	}

	protected void OMD_NewDamage_HolderPressModeServerTick(float deltaTime)
	{
	}


	protected string OMD_NewDamage_GetPressModeAnimSkipReason()
	{
		if (!m_OMD_ND_HolderPressModeActive)
		{
			return "mode_inactive";
		}

		if (m_OMD_ND_PressModeCallback)
		{
			return "callback_active";
		}

		if (GetCommand_Action())
		{
			return "command_action";
		}

		ActionManagerBase actionMgr;
		actionMgr = GetActionManager();
		if (actionMgr)
		{
			if (actionMgr.GetRunningAction())
			{
				return "running_action";
			}
		}

		return "";
	}


	bool OMD_NewDamage_CanStartPressModeAnim()
	{
		string skipReason;
		skipReason = OMD_NewDamage_GetPressModeAnimSkipReason();
		if (skipReason != "")
		{
			return false;
		}

		return true;
	}


	protected void OMD_NewDamage_PressAnimSkipLog(string reason)
	{
		if (!OMD_NewDamageConfig.IsDebugEnabled())
		{
			return;
		}

		string line;
		line = "[OMD_NewDamage][PRESS] anim skip reason=" + reason;
		if (GetGame() && GetGame().IsClient())
		{
			line = "[OMD_NewDamage][PRESS][CL] anim skip reason=" + reason;
		}

		Print(line);
	}


	protected void OMD_NewDamage_PressAnimStartLog(int cmdId)
	{
		if (!OMD_NewDamageConfig.IsDebugEnabled())
		{
			return;
		}

		string line;
		line = "[OMD_NewDamage][PRESS] anim start cmd=" + cmdId.ToString();
		if (GetGame() && GetGame().IsClient())
		{
			line = "[OMD_NewDamage][PRESS][CL] anim start cmd=" + cmdId.ToString();
		}

		Print(line);
	}


	protected void OMD_NewDamage_ResetPressModeAnimLocalState(string reason)
	{
		m_OMD_ND_PressExitRequestPending = false;
		m_OMD_ND_PressModeAnimDesired = false;
		m_OMD_ND_PressModeAnimRetryCount = 0;
		m_OMD_ND_PressModeAnimEverStarted = false;
		m_OMD_ND_PressHandsItemTracked = null;
		if (GetGame())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.OMD_NewDamage_ScheduledPressModeAnimAttempt);
		}

		if (OMD_NewDamageConfig.IsDebugEnabled())
		{
			if (GetGame() && GetGame().IsClient())
			{
				string line;
				line = "[OMD_NewDamage][PRESS][CL] state reset reason=" + reason;
				Print(line);
			}
		}
	}


	void OMD_NewDamage_SchedulePressModeAnim()
	{
		if (!m_OMD_ND_HolderPressModeActive)
		{
			OMD_NewDamage_PressAnimSkipLog("mode_inactive");
			return;
		}

		if (m_OMD_ND_PressExitRequestPending)
		{
			OMD_NewDamage_PressAnimSkipLog("exit_pending");
			return;
		}

		if (m_OMD_ND_PressModeAnimEverStarted)
		{
			OMD_NewDamage_PressAnimSkipLog("already_started");
			return;
		}

		if (m_OMD_ND_PressModeCallback)
		{
			OMD_NewDamage_PressAnimSkipLog("callback_active");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.OMD_NewDamage_ScheduledPressModeAnimAttempt);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OMD_NewDamage_ScheduledPressModeAnimAttempt, 150, false);
			return;
		}

		m_OMD_ND_PressModeAnimDesired = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.OMD_NewDamage_ScheduledPressModeAnimAttempt);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OMD_NewDamage_ScheduledPressModeAnimAttempt, 150, false);
	}


	void OMD_NewDamage_ScheduledPressModeAnimAttempt()
	{
		if (!m_OMD_ND_HolderPressModeActive)
		{
			OMD_NewDamage_PressAnimSkipLog("mode_inactive");
			return;
		}

		if (m_OMD_ND_PressExitRequestPending)
		{
			OMD_NewDamage_PressAnimSkipLog("exit_pending");
			return;
		}

		if (!m_OMD_ND_PressModeAnimDesired)
		{
			OMD_NewDamage_PressAnimSkipLog("anim_not_desired");
			return;
		}

		if (m_OMD_ND_PressModeAnimEverStarted)
		{
			OMD_NewDamage_PressAnimSkipLog("already_started");
			return;
		}

		if (m_OMD_ND_PressModeCallback)
		{
			OMD_NewDamage_PressAnimSkipLog("callback_active");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.OMD_NewDamage_ScheduledPressModeAnimAttempt);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OMD_NewDamage_ScheduledPressModeAnimAttempt, 150, false);
			return;
		}

		string skipReason;
		skipReason = OMD_NewDamage_GetPressModeAnimSkipReason();
		if (skipReason != "")
		{
			OMD_NewDamage_PressAnimSkipLog(skipReason);
			OMD_NewDamage_OnPressModeAnimStartFailed();
			return;
		}

		OMD_NewDamage_EnsurePressModeAnim();
		if (!m_OMD_ND_PressModeCallback)
		{
			OMD_NewDamage_OnPressModeAnimStartFailed();
		}
	}


	protected void OMD_NewDamage_OnPressModeAnimStartFailed()
	{
		if (m_OMD_ND_PressModeAnimRetryCount < 1)
		{
			m_OMD_ND_PressModeAnimRetryCount = m_OMD_ND_PressModeAnimRetryCount + 1;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.OMD_NewDamage_ScheduledPressModeAnimAttempt);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OMD_NewDamage_ScheduledPressModeAnimAttempt, 150, false);
			return;
		}

		m_OMD_ND_PressModeAnimDesired = false;
		if (GetGame().IsServer())
		{
			OMD_NewDamage_RequestPressModeExit("anim_lost", "");
		}
		else
		{
			if (!m_OMD_ND_PressExitRequestPending)
			{
				m_OMD_ND_PressExitRequestPending = true;
				OMD_NewDamage_SendPressModeExitRPC("anim_lost");
			}
		}
	}


	void OMD_NewDamage_EnsurePressModeAnim()
	{
		if (!m_OMD_ND_HolderPressModeActive && !m_OMD_ND_PressModeAnimDesired)
		{
			return;
		}

		if (!m_OMD_ND_HolderPressModeActive)
		{
			OMD_NewDamage_StopPressModeAnim();
			return;
		}

		if (m_OMD_ND_PressModeAnimEverStarted)
		{
			return;
		}

		string skipReason;
		skipReason = OMD_NewDamage_GetPressModeAnimSkipReason();
		if (skipReason != "")
		{
			OMD_NewDamage_PressAnimSkipLog(skipReason);
			return;
		}

		int cmdId;
		cmdId = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAG;
		if (m_OMD_ND_HolderPressModeKind == OMD_ND_PRESS_MODE_TARGET)
		{
			cmdId = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		}

		int stanceMask;
		stanceMask = DayZPlayerConstants.STANCEMASK_ERECT + DayZPlayerConstants.STANCEMASK_CROUCH;
		OMD_NewDamage_PressModeCB cb;
		cb = OMD_NewDamage_PressModeCB.Cast(StartCommand_Action(cmdId, OMD_NewDamage_PressModeCB, stanceMask));
		if (cb)
		{
			cb.m_IsFullbody = true;
			cb.m_Player = this;
			cb.EnableCancelCondition(true);
			m_OMD_ND_PressModeCallback = cb;
			m_OMD_ND_PressModeAnimDesired = true;
			m_OMD_ND_PressModeAnimEverStarted = true;
			OMD_NewDamage_PressAnimStartLog(cmdId);
		}
		else
		{
			OMD_NewDamage_PressAnimSkipLog("start_command_failed");
		}
	}


	void OMD_NewDamage_StopPressModeAnim()
	{
		m_OMD_ND_PressModeAnimDesired = false;
		OMD_NewDamage_PressModeCB cb;
		cb = m_OMD_ND_PressModeCallback;
		m_OMD_ND_PressModeCallback = null;
		if (cb)
		{
			cb.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
		}
	}


	void OMD_NewDamage_OnPressModeCallbackEnd()
	{
		m_OMD_ND_PressModeCallback = null;
		if (!m_OMD_ND_HolderPressModeActive)
		{
			return;
		}

		if (!m_OMD_ND_PressModeAnimDesired)
		{
			return;
		}

		if (m_OMD_ND_PressModeAnimEverStarted)
		{
			m_OMD_ND_PressModeAnimDesired = false;
			if (GetGame().IsServer())
			{
				OMD_NewDamage_RequestPressModeExit("anim_lost", "");
			}
			else
			{
				if (!m_OMD_ND_PressExitRequestPending)
				{
					m_OMD_ND_PressExitRequestPending = true;
					OMD_NewDamage_SendPressModeExitRPC("anim_lost");
				}
			}

			return;
		}

		OMD_NewDamage_OnPressModeAnimStartFailed();
	}


	protected void OMD_NewDamage_InitPressInterruptInputs()
	{
		if (m_OMD_ND_PressInterruptInputs)
		{
			return;
		}

		// EmoteManager.m_InterruptInputs (emotemanager.c:191-201) + task 0 UA checklist.
		// Real names from P:\bin\constants.xml / preset_x1mousekey.xml (no UAJump / UAStanceCrouch).
		m_OMD_ND_PressInterruptInputs = new array<string>;
		m_OMD_ND_PressInterruptInputs.Insert("UAMoveForward");
		m_OMD_ND_PressInterruptInputs.Insert("UAMoveBack");
		m_OMD_ND_PressInterruptInputs.Insert("UAMoveLeft");
		m_OMD_ND_PressInterruptInputs.Insert("UAMoveRight");
		m_OMD_ND_PressInterruptInputs.Insert("UATurbo");
		m_OMD_ND_PressInterruptInputs.Insert("UAWalkRunTemp");
		m_OMD_ND_PressInterruptInputs.Insert("UAStance");
		m_OMD_ND_PressInterruptInputs.Insert("UAStand");
		m_OMD_ND_PressInterruptInputs.Insert("UACrouch");
		m_OMD_ND_PressInterruptInputs.Insert("UAProne");
		m_OMD_ND_PressInterruptInputs.Insert("UAGetOver");
		m_OMD_ND_PressInterruptInputs.Insert("UATempRaiseWeapon");
		m_OMD_ND_PressInterruptInputs.Insert("UAFire");
		m_OMD_ND_PressInterruptInputs.Insert("UADefaultAction");
		m_OMD_ND_PressInterruptInputs.Insert("UAAction");
		m_OMD_ND_PressInterruptInputs.Insert("UAToggleWeapons");
		m_OMD_ND_PressInterruptInputs.Insert("UAUIGesturesOpen");
	}


	protected string OMD_NewDamage_FindPressInterruptInputPress()
	{
		OMD_NewDamage_InitPressInterruptInputs();
		int i;
		int count;
		count = m_OMD_ND_PressInterruptInputs.Count();
		string inputName;
		UAInput input;
		for (i = 0; i < count; i++)
		{
			inputName = m_OMD_ND_PressInterruptInputs.Get(i);
			input = GetUApi().GetInputByName(inputName);
			if (!input)
			{
				continue;
			}

			if (input.LocalPress())
			{
				return inputName;
			}
		}

		return "";
	}


	protected void OMD_NewDamage_SuppressPressInputByName(string inputName)
	{
		if (inputName == "")
		{
			return;
		}

		UAInput input;
		input = GetUApi().GetInputByName(inputName);
		if (input)
		{
			input.Supress();
		}
	}


	protected void OMD_NewDamage_RequestPressModeInputExit(string inputName)
	{
		if (inputName == "")
		{
			return;
		}

		OMD_NewDamage_SuppressPressInputByName(inputName);
		if (m_OMD_ND_PressExitRequestPending)
		{
			return;
		}

		m_OMD_ND_PressExitRequestPending = true;
		if (OMD_NewDamageConfig.IsDebugEnabled())
		{
			string line;
			line = "[OMD_NewDamage][PRESS][CL] input exit name=" + inputName;
			Print(line);
		}

		OMD_NewDamage_SendPressModeExitRPC(inputName);
	}


	protected void OMD_NewDamage_PressModeClientTick(float deltaTime)
	{
		bool inMode;
		inMode = m_OMD_ND_HolderPressModeActive;

		if (inMode != m_OMD_ND_PressExitClientInModeWas)
		{
			m_OMD_ND_PressExitClientInModeWas = inMode;
			if (!inMode)
			{
				m_OMD_ND_PressExitRequestPending = false;
				m_OMD_ND_PressHandsItemTracked = null;
			}
			else
			{
				m_OMD_ND_PressExitRequestPending = false;
				m_OMD_ND_PressHandsItemTracked = GetItemInHands();
			}

			if (OMD_NewDamageConfig.IsDebugEnabled())
			{
				string modeLine;
				modeLine = "[OMD_NewDamage][PRESS][CL] mode sync inMode=" + inMode.ToString();
				Print(modeLine);
			}
		}

		if (!inMode)
		{
			return;
		}

		string interruptName;
		interruptName = OMD_NewDamage_FindPressInterruptInputPress();
		if (interruptName != "")
		{
			OMD_NewDamage_RequestPressModeInputExit(interruptName);
			return;
		}

		EntityAI handsNow;
		handsNow = GetItemInHands();
		if (handsNow != m_OMD_ND_PressHandsItemTracked)
		{
			m_OMD_ND_PressHandsItemTracked = handsNow;
			OMD_NewDamage_RequestPressModeInputExit("hands_item");
			return;
		}

		if (GetGame().GetUIManager())
		{
			if (GetGame().GetUIManager().IsMenuOpen(MENU_GESTURES))
			{
				OMD_NewDamage_RequestPressModeInputExit("UAUIGesturesOpen");
			}
		}
	}


	protected void OMD_NewDamage_SendPressModeExitRPC(string inputId)
	{
		if (GetGame().IsDedicatedServer())
		{
			return;
		}

		ScriptRPC rpc;
		rpc = new ScriptRPC();
		rpc.Write(inputId);
		rpc.Send(this, OMD_NewDamage_ERPCs.RPC_PRESS_MODE_EXIT, true, null);
	}


	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT)
		{
			if (IsControlledPlayer())
			{
				OMD_NewDamage_PressModeClientTick(pDt);
			}
		}

		super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);
		OMD_NewDamage_ApplyRunSpeedLimit();
	}


	protected void OMD_NewDamage_ApplyRunSpeedLimit()
	{
		HumanInputController hic;
		hic = GetInputController();
		if (!hic)
		{
			return;
		}

		if (!m_OMD_ND_CachedRunAllowed)
		{
			hic.SetErectSpeedLimit(true, DayZPlayerConstants.MOVEMENTIDX_WALK);
			hic.SetCrouchSpeedLimit(true, DayZPlayerConstants.MOVEMENTIDX_WALK);
			m_OMD_ND_RunSpeedLimited = true;
			return;
		}

		if (!m_OMD_ND_RunSpeedLimited)
		{
			return;
		}

		if (hic.GetErectSpeedLimit() == DayZPlayerConstants.MOVEMENTIDX_WALK)
		{
			hic.SetErectSpeedLimit(false);
		}

		if (hic.GetCrouchSpeedLimit() == DayZPlayerConstants.MOVEMENTIDX_WALK)
		{
			hic.SetCrouchSpeedLimit(false);
		}

		m_OMD_ND_RunSpeedLimited = false;
	}


	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (GetGame().IsClient())
		{
			OMD_NewDamage_UnpackVitalLevelsFromNet();
		}

		if (m_OMD_ND_HolderPressModeActive)
		{
			m_OMD_ND_PressExitRequestPending = false;
			m_OMD_ND_PressModeAnimDesired = true;
			if (!m_OMD_ND_PressModeAnimEverStarted)
			{
				OMD_NewDamage_SchedulePressModeAnim();
			}
			else
			{
				OMD_NewDamage_PressAnimSkipLog("already_started");
			}
		}
		else
		{
			OMD_NewDamage_ResetPressModeAnimLocalState("netsync");
			OMD_NewDamage_StopPressModeAnim();
		}
	}


	protected void OMD_NewDamage_UnpackVitalLevelsFromNet()
	{
		int v;
		int syncA;
		syncA = m_OMD_ND_OrganDamageSyncA;

		v = syncA / 1;
		v = v % 128;
		m_OMD_ND_HeartDamage = v;

		v = syncA / 128;
		v = v % 128;
		m_OMD_ND_LungLDamage = v;

		v = syncA / 16384;
		v = v % 128;
		m_OMD_ND_LungRDamage = v;

		v = syncA / 2097152;
		v = v % 128;
		m_OMD_ND_LiverDamage = v;

		int syncB;
		syncB = m_OMD_ND_OrganDamageSyncB;

		v = syncB / 1;
		v = v % 128;
		m_OMD_ND_GutsDamage = v;

		v = syncB / 128;
		v = v % 128;
		m_OMD_ND_SpleenDamage = v;

		// Client copy of the server level. Sync B has no free bits for the face scale.
		m_OMD_ND_FaceLevel = (syncB / 16384) % 4;

		OMD_NewDamage_RefreshAllScaleOrganLevels();

		int arteryPacked;
		arteryPacked = m_OMD_ND_ArteryLevelsSync;
		m_OMD_ND_ArtArmsLLevel = (arteryPacked / 1) % 4;
		m_OMD_ND_ArtArmsRLevel = (arteryPacked / 4) % 4;
		m_OMD_ND_ArtLegsLLevel = (arteryPacked / 16) % 4;
		m_OMD_ND_ArtLegsRLevel = (arteryPacked / 64) % 4;
		m_OMD_ND_ArtCarotidLevel = (arteryPacked / 256) % 4;
		m_OMD_ND_ArtFemoralLevel = (arteryPacked / 1024) % 4;
		m_OMD_ND_ArtSubclavianLLevel = (arteryPacked / 4096) % 4;
		m_OMD_ND_ArtSubclavianRLevel = (arteryPacked / 16384) % 4;
		m_OMD_ND_ArtCarotidSide = (arteryPacked / 65536) % 4;
		m_OMD_ND_ArtFemoralSide = (arteryPacked / 262144) % 4;
	}


	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type != OMD_NewDamage_ERPCs.RPC_PRESS_MODE_EXIT)
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		string inputId;
		inputId = "";
		if (!ctx.Read(inputId))
		{
			inputId = "UATempRaiseWeapon";
		}

		if (sender)
		{
			if (GetIdentity())
			{
				if (sender.GetPlainId() != GetIdentity().GetPlainId())
				{
					return;
				}
			}
		}

		if (OMD_NewDamageConfig.IsDebugEnabled())
		{
			string fromId;
			fromId = "-";
			if (sender)
			{
				fromId = sender.GetPlainId();
			}

			string exitSrvLine;
			exitSrvLine = "[OMD_NewDamage][PRESS] exit request received from=" + fromId;
			exitSrvLine = exitSrvLine + " input=" + inputId;
			Print(exitSrvLine);
		}

		if (inputId == "anim_lost")
		{
			OMD_NewDamage_RequestPressModeExit("anim_lost", "");
			return;
		}

		string exitReason;
		exitReason = "input:" + inputId;
		OMD_NewDamage_RequestPressModeExit(exitReason, inputId);
	}


	// --- moved from _90_Debug (cross-file API) ---
	protected void OMD_NewDamage_LogGlobalHp(TotalDamageResult damageResult, int damageType, int component, string dmgZone, string ammo, float hpBefore)
	{
		float zoneDmg;
		zoneDmg = 0;
		float globalDmg;
		globalDmg = 0;

		if (damageResult)
		{
			zoneDmg = damageResult.GetDamage(dmgZone, "Health");
			globalDmg = damageResult.GetDamage("", "Health");
		}

		float hpAfter;
		hpAfter = GetHealth("", "Health");

		float headHp;
		headHp = GetHealth("Head", "Health");
		float brainHp;
		brainHp = GetHealth("Brain", "Health");

		bool aliveFlag;
		aliveFlag = IsAlive();

		string line;
		line = "[OMD_NewDamage][GLOBAL] zone=" + dmgZone;
		line = line + " component=" + component.ToString();
		line = line + " ammo=" + ammo;
		line = line + " dmgType=" + damageType.ToString();
		line = line + " zoneDmg=" + zoneDmg.ToString();
		line = line + " globalDmg=" + globalDmg.ToString();
		line = line + " hpBefore=" + hpBefore.ToString();
		line = line + " hpAfter=" + hpAfter.ToString();
		line = line + " headHp=" + headHp.ToString();
		line = line + " brainHp=" + brainHp.ToString();
		line = line + " alive=" + aliveFlag.ToString();
		Print(line);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OMD_NewDamage_LogGlobalHpLater, 1000, false, hpBefore, dmgZone);
	}


	protected void OMD_NewDamage_LogGlobalHpLater(float hpBefore, string dmgZone)
	{
		float hpNow;
		hpNow = GetHealth("", "Health");

		string line;
		line = "[OMD_NewDamage][GLOBAL+1s] zone=" + dmgZone;
		line = line + " hpBefore=" + hpBefore.ToString();
		line = line + " hpNow=" + hpNow.ToString();
		Print(line);
	}


	protected void OMD_NewDamage_LogHitCalibration(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef, float hpBeforeHealth, float hpBeforeBlood, float hpBeforeShock, int hitOverride)
	{
		string compName;
		compName = GetActionComponentName(component, "fire");

		vector hitModel;
		hitModel = WorldToModel(modelPos);

		vector spine3Local;
		spine3Local = OMD_NewDamage_GetHitLocalToBone(hitModel, "Spine3");

		vector headLocal;
		headLocal = OMD_NewDamage_GetHitLocalToBone(hitModel, "Head");

		vector pelvisLocal;
		pelvisLocal = OMD_NewDamage_GetHitLocalToBone(hitModel, "Pelvis");

		float shooterDist;
		shooterDist = -1;

		vector bulletDirLocal;
		bulletDirLocal = "0 0 0";

		if (source)
		{
			shooterDist = vector.Distance(GetPosition(), source.GetPosition());
			bulletDirLocal = OMD_NewDamage_GetBulletDirLocal(source, hitModel);
		}

		string stance;
		stance = OMD_NewDamage_GetStanceName();

		string vestClass;
		vestClass = "-";
		float vestHealthMul;
		vestHealthMul = -1;

		ItemBase vestItem;
		vestItem = ItemBase.Cast(GetItemOnSlot("Vest"));
		if (vestItem)
		{
			vestClass = vestItem.GetType();
			vestHealthMul = OMD_NewDamage_GetGlobalArmorProjectileHealth(vestItem);
		}

		string bodyClass;
		bodyClass = "-";

		ItemBase bodyItem;
		bodyItem = ItemBase.Cast(GetItemOnSlot("Body"));
		if (bodyItem)
		{
			bodyClass = bodyItem.GetType();
		}

		string headgearClass;
		headgearClass = "-";
		float helmetHealthMul;
		helmetHealthMul = -1;

		ItemBase headgearItem;
		headgearItem = ItemBase.Cast(GetItemOnSlot("Headgear"));
		if (headgearItem)
		{
			headgearClass = headgearItem.GetType();
			helmetHealthMul = OMD_NewDamage_GetGlobalArmorProjectileHealth(headgearItem);
		}

		bool aliveFlag;
		aliveFlag = IsAlive();

		float zoneHealth;
		zoneHealth = 0;
		float zoneBlood;
		zoneBlood = 0;
		float zoneShock;
		zoneShock = 0;
		float globalHealth;
		globalHealth = 0;
		float globalBlood;
		globalBlood = 0;
		float globalShock;
		globalShock = 0;

		if (damageResult)
		{
			zoneHealth = damageResult.GetDamage(dmgZone, "Health");
			zoneBlood = damageResult.GetDamage(dmgZone, "Blood");
			zoneShock = damageResult.GetDamage(dmgZone, "Shock");
			globalHealth = damageResult.GetDamage("", "Health");
			globalBlood = damageResult.GetDamage("", "Blood");
			globalShock = damageResult.GetDamage("", "Shock");
		}

		CachedObjectsArrays.ARRAY_STRING.Clear();
		GetActionComponentNameList(component, CachedObjectsArrays.ARRAY_STRING, "fire");

		string fireNames;
		fireNames = "";
		int fireNameIndex;
		fireNameIndex = 0;
		int fireNameCount;
		fireNameCount = CachedObjectsArrays.ARRAY_STRING.Count();
		string oneFireName;
		oneFireName = "";
		while (fireNameIndex < fireNameCount)
		{
			oneFireName = CachedObjectsArrays.ARRAY_STRING.Get(fireNameIndex);
			if (fireNames != "")
			{
				fireNames = fireNames + ",";
			}

			fireNames = fireNames + oneFireName;
			fireNameIndex = fireNameIndex + 1;
		}

		CachedObjectsArrays.ARRAY_STRING.Clear();
		if (fireNames == "")
		{
			fireNames = "-";
		}

		string sourceType;
		sourceType = "-";
		bool isWeapon;
		isWeapon = false;
		if (source)
		{
			sourceType = source.GetType();
			isWeapon = source.IsWeapon();
		}

		float hpAfterHealth;
		hpAfterHealth = GetHealth("", "Health");
		float hpAfterBlood;
		hpAfterBlood = GetHealth("", "Blood");
		float hpAfterShock;
		hpAfterShock = GetHealth("", "Shock");

		float modelPosX;
		modelPosX = modelPos[0];
		float modelPosY;
		modelPosY = modelPos[1];
		float modelPosZ;
		modelPosZ = modelPos[2];
		bool modelPosZero;
		modelPosZero = false;
		if (modelPosX == 0)
		{
			if (modelPosY == 0)
			{
				if (modelPosZ == 0)
				{
					modelPosZero = true;
				}
			}
		}

		string line;
		line = "[OMD_NewDamage][HIT] zone=" + dmgZone;
		line = line + " ammo=" + ammo;
		line = line + " dmgType=" + damageType.ToString();
		line = line + " component=" + component.ToString();
		if (compName != "")
		{
			line = line + " compName=" + compName;
		}
		line = line + " modelPos=" + modelPos.ToString(false);
		line = line + " override=" + hitOverride.ToString();
		line = line + " hitModel=" + hitModel.ToString(false);
		line = line + " spine3Local=" + spine3Local.ToString(false);
		line = line + " headLocal=" + headLocal.ToString(false);
		line = line + " pelvisLocal=" + pelvisLocal.ToString(false);
		line = line + " shooterDist=" + shooterDist.ToString();
		line = line + " bulletDirLocal=" + bulletDirLocal.ToString(false);
		line = line + " stance=" + stance;
		line = line + " vest=" + vestClass;
		line = line + " vestHealthMul=" + vestHealthMul.ToString();
		line = line + " body=" + bodyClass;
		line = line + " headgear=" + headgearClass;
		line = line + " helmetHealthMul=" + helmetHealthMul.ToString();
		line = line + " alive=" + aliveFlag.ToString();
		line = line + " zoneHealth=" + zoneHealth.ToString();
		line = line + " zoneBlood=" + zoneBlood.ToString();
		line = line + " zoneShock=" + zoneShock.ToString();
		line = line + " globalHealth=" + globalHealth.ToString();
		line = line + " globalBlood=" + globalBlood.ToString();
		line = line + " globalShock=" + globalShock.ToString();
		line = line + " speedCoef=" + speedCoef.ToString();
		line = line + " fireNames=" + fireNames;
		line = line + " source=" + sourceType;
		line = line + " isWeapon=" + isWeapon.ToString();
		line = line + " hpBeforeHealth=" + hpBeforeHealth.ToString();
		line = line + " hpBeforeBlood=" + hpBeforeBlood.ToString();
		line = line + " hpBeforeShock=" + hpBeforeShock.ToString();
		line = line + " hpAfterHealth=" + hpAfterHealth.ToString();
		line = line + " hpAfterBlood=" + hpAfterBlood.ToString();
		line = line + " hpAfterShock=" + hpAfterShock.ToString();
		line = line + " modelPosZero=" + modelPosZero.ToString();
		Print(line);
	}

	protected void OMD_NewDamage_VitalServerTick(float deltaTime)
	{
	}


	protected void OMD_NewDamage_GetIncomingHitDamageParts(TotalDamageResult damageResult, string dmgZone, float hpBefore, out float outHp, out float outShock, out float outBlood)
	{
		outHp = 0;
		outShock = 0;
		outBlood = 0;
		float part;
		part = 0;

		if (damageResult)
		{
			part = damageResult.GetDamage("", "Health");
			if (part > outHp)
			{
				outHp = part;
			}

			part = damageResult.GetDamage(dmgZone, "Health");
			if (part > outHp)
			{
				outHp = part;
			}

			part = damageResult.GetDamage("", "Shock");
			if (part > outShock)
			{
				outShock = part;
			}

			part = damageResult.GetDamage(dmgZone, "Shock");
			if (part > outShock)
			{
				outShock = part;
			}

			part = damageResult.GetDamage("", "Blood");
			if (part > outBlood)
			{
				outBlood = part;
			}

			part = damageResult.GetDamage(dmgZone, "Blood");
			if (part > outBlood)
			{
				outBlood = part;
			}
		}

		if (outHp <= 0)
		{
			if (hpBefore > 0)
			{
				float hpAfter;
				hpAfter = GetHealth("", "Health");
				part = hpBefore - hpAfter;
				if (part > outHp)
				{
					outHp = part;
				}
			}
		}
	}


	protected float OMD_NewDamage_GetIncomingHitDamage(TotalDamageResult damageResult, string dmgZone, float hpBefore)
	{
		float hitHp;
		hitHp = 0;
		float hitShock;
		hitShock = 0;
		float hitBlood;
		hitBlood = 0;
		OMD_NewDamage_GetIncomingHitDamageParts(damageResult, dmgZone, hpBefore, hitHp, hitShock, hitBlood);
		float hitDmg;
		hitDmg = hitHp;
		if (hitShock > hitDmg)
		{
			hitDmg = hitShock;
		}

		if (hitBlood > hitDmg)
		{
			hitDmg = hitBlood;
		}

		return hitDmg;
	}


	protected void OMD_NewDamage_TryPressModeDamageExit(TotalDamageResult damageResult, int damageType, EntityAI source, string dmgZone, string ammo, float hpBefore)
	{
		bool isHolder;
		isHolder = m_OMD_ND_HolderPressModeActive;
		bool isPatient;
		isPatient = m_OMD_ND_PressActive;
		bool inMode;
		inMode = false;
		if (isHolder)
		{
			inMode = true;
		}

		if (isPatient)
		{
			inMode = true;
		}

		float hitHp;
		hitHp = 0;
		float hitShock;
		hitShock = 0;
		float hitBlood;
		hitBlood = 0;
		OMD_NewDamage_GetIncomingHitDamageParts(damageResult, dmgZone, hpBefore, hitHp, hitShock, hitBlood);
		float hitDmg;
		hitDmg = hitHp;
		if (hitShock > hitDmg)
		{
			hitDmg = hitShock;
		}

		if (hitBlood > hitDmg)
		{
			hitDmg = hitBlood;
		}

		string ammoName;
		ammoName = ammo;
		if (ammoName == "")
		{
			ammoName = "-";
		}

		if (inMode)
		{
			if (OMD_NewDamageConfig.IsDebugEnabled())
			{
				string hitCheckLine;
				hitCheckLine = "[OMD_NewDamage][PRESS] hit check inMode=" + inMode.ToString();
				hitCheckLine = hitCheckLine + " isHolder=" + isHolder.ToString();
				hitCheckLine = hitCheckLine + " isPatient=" + isPatient.ToString();
				hitCheckLine = hitCheckLine + " hp=" + hitHp.ToString();
				hitCheckLine = hitCheckLine + " shock=" + hitShock.ToString();
				hitCheckLine = hitCheckLine + " blood=" + hitBlood.ToString();
				hitCheckLine = hitCheckLine + " dmg=" + hitDmg.ToString();
				hitCheckLine = hitCheckLine + " ammo=" + ammoName;
				Print(hitCheckLine);
			}
		}

		if (!inMode)
		{
			return;
		}

		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		if (cfg)
		{
			if (!cfg.holdPressureInterruptOnDamage)
			{
				return;
			}
		}

		if (hitDmg <= 0)
		{
			return;
		}

		if (source)
		{
			if (source == this)
			{
				return;
			}

			EntityAI sourceRoot;
			sourceRoot = source.GetHierarchyRoot();
			if (sourceRoot == this)
			{
				return;
			}

			PlayerBase sourcePlayer;
			sourcePlayer = PlayerBase.Cast(source);
			if (!sourcePlayer)
			{
				sourcePlayer = PlayerBase.Cast(source.GetHierarchyRootPlayer());
			}

			if (sourcePlayer == this)
			{
				return;
			}
		}

		string sourceClass;
		sourceClass = "-";
		if (source)
		{
			sourceClass = source.GetType();
			if (sourceClass == "")
			{
				sourceClass = "-";
			}
		}

		string hitSource;
		hitSource = "ammo=" + ammoName;
		hitSource = hitSource + " dmgType=" + damageType.ToString();
		hitSource = hitSource + " source=" + sourceClass;
		hitSource = hitSource + " zone=" + dmgZone;

		if (isPatient)
		{
			m_OMD_ND_PressExternalHitPending = true;
			m_OMD_ND_PressInterruptHitDamage = hitDmg;
			m_OMD_ND_PressInterruptDamageSource = hitSource;
			OMD_NewDamage_EndPressWound("damage");
		}

		if (isHolder)
		{
			if (m_OMD_ND_HolderPressModeActive)
			{
				OMD_NewDamage_RequestPressModeExit("damage", "");
			}
		}
	}


	void OMD_NewDamage_BeginFreshFall(float height)
	{
		m_OMD_ND_JumpOutPending = false;
		m_OMD_ND_JumpOutTransport = null;
		m_OMD_ND_FreshFallActive = true;
		m_OMD_ND_FreshFallHeight = height;
	}


	void OMD_NewDamage_EndFreshFall()
	{
		m_OMD_ND_FreshFallActive = false;
		m_OMD_ND_FreshFallHeight = 0;
	}


	void OMD_NewDamage_ClearFallPending()
	{
		m_OMD_ND_FallPendingHp = 0;
		m_OMD_ND_FallKillPending = false;
		m_OMD_ND_FallPendingActive = false;
	}

	void OMD_NewDamage_ApplyFallPendingAfterShock(float height)
	{
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	protected void OMD_NewDamage_TryApplyGlobalHitMitigationFromEEHitBy(TotalDamageResult damageResult, string dmgZone, string ammo, EntityAI source)
	{
	}


	float OMD_NewDamage_GetBrainDamageValue()
	{
		return m_OMD_ND_BrainDamage;
	}


	int OMD_NewDamage_GetBrainLevelValue()
	{
		return m_OMD_ND_BrainLevel;
	}


	void OMD_NewDamage_SetBrainDamageValue(float damage)
	{
		m_OMD_ND_BrainDamage = OMD_NewDamage_ClampOrganDamage(damage);
		m_OMD_ND_BrainLevel = OMD_NewDamage_DamageToLevel(m_OMD_ND_BrainDamage, OMD_NewDamageConfig.Get());
		OMD_NewDamage_SyncVitalLevelsToNet();
	}

	protected void OMD_NewDamage_TryBrainMeleeHit(TotalDamageResult damageResult, int damageType, EntityAI source, string dmgZone, string ammo)
	{
	}

	void OMD_SetHitOverride(string compName, vector modelPos)
	{
	}


	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		bool hitOverrideActive;
		hitOverrideActive = m_OMD_HitOverrideActive;
		string hitOverrideComp;
		hitOverrideComp = m_OMD_HitOverrideComp;
		vector hitOverrideModelPos;
		hitOverrideModelPos = m_OMD_HitOverrideModelPos;
		m_OMD_HitOverrideActive = false;

		if (OMD_NewDamageConfig.IsDebugEnabled())
		{
			string enterComp;
			enterComp = GetActionComponentName(component, "fire");
			Print("[OMD_NewDamage][ENTER] zone=" + dmgZone + " comp=" + enterComp);
		}

		float hpBefore;
		hpBefore = 0;
		float hpBeforeBlood;
		hpBeforeBlood = 0;
		float hpBeforeShock;
		hpBeforeShock = 0;
		bool logGlobalHp;
		logGlobalHp = false;
		bool alreadyDead;
		alreadyDead = false;

		if (GetGame().IsServer())
		{
			if (!IsAlive())
			{
				alreadyDead = true;
			}

			if (GetHealth("", "Health") <= 0)
			{
				alreadyDead = true;
			}

			hpBefore = GetHealth("", "Health");
			if (OMD_NewDamageConfig.IsGlobalHpLogEnabled())
			{
				logGlobalHp = true;
			}

			if (OMD_NewDamageConfig.IsHitCalibrationLogEnabled())
			{
				if (OMD_NewDamage_IsCalibrationZone(dmgZone))
				{
					hpBeforeBlood = GetHealth("", "Blood");
					hpBeforeShock = GetHealth("", "Shock");
				}
			}
		}

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (!GetGame().IsServer())
		{
			return;
		}

		OMD_NewDamage_TryPressModeDamageExit(damageResult, damageType, source, dmgZone, ammo, hpBefore);

		if (logGlobalHp)
		{
			OMD_NewDamage_LogGlobalHp(damageResult, damageType, component, dmgZone, ammo, hpBefore);
		}

		if (alreadyDead)
		{
			return;
		}

		OMD_NewDamage_TryApplyGlobalHitMitigationFromEEHitBy(damageResult, dmgZone, ammo, source);

		bool skipLowSpeedOrgan;
		skipLowSpeedOrgan = false;
		if (damageType == DamageType.FIRE_ARM)
		{
			OMD_NewDamageConfigData speedCfg;
			speedCfg = OMD_NewDamageConfig.Get();
			if (speedCfg)
			{
				if (speedCoef < speedCfg.organMinSpeedCoefFirearm)
				{
					skipLowSpeedOrgan = true;
					if (OMD_NewDamageConfig.IsDebugEnabled())
					{
						string skipCompName;
						skipCompName = GetActionComponentName(component, "fire");
						string skipLine;
						skipLine = "[OMD][ORGAN] skip lowSpeed speedCoef=" + speedCoef.ToString();
						skipLine = skipLine + " min=" + speedCfg.organMinSpeedCoefFirearm.ToString();
						skipLine = skipLine + " comp=" + skipCompName;
						skipLine = skipLine + " zone=" + dmgZone;
						Print(skipLine);
					}
				}
			}
		}

		if (!skipLowSpeedOrgan)
		{
			OMD_NewDamage_ProcessOrganHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, hitOverrideActive, hitOverrideComp, hitOverrideModelPos);
		}
		OMD_NewDamage_TryCreatureOrganHit(damageResult, source, dmgZone, ammo);
		OMD_NewDamage_TryBrainMeleeHit(damageResult, damageType, source, dmgZone, ammo);
		OMD_NewDamage_TryArteryDropOnHit();

		if (!OMD_NewDamageConfig.IsHitCalibrationLogEnabled())
		{
			return;
		}

		if (!OMD_NewDamage_IsCalibrationZone(dmgZone))
		{
			return;
		}

		vector logModelPos;
		logModelPos = modelPos;
		int logOverride;
		logOverride = 0;
		if (hitOverrideActive)
		{
			logModelPos = hitOverrideModelPos;
			logOverride = 1;
		}

		OMD_NewDamage_LogHitCalibration(damageResult, damageType, source, component, dmgZone, ammo, logModelPos, speedCoef, hpBefore, hpBeforeBlood, hpBeforeShock, logOverride);
	}


	protected bool OMD_NewDamage_IsPlayerMeleeSource(EntityAI source)
	{
		if (!source)
		{
			return false;
		}

		if (source.IsInherited(ZombieBase))
		{
			return false;
		}

		if (source.IsInherited(AnimalBase))
		{
			return false;
		}

		PlayerBase attacker;
		attacker = PlayerBase.Cast(source);
		if (attacker)
		{
			return true;
		}

		attacker = PlayerBase.Cast(source.GetHierarchyRootPlayer());
		if (attacker)
		{
			return true;
		}

		return false;
	}


	protected string OMD_NewDamage_GetMeleeWeaponClassname(EntityAI source)
	{
		if (!source)
		{
			return "";
		}

		PlayerBase attacker;
		attacker = PlayerBase.Cast(source);
		if (!attacker)
		{
			attacker = PlayerBase.Cast(source.GetHierarchyRootPlayer());
		}

		if (attacker)
		{
			HumanInventory inv;
			inv = attacker.GetHumanInventory();
			if (inv)
			{
				EntityAI handsItem;
				handsItem = inv.GetEntityInHands();
				if (handsItem)
				{
					return handsItem.GetType();
				}
			}
		}

		return source.GetType();
	}


	protected bool OMD_NewDamage_IsMeleeForceBluntItem(string classname, OMD_NewDamageConfigData cfg)
	{
		if (classname == "")
		{
			return false;
		}

		if (!cfg)
		{
			return false;
		}

		if (!cfg.meleeForceBluntItems)
		{
			return false;
		}

		if (cfg.meleeForceBluntItems.Find(classname) < 0)
		{
			return false;
		}

		return true;
	}


	protected float OMD_NewDamage_GetAmmoBleedThreshold(string ammo)
	{
		if (ammo == "")
		{
			return 0;
		}

		return GetGame().ConfigGetFloat("CfgAmmo " + ammo + " DamageApplied bleedThreshold");
	}


	string OMD_NewDamage_ClassifyOrganHitWeapon(int damageType, EntityAI source, string ammo, float bloodDamage, OMD_NewDamageConfigData cfg)
	{
		if (damageType == DamageType.FIRE_ARM)
		{
			return "firearm";
		}

		if (damageType != DamageType.CLOSE_COMBAT)
		{
			return "";
		}

		if (!OMD_NewDamage_IsPlayerMeleeSource(source))
		{
			return "blunt";
		}

		string weaponClass;
		weaponClass = OMD_NewDamage_GetMeleeWeaponClassname(source);
		if (OMD_NewDamage_IsMeleeForceBluntItem(weaponClass, cfg))
		{
			return "blunt";
		}

		float bleed;
		bleed = OMD_NewDamage_GetAmmoBleedThreshold(ammo);
		if (bleed <= cfg.meleeSharpMinBleedThreshold)
		{
			return "blunt";
		}

		if (bloodDamage <= 0)
		{
			return "blunt";
		}

		if (ammo.Contains("MeleeFist"))
		{
			return "blunt";
		}

		if (OMD_NewDamage_AmmoHasPrefix(ammo, "MeleeBlunt"))
		{
			return "blunt";
		}

		if (ammo == "FirearmHit_Rifle_Buttstock")
		{
			return "blunt";
		}

		if (ammo == "FirearmHit_Rifle")
		{
			return "blunt";
		}

		return "sharp";
	}


	protected bool OMD_NewDamage_AmmoHasPrefix(string ammo, string prefix)
	{
		if (ammo == "")
		{
			return false;
		}

		if (prefix == "")
		{
			return false;
		}

		if (ammo.Length() < prefix.Length())
		{
			return false;
		}

		if (ammo.IndexOf(prefix) != 0)
		{
			return false;
		}

		return true;
	}


	string OMD_NewDamage_ClassifyCreatureOrganSource(EntityAI source, string ammo)
	{
		if (ammo == "Dummy_Light")
		{
			return "";
		}

		if (ammo == "MeleeBearShock")
		{
			return "";
		}

		if (source)
		{
			if (source.IsInherited(ZombieBase))
			{
				return "infected";
			}

			if (source.IsInherited(AnimalBase))
			{
				return "animal";
			}
		}

		if (OMD_NewDamage_AmmoHasPrefix(ammo, "MeleeFist"))
		{
			return "fist";
		}

		if (OMD_NewDamage_AmmoHasPrefix(ammo, "MeleeBlunt"))
		{
			return "blunt";
		}

		if (ammo == "FirearmHit_Rifle_Buttstock")
		{
			return "blunt";
		}

		if (ammo == "FirearmHit_Rifle")
		{
			return "blunt";
		}

		return "";
	}

	protected void OMD_NewDamage_TryCreatureOrganHit(TotalDamageResult damageResult, EntityAI source, string dmgZone, string ammo)
	{
	}

	protected void OMD_NewDamage_ProcessOrganHit(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, bool hitOverrideActive, string hitOverrideComp, vector hitOverrideModelPos)
	{
	}


	protected void OMD_NewDamage_ProcessBrainKill()
	{
		if (!IsAlive())
		{
			return;
		}

		string cause;
		cause = m_OMD_Symptom_DeathCause;
		if (cause == "")
		{
			cause = "brain";
		}
		OMD_NewDamageConfig.Get();
		OMD_NewDamage_LogDeath("brain", m_OMD_ND_BrainLevel, cause);
		SetHealth(0);
	}


	float OMD_NewDamage_DebugGetOrganTimer(string organKey)
	{
		// Legacy API for /omdorgan reply: death timers removed, always 0.
		return 0;
	}


	// Treatment v1 hooks — empty stubs (R11). Logic later.
	bool OMD_CanTreat(PlayerBase patient, string damageId, PlayerBase medic, ItemBase tool)
	{
		return false;
	}


	void OMD_OnTreatStart(PlayerBase patient, string damageId, PlayerBase medic, ItemBase tool)
	{
	}


	void OMD_OnTreatSuccess(PlayerBase patient, string damageId, PlayerBase medic, ItemBase tool)
	{
	}


	void OMD_OnTreatFail(PlayerBase patient, string damageId, PlayerBase medic, ItemBase tool)
	{
	}


	float OMD_GetTreatTime(PlayerBase patient, string damageId, PlayerBase medic, ItemBase tool)
	{
		return 0;
	}


	float OMD_GetTreatFailChance(PlayerBase patient, string damageId, PlayerBase medic, ItemBase tool)
	{
		return 1;
	}


	protected int OMD_NewDamage_GetSymptomDebuffLevel()
	{
		int level;
		level = m_OMD_ND_HeartLevel;
		int lungLevel;
		lungLevel = m_OMD_ND_LungLLevel;
		if (m_OMD_ND_LungRLevel > lungLevel)
		{
			lungLevel = m_OMD_ND_LungRLevel;
		}

		if (lungLevel > level)
		{
			level = lungLevel;
		}

		if (m_OMD_ND_FaceLevel > level)
		{
			level = m_OMD_ND_FaceLevel;
		}

		int art;
		art = OMD_NewDamage_GetWorstLegsArteryLevel();
		int arms;
		arms = OMD_NewDamage_GetWorstArmsArteryLevel();
		if (arms > art)
		{
			art = arms;
		}

		if (m_OMD_ND_ArtCarotidLevel > art)
		{
			art = m_OMD_ND_ArtCarotidLevel;
		}

		if (m_OMD_ND_ArtSubclavianLLevel > art)
		{
			art = m_OMD_ND_ArtSubclavianLLevel;
		}

		if (m_OMD_ND_ArtSubclavianRLevel > art)
		{
			art = m_OMD_ND_ArtSubclavianRLevel;
		}

		if (art > level)
		{
			level = art;
		}

		return level;
	}


	protected void OMD_NewDamage_LogDebuffCacheSymptoms()
	{
		int level;
		level = OMD_NewDamage_GetSymptomDebuffLevel();
		bool sprintBanned;
		sprintBanned = false;
		if (!m_OMD_ND_CachedSprintAllowed)
		{
			sprintBanned = true;
		}

		bool wasBanned;
		wasBanned = false;
		if (!m_OMD_Symptom_SprintWasAllowed)
		{
			wasBanned = true;
		}

		float sprintValue;
		sprintValue = 1;
		if (sprintBanned)
		{
			sprintValue = 0;
		}

		OMD_NewDamage_LogCachedBool("body", level, "SprintBan", sprintBanned, wasBanned, sprintValue, "debuff");
		m_OMD_Symptom_SprintWasAllowed = m_OMD_ND_CachedSprintAllowed;

		bool runBanned;
		runBanned = false;
		if (!m_OMD_ND_CachedRunAllowed)
		{
			runBanned = true;
		}

		bool runWasBanned;
		runWasBanned = false;
		if (!m_OMD_Symptom_RunWasAllowed)
		{
			runWasBanned = true;
		}

		float runValue;
		runValue = 1;
		if (runBanned)
		{
			runValue = 0;
		}

		OMD_NewDamage_LogCachedBool("body", level, "RunBan", runBanned, runWasBanned, runValue, "debuff");
		m_OMD_Symptom_RunWasAllowed = m_OMD_ND_CachedRunAllowed;
		OMD_NewDamage_LogCachedFloat("body", level, "StaminaDrain", m_OMD_ND_CachedStaminaDrainMult, m_OMD_Symptom_StaminaDrain, 1, "debuff");
		m_OMD_Symptom_StaminaDrain = m_OMD_ND_CachedStaminaDrainMult;
		OMD_NewDamage_LogCachedFloat("body", level, "StaminaRegen", m_OMD_ND_CachedStaminaRegenMult, m_OMD_Symptom_StaminaRegen, 1, "debuff");
		m_OMD_Symptom_StaminaRegen = m_OMD_ND_CachedStaminaRegenMult;
		OMD_NewDamage_LogCachedFloat("body", level, "AimShake", m_OMD_ND_CachedAimShake, m_OMD_Symptom_AimShake, 1, "debuff");
		m_OMD_Symptom_AimShake = m_OMD_ND_CachedAimShake;
	}

};
