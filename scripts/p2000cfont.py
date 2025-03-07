import numpy as np
from PIL import Image

img = Image.open("P2000C_font_mini.png")

# Convert the image to grayscale (black & white processing)
img_gray = img.convert("L")

# Convert to numpy array for processing
img_array = np.array(img_gray)

# Define character grid
num_chars = 128  # Extract 128 characters
box_w, box_h = 12, 12
char_w, char_h = 8, 12  # Each character is 8x8 pixels

# Create an array to store the extracted characters (128 x 8 bytes)
char_data = np.zeros((num_chars, char_h), dtype=np.uint8)

# Extract characters
for i in range(num_chars):
    col = i % 16  # Column in 16x8 grid
    row = i // 16  # Row in 16x8 grid
    
    # Get character region
    char_x = col * box_w
    char_y = row * box_h
    
    char_img = img_array[char_y:char_y + char_h, char_x:char_x + char_w]
    
    # Convert each row to a byte (bit-packing pixels)
    for y in range(char_h):
        byte_val = 0
        for x in range(char_w):
            if char_img[y, x] > 128:  # Assume thresholding at 128
                byte_val |= (1 << x)  # Set bit if pixel is dark
        char_data[i, y] = byte_val

# Save extracted character data as binary file
char_data.tofile("p2000c_font_128x12.bin")

def generate_c_font_file(binary_file, output_c_file):
    # Load binary font data
    font_data = np.fromfile(binary_file, dtype=np.uint8).reshape((128, 12))

    # Open the C file for writing
    with open(output_c_file, "w") as f:
        f.write("#ifndef _FONT_H\n")
        f.write("#define _FONT_H\n\n")

        f.write("static const unsigned char font[] = {\n")

        # Write each character's byte data
        for i, char in enumerate(font_data):
            f.write("    " + ", ".join(f"0x{byte:02X}" for byte in char) + ", // Character %d\n" % i)

        f.write("};\n\n")
        f.write("#endif\n")

# Example usage:
binary_file = "p2000c_font_128x12.bin"
output_c_file = "../src/font.c"
generate_c_font_file(binary_file, output_c_file)