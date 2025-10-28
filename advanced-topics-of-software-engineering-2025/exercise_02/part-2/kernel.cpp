// This code is derived from work done by the Circle project (see description below).
//
// kernel.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014-2018  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "kernel.h"
#include <circle/string.h>

#include <circle/sched/task.h>
#include <circle/net/in.h>
#include <circle/net/socket.h>
#include <circle/net/ipaddress.h>

static const char FromKernel[] = "kernel";

CActLED CKernel::ActLED; // initialize built-in LED (if available)

CKernel::CKernel (void)
:	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
	m_Timer (&m_Interrupt),
	m_Logger (m_Options.GetLogLevel (), &m_Timer),
	m_GPIOManager (&m_Interrupt), 		// set up GPIO manager

	m_GPIO18 (18, GPIOModeOutput), 		// TODO: modify to your output pin 
	
	m_USBHCI (&m_Interrupt, &m_Timer)
{
	ActLED.Blink (5);		// show we are alive
	// TODO: initialize LED as off
}

CKernel::~CKernel (void)
{
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice (m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize (pTarget);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize ();
	}

	if (bOK)
	{
		bOK = m_GPIOManager.Initialize ();
	}

	if (bOK)
	{
		bOK = m_USBHCI.Initialize ();
	}

	// // Uncomment this for TCP-related functionalities
	// if (bOK)
	// {
	// 	bOK = m_Net.Initialize ();
	// }

	return bOK;
}

TShutdownMode CKernel::Run (void)
{
	// Write to console
	m_Logger.Write (FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

	// // Get IP Config - Uncomment these lines for TCP-related functionalities
	// CString IPString;
	// m_Net.GetConfig ()->GetIPAddress ()->Format (&IPString);
	// m_Logger.Write (FromKernel, LogNotice, "Network config starting up.");

	// Set up GPIO pin interrupt
	unsigned int myPin = ; 		// TODO: set this based on the pin to be used for interrupt
	CGPIOPin myInputPin (myPin, GPIOModeInput, &m_GPIOManager);

	// What do these two lines of code do?
	myInputPin.ConnectInterrupt (foo, this);
	myInputPin.EnableInterrupt (GPIOInterruptOnFallingEdge);

	m_Logger.Write (FromKernel, LogNotice, "Pin interrupt configured.");

	while (1)
	{

		// Write status message
		m_Logger.Write (FromKernel, LogNotice, "In Loop.");	

	}

	return ShutdownHalt;
}

void foo ( void *pParam )
{

	CKernel *pThis = (CKernel *) pParam;
	pThis->myInterruptHandler();

}

void CKernel::myInterruptHandler ()
{

	m_Logger.Write (FromKernel, LogNotice, "Entered Interrupt.");

}
