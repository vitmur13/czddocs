void fb_fail_with_error(int error_code, string error_message) {
	GetGame().AdminLog(error_message);
	Print(error_message);
	GetGame().OpenURL("https://felixbot.ru/dzplugin?error_code=" + error_code);
};
void fb_warn_with_error(int error_code, string error_message) {
	GetGame().AdminLog(error_message);
	Print(error_message);
	GetGame().OpenURL("https://felixbot.ru/dzplugin?error_code=" + error_code);
};