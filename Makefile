LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lm 
OBJS = $(addprefix $(OBJDIR)/,lambda_GameBase.o lambda_Game.o lambda_InputHandler.o)
OBJDIR = objs
FLAGS = -Wall

.PHONY: clean

main: $(OBJS) $(OBJDIR)/main.o
	g++ $^ -o main $(LIBS)

$(OBJDIR)/main.o: src/main.cpp
	g++ -c -I include $< -o $@ $(FLAGS)

$(OBJDIR)/lambda_InputHandler.o: include/lambda_InputHandler.cpp include/lambda_InputHandler.h
	g++ -c $< -o $@

$(OBJDIR)/lambda_Game.o: include/lambda_Game.cpp include/lambda_Game.h
	g++ -c $< -o $@

$(OBJDIR)/lambda_GameBase.o: include/lambda_GameBase.cpp include/lambda_GameBase.h
	g++ -c $< -o $@

clean:
	rm $(OBJS) $(OBJDIR)/main.o -v main
