import argparse
import joblib
import torch
import numpy as np
import matplotlib.pyplot as plt
from data_loader import load_data
from modelnet import Net


def show_image(flat_img, true_label, pred_label):
    img = flat_img.reshape(28, 28)
    plt.imshow(img, cmap="gray")
    plt.title(f"True: {true_label} | Pred: {pred_label}")
    plt.axis("off")
    plt.show()


def main():
    parser = argparse.ArgumentParser(description="Predict using saved models")
    parser.add_argument("--model", type=str, required=True,
                        choices=["naive_bayes", "naive_bayes2",
                                 "logistic", "one-vs-all", "net"],
                        help="Select which model to use")
    args = parser.parse_args()

    X_train, y_train, X_test, y_test = load_data("data")

    if args.model == "net":
        model = Net()
        model.load_state_dict(torch.load("results/net.pth"))
        model.eval()
    else:
        path_dict = {
            "naive_bayes": "results/naive_bayes.pkl",
            "naive_bayes2": "results/naive_bayes2.pkl",
            "logistic": "results/logistic.pkl",
            "one-vs-all": "results/one-all.pkl"
        }
        model = joblib.load(path_dict[args.model])

    print(f"Testing model: {args.model}")

    for i in range(0, min(2000, len(X_test)), 200):
        x = X_test[i]
        y_true = y_test[i]

        if args.model == "net":
            input_tensor = torch.tensor(x, dtype=torch.float32).unsqueeze(0)
            with torch.no_grad():
                output = model(input_tensor)
                y_pred = torch.argmax(output, dim=1).item()
            print(f"True label: {y_true}, Predicted: {y_pred}")
            show_image(x, y_true, y_pred)

        elif args.model == "one-vs-all":
            scores = np.column_stack([m.predict(X_test) for m in model])
            y_pred_full = np.argmax(scores, axis=1)
            y_pred = y_pred_full[i]
            print(f"True label: {y_true}, Predicted: {y_pred}")
            show_image(x, y_true, y_pred)

        else:
            y_pred = model.predict([x])[0]
            print(f"True label: {y_true}, Predicted: {y_pred}")
            show_image(x, y_true, y_pred)


if __name__ == "__main__":
    main()
