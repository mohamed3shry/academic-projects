import numpy as np
import time
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import KFold
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix
from logger import get_logger
from data_loader import load_data
import matplotlib.pyplot as plt
import seaborn as sns
import joblib



logger = get_logger(log_dir="results", log_file="one-all.log")
logger.info("Starting Linear Regression OvA with 5-Fold CV...")



def train_one_vs_all_cv(X_train, y_train, k=5):
    kf = KFold(n_splits=k, shuffle=True, random_state=42)
    accuracies, precisions, recalls, f1s = [], [], [], []
    
    start_time = time.time()
    
    for fold, (train_idx, val_idx) in enumerate(kf.split(X_train), 1):
        X_tr, X_val = X_train[train_idx], X_train[val_idx]
        y_tr, y_val = y_train[train_idx], y_train[val_idx]
        
   
        classes = np.unique(y_tr)
        models = []
        for cls in classes:
            y_binary = (y_tr == cls).astype(float)
            model = LinearRegression()
            model.fit(X_tr, y_binary)
            models.append(model)
        
 
        y_pred_scores = np.column_stack([m.predict(X_val) for m in models])
        y_pred = np.argmax(y_pred_scores, axis=1)
        
  
        acc = accuracy_score(y_val, y_pred)
        prec = precision_score(y_val, y_pred, average="macro")
        rec = recall_score(y_val, y_pred, average="macro")
        f1 = f1_score(y_val, y_pred, average="macro")
        
        logger.info(f"Fold {fold}: Accuracy={acc:.4f}, Precision={prec:.4f}, Recall={rec:.4f}, F1={f1:.4f}")
        accuracies.append(acc)
        precisions.append(prec)
        recalls.append(rec)
        f1s.append(f1)
    
    elapsed = time.time() - start_time
    logger.info(f"Training completed in {elapsed:.2f}s ({elapsed/60:.2f} minutes)")
    
    logger.info(f"CV Mean Accuracy: {np.mean(accuracies):.4f} ± {np.std(accuracies):.4f}")
    logger.info(f"CV Mean Precision: {np.mean(precisions):.4f} ± {np.std(precisions):.4f}")
    logger.info(f"CV Mean Recall: {np.mean(recalls):.4f} ± {np.std(recalls):.4f}")
    logger.info(f"CV Mean F1-score: {np.mean(f1s):.4f} ± {np.std(f1s):.4f}")
    
    return models  


X, y, X_test, y_test = load_data("data")
models = train_one_vs_all_cv(X, y, k=5)

joblib.dump(models, "results/one-all.pkl")


y_test_pred_scores = np.column_stack([m.predict(X_test) for m in models])
y_test_pred = np.argmax(y_test_pred_scores, axis=1)

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
plt.savefig("results/one-all_confusion.png")
plt.show()

