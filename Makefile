# Nombre del ejecutable final
TARGET = program

# Directorios
SRC_DIR = src
BUILD_DIR = build
INC_DIRS = -Iinclude

# Archivos fuente y objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o, $(SRCS))

# Regla principal: compilar todo
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Regla para compilar archivos .c a .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(INC_DIRS) -c $< -o $@

# Crear el directorio de compilación si no existe
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limpiar archivos compilados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
