modded class DayZGame {
    private ref FelixBotAPI m_felix_bot_api;

    private int m_tps = 0;
    private int m_tps_time = 0;

    private int m_ticks = 0;

    private ref FelixBotConfig m_felix_bot_config = new FelixBotConfig;

    FelixBotAPI GetFelixBotAPI() {
        return m_felix_bot_api;
    }

    void SetupFelixBotAPI() {
        m_felix_bot_api = new FelixBotAPI(m_felix_bot_config);
    }

    FelixBotConfig GetFBCFG() {
        return m_felix_bot_config;
    }

    int GetServerFPS() {
        return this.m_tps;
    }

    override void OnUpdate(bool doSim, float timeslice) {
        super.OnUpdate(doSim, timeslice);
        this.m_ticks++;
        if (this.m_tps_time + 1 < GetGame().GetTickTime()) {
            this.m_tps_time = GetGame().GetTickTime();
            this.m_tps = this.m_ticks / 2;
            this.m_ticks = 0;
        }
    }


	void felix_debug(string text)
	{
		string dir_log_Location = "logs";
		string file_name = "felixbot.txt";

		private string file_log = file_name;
		private FileHandle file;
		file = OpenFile(file_log, FileMode.WRITE);
        FPrintln(file, text);
		CloseFile(file);
	};
};

