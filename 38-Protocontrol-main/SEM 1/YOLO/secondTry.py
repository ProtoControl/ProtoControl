import torch
from PIL import Image



image_to_analyze = r"C:\Users\willr\Downloads\LiveDemo.jpg"


def load_model(weights_path, img_size=640):
    # Load a model from Ultralytics YOLOv5 repository
    model = torch.hub.load('ultralytics/yolov5', 'custom', path=weights_path)
    model.conf = 0.25  # confidence threshold (0-1)
    model.iou = 0.45  # NMS IoU threshold (0-1)
    model.classes = None  # (optional list) filter by class, i.e. = [0, 15, 16] for persons, cats and dogs
    model.max_det = 1000  # maximum number of detections per image
    return model

def process_image(image_path, model):
    # Load image
    img = Image.open(image_path)

    # Perform inference
    results = model(img)

    # Results
    results.print()
    results.show()  # or .save()

    return results

def main():
    model = load_model(r"C:\Users\willr\Documents\ProtoControl\YOLO\best.pt")
    results = process_image(image_to_analyze, model)

if __name__ == "__main__":
    main()
