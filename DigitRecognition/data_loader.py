import os
import cv2
import numpy as np
import matplotlib.pyplot as plt

def load_data(root="data"):
    imgs = []
    labels = []
    imgs2 = []
    labels2 = []

    for label in range(10):
        c = 0
        folder = os.path.join(root, str(label))
        for filename in os.listdir(folder):
            path = os.path.join(folder, filename)

            img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)

            if img is None:
                print("Corrupted image:", path)
                continue

            if img.shape != (28, 28):
                img = cv2.resize(img, (28, 28))

            img = img.astype(np.float32)
            img = img.flatten() / 255.0
            c += 1

            if c > 800:
                imgs2.append(img)
                labels2.append(label)
            else:
                imgs.append(img)
                labels.append(label)

    X = np.stack(imgs)
    y = np.array(labels)

    X_test = np.stack(imgs2)
    y_test = np.array(labels2)

    return X, y, X_test, y_test


def show_image(flat_img, label):
    img = flat_img.reshape(28, 28)  
    plt.imshow(img, cmap="gray")
    plt.title(f"Label = {label}")
    plt.axis("off")
    plt.show()


if __name__ == "__main__":
    X, y, X_test, y_test = load_data(
    r"C:\Users\elwaled\Desktop\Handwritten Digit Recognition Using Machine Learning Techniques\data"
)


    print("Train shape:", X.shape, y.shape)
    print("Test shape:", X_test.shape, y_test.shape)
   
    for i in range(0, 2000, 200):
        print("Label:", y[i])
        show_image(X[i], y[i])
