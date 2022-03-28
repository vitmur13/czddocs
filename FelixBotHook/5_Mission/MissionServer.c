modded class MissionServer
{

	void MissionServer() {
	}

	void fb_server_reporting() {
		GetDayZGame().GetFelixBotAPI().sendServerReport();
	}

	ref array<string> FBParseCommand(string CommandLine)
	{
		ref array<string> tempstr = new array<string>;
		ref array<string> cmdline = new array<string>;
		string cmdparams;
		CommandLine.Split(" ", tempstr);
		cmdline.Insert(tempstr.Get(0));
		if (tempstr.Count() > 1)
		{
			for (int i = 1; i < tempstr.Count(); i++)
			{
				cmdparams = cmdparams + tempstr.Get(i) + " ";
			}
			cmdparams = cmdparams.Trim();
			cmdline.Insert(cmdparams);
		}
		return cmdline;
	}

	//Find player
	static PlayerBase GetTargetPlayer(string cData)
	{
		if (cData.Length() > 0)
		{
			array<Man> players = new array<Man>;
			GetGame().GetPlayers(players);
			for (int i = 0; i < players.Count(); ++i)
			{
				PlayerIdentity selectedIdentity = PlayerBase.Cast(players.Get(i)).GetIdentity();
				if (selectedIdentity.GetName() == cData || selectedIdentity.GetPlainId() == cData || selectedIdentity.GetPlayerId() == cData.ToInt())
				{
					return PlayerBase.Cast(players.Get(i));
				}
			}
		}
		return NULL;
	}

	void FBOnAdminChatRequest(Param request_params)
	{
		ChatMessageEventParams chat_params = ChatMessageEventParams.Cast(request_params);
		switch (chat_params.param1)
		{
			case 0:
			{
				if (chat_params.param2 == "Server" || chat_params.param2 == "System")
				{
					break;
				}

				PlayerBase player;
				PlayerIdentity playerIdentity

				array<Man> players = new array<Man>;
				GetGame().GetPlayers(players);
				for (int i = 0; i < players.Count(); ++i)
				{
					playerIdentity = PlayerBase.Cast(players.Get(i)).GetIdentity();
					if (playerIdentity.GetName() == chat_params.param2 || playerIdentity.GetPlainId() == chat_params.param2 || playerIdentity.GetPlayerId() == chat_params.param2.ToInt())
					{
						player = PlayerBase.Cast(players.Get(i));
					}
				}

				//DEBUG CHAT INPUT END
				if (chat_params.param1 == 0 && chat_params.param2 != "" && chat_params.param3 != "") //trigger only when channel is Global == 0,  Player Name does not equal to null and command 
				{
					if (player && playerIdentity)
					{

						ref array<string> chatCommand = FBParseCommand(chat_params.param3);

						//VARS
						string cCommand, cData, cData2;

						EntityAI itemEnt = NULL;

						//VARS

						cCommand = chatCommand.Get(0);
						cCommand.ToLower();
						string cFullCommand = cCommand;
						if (chatCommand.Count() > 1)
						{
							cData = chatCommand.Get(1);
							cFullCommand = cCommand + " " + cData;
						}
						else {
							cData = "";
						}

						switch (cCommand)
						{

								//---- SPAWN:OBJECTS ----//
							case "!fpromo":
							{
								if (cData == "") {
									FBSCM(player, "FelixBot: !fpromo <code>");
								}
								else {
									
									usagePromocode(player, cData);

								}
							}

							break;
							//---- SPAWN:OBJECTS ----//
							case "/fpromo":
							{
								if (cData == "") {
									FBSCM(player, "FelixBot: !fpromo <code>");
								}
								else {

									usagePromocode(player, cData);

								}
							}
							break;
							//---- SPAWN:OBJECTS ----//
							case "@fpromo":
							{
								if (cData == "") {
									FBSCM(player, "FelixBot: !fpromo <code>");
								}
								else {

									usagePromocode(player, cData);

								}
							}
							break;
							//---- SPAWN:OBJECTS ----//
							case "#fpromo":
							{
								if (cData == "") {
									FBSCM(player, "FelixBot: !fpromo <code>");
								}
								else {

									usagePromocode(player, cData);

								}
							}
							break;
						}
					}
				}
			}
		}
	}
	
	//static const string fb_configuration_path = "felixbot.cfg";
	static const string fb_configuration_path = "$profile:felixbot.cfg";

	override void OnInit() {

		FelixBotConfig config = GetDayZGame().GetFBCFG();

		GetDayZGame().AdminLog("FelixBotHook.MissionServer.OnInit()");

		if (!FileExist(fb_configuration_path)) {
			fb_fail_with_error(1, "FelixBotHook requires a felixbot.cfg in the profiles path!");
			return;
		}

		JsonFileLoader <FelixBotConfig>.JsonLoadFile(fb_configuration_path, config);
		if (!config.api_token) {
			fb_fail_with_error(2, "FelixBotHook requires APIToken Key");
			return;
		}

		GetDayZGame().SetupFelixBotAPI();
		GetDayZGame().GetFelixBotAPI().verify_credentials();

		GetGame().AdminLog("FelixBotHook.MissionServer.OnInit() Initial checks succeeded.");

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fb_server_reporting, 30000, true);

		GetRPCManager().AddRPC( "RPC_FB_MissionServer", "HandleChatCommand", this, SingeplayerExecutionType.Server );    
		GetRPCManager().AddRPC( "RPC_FB_MissionServer", "HandleBreachingChargeExplode", this, SingeplayerExecutionType.Server );    

		super.OnInit();
	}

	void HandleBreachingChargeExplode(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target) {
	        Param1<string> data; //chat string
        if ( !ctx.Read( data ) ) return;

        if (type == CallType.Server)
        {
			GetDayZGame().GetFelixBotAPI().FBNotificationMessageHandle("Ñ4", "C4 activated in coordinates: " + data.param1);
		}
	}

	void HandleChatCommand( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<string> data; //chat string
        if ( !ctx.Read( data ) ) return;

        if (type == CallType.Server)
        {
			
			PlayerBase player = GetTargetPlayer(sender.GetPlainId());


			ref array<string> chatCommand = FBParseCommand(data.param1);

			//VARS
			string cCommand, cData, cData2;

			EntityAI itemEnt = NULL;

			//VARS

			cCommand = chatCommand.Get(0);
			cCommand.ToLower();
			string cFullCommand = cCommand;
			if (chatCommand.Count() > 1)
			{
				cData = chatCommand.Get(1);
				cFullCommand = cCommand + " " + cData;
			}
			else {
				cData = "";
			}

			switch (cCommand)
			{
				case "$fpromo":
				{
					if (cData == "") {
						FBSCM(player, "FelixBot: $fpromo <code>");
					}
					else {
						usagePromocode(player, cData);
					}
				}
			}

        }
    }

    override void OnEvent(EventType eventTypeId, Param params) {
        super.OnEvent(eventTypeId,params);

        if(eventTypeId == ChatMessageEventTypeID) {

            ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );
            string message = chat_params.param3, prefix, param0, command;

			GetDayZGame().GetFelixBotAPI().newChatMessage(message, chat_params.param2);

			if (chat_params.param1 == 0 && chat_params.param2 != "" && chat_params.param3 != "") //trigger only when channel is Global == 0, Player Name does not equal to null and entered command
			{
				FBOnAdminChatRequest(params); //Redirect call from chat to AdminMod
			}
        }
    }
};