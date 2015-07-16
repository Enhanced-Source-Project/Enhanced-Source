"Resource/UI/OptionsFlyout.res"
{
	"PnlBackground"
	{
		"ControlName"		"Panel"
		"fieldName"			"PnlBackground"
		"xpos"				"0"
		"ypos"				"0"
		"zpos"				"-1"
		"wide"				"156"
		"tall"				"65" [$X360]
		"tall"				"100" [$WIN32]
		"visible"			"1"
		"enabled"			"1"
		"paintbackground"	"1"
		"paintborder"		"1"
	}

	"BtnAudioVideo"	[$X360]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnAudioVideo"
		"xpos"					"0"
		"ypos"					"0"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"navUp"					"BtnStorage"
		"navDown"				"BtnController"
		"tooltiptext"			"#L4D360UI_Options_AudioVideo"
		"disabled_tooltiptext"	"#L4D360UI_Options_AudioVideo_Disabled"
		"labelText"				"#L4D360UI_AudioVideo"
		"style"					"FlyoutMenuButton"
		"command"				"AudioVideo"
	}
	
	"BtnController"	[$X360]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnController"
		"xpos"					"0"
		"ypos"					"20"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"proportionalToParent"	"1"
		"navUp"					"BtnAudioVideo"
		"navDown"				"BtnStorage"
		"tooltiptext"			"#L4D360UI_Options_Controller"
		"disabled_tooltiptext"	"#L4D360UI_Options_Controller_Disabled"
		"labelText"				"#L4D360UI_Controller"
		"style"					"FlyoutMenuButton"
		"command"				"Controller"
	}

	"BtnStorage"	[$X360]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnStorage"
		"xpos"					"0"
		"ypos"					"40"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"EnableCondition"		"Never" [$DEMO]
		"tabPosition"			"0"
		"proportionalToParent"	"1"
		"navUp"					"BtnController"
		"navDown"				"BtnAudioVideo"
		"tooltiptext"			"#L4D360UI_Options_Storage"
		"disabled_tooltiptext"	"#L4D360UI_Options_Storage_Disabled"
		"labelText"				"#L4D360UI_Storage"
		"style"					"FlyoutMenuButton"
		"command"				"Storage"
	}
	
	"BtnVideo"	[$WIN32]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnVideo"
		"xpos"					"0"
		"ypos"					"0"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"navUp"					"BtnCloud"
		"navDown"				"BtnBrightness"
		"tooltiptext"			"#L4D_video_tip"
		"labelText"				"#GameUI_Video"
		"style"					"FlyoutMenuButton"
		"command"				"Video"
	}
	
	"BtnBrightness"	[$WIN32]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnBrightness"
		"xpos"					"0"
		"ypos"					"20"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"navUp"					"BtnVideo"
		"navDown"				"BtnAudio"
		"tooltiptext"			"#GameUI_AdjustGamma"
		"disabled_tooltiptext"	"#L4D360UI_AdjustGamma_Disabled"
		"labelText"				"#GameUI_Brightness"
		"style"					"FlyoutMenuButton"
		"command"				"Brightness"
	}
	
	"BtnAudio"	[$WIN32]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnAudio"
		"xpos"					"0"
		"ypos"					"40"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"navUp"					"BtnBrightness"
		"navDown"				"BtnController"
		"tooltiptext"			"#L4D_audio_tip"
		"labelText"				"#GameUI_Audio"
		"style"					"FlyoutMenuButton"
		"command"				"Audio"
	}
	
	"BtnController"	[$WIN32]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnController"
		"xpos"					"0"
		"ypos"					"60"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"proportionalToParent"	"1"
		"navUp"					"BtnAudio"
		"navDown"				"BtnMultiplayer"
		"tooltiptext"				"#L4D_keyboard_tip"
		"labelText"				"#GameUI_Keyboard"
		"style"					"FlyoutMenuButton"
		"command"				"KeyboardMouse"
	}
	
	"BtnMouse"	[$WIN32]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnMouse"
		"xpos"					"0"
		"ypos"					"80"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"proportionalToParent"	"1"
		"navUp"					"BtnController"
		"navDown"				"BtnCloud"
		"tooltiptext"				"#L4D_keyboard_tip"
		"labelText"				"#GameUI_Mouse"
		"style"					"FlyoutMenuButton"
		"command"				"Mouse" //"MultiplayerSettings"
	}

}