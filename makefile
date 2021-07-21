CC = gcc
TARGET = dash
DEPS = header.h
OBJ = main.o pipe_and_run.o env.o input.o display.o builtin.o ls.o execut.o pinfo.o history.o nightwatch.o redirect.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $<

dash: $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -r $(OBJ) $(TARGET)
