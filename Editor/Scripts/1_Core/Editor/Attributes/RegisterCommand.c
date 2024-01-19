class RegisterCommand
{
	static ref array<typename> Instances = {};
	
	void RegisterCommand(typename type)
	{				
		Instances.Insert(type);
	}
}