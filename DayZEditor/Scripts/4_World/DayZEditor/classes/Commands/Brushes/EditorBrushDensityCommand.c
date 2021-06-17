class EditorBrushDensityCommand: EditorCommand
{
	override void CanExecuteChanged(bool state)
	{
		if (m_ViewBinding) {
			Widget root = m_ViewBinding.GetLayoutRoot().GetParent();
			root.Show(state);
			root.Enable(state);			
		}
	}
	
	override string GetName() 
	{
		return "Brush Density";
	}
}