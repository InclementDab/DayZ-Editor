class RegisterBrush: Class
{
	static ref array<ref Param2<typename, string>> Instances = {};
	
	void RegisterBrush(typename brush, string name = "Default Brush")
	{
		if (!Instances) {
			Instances = {};
		}
				
		Instances.Insert(new Param2<typename, string>(brush, name));
	}	
}

class RegisterEditorCommand: Class
{
	static ref array<typename> Instances = {};
	
	void RegisterEditorCommand(typename brush)
	{
		if (!Instances) {
			Instances = {};
		}
			
		Instances.Insert(brush);
	}		
}