import torch
import torch.nn as nn
from torch.utils.data import DataLoader, TensorDataset
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix
import matplotlib.pyplot as plt
from data_loader import load_data
from logger import get_logger
import seaborn as sns
import numpy as np
import time
import os
from modelnet import Net


logger = get_logger(log_dir="results", log_file="net.log")
logger.info("Starting training...")


device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
logger.info(f"Using device: {device}")


X, y, X_test, y_test = load_data("data")
X = torch.tensor(X, dtype=torch.float32).to(device)
y = torch.tensor(y, dtype=torch.long).to(device)
X_test = torch.tensor(X_test, dtype=torch.float32).to(device)
y_test = torch.tensor(y_test, dtype=torch.long).to(device)

train_dataset = TensorDataset(X, y)
train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)

test_dataset = TensorDataset(X_test, y_test)
test_loader = DataLoader(test_dataset, batch_size=64, shuffle=False)




model = Net().to(device)
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
scheduler = torch.optim.lr_scheduler.StepLR(optimizer, step_size=30, gamma=0.5)


num_epochs = 100
train_losses = []

start_time = time.time()  

for epoch in range(num_epochs):
    model.train()
    running_loss = 0
    for xb, yb in train_loader:
        optimizer.zero_grad()
        outputs = model(xb)
        loss = criterion(outputs, yb)
        loss.backward()
        optimizer.step()
        running_loss += loss.item()
    scheduler.step()
    epoch_loss = running_loss / len(train_loader)
    train_losses.append(epoch_loss)
    logger.info(f"Epoch {epoch+1}/{num_epochs}, Loss: {epoch_loss:.4f}, LR: {optimizer.param_groups[0]['lr']:.6f}")

end_time = time.time()  
elapsed = end_time - start_time
logger.info(f"Training completed in {elapsed:.2f}s ({elapsed/60:.2f} minutes)")


os.makedirs("results", exist_ok=True)
torch.save(model.state_dict(), "results/net.pth")
logger.info("Model saved to results/net.pth")


model.eval()
all_preds = []
all_labels = []

with torch.no_grad():
    for xb, yb in test_loader:
        outputs = model(xb)
        _, preds = torch.max(outputs, 1)
        all_preds.extend(preds.cpu().numpy())
        all_labels.extend(yb.cpu().numpy())

acc = accuracy_score(all_labels, all_preds)
prec = precision_score(all_labels, all_preds, average="macro")
rec = recall_score(all_labels, all_preds, average="macro")
f1 = f1_score(all_labels, all_preds, average="macro")
cm = confusion_matrix(all_labels, all_preds)

logger.info(f"Test Accuracy: {acc:.4f}")
logger.info(f"Precision: {prec:.4f}")
logger.info(f"Recall: {rec:.4f}")
logger.info(f"F1-score: {f1:.4f}")


plt.figure(figsize=(6,4))
plt.plot(range(1, num_epochs+1), train_losses, marker='o')
plt.title("Training Loss Curve")
plt.xlabel("Epoch")
plt.ylabel("Loss")
plt.grid(True)
plt.savefig("results/net_curve.png")
plt.show()


plt.figure(figsize=(8,6))
sns.heatmap(cm, annot=True, fmt="d", cmap="Blues")
plt.title("Confusion Matrix")
plt.xlabel("Predicted")
plt.ylabel("True")
plt.savefig("results/net_confusion.png")
plt.show()
