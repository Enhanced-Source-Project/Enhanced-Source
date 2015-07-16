"Resource/UI/HoldoutWaveEndPanel.res"
{
	"HoldoutWaveEndPanel"
	{
		"ControlName"		"EditablePanel"
		"fieldName"			"HoldoutWaveEndPanel"
		"xpos"				"c-300"
		"ypos"				"100"
		"wide"				"600"
		"tall"				"250"
		"PaintBackgroundType"	"0"
		"bgcolor_override"	"0 0 0 128"
	}
	
	"WaveCompleteLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"WaveCompleteLabel"
		"xpos"			"300"
		"ypos"			"0"
		"zpos"			"0"
		"wide"			"150"
		"tall"			"30"
		"enabled"		"1"
		"textAlignment"   "center"
		"font"			"DefaultLarge"
		"fgcolor_override"	"0 0 0 0"		// label starts out invisible and is animated in using event WaveAnnounce
		"PaintBackgroundType"	"0"
		"bgcolor_override"	"0 0 0 0"
	}
}