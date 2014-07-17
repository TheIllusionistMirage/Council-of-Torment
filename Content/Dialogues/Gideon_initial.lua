dialogue = {
	line_1 = {
		words = "Good morning, dear Baldric!",

		options = {
			option1 = {
				text = "Good morning, uncle.",
				next_line = "line2"
			}
			-----------------------------------------
			option2 = {
				text = "What is that book you're holding?",
				next_line = "line3"
			}
			-----------------------------------------
			option3 = {
				text = "Goodbye.",
				next-line = "none"
			}
		}
	}
----------------------------------------------------------
	line 2 = {
		words = "I have a task for you. Hill's Top Tower is getting old, even older than me. The floor is short from collapsing right under my very feet. I need you to fetch some materials from the village south of here...",

		options = {
			option1 = {
				text = "Continue...",
				next_line = "none"
			}
		}
	}
----------------------------------------------------------
	line_3 = {
		words = "This old tome? It's none of your concern, dear apprentice. Maybe someday though...",

		options = {
			text = "I understand...",
			next_line = "line1"
		}
	}
}
