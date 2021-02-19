class EditorScriptEditorCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorMultilineEditBoxDialog edit_dialog = new EditorMultilineEditBoxDialog("Script Console");
		
		string script_test;
		edit_dialog.ShowDialog(script_test);
		
		string file_name = "$profile:Editor/test.c";
		FileHandle handle = OpenFile(file_name, FileMode.WRITE);
		FPrintln(handle, "static void main() {");
		FPrintln(handle, script_test);
		FPrintln(handle, "}");
		
		if (handle) {
			CloseFile(handle);
		}
		
		ScriptModule script_module = ScriptModule.LoadScript(GetGame().GetMission().MissionScript, file_name, true);
		if (!script_module) {
			MessageBox.Show("Error", "Invalid Syntax in Script Editor", MessageBoxButtons.OK);
			return;
		}
		
		script_module.Call(null, "main", null);
	}
	
	override string GetName() 
	{
		return "Script Editor";
	}
}