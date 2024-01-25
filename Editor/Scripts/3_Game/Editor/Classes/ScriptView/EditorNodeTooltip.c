class EditorNodeTooltip: ScriptView
{
	ImageWidget Icon;
	TextWidget Type, Name;

	void EditorNodeTooltip(string type, string name, Symbols icon)
	{
		Type.SetText(type);
		Name.SetText(name);
		
		Icon.LoadImageFile(0, icon.Solid());
		Icon.SetImage(0);
	}
	
	static EditorNodeTooltip CreateOnWidget(string type, string name, Symbols icon, Widget widget, TooltipPositions position)
	{
		float pos_x, pos_y;
		float b_x, b_y, b_w, b_h;
		widget.GetScreenPos(b_x, b_y);
		widget.GetScreenSize(b_w, b_h);
		
		float w, h;
		EditorNodeTooltip node_tooltip = new EditorNodeTooltip(type, name, icon);
		node_tooltip.GetLayoutRoot().GetScreenSize(w, h);
		
		switch (position) {
			case TooltipPositions.BOTTOM_LEFT: {
				pos_x = b_x;
				pos_y = b_y + b_h;
				break;
			}		
			
			case TooltipPositions.TOP_LEFT: {
				pos_x = b_x;
				pos_y = b_y - h;
				break;
			}
			
			case TooltipPositions.BOTTOM_RIGHT: {
				pos_x = b_x + b_w;
				pos_y = b_y + b_h;
				break;
			}
			
			case TooltipPositions.TOP_RIGHT: {
				pos_x = b_x + b_w;
				pos_y = b_y - h;
				break;
			}
		}
		
		node_tooltip.GetLayoutRoot().SetPos(pos_x, pos_y);
		return node_tooltip;
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Tooltip.layout";
	}
}