class RegisterCommand
{
	static ref array<ref Param2<typename, string>> Instances = {};
	
	void RegisterCommand(typename type, string display_name)
	{
		Instances.Insert(new Param2<typename, string>(type, display_name));
	}
}