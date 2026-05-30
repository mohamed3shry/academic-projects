import numpy as np
import time
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import KFold
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix
from logger import get_logger
from data_loader import load_data
import matplotlib.pyplot as plt
import seaborn as sns
import joblib
import os


logger = get_logger(log_dir="results", log_file="logistic.log")
logger.info("Starting multi-class Logistic Regression with 5-Fold CV (using last fold model)...")


X, y, X_test, y_test = load_data("data")


kf = KFold(n_splits=5, shuffle=True, random_state=42)
cv_accuracies, cv_precisions, cv_recalls, cv_f1s = [], [], [], []

start_time = time.time()

for fold, (train_idx, val_idx) in enumerate(kf.split(X), 1):
    X_tr, X_val = X[train_idx], X[val_idx]
    y_tr, y_val = y[train_idx], y[val_idx]

    model = LogisticRegression(
        multi_class='multinomial',
        solver='saga',
        max_iter=200
    )
    model.fit(X_tr, y_tr)

 
    y_val_pred = model.predict(X_val)

   
    acc = accuracy_score(y_val, y_val_pred)
    prec = precision_score(y_val, y_val_pred, average="macro")
    rec = recall_score(y_val, y_val_pred, average="macro")
    f1 = f1_score(y_val, y_val_pred, average="macro")

    logger.info(f"Fold {fold}: Accuracy={acc:.4f}, Precision={prec:.4f}, Recall={rec:.4f}, F1={f1:.4f}")
    cv_accuracies.append(acc)
    cv_precisions.append(prec)
    cv_recalls.append(rec)
    cv_f1s.append(f1)

elapsed = time.time() - start_time
logger.info(f"Training completed in {elapsed:.2f}s ({elapsed/60:.2f} minutes)")
logger.info(f"CV Mean Accuracy: {np.mean(cv_accuracies):.4f} ± {np.std(cv_accuracies):.4f}")
logger.info(f"CV Mean Precision: {np.mean(cv_precisions):.4f} ± {np.std(cv_precisions):.4f}")
logger.info(f"CV Mean Recall: {np.mean(cv_recalls):.4f} ± {np.std(cv_recalls):.4f}")
logger.info(f"CV Mean F1-score: {np.mean(cv_f1s):.4f} ± {np.std(cv_f1s):.4f}")



final_model = model  
joblib.dump(final_model, "results/logistic.pkl")
logger.info("Final model (last fold) saved to results/logistic_lastfold_model.pkl")

y_test_pred = final_model.predict(X_test)
acc = accuracy_score(y_test, y_test_pred)
prec = precision_score(y_test, y_test_pred, average="macro")
rec = recall_score(y_test, y_test_pred, average="macro")
f1 = f1_score(y_test, y_test_pred, average="macro")
cm = confusion_matrix(y_test, y_test_pred)

logger.info("===== Test Set Results =====")
logger.info(f"Test Accuracy: {acc:.4f}")
logger.info(f"Precision: {prec:.4f}")
logger.info(f"Recall: {rec:.4f}")
logger.info(f"F1-score: {f1:.4f}")


plt.figure(figsize=(8,6))
sns.heatmap(cm, annot=True, fmt="d", cmap="Blues")
plt.title("Confusion Matrix")
plt.xlabel("Predicted")
plt.ylabel("True")
plt.savefig("results/logistic_confusion.png")
plt.show()
