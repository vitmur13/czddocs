modded class NotificationSystem
{
	override void AddNotif(ref NotificationRuntimeData data)
    {
        super.AddNotif(data);
    }
	
	static void SendNotificationToPlayerIdentityExtended( PlayerIdentity player, float show_time, string title_text, string detail_text = "", string icon = "" )
	{		
		if(player == null) GetDayZGame().GetFelixBotAPI().FBNotificationMessageHandle(title_text, detail_text);

		ScriptRPC rpc = new ScriptRPC();
		
		rpc.Write(show_time);
		rpc.Write(title_text);
		rpc.Write(detail_text);
		rpc.Write(icon);

		rpc.Send(null, ERPCs.RPC_SEND_NOTIFICATION_EXTENDED, true, player)
	}
}