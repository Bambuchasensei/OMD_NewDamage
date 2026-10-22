modded class InediaInfectedAI_PainManager
{

	override float GetDeepWoundsBleedingRateWithMultipliers()
	{
		float bloodLossRatePerSec;
		bloodLossRatePerSec = super.GetDeepWoundsBleedingRateWithMultipliers();

		PlayerBase player;
		player = PlayerBase.Cast(m_Player);
		if (!player)
		{
			return bloodLossRatePerSec;
		}

		float afterPress;
		afterPress = player.OMD_NewDamage_ApplyPressDeepWoundBleedMultiplier(this, bloodLossRatePerSec);
		float unconMult;
		unconMult = player.OMD_NewDamage_GetUnconsciousInediaDeepMult();
		return afterPress * unconMult;
	}

};
