#define ICON_PRIMARY_1				"g\\mp\\icons\\map\\ico_c4_1.tga"
#define ICON_PRIMARY_2				"g\\mp\\icons\\map\\ico_c4_1.tga"
#include <inc\maxovo\!max_multiplayer.h>
#include <inc\sc_interdef.h>

#define GAME_TYPE		GAME_TYPE_COOP

dword	SIDE1			= SC_P_SIDE_US;		//musi byt definovano
dword	SIDE2			= SC_P_SIDE_VC;		//musi byt definovano

BOOL side1_rule1(s_MAX_RULE_data *MAX_RULE_data);
BOOL side1_rule2(s_MAX_RULE_data *MAX_RULE_data);
BOOL MP_bomb1_explode(s_MAX_RULE_data *MAX_RULE_data);
BOOL MP_bomb2_explode(s_MAX_RULE_data *MAX_RULE_data);

#define	GROUPS_COUNT_SIDE1	1
char	respawn_groups_side1_names[GROUPS_COUNT_SIDE1][MAX_STRING_LEN] =	{	"COOP_US"	};
char	respawn_groups_side1_op_names[GROUPS_COUNT_SIDE1][MAX_STRING_LEN] =	{		   ""	};
BOOL	respawn_groups_side1_active[GROUPS_COUNT_SIDE1] =					{	    TRUE	};
p_fce	respawn_groups_side1_rules[GROUPS_COUNT_SIDE1] =					{		NULL	};

#define WINNING_RULES_SIDE1_COUNT	3

p_fce	WINNING_RULES_SIDE1[WINNING_RULES_SIDE1_COUNT] =				{	MAX_MUL_RULE_side_wins_opp_death_all , MP_bomb1_explode , MP_bomb2_explode };

// definuje se jake dalsi pravidla musi jeste platit s aktualnim pravidlem, -1 znamena zadne dalsi pravidlo
int		WINNING_RULES_SIDE1_OPP[WINNING_RULES_SIDE1_COUNT][WINNING_RULES_SIDE1_COUNT-1] =			{	0,	0,		
																 1,	2,	
																0,	-1,	};

#define OBJECTS_COUNT				2
char	*MAX_MUL_object_names[OBJECTS_COUNT] =							{	"aa_gun_destr",	"t_vysilacka_01"	};

BOOL MAX_MUL_get_winning_side(s_MAX_RULE_data *MAX_RULE_data);

#define ROUND_TIMEOUT_WINNING_SIDE_COUNT	1 //todle asi bude vzdy pouze 1, ale ...
p_fce	ROUND_TIMEOUT_WINNING_SIDE[ROUND_TIMEOUT_WINNING_SIDE_COUNT] =	{	MAX_MUL_get_winning_side	};
//-------------------------------- MAPA ---------------------------------
#define MAP_NAME		"Levels\\Minefactory\\data\\minefactory_mp\\mapfpv\\map.DDS"
#define MAP_TRANSFORM	"Levels\\Minefactory\\data\\minefactory_mp\\mapfpv\\map.dat"

void MAX_MUL_define_custom_items() {
	int		i, objective = 1;
	for (i=0; i<OBJECTS_COUNT; i++) {
		MAX_MUL_add_item_primary(MAX_MUL_object_names[i], SC_MP_MAP_SIDEMASK_SIDE1|SC_MP_MAP_SIDEMASK_SPECTATOR, objective++);
	}
}

int MAX_MUL_define_custom_objectives() {
	MAX_MUL_add_objective(1, SC_MP_MAP_SIDEMASK_SIDE1|SC_MP_MAP_SIDEMASK_SPECTATOR, "Destroy AA_Gun in enemy base", "g\\mp\\icons\\map\\ch_object1.tga");
	MAX_MUL_add_objective(2, SC_MP_MAP_SIDEMASK_SIDE1|SC_MP_MAP_SIDEMASK_SPECTATOR, "Destroy Radio in enemy base", "G\\MP\\items\\vysilacka\\t_vysilacka_01.dds");
	/*MAX_MUL_add_objective(3, SC_MP_MAP_SIDEMASK_SIDE1|SC_MP_MAP_SIDEMASK_SPECTATOR, 1007, "Levels\\oil_station\\data\\oil_mp\\ws_primary_A.bmp");
	MAX_MUL_add_objective(4, SC_MP_MAP_SIDEMASK_SIDE1|SC_MP_MAP_SIDEMASK_SPECTATOR, 1007, "Levels\\oil_station\\data\\oil_mp\\ws_primary_A.bmp");*/
	return 1;
}
/*if (CLN_object_status == STATE_DESTROYED)	
	MAX_MUL_change_objective(1, SC_MP_MAP_SIDEMASK_SIDE1|SC_MP_MAP_SIDEMASK_SPECTATOR, 1083, "g\\mp\\icons\\map\\ch_object1.tga");
	MAX_MUL_change_objective(2, SC_MP_MAP_SIDEMASK_SIDE1|SC_MP_MAP_SIDEMASK_SPECTATOR, 1083, "G\\MP\\items\\vysilacka\\t_vysilacka_01.dds");
	return 1;
}*/


//-----------------------------------------------------------------------
#include <inc\maxovo\!max_multiplayer.cxx>
//-----------------------------------------------------------------------
//------------------------------ PRAVIDLA -------------------------------
//-----------------------------------------------------------------------
int	iii;

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
BOOL MAX_MUL_get_winning_side(s_MAX_RULE_data *MAX_RULE_data) {
	MAX_RULE_data->side = SIDE2;
	return TRUE; //SIDE2 brani
}

BOOL MP_bomb1_explode(s_MAX_RULE_data *MAX_RULE_data) {
	static int pos_in_array = 0;
	if (MAX_RULE_data->reinit == TRUE) {
		MAX_RULE_data->reinit = FALSE;
		for (iii=0; iii<OBJECTS_COUNT; iii++) {
			if (MAX_MUL_object[iii].nod == SC_NOD_GetNoMessage_Entity(MAX_MUL_object_names[0])) { // u HB
				pos_in_array = iii;
				break;
			}
		}
		if (iii >= OBJECTS_COUNT) SC_message("Fatal ERROR - nepodarilo ne najit MP_bomb1_explode!!!");
		return FALSE;
	}

	if ((MAX_MUL_object[pos_in_array].nod) && (MAX_MUL_object[pos_in_array].SRV_object_status->state == STATE_DESTROYED)) return TRUE;
	
	return FALSE;
}

//-----------------------------------------------------------------------

BOOL MP_bomb2_explode(s_MAX_RULE_data *MAX_RULE_data) {
	static int pos_in_array = 1;
	if (MAX_RULE_data->reinit == TRUE) {
		MAX_RULE_data->reinit = FALSE;
		for (iii=0; iii<OBJECTS_COUNT; iii++) {
			if (MAX_MUL_object[iii].nod == SC_NOD_GetNoMessage_Entity(MAX_MUL_object_names[1])) { // u HELI
				pos_in_array = iii;
				break;
			}
		}
		if (iii >= OBJECTS_COUNT) SC_message("Fatal ERROR - nepodarilo ne najit MP_bomb2_explode!!!");
		return FALSE;
	}

	if ((MAX_MUL_object[pos_in_array].nod) && (MAX_MUL_object[pos_in_array].SRV_object_status->state == STATE_DESTROYED)) return TRUE;

	return FALSE;
}
//-----------------------------------------------------------------------