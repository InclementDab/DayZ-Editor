
enum TinyDickJim {
	1,
	2,
	3
};



typedef string BetterString;
class BetterString: string
{
	void Set(int n, string _value)
	{
		string pre = value.Substring(0, n);
		n += 1;
		int length = value.Length() - n;
		string post = value.Substring(n, length);
		value = pre + _value + post;
	}
	
	int Count()
	{
		return value.Length();
	}
	
	array<string> Test()
	{
		return new array<string>;
	}
	
	proto native MapIterator Begin();
	proto native MapIterator End();
	proto native MapIterator Next(MapIterator it);
	//proto int GetIteratorKey(MapIterator it);
}



[WorkbenchPluginAttribute("MVC Controller Manager", "Edit MVC Settings", "Alt+3", "", {"ResourceManager", "ScriptEditor", "TestAddon"})]
class MVCEditorPlugin: WorkbenchPlugin
{
	// vector3, button, editboxWithButton, script
	
	///< can be "editbox", "combobox", "spinbox", "slider", "font", "fileeditbox", "colorPicker", "flags", "resourceNamePicker"
	
	static string name = "script";
	
	[Attribute("", name)]
	string test1;
	
	[Attribute("", name)]
	int test2;
	
	[Attribute("", name)]
	float test3;

	
	[Attribute("", name)]
	vector test5;
			
	[Attribute("", name, "", "", ParamEnumArray.FromEnum(EBool))]
	int test7;
	
	[Attribute("1 1 1", "color", "Color", "", NULL )]
	vector Color;
	
	//[Attribute("", "resourceNamePicker", "Model", "xob")]
	//string Model;
	//[Attribute("1", "combobox", "Physics", "", { ParamEnum("None", "2"), ParamEnum("Static", "1"), ParamEnum("Dynamic", "0") } )]
	//int Type;

	[Attribute("false", name)]
	bool Debug;
	

	
	void MVCEditorPlugin()
	{
		Print("MVCEditorPlugin");
	}
	
	void ~MVCEditorPlugin()
	{
		Print("~MVCEditorPlugin");		
	}
		
	
	
	override void Run()
	{
		
		//WBModuleDef module = Workbench.GetModule("ParticleEditor");
		Workbench.OpenModule("TestAddon");
		WBModuleDef module = Workbench.GetModule("TestAddon");
		
		Print(module);
		Print(module.GetNumContainers());
	
		
		
		Print(FileExist("$CurrentDir:/ToolAddons/TestAddon/init.c"));

		
		if (module) {
			Workbench.OpenModule("ResourceManager");
		}

	
		//Workbench.GetModule("ResourceManager").GetNumContainers();
		Workbench.ScriptDialog("Edit View Binding Options", "", this);
	}
	

	
	override void RunCommandline() 
	{
		Print("WTF");
	}
	
	[ButtonAttribute("Close")]
	void Close()
	{
		Print(test1);
	}	
}




class TestingTool
{
	//! Filled by workbench
	WorldEditorAPI m_API;
	
	void TestingTool()
	{
		Print("TestingTool");
		Print(m_API);
	}
}


