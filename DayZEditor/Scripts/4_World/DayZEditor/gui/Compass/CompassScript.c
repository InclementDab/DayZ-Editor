class CompassScript: ScriptWidgetBase
{
	static const int TICK_PADDING_COUNT = 4;
	static const int TICK_PADDING_COUNT2 = 8;
		
	protected TextWidget m_HeadingWidget, m_HeadingIndicator;
	
	protected Widget m_LeftSpacer, m_RightSpacer;
	
	protected ref map<int, ref CompassTickView> m_CompassTickViews = new map<int, ref CompassTickView>();
	protected int m_CardinalFacingDirection;
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		m_HeadingWidget = TextWidget.Cast(w.FindAnyWidget("CompassText"));
		m_LeftSpacer = w.FindAnyWidget("CompassLeftSpacer");
		m_RightSpacer = w.FindAnyWidget("CompassRightSpacer");
		m_HeadingIndicator = TextWidget.Cast(w.FindAnyWidget("CompassIndicator"));
		
		if (!m_HeadingWidget) {
			ErrorEx("No heading widget found");
		}		
	}
	
	protected override void Update(float timeslice)
	{		
		//m_LayoutRoot.Show(GetGame().GetMission().GetHud().IsVisible() && profile_settings.ShowCompass);
		
		vector camera_direction = GetGame().GetCurrentCameraDirection();
		float heading_angle = Math.Atan2(camera_direction[2], camera_direction[0]) - Math.PI_HALF;

		// value from -1 to 1
		float heading_angle_degrees = (Math.PI2 - heading_angle) * Math.RAD2DEG;
		
		if (heading_angle_degrees < 0) {
			heading_angle_degrees += 360;
		}
		
		if (heading_angle_degrees > 360) {
			heading_angle_degrees -= 360;
		}
		
		float heading_angle_dir = (Math.PI2 - heading_angle);
		if (heading_angle_dir > Math.PI2) {
			heading_angle_dir -= Math.PI2;
		}
				
		heading_angle_dir /= Math.PI;
		heading_angle_dir *= 4;
		int cardinal_facing_dir = Math.Floor(heading_angle_dir) * 45;
		int cardinal_facing_dir_delta = heading_angle_dir * 45 - cardinal_facing_dir;

        bool show_degrees = GetUApi().GetInputByID(UAZoomIn).LocalValue() && !(GetEditor().GetCameraLockFlags() & ECameraLockFlag.LOCK_MOVE) && !(GetEditor().GetCameraLockFlags() & ECameraLockFlag.LOCK_LOOK);
		if (show_degrees) {
			m_HeadingWidget.SetTextFormat("%1°", (int)heading_angle_degrees);
		} else {
			m_HeadingWidget.SetText(CardinalDirToString(cardinal_facing_dir));
		}
		
		m_HeadingIndicator.Show(false);		
		
		if (m_CardinalFacingDirection != cardinal_facing_dir) {
			m_CardinalFacingDirection = cardinal_facing_dir;
			m_CompassTickViews.Clear();
			
			for (int i = 0; i < TICK_PADDING_COUNT; i++) {
				int cardinal_left = (cardinal_facing_dir - (TICK_PADDING_COUNT - i - 1) * 45) + 360;
				int cardinal_right = cardinal_facing_dir + 45 * (i + 1);
				
				cardinal_right = Math.Rollover(cardinal_right, 0, 360);
				cardinal_left = Math.Rollover(cardinal_left, 0, 360);
			
				if (m_CompassTickViews[cardinal_left] || m_CompassTickViews[cardinal_right]) {
					//Print(cardinal_left);
				}
				
				m_CompassTickViews[cardinal_left] = new CompassTickView(cardinal_left, -1);
				m_LeftSpacer.AddChild(m_CompassTickViews[cardinal_left].GetLayoutRoot());
			
				m_CompassTickViews[cardinal_right] = new CompassTickView(cardinal_right, 1);
				m_RightSpacer.AddChild(m_CompassTickViews[cardinal_right].GetLayoutRoot());
			}
		}

		float l = 0, r = 0;
		Widget left_outside = m_LeftSpacer.GetChildren();
		Widget left_inside = m_LeftSpacer.GetChildren();
		if (!left_inside) {
			return;
		}
		
		while (left_inside && left_inside.GetSibling()) {
			l += 1;
			left_inside.SetAlpha(l / TICK_PADDING_COUNT);
			left_inside = left_inside.GetSibling();
		}
		
		Widget right_inside = m_RightSpacer.GetChildren();
		Widget right_outside = m_RightSpacer.GetChildren();
		if (!right_outside) {
			return;
		}
		
		while (right_outside && right_outside.GetSibling()) {
			right_outside = right_outside.GetSibling();
			
			right_outside.SetAlpha(1 - (r / TICK_PADDING_COUNT));
			r++;
		}		
		
		float tick_view_w, tick_view_h;
		left_inside.GetScreenSize(tick_view_w, tick_view_h);

		float tick_view_w_half = tick_view_w / 2;
		
		float dir_delta_amt = cardinal_facing_dir_delta / 45;
		float tick_view_amt_w = dir_delta_amt * tick_view_w;
		
		// alphas for end pieces
		float dir_alpha = Math.Interpolate(dir_delta_amt, 0.05, 0.3, 0, 1);
		float dir_alpha_inv = Math.Interpolate(dir_delta_amt, 0.7, 0.95, 1, 0);
				
		m_LeftSpacer.SetPos(-tick_view_w + tick_view_w_half + tick_view_amt_w, 0);
		left_inside.SetAlpha(dir_alpha);
		//right_outside.SetAlpha(dir_alpha);
		
		right_inside.SetAlpha(dir_alpha_inv);
		//left_outside.SetAlpha(dir_alpha_inv);
		m_RightSpacer.SetPos(tick_view_w - tick_view_w_half - tick_view_amt_w, 0);
	}
		
	static string CardinalDirToString(int cardinal_dir)
	{
		cardinal_dir = Math.Rollover(cardinal_dir, 0, 360);		
		switch (cardinal_dir) {
			case 360:
			case 0: return "N";
			case 45: return "NE";
			case 90: return "E";
			case 135: return "SE";
			case 180: return "S";
			case 225: return "SW";
			case 270: return "W";
			case 315: return "NW";
		}
		
		return string.Empty;
	}
}
class CompassTickView: ScriptView
{
	protected int m_CardinalAngle;
	
	ImageWidget Spacer_Left, Spacer_Right;
	TextWidget Tick;
	
	void CompassTickView(int cardinal_angle, int dir)
	{
		m_CardinalAngle = cardinal_angle;
		string text = CompassScript.CardinalDirToString(m_CardinalAngle);
		Tick.SetText(text);
		Spacer_Right.SetColor(LinearColor.WHITE);
		Spacer_Left.SetColor(LinearColor.WHITE);
		
		Spacer_Right.Show(false);
		Spacer_Left.Show(false);
	}
	
	override void Update(float dt)
	{
	}
	
	int GetCardinalAngle()
	{
		return m_CardinalAngle;
	}
		
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\hud\\day_z_compass_element.layout";
	}
}

enum eCardinalDirection
{
	NORTH,
	NORTH_EAST,
	
	EAST,
	SOUTH_EAST,
	
	SOUTH,
	SOUTH_WEST,
	
	WEST,
	NORTH_WEST
};