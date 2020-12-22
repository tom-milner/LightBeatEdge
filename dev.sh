
echo "Flashing to board..."
pio run -t upload

echo "Starting Serial Monitor..."
pio device monitor