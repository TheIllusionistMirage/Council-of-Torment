number_of_lines = 2

line_1 = 
{
	words = "Good morning, dear Baldric!",

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
			next_line = "line_3"
		},
		-----------------------------------------
		option3 = 
		{
			text = "Goodbye.",
			next_line = "none"
		}
	}
}

line_2 = 
{
	words = "I have a task for you...  Hillstop Tower is so old and the bricks are falling apart. The floor looks like it is absorbing me and this whole nexus of knowledge with every moment...",

	options = 
	{
		count = 2,
	
		option1 = 
		{
			text = "How can I help?",
			next_line = "none"
		},
		-----------------------------------------
		option2 = 
		{
			text = "Goodbye.",
			next_line = "none"

		},
	}
}
