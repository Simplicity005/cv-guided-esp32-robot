import socket
import sys

# The ESP32's permanent SoftAP IP and Port
ESP32_IP = "192.168.4.1"
ESP32_PORT = 4210

# Initialize a standard low-overhead internet socket using UDP (SOCK_DGRAM)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print("Laptop UDP Controller Connected to Robot Network.")
print("--- Wheel Commands ---")
print("  w = Forward  |  s = Backward")
print("  a = Spin Left|  d = Spin Right")
print("  x = Stop")
print("--- Servo Commands --")
print("  u = Servo 180° |  i = Servo 90° (Center) |  j = Servo 0°")
print("----------------------")
print("  q = Quit Application\n")

# Combined list of all executable control characters
VALID_COMMANDS = ['w', 'a', 's', 'd', 'x', 'u', 'i', 'j']

try:
    while True:
        command = input("Directive: ").strip().lower()
        
        if command == 'q':
            # Send an explicit stop command to park the wheels safely before exiting
            sock.sendto(b'x', (ESP32_IP, ESP32_PORT))
            print("Exiting controller interface.")
            break
            
        if command in VALID_COMMANDS:
            # Convert text string to raw binary bytes (e.g., 'u' -> b'u')
            byte_data = command.encode()
            
            # Broadcast the byte straight into the air targeting the ESP32
            sock.sendto(byte_data, (ESP32_IP, ESP32_PORT))
        else:
            print(f"Invalid key '{command}'! Use w,a,s,d,x for wheels or u,i,j for servo.")

except KeyboardInterrupt:
    # Handle Ctrl+C gracefully by halting the robot safely
    sock.sendto(b'x', (ESP32_IP, ESP32_PORT))
    sys.exit()