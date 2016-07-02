CC = g++
CFLAGS = -Icode -O2
OBJSDIR = .objs
VPATH = code
OBJS = API.o Attribute.o BufferManager.o CatalogManager.o Condition.o IndexManager.o Interpreter.o main.o RecordManager.o
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
