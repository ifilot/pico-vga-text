import serial
import time
import requests

# Define the COM port and baud rate
COM_PORT = "COM5"  # Adjust as necessary
BAUD_RATE = 115200  # Standard baud rate

LOREM_IPSUM = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris nec porta nisl, hendrerit laoreet dolor. Sed nec urna id sapien iaculis luctus eget nec metus. Integer pharetra ipsum ullamcorper, pretium turpis in, eleifend nibh. Mauris commodo magna vitae lectus dignissim, in sagittis lorem varius. Nunc leo leo, volutpat vitae bibendum sit amet, tincidunt at lacus. Vestibulum id sem a nisl mattis finibus. Sed vitae mi in sapien egestas faucibus a id sem. Quisque tempus ut lectus nec posuere.\n \
Fusce mollis libero nec urna vestibulum convallis. Vestibulum pulvinar vitae lorem vel feugiat. Donec imperdiet magna non ipsum blandit sagittis. Aenean dictum facilisis odio, sed iaculis tellus molestie ut. Aliquam eget consequat dolor. Nam convallis, tortor et congue consequat, nibh felis pharetra justo, eget sodales quam libero a nisi. Sed fringilla ac orci eget consectetur. Vestibulum pellentesque orci vel ex tincidunt, sed fermentum magna volutpat. Sed commodo diam at tortor elementum congue. Ut pulvinar pulvinar lobortis. Praesent fringilla risus ac neque vulputate, non congue nibh iaculis. Vestibulum commodo in quam in sodales. Integer nec quam tempor, porta ipsum ut, pretium lacus. Donec a elit non purus porttitor tempor quis non metus. Sed a dolor bibendum, volutpat lectus non, laoreet massa. Vivamus elementum elementum mi at tristique.\n \
Praesent rhoncus felis lorem, sit amet varius ligula malesuada at. Integer pretium orci vitae pellentesque placerat. Phasellus ac hendrerit augue. Nullam at diam sit amet dui condimentum pellentesque nec vitae diam. Proin ut facilisis justo, nec aliquam justo. Sed dictum maximus sapien, eu scelerisque nisi iaculis id. Maecenas scelerisque nisl eu turpis pellentesque, quis facilisis est interdum. Aliquam erat volutpat. Curabitur lorem ligula, tristique ac maximus vitae, lobortis vitae dolor.\n \
Vestibulum id pharetra nunc, eu cursus sapien. In scelerisque pharetra ultricies. Phasellus mattis placerat elit eget sodales. Aenean in velit elementum, fringilla velit eu, rutrum enim. Quisque eu ex varius nibh laoreet tempus. Nam sed dui nec velit varius pellentesque et eget augue. Curabitur ac arcu tellus. Aliquam leo ante, luctus at euismod quis, blandit quis elit. Suspendisse potenti. Mauris eget consequat leo, luctus tristique tellus. Sed lobortis scelerisque velit blandit commodo. Nunc urna lectus, bibendum nec dapibus sed, pulvinar sed ante. Phasellus ullamcorper dictum libero, in lobortis dolor efficitur at.\n \
Fusce vestibulum, ipsum pellentesque efficitur efficitur, nisi lorem tempor neque, sodales vestibulum massa justo ac dui. Ut volutpat magna sagittis est feugiat, viverra rutrum mi finibus. Vestibulum scelerisque vestibulum sapien sit amet finibus. Cras lacinia interdum orci, vel laoreet nibh posuere faucibus. Aliquam egestas ipsum quis luctus auctor. Nunc ullamcorper erat elit, non elementum justo laoreet luctus. Ut quis sapien nec augue eleifend placerat a eu mauris. Proin tincidunt suscipit sollicitudin. Curabitur eget quam euismod, euismod massa et, sagittis risus. Donec sit amet felis quis nulla finibus efficitur vel at orci.\n"

# Open the serial connection
with serial.Serial(COM_PORT, BAUD_RATE, timeout=1) as ser:
    print(f"Connected to {COM_PORT} at {BAUD_RATE} baud.")
    
    for char in LOREM_IPSUM:
        ser.write(char.encode())  # Send each character
        time.sleep(0.0001)  # 1ms delay per character
    
    print("Transmission complete.")