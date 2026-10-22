class OMD_NewDamage_PressModeCB : HumanCommandActionCallback
{
	bool m_IsFullbody;
	PlayerBase m_Player;

	void ~OMD_NewDamage_PressModeCB()
	{
		if (GetGame() && m_Player)
		{
			m_Player.OMD_NewDamage_OnPressModeCallbackEnd();
		}
	}


	bool CancelCondition()
	{
		return false;
	}

};
