modded class MissionGameplay
{

	override void OnMissionStart()
	{
		super.OnMissionStart();
		PPERequesterBank.GetRequester(PPERequesterBank.REQ_OMD_NEWDAMAGE_ORGANS).Start();
	}


	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		PPERequesterBank.GetRequester(PPERequesterBank.REQ_OMD_NEWDAMAGE_ORGANS).Stop();
	}


	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		PPERequesterBank.GetRequester(PPERequesterBank.REQ_OMD_NEWDAMAGE_ORGANS).SetRequesterUpdating(true);
	}

};
