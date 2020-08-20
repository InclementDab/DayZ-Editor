
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


[WorkbenchPluginAttribute("MVC Controller Manager", "Edit MVC Settings", "Alt+3", "", {"ResourceManager", "ScriptEditor"})]
class MVCEditorPlugin: WorkbenchPlugin
{
	// vector3, button, editboxWithButton
	string m_Shortcut;
	ref array<string> m_WBModules;
	
	///< can be "editbox", "combobox", "spinbox", "slider", "font", "fileeditbox", "colorPicker", "flags", "resourceNamePicker"
	[Attribute("", "vector3")]
	vector DataBindingName;
	
	
	
	
	[EditorAttribute("box", "StaticEntities/Walls", "testdesc", Vector(-5, -5, -5), Vector(-5, -5, -5), "255 255 255 255")]
	int test;	
	
	static ref ParamEnumArray param_enums = { ParamEnum("TestButton", "0"), ParamEnum("Option2", "1") };
	[Attribute("Test", "PopupComboBox", "combobox: ", "TestButton", param_enums)]
	string TestButton;

	
	[Attribute("", "PopupComboBox")]
	int spinbox;
	
	
	
	void MVCEditorPlugin()
	{
		Print("MVCEditorPlugin");
		CorePluginCreatedTrigger(this);
	}
	
	void ~MVCEditorPlugin()
	{
		Print("~MVCEditorPlugin");
		Widget w = GetFocus();
		Print(w);
	}
		
	
	
	override void Run()
	{
		
		//WBModuleDef module = Workbench.GetModule("ParticleEditor");
		Workbench.OpenModule("WorldEditor");
		WBModuleDef module = Workbench.GetModule("WorldEditor");
		
		Print(module);
		Print(module.GetNumContainers());
		
		BetterString test = "test";
		test[1] = "f";
		Print(test);
		
		
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
	
}

