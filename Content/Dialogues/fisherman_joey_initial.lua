number_of_lines = 5

line_1 = 
{
	words = "*whistling vigorously*",

	options = 
	{
		count = 3,
		
		option1 = 
		{
			text = "Hi.",
			next_line = "line_2"
		},
		-----------------------------------------
		option2 = 
		{
			text = "Goodbye.",
			next_line = "none"
		}
	}
}
----------------------------------------------------------------
line_2 = 
{
	words = "Well aren't you a sturdy fellow! Never seen you before. Are you from Southdell?",

	options = 
	{
		count = 2,
	
		option1 = 
		{
			text = "No. I grew up in Hill's Top Tower. Who are you, what are you doing here?",
			next_line = "line_3"
		},
		-----------------------------------------
		option2 = 
		{
			text = "Goodbye.",
			next_line = "none"

		},
	}
}
----------------------------------------------------------------
line_3 = 
{
	words = "Oh just fishing... I've been here in the same spot for six years now. There is something about this particular patch of water...",

	options = 
	{
		count = 2,
	
		option1 = 
		{
			text = "Oh?",
			next_line = "line_4"
		},
		-----------------------------------------
		option2 = 
		{
			text = "Goodbye.",
			next_line = "none"

		},

	}
}
----------------------------------------------------------------
line_4 = 
{
	words = "Nobody knows where this small patch of water comes from, because the trees cover up all the endings, but I suspect it's a stream coming from the north ending here. I always catch a lot of fish when I come here!",

	options = 
	{
		count = 2,
	
		option1 = 
		{
			text = "That's pretty interesting. I suppose you don't just eat fish every day, why do you come here? What's your name?",
			next_line = "line_5"
		},
		-----------------------------------------
		option2 = 
		{
			text = "Goodbye.",
			next_line = "none"

		},

	}
}
----------------------------------------------------------------
line_5 = 
{
	words = "Hah. Damn right I don't just eat fish. Name's Joey the Fisherman! I sell all my fish to Southdell's chief; he buys them all and sells them to some dwarven merchant that gives him silver in exchange.",

	options = 
	{
		count = 3,
	
		option1 = 
		{
			text = "Fair exchange, I suppose.",
			next_line = "line_6"
		},
		-----------------------------------------
		option2 = 
		{
			text = "What does he do with silver? It's useless around these parts.",
			next_line = "line_7"
		},
		-----------------------------------------
		option3 = 
		{
			text = "Goodbye.",
			next_line = "none"
		}
	}
}
----------------------------------------------------------------
line_6 = 
{
	words = "Keeps me well fed. I don't have a family, so that gives me all the time in the world to fish.",

	options = 
	{
		count = 2,
	
		option1 = 
		{
			text = "Alright. I think I'll get going now.",
			next_line = "line_8"
		},
		-----------------------------------------
		option2 = 
		{
			text = "Goodbye.",
			next_line = "none"
		}
	}
}
----------------------------------------------------------------
line_7 = 
{
	words = "Silver? Useless? No way! Whoever has silver can buy the most exotic goods in Erden: dwarven luxuries and the like. Chief Rolith doesn't buy anything with it though. Rumor has it that the man collects silver in his basement. Who knows though; he doesn't allow anyone down there.",

	options = 
	{
		count = 2,
	
		option1 = 
		{
			text = "I see. Thanks for your time but I have some business to attend in Southdell; I think I'll get going now!",
			next_line = "line_8"
		},
		-----------------------------------------
		option2 = 
		{
			text = "Goodbye.",
			next_line = "none"
		}
	}
}
----------------------------------------------------------------
line_8 = 
{
	words = "Alright, friend. Pleasure having your company around. Follow the dirth path west of here and you'll reach Southdell in no time.",

	options = 
	{
		count = 1,
	
		option1 = 
		{
			text = "I will. Thanks, Joey!",
			next_line = "none"
		}
	}
}




