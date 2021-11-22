#pragma TextEncoding = "UTF-8"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.



//Proc StartProc(name)
//	String name
//	
//	SetBackground valveswitcher();CtrlBackground start,period=5
//	Button b1,title="Stop",proc=StopProc
//End
//
//
//Proc StopProc(name)
//	String name
//	
//	CtrlBackground stop
//	Button b1,title="Start",proc=StartProc
//End
//



//Abort using Ctrl-Break (Ctrl-Pause) or Shift-Esc

Function valveswitcher()
	
	//	String newDTname = "DateTime_" + num2str(DateTime)
	// 	String newValveStateName = "ValveState_" + num2str(DateTime)

	Variable/G trial_num

	trial_num += 1

	String newDTname = "DateTime_" + num2str(trial_num)
	String newValveStateName = "ValveState_" + num2str(trial_num) 


	
	String/G gDT = newDTName
	String/G gVS = newValveStateName

	Make/O/D/N=1 $(newDTName)
	Make/O/N=1 $(newValveStateName)
	
	Wave currDT = $(newDTName)
	Wave currValveState = $(newValveStateName)	
	
	Variable/G valveState
	nvar valveState 
	valveState = 0
	
	Sleep/S 5

	Variable currIndex = 0

	VDTOperationsPort2 COM5 //Change based on which port your Arduino is connected to.
	
	//Collects time when the signal to change states is sent (not when the valve state is done changing)
	Variable i
	for(i=0; i<10000; i+=1)
		ControlInfo/W=$("test") sample_time
		
		//Variable sample_duration = V_Value
		Variable sample_duration = V_Value * 60
		
		
		VDTWrite2/O=2 "1"
		valveState = 1
		DoUpdate/W=test


		//Note: Arduino takes 6 seconds to fully ensure valve switches over.
		currDT[currIndex] = DateTime
		currValveState[currIndex] = valveState //Valve State recorded when the switch-over occurs.
		currIndex += 1
		Redimension/N=(numpnts(currDT)+1) currDT, currValveState
	
		
		//Sleep 00:05:00		//default is /A, where input is in hh:mm:ss 
		//Add a /S flag if you want only seconds 
		Sleep/S sample_duration

		VDTWrite2/O=2 "2"
		valveState = 2
		DoUpdate/W=test
		
		currDT[currIndex] = DateTime
		currValveState[currIndex] = valveState
		currIndex += 1
		Redimension/N=(numpnts(currDT)+1) currDT, currValveState
		
		//Sleep 00:05:00
		Sleep/S sample_duration
				
		VDTWrite2/O=2 "3"
		valveState = 3
		DoUpdate/W=test
		
		currDT[currIndex] = DateTime
		currValveState[currIndex] = valveState
		currIndex += 1
		Redimension/N=(numpnts(currDT)+1) currDT, currValveState
		
		//Sleep 00:05:00
		Sleep/S sample_duration
				
		VDTWrite2/O=2 "4"
		valveState = 4
		DoUpdate/W=test
		
		currDT[currIndex] = DateTime
		currValveState[currIndex] = valveState
		currIndex += 1
		Redimension/N=(numpnts(currDT)+1) currDT, currValveState
		
		//Sleep 00:05:00
		Sleep/S sample_duration
		
	endfor
	
end




Function SetVarControl(sva) : SetVariableControl
	STRUCT WMSetVariableAction &sva
 
	switch( sva.eventCode )
		case 1: // mouse up
		case 2: // Enter key
		case 3: // Live update
			break
		case -1: // control being killed
			break
	endswitch
 
	return 0
End



Function ButtonControl(ba): ButtonControl
	STRUCT WMButtonAction &ba
	
	switch(ba.eventCode)
		case 1: //mouse down
		case 2: 
			reset()//mouse up inside control
		case 3:
			break
		case -1:
			break
	endswitch
	
	return 0
	
end



Function RunControl(ba): ButtonControl
	STRUCT WMButtonAction &ba
	
	switch(ba.eventCode)
		case 1: //mouse down
		case 2: 
			valveswitcher()//mouse up inside control
		case 3:
			break
		case -1:
			break
	endswitch
	
	return 0
	
end


Function ResetControl(ba): ButtonControl
	STRUCT WMButtonAction &ba
	
	switch(ba.eventCode)
		case 1: //mouse down
		case 2: 
			closePort()//mouse up inside control
		case 3:
			break
		case -1:
			break
	endswitch
	
	return 0
	
end

Function closePort()

	VDT2 resetPorts

end

Function reset()
	nvar trial_num
	
	trial_num = 0

end

Window test() : Panel
	PauseUpdate; Silent 1		// building window...
	NewPanel /W=(1278,288,1499,478) as "Valve Switcher Panel"
	SetVariable sample_time,pos={20.00,20.00},size={180.00,18.00},proc=setvarcontrol,title="Sample Time (mins)"
	SetVariable sample_time,value= _NUM:5
	Button count_reset,pos={75.00,121.00},size={80.00,20.00},proc=buttoncontrol,title="Reset Count"
	Button run_start,pos={80.00,68.00},size={70.00,20.00},proc=runcontrol,title="Start Run"
	Button reset_ports,pos={80.00,145.00},size={70.00,20.00},proc=resetcontrol,title="Reset Ports"
	SetVariable currState,pos={69.00,42.00},size={100.00,18.00},title="Valve State"
	SetVariable currState,limits={0,4,0},value= valveState
EndMacro
