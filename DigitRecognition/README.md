# Handwritten Digit Recognition Using Machine Learning and Deep Learning

## Project Overview

This project implements and compares multiple Machine Learning and Deep Learning approaches for handwritten digit recognition. The goal is to classify grayscale handwritten digit images into one of ten classes (0–9).

The project includes traditional machine learning algorithms, custom One-vs-All classification, and neural networks implemented using PyTorch.

---

## Dataset

### Dataset Structure

The dataset is stored in the `data/` directory and contains:

* 10 folders representing digits `0` through `9`
* 1000 images per digit
* Total: 10,000 grayscale images

### Data Split

* Training Set: 8,000 images (800 per class)
* Test Set: 2,000 images (200 per class)

### Preprocessing

Each image is:

1. Loaded in grayscale
2. Resized to 28×28 pixels
3. Converted to `float32`
4. Normalized to the range `[0,1]`
5. Flattened into a 784-dimensional feature vector

---

## Implemented Models

### 1. Logistic Regression

* Multinomial Logistic Regression
* Solver: SAGA
* 5-Fold Cross Validation
* Softmax-based multi-class classification

**File:** `logisticregression.py`

---

### 2. Gaussian Naive Bayes

* Assumes Gaussian distribution of pixel values
* Fast probabilistic classifier

**File:** `naive_bayes.py`

---

### 3. Multinomial Naive Bayes

* Uses Multinomial distribution assumptions
* Included for performance comparison

**File:** `naive_bayes2.py`

---

### 4. One-vs-All Linear Regression

* Trains 10 binary classifiers
* One classifier for each digit
* Final prediction selected using maximum score

**File:** `one-all.py`

---

### 5. Neural Network (Net)

Architecture:

```text
784 → 512 → 256 → 128 → 64 → 10
```

Features:

* ReLU activations
* Adam optimizer
* CrossEntropyLoss
* Learning rate scheduling

**Files:**

* `net.py`
* `modelnet.py`

---

### 6. Neural Network Variant (Net2)

Alternative neural network architecture used for comparison.

**File:** `net2.py`

---

## Project Structure

```text
DigitRecognition/
│
├── data/
│   ├── 0/
│   ├── 1/
│   ├── ...
│   └── 9/
│
├── results/
│
├── data_loader.py
├── logisticregression.py
├── naive_bayes.py
├── naive_bayes2.py
├── one-all.py
├── net.py
├── net2.py
├── modelnet.py
├── predict.py
├── logger.py
│
└── README.md
```

---

## Features

* Automatic image preprocessing
* Multiple machine learning algorithms
* Deep learning using PyTorch
* Model persistence and loading
* Cross-validation
* Confusion matrix visualization
* Training logs
* Prediction interface

---

## Evaluation Metrics

The models are evaluated using:

* Accuracy
* Precision
* Recall
* F1-Score
* Confusion Matrix

Performance is measured on a separate test set after training.

---
 

## Training Models

### Logistic Regression

```bash
python logisticregression.py
```

### Gaussian Naive Bayes

```bash
python naive_bayes.py
```

### Multinomial Naive Bayes

```bash
python naive_bayes2.py
```

### One-vs-All Linear Regression

```bash
python one-all.py
```

### Neural Network

```bash
python net.py
```

### Neural Network Variant

```bash
python net2.py
```

---

## Making Predictions

Examples:

```bash
python predict.py --model logistic
```

```bash
python predict.py --model naive_bayes
```

```bash
python predict.py --model net
```

```bash
python predict.py --model one-vs-all
```

---

## Generated Outputs

The `results/` directory contains:

* Trained models (`.pkl`, `.pth`)
* Training logs (`.log`)
* Confusion matrices (`.png`)
* Training curves (`.png`)

---

## Technologies Used

* Python
* NumPy
* OpenCV
* Scikit-Learn
* PyTorch
* Matplotlib
* Seaborn
* Joblib

---

## Future Improvements

* Convolutional Neural Networks (CNNs)
* Data Augmentation
* Ensemble Learning
* Hyperparameter Optimization
* Batch Normalization and Dropout
* Additional Classification Models

---

## Team Members

* Mohamed Ali
* Mohamed Abdelrhman
* Mohamed Elsayad
* Mohamed Ashry

 