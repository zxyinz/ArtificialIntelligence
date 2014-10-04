#include"cSanTerminalDeviceWin.h"
using namespace std;
using namespace San;
using namespace San::Device;
#ifdef _WINDOWS
void cSanTerminalDeviceWin::iSetTerminalTittle(SString strTittle)
{
	if(!strTittle.empty())
	{
		this->m_strTittle=strTittle;
		::SetConsoleTitle(this->m_strTittle.c_str());
	}
}
void cSanTerminalDeviceWin::iSetTerminalCodeLocate(SStringA strLocate)
{
	this->m_strCodeLocate=::setlocale(LC_ALL,strLocate.c_str());
}
void cSanTerminalDeviceWin::iSetDefaultTextColor(eSANTERMINALCOLOR Color)
{
	if(Color!=STC_DEFAULT)
	{
		this->m_TextDefColor=Color;
		::SetConsoleTextAttribute(this->m_TerminalHandle,this->m_TextDefColor|(this->m_BackgroundDefColor<<4));
	}
}
void cSanTerminalDeviceWin::iSetDefaultBackgroundColor(eSANTERMINALCOLOR Color)
{
	if(Color!=STC_DEFAULT)
	{
		this->m_BackgroundDefColor=Color;
		::SetConsoleTextAttribute(this->m_TerminalHandle,this->m_TextDefColor|(this->m_BackgroundDefColor<<4));
	}
}
void cSanTerminalDeviceWin::_CreateConsoleDevice()
{
	::AllocConsole();
	::AttachConsole(ATTACH_PARENT_PROCESS);
	::freopen("CONIN$","r+t",stdin);
	::freopen("CONOUT$","w+t",stdout);
}
bool cSanTerminalDeviceWin::iSetTerminalBufferSize(uint32 BufferSize)
{
	if(BufferSize==0)
	{
		return false;
	}
	delete[] this->m_pTerminalBuffer;
	this->m_pTerminalBuffer=nullptr;
	this->m_pTerminalBuffer=new schar[BufferSize];
	::memset(this->m_pTerminalBuffer,0,sizeof(schar)*BufferSize);
	this->m_BufferSize=BufferSize;
	return true;
}
SString cSanTerminalDeviceWin::iGetTerminalTittle()
{
	return this->m_strTittle;
}
SStringA cSanTerminalDeviceWin::iGetTerminalCodeLocate()
{
	return this->m_strCodeLocate;
}
eSANTERMINALCOLOR cSanTerminalDeviceWin::iGetDefaultTextColor()
{
	return this->m_TextDefColor;
}
eSANTERMINALCOLOR cSanTerminalDeviceWin::iGetDefaultBackgroundColor()
{
	return this->m_BackgroundDefColor;
}
uint32 cSanTerminalDeviceWin::iGetTerminalBufferSize()
{
	return this->m_BufferSize;
}
SString cSanTerminalDeviceWin::iGetInputString(schar DelLim)
{
#ifndef _UNICODE
	::getline(cin,this->m_strBuffer,DelLim);
#else
	::getline(wcin,this->m_strBuffer,DelLim);
#endif
	return this->m_strBuffer;
}
void cSanTerminalDeviceWin::iOutputStringA(SStringA strString,eSANTERMINALCOLOR TextColor,eSANTERMINALCOLOR BackgroundColor)
{
	if(strString.empty())
	{
		return;
	}
	if((TextColor==STC_DEFAULT)&&(BackgroundColor==STC_DEFAULT))
	{
		//::printf("%s",strString.c_str());
		::cout<<strString.c_str();
	}
	else
	{
		if(TextColor==STC_DEFAULT)
		{
			TextColor=this->m_TextDefColor;
		}
		if(BackgroundColor==STC_DEFAULT)
		{
			BackgroundColor=this->m_BackgroundDefColor;
		}
		::SetConsoleTextAttribute(this->m_TerminalHandle,TextColor|(BackgroundColor<<4));
		//::printf("%s",strString.c_str());
		::cout<<strString.c_str();
		::SetConsoleTextAttribute(this->m_TerminalHandle,this->m_TextDefColor|(this->m_BackgroundDefColor<<4));
	}
}
void cSanTerminalDeviceWin::iOutputStringW(SStringW strString,eSANTERMINALCOLOR TextColor,eSANTERMINALCOLOR BackgroundColor)
{
	if(strString.empty())
	{
		return;
	}
	if((TextColor==STC_DEFAULT)&&(BackgroundColor==STC_DEFAULT))
	{
		//::wprintf(L"%s",strString.c_str());
		::wcout<<strString.c_str();
	}
	else
	{
		if(TextColor==STC_DEFAULT)
		{
			TextColor=this->m_TextDefColor;
		}
		if(BackgroundColor==STC_DEFAULT)
		{
			BackgroundColor=this->m_BackgroundDefColor;
		}
		::SetConsoleTextAttribute(this->m_TerminalHandle,TextColor|(BackgroundColor<<4));
		//::wprintf(L"%s",strString.c_str());
		::wcout<<strString.c_str();
		::SetConsoleTextAttribute(this->m_TerminalHandle,this->m_TextDefColor|(this->m_BackgroundDefColor<<4));
	}
}
void cSanTerminalDeviceWin::iOutputString(SString strString,eSANTERMINALCOLOR TextColor,eSANTERMINALCOLOR BackgroundColor)
{
#ifndef _UNICODE
	this->iOutputStringA(strString,TextColor,BackgroundColor);
#else
	this->iOutputStringW(strString,TextColor,BackgroundColor);
#endif
}
//void cSanTerminalDeviceWin::iClearLine()
//{
void cSanTerminalDeviceWin::iClearScreen()
{
	::system("cls");
}
#endif