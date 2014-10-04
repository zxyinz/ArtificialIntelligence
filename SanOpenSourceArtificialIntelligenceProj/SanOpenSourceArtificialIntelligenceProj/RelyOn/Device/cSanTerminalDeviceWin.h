#include"..\RenderSystem.h"
#include"..\Core\SanTypes.h"
//#include"..\Core\cSanResourceManagerDef.h"
using namespace std;
#pragma once
namespace San
{
	namespace Device
	{
#ifdef _WINDOWS
#ifndef __CSANTERMINALDEVICEWIN_H__
#define __CSANTERMINALDEVICEWIN_H__
		enum eSANTERMINALCOLOR
		{
			STC_BLACK		= 0x0000,
			STC_BLUE		= 0x0001,
			STC_GREEN		= 0x0002,
			STC_CYAN		= 0x0003,
			STC_RED			= 0x0004,
			STC_MAGENTA		= 0x0005,
			STC_YELLOW		= 0x0006,
			STC_GREY		= 0x0007,
			STC_BLUE_HL		= 0x0009,
			STC_GREEN_HL	= 0x000A,
			STC_CYAN_HL		= 0x000B,
			STC_RED_HL		= 0x000C,
			STC_MAGENTA_HL	= 0x000D,
			STC_YELLOW_HL	= 0x000E,
			STC_WHITE		= 0x000F,
			STC_DEFAULT		= 0x00FF,
		};

		class cSanTerminalDeviceWin
		{
		private:
			SString m_strTerminalName;
			uint32 m_TerminalID;
			SHANDLE m_TerminalHandle;
			SString m_strTittle;
			SStringA m_strCodeLocate;
			eSANTERMINALCOLOR m_TextDefColor;
			eSANTERMINALCOLOR m_BackgroundDefColor;
			schar* m_pTerminalBuffer;//////////////
			uint32 m_BufferSize;///////////////////
			SString m_strBuffer;
		protected:
			void _CreateConsoleDevice();
		public:
			cSanTerminalDeviceWin(SString strTerminalName=_SSTR("SanLabTerminal"),SString strTerminalTittle=_SSTR("San Lab Terminal"))
				:m_strTerminalName(strTerminalName),
				m_TerminalID(0),
				m_TerminalHandle(nullptr),
				m_strTittle(strTerminalTittle),
				m_strCodeLocate(""),
				m_TextDefColor(STC_WHITE),
				m_BackgroundDefColor(STC_BLACK),
				m_pTerminalBuffer(nullptr),
				m_BufferSize(1024),
				m_strBuffer(_SSTR(" "))
			{
				this->m_TerminalHandle=::GetStdHandle(STD_OUTPUT_HANDLE);
				if(this->m_TerminalHandle==nullptr)
				{
					this->_CreateConsoleDevice();
				}
				this->m_TerminalHandle=::GetStdHandle(STD_OUTPUT_HANDLE);
				::SetConsoleTitle(this->m_strTittle.c_str());
				this->m_strCodeLocate=::setlocale(LC_ALL,this->m_strCodeLocate.c_str());
				this->m_pTerminalBuffer=new schar[this->m_BufferSize];
				::memset(this->m_pTerminalBuffer,0,sizeof(schar)*this->m_BufferSize);
				::SetConsoleTextAttribute(this->m_TerminalHandle,(this->m_TextDefColor)|(this->m_BackgroundDefColor<<4));
			};
			~cSanTerminalDeviceWin()
			{
				delete[] this->m_pTerminalBuffer;
				this->m_pTerminalBuffer=nullptr;
			};
			void iSetTerminalTittle(SString strTittle);
			void iSetTerminalCodeLocate(SStringA strLocate);
			void iSetDefaultTextColor(eSANTERMINALCOLOR Color);
			void iSetDefaultBackgroundColor(eSANTERMINALCOLOR Color);
			bool iSetTerminalBufferSize(uint32 BufferSize);
			SString iGetTerminalTittle();
			SStringA iGetTerminalCodeLocate();
			eSANTERMINALCOLOR iGetDefaultTextColor();
			eSANTERMINALCOLOR iGetDefaultBackgroundColor();
			uint32 iGetTerminalBufferSize();
			SString iGetInputString(schar DelLim=_SSTR('\n'));
			void iOutputStringA(SStringA strString,eSANTERMINALCOLOR TextColor=STC_DEFAULT,eSANTERMINALCOLOR BackgroundColor=STC_DEFAULT);
			void iOutputStringW(SStringW strString,eSANTERMINALCOLOR TextColor=STC_DEFAULT,eSANTERMINALCOLOR BackgroundColor=STC_DEFAULT);
			void iOutputString(SString strString,eSANTERMINALCOLOR TextColor=STC_DEFAULT,eSANTERMINALCOLOR BackgroundColor=STC_DEFAULT);
			void iClearLine();/////////////////////
			void iClearScreen();
		};
		typedef cSanTerminalDeviceWin cSanTerminalDevice;
#endif
#endif
	}
}