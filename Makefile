########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I "C:/Program Files/boost"
LDFLAGS = -L "C:/Program Files/boost/stage/lib" -lboost_system-mgw14-mt-x64-1_87 -lws2_32 -liphlpapi

APPNAME = caching-proxy
SRCDIR = src
OBJDIR = obj
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: $(OBJDIR) $(APPNAME)

$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(APPNAME).exe del /q $(APPNAME).exe
	@if exist *.d del /q *.d
else
	rm -rf $(OBJDIR) $(APPNAME) *.d
endif

# Thorough cleanup, including logs and temp files
clear:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(APPNAME).exe del /q $(APPNAME).exe
	@if exist *.d del /q *.d
	@if exist *.log del /q *.log
	@if exist *.tmp del /q *.tmp
else
	rm -rf $(OBJDIR) $(APPNAME) *.d *.log *.tmp
endif