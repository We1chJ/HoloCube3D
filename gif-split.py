from PIL import Image
import os

GIF_PATH = "image.gif"   # ← your gif filename
OUT_WIDTH = 240
OUT_HEIGHT = 240

gif = Image.open(GIF_PATH)

frame_num = 0
try:
    while True:
        frame = gif.convert("RGB").resize((OUT_WIDTH, OUT_HEIGHT))
        pixels = list(frame.getdata())

        rgb565 = []
        for r, g, b in pixels:
            color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            # swap bytes for Adafruit
            color = ((color & 0xFF) << 8) | ((color >> 8) & 0xFF)
            rgb565.append(color)

        with open(f"frame_{frame_num:03d}.h", "w") as f:
            f.write(f"const uint16_t frame_{frame_num:03d}[] PROGMEM = {{\n")
            for i, c in enumerate(rgb565):
                f.write(f"0x{c:04X},")
                if (i + 1) % 16 == 0:
                    f.write("\n")
            f.write("\n};\n")

        print(f"Wrote frame_{frame_num:03d}.h")
        frame_num += 1
        gif.seek(gif.tell() + 1)

except EOFError:
    print(f"\nDone! {frame_num} frames extracted.")