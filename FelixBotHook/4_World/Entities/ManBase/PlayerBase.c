modded class PlayerBase
{	
	void FB_PlayerKilled( PlayerBase player, Object source )  // PlayerBase.c   
	{
		PlayerBase killer = NULL;
		string type = "unknown";
		string k_name = "";
		string k_uid = "";
		string k_item = "";
		int dist = 0;

		if(player == NULL || source == NULL) return;

		PlayerIdentity p_id = player.GetIdentity();

		if ( player && source )
		{	
			if( player == source )	// deaths not caused by another object (starvation, dehydration)
			{
				type = "suicide";
			}
			else if ( source.IsWeapon() || source.IsMeleeWeapon() )  // player
			{				
				killer = PlayerBase.Cast( EntityAI.Cast( source ).GetHierarchyParent() );
				if(killer)
				{
					type = "killer";

					k_name = killer.GetIdentity().GetName();
					k_uid = killer.GetIdentity().GetPlainId();
					dist = vector.Distance( player.GetPosition(), killer.GetPosition() );
					k_item = source.GetDisplayName();
				}
			}
			else // others
			{
				if (source.IsKindOf("AnimalBase") || source.IsInherited(DayZAnimal))
				{
					type = "animal";
				}
				else if (source.IsKindOf("ZombieBase"))
				{
					type = "zombie";
				}	
			}
		}
		else 
		{
			type = "unknown";
			//LogPrint("DEBUG: PlayerKilled() player/source does not exist");
		}	

		GetDayZGame().GetFelixBotAPI().newKillFeedMessage(type, p_id.GetName(), p_id.GetPlainId(), player.GetPosition().ToString(), k_name, k_uid, k_item, dist);
	}

	override void EEKilled(Object killer)
	{
		FB_PlayerKilled(this, killer);
		super.EEKilled(killer);
	}
}