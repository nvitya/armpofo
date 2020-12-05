// exprcalc.cpp
//
// Expression Calculator

#include "exprcalc.h"
#include "math.h"

int TCalcVar::Add(TCalcVar * avar)
{
	floatvalue = floatvalue + avar->floatvalue;
	return EEXPR_OK;
}

int TCalcVar::Sub(TCalcVar * avar)
{
	floatvalue = floatvalue - avar->floatvalue;
	return EEXPR_OK;
}

int TCalcVar::Mul(TCalcVar * avar)
{
	floatvalue = floatvalue * avar->floatvalue;
	return EEXPR_OK;
}

int TCalcVar::Div(TCalcVar * avar)
{
	if (avar->floatvalue == 0.0l)
	{
		return EEXPR_DIV_BY_ZERO;
	}
	floatvalue = floatvalue / avar->floatvalue;
	return EEXPR_OK;
}

int TCalcVar::Pow(TCalcVar * avar)
{
	floatvalue = pow(floatvalue, avar->floatvalue);
	return EEXPR_OK;
}

int TCalcVar::Assign(TCalcVar * avar)
{
	floatvalue = avar->floatvalue;
	return EEXPR_OK;
}

int TCalcVar::AssignU32(unsigned auint)
{
	floatvalue = auint;
	return EEXPR_OK;
}

int TCalcVar::AssignFloat(double afloat)
{
	floatvalue = afloat;
	return EEXPR_OK;
}

int TCalcVar::Negate()
{
	floatvalue = -floatvalue;
	return EEXPR_OK;
}

//-----------------------------------------------------------------------

int TExprCalc::Evaluate(const char * aexpr, unsigned aexprlen, TCalcVar * rresult)
{
	expression = aexpr;
	exprlen = aexprlen;

	sp.Init((char *)expression, exprlen);

	error = CalcExpression(rresult);
	return error;
}

int TExprCalc::ReadFunctionArgument(TCalcVar * rresult)  // expects '(' expr. ')'
{
	int err = EEXPR_OK;
	sp.SkipSpaces();
	if (!sp.CheckSymbol("("))  return EEXPR_OPENING_BRACKET_MISSING;
	sp.SkipSpaces();
	err = CalcExpression(rresult);
	if (err)  return err;
	sp.SkipSpaces();
	if (!sp.CheckSymbol(")"))  return EEXPR_CLOSING_BRACKET_MISSING;
	sp.SkipSpaces();
  return err;
}

int TExprCalc::ProcessIdentifier(TCalcVar * rresult)  // input in sp.prevptr + sp.prevlen
{
	int       err = EEXPR_OK;
	TCalcVar  arg1;

	if (sp.UCComparePrev("SQRT"))
	{
		err = ReadFunctionArgument(&arg1);
		if (err)  return err;
		rresult->AssignFloat( sqrt(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("EXP"))
	{
		err = ReadFunctionArgument(&arg1);
		if (err)  return err;
		rresult->AssignFloat( exp(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("SIN"))
	{
		err = ReadFunctionArgument(&arg1);
		if (err)  return err;
		rresult->AssignFloat( sin(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("COS"))
	{
		err = ReadFunctionArgument(&arg1);
		if (err)  return err;
		rresult->AssignFloat( cos(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("TAN"))
	{
		err = ReadFunctionArgument(&arg1);
		if (err)  return err;
		rresult->AssignFloat( tan(arg1.floatvalue) );
	}

	// constants
	else if (sp.UCComparePrev("PI"))
	{
		rresult->AssignFloat( M_PI );
	}
	else
	{
		return EEXPR_UNKNOWN_IDENTIFIER;
	}

	return err;
}

int TExprCalc::CalcNumber(TCalcVar * rresult)
{
	TCalcVar  var1;
	int       err = EEXPR_OK;
	bool      negate = false;

	sp.SkipWhite();

	if (sp.CheckSymbol("("))
	{
		err = CalcExpression(&var1);
		if (!sp.CheckSymbol(")"))
		{
			return EEXPR_CLOSING_BRACKET_MISSING;
		}
		rresult->Assign(&var1);
		return err;
	}

	// single constant or variable

	if (sp.CheckSymbol("-"))
	{
		negate = true;
	}

	sp.SkipSpaces();

	char c = *sp.readptr;
	if (((c >= '0') && (c <= '9')) || ('.' == c))
	{
		// number
		if (sp.CheckSymbol("0x"))
		{
			// hex value
			if (!sp.ReadHexNumbers())
			{
				return EEXPR_INVALID_HEX_NUMBER;
			}
			var1.AssignU32(sp.PrevHexToInt());
		}
		else
		{
			var1.AssignFloat(sp.ParseFloatNumber());
		}
	}
	else if (((c >= 'a') && (c <= 'z')) || ((c >= 'a') && (c <= 'z')) || ('_' == c))
	{
		// identifier (variable or function)
		sp.ReadAlphaNum();
		err = ProcessIdentifier(&var1);
		if (err)
		{
			return err;
		}
	}
	else
	{
		return EEXPR_INVALID_CHAR;
	}

	if (negate)
	{
		var1.Negate();
	}

	rresult->Assign(&var1);

	return err;
}

int TExprCalc::CalcFactor(TCalcVar * rresult)
{
	TCalcVar var1;
	TCalcVar var2;
	int err = CalcNumber(&var1);
	while (!err)
	{
		sp.SkipWhite();
		if (sp.CheckSymbol("^"))
		{
			err = CalcNumber(&var2);
			if (err)  break;
			err = var1.Pow(&var2);
		}
		else
		{
			break;
		}
	}

	if (!err)
	{
		rresult->Assign(&var1);
	}

	return err;
}

int TExprCalc::CalcTerm(TCalcVar * rresult)
{
	TCalcVar var1;
	TCalcVar var2;
	int err = CalcFactor(&var1);
	while (!err)
	{
		sp.SkipWhite();
		if (sp.CheckSymbol("*"))
		{
			err = CalcTerm(&var2);
			if (err)  break;
			err = var1.Mul(&var2);
		}
		else if (sp.CheckSymbol("/"))
		{
			err = CalcTerm(&var2);
			if (err)  break;
			err = var1.Div(&var2);
		}
		else
		{
			break;
		}
	}

	if (!err)
	{
		rresult->Assign(&var1);
	}

	return err;
}

int TExprCalc::CalcExpression(TCalcVar * rresult)
{
	TCalcVar var1;
	TCalcVar var2;
	int err = CalcTerm(&var1);
	while (!err)
	{
		sp.SkipWhite();
		if (sp.CheckSymbol("+"))
		{
			err = CalcTerm(&var2);
			if (err)  break;
			err = var1.Add(&var2);
		}
		else if (sp.CheckSymbol("-"))
		{
			err = CalcTerm(&var2);
			if (err)  break;
			err = var1.Sub(&var2);
		}
		else
		{
			break;
		}
	}

	if (!err)
	{
		rresult->Assign(&var1);
	}

	return err;
}

