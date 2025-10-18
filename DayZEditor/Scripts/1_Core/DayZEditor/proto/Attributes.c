class RegisterBrush: Class
{
	static ref array<ref Param3<typename, string, bool>> Instances = {};
	
	void RegisterBrush(typename brush, string name = "Default Brush", bool show_in_list = true)
	{
		if (!Instances) {
			Instances = {};
		}
				
		Instances.Insert(new Param3<typename, string, bool>(brush, name, show_in_list));
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