#
#   The following script is used to interpret the output from the training alrigthm from a text file
#   and to create charts and graphs to visualize it
#
import matplotlib.pyplot as plt
data = ""
with open("trainingdata.txt", "r", encoding="utf-8") as f:
    lines = f.readlines()

# Initialize lists to store parsed values
epochs = []
gpu_mems = []
box_losses = []
obj_losses = []
cls_losses = []
instances = []
sizes = []

Ps = []
Rs = []
mAP50s = []
mAP50_95s = []

# Iterate over the lines and parse values
for i in range(len(lines)):
    line = lines[i].strip()

    # Check for epoch line
    if line.startswith("Epoch"):
        # Skip the next line which contains the data header
        continue

    # Parse epoch details
    if "/249" in line:
        values = line.split()
        epochs.append(values[0].split("/")[0])
        gpu_mems.append(values[1])
        box_losses.append(values[2])
        obj_losses.append(values[3])
        cls_losses.append(values[4])
        instances.append(values[5])
        sizes.append(values[6].split(":")[0])

    # Parse evaluation metrics
    if line.startswith("all"):
        values = line.split()
        Ps.append(values[3])
        Rs.append(values[4])
        mAP50s.append(values[5])
        mAP50_95s.append(values[6])

# Print parsed data
for i in range(len(epochs)):
    print(f"Epoch: {epochs[i]}")
    print(f"GPU Memory: {gpu_mems[i]}")
    print(f"Box Loss: {box_losses[i]}")
    print(f"Obj Loss: {obj_losses[i]}")
    print(f"Cls Loss: {cls_losses[i]}")
    print(f"Instances: {instances[i]}")
    print(f"Size: {sizes[i]}")
    print(f"P: {Ps[i]}")
    print(f"R: {Rs[i]}")
    print(f"mAP50: {mAP50s[i]}")
    print(f"mAP50-95: {mAP50_95s[i]}")
    print("----------------------")

# Assuming the previously mentioned lists (epochs, gpu_mems, box_losses, etc.)

print(len(epochs))
print(len(box_losses))
print(len(obj_losses))
print(len(gpu_mems))
print(len(instances))
print(len(sizes))
print(len(Ps))
print(len(Rs))
print(len(mAP50s))
print(len(mAP50_95s))
# ... and so on for the rest of the lists ...
input("test")
# Convert epochs to integers for plotting
# Convert epochs to integers for plotting
epochs = [int(epoch) for epoch in epochs]

# Convert other metrics to floats for plotting
box_losses = [float(loss) for loss in box_losses]
obj_losses = [float(loss) for loss in obj_losses]
cls_losses = [float(loss) for loss in cls_losses]
Ps = [float(p) for p in Ps]
Rs = [float(r) for r in Rs]
mAP50s = [float(m) for m in mAP50s]
mAP50_95s = [float(m) for m in mAP50_95s]

# Plotting Losses
plt.figure(figsize=(10, 5))
plt.plot(epochs, box_losses, label='Box Loss', marker='o')
plt.plot(epochs, obj_losses, label='Obj Loss', marker='o')
plt.plot(epochs, cls_losses, label='Cls Loss', marker='o')
plt.title('Losses over Epochs')
plt.xlabel('Epochs')
plt.ylabel('Loss Value')
plt.legend()
plt.grid(True)
plt.show()

# Plotting Precision
plt.figure(figsize=(10, 5))
plt.plot(epochs, Ps[:-1], label='Precision', marker='o')
plt.title('Precision over Epochs')
plt.xlabel('Epochs')
plt.ylabel('Precision Value')
plt.legend()
plt.grid(True)
plt.show()

# Plotting Recall
plt.figure(figsize=(10, 5))
plt.plot(epochs, Rs[:-1], label='Recall', marker='o')
plt.title('Recall over Epochs')
plt.xlabel('Epochs')
plt.ylabel('Recall Value')
plt.legend()
plt.grid(True)
plt.show()

# Plotting mAP50
plt.figure(figsize=(10, 5))
plt.plot(epochs, mAP50s[:-1], label='mAP50', marker='o')
plt.title('mAP50 over Epochs')
plt.xlabel('Epochs')
plt.ylabel('mAP50 Value')
plt.legend()
plt.grid(True)
plt.show()

# Plotting mAP50-95
plt.figure(figsize=(10, 5))
plt.plot(epochs, mAP50_95s[:-1], label='mAP50-95', marker='o')
plt.title('mAP50-95 over Epochs')
plt.xlabel('Epochs')
plt.ylabel('mAP50-95 Value')
plt.legend()
plt.grid(True)
plt.show()