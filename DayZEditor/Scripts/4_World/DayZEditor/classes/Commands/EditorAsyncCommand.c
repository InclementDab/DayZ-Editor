class EditorAsyncCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args) 
	{
		super.Execute(sender, args);
		thread Call(sender, args);
		return true;
	}
	
	void Call(Class sender, CommandArgs args);
}