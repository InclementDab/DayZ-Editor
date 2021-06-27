class EditorMath
{
	static float RGBtoHue(float r, float b, float g)
	{ 
        // h, s, v = hue, saturation, value
        float cmax = Math.Max(r, Math.Max(g, b)); // maximum of r, g, b
        float cmin = Math.Min(r, Math.Min(g, b)); // minimum of r, g, b
        float diff = cmax - cmin; // diff of cmax and cmin.
        float h = -1, s = -1;
         
        // if cmax and cmax are equal then h = 0
        if (cmax == cmin) {
            return 0;
		}
 
        // if cmax equal r then compute h
        else if (cmax == r) {
			return Remainder(60 * ((g - b) / diff) + 360, 360);
		}
 
        // if cmax equal g then compute h
        else if (cmax == g) {
			return Remainder(60 * ((b - r) / diff) + 120, 360);
		}
 
        // if cmax equal b then compute h
        else if (cmax == b) {
			return Remainder(60 * ((r - g) / diff) + 240, 360);
		}
		
		return 0;
	}
	
	static vector HSVtoRGB(float h, float s, float v)
	{
	    /*if (h > 360 || h < 0 || s > 100 || s < 0 || v > 100 || v < 0) {
	        return;
	    }*/
				
		s /= 100;
		v /= 100;
	    float c = s * v;
		float xx = Remainder(h / 60, 2);
	    float x = c * (1 - Math.AbsFloat(xx - 1));
		Print(x);
	    float m = v - c;
	    if (h >= 0 && h < 60) {
	        rgb[0] = c; rgb[1] = x; rgb[2] = 0;
	    }
	    else if (h >= 60 && h < 120) {
	        rgb[0] = x; rgb[1] = c; rgb[2] = 0;
	    }
	    else if (h >= 120 && h < 180) {
	        rgb[0] = 0; rgb[1] = c; rgb[2] = x;
	    }
	    else if (h >= 180 && h < 240) {
	        rgb[0] = 0; rgb[1] = x; rgb[2] = c;
	    }
	    else if (h >= 240 && h < 300) {
	        rgb[0] = x; rgb[1] = 0; rgb[2] = c;
	    }
	    else {
	        rgb[0] = c; rgb[1] = 0; rgb[2] = x;
	    }
		
		for (int i = 0; i < 3; i++) {
			rgb[i] = (rgb[i] + m);
		}
	}
	
	static float Remainder(float num, float divisor)
	{
	    return (num - divisor * (num / divisor));
	}
	
	static float SmoothLerp(float a, float b, float t)
	{
		return (b - a) * SmoothTime(t) + a;
	}
	
	static float SmoothTime(float t)
	{
		return (-Math.Cos(Math.PI * t) / 2) + 0.5;
	}
	
	static vector CalculateCubicBezierPoint(float t, vector p0, vector p1, vector p2, vector p3)
	{
	    float u = 1 - t;
	    float tt = t * t;
	    float uu = u * u;
	    float uuu = uu * u;
	    float ttt = tt * t;
	    
	    vector p = uuu * p0; 
	    p += 3 * uu * t * p1; 
	    p += 3 * u * tt * p2; 
	    p += ttt * p3; 
	    
	    return p;
	}
	
	static vector CalculateQuadraticBezierPoint(float t, vector p0, vector p1, vector p2)
	{
	    float u = 1 - t;
	    float tt = t * t;
	    float uu = u * u;
	    
	    vector p = uu * p0; 
	    p += 2 * u * t * p1; 
	    p += tt * p2; 
	    
	    return p;
	}
		
	static void LerpBezier(float duration, vector p0, vector p1, vector p2, vector p3)
	{
		Debug.DestroyAllShapes();
		Debug.DrawSphere(p0);
		Debug.DrawSphere(p1);
		Debug.DrawSphere(p2);
		Debug.DrawSphere(p3);
		vector mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			vector.Zero
		};
		
		Shape shape = Debug.DrawSphere(vector.Zero, 1, COLOR_YELLOW);
		int i = 0;
		while (i < duration * 1000) {
			
			mat[3] = CalculateCubicBezierPoint(SmoothTime((1 / duration) * i / 1000), p0, p1, p2, p3);
			shape.SetMatrix(mat);
			
			Sleep(10);
			i += 10;
		}
	}

	static void LerpBezier(float duration, vector p0, vector p1, vector p2)
	{
		Debug.DestroyAllShapes();
		Debug.DrawSphere(p0);
		Debug.DrawSphere(p1);
		Debug.DrawSphere(p2);
		
		vector mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			p0
		};
		
		Shape shape = Debug.DrawSphere(vector.Zero, 1, COLOR_YELLOW);
		int i = 0;
		
		Camera old = Camera.GetCurrentCamera();
		Camera c = Camera.Cast(GetGame().CreateObject("staticcamera", p0));
		c.SetActive(true);
		
		while (i < duration * 1000) {
			
			vector pos_old = mat[3];
			
			//Camera.InterpolateTo(Camera.GetCurrentCamera(), 5, 0);
			
			
			mat[3] = CalculateQuadraticBezierPoint((1 / duration) * i / 1000, p0, p1, p2);
			//shape.SetMatrix(mat);
			c.MoveInTime(mat, 10);
			c.LookAt(GetGame().GetPlayer().GetPosition());
			
			Debug.DrawLine(pos_old, mat[3]);
			
			Sleep(10);
			i += 10;
		}
		
		c.SetActive(false);
	}
	
	static vector LerpVector(vector p1, vector p2, float time)
	{
		vector v1;
		for (int i = 0; i < 3; i++) {
			v1[i] = Math.Lerp(p1[i], p2[i], time);
		}
		
		return v1;
	}
	
	static vector SmoothLerpVector(vector p1, vector p2, float time)
	{
		vector v1;
		for (int i = 0; i < 3; i++) {
			v1[i] = SmoothLerp(p1[i], p2[i], time);
		}
		
		return v1;
	}
	
	// vector::RotateAroundPoint is broken
	static vector RotateAroundPoint(vector point, vector pos, vector axis, float cosAngle, float sinAngle)
    {
        return vector.RotateAroundZero(pos - point, axis, cosAngle, sinAngle) + point;
    }
}

ref BezierDraw m_BezierDraw;
class BezierDraw
{
	ref OLinkT m_LinkRef0;
	ref OLinkT m_LinkRef1;
	ref OLinkT m_LinkRef2;
	
	ref Timer m_Timer = new Timer(CALL_CATEGORY_SYSTEM);

	void BezierDraw(Object obj0, Object obj1, Object obj2)
	{
		m_LinkRef0 = new OLinkT(obj0);
		m_LinkRef1 = new OLinkT(obj1);
		m_LinkRef2 = new OLinkT(obj2);
		
		m_Timer.Run(0.01, this, "RunBezierTimer", null, true);
	}

	void RunBezierTimer()
	{
		Debug.DestroyAllShapes();
		float i;
		vector point = m_LinkRef0.Ptr().GetPosition();
		vector old_point;
		while (i <= 1.0) {
			
			old_point = point;
			point = EditorMath.CalculateQuadraticBezierPoint(i, m_LinkRef0.Ptr().GetPosition(), m_LinkRef1.Ptr().GetPosition(), m_LinkRef2.Ptr().GetPosition());
			
			Debug.DrawLine(old_point, point, COLOR_WHITE, ShapeFlags.NOZBUFFER);
			
			vector g0, g1;
			
			g0 = old_point;
			g1 = point;
			
			g0[1] = GetGame().SurfaceY(g0[0], g0[2]);
			g1[1] = GetGame().SurfaceY(g1[0], g1[2]);
						
			Debug.DrawLine(g0, g1, COLOR_RED);
			
			Debug.DrawLine(old_point, g0);
			Debug.DrawLine(point, g1);
			
			i += 0.01;
		}
	}
}
