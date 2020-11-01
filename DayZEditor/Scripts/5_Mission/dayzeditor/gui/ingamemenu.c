

modded class InGameMenu
{
	override Widget Init()
	{		
		layoutRoot = super.Init();
		
		if (!IsMissionOffline()) {
			return layoutRoot;
		}

		//m_RespawnButton.Show(false);
		
		ButtonSetText(m_RestartButton, "CHANGE MAP");
		ButtonSetText(m_RespawnButton, "OPEN FILE");
		ButtonSetText(m_ContinueButton, "CONTINUE");
		
		WrapSpacerWidget top = WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("top"));		
		top.AddChildAfter(m_OptionsButton, m_RestartButton);
		
		ImageWidget dayz_logo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
		dayz_logo.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		dayz_logo.SetImage(0);
		dayz_logo.SetFlags(dayz_logo.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		
		return layoutRoot;
	}
	
	
	override void UpdateGUI() 
	{
		if (!IsMissionOffline())
			super.UpdateGUI();
	}
	
	
	override void OnClick_Restart()
	{
		if (!IsMissionOffline())
			return;

		
		EditorLoadMapCommand cmd = new EditorLoadMapCommand();
		cmd.Execute(this, null);
	}
	
	override void OnClick_Respawn()
	{
		if (!GetEditor()) {
			super.OnClick_Respawn();
			return;
		}
		
		GetEditor().CommandManager.OpenCommand.Execute(this, null);
	}
	
	override void OnClick_Exit()
	{
		if (!GetEditor()) {
			super.OnClick_Exit();
			return;
		}
		
		GetEditor().CommandManager.ExitCommand.Execute(this, null);
	}
	
	
}