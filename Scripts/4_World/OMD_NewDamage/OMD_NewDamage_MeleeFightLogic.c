// Guard only while press FB is active. Do NOT early-out on GetCommand_Move()==null:
// during fist/weapon combo command is COMMANDID_MELEE2, so Move is null and HitEvent
// would never run (regression: punches pass through).
// Vanilla SetMeleeBlock is only under COMMANDID_MOVE
// (dayzplayermeleefightlogic_lightheavy.c:202-275, hcm at :143, SetMeleeBlock :262/:267/:273).
modded class DayZPlayerMeleeFightLogic_LightHeavy
{

	override bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		PlayerBase pressPlayer;
		pressPlayer = PlayerBase.Cast(m_Player);
		if (pressPlayer)
		{
			if (pressPlayer.OMD_NewDamage_IsHolderPressModeActive())
			{
				return false;
			}
		}

		return super.HandleFightLogic(pCurrentCommandID, pInputs, pEntityInHands, pMovementState, pContinueAttack);
	}

};
