class FBPromoKitItem {
	string key;
	string value;
};

class FBPromoItem {
	string id;
	string className;
	int count;
	int quantity;
	float metrage;
	string type;
	ref array<ref FBPromoKitItem> items;
};

class FBPromoUsageResponse {
	int error;
	string message;
	ref array<ref FBPromoItem> data;
};

class FBPromoUsage : FBBaseRequestData {

	string code;
	string uid;
	string version;

	void FBPromoUsage(FelixBotConfig config, string version_, string code_, string uid_) {
		api_token = config.api_token;

		version = version_;
		code = code_;
		uid = uid_;
	}
};

void FBSCM(PlayerBase Player, string Message)
{
	
	//NotificationSystem.SendNotificationToPlayerIdentityExtended(NULL, 10, "FelixBot", Message);
	
	
	Param1<string> Msgparam;
	if (Message != "")
	{
	
		if (Player && Player.IsAlive() && Player.GetIdentity())
		{
			NotificationSystem.SendNotificationToPlayerIdentityExtended(Player.GetIdentity(), 5, Message);
			Msgparam = new Param1<string>(Message); //For single player!
			GetGame().RPCSingleParam(Player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, Player.GetIdentity());
		}
		else
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(FBSCM);
		}
	}
	else
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(FBSCM);
	}
}

void FBResetCollision(Object object)
{
	if (object)
	{
		vector roll = object.GetOrientation();
		object.SetOrientation(roll);
		roll[2] = roll[2] - 1;
		object.SetOrientation(roll);
		roll[2] = roll[2] + 1;
		object.SetOrientation(roll);
	}
}


vector FBCreateEntity(ref PlayerBase player, string className, int count, int quantity, float metrage, string type, array<ref FBPromoKitItem> items, vector OldPosition)
{

	EntityAI itemEntity;
	ItemBase itemBase;
	vector position = OldPosition;
	bool isAi = GetGame().IsKindOf( className, "DZ_LightAI" ) || GetGame().IsKindOf(className, "SurvivorBase");

	if (type == "give_on_ground") {

		position[0] = OldPosition[0] + metrage;
		position[2] = OldPosition[2];
		position[1] = OldPosition[1];

		for (int i = 0; i < count; i++) {

			Object object = GetGame().CreateObject(className, position, false, isAi);
			FBResetCollision(object);

			itemEntity = EntityAI.Cast(object);


			if (itemEntity == NULL) {}
			else {
				itemEntity.PlaceOnSurface();
				itemEntity.SetSynchDirty();

				if (itemEntity.IsInherited(ItemBase) && !isAi)
				{
					itemBase = ItemBase.Cast(itemEntity);
					itemBase.SetupSpawnedItem(itemBase, itemEntity.GetHealth("",""), quantity);
				}
			}
		}
	}
	else if (type == "give_in_inv")
	{

		for (int ii = 0; ii < count; ii++) {
			itemEntity = player.GetInventory().CreateInInventory(className);
			if (itemEntity.IsInherited(ItemBase) && !isAi)
			{
				itemBase = ItemBase.Cast(itemEntity);
				itemBase.SetupSpawnedItem(itemBase, itemEntity.GetHealth("",""), quantity);
			}
		}
	}

	else if (type == "give_kit_on_ground")
	{

		position[0] = OldPosition[0] + metrage;
		position[2] = OldPosition[2];
		position[1] = OldPosition[1];

		for (int iii = 0; iii < count; iii++) {

			Object object1 = GetGame().CreateObject(className, position, false, isAi);
			FBResetCollision(object1);

			itemEntity = EntityAI.Cast(object1);


			if (itemEntity == NULL) {}
			else {
				foreach(ref FBPromoKitItem c : items) {
					itemEntity.GetInventory().CreateAttachment(c.value);
				}

				if(GetGame().IsKindOf(className, "Car")) {
					FBAddFuelToCar(itemEntity);
				}

				itemEntity.PlaceOnSurface();
				itemEntity.SetSynchDirty();

				if (itemEntity.IsInherited(ItemBase) && !isAi)
				{
					itemBase = ItemBase.Cast(itemEntity);
					itemBase.SetupSpawnedItem(itemBase, itemEntity.GetHealth("",""), quantity);
				}
				
			}
		}
	}


	else if (type == "give_kit_in_inv")
	{

		for (int iiii = 0; iiii < count; iiii++) {
			itemEntity = player.GetInventory().CreateInInventory(className);
			if (itemEntity)
			{
				foreach(ref FBPromoKitItem cc : items) {
					itemEntity.GetInventory().CreateAttachment(cc.value);
				}
				itemBase = ItemBase.Cast(itemEntity);
				itemBase.SetQuantity(1);

				if (itemEntity.IsInherited(ItemBase) && !isAi)
				{
					itemBase = ItemBase.Cast(itemEntity);
					itemBase.SetupSpawnedItem(itemBase, itemEntity.GetHealth("",""), quantity);
				}
			}
		}
	}

	return position;
}


void FBAddFuelToCar(EntityAI carEntity)
{
	Car vehicle = Car.Cast(carEntity); 
    if ( vehicle == NULL || carEntity == NULL)
    	return;
		
	carEntity.SetHealthMax("", "Health");
	carEntity.SetHealthMax();
	CarScript.Cast(vehicle).RefillAllLiquids();
}

void usagePromocode(ref PlayerBase player, string code) {
	ref PlayerIdentity playerIdentity = PlayerBase.Cast(player).GetIdentity();

	string request_json, response_raw, error;
	ref FelixBotAPI api = GetDayZGame().GetFelixBotAPI();

	FBPromoUsage request = new FBPromoUsage(GetDayZGame().GetFBCFG(), api.version, code, playerIdentity.GetPlainId());
	api.json.WriteToString(request, false, request_json);

	api.get_context().SetHeader("application/json");
	response_raw = api.get_context().POST_now("/methods/usagePromocode", request_json);

	ref FBPromoUsageResponse response = new FBPromoUsageResponse;
	api.json.ReadFromString(response, response_raw, error);

	switch (response.error)
	{
		case 1: {
			FBSCM(player, "FelixBot: Promocode not found!");
		}
		break;
		case 2: {
			FBSCM(player, "FelixBot: Promocode was used!");
		}
		break;
		case 3: {
			FBSCM(player, "FelixBot: Promocode has been used the maximum number!");
		}
		break;
		default:
		{
			vector pos = player.GetPosition();
			foreach(ref FBPromoItem tmp : response.data) {
				pos = FBCreateEntity(player, tmp.className, tmp.count, tmp.quantity, tmp.metrage, tmp.type, tmp.items, pos);
			}
			FBSCM(player, "FelixBot: All is ready!");
		}
		break;
	}

	//FBSCM(player, "Code: " + response.error);
	//FBSCM(player, "Message: " + response.message);


	delete response.data;
	delete response;

};