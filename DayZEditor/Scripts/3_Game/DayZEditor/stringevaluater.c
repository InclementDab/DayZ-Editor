class StringEvaluaterEvaluater
{
	private string m_Value;
	
	float Parse(string value)
	{
		m_Value = value;
		int pos = -1;
		int ch = 0;
		NextChar(pos, ch);
		float x = ParseExpression(pos, ch);
		if (pos < value.Length()) {
			Error("Unexpected: " + ch + " " + value);
			return -1;
		}
		
		return x;
	}
	
	private void NextChar(out int pos, out int ch) 
	{
		pos++;
		if (pos < m_Value.Length()) {
			ch = m_Value[pos].Hash();
		} else {
			ch = -1;
		}
	}
	
	private bool Eat(int charToEat, out int pos, out int ch) 
	{
	    while (ch == 32) NextChar(pos, ch);
	    if (ch == charToEat) {
	        NextChar(pos, ch);
	        return true;
	    }
	    return false;
	}
	
	private float ParseExpression(out int pos, out int ch) 
	{
	    float x = ParseTerm(pos, ch);
	    while (!false) {
	        if      (Eat("+".Hash(), pos, ch)) x += ParseTerm(pos, ch); // addition
	        else if (Eat("-".Hash(), pos, ch)) x -= ParseTerm(pos, ch); // subtraction
	        else return x;
	    }
		
		return x;
	}
	
	private float ParseTerm(out int pos, out int ch) 
	{
	    float x = ParseFactor(pos, ch);
	    while (true) {
	        if      (Eat("*".Hash(), pos, ch)) x *= ParseFactor(pos, ch); // multiplication
	        else if (Eat("/".Hash(), pos, ch)) x /= ParseFactor(pos, ch); // division
	        else return x;
	    }
		
		return x;
	}
	
	private float ParseFactor(out int pos, out int ch) 
	{
	    if (Eat("+".Hash(), pos, ch)) return ParseFactor(pos, ch); // unary plus
	    if (Eat("-".Hash(), pos, ch)) return -ParseFactor(pos, ch); // unary minus
	
	    float x;
	    int startPos = pos;
	    if (Eat("(".Hash(), pos, ch)) { // parentheses
	        x = ParseExpression(pos, ch);
	        Eat(")".Hash(), pos, ch);
	    } else if ((ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash()) { // numbers
	        while ((ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash()) 
				NextChar(pos, ch);

	        x = (m_Value.Substring(startPos, pos - startPos)).ToFloat();
	    } else if (ch >= "a".Hash() && ch <= "z".Hash()) { // functions
	        while (ch >= "a".Hash() && ch <= "z".Hash()) NextChar(pos, ch);
	        string fnc = m_Value.Substring(startPos, pos - startPos);
	        x = ParseFactor(pos, ch);
	        if (fnc == "sqrt") x = Math.Sqrt(x);
	        else if (fnc == "sin") x = Math.Sin(x * Math.DEG2RAD);
	        else if (fnc == "cos") x = Math.Cos(x * Math.DEG2RAD);
	        else if (fnc == "tan") x = Math.Tan(x * Math.DEG2RAD);
	        else Error("Unknown function: " + fnc);
	    } else {
	        //Error("Unexpected: " + ch);
	    }
	
	    if (Eat("^".Hash(), pos, ch)) x = Math.Pow(x, ParseFactor(pos, ch)); // exponentiation
	
	    return x;
	}
}


typedef string StringEvaluater;
class StringEvaluater: string
{	
	float Parse()
	{
		StringEvaluaterEvaluater evaluater = new StringEvaluaterEvaluater();
		return evaluater.Parse(value);
	}	
}