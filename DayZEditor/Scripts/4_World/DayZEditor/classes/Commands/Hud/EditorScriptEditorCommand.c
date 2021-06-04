class EditorScriptEditorCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorMultilineEditBoxDialog edit_dialog = new EditorMultilineEditBoxDialog("Script Console");
		
		string script_content;
		edit_dialog.ShowDialog(script_content);
		
		string sanitized_content;
		for (int i = 0; i < script_content.Length(); i++) {
			if (script_content[i] == "\n") {
				continue;
			}
			
			sanitized_content += script_content[i];
		}
		
		string file_name = "$profile:Editor/temp.c";
		FileHandle handle = OpenFile(file_name, FileMode.WRITE);
		FPrintln(handle, "static void main() {");
		FPrintln(handle, sanitized_content);
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
		
		DeleteFile(file_name);
	}
	
	override string GetName() 
	{
		return "Script Editor";
	}
}