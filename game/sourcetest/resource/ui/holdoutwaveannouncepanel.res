"Resource/UI/HoldoutWaveEndPanel.res"
{
	"HoldoutWaveAnnouncePanel"
	{
		"ControlName"		"EditablePanel"
		"fieldName"			"HoldoutWaveAnnouncePanel"
		"xpos"				"c-200"
		"ypos"				"100"
		"wide"				"400"
		"tall"				"80"
		"PaintBackgroundType"	"0"
		"bgcolor_override"	"0 0 0 128"
	}
	
	"WaveAnnounceLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"WaveAnnounceLabel"
		"xpos"			"250"
		"ypos"			"0"
		"zpos"			"0"
		"wide"			"100"
		"tall"			"30"
		"enabled"		"1"
		"textAlignment"   "center"
		"font"			"DefaultExtraLarge"
		"fgcolor_override"	"255 255 255 0"		// label starts out invisible and is animated in using event WaveAnnounce
		"PaintBackgroundType"	"0"
		"bgcolor_override"	"0 0 0 0"
	}
	"GetReadyLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"GetReadyLabel"
		"xpos"			"250"
		"ypos"			"30"
		"zpos"			"0"
		"wide"			"70"
		"tall"			"15"
		"enabled"		"1"
		"textAlignment"   "center"
		"labelText"		"#asw_holdout_get_ready"
		"fgcolor_override"	"0 0 0 0"		// label starts out invisible and is animated in using event WaveAnnounce
		"PaintBackgroundType"	"0"
		"bgcolor_override"	"0 0 0 0"
	}
}