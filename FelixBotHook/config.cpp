class CfgPatches
{
	class FelixBotHook
	{
		units[]={};
		weapons[]={};
		requiredAddons[]={
			"JM_CF_Scripts"
		};
	};
};

class CfgMods
{
	class FelixBotHook
	{
		dir = "FelixBotHook";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "FelixBotHook";
		credits = "";
		author = "Weishaypt";
		authorID = "0"; 
		version = "1.1.0"; 
		extra = 0;
		type = "mod";
		
		dependencies[] = {"Common", "Game", "Mission", "World"};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"FelixBotHook/3_Game"};
			}

			class missionScriptModule
			{
				value = "";
				files[] = {"FelixBotHook/5_Mission"};
			};

			class worldScriptModule
			{
				value = "";
				files[] = { "FelixBotHook/4_World" };
			};
		};
	};
};