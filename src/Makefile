CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =

SRCDIR = src
BUILDDIR = build
TARGET = calc

SRCEXT = c
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET)
