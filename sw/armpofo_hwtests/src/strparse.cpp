#include "strparse.h"
#include "string.h"
#include <strings.h>

TStrParseObj::TStrParseObj()
{
  Init(NULL, 0);
}

TStrParseObj::TStrParseObj(char * astr, int buflen)
{
  Init(astr, buflen);
}

void TStrParseObj::Init(char * astr, int buflen)
{
  bufstart = astr;
  if (buflen == 0 && (bufstart != NULL))
  {
    buflen = strlen(astr);
  }
  bufend = bufstart + buflen;

  readptr = bufstart;
  prevptr = bufstart;
  prevlen = 0;
}

void TStrParseObj::SkipSpaces(bool askiplineend)
{
  char * cp;
  cp = readptr;
  while ( (cp < bufend) && ( (*cp == 32) || (*cp == 9) || (askiplineend && ((*cp == 13) || (*cp == 10))) ) )
  {
    ++cp;
  }
  readptr = cp;
}

void TStrParseObj::SkipWhite()
{
  while (1)
  {
    SkipSpaces();
    if ((readptr < bufend) && (*readptr == commentmarker))
    {
      ReadTo("\n\r");
    }
    else
    {
      return;
    }
  }
}


/*
  skips line end too, but LineLength does not contain the line end chars
  bufend shows the end of the buffer (one after the last character)
  so bufend-bufstart = buffer length
  returns false if end of buffer reached without line end
*/
bool TStrParseObj::ReadLine()
{
  char * cp;

  prevptr = readptr;
  cp = readptr;

  while ((cp < bufend) && (*cp != 13) && (*cp != 10))
  {
    ++cp;
  }

  prevlen = cp - readptr;

  // skip the line end, but only one!
  if ((cp < bufend) && (*cp == 13))  ++cp;
  if ((cp < bufend) && (*cp == 10))  ++cp;

  readptr = cp;

  return (prevptr < bufend);
}

bool TStrParseObj::ReadTo(const char * checkchars)  // reads until one of the checkchars reached.
{
  char * cp;
  char * ccstart;
  char * ccend;
  char * ccptr;

  prevptr = readptr;
  cp = readptr;
  ccstart = (char *)checkchars;
  ccend = ccstart + strlen(checkchars);

  while (cp < bufend)
  {
    // check chars
    ccptr = ccstart;
    while (ccptr < ccend)
    {
      if (*ccptr == *cp)
      {
        prevlen = cp - readptr;
        readptr = cp;
        return true;
      }
      ++ccptr;
    }

    ++cp;
  }

  // end of buffer, store the remaining length:
  prevlen = cp - readptr;
  readptr = cp;
  return false;
}

bool TStrParseObj::SearchPattern(const char * checkchars)  // reads until the checkstring is found, readptr points to matching start
{
  char * cp;
  char * cps;
  char * ccstart;
  char * ccend;
  char * ccptr;
  unsigned ccslen = strlen(checkchars);

  prevptr = readptr;
  cps = readptr;
  ccstart = (char *)checkchars;
  ccend = ccstart + ccslen;

  // check start pos cycle
  while (cps < bufend - ccslen)
  {
    // check chars cycle
    cp = cps;
    ccptr = ccstart;
    char match = 1;
    while (ccptr < ccend)
    {
      if (*cp != *ccptr)
      {
        match = 0;
        break;
      }
      ++cp;
      ++ccptr;
    }

    if (match)
    {
      // does not skip the matching pattern, readptr points to the matching pattern
      prevlen = cps - readptr;
      readptr = cps;
      return true;
    }

    ++cps;
  }

  return false;
}

bool TStrParseObj::ReadToChar(char achar)
{
  char * cp = readptr;
  bool result = false;

  prevptr = readptr;

  while (cp < bufend)
  {
    if (*cp == achar)
    {
      result = true;
      break;
    }
    ++cp;
  }

  prevlen = cp - readptr;
  readptr = cp;

  return result;
}

bool TStrParseObj::CheckSymbol(const char * checkstring)
{
  char * cp = readptr;
  char * csptr = (char *)checkstring;
  char * csend = csptr + strlen(checkstring);

  while ((csptr < csend) && (cp < bufend) && (*csptr == *cp))
  {
    ++csptr;
    ++cp;
  }

  if (csptr != csend)
  {
    return false;
  }

  readptr = cp;
  return true;
}

bool TStrParseObj::ReadAlphaNum()
{
  char * cp = readptr;
  bool result = false;

  prevptr = readptr;

  while (cp < bufend)
  {
    char c = *cp;

    if (
        ((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))
        || (c == '_')
        || ((c == '-') && (cp == readptr))  // sign: allowed only at the first character
       )
    {
      result = true;
      ++cp;
    }
    else
    {
      break;
    }
  }

  prevlen = cp - readptr;
  readptr = cp;

  return result;
}

bool TStrParseObj::ReadDecimalNumbers()
{
  char * cp = readptr;
  bool result = false;

  prevptr = readptr;

  while (cp < bufend)
  {
    char c = *cp;

    if (
        ((c >= '0') && (c <= '9'))
       )
    {
      result = true;
      ++cp;
    }
    else
    {
      break;
    }
  }

  prevlen = cp - readptr;
  readptr = cp;

  return result;
}

bool TStrParseObj::ReadHexNumbers()
{
  char * cp = readptr;
  bool result = false;

  prevptr = readptr;

  while (cp < bufend)
  {
    char c = *cp;

    if (
        ((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f'))
       )
    {
      result = true;
      ++cp;
    }
    else
    {
      break;
    }
  }

  prevlen = cp - readptr;
  readptr = cp;

  return result;
}

double TStrParseObj::ParseFloatNumber()
{
	double  result = 0;
	double  frac_mul = 0.1;
	bool    fractional = false;

  char * cp = readptr;
  prevptr = readptr;

  while (cp < bufend)
  {
    char c = *cp;

    if ((c >= '0') && (c <= '9'))
    {
    	if (fractional)
    	{
    		result = result + (frac_mul * (*cp - '0'));
    		frac_mul = frac_mul * 0.1l;
    	}
    	else
    	{
    		result = result * 10.0l + (*cp - '0');
    	}
      ++cp;
    }
    else if ('.' == c)
    {
    	fractional = true;
    	++cp;
    }
    else
    {
      break;
    }
  }

  prevlen = cp - readptr;
  readptr = cp;

  return result;
}


bool TStrParseObj::ReadQuotedString()
{
  if (readptr >= bufend)
  {
    return false;
  }

  if (*readptr != '"')
  {
    return false;
  }

  ++readptr;  // skip "

  ReadToChar('"'); // read to line end

  if ((readptr < bufend) && (*readptr == '"'))
  {
    ++readptr;
  }

  return true;
}

bool TStrParseObj::UCComparePrev(const char * checkstring)
{
  return PCharUCCompare(&prevptr, prevlen, checkstring);
}

int TStrParseObj::PrevToInt()
{
  return PCharToInt(prevptr, prevlen);
}

unsigned TStrParseObj::PrevHexToInt()
{
  return PCharHexToInt(prevptr, prevlen);
}

int TStrParseObj::GetLineNum(const char * pos)
{
  int result = 1;
  char * cp = bufstart;

  while ((cp < pos) && (cp < bufend))
  {
    if (*cp == '\n')
    {
      ++result;
    }
    ++cp;
  }

  return result;
}

int TStrParseObj::GetLineNum()
{
  return GetLineNum(prevptr);
}

//-------------------------------------------------------------------------

bool PCharUCCompare(char * * ReadPtr, int len, const char * checkstring)
{
  char * cp = *ReadPtr;
  char * bufend = cp + len;
  char * csptr = (char *)checkstring;
  char * csend = csptr + strlen(checkstring);

  while ((csptr < csend) and (cp < bufend))
  {
    char c = *cp;
    if ((c >= 'a') && (c <= 'z'))  c &= 0xDF;

    if (c != *csptr)
    {
      break;
    }

    ++csptr;
    ++cp;
  }

  if (csptr != csend)
  {
    return false;
  }

  *ReadPtr = cp;
  return true;
}

int PCharToInt(char * ReadPtr, int len)
{
  char * cp = ReadPtr;
  char * endp = ReadPtr + len;
  int result = 0;

  while (cp < endp)
  {
    result = result * 10 + (*cp - '0');
    ++cp;
  }

  return result;
}

unsigned PCharHexToInt(char * ReadPtr, int len)
{
  char * cp = ReadPtr;
  char * endp = ReadPtr + len;
  int result = 0;

  while (cp < endp)
  {
    char c = *cp;
    if      (c >= '0' && c <= '9')  result = (result << 4) + (c - '0');
    else if (c >= 'A' && c <= 'F')  result = (result << 4) + (c - 'A' + 10);
    else if (c >= 'a' && c <= 'f')  result = (result << 4) + (c - 'a' + 10);
    else
      break;

    ++cp;
  }

  return result;
}


//EOF
