CC = g++
CFLAGS = -Isrc -O2
OBJSDIR = .objs
VPATH = src
OBJS = API.o Block.o BPlus_Tree.o CatalogManager.o IndexManager.o Interpreter.o main.o RecordManager.o
TARGET = minisql 
$(OBJSDIR):
	mkdir -p ./$@
$(TARGET):$(OBJSDIR) $(OBJS)
	$(CC) -o $(TARGET) $(OBJSDIR)/*.o $(CFLAGS) -g
$(OBJS):%.o:%.cpp
	$(CC) -c $(CFLAGS) $< -o $(OBJSDIR)/$@ -g
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJSDIR)/*.o
