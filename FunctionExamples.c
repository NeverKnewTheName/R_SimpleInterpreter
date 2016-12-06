Integer Func(Integer a)
{
	a++;
	a++;
	a++;
	return a;
}


Integer Func()
{
    Integer a;
    a = 5;
    a++;
    return a;
}

Integer Func()
{
    Integer a = 5;
    a = a + 5;
    return a;
}

Integer Func()
{
    Integer a = 5;
    Double b = 4.2;
    a = b = a + b;
    return b;
}

Integer Func()
{
    Integer a = 5;
    Integer b = 4;
    a = b = a + b;
    return a;
}

String Func()
{
    Integer a = 5;
    Integer b = 4;
    a = b = a + b;
    return "a: " + a + " b: " + b + " a + b = " + (a+b);
}


Integer intA = 5;
Integer FuncA(Integer a)
{
    return a++;
};

return FuncA(intA);

Integer FuncA()
{
    return 5;
};
return FuncA();

Integer intA = 5;
String FuncA(Integer a)
{
    String output = "Output: ";
    output + " a: " + a;
    Integer b = 10;
    output + " b: " + b;
    a = 4;
    output + " a: " + a;
    b=6;
    output + " b: " + b;
    return output + " a+b: " + (a + b);
};

return FuncA(intA);


String makeRed(String textToColor)
{
	return "<font color=\"red\">" + textToColor + "</font>";
};

return makeRed("this should be red");

String makeRed(String textToColor)
{
	return "<font color=\"red\">" + textToColor + "</font>";
};

return makeRed("this should be red");


Integer intA = 5;
Integer FuncA(Integer a)
{
    Integer b = 3;
    return a+b;
};

return FuncA(intA);


Integer a = 5;
String func(Integer b)
{
	String out = "out ->";
	Integer a = b;

	out = out + " b: " +b;
	out = out + " a: " +a;
	a = a + 2;
	out = out + " a: " +a;
	
	return out;
};

return func(a);




Integer a = 5;

Integer add(Integer a, Integer b)
{
	return a + b;
};

String func(Integer b)
{
	String out = "out ->";
	Integer a;
	a = b;

	out = out + " b: " +b;
	out = out + " a: " +a;
	a = a + 2;
	out = out + " a: " +a;
	
	return out;
};

return func(add(a, 3));


Integer a = 5;

Integer add(Integer a, Integer c)
{
	Integer b = c;
	return D0 + a + b;
};

String func(Integer b)
{
	String out = "out ->";
	Integer a;
	a = b;

	out = out + " D0: " + D0;
	out = out + " D3: " + D2;
	out = out + " D4: " + D3;

	out = out + " b: " +b;
	out = out + " a: " +a;
	a = a + 2;
	out = out + " a: " +a;
	out = out + " D2 + D3: " + (D2 + D3);
	return out;
};

return func(add(a, 3));


Integer a;

Integer add(Integer d Integer c)
{
	Integer b = c;
	a = 55;
	return d + c;
};

String func(Integer b)
{
	String out = "out ->";
	Integer f;
	f = b;

	out = out + " a: " +a;
	a = 10;
	out = out + " a: " +a;
	
	out = out + " b: " +b;
	out = out + " f: " +f;
	f = add(4 2);
	out = out + " f: " +f;
	out = out + " a: " +a;
	return out;
};

return func(add(4 5));





Integer a = 4;

Integer Fak(Integer val)
{
    return ((val > 0) ? val * Fak(val-1) : 1;
}

return Fak(a);