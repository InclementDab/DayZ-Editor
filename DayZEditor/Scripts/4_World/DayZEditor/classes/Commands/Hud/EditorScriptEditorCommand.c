class EditorScriptEditorCommand: EditorAsyncCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorScriptEditorDialog edit_dialog = new EditorScriptEditorDialog(GetName());
		
		string script_content;
		edit_dialog.ShowDialog(script_content);
		
		string sanitized_content;
		for (int i = 0; i < script_content.Length(); i++) {
			if (script_content[i] == "\n") {
				continue;
			}
			
			sanitized_content += script_content[i];
		}
		
		string file_name = "$saves:_.c";
		FileHandle handle = OpenFile(file_name, FileMode.WRITE);
		string file_data = "static void main()\n{\n" + sanitized_content + "\n}";
		FPrintln(handle, file_data);		
		
		if (handle) {
			CloseFile(handle);
		}
		
		ScriptModule script_module = ScriptModule.LoadScript(GetGame().GetMission().MissionScript, file_name, true);
		if (!script_module) {
			MessageBox.Show("Error", "Invalid Syntax in Script Editor", MessageBoxButtons.OK);
			return;
		}
		
		script_module.CallFunction(null, "main", null, null);
		
		DeleteFile(file_name);
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_SCRIPT_EDITOR";
	}
}