class RegisterCommand
{
	static ref array<ref Param3<typename, string, string>> Instances = {};
	
	void RegisterCommand(typename type, string display_name, Symbols icon)
	{
		Instances.Insert(new Param3<typename, string, string>(type, display_name, icon));
	}
}