class CfgPatches
{
	class OMD_NewDamage
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Characters",
			"InediaInfectedAI",
			"OMD_Bridge",
			"TerjeCore",
			"TerjeMedicine",
			"TerjeSkills"
		};
	};
};

class CfgMods
{
	class OMD_NewDamage
	{
		dir = "OMD_NewDamage";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 0;
		name = "OMD NewDamage";
		credits = "OMD";
		author = "OMD";
		authorID = "0";
		version = "0.1.0";
		type = "mod";
		dependencies[] = { "Game", "World", "Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "OMD_NewDamage/Scripts/3_Game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "OMD_NewDamage/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "OMD_NewDamage/Scripts/5_Mission" };
			};
		};
	};
};

class CfgVehicles
{
	class Man;
	class SurvivorBase: Man
	{
		class DamageSystem
		{
			class DamageZones
			{
				class Head
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0.10;
					};
					fatalInjuryCoef = 0;
				};
				class Brain
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0.0;
					};
					fatalInjuryCoef = 0;
				};
				class Torso
				{
					class Health
					{
						transferToGlobalCoef = 0.05;
					};
				};
				class LeftLeg
				{
					class Health
					{
						transferToGlobalCoef = 0.1;
					};
				};
				class RightLeg
				{
					class Health
					{
						hitpoints = 125;
						transferToGlobalCoef = 0.1;
					};
					class Blood
					{
						hitpoints = 100;
						transferToGlobalCoef = 0;
					};
					class Shock
					{
						hitpoints = 0;
						transferToGlobalCoef = 1;
					};
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage = 1.2;
							};
							class Blood
							{
								damage = 1;
							};
							class Shock
							{
								damage = 0.33;
							};
						};
					};
					fatalInjuryCoef = -1;
					componentNames[] =
					{
						"dmgZone_rightLeg"
					};
					inventorySlots[] =
					{
						"Legs"
					};
				};
			};
		};
	};
};

// R14: temporary vanilla samples behind OMD_ SoundSet names.
class CfgSoundShaders
{
	class OMD_Heart_Heartbeat_Base_SoundShader
	{
		samples[] = {{"OMD_NewDamage\Sounds\hearthbit.ogg",1}};
		volume = 0.6;
		range = 8;
	};
	class OMD_Heart_Heartbeat_L1_SoundShader: OMD_Heart_Heartbeat_Base_SoundShader
	{
		volume = 0.35;
	};
	class OMD_Heart_Heartbeat_L2_SoundShader: OMD_Heart_Heartbeat_Base_SoundShader
	{
		volume = 0.55;
	};
	class OMD_Heart_Heartbeat_L3_SoundShader: OMD_Heart_Heartbeat_Base_SoundShader
	{
		volume = 0.85;
	};
	class OMD_Lung_CoughBlood_SoundShader
	{
		samples[] = {{"DZ\sounds\Characters\voice\male_1\cough_1",1}};
		volume = 0.8;
		range = 20;
	};
	class OMD_Lung_Gurgle_SoundShader
	{
		samples[] = {{"DZ\sounds\Characters\voice\male_1\HeavyBreathing_1",1}};
		volume = 0.5;
		range = 12;
	};
	class OMD_Lung_BreathHeavy_SoundShader
	{
		samples[] = {{"DZ\sounds\Characters\voice\male_1\HeavyBreathing_2",1}};
		volume = 0.65;
		range = 12;
	};
};

class CfgSoundSets
{
	class OMD_Heart_Heartbeat_L1_SoundSet
	{
		soundShaders[] = {"OMD_Heart_Heartbeat_L1_SoundShader"};
		volumeFactor = 1;
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class OMD_Heart_Heartbeat_L2_SoundSet
	{
		soundShaders[] = {"OMD_Heart_Heartbeat_L2_SoundShader"};
		volumeFactor = 1;
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class OMD_Heart_Heartbeat_L3_SoundSet
	{
		soundShaders[] = {"OMD_Heart_Heartbeat_L3_SoundShader"};
		volumeFactor = 1;
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class OMD_Lung_CoughBlood_SoundSet
	{
		soundShaders[] = {"OMD_Lung_CoughBlood_SoundShader"};
		volumeFactor = 1;
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class OMD_Lung_Gurgle_SoundSet
	{
		soundShaders[] = {"OMD_Lung_Gurgle_SoundShader"};
		volumeFactor = 1;
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class OMD_Lung_BreathHeavy_SoundSet
	{
		soundShaders[] = {"OMD_Lung_BreathHeavy_SoundShader"};
		volumeFactor = 1;
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
};
