modded class DayZPlayerImplementFallDamage
{

	override void HandleFallDamage(FallDamageData pData)
	{
		PlayerBase playerBase;
		playerBase = PlayerBase.Cast(m_Player);
		float fallHeight;
		fallHeight = 0;
		if (playerBase)
		{
			playerBase.OMD_NewDamage_ClearFallPending();
			if (pData)
			{
				fallHeight = pData.m_Height;
				playerBase.OMD_NewDamage_BeginFreshFall(fallHeight);
			}
		}

		super.HandleFallDamage(pData);

		if (playerBase)
		{
			playerBase.OMD_NewDamage_ApplyFallPendingAfterShock(fallHeight);
			playerBase.OMD_NewDamage_EndFreshFall();
		}

		if (m_FallDamageData)
		{
			m_FallDamageData.m_Height = 0;
		}
	}

};
