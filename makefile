TAG = snake
HDRS = snake.h view.h control.h model.h keypressable.h
DEP1 = snake
DEP2 = model
DEP3 = control
DEP4 = view
DEPS = $(DEP1).o $(DEP2).o $(DEP3).o $(DEP4).o
CFLAGS += -Wall -g
CC = g++

$(TAG): $(DEPS)
	$(CC) $(CFLAGS) $(DEPS) -o $@
	$(RM) $(DEPS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

$(TAG): $(HDR)

clean:
	$(RM) $(DEPS) $(TAG)

