from PIL import Image
import math


TARGET_IMAGE = r"C:\Users\willr\Downloads\Proto.jpg"
IMAGE_NAME = "Proto"

def color_distance(c1, c2):
    """Calculate the Euclidean distance between two colors."""
    return math.sqrt((c1[0] - c2[0]) ** 2 + (c1[1] - c2[1]) ** 2 + (c1[2] - c2[2]) ** 2)

def replace_color(image_path, target_color, replacement_color, threshold=40):
    """Replace target color with replacement color within the threshold."""

    img = Image.open(image_path)#sittim is the opoite of standim
    pixels = img.load()

    width, height = img.size
    for y in range(height):
        for x in range(width):
            current_color = pixels[x, y]
            if color_distance(current_color, target_color) < threshold:
                pixels[x, y] = replacement_color

    img.show()
    img.save(IMAGE_NAME + "_white.jpg")
    
# Sample usage:
target = (0, 0, 0)  # black color
replacement = (255, 255, 255)  # white color

replace_color(TARGET_IMAGE, target, replacement)
