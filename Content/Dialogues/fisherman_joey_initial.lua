number_of_lines = 5

line_1 = 
{
	words = "Good morning, my dear apprentice!",

	options = 
	{
		count = 3,
		
		option1 = 
		{
			text = "Good morning, uncle.",
			next_line = "line_2"
		},
		-----------------------------------------
		option2 = 
		{
			text = "What is that book you're holding?",
			next_line = "line_5"
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
line_2 = 
{
	words = "Oh I am getting so old... I have a task for you... Hill's top tower, our home, is slowly falling apart. The cracked floors are slowly collapsing underneath my feet...",

	options = 
	{
		count = 2,
	
		option1 = 
		{
			text = "How can I help?",
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
	words = "There's a small village south of here, mostly dealing in lumber and crops. Go to the main lumber station there and have him send 200 units of lumber.",

	options = 
	{
		count = 1,
	
		option1 = 
		{
			text = "I will return at once.",
			next_line = "line_4"
		},
	}
}
----------------------------------------------------------------
line_4 = 
{
	words = "Wait. You will need these gold coins. Don't spend them on anything else!",

	options = 
	{
		count = 1,
	
		option1 = 
		{
			text = "Don't worry, uncle!",
			next_line = "none"
		},
	}
}
----------------------------------------------------------------
line_5 = 
{
	words = "Oh, this old tome? I will explain it in time... For now, it's enough for you to know that it contains knowledge...",

	options = 
	{
		count = 2,
	
		option1 = 
		{
			text = "What was it you wanted to tell me?",
			next_line = "line_2"
		},
		-----------------------------------------
		option2 = 
		{
			text = "Goodbye.",
			next_line = "none"

		},
	}
}




