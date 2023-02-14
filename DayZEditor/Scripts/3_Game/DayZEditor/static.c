static bool RecursiveGetParent(out Widget w, string name)
{
	if (!w) 
		return false;
	
	if (w.GetName() == name) 
		return true;
	
	w = w.GetParent();
	
	if (w) {
		return RecursiveGetParent(w, name);
	}
	
	return false;
}

static bool RecursiveGetParent(out Widget w, typename type)
{
	if (!w) {
		return false;
	}
	
	if (w.IsInherited(type)) {
		return true;
	}
	
	w = w.GetParent();
	
	if (w) {
		return RecursiveGetParent(w, type);
	}
	
	return false;
}

static Widget GetWidgetRoot(Widget w)
{
	Widget parent = w;
	_GetWidgetRoot(parent);
	return parent;
}

static void _GetWidgetRoot(out Widget w)
{
	if (w.GetParent() == null) {
		return;	
	}
	
	w = w.GetParent();
	_GetWidgetRoot(w);
}

static const int COLOR_SALMON	= ARGB(255, 192, 94, 83);
static const int COLOR_SALMON_A	= ARGB(128, 50, 100, 100);
static const int COLOR_CANDY 	= ARGB(255, 231, 76, 60);
static const int COLOR_APPLE 	= ARGB(255, 106, 176, 76);
static const int COLOR_JELLY 	= ARGB(255, 240, 147, 43);
static const int COLOR_PALE_B   = ARGB(255, 50, 100, 200);
static const int COLOR_BLACK	= ARGB(255, 0, 0, 0);
static const int COLOR_WHITE_A	= ARGB(75, 255, 255, 255);
static const int COLOR_EMPTY 	= ARGB(0, 0, 0, 0);

//CameraTrackColors
static const int COLOR_BLUE_LIGHT		= ARGB(120, 57, 148, 230);
static const int COLOR_BLUE_HIGHLIGHT 	= ARGB(245, 10, 10, 40);
static const int COLOR_BLUE_HIGHLIGHT_OUTLINE 	= ARGB(255, 73, 170, 255);
static const int COLOR_DEFAULT_OUTLINE 	= ARGB(225, 255, 255, 255);
static const int COLOR_DEFAULT_PANEL 	= ARGB(245, 45, 45, 45);
static const int COLOR_DEFAULT_ICON     = ARGB(70, 255, 255, 255);