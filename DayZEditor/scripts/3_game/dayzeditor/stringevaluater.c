


class StringEvaluater
{
	int pos = -1;
	int ch;
	string value;
	
	float Parse(string _value)
	{
		pos = -1;
		value = _value;
		NextChar();
		float x = ParseExpression();
		if (pos < value.Length()) Error("Unexpected: " + ch);
		return x;
	}
	
	void NextChar() 
	{
		pos++;
		if (pos < value.Length()) {
			ch = value.Get(pos).Hash();
		} else {
			ch = -1;
		}
	}
	
	bool Eat(int charToEat) 
	{
	    while (ch == 32) NextChar();
	    if (ch == charToEat) {
	        NextChar();
	        return true;
	    }
	    return false;
	}
	
	float ParseExpression() 
	{
	    float x = ParseTerm();
	    while (!false) {
	        if      (Eat("+".Hash())) x += ParseTerm(); // addition
	        else if (Eat("-".Hash())) x -= ParseTerm(); // subtraction
	        else return x;
	    }
		
		return x;
	}
	
	float ParseTerm() 
	{
	    float x = ParseFactor();
	    while (true) {
	        if      (Eat("*".Hash())) x *= ParseFactor(); // multiplication
	        else if (Eat("/".Hash())) x /= ParseFactor(); // division
	        else return x;
	    }
		
		return x;
	}
	
	float ParseFactor() 
	{
	    if (Eat("+".Hash())) return ParseFactor(); // unary plus
	    if (Eat("-".Hash())) return -ParseFactor(); // unary minus
	
	    float x;
	    int startPos = pos;
	    if (Eat("(".Hash())) { // parentheses
	        x = ParseExpression();
	        Eat(")".Hash());
	    } else if ((ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash()) { // numbers
	        while ((ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash()) 
				NextChar();

	        x = (value.Substring(startPos, pos - startPos)).ToFloat();
	    } else if (ch >= "a".Hash() && ch <= "z".Hash()) { // functions
	        while (ch >= "a".Hash() && ch <= "z".Hash()) NextChar();
	        string fnc = value.Substring(startPos, pos - startPos);
	        x = ParseFactor();
	        if (fnc == "sqrt") x = Math.Sqrt(x);
	        else if (fnc == "sin") x = Math.Sin(x * Math.DEG2RAD);
	        else if (fnc == "cos") x = Math.Cos(x * Math.DEG2RAD);
	        else if (fnc == "tan") x = Math.Tan(x * Math.DEG2RAD);
	        else Error("Unknown function: " + fnc);
	    } else {
	        //Error("Unexpected: " + ch);
	    }
	
	    if (Eat("^".Hash())) x = Math.Pow(x, ParseFactor()); // exponentiation
	
	    return x;
	}
}