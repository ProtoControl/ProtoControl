import torch
import os
from PIL import Image

image_to_analyze = r"C:\Users\willr\Downloads\Test_Image.jpg"
def resize_image(image_path):
    with Image.open(image_path) as img:
        resized_img = img.resize((640, 640), Image.ANTIALIAS)
        resized_img.save(image_path)




def run_initial_scan(img_directory):     #Runs the initial scan of the image directory
    resize_image(image_to_analyze)
    command = f"""python yolov5\detect.py --source "{img_directory}" --weights best.pt --conf {CONFIDENCE_INTERVAL} --name {RUN_NAME} --save-txt --save-crop --save-conf"""
    print('Running: \n' + command + '\n')
    os.system(command)
def run_yolo_on_image(image_path):
    # Load the model architecture
    model = torch.hub.load('ultralytics/yolov5', 'yolov5s')  # Here 'yolov5s' represents the small version. Replace with 'yolov5m', 'yolov5l', or 'yolov5x' if you trained with a different size.

    # Load custom weights
    model.load_state_dict(torch.load(r"C:\Users\willr\Documents\ProtoControl\YOLO\best.pt"))

    # Inference on the given image
    results = model(image_path)

    # Display the original image
    original_image = Image.open(image_path)
    original_image.show(title="Original Image")

    # Display the image with bounding boxes
    results.show(title="Analyzed Image")

#run run run!!
#run_yolo_on_image(image_to_analyze)


if __name__ == "__main__":
    run_yolo_on_image(image_to_analyze)
