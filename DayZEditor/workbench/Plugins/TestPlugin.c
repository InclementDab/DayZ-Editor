
[WorkbenchPluginAttribute("Tylers test plugin", "Hello world!")]
class TestPlugin: WorkbenchPlugin
{
	
	override void Run()
	{
		WBModuleDef m = Workbench.GetModule("TestEditor");
		//Workbench.ScriptDialog("hii", "test", m);
	}
	
	
};


typedef int[] TestEditor;
class TestEditor: ScriptEditor
{
	
	void TestEditor()
	{
		Print("wtf that worked");
	}
	
	
};