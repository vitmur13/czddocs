class FBBaseRequestData {
	string api_token;

	void FBBaseRequestData(FelixBotConfig config) {
		api_token = config.api_token;
	}
};

class NewChatMessageData : FBBaseRequestData {
	string message;
	string from;

	void NewChatMessageData(FelixBotConfig config, string message_, string from_) {
		api_token = config.api_token;
		message = message_;
		from = from_;
	}
};

class NewKillFeedMessage : FBBaseRequestData {
	string type;
	string player_name;
	string player_uid;
	string player_pos;
	string killer_name;
	string killer_uid;
	string killer_item;
	int dist;

	void NewKillFeedMessage(FelixBotConfig config, string type_, string player_name_, string player_uid_, string player_pos_, string killer_name_, string killer_uid_, string killer_item_, int dist_) {
		api_token = config.api_token;
		type = type_;
		player_name = player_name_;
		player_uid = player_uid_;
		player_pos = player_pos_;
		killer_name = killer_name_;
		killer_uid = killer_uid_;
		killer_item = killer_item_;
		dist = dist_;
	}
};

class FBNotificationHandleRequest : FBBaseRequestData {

	string from;
	string message;

	void FBNotificationHandleRequest(FelixBotConfig config, string from_, string message_) {
		api_token = config.api_token;

		from = from_;
		message = message_;
	}
};

class FBRaidNotificationHandleRequest : FBBaseRequestData {

	string position;

	void FBRaidNotificationHandleRequest(FelixBotConfig config, string position_) {
		api_token = config.api_token;

		position = position_;
	}
};

class FBServerReportData : FBBaseRequestData {

	float server_fps;
	float server_tick_time;
	int player_count;

	void FBServerReportData(FelixBotConfig config) {
		api_token = config.api_token;

		float fps = GetDayZGame().GetServerFPS();

		this.server_fps = fps;
		this.server_tick_time = GetGame().GetTickTime();



		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		this.player_count = players.Count();
	}
};



class FelixBotVerifyResponse {
	int status;
	string version;
	string error;
};




class FelixBotAPI {
	static const string version = "0.8.0";
	//private static string api_base = "https://salem.felixbot.ru/api/integration/dzplugin";
	private static string api_base = "https://felixbot.ru/api/integration/dzplugin";
	ref JsonSerializer json = new JsonSerializer;
	private RestContext ctx;
	private FelixBotConfig config;

	void FelixBotAPI(FelixBotConfig config_) {
		this.config = config_;
		RestApi c_core = CreateRestApi();
		ctx = c_core.GetRestContext(api_base);
	}

	RestContext get_context() {
		return this.ctx;
	}

	FelixBotConfig get_config() {
		return this.config;
	}

	void newChatMessage(string message, string from) {
		if (message.Contains("!fpromo")) return;

		string request_json, response_raw, error;
		NewChatMessageData request = new NewChatMessageData(this.config, message, from);

		json.WriteToString(request, false, request_json);

		ctx.SetHeader("application/json");
		ctx.POST(new FBHTTPSilentCallBack, "/methods/newChatMessage", request_json);
	}

	void newKillFeedMessage(string type, string player_name, string player_uid, string player_pos, string killer_name, string killer_uid, string killer_item, int dist) {
		string request_json, response_raw, error;
		NewKillFeedMessage request = new NewKillFeedMessage(this.config, type, player_name, player_uid, player_pos, killer_name, killer_uid, killer_item, dist);

		json.WriteToString(request, false, request_json);

		ctx.SetHeader("application/json");
		ctx.POST(new FBHTTPSilentCallBack, "/methods/newKillFeedMessage", request_json);
	}

	void sendServerReport() {

		string request_json, response_raw, error;
		FBServerReportData request = new FBServerReportData(this.config);

		json.WriteToString(request, false, request_json);

		ctx.SetHeader("application/json");
		ctx.POST(new FBHTTPSilentCallBack, "/methods/sendServerReport", request_json);
	
	}


	void verify_credentials() {
		string request_json, response_raw, error;

		FBBaseRequestData request = new FBBaseRequestData(this.config);
		json.WriteToString(request, false, request_json);

		ctx.SetHeader("application/json");
		response_raw = ctx.POST_now("", request_json);

		FelixBotVerifyResponse response = new FelixBotVerifyResponse;
		json.ReadFromString(response, response_raw, error);


		if (response.status != 1) {
			fb_fail_with_error(3, "FelixBotHook could not verify supplied API credentials");
		}
		
		if(response.version != this.version) {
			fb_warn_with_error(4, "FelixBotHook version invalid");
		}
	}

	void FBNotificationMessageHandle(string from, string message) {
		string request_json, response_raw, error;
		ref FelixBotAPI api = GetDayZGame().GetFelixBotAPI();

		FBNotificationHandleRequest request = new FBNotificationHandleRequest(GetDayZGame().GetFBCFG(), from, message);
		api.json.WriteToString(request, false, request_json);

		api.get_context().SetHeader("application/json");
		response_raw = api.get_context().POST_now("/methods/handleNotificationMessage", request_json);
	}

	void FBRaidNotificationMessageHandle(string position) {
		string request_json, response_raw, error;
		ref FelixBotAPI api = GetDayZGame().GetFelixBotAPI();

		FBRaidNotificationHandleRequest request = new FBRaidNotificationHandleRequest(GetDayZGame().GetFBCFG(), position);
		api.json.WriteToString(request, false, request_json);

		api.get_context().SetHeader("application/json");
		response_raw = api.get_context().POST_now("/methods/handleRaidNotificationMessage", request_json);
	}

};

class FBHTTPSilentCallBack : RestCallback
{
	override void OnError(int errorCode) {};
	override void OnTimeout() {};
	override void OnSuccess(string data, int dataSize) {};
};