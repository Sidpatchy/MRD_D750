import os
import sys
from PIL import Image

PALETTE = [
    (0, 0, 0),  # Black
    (255, 255, 255),  # White
    (255, 0, 0)  # Red
]

def quantize_image_to_palette(img, palette):
    """Quantize an image to a specific palette using Floyd-Steinberg dithering."""
    img = img.convert("RGB")
    pixels = img.load()
    width, height = img.size

    for y in range(height):
        for x in range(width):
            old_pixel = img.getpixel((x, y))
            new_pixel = closest_colour(old_pixel, palette)
            pixels[x, y] = new_pixel

            quant_error = tuple(oe - ne for oe, ne in zip(old_pixel, new_pixel))
            distribute_error(img, x + 1, y, quant_error, 7 / 16)
            distribute_error(img, x - 1, y + 1, quant_error, 3 / 16)
            distribute_error(img, x, y + 1, quant_error, 5 / 16)
            distribute_error(img, x + 1, y + 1, quant_error, 1 / 16)

    return img

def distribute_error(img, x, y, error, coefficient):
    """Distribute quantization error to a specific pixel."""
    if 0 <= x < img.width and 0 <= y < img.height:
        pixel = img.getpixel((x, y))
        modified_pixel = tuple(min(max(int(pixel[i] + error[i] * coefficient), 0), 255) for i in range(3))
        img.putpixel((x, y), modified_pixel)

def closest_colour(rgb, palette, for_red=False):
    """Get the closest color from the palette to a given RGB value."""
    if for_red:
        return min([(255, 0, 0)], key=lambda colour: sum(abs(c - rc) for c, rc in zip(colour, rgb)))
    else:
        return min(palette, key=lambda colour: sum(abs(c - rc) for c, rc in zip(colour, rgb)))

def convert_image_to_arrays(img, palette):
    """Convert the image to two byte array formats using the palette."""
    pixels = list(img.getdata())
    colour_to_byte = {colour: index for index, colour in enumerate(palette)}

    bw_byte_array = []
    red_byte_array = []

    for i in range(0, len(pixels), 8):
        bw_byte, red_byte = 0, 0
        for j in range(8):
            pixel_index = i + j
            if pixel_index < len(pixels):
                # Black/White layer
                color = closest_colour(pixels[pixel_index], palette, for_red=False)
                bit = colour_to_byte[color]
                bw_byte |= bit << (7 - j)

                # Red layer
                color = closest_colour(pixels[pixel_index], palette, for_red=True)
                bit = colour_to_byte[color] if color in colour_to_byte else 0
                red_byte |= bit << (7 - j)

        bw_byte_array.append(bw_byte)
        red_byte_array.append(red_byte)

    return bw_byte_array, red_byte_array

def generate_c_file(image_path, output_dir):
    """Generate C and H files from an image."""
    img = Image.open(image_path).resize((800, 480))
    img = quantize_image_to_palette(img, PALETTE)
    bw_byte_array, red_byte_array = convert_image_to_arrays(img, PALETTE)

    base_filename = os.path.splitext(os.path.basename(image_path))[0]
    c_filename, h_filename = base_filename + ".cpp", base_filename + ".h"

    with open(os.path.join(output_dir, c_filename), 'w') as f:
        f.write(f"const unsigned char {base_filename}_b[48000] = {{ /* 0X00,0X01,0X20,0X03,0XE0,0X01, */\n")
        for i, byte in enumerate(bw_byte_array):
            if i and i % 16 == 0:
                f.write(",\n")
            else:
                f.write(",")
            f.write(f"0x{byte:02X}")
        f.write("\n};\n\n")

        f.write(f"const unsigned char {base_filename}_ry[48000] = {{ /* 0X00,0X01,0X20,0X03,0XE0,0X01, */\n")
        for i, byte in enumerate(red_byte_array):
            if i and i % 16 == 0:
                f.write(",\n")
            else:
                f.write(",")
            f.write(f"0x{byte:02X}")
        f.write("\n};\n")

    with open(os.path.join(output_dir, h_filename), 'w') as f:
        f.write(f"#ifndef _{base_filename.upper()}_H_\n")
        f.write(f"#define _{base_filename.upper()}_H_\n\n")
        f.write(f"extern const unsigned char {base_filename}_b[48000];\n")
        f.write(f"extern const unsigned char {base_filename}_ry[48000];\n\n")
        f.write("#endif\n")

    print(f"Generated: {os.path.join(output_dir, c_filename)}")
    print(f"Generated: {os.path.join(output_dir, h_filename)}")

def main(input_dir, output_dir):
    """Main processing function."""
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    for filename in os.listdir(input_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg')):
            generate_c_file(os.path.join(input_dir, filename), output_dir)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script_name.py <input_directory> <output_directory>")
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])