

static void CorePluginCreatedTrigger(Class context)
{
	
	g_Script.Call(null, "GamelibPluginCreatedTrigger", context);
	
}


[WorkbenchToolAttribute("REEEEEEE2", "", "Alt+2")]
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
