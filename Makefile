# Nombre del ejecutable final
TARGET = libcid3
PATHLIB = /usr/local/lib
PATHINC = /usr/local/include/cid3

# Directorios
SRC_DIR = src
BUILD_DIR = build
INC_DIRS = -Iinclude

# Archivos fuente y objetos
SRCS = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/utils/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o, $(SRCS))

# Regla principal: compilar todo
$(TARGET): $(OBJS)
	$(CC) -shared $(OBJS) -o $(BUILD_DIR)/$(TARGET).so

# Regla para compilar archivos .c a .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -fPIC $(INC_DIRS) -c $< -o $@

# Crear el directorio de compilación si no existe
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/utils

# Limpiar archivos compilados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

install:
	mkdir -p $(PATHLIB)
	mkdir -p $(PATHINC)
	cp $(BUILD_DIR)/$(TARGET).so $(PATHLIB)/libcid3.so
	cp -r include/* -d $(PATHINC)

uninstall:
	rm -rf $(PATHINC)
	rm -rf $(PATHLIB)/libcid3.so
