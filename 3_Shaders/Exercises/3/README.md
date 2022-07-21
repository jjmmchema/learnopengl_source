# Why does the bottom-left part of the triangle appear black?
Answer:
This is because all those pixels below x = 0 and on the left side of y = 0  
have negative x and y values, therefore because x is mapped to the amount of  
red value of the pixel and y to the amount of green value this results not really  
in a black color specifically but in two undefined values for red and green colors  
since all RGBA value must be greater than 0. Because the undefined problem, OpenGL
uses a "default" color, in this case it's black.
