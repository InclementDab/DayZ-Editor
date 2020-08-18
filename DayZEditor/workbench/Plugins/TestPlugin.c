


[WorkbenchPluginAttribute("Tylers test plugin", "Hello world!", "", "")]
class TestPlugin: WorkbenchPlugin
{
	
	override void Run()
	{
		
	
	}
	
	override void Configure()
	{
		
		Print(Workbench.OpenModule("TestEditor"));
		
		TestEditor editor = Workbench.GetModule("TestEditor");
		BaseContainer container = editor.GetContainer();
		
		Print(editor);
		Print(container);
		
		//Workbench.ScriptDialog("Configure My Ass", string.Format("Test %1 \n %2", t), this);
	}
	
	[ButtonAttribute("TestButton")]
	void Test()
	{
		
	}
};

